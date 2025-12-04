#include "connection.h"
#include <QProcess>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QDebug>
#include <QTemporaryFile>
#include <QMessageBox>
#include <QApplication>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// CONFIGURATION SENDGRID - MODIFIÉ AVEC VOTRE EMAIL
QString Connection::sendGridApiKey = "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";
QString Connection::sendGridFromEmail = "malekchhoumi1920@gmail.com";  // VOTRE EMAIL
QString Connection::sendGridFromName = "Système de Gestion Malek";

// CONFIGURATION TWILIO - POUR LES SMS (OPTIONNEL)
QString Connection::twilioAccountSID = "ACvotre_account_sid";
QString Connection::twilioAuthToken = "votre_auth_token";
QString Connection::twilioPhoneNumber = "+1234567890";

Connection::Connection()
{
    qDebug() << "📧 Service d'initialisé";
}

bool Connection::createconnect()
{
    bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projet");
    db.setUserName("malek");
    db.setPassword("esprit2026");

    if (db.open())
        test=true;

    return test;
}

// Méthodes statiques pour la récupération de mot de passe
QString Connection::genererCodeVerification()
{
    // Génère un code de 6 chiffres
    int code = QRandomGenerator::global()->bounded(100000, 999999);
    return QString::number(code);
}

QString Connection::genererMotDePasseTemporaire()
{
    // Génère un mot de passe temporaire de 8 caractères alphanumériques
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString password;
    password.reserve(8);

    for (int i = 0; i < 8; ++i) {
        int index = QRandomGenerator::global()->bounded(chars.length());
        password.append(chars.at(index));
    }

    return password;
}

bool Connection::envoyerEmail(const QString& destinataire, const QString& sujet, const QString& message)
{
    qDebug() << "=== TENTATIVE D'ENVOI EMAIL AVEC SENDGRID ===";
    qDebug() << "De:" << sendGridFromEmail;
    qDebug() << "À:" << destinataire;
    qDebug() << "Sujet:" << sujet;

    // Vérifier que la clé API est configurée
    if (sendGridApiKey.isEmpty() || sendGridApiKey.contains("votre_cle_api")) {
        qDebug() << "❌ Clé API SendGrid non configurée";

        // Mode développement - afficher le code
        QMessageBox::information(nullptr, "🔐 Mode Développement",
                                 QString("📧 **Email** : %1\n\n"
                                         "📋 **Message** : %2\n\n"
                                         "⚠️ **Configuration SendGrid requise**\n"
                                         "Clé API détectée mais vérifiez la configuration.")
                                     .arg(destinataire)
                                     .arg(message));
        return true;
    }

    // Essayer d'envoyer avec SendGrid
    bool succes = envoyerEmailSendGrid(destinataire, sujet, message);

    if (succes) {
        qDebug() << "✅ Email envoyé avec succès via SendGrid!";
        QMessageBox::information(nullptr, "✅ Email Envoyé",
                                 "📧 **Email envoyé avec succès !**\n\n"
                                 "Un code de vérification a été envoyé à :\n"
                                 "**" + destinataire + "**\n\n"
                                       "Vérifiez votre boîte de réception.");
        return true;
    } else {
        qDebug() << "❌ Échec envoi email SendGrid";

        // Fallback: mode développement
        QMessageBox::information(nullptr, "🔐 Mode Développement",
                                 QString("📧 **Email** : %1\n\n"
                                         "📋 **Message** : %2\n\n"
                                         "⚠️ **SendGrid a rencontré une erreur**\n"
                                         "Vérifiez votre clé API et la configuration.")
                                     .arg(destinataire)
                                     .arg(message));
        return true;
    }
}

bool Connection::envoyerEmailSendGrid(const QString& destinataire, const QString& sujet, const QString& message)
{
    QNetworkAccessManager manager;

    // URL de l'API SendGrid
    QUrl url("https://api.sendgrid.com/v3/mail/send");
    QNetworkRequest request(url);

    // Headers SendGrid
    request.setRawHeader("Authorization", "Bearer " + sendGridApiKey.toUtf8());
    request.setRawHeader("Content-Type", "application/json");

    // Construction du JSON pour SendGrid
    QJsonObject json;

    // Personalizations
    QJsonArray personalizations;
    QJsonObject personalization;
    QJsonArray toArray;
    QJsonObject toObject;

    toObject["email"] = destinataire;
    toArray.append(toObject);
    personalization["to"] = toArray;
    personalizations.append(personalization);

    json["personalizations"] = personalizations;

    // From - AVEC VOTRE EMAIL
    QJsonObject fromObject;
    fromObject["email"] = sendGridFromEmail;
    fromObject["name"] = sendGridFromName;
    json["from"] = fromObject;

    // Subject
    json["subject"] = sujet;

    // Content
    QJsonArray contentArray;
    QJsonObject contentObject;
    contentObject["type"] = "text/plain";
    contentObject["value"] = message;
    contentArray.append(contentObject);
    json["content"] = contentArray;

    // Convertir en JSON
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    qDebug() << "📤 Envoi à SendGrid...";
    qDebug() << "URL: " << url.toString();
    qDebug() << "Data size:" << data.size() << "bytes";

    // Envoi synchrone
    QNetworkReply *reply = manager.post(request, data);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool succes = false;
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray response = reply->readAll();

    qDebug() << "Status Code:" << statusCode;
    qDebug() << "Response:" << response;

    if (reply->error() == QNetworkReply::NoError) {
        if (statusCode == 202) {
            qDebug() << "✅ SendGrid: Email accepté (202)";
            succes = true;
        } else {
            qDebug() << "❌ SendGrid: Statut HTTP" << statusCode;
            QMessageBox::warning(nullptr, "Erreur SendGrid",
                                 QString("Statut HTTP: %1\nRéponse: %2")
                                     .arg(statusCode)
                                     .arg(QString::fromUtf8(response)));
        }
    } else {
        qDebug() << "❌ Erreur réseau SendGrid:" << reply->errorString();
        QMessageBox::warning(nullptr, "Erreur Réseau",
                             QString("Erreur: %1\nDétails: %2")
                                 .arg(reply->errorString())
                                 .arg(QString::fromUtf8(response)));
    }

    reply->deleteLater();
    return succes;
}

bool Connection::envoyerSMS(const QString& telephone, const QString& message)
{
    qDebug() << "=== ENVOI SMS ===";
    qDebug() << "Téléphone: +216" << telephone;
    qDebug() << "Message:" << message;

    // Vérifier la configuration Twilio
    if (twilioAccountSID.contains("votre_account") || twilioAuthToken.contains("votre_auth")) {
        // Mode simulation
        QMessageBox::information(nullptr, "📱 SMS Simulé",
                                 QString("**SMS serait envoyé à :** +216%1\n\n"
                                         "**Message :** %2\n\n"
                                         "Pour les SMS réels : configurez Twilio")
                                     .arg(telephone)
                                     .arg(message));
        return true;
    } else {
        // Envoi réel via Twilio
        return envoyerSMSTwilio(telephone, message);
    }
}

bool Connection::envoyerSMSTwilio(const QString& telephone, const QString& message)
{
    // Format du numéro
    QString numeroComplet = "+216" + telephone;

    // Construction de l'URL Twilio
    QUrl url(QString("https://api.twilio.com/2010-04-01/Accounts/%1/Messages.json").arg(twilioAccountSID));

    // Préparation des données
    QUrlQuery postData;
    postData.addQueryItem("To", numeroComplet);
    postData.addQueryItem("From", twilioPhoneNumber);
    postData.addQueryItem("Body", message);

    // Préparation de la requête
    QNetworkRequest request(url);
    QString auth = twilioAccountSID + ":" + twilioAuthToken;
    QByteArray authData = auth.toUtf8().toBase64();
    request.setRawHeader("Authorization", "Basic " + authData);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Envoi synchrone
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool succes = false;
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "✅ SMS envoyé avec succès via Twilio";
        succes = true;
    } else {
        qDebug() << "❌ Erreur Twilio:" << reply->errorString();
        QMessageBox::warning(nullptr, "Erreur Twilio",
                             QString("Erreur: %1\nVérifiez la configuration Twilio.")
                                 .arg(reply->errorString()));
    }

    reply->deleteLater();
    return succes;
}
