#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class Equipement
{
private:
    QString id;
    QString nom;
    QString type;
    QString zone;
    QString etat;
    QDate dateEQ;
    QString consommation;
    QString idEmp;

public:
    Equipement();
    Equipement(QString id, QString nom, QString type, QString zone, QString etat, QDate dateEQ, QString consommation);

    // Getters
    QString getId() const { return id; }
    QString getNom() const { return nom; }
    QString getType() const { return type; }
    QString getZone() const { return zone; }
    QString getEtat() const { return etat; }
    QDate getDateEQ() const { return dateEQ; }
    QString getConsommation() const { return consommation; }
    QString getIdEmp() const { return idEmp; }

    // Setters
    void setId(QString id) { this->id = id; }
    void setNom(QString nom) { this->nom = nom; }
    void setType(QString type) { this->type = type; }
    void setZone(QString zone) { this->zone = zone; }
    void setEtat(QString etat) { this->etat = etat; }
    void setDateEQ(QDate date) { this->dateEQ = date; }
    void setConsommation(QString conso) { this->consommation = conso; }
    void setIdEmp(QString idEmp) { this->idEmp = idEmp; }

    // Méthodes CRUD
    bool ajouter();
    bool modifier();
    static bool supprimer(QString id);
    static QSqlQueryModel* afficher();

    // Méthodes de tri
    static QSqlQueryModel* trierParNom(bool ascending = true);
    static QSqlQueryModel* trierParDate(bool ascending = true);
    static QSqlQueryModel* trierParConsommation(bool ascending = true);
};

#endif // EQUIPEMENT_H
