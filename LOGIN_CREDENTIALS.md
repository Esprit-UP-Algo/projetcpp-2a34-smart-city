# 🔐 Identifiants de Connexion - SmartCity

## Utilisateurs de Test

Après avoir exécuté le script `create_test_user.sql` dans votre base de données Oracle, vous pouvez utiliser les identifiants suivants :

### 👤 Compte Administrateur
- **Email:** `admin@smartcity.com`
- **Mot de passe:** `admin123`
- **CIN:** 10010010
- **Poste:** Administrateur

### 👤 Compte Utilisateur Test 1
- **Email:** `test@smartcity.com`
- **Mot de passe:** `test123`
- **CIN:** 20020020
- **Poste:** Utilisateur

### 👤 Compte Démonstration
- **Email:** `demo@smartcity.com`
- **Mot de passe:** `demo123`
- **CIN:** 30030030
- **Poste:** Démonstration

---

## 📝 Instructions

### Option 1: Utiliser le script SQL
1. Ouvrez votre client Oracle (SQL Developer, SQL*Plus, etc.)
2. Connectez-vous à votre base de données
3. Exécutez le fichier `create_test_user.sql`
4. Vérifiez que les utilisateurs ont été créés avec :
   ```sql
   SELECT CIN, NOM, PRENOM, EMAIL, POSTE FROM EMPLOYE;
   ```

### Option 2: Créer un utilisateur via l'interface
1. Lancez l'application
2. Connectez-vous avec un compte existant (ou créez-en un via SQL)
3. Allez dans la section "Gestion des Employés"
4. Cliquez sur "Ajouter"
5. Remplissez les champs :
   - **CIN:** 10010010
   - **Nom:** Admin
   - **Prénom:** System
   - **Email:** admin@smartcity.com
   - **Mot de passe:** admin123
   - **Téléphone:** 12345678
   - **Poste:** Administrateur
   - **Salaire:** 5000

---

## ⚠️ Important

- **Sécurité:** Changez les mots de passe par défaut en production !
- **Base de données:** Assurez-vous que la connexion à Oracle est configurée dans `connection.cpp`
- **Table:** La table doit s'appeler `EMPLOYE` (en majuscules) dans Oracle

---

## 🔍 Vérification

Pour vérifier qu'un utilisateur existe dans la base de données :
```sql
SELECT * FROM EMPLOYE WHERE EMAIL = 'admin@smartcity.com';
```

Pour voir tous les utilisateurs :
```sql
SELECT CIN, NOM, PRENOM, EMAIL, POSTE FROM EMPLOYE ORDER BY CIN;
```

