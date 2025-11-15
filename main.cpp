// main.cpp
#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Connexion à la base de données
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");  // DSN configuré dans Windows ODBC
    db.setUserName("dorra");
    db.setPassword("oracle123");

    if (!db.open()) {
        qDebug() << "❌ ERREUR CONNEXION:" << db.lastError().text();
        QMessageBox::critical(nullptr, "Erreur",
                              "Connexion échouée à la base de données!\n" +
                                  db.lastError().text());
        return 1;
    }

    qDebug() << "✅ Connexion réussie à la base de données!";

    MainWindow w;
    w.show();

    return a.exec();
}
