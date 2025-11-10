#include "connection.h"
#include <QSqlError>
#include <QDebug>

bool Connection::createConnect()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("dorra");
    db.setPassword("oracle123");

    if(!db.open())
    {
        qDebug() << "❌ Erreur connexion:" << db.lastError().text();
        return false;
    }

    qDebug() << "✅ Connexion réussie à la base de données!";
    return true;
}
