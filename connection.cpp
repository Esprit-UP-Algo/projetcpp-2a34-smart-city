#include "connection.h"
#include "notificationmanager.h"
#include <QRandomGenerator>
#include <QDebug>

Connection::Connection()
{

}

bool Connection::createconnect()
{
    bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");//inserer le nom de la source de données
    db.setUserName("anis");//inserer nom de l'utilisateur
    db.setPassword("hana123");//inserer mot de passe de cet utilisateur

    if (db.open()) {
        test=true;
        // Set auto-commit to false for Oracle to allow manual commits
        // Oracle requires explicit COMMIT for data to be saved
        qDebug() << "✅ Database connection successful!";
        qDebug() << "Database:" << db.databaseName();
        qDebug() << "Driver:" << db.driverName();
    } else {
        qDebug() << "❌ Database connection failed!";
        qDebug() << "Error:" << db.lastError().text();
    }

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

bool Connection::envoyerEmail(const QString &email, const QString &sujet, const QString &message)
{
    NotificationManager notificationManager;
    return notificationManager.sendEmail(email, sujet, message);
}
