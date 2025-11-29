# Configuration des Clés API

## 📍 Où configurer les clés

Toutes les clés API sont à configurer dans le fichier **`notificationmanager.cpp`** aux lignes 25-35.

## 📧 Configuration Gmail SMTP

Ouvrez `notificationmanager.cpp` et modifiez les lignes suivantes :

```cpp
emailUsername = "YOUR_EMAIL@gmail.com";  // ← Remplacez par votre email Gmail
emailPassword = "YOUR_APP_PASSWORD";     // ← Remplacez par votre mot de passe d'application Gmail
```

### Comment obtenir un mot de passe d'application Gmail :

1. Allez sur [myaccount.google.com](https://myaccount.google.com)
2. Sécurité → Validation en 2 étapes (doit être activée)
3. Mots de passe des applications → Sélectionnez "Autre" → Nommez-le "SmartCity"
4. Copiez le mot de passe généré (16 caractères)
5. Collez-le dans `emailPassword`

**⚠️ Important** : Utilisez un **mot de passe d'application**, pas votre mot de passe Gmail normal !

---

## 📱 Configuration Twilio SMS

Ouvrez `notificationmanager.cpp` et modifiez les lignes suivantes :

```cpp
smsApiKey = "ACxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";  // ← Votre Twilio Account SID
smsApiSecret = "YOUR_TWILIO_AUTH_TOKEN";            // ← Votre Twilio Auth Token
smsSenderNumber = "+1234567890";                   // ← Votre numéro Twilio (format: +1234567890)
```

### Comment obtenir vos clés Twilio :

1. Créez un compte sur [Twilio](https://www.twilio.com/)
2. Allez dans le Dashboard
3. Copiez votre **Account SID** (commence par `AC`)
4. Copiez votre **Auth Token** (cliquez sur "view" pour le voir)
5. Obtenez un numéro de téléphone Twilio (ou utilisez un numéro d'essai)

**Note** : Le numéro d'expéditeur doit être au format international avec le `+` (ex: `+1234567890`)

---

## ✅ Vérification

Après avoir configuré les clés :

1. Recompilez l'application
2. Ouvrez l'application
3. Allez dans **Notifications → Configuration**
4. Entrez l'adresse email et le numéro de téléphone du destinataire
5. Cliquez sur **"Tester"** pour vérifier que tout fonctionne

---

## 🔒 Sécurité

- ⚠️ **Ne commitez jamais** le fichier `notificationmanager.cpp` avec vos vraies clés dans Git
- Ajoutez `notificationmanager.cpp` au `.gitignore` si vous partagez le code
- Ou utilisez des variables d'environnement (implémentation avancée)

---

## 📝 Exemple de configuration complète

```cpp
// Gmail SMTP
emailUsername = "monapp.smartcity@gmail.com";
emailPassword = "abcd efgh ijkl mnop";  // Mot de passe d'application Gmail

// Twilio SMS
smsApiKey = "ACa1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6";
smsApiSecret = "your_twilio_auth_token_here";
smsSenderNumber = "+1234567890";  // Votre numéro Twilio
```

Une fois configuré, l'application utilisera automatiquement Gmail pour les emails et Twilio pour les SMS !

