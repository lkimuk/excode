import QtQuick 2.12
import QtQuick.Controls 2.12

// language: https://www.flaticon.com/uicons/8#
//           https://www.flaticon.com/uicons/9#

Item {
    id: root
    property string imageSource

    Rectangle {
        id: borderRectangle
        width: root.width + 40
        height: coolComboBox.height
        border.color: "#ffffff"
        border.width: coolComboBox.pressed ? 2 : 1
        radius: 2
        color: "transparent"

        anchors.left: parent.left
        anchors.top: parent.top

        Image {
            x: 5
            y: 5
            width: parent.height - 10
            height: parent.height - 10
            source: imageSource
        }

        ComboBox {
            id: coolComboBox
            width: root.width
            height: root.height
            anchors.top: parent.top
            anchors.right: parent.right


            model: ["First", "Second", "Third"]

            delegate: ItemDelegate {
                id: itemDelegate
                width: coolComboBox.width
                height: coolComboBox.height
                highlighted: coolComboBox.highlightedIndex === index

                contentItem: Text {
                    text: modelData
                    color: "#ffffff"
                    font: coolComboBox.font
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: itemDelegate.highlighted ? "#eef1c4" : "#28737c"
                    opacity: 0.6
                }
            }

            contentItem: Text {
                leftPadding: 0
                rightPadding: coolComboBox.indicator.width + coolComboBox.spacing

                text: coolComboBox.displayText
                color: "#ffffff"
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                color: "#28737c"
                implicitWidth: coolComboBox.width
                implicitHeight: coolComboBox.height
                border.color: "#ffffff"
                border.width: coolComboBox.pressed ? 2 : 1
                radius: 2
            }

            popup: Popup {
                id: popup
                y: coolComboBox.height - 1
                width: coolComboBox.width
                implicitHeight: contentItem.implicitHeight
                padding: 1

                contentItem: ListView {
                    clip: true
                    implicitHeight: contentHeight
                    model: popup.visible ? coolComboBox.delegateModel : null
                    currentIndex: coolComboBox.highlightedIndex

                    ScrollIndicator.vertical: ScrollIndicator { }
                }

                background: Rectangle {
                    color: "#bb977d"
                    border.color: "#ffffff"
                    radius: 2
                }
            }

            indicator: Label {
                width: 10
                height: 10
                x: coolComboBox.width - coolComboBox.rightPadding - width
                y: coolComboBox.topPadding / 2 + (coolComboBox.availableHeight - height) / 2;
                text: popup.visible ? qsTr("\u2227") : qsTr("\u2228")
                color: "#ffffff"
            }
        }
    }

}
