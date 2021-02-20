import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtQml 2.15

import SceneGraphRendering 1.0

import Utils 1.0

ApplicationWindow {
    title: qsTr("Smoke")
    visible: true

    width: 1440
    height: 720
    minimumWidth: 600
    minimumHeight: 500

    Material.theme: Material.Dark
    Material.accent: Material.Orange

    RowLayout {
        anchors.fill: parent
        spacing : 1
       
        ColumnLayout {
            id : viewport
            spacing : 1

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth : 70 // %

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight: 10 // %

                color: Palette.window
                
                Text {
                    text: "This rectangle is a QML Item !"
                    color : Palette.windowText
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

            }

            MyQuickFBO {
                id: fbo
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight: 90 // %

                MouseArea {
                    anchors.fill: parent
                    onPressed: fbo.onMousePress()
                    onReleased: fbo.onMouseRelease()
                    onWheel: fbo.onWheelScroll(wheel.angleDelta.y / 120)
                    onPositionChanged: if (pressed) fbo.update()
                }
            }

        }

        ColumnLayout {
            id : controllers
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth : 30 // %
            
            spacing : 1

            RowLayout {
            id : ceneHierarchy


                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color : Palette.window

                    Rectangle {
                        id : header
                        Layout.fillWidth: true
                        height: 30
                        color : Palette.midlight

                        Text {
                            text: "Scene Hierarchy"
                            color : Palette.windowText
                            anchors.fill: parent
                            horizontalAlignment: Text.AlignHLeft
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 32
                        }
                    }

                    Text {
                        text: "This rectangle is a QML Item !"
                        color : Palette.windowText
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            RowLayout {
            id : componentControllers

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color : Palette.window



                    Text {
                        text: "This rectangle is a QML Item !"
                        color : Palette.windowText
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    ListView {
                        width: 180; height: 200
                        model: fbo.sdfRendererProperties.attributes

                        delegate: Slider {
                            value: object.value
                            from : 0
                            to : 1
                            stepSize : 0.01
                            onMoved: {
                                object.value = value
                            }
                        }
                    }
                }
            }
        }
    }
}



 /*
                    MenuBar {
                        Menu {
                            title: qsTr("File")
                            MenuItem {
                                text: qsTr("&Open")
                                onTriggered: console.log("Open action triggered");
                            }
                            MenuItem {
                                text: qsTr("Exit")
                                onTriggered: Qt.quit();
                            }
                        }
                    }
                    */