#include "parking.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

// ===============================
// 🔹 Constructeurs
// ===============================
Parking::Parking() {}

Parking::Parking(QString id, QString nom, QString localisation, int capacite, int placeslibres,
                 QString statut, QString type, double tarif)
{
    this->id = id;
    this->nom = nom;
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
    // Vérifier si la connexion existe déjà
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) {
            qDebug() << "❌ Erreur connexion base de données";
            return false;
        }
    }
    
    QSqlQuery query;
    query.prepare("INSERT INTO PARKING (ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES, STATUT, TYPE, TARIFICATION, IDEMP) "
                  "VALUES (:id, :nom, :localisation, :capacite, :placeslibres, :statut, :type, :tarif, NULL)");

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
    // Vérifier si la connexion existe déjà
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) {
            return nullptr;
        }
    }
    
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES, STATUT, TYPE, TARIFICATION FROM PARKING ORDER BY ID_PARKING");

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
    // Vérifier si la connexion existe déjà
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) {
            return false;
        }
    }
    
    QSqlQuery query;
    query.prepare("DELETE FROM PARKING WHERE ID_PARKING = :id");
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
    // Vérifier si la connexion existe déjà
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) {
            return false;
        }
    }
    
    QSqlQuery query;
    query.prepare("UPDATE PARKING SET "
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

// ===============================
// 🔹 RECHERCHER PAR LOCALISATION
// ===============================
QSqlQueryModel *Parking::rechercherParLocalisation(QString localisation)
{
    // Vérifier si la connexion existe déjà
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) {
            return nullptr;
        }
    }
    
    QSqlQueryModel *model = new QSqlQueryModel();
    QString sql = QString("SELECT ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES, STATUT, TYPE, TARIFICATION "
                          "FROM PARKING "
                          "WHERE LOWER(LOCALISATION) LIKE LOWER('%%1%') "
                          "ORDER BY ID_PARKING").arg(localisation);
    model->setQuery(sql);

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
// 🔹 TRIER PAR CAPACITÉ
// ===============================
QSqlQueryModel *Parking::trierParCapacite()
{
    // Vérifier si la connexion existe déjà
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) {
            return nullptr;
        }
    }
    
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES, STATUT, TYPE, TARIFICATION "
                    "FROM PARKING "
                    "ORDER BY CAPACITE DESC");

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
