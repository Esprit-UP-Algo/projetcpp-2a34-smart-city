# 🔐 Identifiants de Connexion - SmartCity (MIS À JOUR)

## ⚠️ IMPORTANT : Structure de Table Corrigée

La structure réelle de votre table `EMPLOYE` est différente. J'ai mis à jour le script SQL.

### Structure Réelle :
- `ID_EMP` (au lieu de généré automatiquement)
- `N_TEL` (au lieu de N_TELEPHON)
- `PRENOM`
- `MOT_DE_PASSE` (au lieu de MOTDEPASS)
- `NOM_EMP` (au lieu de NOM)
- `EMAIL` (limité à 20 caractères)
- `POSTE`
- `CIN`
- **Pas de colonne SALAIRE**

---

## 👤 Utilisateurs de Test (Emails Courts)

### Compte Administrateur
- **Email:** `admin@smartcity.tn` (19 caractères)
- **Mot de passe:** `admin123`
- **CIN:** 10010010
- **Poste:** Administrateur

### Compte Utilisateur Test 1
- **Email:** `test@smartcity.tn` (18 caractères)
- **Mot de passe:** `test123`
- **CIN:** 20020020
- **Poste:** Utilisateur

### Compte Démonstration
- **Email:** `demo@smartcity.tn` (18 caractères)
- **Mot de passe:** `demo123`
- **CIN:** 30030030
- **Poste:** Demonstration

---

## 📝 Instructions d'Exécution

### Dans Oracle SQL Developer :

1. **Ouvrez le fichier `create_test_user.sql`** (déjà ouvert dans votre éditeur)

2. **Vérifiez que vous êtes connecté à la bonne base** (`gestion_dechet`)

3. **Exécutez le script :**
   - Sélectionnez tout le contenu (Ctrl+A)
   - Cliquez sur "Run Script" (F5) ou le bouton d'exécution
   - Vérifiez qu'il n'y a pas d'erreurs

4. **Vérifiez les résultats :**
   ```sql
   SELECT ID_EMP, NOM_EMP, PRENOM, EMAIL, POSTE, CIN FROM EMPLOYE;
   ```

---

## 🔧 Si vous avez des erreurs de doublons

Si les ID_EMP existent déjà, utilisez des valeurs différentes :

```sql
-- Vérifier les ID_EMP existants
SELECT MAX(ID_EMP) FROM EMPLOYE;

-- Utiliser des ID_EMP plus grands (ex: 10, 11, 12)
INSERT INTO EMPLOYE (ID_EMP, N_TEL, PRENOM, MOT_DE_PASSE, NOM_EMP, EMAIL, POSTE, CIN)
VALUES (10, 12345678, 'System', 'admin123', 'Admin', 'admin@smartcity.tn', 'Administrateur', 10010010);
```

---

## 📧 Note sur les Emails

Les emails sont limités à **20 caractères maximum** dans votre table. J'ai utilisé `.tn` au lieu de `.com` pour respecter cette limite.

Si vous voulez permettre des emails plus longs, exécutez le script `FIX_EMAIL_COLUMN.sql` pour augmenter la taille de la colonne.

