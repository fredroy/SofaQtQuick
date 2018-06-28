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
        anchors.margins: 50
        color: "darkRed"

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

            SofaEntity {
                id: sceneRoot
                sofaScene: SofaApplication.sofaScene

                Camera {
                    id: camera
                    projectionType: CameraLens.PerspectiveProjection
                    fieldOfView: 45
                    nearPlane : 0.1
                    farPlane : 100.0
                    position: Qt.vector3d( 0.0, 0.0, 10.0 )
                    upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
                    viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
                }

                OrbitCameraController {
                        linearSpeed : 10
                        lookSpeed : 100
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

                onSofaSceneChanged : {
                   sceneRoot.updateGraph()
                }

                // Component.onCompleted: {
                //     sceneRoot.updateGraph()
                // }


                // PhongMaterial {
                //     id: material
                // }

                // TorusMesh {
                //     id: torusMesh
                //     radius: 5
                //     minorRadius: 1
                //     rings: 100
                //     slices: 20
                // }    
                // Transform {
                //     id: torusTransform
                //     scale3D: Qt.vector3d(1.5, 1, 0.5)
                //     rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 45)
                // }

                // Entity {
                //     id: torusEntity

                //     components: [ torusMesh, material, torusTransform ]
                // }
            }

        }
    }

}
