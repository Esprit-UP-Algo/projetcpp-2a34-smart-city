#ifndef ALERTEMAP_H
#define ALERTEMAP_H

#include <QWidget>
#include <QQuickWidget>
#include <QQmlContext>
#include <QGeoCoordinate>
#include <QList>
#include <QVariant>
#include <QObject>
#include "alerte.h"

// Structure pour représenter une alerte avec ses coordonnées géographiques
struct AlertData {
    int id;
    QString type;
    int idClient;
    QString statut;
    QString description;
    QString gravite;
    QDate date;
    double latitude;
    double longitude;
    
    AlertData() : id(0), idClient(0), latitude(0.0), longitude(0.0) {}
    
    // Convertir en QVariantMap pour QML
    QVariantMap toVariantMap() const {
        QVariantMap map;
        map["id"] = id;
        map["type"] = type;
        map["idClient"] = idClient;
        map["statut"] = statut;
        map["description"] = description;
        map["gravite"] = gravite;
        map["date"] = date.toString("dd/MM/yyyy");
        map["latitude"] = latitude;
        map["longitude"] = longitude;
        return map;
    }
};

// Bridge class pour communiquer entre C++ et QML
class AlertMapBridge : public QObject
{
    Q_OBJECT
    
public:
    explicit AlertMapBridge(QObject *parent = nullptr) : QObject(parent) {}
    
signals:
    void markerClicked(const QVariantMap &alertData);
    
public slots:
    void onMarkerClicked(const QVariantMap &alertData) {
        emit markerClicked(alertData);
    }
};

class AlerteMap : public QWidget
{
    Q_OBJECT

public:
    explicit AlerteMap(QWidget *parent = nullptr);
    ~AlerteMap();
    
    // Charger les alertes depuis la base de données
    void loadAlerts();
    
    // Ajouter une alerte sur la carte
    void addAlert(const AlertData &alert);
    
    // Supprimer une alerte de la carte
    void removeAlert(int alertId);
    
    // Rafraîchir toutes les alertes
    void refreshAlerts();
    
    // Centrer la carte sur une coordonnée
    void centerMap(double latitude, double longitude, int zoomLevel = 12);
    
    // Centrer sur la première alerte
    void centerOnFirstAlert();

private slots:
    void onMarkerClicked(const QVariantMap &alertData);
    void showAlertDetails(const AlertData &alert);

private:
    QQuickWidget *m_mapWidget;
    AlertMapBridge *m_bridge;
    QList<AlertData> m_alerts;
    
    // Obtenir la couleur du marqueur selon la gravité
    QString getMarkerColor(const QString &gravite) const;
    
    // Charger les alertes depuis la base de données
    QList<AlertData> fetchAlertsFromDatabase();
    
    // Vérifier la disponibilité du plugin OSM
    static bool checkOSMPluginAvailable();
    static QStringList getAvailableGeoServiceProviders();
};

#endif // ALERTEMAP_H

