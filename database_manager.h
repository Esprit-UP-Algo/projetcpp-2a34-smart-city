#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QObject>
#include <QString>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);

    // Fonctions de gestion des employés
    void afficherTousLesEmployes();
    bool ajouterEmploye(const QString &cin, const QString &nom, const QString &prenom, 
                       const QString &email, const QString &motDePasse, const QString &poste);
    bool modifierMotDePasse(const QString &email, const QString &nouveauMotDePasse);
    bool supprimerEmploye(const QString &email);
    
    // Fonctions de vérification
    bool emailExiste(const QString &email);
    QString getMotDePasse(const QString &email);
    
    // Fonctions utilitaires
    void creerAdminParDefaut();
    void reinitialiserBase();

signals:
    void operationTerminee(bool succes, const QString &message);
};

#endif // DATABASE_MANAGER_H
