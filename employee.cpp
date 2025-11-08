#include "employee.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtDebug>
#include <QObject>

employee::employee() {
    cin=0;
    nom="";
    prenom="";
    email="";
    motdepass="";
    n_telephon=0;
    poste="";
    salaire=0;
}

employee::employee(int cin,QString nom,QString prenom ,QString email ,QString motdepass,int n_telephon,QString poste,float salaire)
{
    this->cin=cin ;
    this->nom=nom ;
    this-> prenom=prenom ;
    this ->email=email;
    this ->motdepass=motdepass ;
    this->n_telephon=n_telephon;
    this->poste=poste;
    this->salaire=salaire;
}

// Implémentation des getters
int employee::getcin() {
    return cin;
}

QString employee::getnom() {
    return nom;
}

QString employee::getprenom() {
    return prenom;
}

QString employee::getemail() {
    return email;
}

QString employee::getmotdepass() {
    return motdepass;
}

int employee::getn_telephon() {
    return n_telephon;
}

QString employee::getposte() {
    return poste;
}

float employee::getsalaire() {
    return salaire;
}

// Implémentation des setters
void employee::setcin(int c) {
    cin = c;
}

void employee::setnom(QString n) {
    nom = n;
}

void employee::setprenom(QString p) {
    prenom = p;
}

void employee::setemail(QString e) {
    email = e;
}

void employee::setmotdepass(QString m) {
    motdepass = m;
}

void employee::setn_telephon(int t) {
    n_telephon = t;
}

void employee::setposte(QString p) {
    poste = p;
}

void employee::setsalaire(float s) {
    salaire = s;
}

bool employee::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO employe (CIN,NOM,PRENOM,EMAIL,MOTDEPASS,N_TELEPHON,POSTE,SALAIRE) "
                  "VALUES (:CIN, :NOM, :PRENOM, :EMAIL, :MOTDEPASS, :N_TELEPHON, :POSTE, :SALAIRE)");

    query.bindValue(":CIN", cin);
    query.bindValue(":NOM", nom);
    query.bindValue(":PRENOM", prenom);
    query.bindValue(":EMAIL", email);
    query.bindValue(":MOTDEPASS", motdepass);
    query.bindValue(":N_TELEPHON", n_telephon);
    query.bindValue(":POSTE", poste);
    query.bindValue(":SALAIRE", salaire);

    return query.exec();
}

bool employee::modifier(int cin,QString nom,QString prenom ,QString email ,QString motdepass,int n_telephon,QString poste,float salaire)
{
    QSqlQuery query;
    query.prepare("Update employe set nom = :nom , prenom = :prenom , email=:email , motdepass=:motdepass, n_telephon=:n_telephon , poste=:poste , salaire=:salaire where CIN = :cin");
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email",email);
    query.bindValue(":motdepass", motdepass);
    query.bindValue(":n_telephon", n_telephon);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);

    return query.exec();
}

QSqlQueryModel * employee::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT* FROM EMPLOYE");
    model->setHeaderData(0, Qt::Horizontal, QObject:: tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject:: tr("cin"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("motdepass"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("n_telephon"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("poste"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("salaire"));

    return model;
}

bool employee::supprimer(QString id)
{
    QSqlQuery query;
    query.prepare("delete from EMPLOYE where id_emp=:id");
    query.bindValue(":id", id);
    return query.exec();
}

// Tri par nom
QSqlQueryModel * employee::trierParNom()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM EMPLOYE ORDER BY NOM ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("motdepass"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("n_telephon"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("poste"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("salaire"));

    return model;
}

// Recherche par CIN
QSqlQueryModel * employee::rechercherParCIN(int cin)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYE WHERE CIN = :cin");
    query.bindValue(":cin", cin);
    query.exec();

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("motdepass"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("n_telephon"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("poste"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("salaire"));

    return model;
}

// ============================================
// NOUVELLES FONCTIONS STATISTIQUES
// ============================================

// Tri par salaire (croissant ou décroissant)
QSqlQueryModel * employee::trierParSalaire(bool croissant)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString ordre = croissant ? "ASC" : "DESC";
    model->setQuery("SELECT * FROM EMPLOYE ORDER BY SALAIRE " + ordre);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("motdepass"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("n_telephon"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("poste"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("salaire"));
    return model;
}

// Filtrer par poste
QSqlQueryModel * employee::filtrerParPoste(QString poste)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYE WHERE POSTE = :poste ORDER BY SALAIRE DESC");
    query.bindValue(":poste", poste);
    query.exec();

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("motdepass"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("n_telephon"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("poste"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("salaire"));
    return model;
}

// Statistiques : Nombre d'employés par poste
QMap<QString, int> employee::statistiquesParPoste()
{
    QMap<QString, int> stats;
    QSqlQuery query("SELECT POSTE, COUNT(*) as nombre FROM EMPLOYE GROUP BY POSTE");

    while (query.next()) {
        QString poste = query.value(0).toString();
        int nombre = query.value(1).toInt();
        stats[poste] = nombre;
    }

    return stats;
}

// Salaire moyen par poste
QMap<QString, float> employee::salaireMoyenParPoste()
{
    QMap<QString, float> stats;
    QSqlQuery query("SELECT POSTE, AVG(SALAIRE) as moyenne FROM EMPLOYE GROUP BY POSTE");

    while (query.next()) {
        QString poste = query.value(0).toString();
        float moyenne = query.value(1).toFloat();
        stats[poste] = moyenne;
    }

    return stats;
}

// Salaire moyen global
float employee::salaireMoyen()
{
    QSqlQuery query("SELECT AVG(SALAIRE) FROM EMPLOYE");
    if (query.next()) {
        return query.value(0).toFloat();
    }
    return 0.0;
}

// Salaire minimum
float employee::salaireMin()
{
    QSqlQuery query("SELECT MIN(SALAIRE) FROM EMPLOYE");
    if (query.next()) {
        return query.value(0).toFloat();
    }
    return 0.0;
}

// Salaire maximum
float employee::salaireMax()
{
    QSqlQuery query("SELECT MAX(SALAIRE) FROM EMPLOYE");
    if (query.next()) {
        return query.value(0).toFloat();
    }
    return 0.0;
}

// Nombre total d'employés
int employee::nombreEmployes()
{
    QSqlQuery query("SELECT COUNT(*) FROM EMPLOYE");
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

// Nombre d'employés par poste
int employee::nombreEmployesParPoste(QString poste)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM EMPLOYE WHERE POSTE = :poste");
    query.bindValue(":poste", poste);
    query.exec();

    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}
