import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls 1.4 as OldControls
import QtQuick.Controls.Styles 1.4 as OldControlsStyles
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtQuick.Window 2.2    
import QtQuick.Dialogs 1.2
import QtQml 2.15

import SceneGraphRendering 1.0
import MyTree 1.0
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

    Component {
        id: fakePlace
        TreeElement {
            property string name: getFakePlaceName()
            property string population: getFakePopulation()
            property string type: "Fake place"
            function getFakePlaceName() {
                var rez = "";
                for(var i = 0;i < Math.round(3 + Math.random() * 7);i ++) {
                    rez += String.fromCharCode(97 + Math.round(Math.random() * 25));
                }
                return rez.charAt(0).toUpperCase() + rez.slice(1);
            }
            function getFakePopulation() {
                var num = Math.round(Math.random() * 100000000);
                num = num.toString().split("").reverse().join("");
                num = num.replace(/(\d{3})/g, '$1,');
                num = num.split("").reverse().join("");
                return num[0] === ',' ? num.slice(1) : num;
            }
        }
    }

    TreeModel {
        id: treemodel
        roles: ["name","population"]

        TreeElement {
            property string name: "Asia"
            property string population: "4,164,252,000"
            property string type: "Continent"
            TreeElement {
                property string name: "China";
                property string population: "1,343,239,923"
                property string type: "Country"
                TreeElement { property string name: "Shanghai"; property string population: "20,217,700"; property string type: "City" }
                TreeElement { property string name: "Beijing"; property string population: "16,446,900"; property string type: "City" }
                TreeElement { property string name: "Chongqing"; property string population: "11,871,200"; property string type: "City" }
            }
            TreeElement {
                property string name: "India";
                property string population: "1,210,193,422"
                property string type: "Country"
                TreeElement { property string name: "Mumbai"; property string population: "12,478,447"; property string type: "City" }
                TreeElement { property string name: "Delhi"; property string population: "11,007,835"; property string type: "City" }
                TreeElement { property string name: "Bengaluru"; property string population: "8,425,970"; property string type: "City" }
            }
            TreeElement {
                property string name: "Indonesia";
                property string population: "248,645,008"
                property string type: "Country"
                TreeElement {property string name: "Jakarta"; property string population: "9,588,198"; property string type: "City" }
                TreeElement {property string name: "Surabaya"; property string population: "2,765,487"; property string type: "City" }
                TreeElement {property string name: "Bandung"; property string population: "2,394,873"; property string type: "City" }
            }
        }
        TreeElement { property string name: "Africa"; property string population: "1,022,234,000"; property string type: "Continent" }
        TreeElement { property string name: "North America"; property string population: "542,056,000"; property string type: "Continent" }
        TreeElement { property string name: "South America"; property string population: "392,555,000"; property string type: "Continent" }
        TreeElement { property string name: "Antarctica"; property string population: "4,490"; property string type: "Continent" }
        TreeElement { property string name: "Europe"; property string population: "738,199,000"; property string type: "Continent" }
        TreeElement { property string name: "Australia"; property string population: "29,127,000"; property string type: "Continent" }
    }





      
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
                         font.family: Icons.fontFamily
                         text: Icons.undo
                         width: 50
                         //onClicked: model.submit()
                    }

                    Button {
                         font.family: Icons.fontFamily
                         text: Icons.redo
                         width: 50
                         //onClicked: model.submit()
                    }

                    Button {
                         font.family: Icons.fontFamily
                         text: Icons.check_box_outline_blank
                         width: 50
                         //onClicked: model.submit()
                    }

                
                    Button {
                         font.family: Icons.fontFamily
                         text: Icons.radio_button_unchecked
                         width: 50
                         //onClicked: model.submit()
                    }

                

                    Button {
                         font.family: Icons.fontFamily
                         text: Icons.lightbulb
                         width: 50
                         //onClicked: model.submit()
                    }

                                
                    Button {
                         font.family: Icons.fontFamily
                         text: Icons.wb_sunny
                         width: 50
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
            Layout.preferredWidth : 35 // %
            spacing : 1

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight : 35 // %
                id : hierarchy
                spacing : 0

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight : 15 // %
                    color :  Palette.darkestGrey

                        
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

             OldControls.TreeView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight : 85 // %

                style: OldControlsStyles.TreeViewStyle {
                    indentation : 30
                    backgroundColor : Palette.window 
                    frame: Rectangle {color: "transparent"; border.width : 0 }
                   

                   headerDelegate:  Rectangle { 
                        height: 25
                        color: Palette.window
                        border.width : 1 
                        border.color : Palette.darkestGrey
                        Text {
                         height : parent.height
                         verticalAlignment : Text.AlignVCenter
                         color: (styleData.selected) ? Palette.highlightedText : Palette.windowText
                         text: styleData.value
                         font.family : Fonts.workSans.semiBold.name
                         font.pixelSize: 14
                         leftPadding : 4
                        }

                    }

                   
                }


                model: treemodel
                OldControls.TableViewColumn {
                    title: "Name"
                    role: "name"
                    width: 200
                }
                OldControls.TableViewColumn {
                    title: "Type"
                    role: "population"
                    width: 200
                }

                onDoubleClicked: {
                    var element = fakePlace.createObject(treemodel);
                    treemodel.insertNode(element, index, -1);
                }
                onPressAndHold: {
                    var element = treemodel.getNodeByIndex(index);
                    messageDialog.text = element.type + ": " + element.name + "\nPopulation: " + element.population;
                    messageDialog.open();
                }
            }
            /*
            MessageDialog {
                  id: messageDialog
                  title: "Info"
              }
              */
        }

         
        RowLayout {
            id : componentControllers

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight : 65 // %

                    color : Palette.window


                    Rectangle {
                        id : head2
                        anchors.top : parent.top
                        height: 40
                        
                        color :  "#FFFFFF" 

                        
                        Text {
                            text: "Components"
                            color : Palette.windowText
                            anchors.fill: parent
                            anchors.leftMargin: 10;

                            horizontalAlignment: Text.AlignHLeft
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 18
                            font.family: Fonts.workSans.semiBold.name
                        }

                        Rectangle {
  
                           height: 1
                           color : "#FFFFFF"
                           anchors.bottom : parent.bottom
                        }
                    }

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

    /*
    StackView {
        id: stackView
        anchors.top: head2.bottom
        anchors.fill: parent

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




    }*/




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