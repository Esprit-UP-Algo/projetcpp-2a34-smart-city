#include "notificationmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSslSocket>
#include <QSettings>
#include <QSqlDatabase>
#include <QTimer>
#include <QEventLoop>
#include <QAuthenticator>
#include <QByteArray>
#include <QTextStream>
#include <QRegularExpression>

NotificationManager::NotificationManager(QObject *parent)
    : QObject(parent), smtpPort(587), useSSL(true), emailEnabled(false), 
      smsEnabled(false), pushEnabled(true), nextRuleId(1), nextNotificationId(1)
{
    // ============================================
    // HARDCODED GMAIL SMTP SETTINGS
    // ============================================
    // TODO: Replace with your actual Gmail credentials
    smtpServer = "smtp.gmail.com";
    smtpPort = 587;
    useSSL = true;
    emailUsername = "aniskontra123@gmail.com";  // ← CHANGE THIS
    emailPassword = "jdoc amri gmzs rzqv";      // ← CHANGE THIS (Use Gmail App Password, not regular password)
    
    // ============================================
    // SMS FUNCTIONALITY REMOVED - Only Email SMTP is supported
    // ============================================
    smsApiKey = "";
    smsApiSecret = "";
    smsSenderNumber = "";
    smsEnabled = false;
    
    initializeDatabase();
    loadSettings();
    
    // Enable email if credentials are configured (not default placeholders)
    if (!emailUsername.isEmpty() && !emailPassword.isEmpty() && 
        emailUsername != "YOUR_EMAIL@gmail.com" && emailPassword != "YOUR_APP_PASSWORD") {
        emailEnabled = true;
        qDebug() << "Email notifications enabled (Gmail SMTP)";
    } else {
        qDebug() << "⚠️ Email credentials not configured. Please set emailUsername and emailPassword in notificationmanager.cpp";
    }
    
    // SMS functionality is disabled
    smsEnabled = false;
}

NotificationManager::~NotificationManager()
{
    saveSettings();
}

void NotificationManager::initializeDatabase()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Database not open in NotificationManager::initializeDatabase()";
        return;
    }
    
    QSqlQuery query;
    
    // Create notification history table
    if (!query.exec("CREATE TABLE IF NOT EXISTS NOTIFICATION_HISTORY ("
               "ID INTEGER PRIMARY KEY, "
               "TIMESTAMP TIMESTAMP, "
               "ALERT_TYPE VARCHAR(50), "
               "TITLE VARCHAR(200), "
               "MESSAGE TEXT, "
               "CHANNELS VARCHAR(50), "
               "SENT INTEGER, "
               "RECIPIENT VARCHAR(200))")) {
        qDebug() << "Error creating NOTIFICATION_HISTORY table:" << query.lastError().text();
    }
    
    // Create alert rules table
    if (!query.exec("CREATE TABLE IF NOT EXISTS ALERT_RULES ("
               "ID INTEGER PRIMARY KEY, "
               "TYPE VARCHAR(50), "
               "NAME VARCHAR(100), "
               "CONDITION_TEXT TEXT, "
               "MESSAGE TEXT, "
               "CHANNELS VARCHAR(50), "
               "ENABLED INTEGER, "
               "THRESHOLD REAL, "
               "PRIORITY INTEGER)")) {
        qDebug() << "Error creating ALERT_RULES table:" << query.lastError().text();
    }
    
    // Load rules from database
    if (query.exec("SELECT * FROM ALERT_RULES")) {
        while (query.next()) {
            AlertRule rule;
            rule.type = static_cast<AlertType>(query.value("TYPE").toString().toInt());
            rule.name = query.value("NAME").toString();
            rule.condition = query.value("CONDITION_TEXT").toString();
            rule.message = query.value("MESSAGE").toString();
            rule.channels = static_cast<NotificationTypes>(query.value("CHANNELS").toInt());
            rule.enabled = query.value("ENABLED").toBool();
            rule.threshold = query.value("THRESHOLD").toDouble();
            rule.priority = query.value("PRIORITY").toInt();
            alertRules.append(rule);
        }
    }
}

void NotificationManager::setRecipientEmail(const QString &email)
{
    if (!email.isEmpty()) {
        recipients[email] = recipients.value(email, ""); // Keep existing phone if any
        emailEnabled = true;
    }
}

// SMS functionality removed - setRecipientPhone no longer needed

void NotificationManager::setEmailSettings(const QString &smtpServer, int port, 
                                           const QString &username, const QString &password, bool useSSL)
{
    this->smtpServer = smtpServer;
    this->smtpPort = port;
    this->emailUsername = username;
    this->emailPassword = password;
    this->useSSL = useSSL;
    this->emailEnabled = true;
}

// SMS functionality removed - setSMSSettings function removed

void NotificationManager::addRecipient(const QString &email, const QString &phone)
{
    recipients[email] = phone;
}

void NotificationManager::addAlertRule(const AlertRule &rule)
{
    AlertRule newRule = rule;
    int ruleId = nextRuleId++;
    alertRules.append(newRule);
    
    // Save to database
    QSqlQuery query;
    query.prepare("INSERT INTO ALERT_RULES (ID, TYPE, NAME, CONDITION_TEXT, MESSAGE, CHANNELS, ENABLED, THRESHOLD, PRIORITY) "
                  "VALUES (:id, :type, :name, :condition, :message, :channels, :enabled, :threshold, :priority)");
    query.bindValue(":id", ruleId);
    query.bindValue(":type", static_cast<int>(newRule.type));
    query.bindValue(":name", newRule.name);
    query.bindValue(":condition", newRule.condition);
    query.bindValue(":message", newRule.message);
    query.bindValue(":channels", static_cast<int>(newRule.channels));
    query.bindValue(":enabled", newRule.enabled ? 1 : 0);
    query.bindValue(":threshold", newRule.threshold);
    query.bindValue(":priority", newRule.priority);
    query.exec();
}

void NotificationManager::removeAlertRule(int ruleId)
{
    for (int i = 0; i < alertRules.size(); ++i) {
        // Note: In a real implementation, you'd need to track rule IDs properly
        alertRules.removeAt(i);
        break;
    }
    
    QSqlQuery query;
    query.prepare("DELETE FROM ALERT_RULES WHERE ID = :id");
    query.bindValue(":id", ruleId);
    query.exec();
}

void NotificationManager::updateAlertRule(int ruleId, const AlertRule &rule)
{
    QSqlQuery query;
    query.prepare("UPDATE ALERT_RULES SET TYPE = :type, NAME = :name, CONDITION_TEXT = :condition, "
                  "MESSAGE = :message, CHANNELS = :channels, ENABLED = :enabled, "
                  "THRESHOLD = :threshold, PRIORITY = :priority WHERE ID = :id");
    query.bindValue(":id", ruleId);
    query.bindValue(":type", static_cast<int>(rule.type));
    query.bindValue(":name", rule.name);
    query.bindValue(":condition", rule.condition);
    query.bindValue(":message", rule.message);
    query.bindValue(":channels", static_cast<int>(rule.channels));
    query.bindValue(":enabled", rule.enabled ? 1 : 0);
    query.bindValue(":threshold", rule.threshold);
    query.bindValue(":priority", rule.priority);
    query.exec();
}

NotificationManager::AlertRule NotificationManager::getAlertRule(int ruleId) const
{
    QSqlQuery query;
    query.prepare("SELECT * FROM ALERT_RULES WHERE ID = :id");
    query.bindValue(":id", ruleId);
    query.exec();
    
    if (query.next()) {
        AlertRule rule;
        rule.type = static_cast<AlertType>(query.value("TYPE").toInt());
        rule.name = query.value("NAME").toString();
        rule.condition = query.value("CONDITION_TEXT").toString();
        rule.message = query.value("MESSAGE").toString();
        rule.channels = static_cast<NotificationTypes>(query.value("CHANNELS").toInt());
        rule.enabled = query.value("ENABLED").toBool();
        rule.threshold = query.value("THRESHOLD").toDouble();
        rule.priority = query.value("PRIORITY").toInt();
        return rule;
    }
    
    return AlertRule();
}

QList<NotificationManager::Notification> NotificationManager::getNotificationsByType(AlertType type) const
{
    QList<Notification> filtered;
    QSqlQuery query;
    query.prepare("SELECT * FROM NOTIFICATION_HISTORY WHERE ALERT_TYPE = :type ORDER BY TIMESTAMP DESC");
    query.bindValue(":type", static_cast<int>(type));
    query.exec();
    
    while (query.next()) {
        Notification notification;
        notification.id = query.value("ID").toInt();
        notification.timestamp = query.value("TIMESTAMP").toDateTime();
        notification.alertType = static_cast<AlertType>(query.value("ALERT_TYPE").toInt());
        notification.title = query.value("TITLE").toString();
        notification.message = query.value("MESSAGE").toString();
        notification.channels = static_cast<NotificationTypes>(query.value("CHANNELS").toInt());
        notification.sent = query.value("SENT").toBool();
        notification.recipient = query.value("RECIPIENT").toString();
        filtered.append(notification);
    }
    
    return filtered;
}

void NotificationManager::clearNotificationHistory()
{
    QSqlQuery query;
    query.exec("DELETE FROM NOTIFICATION_HISTORY");
    notificationHistory.clear();
}

bool NotificationManager::sendNotification(const QString &title, const QString &message, 
                                          AlertType type, NotificationTypes channels)
{
    Notification notification;
    notification.id = nextNotificationId++;
    notification.timestamp = QDateTime::currentDateTime();
    notification.alertType = type;
    notification.title = title;
    notification.message = message;
    notification.channels = channels;
    notification.sent = false;
    
    bool success = true;
    
    // Send via email
    if (channels & Email && emailEnabled) {
        for (auto it = recipients.begin(); it != recipients.end(); ++it) {
            if (sendEmail(it.key(), title, message)) {
                notification.sent = true;
            } else {
                success = false;
            }
        }
    }
    
    // SMS functionality removed - only email and push notifications are supported
    
    // Send push notification
    if (channels & Push && pushEnabled) {
        sendPushNotification(title, message);
        notification.sent = true;
    }
    
    notification.recipient = recipients.keys().join(", ");
    saveNotificationToHistory(notification);
    
    if (notification.sent) {
        emit notificationSent(notification);
    } else {
        emit notificationFailed(notification, "Failed to send notification");
    }
    
    return success;
}

bool NotificationManager::sendEmail(const QString &to, const QString &subject, const QString &body)
{
    if (smtpServer.isEmpty() || emailUsername.isEmpty() || emailPassword.isEmpty()) {
        qDebug() << "Email settings not configured";
        emit notificationFailed(Notification{0, QDateTime::currentDateTime(), SystemAlert, 
                                           subject, body, Email, false, to}, 
                              "Email settings not configured");
        return false;
    }
    
    qDebug() << "Sending email to:" << to;
    qDebug() << "Subject:" << subject;
    qDebug() << "SMTP Server:" << smtpServer << "Port:" << smtpPort;
    
    QSslSocket *socket = new QSslSocket(this);
    QEventLoop loop;
    bool success = false;
    QString errorMessage;
    QString lastCommand;
    int state = 0; // 0=connect, 1=ehlo, 2=starttls, 3=ehlo2, 4=auth, 5=username, 6=password, 7=mailfrom, 8=rcptto, 9=data, 10=done
    
    // Helper function to send command and wait
    auto sendCommand = [&](const QString &cmd) {
        lastCommand = cmd;
        qDebug() << "Sending:" << cmd.trimmed();
        socket->write(cmd.toUtf8());
        socket->flush();
    };
    
    // Connect signals
    QObject::connect(socket, &QSslSocket::readyRead, [&]() {
        QByteArray data = socket->readAll();
        QString response = QString::fromUtf8(data);
        qDebug() << "SMTP Response:" << response.trimmed();
        
        // Parse response code (first 3 digits)
        int code = 0;
        if (response.length() >= 3) {
            code = response.left(3).toInt();
        }
        
        if (code == 220 && state == 0) {
            // Server greeting
            state = 1;
            sendCommand("EHLO " + smtpServer + "\r\n");
        } else if (code == 250 && state == 1) {
            // EHLO successful, start TLS
            if (useSSL && smtpPort == 587) {
                state = 2;
                sendCommand("STARTTLS\r\n");
            } else {
                state = 4;
                sendCommand("AUTH LOGIN\r\n");
            }
        } else if (code == 220 && state == 2) {
            // STARTTLS accepted, start encryption
            socket->startClientEncryption();
            state = 3;
        } else if (code == 250 && state == 3) {
            // EHLO after TLS successful
            state = 4;
            sendCommand("AUTH LOGIN\r\n");
        } else if (code == 334 && state == 4) {
            // AUTH LOGIN accepted, send username
            state = 5;
            sendCommand(emailUsername.toUtf8().toBase64() + "\r\n");
        } else if (code == 334 && state == 5) {
            // Username accepted, send password
            state = 6;
            sendCommand(emailPassword.toUtf8().toBase64() + "\r\n");
        } else if (code == 235 && state == 6) {
            // Authentication successful
            state = 7;
            sendCommand("MAIL FROM: <" + emailUsername + ">\r\n");
        } else if (code == 250 && state == 7) {
            // MAIL FROM accepted
            state = 8;
            sendCommand("RCPT TO: <" + to + ">\r\n");
        } else if (code == 250 && state == 8) {
            // RCPT TO accepted
            state = 9;
            sendCommand("DATA\r\n");
        } else if (code == 354 && state == 9) {
            // Ready to receive data
            QString emailData = "From: " + emailUsername + "\r\n";
            emailData += "To: " + to + "\r\n";
            emailData += "Subject: " + subject + "\r\n";
            emailData += "MIME-Version: 1.0\r\n";
            emailData += "Content-Type: text/plain; charset=UTF-8\r\n";
            emailData += "\r\n";
            emailData += body + "\r\n";
            emailData += ".\r\n";
            sendCommand(emailData);
            state = 10;
        } else if (code == 250 && state == 10) {
            // Email sent successfully
            sendCommand("QUIT\r\n");
            success = true;
            loop.quit();
        } else if (code >= 400 && code < 600) {
            // Error response
            errorMessage = "SMTP error " + QString::number(code) + ": " + response.left(100);
            qDebug() << "SMTP Error:" << errorMessage;
            loop.quit();
        }
    });
    
    QObject::connect(socket, &QSslSocket::encrypted, [&]() {
        qDebug() << "SSL/TLS connection established";
        // Send EHLO again after encryption
        state = 3;
        sendCommand("EHLO " + smtpServer + "\r\n");
    });
    
    QObject::connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
                     [&](QAbstractSocket::SocketError error) {
        errorMessage = "Socket error: " + socket->errorString();
        qDebug() << errorMessage;
        loop.quit();
    });
    
    QObject::connect(socket, &QSslSocket::sslErrors, [&](const QList<QSslError> &errors) {
        qDebug() << "SSL errors occurred, ignoring for development";
        for (const QSslError &error : errors) {
            qDebug() << "SSL Error:" << error.errorString();
        }
        socket->ignoreSslErrors(); // In production, handle SSL errors properly
    });
    
    // Connect to server
    if (useSSL && smtpPort == 465) {
        // Direct SSL connection
        socket->connectToHostEncrypted(smtpServer, smtpPort);
    } else {
        // Plain connection, then STARTTLS
        socket->connectToHost(smtpServer, smtpPort);
    }
    
    if (!socket->waitForConnected(10000)) {
        errorMessage = "Failed to connect to SMTP server: " + socket->errorString();
        qDebug() << errorMessage;
        socket->deleteLater();
        emit notificationFailed(Notification{0, QDateTime::currentDateTime(), SystemAlert, 
                                           subject, body, Email, false, to}, 
                              errorMessage);
        return false;
    }
    
    qDebug() << "Connected to SMTP server";
    
    // Wait for completion (with timeout)
    QTimer::singleShot(30000, &loop, &QEventLoop::quit); // 30 second timeout
    loop.exec();
    
    socket->disconnectFromHost();
    if (socket->state() != QAbstractSocket::UnconnectedState) {
        socket->waitForDisconnected(3000);
    }
    socket->deleteLater();
    
    if (success) {
        qDebug() << "Email sent successfully to" << to;
        return true;
    } else {
        if (errorMessage.isEmpty()) {
            errorMessage = "Email sending failed - timeout or unknown error. Last state: " + QString::number(state);
        }
        qDebug() << "Failed to send email:" << errorMessage;
        emit notificationFailed(Notification{0, QDateTime::currentDateTime(), SystemAlert, 
                                           subject, body, Email, false, to}, 
                              errorMessage);
        return false;
    }
}

// SMS functionality removed - all SMS functions have been removed

void NotificationManager::sendPushNotification(const QString &title, const QString &message)
{
    qDebug() << "Push Notification - Title:" << title;
    qDebug() << "Push Notification - Message:" << message;
    
    // For desktop applications, this could show a system tray notification
    // For mobile, this would use platform-specific push notification services
    // Qt provides QSystemTrayIcon for desktop notifications
    
    emit notificationSent(Notification{0, QDateTime::currentDateTime(), SystemAlert, 
                                      title, message, Push, true, ""});
}

void NotificationManager::saveNotificationToHistory(const Notification &notification)
{
    QSqlQuery query;
    query.prepare("INSERT INTO NOTIFICATION_HISTORY (ID, TIMESTAMP, ALERT_TYPE, TITLE, MESSAGE, CHANNELS, SENT, RECIPIENT) "
                  "VALUES (:id, :timestamp, :type, :title, :message, :channels, :sent, :recipient)");
    query.bindValue(":id", notification.id);
    query.bindValue(":timestamp", notification.timestamp);
    query.bindValue(":type", static_cast<int>(notification.alertType));
    query.bindValue(":title", notification.title);
    query.bindValue(":message", notification.message);
    query.bindValue(":channels", static_cast<int>(notification.channels));
    query.bindValue(":sent", notification.sent ? 1 : 0);
    query.bindValue(":recipient", notification.recipient);
    query.exec();
}

QList<NotificationManager::Notification> NotificationManager::getNotificationHistory(int limit) const
{
    QList<Notification> history;
    QSqlQuery query;
    query.prepare("SELECT * FROM NOTIFICATION_HISTORY ORDER BY TIMESTAMP DESC LIMIT :limit");
    query.bindValue(":limit", limit);
    query.exec();
    
    while (query.next()) {
        Notification notification;
        notification.id = query.value("ID").toInt();
        notification.timestamp = query.value("TIMESTAMP").toDateTime();
        notification.alertType = static_cast<AlertType>(query.value("ALERT_TYPE").toInt());
        notification.title = query.value("TITLE").toString();
        notification.message = query.value("MESSAGE").toString();
        notification.channels = static_cast<NotificationTypes>(query.value("CHANNELS").toInt());
        notification.sent = query.value("SENT").toBool();
        notification.recipient = query.value("RECIPIENT").toString();
        history.append(notification);
    }
    
    return history;
}

void NotificationManager::checkAlertRules()
{
    for (const AlertRule &rule : alertRules) {
        if (!rule.enabled) continue;
        
        if (checkRuleCondition(rule)) {
            QString details = formatMessage(rule, "");
            sendNotification(rule.name, details, rule.type, rule.channels);
            emit alertTriggered(rule, details);
        }
    }
}

bool NotificationManager::checkRuleCondition(const AlertRule &rule)
{
    // This is a simplified implementation
    // In a real system, you'd parse the condition and evaluate it against database data
    QSqlQuery query;
    
    switch (rule.type) {
        case HighVolume:
            query.prepare("SELECT COUNT(*) FROM DECHETS WHERE VOLUME_POIDS > :threshold");
            query.bindValue(":threshold", rule.threshold);
            if (query.exec() && query.next()) {
                return query.value(0).toInt() > 0;
            }
            break;
            
        case OverdueCollection:
            query.prepare("SELECT COUNT(*) FROM DECHETS WHERE DATE_COLLECTE < SYSDATE - :days");
            query.bindValue(":days", static_cast<int>(rule.threshold));
            if (query.exec() && query.next()) {
                return query.value(0).toInt() > 0;
            }
            break;
            
        case LowRecyclingRate: {
            // Check recycling rate
            query.exec("SELECT COUNT(*) FROM DECHETS WHERE UPPER(ETAT) LIKE '%RECYCLE%'");
            int recycled = 0;
            if (query.next()) recycled = query.value(0).toInt();
            
            query.exec("SELECT COUNT(*) FROM DECHETS");
            int total = 0;
            if (query.next()) total = query.value(0).toInt();
            
            if (total > 0) {
                double rate = (static_cast<double>(recycled) / total) * 100.0;
                return rate < rule.threshold;
            }
            break;
        }
            
        default:
            break;
    }
    
    return false;
}

QString NotificationManager::formatMessage(const AlertRule &rule, const QString &details)
{
    QString message = rule.message;
    if (!details.isEmpty()) {
        message += "\n\n" + details;
    }
    return message;
}

void NotificationManager::saveSettings()
{
    QSettings settings;
    settings.setValue("notification/emailEnabled", emailEnabled);
    // SMS functionality removed - no longer saving SMS settings
    settings.setValue("notification/pushEnabled", pushEnabled);
    // Save recipients
    QStringList emails = recipients.keys();
    settings.setValue("notification/recipientEmails", emails);
    QStringList phones = recipients.values();
    settings.setValue("notification/recipientPhones", phones);
}

void NotificationManager::loadSettings()
{
    QSettings settings;
    emailEnabled = settings.value("notification/emailEnabled", false).toBool();
    // SMS functionality removed - SMS is always disabled
    smsEnabled = false;
    pushEnabled = settings.value("notification/pushEnabled", true).toBool();
    
    // Load recipients
    QStringList emails = settings.value("notification/recipientEmails").toStringList();
    QStringList phones = settings.value("notification/recipientPhones").toStringList();
    recipients.clear();
    for (int i = 0; i < emails.size() && i < phones.size(); i++) {
        recipients[emails[i]] = phones[i];
    }
    
    // Check if credentials are configured (not default placeholders)
    if (!emailUsername.isEmpty() && !emailPassword.isEmpty() && 
        emailUsername != "YOUR_EMAIL@gmail.com" && emailPassword != "YOUR_APP_PASSWORD") {
        emailEnabled = true;
    }
    // SMS functionality removed - SMS is always disabled
    smsEnabled = false;
}

QList<NotificationManager::AlertRule> NotificationManager::getAlertRules() const
{
    return alertRules;
}

