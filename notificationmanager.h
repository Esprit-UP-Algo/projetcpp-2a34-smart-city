#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>
#include <QMap>
#include <QSettings>

class NotificationManager : public QObject
{
    Q_OBJECT

public:
    enum NotificationType {
        Email = 1,
        SMS = 2,
        Push = 4,
        All = Email | SMS | Push
    };
    Q_DECLARE_FLAGS(NotificationTypes, NotificationType)

    enum AlertType {
        HighVolume,
        OverdueCollection,
        LowRecyclingRate,
        SystemAlert,
        ScheduledCollection,
        Custom
    };

    struct AlertRule {
        AlertType type;
        QString name;
        QString condition;  // SQL-like condition or expression
        QString message;
        NotificationTypes channels;
        bool enabled;
        double threshold;  // For numeric conditions
        int priority;  // 1-5, 5 being highest
    };

    struct Notification {
        int id;
        QDateTime timestamp;
        AlertType alertType;
        QString title;
        QString message;
        NotificationTypes channels;
        bool sent;
        QString recipient;
    };

    explicit NotificationManager(QObject *parent = nullptr);
    ~NotificationManager();

    // Configuration
    // Simplified: Only set recipient email, SMTP credentials are hardcoded
    void setRecipientEmail(const QString &email);
    void addRecipient(const QString &email, const QString &phone = "");
    
    // Advanced: Set custom credentials (optional, uses Gmail by default)
    void setEmailSettings(const QString &smtpServer, int port, const QString &username, 
                         const QString &password, bool useSSL = true);

    // Alert Rules Management
    void addAlertRule(const AlertRule &rule);
    void removeAlertRule(int ruleId);
    void updateAlertRule(int ruleId, const AlertRule &rule);
    QList<AlertRule> getAlertRules() const;
    AlertRule getAlertRule(int ruleId) const;

    // Notification Sending
    bool sendNotification(const QString &title, const QString &message, 
                         AlertType type = SystemAlert, NotificationTypes channels = All);
    bool sendEmail(const QString &to, const QString &subject, const QString &body);
    void sendPushNotification(const QString &title, const QString &message);

    // Notification History
    void saveNotificationToHistory(const Notification &notification);
    QList<Notification> getNotificationHistory(int limit = 100) const;
    QList<Notification> getNotificationsByType(AlertType type) const;
    void clearNotificationHistory();

    // Alert Checking (to be called periodically)
    void checkAlertRules();

    // Settings
    void saveSettings();
    void loadSettings();
    bool isEmailEnabled() const { return emailEnabled; }
    bool isPushEnabled() const { return pushEnabled; }
    void setEmailEnabled(bool enabled) { emailEnabled = enabled; }
    void setPushEnabled(bool enabled) { pushEnabled = enabled; }

signals:
    void notificationSent(const Notification &notification);
    void notificationFailed(const Notification &notification, const QString &error);
    void alertTriggered(const AlertRule &rule, const QString &details);

private:
    // Email settings
    QString smtpServer;
    int smtpPort;
    QString emailUsername;
    QString emailPassword;
    bool useSSL;
    bool emailEnabled;

    // SMS settings
    QString smsApiKey;
    QString smsApiSecret;
    QString smsSenderNumber;
    bool smsEnabled;

    // Push notifications
    bool pushEnabled;

    // Recipients
    QMap<QString, QString> recipients;  // email -> phone

    // Alert Rules
    QList<AlertRule> alertRules;
    int nextRuleId;

    // Notification History
    QList<Notification> notificationHistory;
    int nextNotificationId;

    // Helper methods
    bool checkRuleCondition(const AlertRule &rule);
    QString formatMessage(const AlertRule &rule, const QString &details);
    void initializeDatabase();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(NotificationManager::NotificationTypes)

#endif // NOTIFICATIONMANAGER_H

