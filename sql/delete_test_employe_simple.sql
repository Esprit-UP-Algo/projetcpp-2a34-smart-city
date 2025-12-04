-- Script simple pour supprimer test Employe
-- Copiez-collez ces commandes directement dans votre client Oracle

-- Commande 1: Vérifier avant suppression
SELECT * FROM EMPLOYE WHERE CIN = 99999999;

-- Commande 2: Supprimer test Employe
DELETE FROM EMPLOYE WHERE CIN = 99999999;

-- Commande 3: Vérifier après suppression  
SELECT * FROM EMPLOYE WHERE CIN = 99999999;

-- Commande 4: Voir tous les employés restants
SELECT CIN, NOM, PRENOM, RFID_UID FROM EMPLOYE ORDER BY CIN;
