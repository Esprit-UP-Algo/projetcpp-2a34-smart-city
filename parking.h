#ifndef PARKING_H
#define PARKING_H

#include <QString>
#include <QSqlQueryModel>

class Parking
{
private:
    QString id;
    QString nom;
    QString localisation;
    int capacite;
    int placeslibres;
    QString statut;
    QString type;
    double tarif;

public:
    // Constructeurs
    Parking();
    Parking(QString id, QString localisation, int capacite, int placeslibres,
            QString statut, QString type, double tarif);

    // Getters
    QString getId() { return id; }
    QString getNom() { return nom; }
    QString getLocalisation() { return localisation; }
    int getCapacite() { return capacite; }
    int getPlaceslibres() { return placeslibres; }
    QString getStatut() { return statut; }
    QString getType() { return type; }
    double getTarif() { return tarif; }

    // Setters
    void setId(QString id) { this->id = id; }
    void setNom(QString nom) { this->nom = nom; }
    void setLocalisation(QString localisation) { this->localisation = localisation; }
    void setCapacite(int capacite) { this->capacite = capacite; }
    void setPlaceslibres(int placeslibres) { this->placeslibres = placeslibres; }
    void setStatut(QString statut) { this->statut = statut; }
    void setType(QString type) { this->type = type; }
    void setTarif(double tarif) { this->tarif = tarif; }

    // Méthodes CRUD
    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimerParId(QString id);
    bool modifierParId(QString id);
};

#endif // PARKING_H
