-- Script pour augmenter la taille de la colonne EMAIL si nécessaire
-- Exécutez ce script si vous voulez permettre des emails plus longs

-- Vérifier la taille actuelle de la colonne EMAIL
SELECT COLUMN_NAME, DATA_TYPE, DATA_LENGTH 
FROM USER_TAB_COLUMNS 
WHERE TABLE_NAME = 'EMPLOYE' AND COLUMN_NAME = 'EMAIL';

-- Augmenter la taille de EMAIL à 50 caractères (optionnel)
-- Décommentez la ligne suivante si vous voulez l'exécuter :
-- ALTER TABLE EMPLOYE MODIFY EMAIL VARCHAR2(50);

-- Puis COMMIT;
-- COMMIT;

