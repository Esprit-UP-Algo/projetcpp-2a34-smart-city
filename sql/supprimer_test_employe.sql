-- Supprimer l'employé "test Employe" de la base de données
-- Garder seulement "malek jlid" pour les tests RFID

-- Vérifier d'abord les employés existants
SELECT CIN, NOM, PRENOM, RFID_UID FROM EMPLOYE WHERE NOM = 'test' AND PRENOM = 'Employe';

-- Supprimer l'employé test
DELETE FROM EMPLOYE 
WHERE NOM = 'test' 
AND PRENOM = 'Employe';

-- Vérifier après suppression
SELECT CIN, NOM, PRENOM, RFID_UID FROM EMPLOYE;

-- Afficher les employés restants (devrait contenir malek jlid)
SELECT CIN, NOM, PRENOM, EMAIL, RFID_UID FROM EMPLOYE 
WHERE NOM = 'malek' AND PRENOM = 'jlid';
