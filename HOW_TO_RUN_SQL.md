# 📘 Comment Exécuter le Script SQL dans Oracle

## Méthode 1: Oracle SQL Developer (Recommandé - Interface Graphique)

### Étapes:
1. **Ouvrir Oracle SQL Developer**
   - Lancez l'application Oracle SQL Developer depuis le menu Démarrer

2. **Se connecter à la base de données**
   - Cliquez sur l'icône "+" à côté de "Connections" dans le panneau gauche
   - Remplissez les informations :
     - **Connection Name:** SmartCity (ou un nom de votre choix)
     - **Username:** `anis` (selon votre connection.cpp)
     - **Password:** `hana123` (selon votre connection.cpp)
     - **Hostname:** localhost (ou l'adresse de votre serveur Oracle)
     - **Port:** 1521 (port par défaut Oracle)
     - **SID:** XE (ou votre SID Oracle) OU **Service name:** Source_Projet2A
   - Cliquez sur "Test" pour vérifier la connexion
   - Cliquez sur "Save" puis "Connect"

3. **Ouvrir le script SQL**
   - Fichier → Ouvrir (ou Ctrl+O)
   - Naviguez vers le fichier `create_test_user.sql`
   - Le script s'ouvrira dans l'éditeur

4. **Exécuter le script**
   - Cliquez sur le bouton "Run Script" (F5) ou
   - Cliquez droit dans l'éditeur → "Run Script"
   - Vérifiez les résultats dans l'onglet "Script Output" en bas

5. **Vérifier les données**
   - Dans l'onglet "Worksheet", tapez :
     ```sql
     SELECT CIN, NOM, PRENOM, EMAIL, POSTE FROM EMPLOYE;
     ```
   - Cliquez sur "Run Statement" (Ctrl+Enter)
   - Vous devriez voir les 3 utilisateurs créés

---

## Méthode 2: SQL*Plus (Ligne de commande)

### Étapes:
1. **Ouvrir SQL*Plus**
   - Ouvrez l'invite de commande (cmd)
   - Tapez : `sqlplus`

2. **Se connecter**
   ```
   Enter user-name: anis
   Enter password: hana123
   ```

3. **Exécuter le script**
   ```
   SQL> @C:\Users\anisk\OneDrive\Desktop\smartcity\smartcity\create_test_user.sql
   ```
   (Ajustez le chemin selon votre emplacement)

4. **Vérifier**
   ```sql
   SQL> SELECT CIN, NOM, PRENOM, EMAIL, POSTE FROM EMPLOYE;
   ```

---

## Méthode 3: Copier-Coller Directement

### Dans SQL Developer:
1. Connectez-vous à votre base de données
2. Ouvrez un nouveau "Worksheet" (Ctrl+Shift+N)
3. Copiez-collez ce code :

```sql
-- Utilisateur Admin
INSERT INTO EMPLOYE (CIN, NOM, PRENOM, EMAIL, MOTDEPASS, N_TELEPHON, POSTE, SALAIRE)
VALUES (10010010, 'Admin', 'System', 'admin@smartcity.com', 'admin123', 12345678, 'Administrateur', 5000.0);

-- Utilisateur Test 1
INSERT INTO EMPLOYE (CIN, NOM, PRENOM, EMAIL, MOTDEPASS, N_TELEPHON, POSTE, SALAIRE)
VALUES (20020020, 'Test', 'User', 'test@smartcity.com', 'test123', 87654321, 'Utilisateur', 3000.0);

-- Utilisateur Test 2
INSERT INTO EMPLOYE (CIN, NOM, PRENOM, EMAIL, MOTDEPASS, N_TELEPHON, POSTE, SALAIRE)
VALUES (30030030, 'Demo', 'Account', 'demo@smartcity.com', 'demo123', 11223344, 'Démonstration', 2500.0);

COMMIT;
```

4. Sélectionnez tout le code (Ctrl+A)
5. Cliquez sur "Run Statement" (Ctrl+Enter)
6. Vérifiez le message "3 rows inserted" dans les résultats

---

## Méthode 4: Via l'Application Qt (Alternative)

Si vous ne pouvez pas accéder à Oracle directement :

1. **Lancez votre application Qt**
2. **Connectez-vous** (vous devrez créer un premier utilisateur via SQL d'abord)
3. **Allez dans "Gestion des Employés"**
4. **Cliquez sur "Ajouter"**
5. **Remplissez les champs :**
   - CIN: `10010010`
   - Nom: `Admin`
   - Prénom: `System`
   - Email: `admin@smartcity.com`
   - Mot de passe: `admin123`
   - Téléphone: `12345678`
   - Poste: `Administrateur`
   - Salaire: `5000`
6. **Cliquez sur "Ajouter"**

Répétez pour les autres utilisateurs.

---

## ⚠️ Erreurs Possibles et Solutions

### Erreur: "table or view does not exist"
- **Solution:** Vérifiez que la table s'appelle bien `EMPLOYE` (en majuscules)
- Vérifiez que vous êtes connecté au bon schéma/utilisateur

### Erreur: "unique constraint violated"
- **Solution:** Les utilisateurs existent déjà. Utilisez :
  ```sql
  SELECT * FROM EMPLOYE WHERE EMAIL = 'admin@smartcity.com';
  ```
  Pour voir les utilisateurs existants

### Erreur: "insufficient privileges"
- **Solution:** Vérifiez que votre utilisateur a les droits INSERT sur la table EMPLOYE

---

## ✅ Vérification Finale

Pour vérifier que tout fonctionne :

```sql
-- Voir tous les utilisateurs
SELECT CIN, NOM, PRENOM, EMAIL, POSTE FROM EMPLOYE ORDER BY CIN;

-- Vérifier un utilisateur spécifique
SELECT * FROM EMPLOYE WHERE EMAIL = 'admin@smartcity.com';
```

---

## 📍 Emplacement du Fichier SQL

Le fichier `create_test_user.sql` se trouve dans :
```
C:\Users\anisk\OneDrive\Desktop\smartcity\smartcity\create_test_user.sql
```

