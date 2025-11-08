#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlDatabase>

class employee
{
public:
    employee();
    employee(int,QString,QString,QString,QString ,int,QString,float);
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
    bool modifier(int,QString,QString,QString,QString ,int,QString,float);
    QSqlQueryModel *afficher();
    bool supprimer(QString);

private :
    int cin,n_telephon;
    QString nom,prenom,email,motdepass,poste;
    float salaire;
};

#endif // EMPLOYEE_H
