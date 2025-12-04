#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class Equipement
{
private:
    int id;
    QString nom;
    QString typeEquipement;
    QString zone;
    QString etat;
    QDate dateInstallation;
    double consommation;

public:
    Equipement();
    Equipement(int id, const QString &nom, const QString &typeEquipement,
               const QString &zone, const QString &etat,
               const QDate &dateInstallation, double consommation);

    // Getters
    int getId() const { return id; }
    QString getNom() const { return nom; }
    QString getType() const { return typeEquipement; }
    QString getZone() const { return zone; }
    QString getEtat() const { return etat; }
    QDate getDateInstallation() const { return dateInstallation; }
    double getConsommation() const { return consommation; }

    // Setters
    void setId(int newId) { id = newId; }
    void setNom(const QString &newNom) { nom = newNom; }
    void setType(const QString &newType) { typeEquipement = newType; }
    void setZone(const QString &newZone) { zone = newZone; }
    void setEtat(const QString &newEtat) { etat = newEtat; }
    void setDateInstallation(const QDate &date) { dateInstallation = date; }
    void setConsommation(double conso) { consommation = conso; }

    // Méthodes CRUD
    bool ajouter();
    bool modifier();
    static bool supprimer(int id);
    static QSqlQueryModel* afficher();

    // Méthodes de tri
    static QSqlQueryModel* trierParNom(bool ascending = true);
    static QSqlQueryModel* trierParDate(bool ascending = true);
    static QSqlQueryModel* trierParConsommation(bool ascending = true);
};

#endif // EQUIPEMENT_H
