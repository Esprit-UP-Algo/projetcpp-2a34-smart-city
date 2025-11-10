#ifndef DECHET_H
#define DECHET_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class Dechet
{
public:
    Dechet();
    Dechet(int cn, const QDate& date_collecte,
           const QString& lieu, double volume_poids,
           const QString& etat);

    // ---- CRUD ----
    bool ajouter();
    bool modifier();
    static bool supprimer(int cn);
    static QSqlQueryModel* afficher();
    static QSqlQueryModel* afficherTriePar(const QString& colonne, bool asc);
    static QSqlQueryModel* rechercher(const QString& motif);

    static bool colonneValide(const QString& col);
    static QString mapColonne(const QString& col);

private:
    int m_cn;
    QDate m_dateCollecte;
    QString m_lieu;
    double m_volumePoids;
    QString m_etat;
};

#endif // DECHET_H
