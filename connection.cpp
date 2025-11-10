#include "connection.h"
#include <QDebug>
#include <QSqlError>

Connection::Connection()
{
    // Utiliser un nom de connexion unique pour éviter les conflits
    if (QSqlDatabase::contains("oracle_connection")) {
        db = QSqlDatabase::database("oracle_connection");
    } else {
        db = QSqlDatabase::addDatabase("QODBC", "oracle_connection");
    }
}

Connection::~Connection()
{
    if (db.isOpen()) db.close();
}

Connection& Connection::createInstance()
{
    static Connection instance;
    return instance;
}

bool Connection::createConnect()
{
    if (db.isOpen()) return true; // ✅ éviter de rouvrir inutilement

    db.setDatabaseName("Projet_2A");
    db.setUserName("NOUR");
    db.setPassword("Mimou22!");

    if (db.open()) {
        qDebug() << "Connexion réussie";
        return true;
    } else {
        qDebug() << "Échec:" << db.lastError().text();
        return false;
    }
}

QSqlDatabase& Connection::getDatabase()
{
    return db;
}

