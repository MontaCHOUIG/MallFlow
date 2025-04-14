import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15

Map {
    id: map
    anchors.fill: parent
    plugin: Plugin { name: "osm" }
    center: QtPositioning.coordinate(36.8065, 10.1815) // Coordonnées par défaut
    zoomLevel: 12

    // Déclarer les fonctions comme invocables
    signal clearMarkers()
    signal addMarker(real lat, real lon)
    signal centerOn(real lat, real lon)

    // Implémentation des fonctions
    onClearMarkers: markerModel.clear()
    onAddMarker: markerModel.append({ "lat": lat, "lon": lon })
    onCenterOn: {
        map.center = QtPositioning.coordinate(lat, lon);
        map.zoomLevel = 15;
    }

    // Modèle pour les marqueurs
    property ListModel markerModel: ListModel {}

    // Affichage des marqueurs
    MapItemView {
        model: markerModel
        delegate: MapQuickItem {
            coordinate: QtPositioning.coordinate(lat, lon)
            sourceItem: Image {
                source: "qrc:/imgs/pin.png"
                width: 32
                height: 32
            }
        }
    }
}
