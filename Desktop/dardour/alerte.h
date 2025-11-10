#ifndef ALERTE_H
#define ALERTE_H
#include <QString>
#include <QTableWidget>

class alerte
{
public:
    // Constructeurs
    alerte() {}
    alerte(QString id, QString type, QString client, QString statut,
           QString description, QString gravite, QString datee);

    // Getters
    QString getId() const { return id; }
    QString getType() const { return type; }
    QString getClient() const { return client; }
    QString getStatut() const { return statut; }
    QString getDescription() const { return description; }
    QString getGravite() const { return gravite; }
    QString getDatee() const { return datee; }

    // Setters
    void setId(QString i) { id = i; }
    void setType(QString t) { type = t; }
    void setClient(QString c) { client = c; }
    void setStatut(QString s) { statut = s; }
    void setDescription(QString d) { description = d; }
    void setGravite(QString g) { gravite = g; }
    void setDatee(QString d) { datee = d; }

    // Fonctions CRUD
    bool ajouter();
    bool supprimer(int id);
    bool modifier(int id);
    void afficherTable(QTableWidget* table);

private:
    QString id, type, client, statut, description, gravite, datee;
};

#endif // ALERTE_H
