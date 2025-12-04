#include "alertemap.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QTimer>
#include <QRandomGenerator>
#include <QFile>
#include <QFileInfo>
#ifdef LOCATION_AVAILABLE
#include <QGeoServiceProvider>
#include <QDir>
#include <QCoreApplication>
#include <QLibraryInfo>
#endif

AlerteMap::AlerteMap(QWidget *parent)
    : QWidget(parent)
    , m_mapWidget(nullptr)
    , m_bridge(nullptr)
{
    // Vérifier la disponibilité du plugin OSM
    qDebug() << "=== VÉRIFICATION DU PLUGIN OSM ===";
    QStringList availableProviders = getAvailableGeoServiceProviders();
    qDebug() << "📋 Plugins de service géographique disponibles:" << availableProviders;
    
    bool osmAvailable = checkOSMPluginAvailable();
    if (!osmAvailable) {
        qDebug() << "❌ Plugin OSM non disponible!";
        qDebug() << "   Plugins disponibles:" << availableProviders.join(", ");
    } else {
        qDebug() << "✅ Plugin OSM disponible";
    }
    
    // Créer le bridge pour la communication C++/QML
    m_bridge = new AlertMapBridge(this);
    connect(m_bridge, &AlertMapBridge::markerClicked, this, &AlerteMap::onMarkerClicked);
    
    // Créer le layout principal
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    
    // Créer le QQuickWidget pour afficher la carte
    m_mapWidget = new QQuickWidget(this);
    m_mapWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    
    // Enregistrer le bridge dans le contexte QML
    m_mapWidget->rootContext()->setContextProperty("alertMapBridge", m_bridge);
    
    // Charger le fichier QML de la carte
    // Essayer d'abord depuis le système de fichiers, sinon depuis les ressources
    QUrl qmlUrl;
    QFile qmlFile("qml/AlertMap.qml");
    if (qmlFile.exists()) {
        // Utiliser le fichier local s'il existe (pour le développement)
        qmlUrl = QUrl::fromLocalFile(QFileInfo(qmlFile).absoluteFilePath());
        qDebug() << "✅ Chargement de la carte depuis le système de fichiers:" << qmlUrl.toString();
    } else {
        // Utiliser les ressources compilées
        qmlUrl = QUrl("qrc:/qml/AlertMap.qml");
        qDebug() << "✅ Chargement de la carte depuis les ressources:" << qmlUrl.toString();
    }
    
    m_mapWidget->setSource(qmlUrl);
    
    // Vérifier les erreurs QML
    if (m_mapWidget->status() == QQuickWidget::Error) {
        qDebug() << "❌ Erreur lors du chargement de la carte QML:";
        qDebug() << "   URL utilisée:" << qmlUrl.toString();
        qDebug() << "   Fichier local existe:" << qmlFile.exists();
        for (const QQmlError &error : m_mapWidget->errors()) {
            qDebug() << "  " << error.toString();
            qDebug() << "     Fichier:" << error.url().toString();
            qDebug() << "     Ligne:" << error.line();
            qDebug() << "     Message:" << error.description();
        }
        
        // Afficher un message d'erreur à la place avec plus de détails
        QStringList availableProviders = getAvailableGeoServiceProviders();
        bool osmAvailable = checkOSMPluginAvailable();
        
        QString pluginInfo;
        if (osmAvailable) {
            pluginInfo = "✅ Plugin OSM: Disponible";
        } else {
            pluginInfo = QString("❌ Plugin OSM: Non disponible\n   Plugins disponibles: %1")
                         .arg(availableProviders.isEmpty() ? "Aucun" : availableProviders.join(", "));
        }
        
        QString errorDetails = QString(
            "⚠️ Impossible de charger la carte.\n\n"
            "Détails techniques:\n"
            "- URL utilisée: %1\n"
            "- Fichier local existe: %2\n"
            "- %3\n\n"
            "Solutions possibles:\n"
            "1. Vérifiez que Qt Location est installé:\n"
            "   - Ouvrez Qt Maintenance Tool\n"
            "   - Installez le module 'Qt Location'\n"
            "   - Recompilez l'application\n\n"
            "2. Vérifiez que le plugin OSM est disponible:\n"
            "   - Windows: Cherchez 'qmlgeoservice_osm.dll' dans:\n"
            "     C:\\Qt\\6.x.x\\mingw_64\\plugins\\geoservices\\\n"
            "   - Linux: Cherchez 'libqmlgeoservice_osm.so' dans:\n"
            "     /usr/lib/qt6/plugins/geoservices/\n"
            "   - Si le fichier n'existe pas, réinstallez Qt Location\n\n"
            "3. Vérifiez que AlertMap.qml est dans resources.qrc\n"
            "   et que l'application a été recompilée.\n\n"
            "4. Vérifiez votre connexion Internet\n"
            "   (OpenStreetMap nécessite Internet).\n\n"
            "5. Consultez les logs de la console pour plus de détails."
        ).arg(qmlUrl.toString())
         .arg(qmlFile.exists() ? "Oui" : "Non")
         .arg(pluginInfo);
        
        QLabel *errorLabel = new QLabel(errorDetails, this);
        errorLabel->setAlignment(Qt::AlignCenter);
        errorLabel->setStyleSheet("color: red; font-size: 14px; padding: 20px;");
        errorLabel->setWordWrap(true);
        layout->addWidget(errorLabel);
    } else {
        layout->addWidget(m_mapWidget);
        
        // Connecter le signal statusChanged pour être notifié quand la carte est prête
        connect(m_mapWidget, &QQuickWidget::statusChanged, [this](QQuickWidget::Status status) {
            qDebug() << "📊 Statut de la carte changé:" << status;
            if (status == QQuickWidget::Ready) {
                qDebug() << "✅ La carte est prête, chargement des alertes...";
                // Charger les alertes une fois que la carte est prête
                QTimer::singleShot(500, this, &AlerteMap::loadAlerts);
            }
        });
    }
    
    // Charger les alertes au démarrage (attendre que la carte soit prête)
    QTimer::singleShot(1000, this, &AlerteMap::loadAlerts);
}

AlerteMap::~AlerteMap()
{
}

void AlerteMap::loadAlerts()
{
    qDebug() << "=== CHARGEMENT DES ALERTES POUR LA CARTE ===";
    
    // Vérifier que la carte est prête
    if (!m_mapWidget) {
        qDebug() << "❌ m_mapWidget est null, impossible de charger les alertes";
        return;
    }
    
    if (m_mapWidget->status() == QQuickWidget::Error) {
        qDebug() << "❌ Erreur dans le widget QML, impossible de charger les alertes";
        for (const QQmlError &error : m_mapWidget->errors()) {
            qDebug() << "  Erreur QML:" << error.toString();
        }
        return;
    }
    
    // Charger les alertes depuis la base de données
    m_alerts = fetchAlertsFromDatabase();
    
    qDebug() << "✅ Nombre d'alertes chargées:" << m_alerts.size();
    
    if (m_alerts.isEmpty()) {
        qDebug() << "⚠️ Aucune alerte trouvée dans la base de données";
        centerMap(36.8065, 10.1815, 10);
        return;
    }
    
    // Attendre que la carte soit complètement chargée avant d'ajouter les marqueurs
    if (m_mapWidget->status() != QQuickWidget::Ready) {
        qDebug() << "⏳ Attente du chargement de la carte... Statut:" << m_mapWidget->status();
        QTimer::singleShot(1000, this, &AlerteMap::loadAlerts);
        return;
    }
    
    // Ajouter chaque alerte sur la carte
    for (const AlertData &alert : m_alerts) {
        addAlert(alert);
    }
    
    // Centrer sur la première alerte
    centerOnFirstAlert();
}

QList<AlertData> AlerteMap::fetchAlertsFromDatabase()
{
    QList<AlertData> alerts;
    
    // Détecter les colonnes disponibles dans la table ALERTE
    QSqlQuery checkColumns;
    checkColumns.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'ALERTE' ORDER BY COLUMN_ID");
    
    QStringList existingColumns;
    if (checkColumns.exec()) {
        while (checkColumns.next()) {
            existingColumns << checkColumns.value(0).toString().toUpper();
        }
        qDebug() << "📍 Colonnes trouvées dans ALERTE:" << existingColumns;
    } else {
        qDebug() << "⚠️ Impossible de vérifier les colonnes de la table ALERTE";
    }
    
    // Déterminer les noms de colonnes réels (comme dans alerte.cpp)
    QString colId = existingColumns.contains("ID_AL") ? "ID_AL" : 
                    existingColumns.contains("ID") ? "ID" : 
                    existingColumns.contains("ID_ALERTE") ? "ID_ALERTE" : "ID_AL";
    QString colType = existingColumns.contains("TYPE") ? "TYPE" : "TYPE";
    QString colIdClient = existingColumns.contains("CLIENT") ? "CLIENT" : 
                          existingColumns.contains("ID_CLIENT") ? "ID_CLIENT" : 
                          existingColumns.contains("IDCLIENT") ? "IDCLIENT" : "CLIENT";
    QString colStatut = existingColumns.contains("STATUT") ? "STATUT" : "STATUT";
    QString colDescription = existingColumns.contains("DESCRIPTION") ? "DESCRIPTION" : "DESCRIPTION";
    QString colGravite = existingColumns.contains("GRAVITE") ? "GRAVITE" : "GRAVITE";
    QString colDate = existingColumns.contains("DATE_ET_HEURE") ? "DATE_ET_HEURE" :
                      existingColumns.contains("DATE_ALERTE") ? "DATE_ALERTE" :
                      existingColumns.contains("DATEALERTE") ? "DATEALERTE" :
                      existingColumns.contains("DATE") ? "DATE" : "DATE_ET_HEURE";
    
    // Vérifier si les colonnes LATITUDE et LONGITUDE existent
    bool hasLatitude = existingColumns.contains("LATITUDE");
    bool hasLongitude = existingColumns.contains("LONGITUDE");
    
    qDebug() << "📍 Colonnes géographiques disponibles: LATITUDE=" << hasLatitude << "LONGITUDE=" << hasLongitude;
    
    // Construire la requête SELECT
    QString selectQuery = QString("SELECT %1, %2, %3, %4, %5, %6, %7")
                          .arg(colId, colType, colIdClient, colStatut, colDescription, colGravite, colDate);
    
    // Ajouter les colonnes géographiques si elles existent
    if (hasLatitude && hasLongitude) {
        selectQuery += QString(", NVL(LATITUDE, 0) as LATITUDE, NVL(LONGITUDE, 0) as LONGITUDE");
    }
    
    selectQuery += " FROM ALERTE ORDER BY " + colId + " DESC";
    
    qDebug() << "📍 Requête SQL:" << selectQuery;
    
    QSqlQuery query;
    query.prepare(selectQuery);
    
    if (!query.exec()) {
        qDebug() << "❌ Erreur lors de la récupération des alertes:" << query.lastError().text();
        return alerts;
    }
    
    int latIndex = -1;
    int lonIndex = -1;
    if (hasLatitude && hasLongitude) {
        latIndex = query.record().indexOf("LATITUDE");
        lonIndex = query.record().indexOf("LONGITUDE");
    }
    
    while (query.next()) {
        AlertData alert;
        alert.id = query.value(colId).toInt();
        alert.type = query.value(colType).toString();
        alert.idClient = query.value(colIdClient).toInt();
        alert.statut = query.value(colStatut).toString();
        alert.description = query.value(colDescription).toString();
        alert.gravite = query.value(colGravite).toString();
        alert.date = query.value(colDate).toDate();
        
        // Récupérer les coordonnées si elles existent, sinon générer des coordonnées par défaut
        if (hasLatitude && hasLongitude && latIndex >= 0 && lonIndex >= 0) {
            alert.latitude = query.value(latIndex).toDouble();
            alert.longitude = query.value(lonIndex).toDouble();
        } else {
            // Générer des coordonnées par défaut autour de Tunis
            alert.latitude = 0.0;
            alert.longitude = 0.0;
        }
        
        // Si les coordonnées sont à 0 ou n'existent pas, générer des coordonnées aléatoires autour de Tunis
        if (alert.latitude == 0.0 && alert.longitude == 0.0) {
            // Générer des coordonnées aléatoires autour de Tunis pour la démonstration
            // Utiliser l'ID de l'alerte pour avoir des coordonnées reproductibles
            int randomLat = (alert.id * 37) % 100 - 50; // Valeur entre -50 et 50
            int randomLon = (alert.id * 41) % 100 - 50;
            alert.latitude = 36.8065 + randomLat / 1000.0; // ±0.05 degrés autour de Tunis
            alert.longitude = 10.1815 + randomLon / 1000.0;
            qDebug() << "📍 Coordonnées générées pour alerte #" << alert.id 
                     << ":" << alert.latitude << "," << alert.longitude;
        }
        
        alerts.append(alert);
    }
    
    qDebug() << "✅ " << alerts.size() << " alertes chargées depuis la base de données";
    
    return alerts;
}

void AlerteMap::addAlert(const AlertData &alert)
{
    if (!m_mapWidget) {
        qDebug() << "⚠️ m_mapWidget est null";
        return;
    }
    
    if (m_mapWidget->status() != QQuickWidget::Ready) {
        qDebug() << "⚠️ La carte n'est pas prête. Statut:" << m_mapWidget->status();
        // Attendre un peu et réessayer
        QTimer::singleShot(500, [this, alert]() {
            addAlert(alert);
        });
        return;
    }
    
    QQuickItem *rootObject = m_mapWidget->rootObject();
    if (!rootObject) {
        qDebug() << "❌ Root object QML introuvable";
        return;
    }
    
    // Appeler la fonction QML pour ajouter un marqueur
    QVariantMap alertMap = alert.toVariantMap();
    alertMap["color"] = getMarkerColor(alert.gravite);
    
    qDebug() << "📍 Ajout du marqueur pour l'alerte #" << alert.id 
             << "à (" << alert.latitude << "," << alert.longitude << ")"
             << "Couleur:" << alertMap["color"].toString();
    
    bool success = QMetaObject::invokeMethod(rootObject, "addMarker",
                              Q_ARG(QVariant, QVariant::fromValue(alertMap)));
    
    if (!success) {
        qDebug() << "❌ Échec de l'invocation de addMarker()";
    }
}

void AlerteMap::removeAlert(int alertId)
{
    if (!m_mapWidget || m_mapWidget->status() != QQuickWidget::Ready) {
        return;
    }
    
    QQuickItem *rootObject = m_mapWidget->rootObject();
    if (!rootObject) {
        return;
    }
    
    QMetaObject::invokeMethod(rootObject, "removeMarker",
                              Q_ARG(QVariant, alertId));
}

void AlerteMap::refreshAlerts()
{
    // Supprimer tous les marqueurs existants
    if (m_mapWidget && m_mapWidget->status() == QQuickWidget::Ready) {
        QQuickItem *rootObject = m_mapWidget->rootObject();
        if (rootObject) {
            QMetaObject::invokeMethod(rootObject, "clearMarkers");
        }
    }
    
    // Recharger les alertes
    loadAlerts();
}

void AlerteMap::centerMap(double latitude, double longitude, int zoomLevel)
{
    if (!m_mapWidget || m_mapWidget->status() != QQuickWidget::Ready) {
        return;
    }
    
    QQuickItem *rootObject = m_mapWidget->rootObject();
    if (!rootObject) {
        return;
    }
    
    QMetaObject::invokeMethod(rootObject, "centerMap",
                              Q_ARG(QVariant, latitude),
                              Q_ARG(QVariant, longitude),
                              Q_ARG(QVariant, zoomLevel));
}

void AlerteMap::centerOnFirstAlert()
{
    if (m_alerts.isEmpty()) {
        return;
    }
    
    const AlertData &firstAlert = m_alerts.first();
    centerMap(firstAlert.latitude, firstAlert.longitude, 12);
}

QString AlerteMap::getMarkerColor(const QString &gravite) const
{
    if (gravite == "Critique") {
        return "#8B0000"; // Dark red
    } else if (gravite == "Élevée") {
        return "#FF0000"; // Red
    } else if (gravite == "Moyenne") {
        return "#FFA500"; // Orange
    } else if (gravite == "Faible") {
        return "#00FF00"; // Green
    }
    return "#808080"; // Gray par défaut
}

void AlerteMap::onMarkerClicked(const QVariantMap &alertData)
{
    AlertData alert;
    alert.id = alertData["id"].toInt();
    alert.type = alertData["type"].toString();
    alert.idClient = alertData["idClient"].toInt();
    alert.statut = alertData["statut"].toString();
    alert.description = alertData["description"].toString();
    alert.gravite = alertData["gravite"].toString();
    alert.date = QDate::fromString(alertData["date"].toString(), "dd/MM/yyyy");
    alert.latitude = alertData["latitude"].toDouble();
    alert.longitude = alertData["longitude"].toDouble();
    
    showAlertDetails(alert);
}

void AlerteMap::showAlertDetails(const AlertData &alert)
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Détails de l'Alerte #" + QString::number(alert.id));
    dialog->setMinimumWidth(400);
    
    QFormLayout *layout = new QFormLayout(dialog);
    
    // Créer les champs en lecture seule
    QLabel *lblId = new QLabel(QString::number(alert.id), dialog);
    QLabel *lblType = new QLabel(alert.type, dialog);
    QLabel *lblClient = new QLabel(QString::number(alert.idClient), dialog);
    QLabel *lblStatut = new QLabel(alert.statut, dialog);
    QLabel *lblDescription = new QLabel(alert.description, dialog);
    lblDescription->setWordWrap(true);
    QLabel *lblGravite = new QLabel(alert.gravite, dialog);
    QLabel *lblDate = new QLabel(alert.date.toString("dd/MM/yyyy"), dialog);
    QLabel *lblCoords = new QLabel(QString("%1, %2").arg(alert.latitude, 0, 'f', 6).arg(alert.longitude, 0, 'f', 6), dialog);
    
    // Appliquer une couleur selon la gravité
    QString color = getMarkerColor(alert.gravite);
    lblGravite->setStyleSheet(QString("color: %1; font-weight: bold;").arg(color));
    
    layout->addRow("ID:", lblId);
    layout->addRow("Type:", lblType);
    layout->addRow("ID Client:", lblClient);
    layout->addRow("Statut:", lblStatut);
    layout->addRow("Description:", lblDescription);
    layout->addRow("Gravité:", lblGravite);
    layout->addRow("Date:", lblDate);
    layout->addRow("Coordonnées:", lblCoords);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
    layout->addRow(buttonBox);
    
    dialog->exec();
    delete dialog;
}

// Vérifier si le plugin OSM est disponible
bool AlerteMap::checkOSMPluginAvailable()
{
#ifdef LOCATION_AVAILABLE
    QStringList providers = getAvailableGeoServiceProviders();
    return providers.contains("osm", Qt::CaseInsensitive);
#else
    return false;
#endif
}

// Obtenir la liste des plugins de service géographique disponibles
QStringList AlerteMap::getAvailableGeoServiceProviders()
{
    QStringList providers;
    
#ifdef LOCATION_AVAILABLE
    // Obtenir la liste des providers disponibles
    providers = QGeoServiceProvider::availableServiceProviders();
    
    qDebug() << "🔍 Recherche des plugins de service géographique...";
    qDebug() << "   Nombre de plugins trouvés:" << providers.size();
    
    // Vérifier aussi les fichiers de plugin dans le système de fichiers
    QStringList pluginPaths;
    
    // Chemin standard des plugins Qt
    QString appDir = QCoreApplication::applicationDirPath();
    QStringList searchPaths;
    
    // Windows
    searchPaths << appDir + "/plugins/geoservices";
    searchPaths << appDir + "/../plugins/geoservices";
    
    // Chemins Qt standards
    QString qtPluginsPath = QLibraryInfo::path(QLibraryInfo::PluginsPath);
    if (!qtPluginsPath.isEmpty()) {
        searchPaths << qtPluginsPath + "/geoservices";
    }
    
    // Chemins personnalisés (selon l'installation)
    #ifdef Q_OS_WIN
    searchPaths << "C:/Qt/*/plugins/geoservices";
    #elif defined(Q_OS_LINUX)
    searchPaths << "/usr/lib/qt6/plugins/geoservices";
    searchPaths << "/usr/local/lib/qt6/plugins/geoservices";
    #elif defined(Q_OS_MAC)
    searchPaths << "/usr/local/lib/qt6/plugins/geoservices";
    #endif
    
    qDebug() << "🔍 Recherche des fichiers de plugin OSM dans:";
    for (const QString &path : searchPaths) {
        QDir dir(path);
        if (dir.exists()) {
            qDebug() << "   ✅ Dossier trouvé:" << path;
            QStringList filters;
            #ifdef Q_OS_WIN
            filters << "*osm*.dll";
            #elif defined(Q_OS_LINUX)
            filters << "*osm*.so";
            #elif defined(Q_OS_MAC)
            filters << "*osm*.dylib";
            #endif
            
            QStringList files = dir.entryList(filters, QDir::Files);
            if (!files.isEmpty()) {
                qDebug() << "      Fichiers OSM trouvés:" << files;
                pluginPaths << files;
            } else {
                qDebug() << "      ❌ Aucun fichier OSM trouvé";
            }
        } else {
            qDebug() << "   ❌ Dossier introuvable:" << path;
        }
    }
    
    if (pluginPaths.isEmpty() && !providers.contains("osm", Qt::CaseInsensitive)) {
        qDebug() << "⚠️ ATTENTION: Plugin OSM non trouvé dans les fichiers système";
        qDebug() << "   Vérifiez que Qt Location est correctement installé";
    }
    
#else
    qDebug() << "❌ Qt Location n'est pas disponible (LOCATION_AVAILABLE non défini)";
#endif
    
    return providers;
}


