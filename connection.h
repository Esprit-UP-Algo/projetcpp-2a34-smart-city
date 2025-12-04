#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>
#include <QMessageBox>

class Connection
{
public:
    Connection();
    bool createconnect();

    // Méthodes statiques pour la récupération de mot de passe
    static QString genererCodeVerification();
    static QString genererMotDePasseTemporaire();
    static bool envoyerEmail(const QString &email, const QString &sujet, const QString &message);
    static bool envoyerSMS(const QString &telephone, const QString &message);

private:
    // Configuration SendGrid
    static QString sendGridApiKey;
    static QString sendGridFromEmail;
    static QString sendGridFromName;
    
    // Configuration Twilio
    static QString twilioAccountSID;
    static QString twilioAuthToken;
    static QString twilioPhoneNumber;
    
    // Méthodes privées
    static bool envoyerEmailSendGrid(const QString &destinataire, const QString &sujet, const QString &message);
    static bool envoyerSMSTwilio(const QString &telephone, const QString &message);
};

#endif // CONNECTION_H
