import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Pickers 1.0
import ru.sashikknox 1.0

Page {
    id: page

    property string resourcesPath: AuroraData.resourcesPath

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height

        Column {
            id: column

            width: parent.width
            spacing: Theme.paddingMedium

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - x * 2
                text: qsTr("Выбирите папку с ресурсами игры")
            }

            Row {
                x: Theme.horizontalPageMargin
                spacing: Theme.paddingMedium

                Label {
                    id: pathLabel

                    width: page.width - Theme.horizontalPageMargin * 2 - Theme.paddingMedium - resourcesButton.width
                    text: resourcesPath
                }

                IconButton {
                    id: resourcesButton
                    onClicked:  pageStack.push(folderPickerPage)
                    icon.source: "image://theme/icon-m-file-folder"
                    icon.width: Theme.iconSizeMedium
                    icon.height: Theme.iconSizeMedium
                }
            }

            Button {
                onClicked: AuroraData.quit()

                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Начать игру")
            }
        }
    }

    Component {
        id: folderPickerPage
        FolderPickerPage {
            dialogTitle: qsTr("Выбирите папку с ресурсами игры")
            onSelectedPathChanged: AuroraData.resourcesPath = selectedPath
        }
    }

    Component.onCompleted: {
        console.log("AuroraData", AuroraData)
    }
}
