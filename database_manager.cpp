#include "database_manager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
}

// Afficher tous les employés
void DatabaseManager::afficherTousLesEmployes()
{
    QSqlQuery query;
    if (query.exec("SELECT CIN, NOM, PRENOM, EMAIL, MOTDEPASS, POSTE FROM EMPLOYE")) {
        qDebug() << "=== LISTE DES EMPLOYÉS ===";
        qDebug() << "CIN\t| NOM\t\t| PRENOM\t| EMAIL\t\t\t| MOT DE PASSE\t| POSTE";
        qDebug() << "--------------------------------------------------------------------------------------------------------";
        
        while (query.next()) {
            QString cin = query.value(0).toString();
            QString nom = query.value(1).toString();
            QString prenom = query.value(2).toString();
            QString email = query.value(3).toString();
            QString pass = query.value(4).toString();
            QString poste = query.value(5).toString();
            
            qDebug() << cin << "\t|" << nom << "\t|" << prenom << "\t|" << email << "\t|" << pass << "\t|" << poste;
        }
        qDebug() << "========================================================================================================";
    } else {
        qDebug() << "Erreur lors de la lecture des employés:" << query.lastError().text();
    }
}

// Ajouter un employé
bool DatabaseManager::ajouterEmploye(const QString &cin, const QString &nom, const QString &prenom, 
                                    const QString &email, const QString &motDePasse, const QString &poste)
{
    QSqlQuery query;
    QString sql = "INSERT INTO EMPLOYE (CIN, NOM, PRENOM, EMAIL, MOTDEPASS, POSTE) VALUES (?, ?, ?, ?, ?, ?)";
    query.prepare(sql);
    query.addBindValue(cin);
    query.addBindValue(nom);
    query.addBindValue(prenom);
    query.addBindValue(email);
    query.addBindValue(motDePasse);
    query.addBindValue(poste);
    
    if (query.exec()) {
        qDebug() << "✅ Employé ajouté avec succès:" << nom << prenom;
        return true;
    } else {
        qDebug() << "❌ Erreur lors de l'ajout de l'employé:" << query.lastError().text();
        return false;
    }
}

// Modifier le mot de passe d'un employé
bool DatabaseManager::modifierMotDePasse(const QString &email, const QString &nouveauMotDePasse)
{
    QSqlQuery query;
    QString sql = "UPDATE EMPLOYE SET MOTDEPASS = ? WHERE EMAIL = ?";
    query.prepare(sql);
    query.addBindValue(nouveauMotDePasse);
    query.addBindValue(email);
    
    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            qDebug() << "✅ Mot de passe modifié pour:" << email;
            return true;
        } else {
            qDebug() << "❌ Aucun employé trouvé avec cet email:" << email;
            return false;
        }
    } else {
        qDebug() << "❌ Erreur lors de la modification du mot de passe:" << query.lastError().text();
        return false;
    }
}

// Vérifier si un email existe
bool DatabaseManager::emailExiste(const QString &email)
{
    QSqlQuery query;
    QString sql = "SELECT COUNT(*) FROM EMPLOYE WHERE EMAIL = ?";
    query.prepare(sql);
    query.addBindValue(email);
    
    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }
    return false;
}

// Obtenir le mot de passe d'un email
QString DatabaseManager::getMotDePasse(const QString &email)
{
    QSqlQuery query;
    QString sql = "SELECT MOTDEPASS FROM EMPLOYE WHERE EMAIL = ?";
    query.prepare(sql);
    query.addBindValue(email);
    
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return "";
}

// Supprimer un employé
bool DatabaseManager::supprimerEmploye(const QString &email)
{
    QSqlQuery query;
    QString sql = "DELETE FROM EMPLOYE WHERE EMAIL = ?";
    query.prepare(sql);
    query.addBindValue(email);
    
    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            qDebug() << "✅ Employé supprimé:" << email;
            return true;
        } else {
            qDebug() << "❌ Aucun employé trouvé avec cet email:" << email;
            return false;
        }
    } else {
        qDebug() << "❌ Erreur lors de la suppression:" << query.lastError().text();
        return false;
    }
}

// Créer un utilisateur admin par défaut
void DatabaseManager::creerAdminParDefaut()
{
    if (!emailExiste("admin@test.tn")) {
        ajouterEmploye("99999999", "Admin", "System", "admin@test.tn", "admin123", "Administrateur");
        qDebug() << "✅ Utilisateur admin par défaut créé";
    } else {
        qDebug() << "ℹ️ L'utilisateur admin existe déjà";
    }
}

// Réinitialiser la base (optionnel - à utiliser avec prudence)
void DatabaseManager::reinitialiserBase()
{
    QSqlQuery query;
    if (query.exec("DELETE FROM EMPLOYE")) {
        qDebug() << "⚠️ Base de données réinitialisée - tous les employés supprimés";
        creerAdminParDefaut();
    } else {
        qDebug() << "❌ Erreur lors de la réinitialisation:" << query.lastError().text();
    }
}
