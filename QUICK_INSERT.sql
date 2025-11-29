-- Script RAPIDE pour insérer UN utilisateur de test
-- Utilisez ce script si vous voulez juste tester rapidement

-- Vérifier l'ID_EMP maximum existant
SELECT MAX(ID_EMP) FROM EMPLOYE;

-- Insérer un utilisateur avec un ID_EMP disponible (remplacez 10 par un nombre plus grand si nécessaire)
-- Email court: admin@test.tn (15 caractères - bien en dessous de la limite de 20)
INSERT INTO EMPLOYE (ID_EMP, N_TEL, PRENOM, MOT_DE_PASSE, NOM_EMP, EMAIL, POSTE, CIN)
VALUES (10, 12345678, 'Admin', 'admin123', 'Admin', 'admin@test.tn', 'Admin', 10010010);

COMMIT;

-- Vérifier l'insertion
SELECT ID_EMP, NOM_EMP, PRENOM, EMAIL, POSTE, CIN FROM EMPLOYE WHERE ID_EMP = 10;

