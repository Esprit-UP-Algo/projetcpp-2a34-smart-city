#include "employee.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
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

// Getters
int employee::getcin() { return cin; }
QString employee::getnom() { return nom; }
QString employee::getprenom() { return prenom; }
QString employee::getemail() { return email; }
QString employee::getmotdepass() { return motdepass; }
int employee::getn_telephon() { return n_telephon; }
QString employee::getposte() { return poste; }
float employee::getsalaire() { return salaire; }

// Setters
void employee::setcin(int c) { cin = c; }
void employee::setnom(QString n) { nom = n; }
void employee::setprenom(QString p) { prenom = p; }
void employee::setemail(QString e) { email = e; }
void employee::setmotdepass(QString m) { motdepass = m; }
void employee::setn_telephon(int t) { n_telephon = t; }
void employee::setposte(QString p) { poste = p; }
void employee::setsalaire(float s) { salaire = s; }

bool employee::ajouter()
{
    QSqlQuery query;
    // Utiliser le même nom de table que dans afficher() : EMPLOYE en majuscules
    query.prepare("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, EMAIL, MOTDEPASS, N_TELEPHON, POSTE, SALAIRE) "
                  "VALUES (:CIN, :NOM, :PRENOM, :EMAIL, :MOTDEPASS, :N_TELEPHON, :POSTE, :SALAIRE)");

    query.bindValue(":CIN", cin);
    query.bindValue(":NOM", nom);
    query.bindValue(":PRENOM", prenom);
    query.bindValue(":EMAIL", email);
    query.bindValue(":MOTDEPASS", motdepass);
    query.bindValue(":N_TELEPHON", n_telephon);
    query.bindValue(":POSTE", poste);
    query.bindValue(":SALAIRE", salaire);

    // Afficher l'erreur si la requête échoue
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout de l'employé:";
        qDebug() << "Message d'erreur:" << query.lastError().text();
        qDebug() << "Requête:" << query.lastQuery();
        qDebug() << "Valeurs:";
        qDebug() << "  CIN:" << cin;
        qDebug() << "  NOM:" << nom;
        qDebug() << "  PRENOM:" << prenom;
        qDebug() << "  EMAIL:" << email;
        qDebug() << "  N_TELEPHON:" << n_telephon;
        qDebug() << "  POSTE:" << poste;
        qDebug() << "  SALAIRE:" << salaire;
        return false;
    }

    return true;
}

bool employee::modifier(int cin,QString nom,QString prenom ,QString email ,QString motdepass,int n_telephon,QString poste,float salaire)
{
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYE SET NOM = :nom, PRENOM = :prenom, EMAIL = :email, "
                  "MOTDEPASS = :motdepass, N_TELEPHON = :n_telephon, POSTE = :poste, "
                  "SALAIRE = :salaire WHERE CIN = :cin");
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":motdepass", motdepass);
    query.bindValue(":n_telephon", n_telephon);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification:" << query.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel * employee::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM EMPLOYE");
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

bool employee::supprimer(QString id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYE WHERE id_emp = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression:" << query.lastError().text();
        return false;
    }

    return true;
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

// Fonctions statistiques
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

float employee::salaireMoyen()
{
    QSqlQuery query("SELECT AVG(SALAIRE) FROM EMPLOYE");
    if (query.next()) {
        return query.value(0).toFloat();
    }
    return 0.0;
}

float employee::salaireMin()
{
    QSqlQuery query("SELECT MIN(SALAIRE) FROM EMPLOYE");
    if (query.next()) {
        return query.value(0).toFloat();
    }
    return 0.0;
}

float employee::salaireMax()
{
    QSqlQuery query("SELECT MAX(SALAIRE) FROM EMPLOYE");
    if (query.next()) {
        return query.value(0).toFloat();
    }
    return 0.0;
}

int employee::nombreEmployes()
{
    QSqlQuery query("SELECT COUNT(*) FROM EMPLOYE");
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}
