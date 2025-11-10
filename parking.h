#ifndef PARKING_H
#define PARKING_H

#include <QString>
#include <QSqlQueryModel>

class Parking
{
public:
    Parking();
    Parking(QString, QString, int, int, QString, QString, double);

    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimerParId(QString);
    bool modifierParId(QString);

private:
    QString id;
    QString localisation;
    int capacite;
    int placeslibres;
    QString statut;
    QString type;
    double tarif;
};

#endif // PARKING_H
