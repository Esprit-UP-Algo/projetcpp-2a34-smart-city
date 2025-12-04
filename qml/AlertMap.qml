import QtQuick 2.15
import QtQuick.Controls 2.15
import QtLocation 5.15
import QtPositioning 5.15

Item {
    id: root
    
    // Propriétés pour gérer les marqueurs
    property var markers: ({})
    property int markerCounter: 0
    
    // Propriété pour le centre de la carte
    property double centerLatitude: 36.8065  // Tunis par défaut
    property double centerLongitude: 10.1815
    property int zoomLevel: 10
    
    // Plugin de carte OpenStreetMap
    Plugin {
        id: mapPlugin
        name: "osm"
        // Configuration pour utiliser OpenStreetMap directement sans clé API
        // Désactiver le dépôt de fournisseurs qui nécessite une clé API
        PluginParameter {
            name: "osm.mapping.providersrepository.disabled"
            value: "true"
        }
        // Utiliser directement les tuiles OpenStreetMap standard
        PluginParameter {
            name: "osm.mapping.custom.host"
            value: "https://tile.openstreetmap.org/"
        }
        PluginParameter {
            name: "osm.mapping.host"
            value: "https://tile.openstreetmap.org/"
        }
        PluginParameter {
            name: "osm.useragent"
            value: "SmartCityApp/1.0"
        }
        // Désactiver complètement le dépôt de fournisseurs
        PluginParameter {
            name: "osm.mapping.providersrepository.address"
            value: ""
        }
        // Désactiver les messages d'erreur concernant la clé API
        PluginParameter {
            name: "osm.mapping.copyright.hide"
            value: "true"
        }
    }
    
    // Carte principale
    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(centerLatitude, centerLongitude)
        zoomLevel: root.zoomLevel
        
        // MouseArea pour permettre les interactions (zoom, déplacement)
        // Cette approche implémente manuellement le zoom et le déplacement
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            
            // Permettre le zoom avec la molette
            onWheel: function(wheel) {
                var zoomFactor = wheel.angleDelta.y > 0 ? 1.2 : 0.8;
                var newZoom = map.zoomLevel * zoomFactor;
                map.zoomLevel = Math.max(map.minimumZoomLevel, Math.min(map.maximumZoomLevel, newZoom));
                wheel.accepted = true;
            }
            
            // Permettre le déplacement par clic-glisser
            property point pressPoint
            property double pressLatitude: 0.0
            property double pressLongitude: 0.0
            
            onPressed: function(mouse) {
                pressPoint = Qt.point(mouse.x, mouse.y);
                var centerCoord = map.center;
                pressLatitude = centerCoord.latitude;
                pressLongitude = centerCoord.longitude;
                mouse.accepted = true;
            }
            
            onPositionChanged: function(mouse) {
                if (mouse.buttons & Qt.LeftButton && pressPoint) {
                    var dx = mouse.x - pressPoint.x;
                    var dy = mouse.y - pressPoint.y;
                    
                    // Convertir le déplacement en coordonnées géographiques
                    // Calculer le déplacement en degrés
                    var degreesPerPixel = 360.0 / (256.0 * Math.pow(2, map.zoomLevel));
                    var latDelta = -dy * degreesPerPixel;
                    var lonDelta = dx * degreesPerPixel / Math.cos(pressLatitude * Math.PI / 180.0);
                    
                    // Mettre à jour le centre de la carte
                    map.center = QtPositioning.coordinate(
                        pressLatitude + latDelta,
                        pressLongitude + lonDelta
                    );
                }
            }
        }
        
        // Sélectionner explicitement le type de carte OpenStreetMap standard
        Component.onCompleted: {
            // Attendre que les types de carte soient disponibles
            if (map.supportedMapTypes.length > 0) {
                // Chercher le type de carte OpenStreetMap standard (sans filigrane)
                var foundStandard = false;
                for (var i = 0; i < map.supportedMapTypes.length; i++) {
                    var mapType = map.supportedMapTypes[i];
                    // Utiliser le premier type qui n'est pas "satellite" ou "terrain"
                    // et qui ne contient pas "API" dans son nom
                    if (mapType.name && !mapType.name.toLowerCase().includes("satellite") 
                        && !mapType.name.toLowerCase().includes("terrain")
                        && !mapType.name.toLowerCase().includes("api")) {
                        map.activeMapType = mapType;
                        foundStandard = true;
                        console.log("✅ Type de carte sélectionné:", mapType.name);
                        break;
                    }
                }
                // Si aucun type standard trouvé, utiliser le premier disponible
                if (!foundStandard && map.supportedMapTypes.length > 0) {
                    map.activeMapType = map.supportedMapTypes[0];
                    console.log("✅ Type de carte par défaut sélectionné");
                }
            } else {
                console.warn("⚠️ Aucun type de carte disponible");
            }
        }
    }
    
    // Fonction pour ajouter un marqueur
    function addMarker(alertData) {
        console.log("📍 addMarker appelé pour alerte ID:", alertData.id);
        console.log("   Coordonnées:", alertData.latitude, alertData.longitude);
        console.log("   Couleur:", alertData.color);
        
        var markerId = alertData.id;
        var lat = parseFloat(alertData.latitude);
        var lon = parseFloat(alertData.longitude);
        var color = alertData.color || "#FF0000";
        
        if (isNaN(lat) || isNaN(lon)) {
            console.error("❌ Coordonnées invalides:", lat, lon);
            return;
        }
        
        var coordinate = QtPositioning.coordinate(lat, lon);
        
        // Créer un MapQuickItem pour le marqueur
        var mapItem = Qt.createQmlObject(
            'import QtQuick 2.15; ' +
            'import QtLocation 5.15; ' +
            'import QtPositioning 5.15; ' +
            'MapQuickItem { ' +
            '    id: marker' + markerId + '; ' +
            '    coordinate: QtPositioning.coordinate(' + lat + ', ' + lon + '); ' +
            '    anchorPoint.x: markerImage.width / 2; ' +
            '    anchorPoint.y: markerImage.height; ' +
            '    sourceItem: Rectangle { ' +
            '        id: markerImage; ' +
            '        width: 30; ' +
            '        height: 30; ' +
            '        radius: 15; ' +
            '        color: "' + color + '"; ' +
            '        border.color: "#FFFFFF"; ' +
            '        border.width: 2; ' +
            '        Rectangle { ' +
            '            anchors.centerIn: parent; ' +
            '            width: 10; ' +
            '            height: 10; ' +
            '            radius: 5; ' +
            '            color: "#FFFFFF"; ' +
            '        } ' +
            '        MouseArea { ' +
            '            anchors.fill: parent; ' +
            '            onClicked: { ' +
            '                var alertData = { ' +
            '                    id: ' + alertData.id + ', ' +
            '                    type: "' + alertData.type + '", ' +
            '                    idClient: ' + alertData.idClient + ', ' +
            '                    statut: "' + alertData.statut + '", ' +
            '                    description: "' + alertData.description + '", ' +
            '                    gravite: "' + alertData.gravite + '", ' +
            '                    date: "' + alertData.date + '", ' +
            '                    latitude: ' + alertData.latitude + ', ' +
            '                    longitude: ' + alertData.longitude + ' ' +
            '                }; ' +
            '                alertMapBridge.onMarkerClicked(alertData); ' +
            '            } ' +
            '        } ' +
            '    } ' +
            '}',
            map
        );
        
        if (mapItem) {
            map.addMapItem(mapItem);
            markers[markerId] = mapItem;
            console.log("✅ Marqueur ajouté avec succès, ID:", markerId);
        } else {
            console.error("❌ Échec de la création du MapQuickItem");
        }
    }
    
    // Fonction pour supprimer un marqueur
    function removeMarker(markerId) {
        if (markers[markerId]) {
            map.removeMapItem(markers[markerId]);
            markers[markerId].destroy();
            delete markers[markerId];
        }
    }
    
    // Fonction pour supprimer tous les marqueurs
    function clearMarkers() {
        for (var markerId in markers) {
            removeMarker(parseInt(markerId));
        }
    }
    
    // Fonction pour centrer la carte
    function centerMap(lat, lon, zoom) {
        centerLatitude = lat;
        centerLongitude = lon;
        zoomLevel = zoom;
        map.center = QtPositioning.coordinate(lat, lon);
        map.zoomLevel = zoom;
    }
}

