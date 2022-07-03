import QtQuick 2.15
import QtQuick.Controls 2.0
import QtQuick.Window 2.15
import lkimuk 1.0

// color style from http://colrd.com/image-dna/24016/


Window {
    id: root
    width: 1920 / 2
    height: 1080 / 2
    visible: true
    title: qsTr("excode")
    color: "#28737c" // "#a1d8b1"

    Row {
        anchors.left: codeArea.left
        anchors.top: parent.top
        anchors.topMargin: 10

        CoolComboBox {
            width: 100
            height: 30
            imageSource: "palette-white.svg"
        }
    }

    Rectangle {
        id: codeArea
        x: root.width * 0.1
        y: root.height * 0.1
        width: root.width * 0.8
        height: root.height * 0.8
        color: "#4aab5e"
        radius: 3

        ScrollView {
            anchors.topMargin: 30
            anchors.fill: parent

            Row {
                TextArea {
                    id: lineNumber
                    height: code.height
                    text: "1"

                    color: "#eef1c4"
                    readOnly: true
                    horizontalAlignment: TextEdit.AlignRight
                    background: Rectangle { color: "#44eef1c4" }
                }

                TextArea {
                    id: code
                    textFormat: TextEdit.RichText
                    text: "welcome welcome!"
                    color: "#eef1c4"
                    background: Rectangle { color: codeArea.color }
                    selectByMouse: true
                    onTextChanged: updateLineNumber(lineCount)
                }
            }
        }
    }

    Highlighter {
        id: highlighter
        textDocument: code.textDocument
    }


    /**
     * Update the line number when code changed;
     * @param {number} number is current lines of the code.
     */
    function updateLineNumber(number) {
        let tmp = lineNumber.text;
        let arr = tmp.split('\n');
        let cur_lines = +arr[arr.length - 1];

        if (cur_lines === number)
            return; // do nothing
        else if (cur_lines > number) {
            tmp = tmp.substr(0, tmp.indexOf('\n' + cur_lines)); // decrease lines
        } else {
            tmp += '\n' + (cur_lines + 1); // increase lines
        }

        lineNumber.text = tmp;
    }
}
