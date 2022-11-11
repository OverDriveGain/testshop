import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 1.4 as OC
import QtQuick.Controls.Styles 1.4

Page {
	id: cartPage
	property int headerPixelSize: 22
	property int subHeaderPixelSize: 18
	property int textPixelSize: 15
	anchors.fill: parent
	ListModel {
		id: libraryModel
		ListElement {
			quantity: "A Masterpiece"
			product_details: "Gabriel"
			price: 99
			total: 1000
		}
		ListElement {
			quantity: "A Masterpiece"
			product_details: "Gabriel"
			price: 99
			total: 1000
		}
		ListElement {
			quantity: "A Masterpiece"
			product_details: "Gabriel"
			price: 99
			total: 1000
		}
	}
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
				OC.TableView {
					width: parent.width
					height: parent.height
					style: TabViewStyle {
						frameOverlap: 1
						tab: Rectangle {
							color: styleData.selected ? "steelblue" : "lightsteelblue"
							border.color: "steelblue"
							implicitWidth: Math.max(text.width + 4, 80)
							implicitHeight: 20
							radius: 2
							Text {
								id: text
								anchors.centerIn: parent
								text: styleData.title
								color: styleData.selected ? "white" : "black"
							}
						}
						frame: Rectangle {
							color: "steelblue"
						}
					}
					model: [{
							"quantity": 2,
							"price": 44,
							"total": 88,
							"product_details": {
								"label": "Fifa 19",
								"platform": "PS 4",
								"photo": "qrc:/images/examples/product_1.jpg"
							}
						}, {
							"quantity": 1,
							"price": 44,
							"total": 88,
							"product_details": {
								"label": "Glacier White 500GB",
								"platform": "PS 4",
								"photo": "qrc:/images/examples/product_2.jpg"
							}
						}, {
							"quantity": 1,
							"price": 249,
							"total": 249.,
							"product_details": {
								"label": "Platinum Headset",
								"platform": "PS 4",
								"photo": "qrc:/images/examples/product_3.jpg"
							}
						}]
					headerDelegate: Text {
						text: styleData.value
						color: "#292933"
						height: 75
						font.pixelSize: textPixelSize
					}
					OC.TableViewColumn {
						role: "product_details"
						title: "Product Details"
						width: 450
						delegate: Row {
							id: row
							spacing: 20
							Column {
								Image {
									height: row.height
									width: 100
									source: styleData.value.photo
								}
							}
							Column {
								height: row.height
								spacing: 50
								Text {
									text: styleData.value.label
								}
								Text {
									text: styleData.value.platform
								}
								Text {
									text: 'Remove'
								}
							}
						}
					}

					OC.TableViewColumn {
						role: "quantity"
						title: "QUANTITY"
						width: 200
						delegate: Row {
							spacing: 20
							Button {
								width: 20
								height: 20
								text: '-'
							}
							TextField {
								readOnly: true
								width: 40
								height: 30
								text: '0'
							}
							Button {
								width: 20
								height: 20
								text: '+'
							}
						}
					}
					OC.TableViewColumn {
						role: "price"
						title: "PRICE"
						width: 200
						delegate: Text {
							text: styleData.value
						}
					}
					OC.TableViewColumn {
						role: "total"
						title: "TOTAL"
						width: 200
						delegate: Text {
							text: styleData.value
						}
					}
					rowDelegate: Rectangle {
						height: 150
						anchors.bottomMargin: 50
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
