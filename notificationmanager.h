#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslSocket>
#include <QList>
#include <QVariant>

// Intelligent notification system implemented using QSystemTrayIcon,
// real-time alert monitoring using QTimer, and Oracle SQL queries via QSqlQuery.
// Ensures automatic detection of new alerts and immediate user notification.

class NotificationManager : public QObject
{
    Q_OBJECT

public:
    explicit NotificationManager(QObject *parent = nullptr);
    ~NotificationManager();

    // Initialize the notification system
    void initialize();
    
    // Start/Stop monitoring
    void startMonitoring(int intervalSeconds = 5);
    void stopMonitoring();
    
    // Check if system tray is available
    bool isSystemTrayAvailable() const;
    
    // Show manual notification (for testing)
    void showNotification(const QString &title, const QString &message, 
                         QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information);
    
    // Email functionality
    bool sendEmail(const QString &to, const QString &subject, const QString &body);
    void setEmailSettings(const QString &smtpServer, int smtpPort, 
                         const QString &smtpUser, const QString &smtpPassword,
                         bool useSSL = true);
    QString getRecipientEmail() const { return m_recipientEmail; }
    void setRecipientEmail(const QString &email);
    bool isEmailEnabled() const { return m_emailEnabled; }
    void setEmailEnabled(bool enabled);
    
    // Notification history
    struct NotificationEntry {
        QDateTime dateTime;
        QString type; // "System", "Email", "Both"
        QString title;
        QString message;
        QString alertId;
        QString client;
        QString gravite;
        bool emailSent;
        bool systemNotificationSent;
        QString emailRecipient;
    };
    
    QList<NotificationEntry> getNotificationHistory() const { return m_notificationHistory; }
    void clearNotificationHistory();

signals:
    void notificationSent(const QString &title, const QString &message);
    void newAlertDetected(int alertId, const QString &gravite, const QString &client, 
                         const QString &description, const QString &type);
    void criticalAlertDetected(int alertId, const QString &client, const QString &type, 
                              const QString &description);

private slots:
    void checkForNewAlerts();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void showApplication();
    void quitApplication();

private:
    // System tray icon
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayMenu;
    QAction *m_showAction;
    QAction *m_quitAction;
    
    // Timer for periodic checking
    QTimer *m_checkTimer;
    int m_checkInterval; // in seconds
    
    // Database connection
    QSqlDatabase m_database;
    
    // Last processed alert ID
    int m_lastAlertID;
    
    // Email settings
    QNetworkAccessManager *m_networkManager;
    QString m_smtpServer;
    int m_smtpPort;
    QString m_smtpUser;
    QString m_smtpPassword;
    bool m_useSSL;
    QString m_recipientEmail;
    bool m_emailEnabled;
    
    // Notification history
    QList<NotificationEntry> m_notificationHistory;
    static const int MAX_HISTORY_ENTRIES = 1000; // Limit history size
    
    // Helper methods
    void setupTrayIcon();
    void setupTrayMenu();
    void checkDatabaseConnection();
    void processNewAlert(int idAl, const QString &gravite, const QString &client,
                        const QString &description, const QString &type,
                        const QString &statut, const QDateTime &dateEtHeure, int idEmp);
    QString formatNormalNotification(const QString &client, const QString &gravite,
                                    const QString &description);
    QString formatCriticalNotification(const QString &client, const QString &type,
                                      const QString &description);
};

#endif // NOTIFICATIONMANAGER_H
