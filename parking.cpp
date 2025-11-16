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
    this->nom = "";  // Valeur par défaut
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
    query.prepare("INSERT INTO NOUR.PARKING (ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES, STATUT, TYPE, TARIFICATION) "
                  "VALUES (:id, :nom, :localisation, :capacite, :placeslibres, :statut, :type, :tarif)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":localisation", localisation);
    query.bindValue(":capacite", capacite);
    query.bindValue(":placeslibres", placeslibres);
    query.bindValue(":statut", statut);
    query.bindValue(":type", type);
    query.bindValue(":tarif", tarif);

    if (!query.exec()) {
        qDebug() << "❌ Erreur ajout parking :" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Parking ajouté avec succès";
    return true;
}

// ===============================
// 🔹 AFFICHER
// ===============================
QSqlQueryModel *Parking::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES, STATUT, TYPE, TARIFICATION FROM NOUR.PARKING ORDER BY ID_PARKING");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Localisation"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Capacité"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Places libres"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Tarification"));

    return model;
}

// ===============================
// 🔹 SUPPRIMER
// ===============================
bool Parking::supprimerParId(QString id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM NOUR.PARKING WHERE ID_PARKING = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "❌ Erreur suppression :" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Parking supprimé - ID:" << id;
    return true;
}

// ===============================
// 🔹 MODIFIER
// ===============================
bool Parking::modifierParId(QString id)
{
    QSqlQuery query;
    query.prepare("UPDATE NOUR.PARKING SET "
                  "NOM = :nom, "
                  "LOCALISATION = :localisation, "
                  "CAPACITE = :capacite, "
                  "PLACESLIBRES = :placeslibres, "
                  "STATUT = :statut, "
                  "TYPE = :type, "
                  "TARIFICATION = :tarif "
                  "WHERE ID_PARKING = :id");

    query.bindValue(":nom", nom);
    query.bindValue(":localisation", localisation);
    query.bindValue(":capacite", capacite);
    query.bindValue(":placeslibres", placeslibres);
    query.bindValue(":statut", statut);
    query.bindValue(":type", type);
    query.bindValue(":tarif", tarif);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "❌ Erreur modification parking :" << query.lastError().text();
        qDebug() << "   Requête SQL:" << query.lastQuery();
        return false;
    }

    qDebug() << "✅ Parking modifié - ID:" << id;
    return true;
}
