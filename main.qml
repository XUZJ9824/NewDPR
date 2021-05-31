import QtQuick 2.14
import QtQuick.Controls 1.2
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.0

ApplicationWindow {
    id: wndMain
    visible: true
    //visibility: "FullScreen"
    visibility: Qt.WindowFullScreen
    width: 1024
    height: 768
    title: qsTr("Scroll")

    TabBar {
        id: tabBarMain
        x: 10
        y: 10
        width: parent.width - 10
        height: wndMain.height - 10 - 20

        TabButton
        {
            text: "Raw Map"
        }

        TabButton
        {
            text: "Map Drawing"
        }
    }

    StatusBar {
        id: statusbarMain
        anchors.top: parent.top
        RowLayout{
            anchors.fill: parent
            Label {text: "status bar" }
        }
    }
}
