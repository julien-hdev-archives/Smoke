import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtQuick.Window 2.2    
import QtQuick.Dialogs 1.2
import QtQml 2.15

import SceneGraphRendering 1.0
import MyTree 1.0
import Utils 1.0
import Components 1.0


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

            HeaderSection { 
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight : 30 
                Layout.minimumHeight : 25 
                Layout.maximumHeight : 35 
                text : "Viewport"
            }

            Rectangle {
                id : viewportControllers
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight : 35 
                Layout.minimumHeight : 30 
                Layout.maximumHeight : 40 

                color: Palette.window
                
                Grid {
                    id: viewportControllersGrid
                    x: 4; anchors.bottom: parent.bottom; anchors.bottomMargin: 1
                    rows: 1; columns: 6; spacing: 3

                    CButtonIcon { width: 50; height: 40; icon : Icons.undo }
                    CButtonIcon { width: 50; height: 40; icon : Icons.redo }
                    CButtonIcon { width: 50; height: 40; icon : Icons.check_box_outline_blank }
                    CButtonIcon { width: 50; height: 40; icon : Icons.radio_button_unchecked }
                    CButtonIcon { width: 50; height: 40; icon : Icons.lightbulb }
                    CButtonIcon { width: 50; height: 40; icon : Icons.wb_sunny }
                }

                /*
                ListView {
                        anchors.right : parent.right
                        anchors.rightMargin : 30

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
            z: 2
            id : controllers
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth : 35 // %
            spacing : 0

            ColumnLayout {
                z: 2
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight : 35 // %
                id : hierarchy
                spacing : 0

                HeaderSection { 
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight : 30 
                    Layout.minimumHeight : 25 
                    Layout.maximumHeight : 35 
                    text :"Scene Hierarchy" 
                }   

                MyTreeView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight : 85 // %
                }
            }

         
            ColumnLayout {
                id : componentControllers
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight : 65 // %
                spacing : 1

                HeaderSection { 
                    z: 2
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight : 30 
                    Layout.minimumHeight : 25 
                    Layout.maximumHeight : 35 
                    text : "Components"
                }
     
                Rectangle {
                            
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredHeight : 55 // %
                        color : Palette.window
                        z: 1

                        ListView {
                            anchors.fill: parent
                            model: fbo.simulatorProperties.attributes
                            delegate: AttributeLoader { attribute : object }
                        }
                    }                
                }
            }
        }
}
