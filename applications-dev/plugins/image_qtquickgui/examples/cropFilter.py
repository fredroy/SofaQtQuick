import Sofa
import os
import sys
__file = __file__.replace('\\', '/')  # windows compatible filename
currentdir = os.path.dirname(os.path.realpath(__file__))+'/'
currentdir = currentdir.replace('\\', '/')
datadir = currentdir + '/data/'

global imageViewer
imageViewer = None

def createScene(root):
    root.createObject('RequiredPlugin', pluginName='image')
    
    root.createObject('ImageContainer', name='image', filename='data/pelvis_f.raw')
    root.createObject('ImageFilter', name='engine', src='@image', param='50 50 5 200 200 50')
    root.createObject('ImageContainer', name='image2', inputImage='@engine.outputImage', inputTransform='@engine.outputTransform')
    global imageViewer
    imageViewer = root.createObject('ImageViewer', name='viewer', src='@image2', plane='84 53 15')
    root.createObject('PythonScriptController', name="imageViewerScript", filename=__file, classname="PythonScript")
    root.createObject('OglModel', fileMesh='data/pelvis_f.obj', color='1 0.8 0.8 1')

class ImagePlaneController(Sofa.PythonScriptController) :
    leftButton = 0
    rightButton = 1
    middleButton = 2

    def addPoint(self, id, x, y, z):
        return

    def removePoint(self, id):
        return
    
class PythonScript(ImagePlaneController) :
    node = None
    def createGraph(self, root):
        print "image viewer script created"
        sys.stdout.flush()
        self.node = root

    def addPoint(self, id, x, y, z):
        global imageViewer
        print "add", str(id), str(x), str(y), str(z), str(imageViewer)
        sys.stdout.flush()
        return

    def removePoint(self, id):
        global imageViewer
        print "remove", str(id), str(imageViewer)
        sys.stdout.flush()
        return
