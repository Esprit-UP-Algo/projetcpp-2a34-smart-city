#include "equipement.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QObject>

Equipement::Equipement()
    : id(""), consommation("0.0")
{
}

Equipement::Equipement(QString id, QString nom, QString type, QString zone, QString etat, QDate dateEQ, QString consommation)
    : id(id), nom(nom), type(type), zone(zone), etat(etat), dateEQ(dateEQ), consommation(consommation)
{
}

bool Equipement::ajouter()
{
    QSqlQuery query;

    qDebug() << "========================================";
    qDebug() << "TENTATIVE D'AJOUT D'ÉQUIPEMENT";
    qDebug() << "ID:" << id << "| NOM:" << nom << "| CONSO:" << consommation;
    qDebug() << "TYPE:" << type << "| ZONE:" << zone << "| ETAT:" << etat;
    qDebug() << "DATE_EQ:" << dateEQ.toString("yyyy-MM-dd") << "| ID_EMP:" << (idEmp.isEmpty() ? "NULL" : idEmp);
    qDebug() << "========================================";

    // Vérifier d'abord si l'ID existe déjà
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM EQUIPEMENT WHERE ID_EQ = :id");
    checkQuery.bindValue(":id", id);

    if (checkQuery.exec() && checkQuery.next()) {
        int count = checkQuery.value(0).toInt();
        if (count > 0) {
            qDebug() << "❌ ERREUR: ID déjà existant!";
            return false;
        }
    }

    // Vérifier si l'ID_EMP existe dans la table EMPLOYE (contrainte de clé étrangère)
    if (!idEmp.isEmpty()) {
        QSqlQuery checkEmpQuery;
        checkEmpQuery.prepare("SELECT COUNT(*) FROM EMPLOYE WHERE ID_EMP = :idEmp");
        checkEmpQuery.bindValue(":idEmp", idEmp);

        if (checkEmpQuery.exec() && checkEmpQuery.next()) {
            int empCount = checkEmpQuery.value(0).toInt();
            if (empCount == 0) {
                qDebug() << "❌ ERREUR: ID Employé non trouvé dans la table EMPLOYE!";
                qDebug() << "ID Employé fourni:" << idEmp;
                return false;
            }
        }
    }

    // CORRECTION: Utiliser DATE_EQ au lieu de DATE_FQ
    query.prepare("INSERT INTO EQUIPEMENT (ID_EQ, NOM, CONSOMMATION, ID_EMP, TYPE, ZONE, ETAT, DATE_EQ) "
                  "VALUES (:id, :nom, :consommation, :idEmp, :type, :zone, :etat, :dateEQ)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":consommation", consommation);

    // Gestion propre des IDs employés vides
    if (idEmp.isEmpty()) {
        query.bindValue(":idEmp", QVariant(QVariant::String));
    } else {
        query.bindValue(":idEmp", idEmp);
    }

    query.bindValue(":type", type.isEmpty() ? QVariant(QVariant::String) : type);
    query.bindValue(":zone", zone.isEmpty() ? QVariant(QVariant::String) : zone);
    query.bindValue(":etat", etat.isEmpty() ? QVariant(QVariant::String) : etat);
    query.bindValue(":dateEQ", dateEQ.isValid() ? dateEQ : QVariant(QVariant::Date));

    if (query.exec()) {
        qDebug() << "✅ SUCCÈS! Équipement ajouté";
        return true;
    }

    qDebug() << "❌ ÉCHEC:" << query.lastError().text();
    qDebug() << "Code erreur Oracle:" << query.lastError().nativeErrorCode();
    return false;
}

bool Equipement::modifier()
{
    QSqlQuery query;
    // CORRECTION: DATE_EQ (pas DATE_FQ)
    query.prepare("UPDATE EQUIPEMENT SET NOM = :nom, CONSOMMATION = :consommation, "
                  "ID_EMP = :idEmp, TYPE = :type, ZONE = :zone, ETAT = :etat, DATE_EQ = :dateEQ "
                  "WHERE ID_EQ = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":consommation", consommation);
    query.bindValue(":idEmp", idEmp.isEmpty() ? QVariant(QVariant::String) : idEmp);
    query.bindValue(":type", type.isEmpty() ? QVariant(QVariant::String) : type);
    query.bindValue(":zone", zone.isEmpty() ? QVariant(QVariant::String) : zone);
    query.bindValue(":etat", etat.isEmpty() ? QVariant(QVariant::String) : etat);
    query.bindValue(":dateEQ", dateEQ.isValid() ? dateEQ : QVariant(QVariant::Date));

    if (!query.exec()) {
        qDebug() << "Erreur de modification d'équipement:" << query.lastError().text();
        return false;
    }

    int rowsAffected = query.numRowsAffected();
    if (rowsAffected == 0) {
        qDebug() << "Aucun équipement trouvé avec l'ID:" << id;
        return false;
    }

    qDebug() << "✓ Équipement modifié avec succès! ID:" << id;
    return true;
}

bool Equipement::supprimer(QString id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EQUIPEMENT WHERE ID_EQ = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur de suppression d'équipement:" << query.lastError().text();
        return false;
    }

    int rowsAffected = query.numRowsAffected();
    if (rowsAffected == 0) {
        qDebug() << "Aucun équipement trouvé avec l'ID:" << id;
        return false;
    }

    qDebug() << "✓ Équipement supprimé avec succès! ID:" << id;
    return true;
}

QSqlQueryModel* Equipement::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    // CORRECTION: DATE_EQ (pas DATE_FQ)
    model->setQuery("SELECT ID_EQ, NOM, CONSOMMATION, ID_EMP, TYPE, ZONE, ETAT, TO_CHAR(DATE_EQ, 'YYYY-MM-DD') "
                    "FROM EQUIPEMENT ORDER BY ID_EQ");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur d'affichage des équipements:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Consommation"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Zone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date"));

    qDebug() << "Nombre d'équipements affichés:" << model->rowCount();
    return model;
}

QSqlQueryModel* Equipement::trierParNom(bool ascending)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString order = ascending ? "ASC" : "DESC";
    // CORRECTION: DATE_EQ (pas DATE_FQ)
    QString queryStr = "SELECT ID_EQ, NOM, CONSOMMATION, ID_EMP, TYPE, ZONE, ETAT, TO_CHAR(DATE_EQ, 'YYYY-MM-DD') "
                       "FROM EQUIPEMENT ORDER BY NOM " + order;

    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        qDebug() << "Erreur de tri par nom:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Consommation"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Zone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date"));
    return model;
}

QSqlQueryModel* Equipement::trierParConsommation(bool ascending)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString order = ascending ? "ASC" : "DESC";
    // CORRECTION: DATE_EQ (pas DATE_FQ)
    QString queryStr = "SELECT ID_EQ, NOM, CONSOMMATION, ID_EMP, TYPE, ZONE, ETAT, TO_CHAR(DATE_EQ, 'YYYY-MM-DD') "
                       "FROM EQUIPEMENT ORDER BY TO_NUMBER(CONSOMMATION) " + order;

    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        qDebug() << "Erreur de tri par consommation:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Consommation"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Zone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date"));
    return model;
}

QSqlQueryModel* Equipement::trierParDate(bool ascending)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString order = ascending ? "ASC" : "DESC";
    // CORRECTION: DATE_EQ (pas DATE_FQ)
    QString queryStr = "SELECT ID_EQ, NOM, CONSOMMATION, ID_EMP, TYPE, ZONE, ETAT, TO_CHAR(DATE_EQ, 'YYYY-MM-DD') "
                       "FROM EQUIPEMENT ORDER BY DATE_EQ " + order;

    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        qDebug() << "Erreur de tri par date:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Consommation"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Zone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date"));
    return model;
}
