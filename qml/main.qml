import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import SceneGraphRendering 1.0

ApplicationWindow {
    visible: true
    width: 480
    height: 480
    title: qsTr("Smoke Simulator !")

    ColumnLayout {
        anchors.fill: parent

        MyQuickFBO {
            id: fbo
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 380
            MouseArea {
                anchors.fill: parent
                onPressed: fbo.onMousePress()
                onReleased: fbo.onMouseRelease()
                onWheel: fbo.onWheelScroll(wheel.angleDelta.y / 120)
                onPositionChanged: if (pressed) fbo.update()
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 100
            color: "blue"

            Text {
                text: "This rectangle is a QML Item !"
                color: "white"
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}