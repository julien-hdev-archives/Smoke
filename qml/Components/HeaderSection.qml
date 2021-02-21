import QtQuick 2.0
import Utils 1.0

Item {

    id: headerSection
    property alias text: headerText.text

    Rectangle {
        anchors.fill: parent
        color :  Palette.darkestGrey
             
        Text {
            id : headerText
            text: modelData
            color : Palette.windowText
            anchors.fill: parent
            anchors.leftMargin: 10;

            horizontalAlignment: Text.AlignHLeft
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 18
            font.family: Fonts.workSans.semiBold.name
        }

        Rectangle {
            id : borderBottom
            height: 1
            color : "#FFFFFF"
            anchors.bottom : parent.bottom
        }
                    
    }
}