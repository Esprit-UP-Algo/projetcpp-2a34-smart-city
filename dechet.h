#ifndef DECHET_H
#define DECHET_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>

class Dechet
{
private:
    int cin;
    QDate date_collecte;
    QString lieu_collecte;
    float volume_poids;
    QString etat;

public:
    Dechet();
    Dechet(int cin, QDate date_collecte, QString lieu, float vol, QString etat);

    // Getters
    int getCin() const;
    QDate getDateCollecte() const;
    QString getLieuCollecte() const;
    float getVolume() const;
    QString getEtat() const;

    // Setters
    void setCin(int c);
    void setDateCollecte(QDate d);
    void setLieuCollecte(QString l);
    void setVolume(float v);
    void setEtat(QString e);

    // CRUD
    bool ajouter();
    bool modifier();
    bool supprimer(int cin);
    QSqlQueryModel* afficher();

    // Recherche / Tri
    QSqlQueryModel* rechercher(QString critere, QString valeur);
    QSqlQueryModel* trier(QString critere);
};

#endif // DECHET_H
