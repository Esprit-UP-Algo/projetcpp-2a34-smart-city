#ifndef DECHET_H
#define DECHET_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMap>
#include <QVariant>

class Dechet
{
private:
    int cin;
    QDate date_collecte;
    QString lieu_collecte;
    float volume_poids;
    QString etat;
    QString type_dechet;  // Nouveau: type de déchet (organique, plastique, papier, etc.)

public:
    Dechet();
    Dechet(int cin, QDate date_collecte, QString lieu, float vol, QString etat, QString type = "");

    // Getters
    int getCin() const;
    QDate getDateCollecte() const;
    QString getLieuCollecte() const;
    float getVolume() const;
    QString getEtat() const;
    QString getTypeDechet() const;

    // Setters
    void setCin(int c);
    void setDateCollecte(QDate d);
    void setLieuCollecte(QString l);
    void setVolume(float v);
    void setEtat(QString e);
    void setTypeDechet(QString type);

    // CRUD
    bool ajouter();
    bool modifier();
    bool supprimer(int cin);
    QSqlQueryModel* afficher();

    // Recherche / Tri
    QSqlQueryModel* rechercher(QString critere, QString valeur);
    QSqlQueryModel* trier(QString critere);
    
    // Advanced Filtering
    QSqlQueryModel* filtrerParDateRange(QDate dateDebut, QDate dateFin);
    QSqlQueryModel* filtrerParVolumeRange(float volumeMin, float volumeMax);
    QSqlQueryModel* filtrerMultiCritere(QDate dateDebut, QDate dateFin, QString lieu, QString type, float volumeMin, float volumeMax);
    QSqlQueryModel* filtrerParType(QString type);

    // STATISTICS FUNCTIONS
    // Basic Statistics
    int getTotalCollections();
    double getTotalVolume();
    double getAverageVolume();

    // Location Statistics
    QMap<QString, int> getCollectionsByLocation();
    QMap<QString, double> getVolumeByLocation();

    // Status/State Statistics
    QMap<QString, int> getCollectionsByStatus();
    QMap<QString, double> getVolumeByStatus();

    // Date/Time Statistics
    QMap<QString, int> getCollectionsByMonth();
    QMap<QString, int> getCollectionsByYear();

    // Recycling Statistics
    int getRecycledCount();
    int getNonRecycledCount();
    double getRecyclingRate();

    // Quality Statistics
    int getGoodQualityCount();
    int getBadQualityCount();

    // Advanced Statistics
    QMap<QString, double> getAverageVolumeByLocation();
    QMap<QString, double> getAverageVolumeByStatus();

    // Comprehensive Statistics Report
    QMap<QString, QVariant> getAllStatistics();
    
    // Waste Type Statistics
    QMap<QString, int> getCollectionsByType();
    QMap<QString, double> getVolumeByType();
    double getRecyclingRateByType(QString type);
    
    // Trend Analysis
    QMap<QString, int> getCollectionsByDay();
    QMap<QString, int> getCollectionsByWeek();
    QMap<QString, double> getVolumeTrendByMonth();
    
    // Environmental Impact
    double calculateCO2Saved();  // CO2 saved through recycling (kg)
    double calculateEnergySaved(); // Energy equivalent saved (kWh)
    double getEnvironmentalImpactScore();
    
    // Collection Efficiency
    double getAverageCollectionsPerDay();
    int getCollectionsInDateRange(QDate start, QDate end);
    double getAverageVolumePerCollection();
    
    // Export Functions
    bool exportToCSV(QString filename, QSqlQueryModel* model = nullptr);
    bool exportToExcel(QString filename, QSqlQueryModel* model = nullptr);
    
    // Alerts
    QList<QString> getHighVolumeAlerts(float threshold = 1000.0);
    QList<QString> getOverdueCollections(int daysThreshold = 7);
};

#endif // DECHET_H
