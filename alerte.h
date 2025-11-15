#ifndef ALERTE_H
#define ALERTE_H

#include <QString>
#include <QTableWidget>

class alerte
{
public:
    alerte();
    alerte(int id, QString type, QString client, QString statut,
           QString description, QString gravite, QString datee = "");

    // Getters
    int getId() const { return id; }
    QString getType() const { return type; }
    QString getClient() const { return client; }
    QString getStatut() const { return statut; }
    QString getDescription() const { return description; }
    QString getGravite() const { return gravite; }
    QString getDatee() const { return datee; }

    // CRUD
    bool ajouter();
    bool modifier();
    bool supprimer(int id);
    void afficherTable(QTableWidget *table);

private:
    int id;
    QString type, client, statut, description, gravite, datee;
};

#endif // ALERTE_H
