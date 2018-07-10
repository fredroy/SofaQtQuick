#ifndef SOFAQTQUICKGUI_SOFAENTITY_H
#define SOFAQTQUICKGUI_SOFAENTITY_H

#include "SofaQtQuickGUI.h"

#include <QQuickItem>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>

#include <sofa/helper/io/Image.h>
#include <SofaQtQuickGUI/SofaScene.h>
#include <SofaQt3D/Qt3DModel.h>

namespace sofa
{

namespace qtquick
{

class SOFA_SOFAQTQUICKGUI_API SofaEntity : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    typedef sofa::qt3d::Qt3DModel Qt3DModel;

    SofaEntity(QNode* parent = 0);
    ~SofaEntity() {};

    Q_PROPERTY(sofa::qtquick::SofaScene* sofaScene READ sofaScene WRITE setSofaScene NOTIFY sofaSceneChanged)
    Q_PROPERTY(QVector3D sceneCenter READ sceneCenter WRITE setSceneCenter NOTIFY sceneCenterChanged)
    Q_INVOKABLE void updateGraph();
    Q_INVOKABLE void updateSceneCenter();
    Q_INVOKABLE void updateVisualParams();
    Q_INVOKABLE void setupVisualParams();

    SofaScene* sofaScene() const { return mySofaScene; }
    void setSofaScene(SofaScene* newScene);

    QVector3D sceneCenter() const { return mySceneCenter; }
    void setSceneCenter(const QVector3D& newSceneCenter);

signals:
    void sofaSceneChanged(sofa::qtquick::SofaScene* newScene);
    void sceneCenterChanged();

 private slots:
    void updateData();

private:
    SofaScene* mySofaScene;
    helper::vector<Qt3DModel*> m_qt3dModelVector;
    QVector3D mySceneCenter;
    QQuickItem* m_parent;
    Qt3DRender::QCamera* m_camera;
};

}

}

#endif // SOFAQTQUICKGUI_SOFAENTITY_H
