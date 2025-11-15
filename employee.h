#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QSqlQueryModel>
#include <QMap>

class employee
{
public:
    employee();
    employee(int, QString, QString, QString, QString, int, QString, float);

    int getcin();
    QString getnom();
    QString getprenom();
    QString getemail();
    QString getmotdepass();
    int getn_telephon();
    QString getposte();
    float getsalaire();

    void setcin(int);
    void setnom(QString);
    void setprenom(QString);
    void setemail(QString);
    void setmotdepass(QString);
    void setn_telephon(int);
    void setposte(QString);
    void setsalaire(float);

    bool ajouter();
    bool modifier(int, QString, QString, QString, QString, int, QString, float);
    QSqlQueryModel *afficher();
    bool supprimer(QString);

    // Fonctions de tri et recherche
    QSqlQueryModel *trierParNom();
    QSqlQueryModel *rechercherParCIN(int cin);

    // Fonctions statistiques
    QSqlQueryModel *trierParSalaire(bool croissant = true);
    QSqlQueryModel *filtrerParPoste(QString poste);
    QMap<QString, int> statistiquesParPoste();
    QMap<QString, float> salaireMoyenParPoste();
    float salaireMoyen();
    float salaireMin();
    float salaireMax();
    int nombreEmployes();
    int nombreEmployesParPoste(QString poste);

private:
    int cin, n_telephon;
    QString nom, prenom, email, motdepass, poste;
    float salaire;
};

#endif // EMPLOYEE_H
