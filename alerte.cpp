#include "alerte.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

// ==================== CONSTRUCTEURS ====================

Alerte::Alerte() {
    id = 0;
    type = "";
    id_client = 0;
    statut = "";
    description = "";
    gravite = "";
    date = QDate::currentDate();
}

Alerte::Alerte(int i, QString t, int idc, QString s, QString d, QString g, QDate dt) {
    id = i;
    type = t;
    id_client = idc;
    statut = s;
    description = d;
    gravite = g;
    date = dt;
}

// ==================== GETTERS ====================

int Alerte::getId() {
    return id;
}

QString Alerte::getType() {
    return type;
}

int Alerte::getIdClient() {
    return id_client;
}

QString Alerte::getStatut() {
    return statut;
}

QString Alerte::getDescription() {
    return description;
}

QString Alerte::getGravite() {
    return gravite;
}

QDate Alerte::getDate() {
    return date;
}

// ==================== SETTERS ====================

void Alerte::setId(int i) {
    id = i;
}

void Alerte::setType(QString t) {
    type = t;
}

void Alerte::setIdClient(int idc) {
    id_client = idc;
}

void Alerte::setStatut(QString s) {
    statut = s;
}

void Alerte::setDescription(QString d) {
    description = d;
}

void Alerte::setGravite(QString g) {
    gravite = g;
}

void Alerte::setDate(QDate dt) {
    date = dt;
}

// ==================== CREATE ====================

bool Alerte::ajouter() {
    qDebug() << "=== DEBUT AJOUTER ALERTE ===";

    // ========== CONTRÔLES DE SAISIE ==========
    if (type.trimmed().isEmpty()) {
        qDebug() << "❌ Erreur: Le type est obligatoire";
        return false;
    }

    if (id_client <= 0) {
        qDebug() << "❌ Erreur: ID client invalide:" << id_client;
        return false;
    }

    if (statut.trimmed().isEmpty()) {
        qDebug() << "❌ Erreur: Le statut est obligatoire";
        return false;
    }

    QStringList gravitesValides;
    gravitesValides << "Faible" << "Moyenne" << "Élevée" << "Critique";

    if (!gravitesValides.contains(gravite)) {
        qDebug() << "❌ Erreur: Gravité invalide:" << gravite;
        qDebug() << "Gravités valides:" << gravitesValides;
        return false;
    }

    if (date > QDate::currentDate()) {
        qDebug() << "❌ Erreur: La date ne peut pas être dans le futur";
        return false;
    }

    // ========== AFFICHER LES DONNÉES ==========
    qDebug() << "Données à insérer:";
    qDebug() << "  Type:" << type;
    qDebug() << "  ID Client:" << id_client;
    qDebug() << "  Statut:" << statut;
    qDebug() << "  Description:" << description;
    qDebug() << "  Gravité:" << gravite;
    qDebug() << "  Date:" << date.toString("dd/MM/yyyy");

    // ========== VÉRIFIER LA STRUCTURE DE LA TABLE ==========
    QSqlQuery checkColumns;
    checkColumns.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'ALERTE' ORDER BY COLUMN_ID");
    
    QStringList existingColumns;
    if (checkColumns.exec()) {
        while (checkColumns.next()) {
            existingColumns << checkColumns.value(0).toString().toUpper();
        }
        qDebug() << "Colonnes trouvées dans ALERTE:" << existingColumns;
    } else {
        qDebug() << "⚠️ Impossible de vérifier les colonnes. La table ALERTE n'existe peut-être pas.";
        qDebug() << "Erreur:" << checkColumns.lastError().text();
        return false;
    }
    
    if (existingColumns.isEmpty()) {
        qDebug() << "❌ La table ALERTE n'existe pas ou est vide!";
        qDebug() << "Veuillez exécuter le script create_alerte_table.sql dans Oracle SQL Developer";
        return false;
    }
    
    // Déterminer les noms de colonnes réels (Oracle peut être sensible à la casse)
    // Basé sur les colonnes trouvées: ID_AL, GRAVITE, CLIENT, DESCRIPTION, TYPE, STATUT, DATE_ET_HEURE, ID_EMP
    QString colId = existingColumns.contains("ID_AL") ? "ID_AL" : 
                    existingColumns.contains("ID") ? "ID" : 
                    existingColumns.contains("ID_ALERTE") ? "ID_ALERTE" : "";
    QString colType = existingColumns.contains("TYPE") ? "TYPE" : "";
    QString colIdClient = existingColumns.contains("CLIENT") ? "CLIENT" : 
                          existingColumns.contains("ID_CLIENT") ? "ID_CLIENT" : 
                          existingColumns.contains("IDCLIENT") ? "IDCLIENT" : "";
    QString colStatut = existingColumns.contains("STATUT") ? "STATUT" : "";
    QString colDescription = existingColumns.contains("DESCRIPTION") ? "DESCRIPTION" : "";
    QString colGravite = existingColumns.contains("GRAVITE") ? "GRAVITE" : "";
    QString colDate = existingColumns.contains("DATE_ET_HEURE") ? "DATE_ET_HEURE" :
                      existingColumns.contains("DATE_ALERTE") ? "DATE_ALERTE" :
                      existingColumns.contains("DATEALERTE") ? "DATEALERTE" :
                      existingColumns.contains("DATE") ? "DATE" : "";
    
    if (colId.isEmpty() || colType.isEmpty() || colIdClient.isEmpty() || 
        colStatut.isEmpty() || colGravite.isEmpty() || colDate.isEmpty()) {
        qDebug() << "❌ Structure de table incorrecte! Colonnes manquantes.";
        qDebug() << "Colonnes requises: ID (ou ID_AL), TYPE, CLIENT (ou ID_CLIENT), STATUT, GRAVITE, DATE_ET_HEURE (ou DATE_ALERTE)";
        qDebug() << "Colonnes trouvées:" << existingColumns;
        return false;
    }
    
    qDebug() << "✅ Colonnes mappées:";
    qDebug() << "  ID ->" << colId;
    qDebug() << "  TYPE ->" << colType;
    qDebug() << "  ID_CLIENT ->" << colIdClient;
    qDebug() << "  STATUT ->" << colStatut;
    qDebug() << "  DESCRIPTION ->" << (colDescription.isEmpty() ? "(ignoré)" : colDescription);
    qDebug() << "  GRAVITE ->" << colGravite;
    qDebug() << "  DATE_ALERTE ->" << colDate;
    
    // ========== INSERTION DANS LA BD ==========
    QSqlQuery query;
    // Utiliser les noms de colonnes réels
    QStringList columns;
    QStringList placeholders;
    
    columns << colId << colType << colIdClient << colStatut << colGravite << colDate;
    placeholders << ":id" << ":type" << ":id_client" << ":statut" << ":gravite" << ":date";
    
    // Ajouter DESCRIPTION seulement si la colonne existe
    if (!colDescription.isEmpty()) {
        columns << colDescription;
        placeholders << ":description";
    }
    
    QString insertQuery = QString("INSERT INTO ALERTE (%1) VALUES (%2)")
                          .arg(columns.join(", "))
                          .arg(placeholders.join(", "));
    
    query.prepare(insertQuery);

    // Toujours générer un nouvel ID unique lors de l'ajout (ignorer l'ID existant)
    // Essayer d'abord d'utiliser une séquence Oracle si elle existe
    QSqlQuery seqQuery;
    seqQuery.prepare("SELECT SEQ_ALERTE_ID.NEXTVAL FROM DUAL");
    
    if (seqQuery.exec() && seqQuery.next()) {
        id = seqQuery.value(0).toInt();
        qDebug() << "✅ ID généré via séquence Oracle:" << id;
    } else {
        // Si pas de séquence, utiliser MAX(ID) + 1 et vérifier qu'il n'existe pas
        qDebug() << "⚠️ Séquence Oracle non disponible, utilisation de MAX(ID) + 1";
        
        int maxAttempts = 10; // Limite de tentatives
        int attempt = 0;
        bool idFound = false;
        
        while (!idFound && attempt < maxAttempts) {
            QSqlQuery maxIdQuery;
            maxIdQuery.prepare(QString("SELECT NVL(MAX(%1), 0) + 1 FROM ALERTE").arg(colId));
            
            if (maxIdQuery.exec() && maxIdQuery.next()) {
                int newId = maxIdQuery.value(0).toInt() + attempt; // Ajouter attempt pour éviter les collisions
                
                // Vérifier si cet ID existe déjà
                QSqlQuery checkIdQuery;
                checkIdQuery.prepare(QString("SELECT COUNT(*) FROM ALERTE WHERE %1 = :id").arg(colId));
                checkIdQuery.bindValue(":id", newId);
                
                if (checkIdQuery.exec() && checkIdQuery.next()) {
                    int count = checkIdQuery.value(0).toInt();
                    if (count == 0) {
                        // ID n'existe pas, on peut l'utiliser
                        id = newId;
                        idFound = true;
                        qDebug() << "✅ ID unique généré:" << id;
                    } else {
                        // ID existe déjà, on réessaie
                        qDebug() << "⚠️ ID" << newId << "existe déjà, tentative suivante...";
                        attempt++;
                    }
                } else {
                    // Erreur lors de la vérification, utiliser quand même l'ID
                    id = newId;
                    idFound = true;
                    qDebug() << "⚠️ Impossible de vérifier l'ID, utilisation de:" << id;
                }
            } else {
                qDebug() << "❌ Impossible de récupérer le MAX(ID)";
                qDebug() << "Erreur:" << maxIdQuery.lastError().text();
                // Essayer avec un ID basé sur le timestamp si MAX échoue
                id = QDateTime::currentMSecsSinceEpoch() % 1000000; // Utiliser les 6 derniers chiffres du timestamp
                idFound = true;
                qDebug() << "⚠️ Utilisation d'un ID basé sur timestamp:" << id;
            }
        }
        
        if (!idFound) {
            // Dernière tentative : utiliser timestamp
            id = QDateTime::currentMSecsSinceEpoch() % 1000000;
            qDebug() << "⚠️ Utilisation d'un ID basé sur timestamp après" << maxAttempts << "tentatives:" << id;
        }
    }

    query.bindValue(":id", id);
    query.bindValue(":type", type);
    query.bindValue(":id_client", id_client);
    query.bindValue(":statut", statut);
    query.bindValue(":gravite", gravite);
    query.bindValue(":date", date);
    if (!colDescription.isEmpty()) {
        query.bindValue(":description", description.isEmpty() ? QVariant(QVariant::String) : description);
    }
    
    qDebug() << "Requête SQL préparée:" << insertQuery;

    qDebug() << "Query SQL:" << query.lastQuery();

    if(query.exec()) {
        qDebug() << "✅ Alerte ajoutée avec succès!";
        qDebug() << "Lignes affectées:" << query.numRowsAffected();
        return true;
    } else {
        QString errorText = query.lastError().text();
        QString driverText = query.lastError().driverText();
        QString databaseText = query.lastError().databaseText();
        
        qDebug() << "❌ ERREUR SQL!";
        qDebug() << "Erreur complète:" << errorText;
        qDebug() << "Driver:" << driverText;
        qDebug() << "Database:" << databaseText;
        qDebug() << "Query préparée:" << query.lastQuery();
        qDebug() << "Valeurs liées:";
        qDebug() << "  ID:" << id;
        qDebug() << "  Type:" << type;
        qDebug() << "  ID Client:" << id_client;
        qDebug() << "  Statut:" << statut;
        qDebug() << "  Description:" << description;
        qDebug() << "  Gravité:" << gravite;
        qDebug() << "  Date:" << date.toString("yyyy-MM-dd");
        
        // Vérifier si la table existe
        QSqlQuery checkTable("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = 'ALERTE'");
        if (checkTable.exec() && checkTable.next()) {
            int tableExists = checkTable.value(0).toInt();
            if (tableExists == 0) {
                qDebug() << "⚠️ ATTENTION: La table ALERTE n'existe pas dans la base de données!";
                qDebug() << "Veuillez exécuter le script create_alerte_table.sql dans Oracle SQL Developer";
            }
        }
        
        return false;
    }
}

// ==================== READ ====================

QSqlQueryModel* Alerte::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    
    // Vérifier les colonnes réelles de la table
    QSqlQuery checkColumns;
    checkColumns.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'ALERTE' ORDER BY COLUMN_ID");
    
    QStringList existingColumns;
    if (checkColumns.exec()) {
        while (checkColumns.next()) {
            existingColumns << checkColumns.value(0).toString().toUpper();
        }
    }
    
    // Utiliser la fonction helper pour obtenir les noms de colonnes
    ColumnNames cols = getColumnNames();
    
    QString selectQuery = QString("SELECT %1, %2, %3, %4, %5, %6, %7 FROM ALERTE ORDER BY %1 DESC")
                          .arg(cols.id, cols.type, cols.idClient, cols.statut, cols.description, cols.gravite, cols.date);
    
    model->setQuery(selectQuery);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Gravité"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date"));

    qDebug() << "Nombre de lignes affichées:" << model->rowCount();

    return model;
}

// ==================== UPDATE ====================

bool Alerte::modifier() {
    qDebug() << "=== DEBUT MODIFIER ALERTE ===";

    if (id <= 0) {
        qDebug() << "❌ Erreur: ID invalide pour la modification";
        return false;
    }

    if (type.trimmed().isEmpty()) {
        qDebug() << "❌ Erreur: Le type est obligatoire";
        return false;
    }

    if (id_client <= 0) {
        qDebug() << "❌ Erreur: ID client invalide";
        return false;
    }

    if (statut.trimmed().isEmpty()) {
        qDebug() << "❌ Erreur: Le statut est obligatoire";
        return false;
    }

    QStringList gravitesValides;
    gravitesValides << "Faible" << "Moyenne" << "Élevée" << "Critique";

    if (!gravitesValides.contains(gravite)) {
        qDebug() << "❌ Erreur: Gravité invalide";
        return false;
    }

    if (date > QDate::currentDate()) {
        qDebug() << "❌ Erreur: La date ne peut pas être dans le futur";
        return false;
    }

    ColumnNames cols = getColumnNames();
    QSqlQuery query;
    
    QString updateQuery = QString("UPDATE ALERTE SET %1=:type, %2=:id_client, %3=:statut, "
                                  "%4=:description, %5=:gravite, %6=:date "
                                  "WHERE %7=:id")
                          .arg(cols.type, cols.idClient, cols.statut, 
                               cols.description.isEmpty() ? "" : cols.description,
                               cols.gravite, cols.date, cols.id);
    
    query.prepare(updateQuery);
    query.bindValue(":id", id);
    query.bindValue(":type", type);
    query.bindValue(":id_client", id_client);
    query.bindValue(":statut", statut);
    if (!cols.description.isEmpty()) {
        query.bindValue(":description", description);
    }
    query.bindValue(":gravite", gravite);
    query.bindValue(":date", date);

    if(query.exec()) {
        qDebug() << "✅ Alerte modifiée avec succès!";
        return true;
    } else {
        qDebug() << "❌ Erreur modification:" << query.lastError().text();
        return false;
    }
}

// ==================== DELETE ====================

bool Alerte::supprimer(int id) {
    qDebug() << "=== SUPPRESSION ALERTE ID:" << id << "===";

    ColumnNames cols = getColumnNames();
    QSqlQuery query;
    query.prepare(QString("DELETE FROM ALERTE WHERE %1 = :id").arg(cols.id));
    query.bindValue(":id", id);

    if(query.exec()) {
        qDebug() << "✅ Alerte supprimée avec succès!";
        return true;
    } else {
        qDebug() << "❌ Erreur suppression:" << query.lastError().text();
        return false;
    }
}

// ==================== MÉTIERS ====================

QSqlQueryModel* Alerte::rechercher(QString critere) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    ColumnNames cols = getColumnNames();

    // Recherche uniquement par TYPE
    query.prepare(QString("SELECT %1, %2, %3, %4, %5, %6, %7 FROM ALERTE WHERE "
                  "UPPER(%2) LIKE :critere")
                  .arg(cols.id, cols.type, cols.idClient, cols.statut, cols.description, cols.gravite, cols.date));
    query.bindValue(":critere", "%" + critere.toUpper() + "%");
    query.exec();

    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Gravité"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date"));

    return model;
}

QSqlQueryModel* Alerte::trierPar(QString colonne, QString ordre) {
    QSqlQueryModel* model = new QSqlQueryModel();
    ColumnNames cols = getColumnNames();
    
    // Mapper les noms de colonnes logiques vers les noms réels
    QMap<QString, QString> columnMap;
    columnMap["ID"] = cols.id;
    columnMap["TYPE"] = cols.type;
    columnMap["ID_CLIENT"] = cols.idClient;
    columnMap["CLIENT"] = cols.idClient;
    columnMap["STATUT"] = cols.statut;
    columnMap["DESCRIPTION"] = cols.description;
    columnMap["GRAVITE"] = cols.gravite;
    columnMap["DATE_ALERTE"] = cols.date;
    columnMap["DATE_ET_HEURE"] = cols.date;
    columnMap["DATE"] = cols.date;
    
    QString realColumn = columnMap.value(colonne.toUpper(), colonne.toUpper());
    
    // Valider l'ordre
    QString ordreUpper = ordre.toUpper();
    if (ordreUpper != "ASC" && ordreUpper != "DESC") {
        ordreUpper = "ASC";
    }
    
    QString queryStr = QString("SELECT %1, %2, %3, %4, %5, %6, %7 FROM ALERTE ORDER BY %8 %9")
                       .arg(cols.id, cols.type, cols.idClient, cols.statut, cols.description, cols.gravite, cols.date)
                       .arg(realColumn)
                       .arg(ordreUpper);
    model->setQuery(queryStr);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Gravité"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date"));

    return model;
}

QSqlQueryModel* Alerte::filtrerParGravite(QString grav) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    ColumnNames cols = getColumnNames();

    query.prepare(QString("SELECT %1, %2, %3, %4, %5, %6, %7 FROM ALERTE WHERE %6 = :gravite")
                  .arg(cols.id, cols.type, cols.idClient, cols.statut, cols.description, cols.gravite, cols.date));
    query.bindValue(":gravite", grav);
    query.exec();

    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Gravité"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date"));

    return model;
}

int Alerte::compterAlertes() {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM ALERTE");

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        qDebug() << "Nombre total d'alertes:" << count;
        return count;
    }

    return 0;
}

int Alerte::compterParGravite(QString grav) {
    ColumnNames cols = getColumnNames();
    QSqlQuery query;
    query.prepare(QString("SELECT COUNT(*) FROM ALERTE WHERE %1 = :gravite").arg(cols.gravite));
    query.bindValue(":gravite", grav);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}

// ==================== HELPER FUNCTION ====================

Alerte::ColumnNames Alerte::getColumnNames() {
    ColumnNames cols;
    
    QSqlQuery checkColumns;
    checkColumns.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'ALERTE' ORDER BY COLUMN_ID");
    
    QStringList existingColumns;
    if (checkColumns.exec()) {
        while (checkColumns.next()) {
            existingColumns << checkColumns.value(0).toString().toUpper();
        }
    }
    
    // Mapper les colonnes réelles
    cols.id = existingColumns.contains("ID_AL") ? "ID_AL" : 
              existingColumns.contains("ID") ? "ID" : 
              existingColumns.contains("ID_ALERTE") ? "ID_ALERTE" : "ID_AL";
    cols.type = existingColumns.contains("TYPE") ? "TYPE" : "TYPE";
    cols.idClient = existingColumns.contains("CLIENT") ? "CLIENT" : 
                    existingColumns.contains("ID_CLIENT") ? "ID_CLIENT" : 
                    existingColumns.contains("IDCLIENT") ? "IDCLIENT" : "CLIENT";
    cols.statut = existingColumns.contains("STATUT") ? "STATUT" : "STATUT";
    cols.description = existingColumns.contains("DESCRIPTION") ? "DESCRIPTION" : "DESCRIPTION";
    cols.gravite = existingColumns.contains("GRAVITE") ? "GRAVITE" : "GRAVITE";
    cols.date = existingColumns.contains("DATE_ET_HEURE") ? "DATE_ET_HEURE" :
                existingColumns.contains("DATE_ALERTE") ? "DATE_ALERTE" :
                existingColumns.contains("DATEALERTE") ? "DATEALERTE" :
                existingColumns.contains("DATE") ? "DATE" : "DATE_ET_HEURE";
    
    return cols;
}

