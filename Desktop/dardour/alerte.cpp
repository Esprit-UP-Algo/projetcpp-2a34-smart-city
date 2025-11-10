#include "alerte.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QTableWidgetItem>
#include <QDebug>
#include <QSqlDatabase>

// Constructeur
alerte::alerte(QString id, QString type, QString client, QString statut,
               QString description, QString gravite, QString datee)
{
    this->id = id;
    this->type = type;
    this->client = client;
    this->statut = statut;
    this->description = description;
    this->gravite = gravite;
    this->datee = datee;
}

// ======================== AJOUTER ========================
bool alerte::ajouter()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.isOpen())
    {
        qDebug() << "Base non ouverte!";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO ALERTES (ID, TYPE, CLIENT, STATUT, DESCRIPTION, GRAVITE, DATEE) "
                  "VALUES (:id, :type, :client, :statut, :description, :gravite, :datee)");

    query.bindValue(":id", id.toInt());
    query.bindValue(":type", type);
    query.bindValue(":client", client);
    query.bindValue(":statut", statut);
    query.bindValue(":description", description);
    query.bindValue(":gravite", gravite);
    query.bindValue(":datee", datee);

    if(!query.exec())
    {
        qDebug() << "Erreur ajout:" << query.lastError().text();
        return false;
    }

    return true;
}

// ======================== SUPPRIMER ========================
bool alerte::supprimer(int id)
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.isOpen())
    {
        qDebug() << "Base non ouverte!";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM ALERTES WHERE ID=:id");
    query.bindValue(":id", id);

    if(!query.exec())
    {
        qDebug() << "Erreur suppression:" << query.lastError().text();
        return false;
    }

    return true;
}

// ======================== MODIFIER ========================
bool alerte::modifier(int id)
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.isOpen())
    {
        qDebug() << "Base non ouverte!";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE ALERTES SET TYPE=:type, CLIENT=:client, STATUT=:statut, "
                  "DESCRIPTION=:description, GRAVITE=:gravite, DATEE=:datee "
                  "WHERE ID=:id");

    query.bindValue(":id", id);
    query.bindValue(":type", type);
    query.bindValue(":client", client);
    query.bindValue(":statut", statut);
    query.bindValue(":description", description);
    query.bindValue(":gravite", gravite);
    query.bindValue(":datee", datee);

    if(!query.exec())
    {
        qDebug() << "Erreur modification:" << query.lastError().text();
        return false;
    }

    return true;
}

// ======================== AFFICHER DANS QTABLEWIDGET ========================
void alerte::afficherTable(QTableWidget* table)
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.isOpen())
    {
        qDebug() << "Base non ouverte!";
        return;
    }

    QSqlQuery query("SELECT * FROM ALERTES");

    table->setRowCount(0); // vide la table
    int row = 0;

    while(query.next())
    {
        table->insertRow(row);
        int colCount = query.record().count();
        for(int col = 0; col < colCount; ++col)
        {
            table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}
