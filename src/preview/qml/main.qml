import QtQuick 2.0
import QtWebKit 3.0
Rectangle {
    width: 400
    height: 600
    
    WebView {
        width: parent.width -5
        height: parent.height - 5
        url: "http://google.com"
    }

//    MouseArea {
//        anchors.fill: parent
//        onClicked: {
//            Qt.quit();
//        }
//    }
}
