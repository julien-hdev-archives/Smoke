import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Utils 1.0
import Components.AttributeControllers 1.0

Item {
    property var attribute 
    width: 180; height: 80

   Loader {
        Layout.fillWidth: true

  
        sourceComponent: {
            switch(attribute.type)
            {
                case 0: return slider 
                /*case 1: return slider */
            }
        }

    }

    Component {
        id : slider  
        AttributeSliderFloat {
            attr : attribute
        }

      }
}