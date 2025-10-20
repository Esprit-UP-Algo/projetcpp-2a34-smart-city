#include "parking.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// ========== CONSTRUCTEUR PAR DÉFAUT ==========
Parking::Parking()
{
    id_parking = 0;
    nom = "";
    localisation = "";
    statut = "";
    capacite = 0;
    places_libres = 0;
    type = "";
    tarification = 0.0;
}

// ========== CONSTRUCTEUR AVEC PARAMÈTRES ==========
Parking::Parking(int id, QString n, QString loc, QString st, int cap, int pl, QString t, double tarif)
{
    id_parking = id;
    nom = n;
    localisation = loc;
    statut = st;
    capacite = cap;
    places_libres = pl;
    type = t;
    tarification = tarif;
}

// ========== GETTERS ==========
int Parking::getId() const { return id_parking; }
QString Parking::getNom() const { return nom; }
QString Parking::getLocalisation() const { return localisation; }
QString Parking::getStatut() const { return statut; }
int Parking::getCapacite() const { return capacite; }
int Parking::getPlacesLibres() const { return places_libres; }
QString Parking::getType() const { return type; }
double Parking::getTarification() const { return tarification; }

// ========== SETTERS ==========
void Parking::setId(int id) { id_parking = id; }
void Parking::setNom(QString n) { nom = n; }
void Parking::setLocalisation(QString loc) { localisation = loc; }
void Parking::setStatut(QString st) { statut = st; }
void Parking::setCapacite(int cap) { capacite = cap; }
void Parking::setPlacesLibres(int pl) { places_libres = pl; }
void Parking::setType(QString t) { type = t; }
void Parking::setTarification(double tarif) { tarification = tarif; }

// ========== MÉTHODE AJOUTER ==========
bool Parking::ajouter()
{
    QSqlQuery query;

    // Préparer la requête SQL INSERT
    query.prepare("INSERT INTO PARKING (id_parking, nom, localisation, statut, capacite, places_libres, type, tarification) "
                  "VALUES (:id, :nom, :loc, :stat, :cap, :pl, :type, :tarif)");

    // Lier les valeurs aux paramètres
    query.bindValue(":id", id_parking);
    query.bindValue(":nom", nom);
    query.bindValue(":loc", localisation);
    query.bindValue(":stat", statut);
    query.bindValue(":cap", capacite);
    query.bindValue(":pl", places_libres);
    query.bindValue(":type", type);
    query.bindValue(":tarif", tarification);

    // Exécuter la requête et retourner le résultat
    if(query.exec())
    {
        qDebug() << "Parking ajouté avec succès!";
        return true;
    }
    else
    {
        qDebug() << "Erreur lors de l'ajout:" << query.lastError().text();
        return false;
    }
}

// ========== MÉTHODE AFFICHER ==========
QSqlQueryModel* Parking::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    // Exécuter la requête SELECT
    model->setQuery("SELECT * FROM PARKING ORDER BY id_parking");

    // Définir les en-têtes des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Localisation"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Capacité"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Places Libres"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Tarification"));

    return model;
}

// ========== MÉTHODE SUPPRIMER ==========
bool Parking::supprimer(int id)
{
    QSqlQuery query;

    // Préparer la requête DELETE
    query.prepare("DELETE FROM PARKING WHERE id_parking = :id");
    query.bindValue(":id", id);

    // Exécuter et retourner le résultat
    if(query.exec())
    {
        qDebug() << "Parking supprimé avec succès!";
        return true;
    }
    else
    {
        qDebug() << "Erreur lors de la suppression:" << query.lastError().text();
        return false;
    }
}

// ========== MÉTHODE MODIFIER ==========
bool Parking::modifier(int id)
{
    QSqlQuery query;

    // Préparer la requête UPDATE
    query.prepare("UPDATE PARKING SET nom=:nom, localisation=:loc, statut=:stat, "
                  "capacite=:cap, places_libres=:pl, type=:type, tarification=:tarif "
                  "WHERE id_parking=:id");

    // Lier les nouvelles valeurs
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":loc", localisation);
    query.bindValue(":stat", statut);
    query.bindValue(":cap", capacite);
    query.bindValue(":pl", places_libres);
    query.bindValue(":type", type);
    query.bindValue(":tarif", tarification);

    // Exécuter et retourner le résultat
    if(query.exec())
    {
        qDebug() << "Parking modifié avec succès!";
        return true;
    }
    else
    {
        qDebug() << "Erreur lors de la modification:" << query.lastError().text();
        return false;
    }
}
