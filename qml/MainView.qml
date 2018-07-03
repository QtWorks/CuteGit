import QtQuick 2.0
import QtQuick.Controls 1.4
import org.semlanik.cutegit 1.0

FocusScope {
    id: root
    property var commitsForDiff: null
    property bool controlActive: false
    focus: true

    Loader {
        id: mainLoader
        anchors.fill: parent
    }

    states: [
        State {
            name: "initial"
            when: _handler.repositories.count <=0
            PropertyChanges {
                target: mainLoader
                source: "InitialWizard.qml"
            }
        },
        State {
            name: "main"
            when: _handler.repositories.count > 0
            PropertyChanges {
                target: mainLoader
                source: "RepositoryView.qml"
            }
        }
    ]
}
