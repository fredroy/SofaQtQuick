#include "SofaEntity.h"

#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QAttribute>
#include <Qt3DCore/QTransform>

#include <sofa/core/visual/VisualParams.h>
#include <SofaQtQuickGUI/qt3d/DrawToolQt3D.h>
#include <SofaQtQuickGUI/helper/QMathExtensions.h>

namespace sofa
{

namespace qtquick
{
SofaEntity::SofaEntity(QNode* parent)
    : Qt3DCore::QEntity(parent)
    , m_parent(nullptr)
    , m_camera(nullptr)
{

}

void SofaEntity::setSofaScene(SofaScene* newSofaScene)
{
    if (newSofaScene == mySofaScene)
        return;

    mySofaScene = newSofaScene;

    if(mySofaScene)
    {
        connect(mySofaScene, &SofaScene::stepBegin, this, &SofaEntity::updateData);
        this->updateGraph();
        this->setupVisualParams();
    }

    sofaSceneChanged(newSofaScene);
}

void SofaEntity::updateSceneCenter()
{
    if (mySofaScene)
    {
        const sofa::defaulttype::BoundingBox& bbox = mySofaScene->sofaRootNode()->f_bbox.getValue();
        QVector3D newSceneCenter;
        for (unsigned short i = 0; i<3; i++)
            newSceneCenter[i] = (bbox.minBBox()[i] + bbox.maxBBox()[i])*0.5;
        setSceneCenter(newSceneCenter);
    }
}

void SofaEntity::updateData()
{
    for (Qt3DModel* qt3Model : m_qt3dModelVector)
    {
        qt3Model->updateVisual();
    }

    updateSceneCenter();

    updateVisualParams();
}

void SofaEntity::updateVisualParams()
{
    sofa::core::visual::VisualParams* vparams = sofa::core::visual::VisualParams::defaultInstance();
    if (vparams)
    {

        vparams->sceneBBox() = mySofaScene->sofaRootNode()->f_bbox.getValue();

        if (m_camera)
        {
            double modelviewMatrix[16], projectionMatrix[16];
            QMatrix4x4 qtModelViewMAtrix = m_camera->viewMatrix();
            for (unsigned int i = 0; i < 16; i++)
                modelviewMatrix[i] = qtModelViewMAtrix.constData()[i];
            QMatrix4x4 qtProjectionMatrix = m_camera->projectionMatrix();
            for (unsigned int i = 0; i < 16; i++)
                projectionMatrix[i] = qtProjectionMatrix.constData()[i];

            vparams->setProjectionMatrix(projectionMatrix);
            vparams->setModelViewMatrix(modelviewMatrix);

        }
        if(m_parent)
            vparams->viewport() = sofa::helper::fixed_array<int, 4>(0, 0, m_parent->width(), m_parent->height());
    }
}
void SofaEntity::setupVisualParams()
{
    sofa::core::visual::VisualParams* vparams = sofa::core::visual::VisualParams::defaultInstance();
    if (vparams)
    {
        if (!vparams->drawTool())
        {
            vparams->drawTool() = new sofaqtquickgui::qt3d::DrawToolQt3D();
            vparams->setSupported(sofa::core::visual::API_OpenGL); //change this
        }
        // get Camera context
        m_camera = nullptr;
        for (QNode* n : this->childNodes())
        {
            m_camera = qobject_cast<Qt3DRender::QCamera*>(n);
            if (m_camera)
                break;
        }
        //get Scene3D context
        m_parent = nullptr;
        m_parent = qobject_cast<QQuickItem*>(this->parent());

        updateVisualParams();
    }
}

void SofaEntity::updateGraph()
{
    ////add items
    m_qt3dModelVector.clear();
    if(mySofaScene && mySofaScene->sofaRootNode())
    {
        mySofaScene->sofaRootNode()->core::objectmodel::BaseContext::get<Qt3DModel, helper::vector<Qt3DModel*> >(&m_qt3dModelVector, sofa::core::objectmodel::BaseContext::SearchRoot);
        for (Qt3DModel* qt3Model : m_qt3dModelVector)
        {
            qt3Model->qtInitVisual();
            Qt3DCore::QEntity* qt3ModelEntity = qt3Model->getEntity();
            qt3ModelEntity->setParent(this);
        }
    }
}

void SofaEntity::setSceneCenter(const QVector3D& newSceneCenter)
{
    if (newSceneCenter == mySceneCenter)
        return;

    mySceneCenter = newSceneCenter;

    emit sceneCenterChanged();
}

} // namespace qtquick

} // namespace sofa

