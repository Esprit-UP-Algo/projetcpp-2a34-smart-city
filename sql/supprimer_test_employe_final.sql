-- Script SQL pour supprimer "test Employe" et garder seulement "malek jlid"

-- Étape 1: Vérifier les employés actuels
SELECT 'AVANT SUPPRESSION:' as STATUT;
SELECT CIN, NOM, PRENOM, EMAIL, RFID_UID FROM EMPLOYE ORDER BY CIN;

-- Étape 2: Supprimer l'employé test (CIN: 99999999)
DELETE FROM EMPLOYE 
WHERE CIN = 99999999 
AND NOM = 'test' 
AND PRENOM = 'Employe';

-- Étape 3: Vérifier après suppression
SELECT 'APRÈS SUPPRESSION:' as STATUT;
SELECT CIN, NOM, PRENOM, EMAIL, RFID_UID FROM EMPLOYE ORDER BY CIN;

-- Étape 4: Confirmer que malek jlid est toujours présent
SELECT 'EMPLOYÉ MALEK JLID (RESTANT):' as STATUT;
SELECT CIN, NOM, PRENOM, EMAIL, RFID_UID FROM EMPLOYE 
WHERE NOM = 'malek' AND PRENOM = 'jlid';
