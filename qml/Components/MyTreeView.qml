import QtQuick 2.15
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import MyTree 1.0
import Utils 1.0


Item {

    id: myTreeView

    TreeModel {
        id: faketreeModel
        roles: ["name","typeName"]


        TreeElement {
            property string name: "Collection 1"
            property string typeName: "Collection"
            TreeElement {
                property string name: "MainCam";
                property string typeName: "Camera";
            }
            TreeElement {
                property string name: "Lights";
                property string typeName: "Group";
                TreeElement { property string name: "Light1";  property string typeName: "Ponctual Light" }
                TreeElement { property string name: "Light2";  property string typeName: "Global Light" }
            }
            TreeElement {
                property string name: "SmokeEffects";
                property string typeName: "Group";
                TreeElement {property string name: "SmokeLeft";  property string typeName: "SmokeEffect" }
                TreeElement {property string name: "SmokeRight";  property string typeName: "SmokeEffect" }
                TreeElement {property string name: "SmokeCenter";  property string typeName: "SmokeEffect" }
            }
            TreeElement { 
                property string name: "Obstacles";  
                property string typeName: "Group";
                TreeElement { property string name: "Balloon";  property string typeName: "Sphere" }
                TreeElement { property string name: "House";  property string typeName: "Cube" }

            }
        }
    }

    TreeView {
        anchors.fill: parent
        model: faketreeModel


        style: TreeViewStyle {
            indentation : 30
            backgroundColor : Palette.window 
            frame: Rectangle { color: "transparent"; border.width : 1;  border.color : Palette.darkestGrey}
                   

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

            TableViewColumn {
                title: "Name"
                role: "name"
                width: 200
                
            }

            TableViewColumn {
                title: "Type"
                role: "typeName"
                width: 200
            }
    }   
}