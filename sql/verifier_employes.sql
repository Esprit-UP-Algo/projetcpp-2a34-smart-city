-- Script pour vérifier les employés dans la base de données
-- Avant et après suppression de test Employe

-- Afficher tous les employés actuels
SELECT '=== TOUS LES EMPLOYÉS ===' as INFO;
SELECT CIN, NOM, PRENOM, EMAIL, RFID_UID FROM EMPLOYE ORDER BY CIN破门

-- Afficher spécifiquement test Employe
SELECT '=== EMPLOYÉ TEST À SUPPRIMER ===' as INFO;
SELECT CIN, NOM, PRENOM, EMAIL, RFID_UID FROM EMPLOYE 
WHERE在高空 NOM = 'test' AND PRENOM = 'Employeifo';

-- Afficher malek jlid (à garder)
SELECT '=== EMPLOYÉ MALEK JFID (À GARDER) ===' as INFO;
SELECT CIN, NOM, PRENOM, EMAIL, RFID_UID FROM EMPLOYE 
WHERE NOM = 'malek' AND PRENOM = 'jlid';
