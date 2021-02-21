import QtQuick 2.15
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import MyTree 1.0
import Utils 1.0


Item {

    id: myTreeView

    TreeModel {
        id: faketreeModel
        roles: ["name","population"]

        TreeElement {
            property string name: "Collection 1"
            property string type: "Collection"
            TreeElement {
                property string name: "MainCam";
                property string type: "Camera"
            }
            TreeElement {
                property string name: "Lights";
                property string type: "Group"
                TreeElement { property string name: "Light1";  property string type: "Ponctual Light" }
                TreeElement { property string name: "Light2";  property string type: "Global Light" }
            }
            TreeElement {
                property string name: "SmokeEffects";
                property string type: "Group"
                TreeElement {property string name: "SmokeLeft";  property string type: "SmokeEffect" }
                TreeElement {property string name: "SmokeRight";  property string type: "SmokeEffect" }
                TreeElement {property string name: "SmokeCenter";  property string type: "SmokeEffect" }
            }
            TreeElement { 
                property string name: "Obstacles";  
                property string type: "Group" 
                TreeElement { property string name: "Balloon";  property string type: "Sphere" }
                TreeElement { property string name: "House";  property string type: "Cube" }

            }
        }
    }

    TreeView {
                anchors.fill: parent
                style: TreeViewStyle {
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

                    rowDelegate: Rectangle { 
                        height: 25
                        color: (styleData.selected) ? Palette.highlight : (styleData.row % 2) ? Palette.window : Palette.darkestGrey
                    }

                    itemDelegate: Text {
                         verticalAlignment : Text.AlignVCenter
                         color: (styleData.selected) ? Palette.highlightedText : Palette.windowText
                         text: styleData.value
                         font.family : Fonts.workSans.semiBold.name
                         leftPadding : 4
                    }

                    branchDelegate: Image {
                        source: "../images/navigation_next_item.png"
                        width:14; height:14
                        transformOrigin: Item.Center
                        rotation: styleData.isExpanded ? 90 : 0
                    }
                }


                model: faketreeModel
                TableViewColumn {
                    title: "Name"
                    role: "name"
                    width: 200
                }
                TableViewColumn {
                    title: "Type"
                    role: "type"
                    width: 200
                }



            }   
}




/*
                onDoubleClicked: {
                    var element = fakePlace.createObject(treemodel);
                    treemodel.insertNode(element, index, -1);

                onPressAndHold: {
                    var element = treemodel.getNodeByIndex(index);
                    messageDialog.text = element.type + ": " + element.name + "\nPopulation: " + element.population;
                    messageDialog.open();
                }
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




                }*/