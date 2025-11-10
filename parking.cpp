#include "parking.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

// ===============================
// 🔹 Constructeurs
// ===============================
Parking::Parking() {}

Parking::Parking(QString id, QString localisation, int capacite, int placeslibres,
                 QString statut, QString type, double tarif)
{
    this->id = id;
    this->localisation = localisation;
    this->capacite = capacite;
    this->placeslibres = placeslibres;
    this->statut = statut;
    this->type = type;
    this->tarif = tarif;
}

// ===============================
// 🔹 AJOUTER
// ===============================
bool Parking::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO NOUR.PARKING (ID, LOCALISATION, CAPACITE, PLACESLIBRES, STATUT, TYPE, TARIFICATION) "
                  "VALUES (:id, :localisation, :capacite, :placeslibres, :statut, :type, :tarif)");
    query.bindValue(":id", id);
    query.bindValue(":localisation", localisation);
    query.bindValue(":capacite", capacite);
    query.bindValue(":placeslibres", placeslibres);
    query.bindValue(":statut", statut);
    query.bindValue(":type", type);
    query.bindValue(":tarif", tarif);

    if (!query.exec()) {
        qDebug() << "Erreur ajout parking :" << query.lastError().text();
        return false;
    }
    return true;
}

// ===============================
// 🔹 AFFICHER
// ===============================
QSqlQueryModel *Parking::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID, LOCALISATION, CAPACITE, PLACESLIBRES, STATUT, TYPE, TARIFICATION FROM NOUR.PARKING");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Localisation"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Capacité"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Places libres"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Tarif"));

    return model;
}

// ===============================
// 🔹 SUPPRIMER
// ===============================
bool Parking::supprimerParId(QString id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM NOUR.PARKING WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur suppression :" << query.lastError().text();
        return false;
    }
    return true;
}

// ===============================
// 🔹 MODIFIER
// ===============================
bool Parking::modifierParId(QString id)
{
    QSqlQuery query;
    query.prepare("UPDATE NOUR.PARKING SET LOCALISATION = :localisation, "
                  "CAPACITE = :capacite, PLACESLIBRES = :placeslibres, "
                  "STATUT = :statut, TYPE = :type, TARIFICATION = :tarif "
                  "WHERE ID = :id");

    query.bindValue(":localisation", localisation);
    query.bindValue(":capacite", capacite);
    query.bindValue(":placeslibres", placeslibres);
    query.bindValue(":statut", statut);
    query.bindValue(":type", type);
    query.bindValue(":tarif", tarif);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur modification parking :" << query.lastError().text();
        return false;
    }
    return true;
}
