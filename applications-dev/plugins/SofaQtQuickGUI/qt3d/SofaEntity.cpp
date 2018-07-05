#include "SofaEntity.h"

#include <SofaQt3D/Qt3DModel.h>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QAttribute>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DCore/QTransform>

namespace sofa
{

namespace qtquick
{
using sofa::qt3d::Qt3DModel;

SofaEntity::SofaEntity(QNode* parent)
    : Qt3DCore::QEntity(parent)
{

}

void SofaEntity::setSofaScene(SofaScene* newSofaScene)
{
    if (newSofaScene == mySofaScene)
        return;

    mySofaScene = newSofaScene;

    sofaSceneChanged(newSofaScene);
}

void SofaEntity::updateGraph()
{
    ////add items
    helper::vector<Qt3DModel*> qt3dModelVector;
    if(mySofaScene && mySofaScene->sofaRootNode())
    {
        mySofaScene->sofaRootNode()->core::objectmodel::BaseContext::get<Qt3DModel, helper::vector<Qt3DModel*> >(&qt3dModelVector);
        for (Qt3DModel* qt3Model : qt3dModelVector)
        {
            qt3Model->qtInitVisual();
            Qt3DCore::QEntity* qt3ModelEntity = qt3Model->getEntity();
            qt3ModelEntity->setParent(this);
        }
    }
}

} // namespace qtquick

} // namespace sofa

