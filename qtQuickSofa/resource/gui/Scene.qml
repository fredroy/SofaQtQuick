import QtQuick 2.0
import Scene 1.0

Scene {
    id: base

    Component.onCompleted: {
        base.open("Demos/caduceus.scn");
		base.play();
    }
}
