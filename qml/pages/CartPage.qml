import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Page {
	id: cartPage
	anchors.fill: parent
	GridLayout {
		columns: 2
		anchors.fill: parent
		flow: GridLayout.LeftToRight
		Pane {
			Layout.preferredWidth: parent.width * 0.66
			height: parent.height
			Layout.fillWidth: true
			Layout.fillHeight: true
			padding: 50
			RowLayout {
				width: parent.width
				Label {
					text: "Shopping Cart"
					font.pixelSize: 22
					font.italic: true
				}
				Label {
					text: "Items"
					font.pixelSize: 22
					font.italic: true
					Layout.alignment: Qt.AlignRight
				}
			}
		}
		Rectangle {
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.preferredWidth: parent.width * 0.33
			height: parent.height
			color: 'gray'
			Label {
				text: "Order Summary"
				font.pixelSize: 22
				font.italic: true
			}
		}
	}
}
