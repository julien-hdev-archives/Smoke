import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtQml 2.15

import SceneGraphRendering 1.0
import "content"
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
                id : viewportControllers
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight: 10 // %

                color: Palette.window
                

                Grid {
                    id: viewportControllersGrid
                    x: 4; anchors.bottom: parent.bottom; anchors.bottomMargin: 4
                    rows: 1; columns: 6; spacing: 3

                    Button {
                         text: "Undo"
                         //onClicked: model.submit()
                    }

                    Button {
                         text: "Redo"
                         //onClicked: model.submit()
                    }

                    Button {
                         text: "Cube"
                         //onClicked: model.submit()
                    }

                
                    Button {
                         text: "Dome"
                         //onClicked: model.submit()
                    }

                
                    Button {
                         text: "Ponctual Light"
                         //onClicked: model.submit()
                    }

                                
                    Button {
                         text: "Global Light"
                         //onClicked: model.submit()
                    }
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
            id : sceneHierarchy
                
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight : 35 // %

  
                    color : Palette.window

                    Rectangle {
                        id : head
                        anchors.top : parent.top
                        height: 40
                        
                        color :  "#FFFFFF" 

                        
                        Text {
                            text: "Scene Hierarchy"
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
            }

            RowLayout {
            id : componentControllers

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight : 65 // %

                    color : Palette.window


  







/*
        Text {
            font.pixelSize: 42
            Behavior on x { NumberAnimation{ easing.type: Easing.OutCubic} }
            x: backButton.x + backButton.width + 20
            anchors.verticalCenter: parent.verticalCenter
            color: "white"
            text: "Widget Gallery"
        }*/











     BorderImage {
        border.bottom: 8

        width: parent.width
        height: 100

        Rectangle {
            id: backButton
            width: opacity ? 60 : 0
            anchors.left: parent.left
            anchors.leftMargin: 20
            opacity: stackView.depth > 1 ? 1 : 0
            anchors.verticalCenter: parent.verticalCenter
            antialiasing: true
            height: 60
            radius: 4
            color: backmouse.pressed ? "#222" : "transparent"
            Behavior on opacity { NumberAnimation{} }
            Image {
                anchors.verticalCenter: parent.verticalCenter
                source: "images/navigation_previous_item.png"
            }
            MouseArea {
                id: backmouse
                anchors.fill: parent
                anchors.margins: -10
                onClicked: stackView.pop()
            }
        }


     ListModel {
        id: pageModel
        ListElement {
            title: "Buttons"
            page: "content/ButtonPage.qml"
        }
        ListElement {
            title: "Sliders"
            page: "content/SliderPage.qml"
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        // Implements back key navigation
        focus: true
        Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                             stackView.pop();
                             event.accepted = true;
                         }

        initialItem: Item {
            width: parent.width
            height: parent.height
            ListView {
                model: pageModel
                anchors.fill: parent
                delegate: AndroidDelegate {
                    text: title
                    onClicked: stackView.push(Qt.resolvedUrl(page))
                }
            }
        }
    }




                      }
                    /*
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
                    }*/
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