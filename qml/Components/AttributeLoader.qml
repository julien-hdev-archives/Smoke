import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Utils 1.0

Item {
    property var attribute 
    width: 180; height: 80

   Loader {
        Layout.fillWidth: true

  
        sourceComponent: {
            switch(attribute.type)
            {
                case 0: return slider 
                case 1: return slider 
            }
        }

    }

    Component {

        id : slider  
        Rectangle {
        anchors.fill : parent
            RowLayout {
                id : label
                Text { 
                    width: 180; height: 100
                    text : attribute.label
                    color : Palette.windowText
                    horizontalAlignment: Text.AlignHLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 14
                    font.family: Fonts.workSans.semiBold.name
                }
            }

            RowLayout {
                anchors.top : label.bottom
                width: 180; height: 30
                Slider {  
                    Layout.alignment: Qt.AlignVCenter
                    id : slide
                    value: attribute.value
                    from : 0
                    to : 1
                    stepSize : 0.01
                    onMoved: {
                        attribute.value = value
                    }
                }
                Text { 
                    Layout.alignment: Qt.AlignVCenter
                    text : attribute.value 
                }
            }
        }
    }
}