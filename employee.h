#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QSqlQueryModel>
#include <QMap>
#include <QDate>
#include <QImage>

class employee
{
public:
    employee();
    employee(int, QString, QString, QString, QString, int, QString, float, QDate dateEmbauche = QDate::currentDate());
    employee(int, QString, QString, QString, QString, int, QString, float, QDate dateEmbauche, QString faceImagePath);

    // Getters existants
    int getcin();
    QString getnom();
    QString getprenom();
    QString getemail();
    QString getmotdepass();
    int getn_telephon();
    QString getposte();
    float getsalaire();
    QDate getDateEmbauche();
    QString getFaceImagePath();

    // Setters existants
    void setcin(int);
    void setnom(QString);
    void setprenom(QString);
    void setemail(QString);
    void setmotdepass(QString);
    void setn_telephon(int);
    void setposte(QString);
    void setsalaire(float);
    void setDateEmbauche(QDate);
    void setFaceImagePath(QString);

    // Méthodes existantes
    bool ajouter();
    bool ajouterAvecFace(const QImage &faceImage);
    bool modifier(int, QString, QString, QString, QString, int, QString, float, QDate dateEmbauche = QDate::currentDate());
    QSqlQueryModel *afficher();
    bool supprimer(QString);

    // Méthodes statiques pour la reconnaissance faciale
    static QImage capturerVisage(QWidget *parent = nullptr);
    static bool ajouterEmployeAvecFace(employee &emp, QWidget *parent = nullptr);

    // Fonctions de tri et recherche
    QSqlQueryModel *trierParNom(bool croissant = true);
    QSqlQueryModel *trierParSalaire(bool croissant = true);
    QSqlQueryModel *trierParPoste(bool croissant = true);
    QSqlQueryModel *trierParDateEmbauche(bool croissant = true);
    QSqlQueryModel *rechercherParCIN(int cin);
    QSqlQueryModel *filtrerParPoste(QString poste);

    // Fonctions statistiques
    QMap<QString, int> statistiquesParPoste();
    QMap<QString, float> salaireMoyenParPoste();
    QMap<QString, int> statistiquesEmbaucheParMois();
    float salaireMoyen();
    float salaireMin();
    float salaireMax();
    int nombreEmployes();
    int nombreEmployesParPoste(QString poste);

    // Méthodes statiques pour la récupération de mot de passe
    static bool verifierEmailExiste(const QString &email);
    static bool reinitialiserMotDePasse(const QString &email);
    static bool changerMotDePasse(const QString &email, const QString &nouveauMotDePasse);
    static QString getTelephoneParEmail(const QString &email);

    // Méthodes pour générer l'attestation de travail
    static bool genererAttestationTravail(int cinEmploye, const QString &nom, const QString &prenom,
                                          const QString &poste, float salaire, const QDate &dateEmbauche,
                                          const QString &cheminFichier);
    static bool genererAttestationSansDateEmbauche(int cinEmploye, const QString &nom, const QString &prenom,
                                                   const QString &poste, float salaire, const QString &cheminFichier);

    // Méthode pour ajouter la colonne DATE_EMBAUCHE
    static bool ajouterColonneDateEmbauche();
    
    // Méthodes pour la gestion RFID
    static bool ajouterColonneRFID();
    static bool verifierRFIDUID(const QString &rfid_uid);
    static bool associerRFIDAEmploye(int cin, const QString &rfid_uid);
    static QString getEmployeNomByRFID(const QString &rfid_uid);

private:
    int cin, n_telephon;
    QString nom, prenom, email, motdepass, poste, faceImagePath;
    float salaire;
    QDate dateEmbauche;
    
    // Méthodes privées pour la gestion des images
    QString generateFaceImagePath();
    bool saveFaceImage(const QImage &image);
};

#endif // EMPLOYEE_H
