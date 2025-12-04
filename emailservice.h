#ifndef EMAILSERVICE_H
#define EMAILSERVICE_H

#include <QString>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QCryptographicHash>

class EmailService : public QObject
{
    Q_OBJECT

public:
    explicit EmailService(QObject *parent = nullptr);
    
    // Configuration SMTP
    void setSmtpConfig(const QString &server, int port, 
                      const QString &username, const QString &password);
    
    // Configuration Twilio
    void setTwilioConfig(const QString &accountSid, const QString &authToken, 
                        const QString &fromNumber);
    
    // Méthodes d'envoi
    bool sendEmail(const QString &to, const QString &subject, const QString &body);
    bool sendSms(const QString &to, const QString &message);
    bool sendVerificationCode(const QString &to, const QString &code);
    bool sendPasswordReset(const QString &to, const QString &tempPassword);
    
    // Configuration rapide pour les services courants
    void configureGmail(const QString &username, const QString &appPassword);
    void configureOutlook(const QString &username, const QString &password);

private:
    QString m_smtpServer;
    int m_smtpPort;
    QString m_username;
    QString m_password;
    QNetworkAccessManager *m_networkManager;
    
    // Twilio configuration
    QString m_twilioAccountSid;
    QString m_twilioAuthToken;
    QString m_twilioFromNumber;
    
    bool sendViaSmtp(const QString &to, const QString &subject, const QString &body);
    bool sendViaHttpApi(const QString &to, const QString &subject, const QString &body);
    bool sendViaTwilio(const QString &to, const QString &message);
    QString generateBasicAuth(const QString &username, const QString &password);
};

#endif // EMAILSERVICE_H
