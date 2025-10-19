#ifndef ALERTE_H
#define ALERTE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Alerte {
private:
    int id;
    QString type;
    int id_client;
    QString statut;
    QString description;
    QString gravite;
    QDate date;

public:
    // Constructeurs
    Alerte();
    Alerte(int, QString, int, QString, QString, QString, QDate);

    // Getters
    int getId();
    QString getType();
    int getIdClient();
    QString getStatut();
    QString getDescription();
    QString getGravite();
    QDate getDate();

    // Setters
    void setId(int);
    void setType(QString);
    void setIdClient(int);
    void setStatut(QString);
    void setDescription(QString);
    void setGravite(QString);
    void setDate(QDate);

    // CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier();
};

#endif
