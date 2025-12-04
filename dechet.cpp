#include "dechet.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QMap>
#include <QVariant>
#include <QFile>
#include <QTextStream>
#include <QDate>

Dechet::Dechet()
{
    cin = 0;
    volume_poids = 0;
    etat = "";
    type_dechet = "";
}

Dechet::Dechet(int cin, QDate date_collecte, QString lieu, float vol, QString etat, QString type)
{
    this->cin = cin;
    this->date_collecte = date_collecte;
    this->lieu_collecte = lieu;
    this->volume_poids = vol;
    this->etat = etat;
    this->type_dechet = type;
}

// Getters
int Dechet::getCin() const { return cin; }
QDate Dechet::getDateCollecte() const { return date_collecte; }
QString Dechet::getLieuCollecte() const { return lieu_collecte; }
float Dechet::getVolume() const { return volume_poids; }
QString Dechet::getEtat() const { return etat; }
QString Dechet::getTypeDechet() const { return type_dechet; }

// Setters
void Dechet::setCin(int c) { cin = c; }
void Dechet::setDateCollecte(QDate d) { date_collecte = d; }
void Dechet::setLieuCollecte(QString l) { lieu_collecte = l; }
void Dechet::setVolume(float v) { volume_poids = v; }
void Dechet::setEtat(QString e) { etat = e; }
void Dechet::setTypeDechet(QString type) { type_dechet = type; }

// CRUD Operations
bool Dechet::ajouter()
{
    QSqlQuery q;
    q.prepare("INSERT INTO DECHETS (CIN, DATE_COLLECTE, LIEU_COLLECTE, VOLUME_POIDS, ETAT) "
              "VALUES (:cin, :datec, :lieu, :vol, :etat)");

    q.bindValue(":cin", cin);
    q.bindValue(":datec", date_collecte);
    q.bindValue(":lieu", lieu_collecte);
    q.bindValue(":vol", volume_poids);
    q.bindValue(":etat", etat);
    
    qDebug() << "=== ATTEMPTING INSERT ===";
    qDebug() << "CIN:" << cin;
    qDebug() << "Date:" << date_collecte.toString("yyyy-MM-dd");
    qDebug() << "Lieu:" << lieu_collecte;
    qDebug() << "Volume:" << volume_poids;
    qDebug() << "Etat:" << etat;
    
    if (!q.exec()) {
        qDebug() << "❌ SQL Error:" << q.lastError().text();
        qDebug() << "Error Code:" << q.lastError().nativeErrorCode();
        qDebug() << "Error Type:" << q.lastError().type();
        return false;
    }
    
    qDebug() << "✅ Insert successful! Rows affected:" << q.numRowsAffected();
    return true;
}

bool Dechet::modifier()
{
    // Vérifier la connexion à la base de données
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) {
            qDebug() << "❌ Erreur connexion base de données";
            return false;
        }
    }
    
    QSqlQuery q;
    q.prepare("UPDATE DECHETS SET DATE_COLLECTE=:datec, LIEU_COLLECTE=:lieu, "
              "VOLUME_POIDS=:vol, ETAT=:etat WHERE CIN=:cin");

    q.bindValue(":cin", cin);
    q.bindValue(":datec", date_collecte);
    q.bindValue(":lieu", lieu_collecte);
    q.bindValue(":vol", volume_poids);
    q.bindValue(":etat", etat);
    
    qDebug() << "=== ATTEMPTING UPDATE ===";
    qDebug() << "CIN:" << cin;
    qDebug() << "Date:" << date_collecte.toString("yyyy-MM-dd");
    qDebug() << "Lieu:" << lieu_collecte;
    qDebug() << "Volume:" << volume_poids;
    qDebug() << "Etat:" << etat;
    
    if (!q.exec()) {
        qDebug() << "❌ SQL Error in modifier():" << q.lastError().text();
        qDebug() << "Error Code:" << q.lastError().nativeErrorCode();
        qDebug() << "Error Type:" << q.lastError().type();
        return false;
    }
    
    int rowsAffected = q.numRowsAffected();
    qDebug() << "✅ Update successful! Rows affected:" << rowsAffected;
    
    if (rowsAffected == 0) {
        qDebug() << "⚠️ Aucune ligne modifiée. Le CIN existe-t-il dans la base?";
        return false;
    }
    
    return true;
}

bool Dechet::supprimer(int cin)
{
    QSqlQuery q;
    q.prepare("DELETE FROM DECHETS WHERE CIN=:cin");
    q.bindValue(":cin", cin);
    
    if (!q.exec()) {
        qDebug() << "❌ SQL Error in supprimer():" << q.lastError().text();
        qDebug() << "Error Code:" << q.lastError().nativeErrorCode();
        return false;
    }
    
    qDebug() << "✅ Delete successful! Rows affected:" << q.numRowsAffected();
    return true;
}

QSqlQueryModel* Dechet::afficher()
{
    QSqlQueryModel *m = new QSqlQueryModel();
    QSqlQuery query;
    
    if (!query.exec("SELECT CIN, DATE_COLLECTE, LIEU_COLLECTE, VOLUME_POIDS, ETAT FROM DECHETS ORDER BY DATE_COLLECTE DESC")) {
        qDebug() << "❌ Error in afficher():" << query.lastError().text();
        qDebug() << "Error Code:" << query.lastError().nativeErrorCode();
        return m; // Return empty model
    }
    
    m->setQuery(query);
    
    if (m->lastError().isValid()) {
        qDebug() << "❌ Model Error:" << m->lastError().text();
    } else {
        qDebug() << "✅ Model created with" << m->rowCount() << "rows";
    }
    
    return m;
}

QSqlQueryModel* Dechet::rechercher(QString critere, QString valeur)
{
    QSqlQueryModel* m = new QSqlQueryModel();
    QSqlQuery query;

    // Vérifier que le critère et la valeur ne sont pas vides
    if (critere.isEmpty() || valeur.isEmpty()) {
        qDebug() << "⚠️ Critère ou valeur de recherche vide";
        return m;
    }

    QString req;
    QString valeurUpper = valeur.toUpper(); // Convertir la valeur en majuscules pour la recherche
    
    // Comparaison insensible à la casse pour le critère
    if (critere.compare("CIN", Qt::CaseInsensitive) == 0) {
        req = "SELECT CIN, DATE_COLLECTE, LIEU_COLLECTE, VOLUME_POIDS, ETAT FROM DECHETS WHERE UPPER(TO_CHAR(CIN)) LIKE :valeur ORDER BY DATE_COLLECTE DESC";
    } else if (critere.compare("LIEU_COLLECTE", Qt::CaseInsensitive) == 0) {
        req = "SELECT CIN, DATE_COLLECTE, LIEU_COLLECTE, VOLUME_POIDS, ETAT FROM DECHETS WHERE UPPER(LIEU_COLLECTE) LIKE :valeur ORDER BY DATE_COLLECTE DESC";
    } else {
        qDebug() << "❌ Critère de recherche invalide:" << critere;
        return m; // Return empty model if invalid criteria
    }
    
    query.prepare(req);
    query.bindValue(":valeur", "%" + valeurUpper + "%");
    
    if (!query.exec()) {
        qDebug() << "❌ Error in rechercher():" << query.lastError().text();
        qDebug() << "   Query:" << req;
        qDebug() << "   Value:" << valeur;
        return m;
    }
    
    m->setQuery(std::move(query));
    
    // Définir les en-têtes de colonnes
    m->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    m->setHeaderData(1, Qt::Horizontal, QObject::tr("Date de collecte"));
    m->setHeaderData(2, Qt::Horizontal, QObject::tr("Lieu de collecte"));
    m->setHeaderData(3, Qt::Horizontal, QObject::tr("Volume/Poids"));
    m->setHeaderData(4, Qt::Horizontal, QObject::tr("État"));
    
    qDebug() << "✅ Recherche effectuée:" << m->rowCount() << "résultat(s) trouvé(s)";
    
    return m;
}

QSqlQueryModel* Dechet::trier(QString critere)
{
    QSqlQueryModel* m = new QSqlQueryModel();
    QSqlQuery query;
    
    // Validate criteria to prevent SQL injection
    QStringList validCriteria = {"CIN", "DATE_COLLECTE", "LIEU_COLLECTE", "VOLUME_POIDS", "ETAT"};
    if (!validCriteria.contains(critere)) {
        qDebug() << "❌ Invalid sort criteria:" << critere;
        return m; // Return empty model
    }
    
    QString req = QString("SELECT CIN, DATE_COLLECTE, LIEU_COLLECTE, VOLUME_POIDS, ETAT "
                          "FROM DECHETS ORDER BY %1 ASC").arg(critere);
    
    if (!query.exec(req)) {
        qDebug() << "❌ Error in trier():" << query.lastError().text();
        return m;
    }
    
    m->setQuery(query);
    return m;
}

// STATISTICS IMPLEMENTATION

// Basic Statistics
int Dechet::getTotalCollections()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM DECHETS");
    if(query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

double Dechet::getTotalVolume()
{
    QSqlQuery query;
    query.prepare("SELECT SUM(VOLUME_POIDS) FROM DECHETS");
    if(query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

double Dechet::getAverageVolume()
{
    int total = getTotalCollections();
    if(total == 0) return 0.0;
    return getTotalVolume() / total;
}

// Location Statistics
QMap<QString, int> Dechet::getCollectionsByLocation()
{
    QMap<QString, int> locationStats;
    QSqlQuery query;
    query.prepare("SELECT LIEU_COLLECTE, COUNT(*) FROM DECHETS GROUP BY LIEU_COLLECTE");

    if(query.exec()) {
        while(query.next()) {
            locationStats[query.value(0).toString()] = query.value(1).toInt();
        }
    }
    return locationStats;
}

QMap<QString, double> Dechet::getVolumeByLocation()
{
    QMap<QString, double> volumeStats;
    QSqlQuery query;
    query.prepare("SELECT LIEU_COLLECTE, SUM(VOLUME_POIDS) FROM DECHETS GROUP BY LIEU_COLLECTE");

    if(query.exec()) {
        while(query.next()) {
            volumeStats[query.value(0).toString()] = query.value(1).toDouble();
        }
    }
    return volumeStats;
}

// Status/State Statistics
QMap<QString, int> Dechet::getCollectionsByStatus()
{
    QMap<QString, int> statusStats;
    QSqlQuery query;
    query.prepare("SELECT ETAT, COUNT(*) FROM DECHETS GROUP BY ETAT");

    if(query.exec()) {
        while(query.next()) {
            statusStats[query.value(0).toString()] = query.value(1).toInt();
        }
    }
    return statusStats;
}

QMap<QString, double> Dechet::getVolumeByStatus()
{
    QMap<QString, double> volumeStats;
    QSqlQuery query;
    query.prepare("SELECT ETAT, SUM(VOLUME_POIDS) FROM DECHETS GROUP BY ETAT");

    if(query.exec()) {
        while(query.next()) {
            volumeStats[query.value(0).toString()] = query.value(1).toDouble();
        }
    }
    return volumeStats;
}

// Date/Time Statistics
QMap<QString, int> Dechet::getCollectionsByMonth()
{
    QMap<QString, int> monthStats;
    QSqlQuery query;

    // For Oracle
    query.prepare("SELECT TO_CHAR(DATE_COLLECTE, 'YYYY-MM'), COUNT(*) FROM DECHETS GROUP BY TO_CHAR(DATE_COLLECTE, 'YYYY-MM') ORDER BY TO_CHAR(DATE_COLLECTE, 'YYYY-MM')");

    if(query.exec()) {
        while(query.next()) {
            monthStats[query.value(0).toString()] = query.value(1).toInt();
        }
    }
    return monthStats;
}

QMap<QString, int> Dechet::getCollectionsByYear()
{
    QMap<QString, int> yearStats;
    QSqlQuery query;

    // For Oracle
    query.prepare("SELECT TO_CHAR(DATE_COLLECTE, 'YYYY'), COUNT(*) FROM DECHETS GROUP BY TO_CHAR(DATE_COLLECTE, 'YYYY') ORDER BY TO_CHAR(DATE_COLLECTE, 'YYYY')");

    if(query.exec()) {
        while(query.next()) {
            yearStats[query.value(0).toString()] = query.value(1).toInt();
        }
    }
    return yearStats;
}

// Recycling Statistics
int Dechet::getRecycledCount()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM DECHETS WHERE ETAT LIKE '%recycle%' OR ETAT = 'recycle'");
    if(query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

int Dechet::getNonRecycledCount()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM DECHETS WHERE ETAT LIKE '%non recycle%' OR ETAT = 'non recycle'");
    if(query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

double Dechet::getRecyclingRate()
{
    int total = getTotalCollections();
    if(total == 0) return 0.0;
    return (static_cast<double>(getRecycledCount()) / total) * 100.0;
}

// Quality Statistics
int Dechet::getGoodQualityCount()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM DECHETS WHERE LOWER(ETAT) LIKE '%bonne%' OR LOWER(ETAT) = 'bonne' OR LOWER(ETAT) = 'ok'");
    if(query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

int Dechet::getBadQualityCount()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM DECHETS WHERE LOWER(ETAT) LIKE '%mauvaise%' OR LOWER(ETAT) = 'mauvaise'");
    if(query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

// Advanced Statistics
QMap<QString, double> Dechet::getAverageVolumeByLocation()
{
    QMap<QString, double> avgStats;
    QSqlQuery query;
    query.prepare("SELECT LIEU_COLLECTE, AVG(VOLUME_POIDS) FROM DECHETS GROUP BY LIEU_COLLECTE");

    if(query.exec()) {
        while(query.next()) {
            avgStats[query.value(0).toString()] = query.value(1).toDouble();
        }
    }
    return avgStats;
}

QMap<QString, double> Dechet::getAverageVolumeByStatus()
{
    QMap<QString, double> avgStats;
    QSqlQuery query;
    query.prepare("SELECT ETAT, AVG(VOLUME_POIDS) FROM DECHETS GROUP BY ETAT");

    if(query.exec()) {
        while(query.next()) {
            avgStats[query.value(0).toString()] = query.value(1).toDouble();
        }
    }
    return avgStats;
}

// Comprehensive Statistics Report
QMap<QString, QVariant> Dechet::getAllStatistics()
{
    QMap<QString, QVariant> allStats;

    // Basic stats
    allStats["total_collections"] = getTotalCollections();
    allStats["total_volume"] = getTotalVolume();
    allStats["average_volume"] = getAverageVolume();

    // Recycling stats
    allStats["recycled_count"] = getRecycledCount();
    allStats["non_recycled_count"] = getNonRecycledCount();
    allStats["recycling_rate"] = getRecyclingRate();

    // Quality stats
    allStats["good_quality_count"] = getGoodQualityCount();
    allStats["bad_quality_count"] = getBadQualityCount();

    // Location stats
    allStats["collections_by_location"] = QVariant::fromValue(getCollectionsByLocation());
    allStats["volume_by_location"] = QVariant::fromValue(getVolumeByLocation());

    // Status stats
    allStats["collections_by_status"] = QVariant::fromValue(getCollectionsByStatus());
    allStats["volume_by_status"] = QVariant::fromValue(getVolumeByStatus());

    // Time-based stats
    allStats["collections_by_month"] = QVariant::fromValue(getCollectionsByMonth());
    allStats["collections_by_year"] = QVariant::fromValue(getCollectionsByYear());

    // Average stats
    allStats["avg_volume_by_location"] = QVariant::fromValue(getAverageVolumeByLocation());
    allStats["avg_volume_by_status"] = QVariant::fromValue(getAverageVolumeByStatus());

    return allStats;
}

// ==================== ADVANCED FILTERING ====================

QSqlQueryModel* Dechet::filtrerParDateRange(QDate dateDebut, QDate dateFin)
{
    QSqlQueryModel* m = new QSqlQueryModel();
    QSqlQuery q;
    q.prepare("SELECT * FROM DECHETS WHERE DATE_COLLECTE BETWEEN :dateDebut AND :dateFin ORDER BY DATE_COLLECTE");
    q.bindValue(":dateDebut", dateDebut);
    q.bindValue(":dateFin", dateFin);
    q.exec();
    m->setQuery(std::move(q));
    return m;
}

QSqlQueryModel* Dechet::filtrerParVolumeRange(float volumeMin, float volumeMax)
{
    QSqlQueryModel* m = new QSqlQueryModel();
    QSqlQuery q;
    q.prepare("SELECT * FROM DECHETS WHERE VOLUME_POIDS BETWEEN :volumeMin AND :volumeMax ORDER BY VOLUME_POIDS");
    q.bindValue(":volumeMin", volumeMin);
    q.bindValue(":volumeMax", volumeMax);
    q.exec();
    m->setQuery(std::move(q));
    return m;
}

QSqlQueryModel* Dechet::filtrerParType(QString type)
{
    QSqlQueryModel* m = new QSqlQueryModel();
    QSqlQuery q;
    // Note: This assumes TYPE_DECHET column exists. If not, filter by ETAT
    q.prepare("SELECT * FROM DECHETS WHERE UPPER(ETAT) LIKE :type ORDER BY DATE_COLLECTE");
    q.bindValue(":type", "%" + type.toUpper() + "%");
    q.exec();
    m->setQuery(std::move(q));
    return m;
}

QSqlQueryModel* Dechet::filtrerMultiCritere(QDate dateDebut, QDate dateFin, QString lieu, QString type, float volumeMin, float volumeMax)
{
    QSqlQueryModel* m = new QSqlQueryModel();
    QString req = "SELECT * FROM DECHETS WHERE 1=1";
    QSqlQuery q;
    
    if (dateDebut.isValid() && dateFin.isValid()) {
        req += " AND DATE_COLLECTE BETWEEN :dateDebut AND :dateFin";
    }
    if (!lieu.isEmpty()) {
        req += " AND LIEU_COLLECTE LIKE :lieu";
    }
    if (!type.isEmpty()) {
        req += " AND UPPER(ETAT) LIKE :type";
    }
    if (volumeMin >= 0) {
        req += " AND VOLUME_POIDS >= :volumeMin";
    }
    if (volumeMax > 0) {
        req += " AND VOLUME_POIDS <= :volumeMax";
    }
    req += " ORDER BY DATE_COLLECTE";
    
    q.prepare(req);
    if (dateDebut.isValid() && dateFin.isValid()) {
        q.bindValue(":dateDebut", dateDebut);
        q.bindValue(":dateFin", dateFin);
    }
    if (!lieu.isEmpty()) {
        q.bindValue(":lieu", "%" + lieu + "%");
    }
    if (!type.isEmpty()) {
        q.bindValue(":type", "%" + type.toUpper() + "%");
    }
    if (volumeMin >= 0) {
        q.bindValue(":volumeMin", volumeMin);
    }
    if (volumeMax > 0) {
        q.bindValue(":volumeMax", volumeMax);
    }
    
    q.exec();
    m->setQuery(std::move(q));
    return m;
}

// ==================== WASTE TYPE STATISTICS ====================

QMap<QString, int> Dechet::getCollectionsByType()
{
    QMap<QString, int> typeStats;
    QSqlQuery query;
    // Group by ETAT as type indicator (recyclé/non recyclé can indicate type)
    query.prepare("SELECT ETAT, COUNT(*) FROM DECHETS GROUP BY ETAT");
    
    if(query.exec()) {
        while(query.next()) {
            typeStats[query.value(0).toString()] = query.value(1).toInt();
        }
    }
    return typeStats;
}

QMap<QString, double> Dechet::getVolumeByType()
{
    QMap<QString, double> volumeStats;
    QSqlQuery query;
    query.prepare("SELECT ETAT, SUM(VOLUME_POIDS) FROM DECHETS GROUP BY ETAT");
    
    if(query.exec()) {
        while(query.next()) {
            volumeStats[query.value(0).toString()] = query.value(1).toDouble();
        }
    }
    return volumeStats;
}

double Dechet::getRecyclingRateByType(QString type)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM DECHETS WHERE UPPER(ETAT) LIKE :type");
    query.bindValue(":type", "%" + type.toUpper() + "%");
    
    int typeCount = 0;
    if(query.exec() && query.next()) {
        typeCount = query.value(0).toInt();
    }
    
    int total = getTotalCollections();
    if(total == 0) return 0.0;
    return (static_cast<double>(typeCount) / total) * 100.0;
}

// ==================== TREND ANALYSIS ====================

QMap<QString, int> Dechet::getCollectionsByDay()
{
    QMap<QString, int> dayStats;
    QSqlQuery query;
    // For Oracle
    query.prepare("SELECT TO_CHAR(DATE_COLLECTE, 'YYYY-MM-DD'), COUNT(*) FROM DECHETS GROUP BY TO_CHAR(DATE_COLLECTE, 'YYYY-MM-DD') ORDER BY TO_CHAR(DATE_COLLECTE, 'YYYY-MM-DD')");
    
    if(query.exec()) {
        while(query.next()) {
            dayStats[query.value(0).toString()] = query.value(1).toInt();
        }
    }
    return dayStats;
}

QMap<QString, int> Dechet::getCollectionsByWeek()
{
    QMap<QString, int> weekStats;
    QSqlQuery query;
    // For Oracle - get week number
    query.prepare("SELECT TO_CHAR(DATE_COLLECTE, 'YYYY-WW'), COUNT(*) FROM DECHETS GROUP BY TO_CHAR(DATE_COLLECTE, 'YYYY-WW') ORDER BY TO_CHAR(DATE_COLLECTE, 'YYYY-WW')");
    
    if(query.exec()) {
        while(query.next()) {
            weekStats[query.value(0).toString()] = query.value(1).toInt();
        }
    }
    return weekStats;
}

QMap<QString, double> Dechet::getVolumeTrendByMonth()
{
    QMap<QString, double> trendStats;
    QSqlQuery query;
    // For Oracle
    query.prepare("SELECT TO_CHAR(DATE_COLLECTE, 'YYYY-MM'), SUM(VOLUME_POIDS) FROM DECHETS GROUP BY TO_CHAR(DATE_COLLECTE, 'YYYY-MM') ORDER BY TO_CHAR(DATE_COLLECTE, 'YYYY-MM')");
    
    if(query.exec()) {
        while(query.next()) {
            trendStats[query.value(0).toString()] = query.value(1).toDouble();
        }
    }
    return trendStats;
}

// ==================== ENVIRONMENTAL IMPACT ====================

double Dechet::calculateCO2Saved()
{
    // Estimate: 1 kg of recycled waste saves approximately 2.5 kg CO2
    double recycledVolume = 0.0;
    QSqlQuery query;
    query.prepare("SELECT SUM(VOLUME_POIDS) FROM DECHETS WHERE UPPER(ETAT) LIKE '%RECYCLE%'");
    
    if(query.exec() && query.next()) {
        recycledVolume = query.value(0).toDouble();
    }
    
    return recycledVolume * 2.5; // kg CO2 saved
}

double Dechet::calculateEnergySaved()
{
    // Estimate: 1 kg of recycled waste saves approximately 5 kWh
    double recycledVolume = 0.0;
    QSqlQuery query;
    query.prepare("SELECT SUM(VOLUME_POIDS) FROM DECHETS WHERE UPPER(ETAT) LIKE '%RECYCLE%'");
    
    if(query.exec() && query.next()) {
        recycledVolume = query.value(0).toDouble();
    }
    
    return recycledVolume * 5.0; // kWh saved
}

double Dechet::getEnvironmentalImpactScore()
{
    // Score based on recycling rate (0-100)
    double recyclingRate = getRecyclingRate();
    double co2Saved = calculateCO2Saved();
    
    // Normalize: recycling rate (0-100) + CO2 impact (normalized to 0-50)
    double co2Score = qMin(50.0, (co2Saved / 1000.0) * 10.0); // Max 50 points for CO2
    return recyclingRate * 0.5 + co2Score; // Max 100 points
}

// ==================== COLLECTION EFFICIENCY ====================

double Dechet::getAverageCollectionsPerDay()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(DISTINCT DATE_COLLECTE) FROM DECHETS");
    
    int distinctDays = 0;
    if(query.exec() && query.next()) {
        distinctDays = query.value(0).toInt();
    }
    
    if(distinctDays == 0) return 0.0;
    return static_cast<double>(getTotalCollections()) / distinctDays;
}

int Dechet::getCollectionsInDateRange(QDate start, QDate end)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM DECHETS WHERE DATE_COLLECTE BETWEEN :start AND :end");
    query.bindValue(":start", start);
    query.bindValue(":end", end);
    
    if(query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

double Dechet::getAverageVolumePerCollection()
{
    return getAverageVolume();
}

// ==================== EXPORT FUNCTIONS ====================

bool Dechet::exportToCSV(QString filename, QSqlQueryModel* model)
{
    if(!model) {
        model = afficher();
    }
    
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream out(&file);
    
    // Write headers
    for(int col = 0; col < model->columnCount(); ++col) {
        if(col > 0) out << ",";
        out << "\"" << model->headerData(col, Qt::Horizontal).toString() << "\"";
    }
    out << "\n";
    
    // Write data
    for(int row = 0; row < model->rowCount(); ++row) {
        for(int col = 0; col < model->columnCount(); ++col) {
            if(col > 0) out << ",";
            QString value = model->data(model->index(row, col)).toString();
            value.replace("\"", "\"\""); // Escape quotes
            out << "\"" << value << "\"";
        }
        out << "\n";
    }
    
    file.close();
    return true;
}

bool Dechet::exportToExcel(QString filename, QSqlQueryModel* model)
{
    // For Excel, we'll create a CSV file with .xls extension
    // True Excel export would require a library like QXlsx
    QString csvFilename = filename;
    if(!csvFilename.endsWith(".csv") && !csvFilename.endsWith(".xls")) {
        csvFilename += ".csv";
    }
    
    return exportToCSV(csvFilename, model);
}

// ==================== ALERTS ====================

QList<QString> Dechet::getHighVolumeAlerts(float threshold)
{
    QList<QString> alerts;
    QSqlQuery query;
    query.prepare("SELECT CIN, LIEU_COLLECTE, VOLUME_POIDS, DATE_COLLECTE FROM DECHETS WHERE VOLUME_POIDS > :threshold ORDER BY VOLUME_POIDS DESC");
    query.bindValue(":threshold", threshold);
    
    if(query.exec()) {
        while(query.next()) {
            QString alert = QString("⚠️ Volume élevé: CIN %1 à %2 - %3 kg (Date: %4)")
                .arg(query.value(0).toString())
                .arg(query.value(1).toString())
                .arg(query.value(2).toString())
                .arg(query.value(3).toDate().toString("dd/MM/yyyy"));
            alerts.append(alert);
        }
    }
    return alerts;
}

QList<QString> Dechet::getOverdueCollections(int daysThreshold)
{
    QList<QString> alerts;
    QDate thresholdDate = QDate::currentDate().addDays(-daysThreshold);
    
    QSqlQuery query;
    query.prepare("SELECT CIN, LIEU_COLLECTE, DATE_COLLECTE FROM DECHETS WHERE DATE_COLLECTE < :thresholdDate ORDER BY DATE_COLLECTE");
    query.bindValue(":thresholdDate", thresholdDate);
    
    if(query.exec()) {
        while(query.next()) {
            QDate collectDate = query.value(2).toDate();
            int daysOld = collectDate.daysTo(QDate::currentDate());
            QString alert = QString("⏰ Collection ancienne: CIN %1 à %2 - %3 jours (Date: %4)")
                .arg(query.value(0).toString())
                .arg(query.value(1).toString())
                .arg(daysOld)
                .arg(collectDate.toString("dd/MM/yyyy"));
            alerts.append(alert);
        }
    }
    return alerts;
}
