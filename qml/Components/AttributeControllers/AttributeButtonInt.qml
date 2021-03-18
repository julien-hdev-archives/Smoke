import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Utils 1.0

Item {   
        property var attr
        id : buttonInt 
        Rectangle {
            Button {
                width: 180; height: 20
                font.family: Icons.fontFamily
                text: attributLabel
                onClicked: attr.value = 1
            }
        }
}