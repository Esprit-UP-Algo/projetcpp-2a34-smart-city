#include "dechet.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
Dechet::Dechet()
{
    cin = 0;
    volume_poids = 0;
    etat = "";
}

Dechet::Dechet(int cin, QDate date_collecte, QString lieu, float vol, QString etat)
{
    this->cin = cin;
    this->date_collecte = date_collecte;
    this->lieu_collecte = lieu;
    this->volume_poids = vol;
    this->etat = etat;
}

// Getters
int Dechet::getCin() const { return cin; }
QDate Dechet::getDateCollecte() const { return date_collecte; }
QString Dechet::getLieuCollecte() const { return lieu_collecte; }
float Dechet::getVolume() const { return volume_poids; }
QString Dechet::getEtat() const { return etat; }

// Setters
void Dechet::setCin(int c) { cin = c; }
void Dechet::setDateCollecte(QDate d) { date_collecte = d; }
void Dechet::setLieuCollecte(QString l) { lieu_collecte = l; }
void Dechet::setVolume(float v) { volume_poids = v; }
void Dechet::setEtat(QString e) { etat = e; }

// Ajouter
bool Dechet::ajouter()
{
    QSqlQuery q;
    q.prepare("INSERT INTO DECHETS (CIN, DATE_COLLECTE, LIEU_COLLECTE, VOLUME_POIDS, ETAT) "
              "VALUES (:cin, :datec, :lieu, :vol, :etat)");

    q.bindValue(":cin", cin);
    q.bindValue(":datec", date_collecte);
    q.bindValue(":lieu", lieu_collecte);
    q.bindValue(":vol", volume_poids);
    q.bindValue(":etat", etat);
    if (!q.exec()) {
        qDebug() << "SQL Error:" << q.lastError().text();
        return false;
    }
    return true;
}

// Modifier
bool Dechet::modifier()
{
    QSqlQuery q;
    q.prepare("UPDATE DECHETS SET DATE_COLLECTE=:datec, LIEU_COLLECTE=:lieu, "
              "VOLUME_POIDS=:vol, ETAT=:etat WHERE CIN=:cin");

    q.bindValue(":cin", cin);
    q.bindValue(":datec", date_collecte);
    q.bindValue(":lieu", lieu_collecte);
    q.bindValue(":vol", volume_poids);
    q.bindValue(":etat", etat);
    if (!q.exec()) {
        qDebug() << "SQL Error:" << q.lastError().text();
        return false;
    }
    return true;

}

// Supprimer
bool Dechet::supprimer(int cin)
{
    QSqlQuery q;
    q.prepare("DELETE FROM DECHETS WHERE CIN=:cin");
    q.bindValue(":cin", cin);
    if (!q.exec()) {
        qDebug() << "SQL Error:" << q.lastError().text();
        return false;
    }
    return true;

}

// Afficher
QSqlQueryModel* Dechet::afficher()
{
    QSqlQueryModel *m = new QSqlQueryModel();
    m->setQuery("SELECT * FROM DECHETS");
    return m;
}

// Rechercher
QSqlQueryModel* Dechet::rechercher(QString critere, QString valeur)
{
    QSqlQueryModel* m = new QSqlQueryModel();

    QString req;

    if (critere == "CIN")
        req = "SELECT * FROM DECHETS WHERE TO_CHAR(CIN) LIKE '%" + valeur + "%'";
    else if (critere == "LIEU_COLLECTE")
        req = "SELECT * FROM DECHETS WHERE LIEU_COLLECTE LIKE '%" + valeur + "%'";

    m->setQuery(req);
    return m;
}



// Trier
QSqlQueryModel* Dechet::trier(QString critere)
{
    QSqlQueryModel* m = new QSqlQueryModel();

    QString req =
        "SELECT CIN, DATE_COLLECTE, LIEU_COLLECTE, VOLUME_POIDS, ETAT "
        "FROM DECHETS ORDER BY " + critere + " ASC";

    m->setQuery(req);
    return m;
}

