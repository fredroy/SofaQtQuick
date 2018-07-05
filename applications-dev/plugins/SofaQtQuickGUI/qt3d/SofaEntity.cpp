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

void SofaEntity::updateData()
{
    for (Qt3DModel* qt3Model : m_qt3dModelVector)
    {
        qt3Model->updateVisual();
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

} // namespace qtquick

} // namespace sofa

