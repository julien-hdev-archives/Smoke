import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Utils 1.0

Item {   
        property var attr
        id : sliderFloat 
        Rectangle {

        anchors.fill : parent
            RowLayout {
                id : label
                Text { 
                    
                    width: parent.width; height: 45
                    Layout.leftMargin : 10
                    Layout.topMargin : 10
                    text : attr.label
                    color : Palette.windowText
                    horizontalAlignment: Text.AlignHLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 14
                    font.family: Fonts.workSans.semiBold.name
                }
            }

            RowLayout {
                anchors.top : label.bottom

                width: 180; height: 20
                Slider {
                    Layout.leftMargin : 10
                    Layout.alignment: Qt.AlignVCenter
                    id : slide
                    value: attr.value
                    from : 0
                    to : 1
                    stepSize : 0.01
                    onMoved: {
                        attr.value = value
                    }
                }
                Text { 
                    Layout.leftMargin : 2
                    width: 180; height: 60
                    text : attr.value
                    color : Palette.toolTipText
                    horizontalAlignment: Text.AlignHLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 14
                    font.family: Fonts.workSans.semiBold.name
                }
            }
        }
}