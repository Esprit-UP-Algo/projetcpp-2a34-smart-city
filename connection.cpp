#include "connection.h"
#include <QtSql/QSqlDatabase>
Connection* Connection::p_instance = nullptr;
Connection::Connection()
{
}
Connection* Connection::instance()
{
    if (p_instance == nullptr) {
        p_instance = new Connection();
    }
    return p_instance;
}

bool Connection::createConnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase ("QODBC");
    db.setDatabaseName ("Source_Projet2A");
    db.setUserName("anis");
    db.setPassword("hana123");
    if (db.open()) {
        test = true;
    }
    return test;
}
Connection::~Connection()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (db.isOpen()) {
        db.close();
    }
}
