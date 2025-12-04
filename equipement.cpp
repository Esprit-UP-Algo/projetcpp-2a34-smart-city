#include "equipement.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QObject>

Equipement::Equipement()
    : id(0),
      consommation(0.0)
{
}

Equipement::Equipement(int id, const QString &nom, const QString &typeEquipement,
                       const QString &zone, const QString &etat,
                       const QDate &dateInstallation, double consommation)
    : id(id),
      nom(nom),
      typeEquipement(typeEquipement),
      zone(zone),
      etat(etat),
      dateInstallation(dateInstallation),
      consommation(consommation)
{
}

bool Equipement::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO EQUIPEMENT (ID, NOM, TYPE_EQUIPEMENT, ZONE, ETAT, DATE_INSTALLATION, CONSOMMATION) "
                  "VALUES (:id, :nom, :type, :zone, :etat, :date_installation, :consommation)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":type", typeEquipement);
    query.bindValue(":zone", zone);
    query.bindValue(":etat", etat);
    query.bindValue(":date_installation", dateInstallation);
    query.bindValue(":consommation", consommation);

    if (!query.exec()) {
        qDebug() << "Erreur d'ajout d'équipement:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Equipement::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE EQUIPEMENT "
                  "SET NOM = :nom, "
                  "    TYPE_EQUIPEMENT = :type, "
                  "    ZONE = :zone, "
                  "    ETAT = :etat, "
                  "    DATE_INSTALLATION = :date_installation, "
                  "    CONSOMMATION = :consommation "
                  "WHERE ID = :id");

    query.bindValue(":nom", nom);
    query.bindValue(":type", typeEquipement);
    query.bindValue(":zone", zone);
    query.bindValue(":etat", etat);
    query.bindValue(":date_installation", dateInstallation);
    query.bindValue(":consommation", consommation);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur de modification d'équipement:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool Equipement::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EQUIPEMENT WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur de suppression d'équipement:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

QSqlQueryModel* Equipement::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID, NOM, TYPE_EQUIPEMENT, ZONE, ETAT, DATE_INSTALLATION, CONSOMMATION "
                    "FROM EQUIPEMENT ORDER BY ID");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur d'affichage des équipements:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Zone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date d'installation"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Consommation"));

    return model;
}

QSqlQueryModel* Equipement::trierParNom(bool ascending)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString order = ascending ? "ASC" : "DESC";
    QString queryStr = QString("SELECT ID, NOM, TYPE_EQUIPEMENT, ZONE, ETAT, DATE_INSTALLATION, CONSOMMATION "
                               "FROM EQUIPEMENT ORDER BY NOM %1").arg(order);
    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        qDebug() << "Erreur de tri par nom:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Zone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date d'installation"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Consommation"));

    return model;
}

QSqlQueryModel* Equipement::trierParConsommation(bool ascending)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString order = ascending ? "ASC" : "DESC";
    QString queryStr = QString("SELECT ID, NOM, TYPE_EQUIPEMENT, ZONE, ETAT, DATE_INSTALLATION, CONSOMMATION "
                               "FROM EQUIPEMENT ORDER BY CONSOMMATION %1").arg(order);
    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        qDebug() << "Erreur de tri par consommation:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Zone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date d'installation"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Consommation"));

    return model;
}

QSqlQueryModel* Equipement::trierParDate(bool ascending)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString order = ascending ? "ASC" : "DESC";
    QString queryStr = QString("SELECT ID, NOM, TYPE_EQUIPEMENT, ZONE, ETAT, DATE_INSTALLATION, CONSOMMATION "
                               "FROM EQUIPEMENT ORDER BY DATE_INSTALLATION %1").arg(order);
    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        qDebug() << "Erreur de tri par date:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Zone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date d'installation"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Consommation"));

    return model;
}
