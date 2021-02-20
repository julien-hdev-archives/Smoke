pragma Singleton
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQml 2.15

QtObject {
    
    // Color
    readonly property color lightestGrey: Material.color(Material.Grey, Material.Shade300)
    readonly property color lightGrey: Material.color(Material.Grey, Material.Shade500)
    readonly property color darkGrey: Material.color(Material.Grey, Material.Shade800)
    readonly property color darkestGrey: Material.color(Material.Grey, Material.Shade900)
    readonly property color lightestTeal: Material.color(Material.Teal, Material.Shade100)
    readonly property color middleTeal: Material.color(Material.Teal, Material.Shade400)

    // Theme
    readonly property color windowText: "#D4D4D4"
    readonly property color window: "#252525"
    readonly property color button: "#2D2D2D"
    readonly property color light: "#3C3C3C"
    readonly property color midlight: "#343434"
    readonly property color dark: "#1E1E1E"
    readonly property color mid: "#252525"
    readonly property color activeText: "#D4D4D4"
    readonly property color disabledText: "#7F7F7F"
    readonly property color inactiveText: "#D4D4D4"
    readonly property color brightText: "#F0F0F0"
    readonly property color buttonText: "#D4D4D4"
    readonly property color base: "#3C3C3C"
    readonly property color shadow: "#000000"
    readonly property color highlight: "#262626"
    readonly property color highlightedText: "#F0F0F0"
    readonly property color link: "#7878FF"
    readonly property color linkVisited: "#FF00FF"
    readonly property color alternateBase: "#2D2D2D"
    readonly property color noRole: "#000000"
    readonly property color toolTipBase: "#FFFFDC"
    readonly property color toolTipText: "#000000"

}