#ifndef SOFAQTQUICKGUI_HELPER_QT3DMODEL_H
#define SOFAQTQUICKGUI_HELPER_QT3DMODEL_H

#include <SofaQt3D.h>

#include <SofaBaseVisual/VisualModelImpl.h>

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QAbstractTextureImage>
#include <Qt3DRender/QAbstractTexture>

#include <Qt3DCore/QTransform>

namespace Qt3DExtras
{
    class QDiffuseSpecularMaterial;
}

namespace sofa
{

namespace qt3d
{

class SOFA_SOFAQT3D_API Qt3DModel : public sofa::component::visualmodel::VisualModelImpl
{
    typedef sofa::component::visualmodel::VisualModelImpl Inherited;

    enum Primitive {
        POINT = 0,
        EDGE = 1,
        TRIANGLE = 2,
        QUAD = 3,
        NOPE = 4
    };
    
    enum TextureType {
        DIFFUSE,
        NORMAL
    };

public:
    Qt3DModel();
    virtual ~Qt3DModel() {}

    void init();
    void initVisual() { Inherited::initVisual(); }
    void updateVisual();
    void qtInitVisual();

    Qt3DCore::QEntity* getEntity() { return m_rootEntity; }
    
private:

    bool ready() { return m_isReady; }
    void setReady(bool ready) { m_isReady = ready; }
    bool m_isReady;

    /// Rendering method.
    void internalDraw(const sofa::core::visual::VisualParams* vparams, bool transparent) {}

    //utility method
    bool findTextureFile(std::string& textureFilename);

    void createPointEntity();
    void createWireframeEntity();
    void createGeometryEntity(const FaceGroup& faceGroup);
    void createMaterials();
    Qt3DRender::QMaterial* buildMaterial(const sofa::helper::types::Material* material);
    void setDiffuseSpecularMaterial(const sofa::helper::types::Material* mat, Qt3DExtras::QDiffuseSpecularMaterial* qtMaterial);
    void setGeometriesEnabled(bool enabled);

    void updateBuffers();
    void createVertexBuffer();
    void initVertexBuffer();
    void createEdgesIndicesBuffer();
    void createTrianglesIndicesBuffer();
    void createQuadsIndicesBuffer();
    void createWireframeIndicesBuffer();
    void updateVertexBuffer();
    void updateEdgesIndicesBuffer();
    void updateTrianglesIndicesBuffer();
    void updateQuadsIndicesBuffer();
    void updateWireframeIndicesBuffer();

    Qt3DCore::QEntity* m_rootEntity;

    Qt3DRender::QBuffer* m_vertexPositionBuffer, *m_vertexNormalBuffer, *m_vertexTexcoordBuffer;
    Qt3DRender::QAttribute *m_positionAttribute, *m_normalAttribute, *m_texcoordAttribute;
    Qt3DRender::QBuffer* m_indexTriangleBuffer, *m_indexQuadBuffer, *m_indexEdgeBuffer, *m_indexWireframeBuffer;
    Qt3DCore::QTransform *m_transform;

    std::size_t m_oldPositionSize, m_oldNormalSize, m_oldEdgeSize, m_oldTriangleSize, m_oldQuadSize;

    sofa::core::DataTracker m_dataTracker;
    std::map<const sofa::helper::types::Material*, Qt3DRender::QMaterial*> m_mapMaterials;
    std::map<const sofa::helper::types::Material*, Qt3DRender::QMaterial*> m_mapEntityMaterial;
    std::vector<Qt3DCore::QEntity*> m_pointEntities;
    std::vector<Qt3DCore::QEntity*> m_edgeEntities;
    std::vector<Qt3DCore::QEntity*> m_triangleEntities;
    std::vector<Qt3DCore::QEntity*> m_quadEntities;
    Qt3DCore::QEntity* m_pointEntity;
    Qt3DCore::QEntity* m_wireframeEntity;

    std::map<const sofa::helper::types::Material*, Qt3DRender::QAbstractTexture*> m_mapDiffuseTextureMaterial;
    std::map<const sofa::helper::types::Material*, Qt3DRender::QAbstractTexture*> m_mapNormalTextureMaterial;
    


};

} // namespace qt3d

} // namespace sofa

#endif // SOFAQTQUICKGUI_HELPER_QT3DMODEL_H
