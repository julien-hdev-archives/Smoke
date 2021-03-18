import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Utils 1.0

Item {   
    property var attr
    id : buttonInt 
    Rectangle {
        anchors.fill : parent
        RowLayout {
            Button {
                width: parent.width; height: 45
                Layout.leftMargin : 10
                Layout.topMargin : 10

                text: attr.label
                onClicked: {attr.value = 1}
                font.pixelSize: 15
                font.family: Fonts.workSans.semiBold.name
            }
        }
    }
}