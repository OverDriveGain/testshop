import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Page {
	id: cartPage
	property int headerPixelSize: 22
	property int subHeaderPixelSize: 18
	property int textPixelSize: 22
	anchors.fill: parent
	GridLayout {
		columns: 2
		anchors.fill: parent
		flow: GridLayout.LeftToRight
		Pane {
			id: leftPane
			Layout.preferredWidth: parent.width * 0.66
			height: parent.height
			Layout.fillWidth: true
			Layout.fillHeight: true
			padding: 50
			Column {
				anchors.fill: parent
				spacing: 50
				RowLayout {
					width: parent.width
					Label {
						text: "Shopping Cart"
						font.pixelSize: headerPixelSize
					}
					Label {
						text: "3 Items"
						font.pixelSize: headerPixelSize
						Layout.alignment: Qt.AlignRight
					}
				}
				RowLayout {
					width: parent.width
					MenuSeparator {
						padding: 0
						topPadding: 12
						bottomPadding: 12
						contentItem: Rectangle {
							implicitWidth: leftPane.width - (leftPane.padding * 2)
							implicitHeight: 1
							color: "#1E000000"
						}
					}
				}
			}
		}
		Pane {
			id: rightPane
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.preferredWidth: parent.width * 0.33
			height: parent.height
			padding: 50
			background: Rectangle {
				anchors.fill: parent
				color: '#F5F5F6'
			}
			Column {
				anchors.fill: parent
				spacing: 50
				RowLayout {
					width: parent.width
					Label {
						text: "Order Summary"
						font.pixelSize: headerPixelSize
					}
				}
				RowLayout {
					width: parent.width
					MenuSeparator {
						padding: 0
						topPadding: 12
						bottomPadding: 12
						contentItem: Rectangle {
							implicitWidth: rightPane.width - (rightPane.padding * 2)
							implicitHeight: 1
							color: "#1E000000"
						}
					}
				}
				RowLayout {
					width: parent.width
					Label {
						text: "ITEMS 3"
						font.pixelSize: subHeaderPixelSize
					}
					Label {
						text: "456.44 $"
						font.pixelSize: subHeaderPixelSize
						Layout.alignment: Qt.AlignRight
					}
				}
				RowLayout {
					width: parent.width
					ColumnLayout {
						width: parent.width
						Label {
							text: "Shipping"
							font.pixelSize: subHeaderPixelSize
						}
						ComboBox {
							implicitWidth: parent.width
							model: ["Standard delivery - $5.00", "Apple", "Coconut"]
						}
					}
				}
				RowLayout {
					width: parent.width
					ColumnLayout {
						width: parent.width
						Label {
							text: "Promo code"
							font.pixelSize: subHeaderPixelSize
						}
						TextField {
							implicitWidth: parent.width
							placeholderText: qsTr("Enter your code")
						}
					}
				}
				RowLayout {
					width: parent.width
					Button {
						text: "Apply"
						font.pixelSize: subHeaderPixelSize
					}
				}
				RowLayout {
					width: parent.width
					MenuSeparator {
						padding: 0
						topPadding: 12
						bottomPadding: 12
						contentItem: Rectangle {
							implicitWidth: rightPane.width - (rightPane.padding * 2)
							implicitHeight: 1
							color: "#1E000000"
						}
					}
				}
				RowLayout {
					width: parent.width
					Label {
						text: "Total cost"
						font.pixelSize: subHeaderPixelSize
					}
					Label {
						text: "462.98 $"
						font.pixelSize: subHeaderPixelSize
						Layout.alignment: Qt.AlignRight
					}
				}
				RowLayout {
					width: parent.width
					Button {
						implicitWidth: parent.width
						text: "CHECKOUT"
						font.pixelSize: subHeaderPixelSize
						background: Rectangle {
							color: "#8071a8"
						}
					}
				}
			}
		}
	}
}
