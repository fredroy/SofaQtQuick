#include "SofaEntity.h"

#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QAttribute>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DCore/QTransform>

namespace sofa
{

namespace qtquick
{
SofaEntity::SofaEntity(QNode* parent)
    : Qt3DCore::QEntity(parent)
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
    // update scene center
    updateSceneCenter();
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

