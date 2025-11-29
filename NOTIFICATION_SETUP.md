# Configuration des Notifications SMS et Email

Ce document explique comment configurer les notifications SMS et Email dans l'application SmartCity.

## 📧 Configuration Email (SMTP)

### Paramètres requis :
1. **Serveur SMTP** : L'adresse du serveur SMTP (ex: `smtp.gmail.com`, `smtp.outlook.com`)
2. **Port** : Le port SMTP (généralement 587 pour TLS, 465 pour SSL)
3. **Nom d'utilisateur** : Votre adresse email
4. **Mot de passe** : Votre mot de passe email (ou mot de passe d'application)
5. **SSL/TLS** : Cochez si votre serveur utilise SSL/TLS

### Exemples de configuration :

#### Gmail :
- **Serveur SMTP** : `smtp.gmail.com`
- **Port** : `587`
- **Utilisateur** : `votre.email@gmail.com`
- **Mot de passe** : Utilisez un [mot de passe d'application](https://support.google.com/accounts/answer/185833)
- **SSL** : ✓ Activé

#### Outlook/Hotmail :
- **Serveur SMTP** : `smtp-mail.outlook.com`
- **Port** : `587`
- **Utilisateur** : `votre.email@outlook.com`
- **Mot de passe** : Votre mot de passe
- **SSL** : ✓ Activé

#### Yahoo :
- **Serveur SMTP** : `smtp.mail.yahoo.com`
- **Port** : `587`
- **Utilisateur** : `votre.email@yahoo.com`
- **Mot de passe** : Votre mot de passe
- **SSL** : ✓ Activé

### ⚠️ Notes importantes :
- Pour Gmail, vous devez activer l'accès des applications moins sécurisées ou utiliser un mot de passe d'application
- Certains fournisseurs nécessitent une authentification à deux facteurs (2FA) avec un mot de passe d'application

---

## 📱 Configuration SMS

L'application supporte plusieurs fournisseurs SMS. Configurez les clés API selon votre fournisseur.

### Option 1 : Twilio (Recommandé)

1. Créez un compte sur [Twilio](https://www.twilio.com/)
2. Obtenez votre **Account SID** et **Auth Token** depuis le dashboard
3. Dans l'application :
   - **Clé API** : Votre Account SID (commence par `AC`)
   - **Secret API** : Votre Auth Token
   - **Numéro Expéditeur** : Votre numéro Twilio (format: +1234567890)

### Option 2 : Nexmo/Vonage

1. Créez un compte sur [Vonage](https://www.vonage.com/)
2. Obtenez votre **API Key** et **API Secret**
3. Dans l'application :
   - **Clé API** : Votre API Key (8 caractères)
   - **Secret API** : Votre API Secret (16 caractères)
   - **Numéro Expéditeur** : Votre nom d'expéditeur (ex: "SmartCity")

### Option 3 : API Générique (Custom Provider)

Pour utiliser un autre fournisseur SMS :

1. **Serveur SMTP** (réutilisé) : Entrez l'URL de l'API (ex: `https://api.votre-provider.com/send`)
2. **Clé API** : Votre clé API
3. **Secret API** : Votre secret API
4. **Numéro Expéditeur** : Votre numéro ou nom d'expéditeur

L'API doit accepter des requêtes POST JSON avec :
```json
{
  "to": "+21612345678",
  "from": "SmartCity",
  "message": "Votre message"
}
```

Et utiliser l'authentification Basic Auth ou header `X-API-Key`.

---

## 🔧 Configuration dans l'Application

1. Ouvrez l'application SmartCity
2. Cliquez sur le bouton **"Configuration"** dans la section Notifications
3. Remplissez les champs :
   - **Section Email** : Entrez vos paramètres SMTP
   - **Section SMS** : Entrez vos clés API
   - **Destinataires** : Ajoutez les emails et numéros de téléphone
4. Cliquez sur **"Enregistrer"**

### Format des numéros de téléphone :
- Format international recommandé : `+21612345678`
- Format local accepté : `012345678` (sera automatiquement converti en +216...)
- L'application détecte automatiquement le format et ajoute le préfixe +216 pour la Tunisie

---

## 🧪 Test de Configuration

1. Après configuration, utilisez le bouton **"Tester"** dans la gestion des règles d'alerte
2. Ou créez une règle d'alerte de test
3. Vérifiez l'historique des notifications pour voir si l'envoi a réussi

---

## 🔒 Sécurité

- Les mots de passe et clés API sont stockés de manière sécurisée dans les paramètres de l'application
- Les clés API ne sont jamais affichées en clair (seuls les premiers caractères sont visibles dans les logs)
- Utilisez des mots de passe d'application plutôt que vos mots de passe principaux quand c'est possible

---

## ❓ Dépannage

### Email ne s'envoie pas :
- Vérifiez que le serveur SMTP et le port sont corrects
- Vérifiez vos identifiants
- Pour Gmail, utilisez un mot de passe d'application
- Vérifiez que votre pare-feu/autorise les connexions sortantes sur le port SMTP

### SMS ne s'envoie pas :
- Vérifiez que vos clés API sont correctes
- Vérifiez que votre compte a des crédits (pour les services payants)
- Vérifiez le format du numéro de téléphone (doit commencer par +)
- Consultez les logs de l'application pour les erreurs détaillées

---

## 📝 Exemple de Configuration Complète

### Email (Gmail) :
```
Serveur SMTP: smtp.gmail.com
Port: 587
Utilisateur: monapp.smartcity@gmail.com
Mot de passe: [Mot de passe d'application Gmail]
SSL: ✓
```

### SMS (Twilio) :
```
Clé API: ACxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
Secret API: votre_auth_token_twilio
Numéro Expéditeur: +1234567890
```

### Destinataires :
```
Email: admin@smartcity.tn
Téléphone: +21612345678
```

---

Pour toute question ou problème, consultez les logs de l'application ou contactez le support.

