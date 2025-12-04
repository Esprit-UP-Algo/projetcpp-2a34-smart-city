#include "notificationmanager.h"
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QApplication>
#include <QMainWindow>
#include <QStyle>
#include <QWidget>
#include <QTcpSocket>
#include <QSslSocket>
#include <QEventLoop>
#include <QRegularExpression>
#include <QSettings>
#include <QTimer>

// Intelligent notification system implemented using QSystemTrayIcon,
// real-time alert monitoring using QTimer, and Oracle SQL queries via QSqlQuery.
// Ensures automatic detection of new alerts and immediate user notification.

NotificationManager::NotificationManager(QObject *parent)
    : QObject(parent)
    , m_trayIcon(nullptr)
    , m_trayMenu(nullptr)
    , m_showAction(nullptr)
    , m_quitAction(nullptr)
    , m_checkTimer(nullptr)
    , m_checkInterval(5)
    , m_lastAlertID(0)
    , m_networkManager(nullptr)
    , m_smtpServer("smtp.gmail.com")
    , m_smtpPort(587)
    , m_smtpUser("")
    , m_smtpPassword("")
    , m_useSSL(true)
    , m_recipientEmail("")
    , m_emailEnabled(false)
{
    // Initialize database connection (use existing connection)
    m_database = QSqlDatabase::database();
    
    // Initialize network manager for email
    m_networkManager = new QNetworkAccessManager(this);
    
    // Load email settings from QSettings
    QSettings settings;
    m_smtpServer = settings.value("email/smtpServer", "smtp.gmail.com").toString();
    m_smtpPort = settings.value("email/smtpPort", 587).toInt();
    m_smtpUser = settings.value("email/smtpUser", "").toString();
    m_smtpPassword = settings.value("email/smtpPassword", "").toString();
    m_useSSL = settings.value("email/useSSL", true).toBool();
    m_recipientEmail = settings.value("email/recipient", "").toString();
    m_emailEnabled = settings.value("email/enabled", false).toBool();
    
    // Get the last alert ID from database to start monitoring from there
    QSqlQuery query;
    query.prepare("SELECT MAX(ID_AL) FROM ALERTE");
    if (query.exec() && query.next()) {
        m_lastAlertID = query.value(0).toInt();
        qDebug() << "📊 NotificationManager: Starting from alert ID:" << m_lastAlertID;
    } else {
        qDebug() << "⚠️ NotificationManager: Could not get last alert ID, starting from 0";
        m_lastAlertID = 0;
    }
}

NotificationManager::~NotificationManager()
{
    stopMonitoring();
    
    if (m_trayIcon) {
        m_trayIcon->hide();
        delete m_trayIcon;
    }
    
    if (m_trayMenu) {
        delete m_trayMenu;
    }
    
    if (m_checkTimer) {
        delete m_checkTimer;
    }
}

void NotificationManager::initialize()
{
    // Check if system tray is available
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qDebug() << "⚠️ System tray is not available on this system";
        return;
    }
    
    // Setup system tray icon
    setupTrayIcon();
    setupTrayMenu();
    
    // Check database connection
    checkDatabaseConnection();
    
    qDebug() << "✅ NotificationManager initialized successfully";
}

void NotificationManager::setupTrayIcon()
{
    if (m_trayIcon) {
        return; // Already initialized
    }
    
    m_trayIcon = new QSystemTrayIcon(this);
    
    // Set icon - use a more visible icon (bell or information icon)
    QIcon icon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);
    if (icon.isNull()) {
        // Fallback to computer icon if information icon not available
        icon = QApplication::style()->standardIcon(QStyle::SP_ComputerIcon);
    }
    m_trayIcon->setIcon(icon);
    m_trayIcon->setToolTip("🔔 Gestion des Alertes - Système de Notification Intelligent\nSurveillance active - Vérification toutes les 5 secondes");
    
    // Connect activation signal (double-click to show window)
    connect(m_trayIcon, &QSystemTrayIcon::activated, 
            this, &NotificationManager::onTrayIconActivated);
    
    // Show the tray icon
    if (!m_trayIcon->isVisible()) {
        m_trayIcon->show();
    }
    
    // Show a welcome notification to confirm the system is active
    m_trayIcon->showMessage(
        "Système de Notification Actif",
        "Le système de notification intelligent surveille maintenant la table ALERTE.\n"
        "Vous recevrez des notifications pour chaque nouvelle alerte détectée.",
        QSystemTrayIcon::Information,
        3000 // Show for 3 seconds
    );
    
    qDebug() << "✅ System tray icon created and shown";
    qDebug() << "   Icon visible:" << m_trayIcon->isVisible();
    qDebug() << "   Tooltip:" << m_trayIcon->toolTip();
}

void NotificationManager::setupTrayMenu()
{
    if (m_trayMenu) {
        return; // Already initialized
    }
    
    m_trayMenu = new QMenu();
    
    // Action: Afficher l'application
    m_showAction = m_trayMenu->addAction("Afficher l'application");
    connect(m_showAction, &QAction::triggered, this, &NotificationManager::showApplication);
    
    // Separator
    m_trayMenu->addSeparator();
    
    // Action: Quitter l'application
    m_quitAction = m_trayMenu->addAction("Quitter l'application");
    connect(m_quitAction, &QAction::triggered, this, &NotificationManager::quitApplication);
    
    // Set menu to tray icon
    if (m_trayIcon) {
        m_trayIcon->setContextMenu(m_trayMenu);
    }
    
    qDebug() << "✅ System tray menu created";
}

void NotificationManager::checkDatabaseConnection()
{
    if (!m_database.isOpen()) {
        qDebug() << "⚠️ Database is not open. Attempting to use default connection...";
        m_database = QSqlDatabase::database();
        
        if (!m_database.isOpen()) {
            qDebug() << "❌ Cannot connect to database. Notifications will not work.";
            return;
        }
    }
    
    qDebug() << "✅ Database connection verified";
}

void NotificationManager::startMonitoring(int intervalSeconds)
{
    m_checkInterval = intervalSeconds;
    
    if (!m_checkTimer) {
        m_checkTimer = new QTimer(this);
        connect(m_checkTimer, &QTimer::timeout, this, &NotificationManager::checkForNewAlerts);
    }
    
    // Start the timer
    m_checkTimer->start(m_checkInterval * 1000); // Convert seconds to milliseconds
    
    qDebug() << "✅ Alert monitoring started. Checking every" << m_checkInterval << "seconds";
    
    // Check immediately on start
    QTimer::singleShot(1000, this, &NotificationManager::checkForNewAlerts);
}

void NotificationManager::stopMonitoring()
{
    if (m_checkTimer && m_checkTimer->isActive()) {
        m_checkTimer->stop();
        qDebug() << "⏸️ Alert monitoring stopped";
    }
}

bool NotificationManager::isSystemTrayAvailable() const
{
    return QSystemTrayIcon::isSystemTrayAvailable();
}

void NotificationManager::showNotification(const QString &title, const QString &message, 
                                          QSystemTrayIcon::MessageIcon icon)
{
    if (!m_trayIcon) {
        qDebug() << "⚠️ System tray icon not initialized. Cannot show notification.";
        return;
    }
    
    m_trayIcon->showMessage(title, message, icon, 5000); // Show for 5 seconds
    
    // Add to history
    NotificationEntry entry;
    entry.dateTime = QDateTime::currentDateTime();
    entry.type = "System";
    entry.title = title;
    entry.message = message;
    entry.systemNotificationSent = true;
    entry.emailSent = false;
    
    m_notificationHistory.prepend(entry);
    
    // Limit history size
    if (m_notificationHistory.size() > MAX_HISTORY_ENTRIES) {
        m_notificationHistory.removeLast();
    }
    
    emit notificationSent(title, message);
}

void NotificationManager::checkForNewAlerts()
{
    // Check database connection
    if (!m_database.isOpen()) {
        checkDatabaseConnection();
        if (!m_database.isOpen()) {
            return; // Cannot proceed without database
        }
    }
    
    // Detect column names (Oracle may have different column names)
    QSqlQuery checkColumns;
    checkColumns.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'ALERTE' ORDER BY COLUMN_ID");
    
    QStringList existingColumns;
    if (checkColumns.exec()) {
        while (checkColumns.next()) {
            existingColumns << checkColumns.value(0).toString().toUpper();
        }
    }
    
    // Determine actual column names
    QString colId = existingColumns.contains("ID_AL") ? "ID_AL" : 
                    existingColumns.contains("ID") ? "ID" : 
                    existingColumns.contains("ID_ALERTE") ? "ID_ALERTE" : "ID_AL";
    QString colGravite = existingColumns.contains("GRAVITE") ? "GRAVITE" : "GRAVITE";
    QString colClient = existingColumns.contains("CLIENT") ? "CLIENT" : 
                       existingColumns.contains("ID_CLIENT") ? "ID_CLIENT" : "CLIENT";
    QString colDescription = existingColumns.contains("DESCRIPTION") ? "DESCRIPTION" : "DESCRIPTION";
    QString colType = existingColumns.contains("TYPE") ? "TYPE" : "TYPE";
    QString colStatut = existingColumns.contains("STATUT") ? "STATUT" : "STATUT";
    QString colDate = existingColumns.contains("DATE_ET_HEURE") ? "DATE_ET_HEURE" :
                      existingColumns.contains("DATE_ALERTE") ? "DATE_ALERTE" :
                      existingColumns.contains("DATEALERTE") ? "DATEALERTE" :
                      existingColumns.contains("DATE") ? "DATE" : "SYSDATE";
    QString colIdEmp = existingColumns.contains("ID_EMP") ? "ID_EMP" : 
                       existingColumns.contains("ID_EMPLOYE") ? "ID_EMPLOYE" : "ID_EMP";
    
    // Query for new alerts using Oracle-compatible syntax
    QSqlQuery query;
    
    // Build query string with detected column names
    QString queryString;
    if (colDate == "SYSDATE") {
        // Use SYSDATE if no date column exists
        queryString = QString(
            "SELECT %1, %2, %3, %4, %5, %6, SYSDATE as %7, %8 "
            "FROM ALERTE "
            "WHERE %1 > :lastID "
            "ORDER BY %1 ASC"
        ).arg(colId, colGravite, colClient, colDescription, colType, colStatut, colDate, colIdEmp);
    } else {
        queryString = QString(
            "SELECT %1, %2, %3, %4, %5, %6, %7, %8 "
            "FROM ALERTE "
            "WHERE %1 > :lastID "
            "ORDER BY %1 ASC"
        ).arg(colId, colGravite, colClient, colDescription, colType, colStatut, colDate, colIdEmp);
    }
    
    query.prepare(queryString);
    query.bindValue(":lastID", m_lastAlertID);
    
    if (!query.exec()) {
        qDebug() << "❌ Error executing alert query:" << query.lastError().text();
        qDebug() << "   Query:" << queryString;
        qDebug() << "   Last ID:" << m_lastAlertID;
        return;
    }
    
    int newAlertsCount = 0;
    
    // Process each new alert
    while (query.next()) {
        int idAl = query.value(0).toInt(); // ID_AL is first column
        QString gravite = query.value(1).toString(); // GRAVITE
        QString client = query.value(2).toString(); // CLIENT
        QString description = query.value(3).toString(); // DESCRIPTION
        QString type = query.value(4).toString(); // TYPE
        QString statut = query.value(5).toString(); // STATUT
        QDateTime dateEtHeure = query.value(6).toDateTime(); // DATE_ET_HEURE
        int idEmp = query.value(7).toInt(); // ID_EMP
        
        // Process the alert
        processNewAlert(idAl, gravite, client, description, type, statut, dateEtHeure, idEmp);
        
        // Update last processed ID
        if (idAl > m_lastAlertID) {
            m_lastAlertID = idAl;
        }
        
        newAlertsCount++;
    }
    
    if (newAlertsCount > 0) {
        qDebug() << "✅ Processed" << newAlertsCount << "new alert(s). Last ID:" << m_lastAlertID;
    }
}

void NotificationManager::processNewAlert(int idAl, const QString &gravite, const QString &client,
                                         const QString &description, const QString &type,
                                         const QString &statut, const QDateTime &dateEtHeure, int idEmp)
{
    qDebug() << "🔔 New alert detected - ID:" << idAl << "Gravité:" << gravite;
    
    // Determine notification type based on gravity
    QString title;
    QString message;
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information;
    
    if (gravite.toUpper() == "CRITIQUE" || gravite.toUpper() == "CRITICAL") {
        // Critical alert - show urgent notification
        title = "⚠️ ALERTE CRITIQUE ⚠️";
        message = formatCriticalNotification(client, type, description);
        icon = QSystemTrayIcon::Critical;
        
        // Emit signal for critical alert
        emit criticalAlertDetected(idAl, client, type, description);
    } else {
        // Normal alert
        title = "Nouvelle alerte détectée";
        message = formatNormalNotification(client, gravite, description);
        
        // Use Information icon for normal alerts, Warning for elevated severity
        if (gravite.toUpper() == "ÉLEVÉE" || gravite.toUpper() == "ELEVEE" || 
            gravite.toUpper() == "HIGH") {
            icon = QSystemTrayIcon::Warning;
        }
        
        // Emit signal for new alert
        emit newAlertDetected(idAl, gravite, client, description, type);
    }
    
    // Create history entry
    NotificationEntry entry;
    entry.dateTime = QDateTime::currentDateTime();
    entry.title = title;
    entry.message = message;
    entry.alertId = QString::number(idAl);
    entry.client = client;
    entry.gravite = gravite;
    entry.systemNotificationSent = false;
    entry.emailSent = false;
    
    // Show system tray notification
    if (m_trayIcon) {
        m_trayIcon->showMessage(title, message, icon, 5000);
        entry.systemNotificationSent = true;
    }
    
    // Send email if enabled and recipient is configured
    bool emailSuccess = false;
    if (m_emailEnabled && !m_recipientEmail.isEmpty()) {
        QString emailSubject = title;
        QString emailBody = QString(
            "Détails de l'alerte:\n\n"
            "ID: %1\n"
            "Client: %2\n"
            "Gravité: %3\n"
            "Type: %4\n"
            "Description: %5\n"
            "Statut: %6\n"
            "Date et Heure: %7\n"
        ).arg(idAl).arg(client).arg(gravite).arg(type).arg(description).arg(statut)
         .arg(dateEtHeure.toString("dd/MM/yyyy hh:mm:ss"));
        
        emailSuccess = sendEmail(m_recipientEmail, emailSubject, emailBody);
        entry.emailSent = emailSuccess;
        entry.emailRecipient = m_recipientEmail;
    }
    
    // Determine notification type
    if (entry.systemNotificationSent && entry.emailSent) {
        entry.type = "Both";
    } else if (entry.systemNotificationSent) {
        entry.type = "System";
    } else if (entry.emailSent) {
        entry.type = "Email";
    } else {
        entry.type = "None";
    }
    
    // Add to history
    m_notificationHistory.prepend(entry);
    
    // Limit history size
    if (m_notificationHistory.size() > MAX_HISTORY_ENTRIES) {
        m_notificationHistory.removeLast();
    }
    
    // Emit notification sent signal
    emit notificationSent(title, message);
}

QString NotificationManager::formatNormalNotification(const QString &client, const QString &gravite,
                                                     const QString &description)
{
    QString message;
    message += "Client: " + client + "\n";
    message += "Gravité: " + gravite + "\n";
    message += "Description: " + description;
    
    return message;
}

QString NotificationManager::formatCriticalNotification(const QString &client, const QString &type,
                                                       const QString &description)
{
    QString message;
    message += "Client: " + client + "\n";
    message += "Type: " + type + "\n";
    message += "Description: " + description;
    
    return message;
}

void NotificationManager::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
        case QSystemTrayIcon::DoubleClick:
        case QSystemTrayIcon::Trigger:
            showApplication();
            break;
        case QSystemTrayIcon::MiddleClick:
            // Optional: Show notification history or quick actions
            break;
        default:
            break;
    }
}

void NotificationManager::showApplication()
{
    // Find and show the main window
    QWidgetList widgets = QApplication::allWidgets();
    for (QWidget *widget : widgets) {
        QMainWindow *mainWindow = qobject_cast<QMainWindow*>(widget);
        if (mainWindow) {
            mainWindow->show();
            mainWindow->raise();
            mainWindow->activateWindow();
            qDebug() << "✅ Application window shown";
            return;
        }
    }
    
    qDebug() << "⚠️ Could not find main window to show";
}

void NotificationManager::quitApplication()
{
    qDebug() << "🛑 Quit requested from system tray";
    QApplication::quit();
}

void NotificationManager::setEmailSettings(const QString &smtpServer, int smtpPort,
                                          const QString &smtpUser, const QString &smtpPassword,
                                          bool useSSL)
{
    m_smtpServer = smtpServer;
    m_smtpPort = smtpPort;
    m_smtpUser = smtpUser;
    m_smtpPassword = smtpPassword;
    m_useSSL = useSSL;
    
    // Save to QSettings
    QSettings settings;
    settings.setValue("email/smtpServer", m_smtpServer);
    settings.setValue("email/smtpPort", m_smtpPort);
    settings.setValue("email/smtpUser", m_smtpUser);
    settings.setValue("email/smtpPassword", m_smtpPassword);
    settings.setValue("email/useSSL", m_useSSL);
    settings.setValue("email/recipient", m_recipientEmail);
    settings.setValue("email/enabled", m_emailEnabled);
    
    qDebug() << "✅ Email settings saved";
}

void NotificationManager::setRecipientEmail(const QString &email)
{
    m_recipientEmail = email;
    QSettings settings;
    settings.setValue("email/recipient", m_recipientEmail);
    qDebug() << "✅ Recipient email saved:" << email;
}

void NotificationManager::setEmailEnabled(bool enabled)
{
    m_emailEnabled = enabled;
    QSettings settings;
    settings.setValue("email/enabled", m_emailEnabled);
    qDebug() << "✅ Email enabled:" << enabled;
}

void NotificationManager::clearNotificationHistory()
{
    m_notificationHistory.clear();
    qDebug() << "✅ Notification history cleared";
}

bool NotificationManager::sendEmail(const QString &to, const QString &subject, const QString &body)
{
    if (!m_emailEnabled || m_smtpServer.isEmpty() || m_smtpUser.isEmpty()) {
        qDebug() << "⚠️ Email not configured or disabled";
        return false;
    }
    
    if (to.isEmpty()) {
        qDebug() << "⚠️ Recipient email address is empty";
        return false;
    }
    
    // Validate email format
    QRegularExpression emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    if (!emailRegex.match(to).hasMatch()) {
        qDebug() << "❌ Invalid email address format:" << to;
        return false;
    }
    
    qDebug() << "📧 Attempting to send email to:" << to;
    qDebug() << "   Subject:" << subject;
    qDebug() << "   SMTP Server:" << m_smtpServer << "Port:" << m_smtpPort;
    
    // Create socket (SSL or regular TCP)
    QAbstractSocket *socket = nullptr;
    if (m_useSSL) {
        QSslSocket *sslSocket = new QSslSocket(this);
        sslSocket->setPeerVerifyMode(QSslSocket::VerifyNone);
        socket = sslSocket;
    } else {
        socket = new QTcpSocket(this);
    }
    
    QEventLoop loop;
    bool success = false;
    QString errorMessage;
    
    // Connect socket signals
    connect(socket, &QAbstractSocket::connected, [&]() {
        qDebug() << "✅ Connected to SMTP server";
        
        if (m_useSSL) {
            QSslSocket *sslSocket = qobject_cast<QSslSocket*>(socket);
            if (sslSocket) {
                sslSocket->startClientEncryption();
                qDebug() << "🔒 Starting SSL encryption...";
            }
        }
    });
    
    connect(socket, &QAbstractSocket::readyRead, [&]() {
        QByteArray response = socket->readAll();
        QString responseStr = QString::fromUtf8(response).trimmed();
        qDebug() << "📥 SMTP Response:" << responseStr;
        
        // Check for errors
        if (responseStr.startsWith("5") || responseStr.startsWith("4")) {
            errorMessage = "SMTP Error: " + responseStr;
            qDebug() << "❌" << errorMessage;
            loop.quit();
            return;
        }
        
        // Handle SMTP conversation
        static int step = 0;
        if (responseStr.contains("220") || responseStr.contains("250") || responseStr.contains("334") || responseStr.contains("235")) {
            step++;
            
            if (step == 1) {
                // EHLO
                QString ehlo = QString("EHLO %1\r\n").arg("localhost");
                socket->write(ehlo.toUtf8());
                qDebug() << "📤 Sending EHLO...";
            } else if (step == 2 && m_useSSL) {
                // After EHLO, start TLS if needed
                if (responseStr.contains("250") && m_smtpPort == 587) {
                    socket->write("STARTTLS\r\n");
                    qDebug() << "📤 Requesting STARTTLS...";
                    step = 1; // Reset for next phase
                }
            } else if (step == 2 || (step == 3 && m_useSSL && m_smtpPort == 587)) {
                // AUTH LOGIN
                socket->write("AUTH LOGIN\r\n");
                qDebug() << "📤 Sending AUTH LOGIN...";
            } else if (step == 3 || (step == 4 && m_useSSL && m_smtpPort == 587)) {
                // Send username (base64)
                QByteArray username = m_smtpUser.toUtf8().toBase64();
                socket->write(username + "\r\n");
                qDebug() << "📤 Sending username...";
            } else if (step == 4 || (step == 5 && m_useSSL && m_smtpPort == 587)) {
                // Send password (base64)
                QByteArray password = m_smtpPassword.toUtf8().toBase64();
                socket->write(password + "\r\n");
                qDebug() << "📤 Sending password...";
            } else if (responseStr.contains("235")) {
                // Authentication successful
                qDebug() << "✅ Authentication successful";
                
                // MAIL FROM
                QString mailFrom = QString("MAIL FROM: <%1>\r\n").arg(m_smtpUser);
                socket->write(mailFrom.toUtf8());
                qDebug() << "📤 Sending MAIL FROM...";
            } else if (responseStr.contains("250") && step > 4) {
                // RCPT TO
                QString rcptTo = QString("RCPT TO: <%1>\r\n").arg(to);
                socket->write(rcptTo.toUtf8());
                qDebug() << "📤 Sending RCPT TO...";
            } else if (responseStr.contains("250") && step > 5) {
                // DATA
                socket->write("DATA\r\n");
                qDebug() << "📤 Sending DATA command...";
            } else if (responseStr.contains("354")) {
                // Send email content
                QString emailContent = QString(
                    "From: %1\r\n"
                    "To: %2\r\n"
                    "Subject: %3\r\n"
                    "MIME-Version: 1.0\r\n"
                    "Content-Type: text/plain; charset=UTF-8\r\n"
                    "\r\n"
                    "%4\r\n"
                    ".\r\n"
                ).arg(m_smtpUser, to, subject, body);
                
                socket->write(emailContent.toUtf8());
                qDebug() << "📤 Sending email content...";
            } else if (responseStr.contains("250") && step > 6) {
                // QUIT
                socket->write("QUIT\r\n");
                qDebug() << "📤 Sending QUIT...";
                success = true;
                loop.quit();
            }
        }
    });
    
    connect(socket, &QAbstractSocket::errorOccurred, [&](QAbstractSocket::SocketError error) {
        errorMessage = QString("Socket error: %1").arg(socket->errorString());
        qDebug() << "❌" << errorMessage;
        loop.quit();
    });
    
    // Connect to SMTP server
    socket->connectToHost(m_smtpServer, m_smtpPort);
    
    // Wait for connection and email sending
    if (!socket->waitForConnected(10000)) {
        errorMessage = "Failed to connect to SMTP server: " + socket->errorString();
        qDebug() << "❌" << errorMessage;
        socket->deleteLater();
        return false;
    }
    
    // Run event loop (with timeout)
    QTimer::singleShot(30000, &loop, &QEventLoop::quit); // 30 second timeout
    loop.exec();
    
    socket->deleteLater();
    
    if (success) {
        qDebug() << "✅ Email sent successfully to:" << to;
        return true;
    } else {
        qDebug() << "❌ Failed to send email:" << errorMessage;
        return false;
    }
}
