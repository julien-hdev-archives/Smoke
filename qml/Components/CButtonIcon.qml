import QtQuick 2.15
import QtQuick.Controls 2.15
import Utils 1.0

Item {
    property alias icon: cButtonIcon.text

    Button {
            anchors.fill : parent
            id: cButtonIcon
            font.family: Icons.fontFamily
            text: modelData
            //onClicked: model.submit()
    }
}