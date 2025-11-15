// alerte.cpp
#include "alerte.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

alerte::alerte() : id(0) {}

alerte::alerte(int id, QString type, QString client, QString statut,
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

// ================= AJOUTER =================
bool alerte::ajouter()
{
    QSqlQuery query;

    // ✅ SANS TO_DATE - Oracle convertit automatiquement
    query.prepare("INSERT INTO ALERTES (ID, TYPE, CLIENT, STATUT, DESCRIPTION, GRAVITE, DATEE) "
                  "VALUES (:id, :type, :client, :statut, :desc, :grav, :date)");

    query.bindValue(":id", id);  // ✅ Pas QString::number() - direct int
    query.bindValue(":type", type);
    query.bindValue(":client", client);
    query.bindValue(":statut", statut);
    query.bindValue(":desc", description);
    query.bindValue(":grav", gravite);

    // ✅ Si date vide → NULL, sinon → QString direct
    if (datee.isEmpty()) {
        query.bindValue(":date", QVariant(QVariant::String));
    } else {
        query.bindValue(":date", datee);
    }

    if (!query.exec()) {
        qDebug() << "ERREUR AJOUT SQL:" << query.lastError().text();
        return false;
    }
    return true;
}

// ================= MODIFIER =================
bool alerte::modifier()
{
    QSqlQuery query;

    query.prepare("UPDATE ALERTES SET "
                  "TYPE = :type, CLIENT = :client, STATUT = :statut, "
                  "DESCRIPTION = :desc, GRAVITE = :grav, DATEE = :date "
                  "WHERE ID = :id");

    query.bindValue(":id", id);
    query.bindValue(":type", type);
    query.bindValue(":client", client);
    query.bindValue(":statut", statut);
    query.bindValue(":desc", description);
    query.bindValue(":grav", gravite);

    if (datee.isEmpty()) {
        query.bindValue(":date", QVariant(QVariant::String));
    } else {
        query.bindValue(":date", datee);
    }

    if (!query.exec()) {
        qDebug() << "ERREUR MODIF SQL:" << query.lastError().text();
        return false;
    }
    return true;
}

// ================= SUPPRIMER =================
bool alerte::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM ALERTES WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "ERREUR SUPPR SQL:" << query.lastError().text();
        return false;
    }
    return true;
}

// ================= AFFICHER =================
void alerte::afficherTable(QTableWidget *table)
{
    table->clearContents();
    table->setRowCount(0);

    QSqlQuery query;

    // ✅ SELECT simple - DATEE comme VARCHAR
    query.prepare("SELECT ID, TYPE, CLIENT, STATUT, DESCRIPTION, GRAVITE, DATEE "
                  "FROM ALERTES ORDER BY ID");

    if (!query.exec()) {
        qDebug() << "ERREUR AFFICHAGE SQL:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        for (int col = 0; col < 7; ++col) {
            table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }

    table->resizeColumnsToContents();
}
