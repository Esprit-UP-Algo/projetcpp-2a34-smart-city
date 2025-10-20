#ifndef PARKING_H
#define PARKING_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Parking
{
private:
    // Les attributs privés (données du parking)
    int id_parking;
    QString nom;
    QString localisation;
    QString statut;
    int capacite;
    int places_libres;
    QString type;
    double tarification;

public:
    // ========== CONSTRUCTEURS ==========
    Parking();  // Constructeur par défaut
    Parking(int, QString, QString, QString, int, int, QString, double);  // Constructeur avec paramètres

    // ========== GETTERS (pour récupérer les valeurs) ==========
    int getId() const;
    QString getNom() const;
    QString getLocalisation() const;
    QString getStatut() const;
    int getCapacite() const;
    int getPlacesLibres() const;
    QString getType() const;
    double getTarification() const;

    // ========== SETTERS (pour modifier les valeurs) ==========
    void setId(int);
    void setNom(QString);
    void setLocalisation(QString);
    void setStatut(QString);
    void setCapacite(int);
    void setPlacesLibres(int);
    void setType(QString);
    void setTarification(double);

    // ========== MÉTHODES CRUD ==========
    bool ajouter();                    // Ajouter un parking dans la BD
    QSqlQueryModel* afficher();        // Afficher tous les parkings
    bool supprimer(int);               // Supprimer un parking par son ID
    bool modifier(int);                // Modifier un parking par son ID
};

#endif // PARKING_H
