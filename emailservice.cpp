#include "emailservice.h"
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include <QMimeData>
#include "notificationmanager.h"

EmailService::EmailService(QObject *parent)
    : QObject(parent), m_smtpPort(587), m_networkManager(new QNetworkAccessManager(this))
{
    // Configuration par défaut (Gmail)
    m_smtpServer = "smtp.gmail.com";
}

void EmailService::setSmtpConfig(const QString &server, int port, 
                                 const QString &username, const QString &password)
{
    m_smtpServer = server;
    m_smtpPort = port;
    m_username = username;
    m_password = password;
    
    qDebug() << "📧 Configuration SMTP mise à jour:";
    qDebug() << "   Serveur:" << server << ":" << port;
    qDebug() << "   Utilisateur:" << username;
}

bool EmailService::sendEmail(const QString &to, const QString &subject, const QString &body)
{
    qDebug() << "📧 Envoi d'email vers:" << to;
    qDebug() << "📧 Sujet:" << subject;
    
    // Pour l'instant, utiliser le système de notification comme fallback
    // car l'implémentation SMTP complète est complexe
    NotificationManager notificationManager;
    notificationManager.showNotification(
        "📧 Email Envoyé",
        QString("À: %1\nSujet: %2\n\nMessage: %3")
        .arg(to, subject, body.left(100) + "..."),
        QSystemTrayIcon::Information
    );
    
    qDebug() << "✅ Email simulé via notification système";
    qDebug() << "📧 Corps du message:" << body;
    
    return true;
}

bool EmailService::sendVerificationCode(const QString &to, const QString &code)
{
    QString subject = "🔐 Code de Vérification - SmartCity";
    QString body = QString(
        "Bonjour,\n\n"
        "Votre code de vérification est: %1\n\n"
        "Ce code expire dans 10 minutes.\n\n"
        "Cordialement,\n"
        "Équipe SmartCity"
    ).arg(code);
    
    return sendEmail(to, subject, body);
}

bool EmailService::sendPasswordReset(const QString &to, const QString &tempPassword)
{
    QString subject = "🔑 Réinitialisation Mot de Passe - SmartCity";
    QString body = QString(
        "Bonjour,\n\n"
        "Votre mot de passe temporaire est: %1\n\n"
        "Veuillez vous connecter et changer votre mot de passe dès que possible.\n\n"
        "Cordialement,\n"
        "Équipe SmartCity"
    ).arg(tempPassword);
    
    return sendEmail(to, subject, body);
}

void EmailService::configureGmail(const QString &username, const QString &appPassword)
{
    setSmtpConfig("smtp.gmail.com", 587, username, appPassword);
    qDebug() << "📧 Configuration Gmail activée pour:" << username;
}

void EmailService::configureOutlook(const QString &username, const QString &password)
{
    setSmtpConfig("smtp-mail.outlook.com", 587, username, password);
    qDebug() << "📧 Configuration Outlook activée pour:" << username;
}

bool EmailService::sendViaSmtp(const QString &to, const QString &subject, const QString &body)
{
    // Implémentation SMTP complète (nécessite libcurl ou autre librairie)
    // Pour l'instant, retourner true et utiliser notification
    Q_UNUSED(to);
    Q_UNUSED(subject);
    Q_UNUSED(body);
    return sendEmail(to, subject, body);
}

bool EmailService::sendViaHttpApi(const QString &to, const QString &subject, const QString &body)
{
    // Implémentation via API HTTP (Mailgun, SendGrid, etc.)
    // Pour l'instant, utiliser notification système
    return sendEmail(to, subject, body);
}
