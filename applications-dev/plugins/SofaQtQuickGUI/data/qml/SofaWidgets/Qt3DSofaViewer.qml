import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import SofaBasics 1.0
import SofaApplication 1.0
import SofaInteractors 1.0
import SofaEntity 1.0
import SofaScene 1.0
import QtQuick.Scene3D 2.0
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

Item {
    id: root

    Rectangle {
        id: scene
        anchors.fill: parent
        anchors.margins: 5
        color: "lightgrey"

        transform: Rotation {
            id: sceneRotation
            axis.x: 1
            axis.y: 0
            axis.z: 0
            origin.x: scene.width / 2
            origin.y: scene.height / 2
        }

        Scene3D {
            id: scene3D
            anchors.fill: scene
            aspects: ["input", "logic"]
            focus: true
            width: scene.width
            height: scene.width

            SofaEntity {
                id: sceneRoot
                sofaScene: SofaApplication.sofaScene

                    Entity {
                        components: [
                            DirectionalLight {
                                id: defaultLight

                                worldDirection: Qt.vector3d(0.3, -3.0, 0.0).normalized();
                                color: "#ffffff"
                                intensity: 1.0
                            }
                        ]
                    }

                Camera {
                    id: camera
                    projectionType: CameraLens.PerspectiveProjection
                    fieldOfView: 45
                    nearPlane : 0.1
                    farPlane : 100.0
                    position: Qt.vector3d( 0.0, 0.0, 40.0 )
                    upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
                    viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
                }

                OrbitCameraController {
                        linearSpeed : 100
                        lookSpeed : 1000
                        camera: camera

                    }

                components: [
                    RenderSettings {
                        activeFrameGraph: ForwardRenderer {
                            camera: camera
                            clearColor: "transparent"
                        }
                    },
                    InputSettings { }
                ]

                Component.onCompleted : {
                    sceneRoot.updateSceneCenter()
                    camera.viewCenter = sceneRoot.sceneCenter
                }
            }

        }
    }
    //////////////////

    Rectangle {
        id: toolPanel
        color: "lightgrey"
        anchors.top: toolPanelSwitch.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.topMargin: -6
        anchors.bottomMargin: 20
        anchors.rightMargin: -radius
        width: 250
        radius: 5
        visible: false
        opacity: 0.9

        // avoid mouse event propagation through the toolpanel to the sofa viewer
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.AllButtons
            onWheel: wheel.accepted = true
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: toolPanel.radius / 2
            anchors.rightMargin: anchors.margins - toolPanel.anchors.rightMargin
            spacing: 2

            Text {
                Layout.fillWidth: true
                text: "Qt3DSofaViewer parameters"
                font.bold: true
                color: "darkblue"
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Flickable {
                    anchors.fill: parent
                    contentHeight: panelColumn.implicitHeight

                    Column {
                        id: panelColumn
                        anchors.fill: parent
                        spacing: 5

                        GroupBox {
                            id: visualPanel
                            implicitWidth: parent.width
                            title: "Visual"

                            GridLayout {
                                anchors.fill: parent
                                columnSpacing: 0
                                rowSpacing: 2
                                columns: 2
    // background

                                Label {
                                    Layout.fillWidth: true
                                    text: "Background"
                                }

                                Rectangle {
                                    Layout.preferredWidth: 48
                                    Layout.preferredHeight: 20
                                    Layout.alignment: Qt.AlignCenter
                                    color: "darkgrey"
                                    radius: 2

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: backgroundColorPicker.open()
                                    }

                                    ColorDialog {
                                        id: backgroundColorPicker
                                        title: "Please choose a background color"
                                        showAlphaChannel: true

                                        property color previousColor
                                        Component.onCompleted: {
                                            previousColor = scene.color;
                                            color = previousColor;
                                        }

                                        onColorChanged: scene.color = color

                                        onAccepted: previousColor = color
                                        onRejected: color = previousColor
                                    }

                                    Rectangle {
                                        anchors.fill: parent
                                        anchors.margins: 2
                                        color: Qt.rgba(scene.color.r, scene.color.g,scene.color.b, 1.0)

                                        ToolTip {
                                            anchors.fill: parent
                                            description: "Background color"
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Image {
        id: toolPanelSwitch
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 26
        anchors.rightMargin: 3
        source: toolPanel.visible ? "qrc:/icon/minus.png" : "qrc:/icon/plus.png"
        width: 12
        height: width
        visible: true

        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true
            onClicked: toolPanel.visible = !toolPanel.visible
        }
    }

}
