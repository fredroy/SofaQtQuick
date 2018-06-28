#ifndef SOFAQTQUICKGUI_SOFAENTITY_H
#define SOFAQTQUICKGUI_SOFAENTITY_H

#include "SofaQtQuickGUI.h"

#include <QtQuick/QQuickFramebufferObject>
#include <Qt3DCore/QEntity>

#include <sofa/helper/io/Image.h>
#include <SofaQtQuickGUI/SofaScene.h>

namespace sofa
{

namespace qtquick
{

class SOFA_SOFAQTQUICKGUI_API SofaEntity : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    SofaEntity(QNode* parent = 0);
    ~SofaEntity() {};

    Q_PROPERTY(sofa::qtquick::SofaScene* sofaScene READ sofaScene WRITE setSofaScene NOTIFY sofaSceneChanged)
    Q_INVOKABLE void updateGraph();

    SofaScene* sofaScene() const { return mySofaScene; }
    void setSofaScene(SofaScene* newScene);

signals:
    void sofaSceneChanged(sofa::qtquick::SofaScene* newScene);

private:
    SofaScene*                  mySofaScene;
};

}

}

#endif // SOFAQTQUICKGUI_SOFAENTITY_H
