#include <SofaQt3D/Qt3dModel.h>

#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>

#include <sofa/core/visual/VisualParams.h>
#include <sofa/defaulttype/RGBAColor.h>
#include <sofa/helper/types/Material.h>
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace qt3d
{

using sofa::defaulttype::RGBAColor;
using sofa::defaulttype::ResizableExtVector;
using namespace sofa;
using namespace sofa::component::visualmodel;

Qt3DModel::Qt3DModel()
    : Inherited()
    , m_vertexPositionBuffer(nullptr)
    , m_indexTriangleBuffer(nullptr)
    , m_indexQuadBuffer(nullptr)
    , m_indexEdgeBuffer(nullptr)
{
}

void Qt3DModel::init() 
{
    Inherited::init();
}

void Qt3DModel::qtInitVisual()
{

    m_rootEntity = new Qt3DCore::QEntity;
    m_transform = new Qt3DCore::QTransform(m_rootEntity);
    m_transform->setScale3D(QVector3D(1.0,1.0,1.0));
    m_transform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 0.0f));

    this->updateBuffers();

    sofa::helper::ReadAccessor< Data< helper::vector<FaceGroup> > > groups = this->groups;
    if (groups.empty())
    {
        const ResizableExtVector<Edge>& edges = this->getEdges();
        const ResizableExtVector<Triangle>& triangles = this->getTriangles();
        const ResizableExtVector<Quad>& quads = this->getQuads();
        const VecCoord& vertices = this->getVertices();

        FaceGroup fg;
        fg.materialId = -1;
        fg.edge0 = 0;
        fg.nbe = edges.size();
        fg.tri0 = 0;
        fg.nbt = triangles.size();
        fg.quad0 = 0;
        fg.nbq = quads.size();
        initGeometryGroup(fg);
    }
    else
    {
        for (const FaceGroup &fg : groups)
            initGeometryGroup(fg);
    }   
}
Qt3DRender::QMaterial* Qt3DModel::buildMaterial(const sofa::helper::types::Material& material)
{
    Qt3DExtras::QPhongMaterial* qtMaterial = new Qt3DExtras::QPhongMaterial();

    RGBAColor ambient = material.useAmbient ? material.ambient : RGBAColor::black();
    RGBAColor diffuse = material.useDiffuse ? material.diffuse : RGBAColor::black();
    RGBAColor specular = material.useSpecular ? material.specular : RGBAColor::black();
    float shininess = material.useShininess ? material.shininess : 45;
    if (shininess > 128.0f) shininess = 128.0f;

    if (shininess == 0.0f)
    {
        specular = RGBAColor::black();
        shininess = 1;
    }

    bool isTransparent = (material.useDiffuse && material.diffuse[3] < 1.0);
    if (isTransparent)
    {
        ambient[3] = 0; //diffuse[3];
                        //diffuse[3] = 0;
        specular[3] = 0;
    }

    qtMaterial->setAmbient(QColor(ambient.r() * 255, ambient.g() * 255, ambient.b() * 255, ambient.a() * 255));
    qtMaterial->setDiffuse(QColor(diffuse.r() * 255, diffuse.g() * 255, diffuse.b() * 255, diffuse.a() * 255));
    qtMaterial->setSpecular(QColor(specular.r() * 255, specular.g() * 255, specular.b() * 255, specular.a() * 255));
    qtMaterial->setShininess(shininess);

    return qtMaterial;
}

void Qt3DModel::initGeometryGroup(const FaceGroup& faceGroup)
{
    const VecCoord& vertices = this->getVertices();
    sofa::helper::types::Material material;
    if (faceGroup.materialId < 0)
        material = this->material.getValue();
    else
        material = this->materials.getValue()[faceGroup.materialId];

    //if (true) // replace with test to draw point
    //{

    //    Qt3DRender::QGeometryRenderer* geometryRenderer = new Qt3DRender::QGeometryRenderer(entity);
    //    Qt3DRender::QGeometry* geometry = new Qt3DRender::QGeometry(entity);

    //    geometry->addAttribute(m_positionAttribute);

    //    geometryRenderer->setInstanceCount(1);
    //    geometryRenderer->setFirstVertex(0);
    //    geometryRenderer->setFirstInstance(0);
    //    geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Points);
    //    geometryRenderer->setGeometry(geometry);
    //    geometryRenderer->setVertexCount(vertices.size());
    //    

    //    entity->addComponent(geometryRenderer);
    //    entity->addComponent(m_material);
    //    entity->addComponent(m_transform);
    //}

    if (faceGroup.nbe > 0)
    {
        Qt3DCore::QEntity* entity = new Qt3DCore::QEntity(m_rootEntity);

        Qt3DRender::QGeometryRenderer* geometryRenderer = new Qt3DRender::QGeometryRenderer(entity);
        Qt3DRender::QGeometry* geometry = new Qt3DRender::QGeometry(entity);

        Qt3DRender::QAttribute *indexAttribute = new Qt3DRender::QAttribute();
        indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
        indexAttribute->setBuffer(m_indexTriangleBuffer);
        indexAttribute->setDataType(Qt3DRender::QAttribute::UnsignedInt);
        indexAttribute->setDataSize(1);
        indexAttribute->setByteOffset(faceGroup.edge0);
        indexAttribute->setByteStride(0);
        indexAttribute->setCount(faceGroup.nbe * 2);

        geometry->addAttribute(m_positionAttribute);
        geometry->addAttribute(m_normalAttribute);
        geometry->addAttribute(indexAttribute);

        geometryRenderer->setInstanceCount(1);
        geometryRenderer->setFirstVertex(0);
        geometryRenderer->setFirstInstance(0);
        geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
        geometryRenderer->setGeometry(geometry);
        geometryRenderer->setVertexCount(faceGroup.nbe * 2);

        Qt3DRender::QMaterial* qtMaterial = buildMaterial(material);
        qtMaterial->setParent(entity);

        entity->addComponent(geometryRenderer);
        entity->addComponent(qtMaterial);
        entity->addComponent(m_transform);

    }
    if (faceGroup.nbt > 0)
    {
        Qt3DCore::QEntity* entity = new Qt3DCore::QEntity(m_rootEntity);

        Qt3DRender::QGeometryRenderer* geometryRenderer = new Qt3DRender::QGeometryRenderer(entity);
        Qt3DRender::QGeometry* geometry = new Qt3DRender::QGeometry(entity);

        Qt3DRender::QAttribute *indexAttribute = new Qt3DRender::QAttribute();
        indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
        indexAttribute->setBuffer(m_indexTriangleBuffer);
        indexAttribute->setDataType(Qt3DRender::QAttribute::UnsignedInt);
        indexAttribute->setDataSize(1);
        indexAttribute->setByteOffset(faceGroup.tri0);
        indexAttribute->setByteStride(0);
        indexAttribute->setCount(faceGroup.nbt * 3);

        geometry->addAttribute(m_positionAttribute);
        geometry->addAttribute(m_normalAttribute);
        geometry->addAttribute(indexAttribute);

        geometryRenderer->setInstanceCount(1);
        geometryRenderer->setFirstVertex(0);
        geometryRenderer->setFirstInstance(0);
        geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);
        geometryRenderer->setGeometry(geometry);
        geometryRenderer->setVertexCount(faceGroup.nbt * 3);

        Qt3DRender::QMaterial* qtMaterial = buildMaterial(material);
        qtMaterial->setParent(entity);

        entity->addComponent(geometryRenderer);
        entity->addComponent(qtMaterial);
        entity->addComponent(m_transform);
    }
    if (faceGroup.nbq > 0)
    {
        //No quad renderer !!
        /// TODO: convert quad into 2 triangles

        //Qt3DRender::QGeometryRenderer* geometryRenderer = new Qt3DRender::QGeometryRenderer(m_rootEntity);
        //Qt3DRender::QGeometry* geometry = new Qt3DRender::QGeometry(m_meshGeometryRenderers[Primitive::QUAD]);

        //Qt3DRender::QAttribute *indexAttribute = new Qt3DRender::QAttribute();
        //indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
        //indexAttribute->setBuffer(m_indexQuadBuffer);
        //indexAttribute->setDataType(Qt3DRender::QAttribute::UnsignedShort);
        //indexAttribute->setDataSize(1);
        //indexAttribute->setByteOffset(faceGroup.quad0);
        //indexAttribute->setByteStride(0);
        //indexAttribute->setCount(faceGroup.nbq * 2);

        //geometry->addAttribute(m_positionAttribute);
        //geometry->addAttribute(m_normalAttribute);
        //geometry->addAttribute(indexAttribute);

        //geometryRenderer->setInstanceCount(1);
        //geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Quad);
        //geometryRenderer->setGeometry(geometry);
        //m_meshGeometryRenderers.push_back(geometryRenderer);

    }
}

void Qt3DModel::createVertexBuffer()
{
    m_vertexPositionBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, m_rootEntity);
    m_vertexNormalBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, m_rootEntity);
    initVertexBuffer();

}

void Qt3DModel::createEdgesIndicesBuffer()
{
    m_indexEdgeBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, m_rootEntity);
    initEdgesIndicesBuffer();
}

void Qt3DModel::createTrianglesIndicesBuffer()
{
    m_indexTriangleBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, m_rootEntity);
    initTrianglesIndicesBuffer();
}


void Qt3DModel::createQuadsIndicesBuffer()
{
    m_indexQuadBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, m_rootEntity);
    initQuadsIndicesBuffer();
}


void Qt3DModel::initVertexBuffer()
{
    unsigned positionsBufferSize, normalsBufferSize;
    unsigned textureCoordsBufferSize = 0, tangentsBufferSize = 0, bitangentsBufferSize = 0;
    const VecCoord& vertices = this->getVertices();
    const VecCoord& vnormals = this->getVnormals();
    const VecTexCoord& vtexcoords = this->getVtexcoords();
    const VecCoord& vtangents = this->getVtangents();
    const VecCoord& vbitangents = this->getVbitangents();
    bool hasTangents = vtangents.size() && vbitangents.size();

    positionsBufferSize = (vertices.size() * sizeof(vertices[0]));
    normalsBufferSize = (vnormals.size() * sizeof(vnormals[0]));

    unsigned int totalSize = positionsBufferSize + normalsBufferSize + textureCoordsBufferSize +
        tangentsBufferSize + bitangentsBufferSize;
    QByteArray qbaPosition(reinterpret_cast<const char*>(vertices.getData()), positionsBufferSize);
    m_vertexPositionBuffer->setData(qbaPosition);
    QByteArray qbaNormal(reinterpret_cast<const char*>(vnormals.getData()), normalsBufferSize);
    m_vertexNormalBuffer->setData(qbaNormal);

    m_positionAttribute = new Qt3DRender::QAttribute();
    m_positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    m_positionAttribute->setBuffer(m_vertexPositionBuffer);
    m_positionAttribute->setDataType(Qt3DRender::QAttribute::Float);
    m_positionAttribute->setDataSize(3);
    m_positionAttribute->setByteOffset(0);
    m_positionAttribute->setByteStride(0);
    m_positionAttribute->setCount(vertices.size());
    m_positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());

    m_normalAttribute = new Qt3DRender::QAttribute();
    m_normalAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    m_normalAttribute->setBuffer(m_vertexNormalBuffer);
    m_normalAttribute->setDataType(Qt3DRender::QAttribute::Float);
    m_normalAttribute->setDataSize(3);
    m_normalAttribute->setByteOffset(0);
    m_normalAttribute->setByteStride(0);
    m_normalAttribute->setCount(vnormals.size());
    m_normalAttribute->setName(Qt3DRender::QAttribute::defaultNormalAttributeName());

}


void Qt3DModel::initEdgesIndicesBuffer()
{
    updateEdgesIndicesBuffer();
}

void Qt3DModel::initTrianglesIndicesBuffer()
{
    updateTrianglesIndicesBuffer();
}

void Qt3DModel::initQuadsIndicesBuffer()
{
    const ResizableExtVector<Quad>& quads = this->getQuads();
    m_indexQuadBuffer->setData(reinterpret_cast<const char*>(quads.getData()));
}

void Qt3DModel::updateVertexBuffer()
{
    const VecCoord& vertices = this->getVertices();
    const VecCoord& vnormals = this->getVnormals();
    const VecTexCoord& vtexcoords = this->getVtexcoords();
    const VecCoord& vtangents = this->getVtangents();
    const VecCoord& vbitangents = this->getVbitangents();
    bool hasTangents = vtangents.size() && vbitangents.size();

    unsigned positionsBufferSize, normalsBufferSize;
    unsigned textureCoordsBufferSize = 0, tangentsBufferSize = 0, bitangentsBufferSize = 0;

    positionsBufferSize = (vertices.size() * sizeof(vertices[0]));
    normalsBufferSize = (vnormals.size() * sizeof(vnormals[0]));

    QByteArray qbaPosition(reinterpret_cast<const char*>(vertices.getData()), positionsBufferSize);
    m_vertexPositionBuffer->setData(qbaPosition);
    QByteArray qbaNormal(reinterpret_cast<const char*>(vnormals.getData()), normalsBufferSize);
    m_vertexNormalBuffer->setData(qbaNormal);

}

void Qt3DModel::updateEdgesIndicesBuffer()
{
    const ResizableExtVector<Edge>& edges = this->getEdges();
    QByteArray qbaEdges(reinterpret_cast<const char*>(edges.getData()), edges.size() * 2 * sizeof(unsigned int));
    m_indexEdgeBuffer->setData(qbaEdges);
}

void Qt3DModel::updateTrianglesIndicesBuffer()
{
    const ResizableExtVector<Triangle>& triangles = this->getTriangles();
    QByteArray qbaTriangles(reinterpret_cast<const char*>(triangles.getData()), triangles.size() * 3 * sizeof(unsigned int));
    m_indexTriangleBuffer->setData(qbaTriangles);
}

void Qt3DModel::updateQuadsIndicesBuffer()
{
    const ResizableExtVector<Quad>& quads = this->getQuads();
    QByteArray qbaQuads(reinterpret_cast<const char*>(quads.getData()), quads.size() * 4 * sizeof(unsigned int));
    m_indexQuadBuffer->setData(qbaQuads);
}

void Qt3DModel::updateBuffers()
{
    const ResizableExtVector<Edge>& edges = this->getEdges();
    const ResizableExtVector<Triangle>& triangles = this->getTriangles();
    const ResizableExtVector<Quad>& quads = this->getQuads();
    const VecCoord& vertices = this->getVertices();
    const VecDeriv& normals = this->getVnormals();
    const VecTexCoord& texCoords = this->getVtexcoords();
    const VecCoord& tangents = this->getVtangents();
    const VecCoord& bitangents = this->getVbitangents();

    if (!m_vertexPositionBuffer)
    {
        createVertexBuffer();
        //Index Buffer Object
        //Edges indices
        if (edges.size() > 0)
            createEdgesIndicesBuffer();
        //Triangles indices
        if (triangles.size() > 0)
            createTrianglesIndicesBuffer();
        //Quads indices
        if (quads.size() > 0)
            createQuadsIndicesBuffer();
    }
    //Update VBO & IBO
    else
    {
        if (m_oldPositionSize != vertices.size() ||
            m_oldNormalSize != normals.size())
            initVertexBuffer();
        else
            updateVertexBuffer();
        //Indices
        //Edges
        if (m_indexEdgeBuffer != nullptr)
            if (m_oldEdgeSize != edges.size())
                initEdgesIndicesBuffer();
            else
                updateEdgesIndicesBuffer();
        else if (edges.size() > 0)
            createEdgesIndicesBuffer();

        //Triangles
        if (m_indexTriangleBuffer != nullptr)
            if (m_oldTriangleSize != triangles.size())
                initTrianglesIndicesBuffer();
            else
                updateTrianglesIndicesBuffer();
        else if (triangles.size() > 0)
            createTrianglesIndicesBuffer();

        //Quads
        if (m_indexQuadBuffer != nullptr)
            if (m_oldQuadSize != quads.size())
                initQuadsIndicesBuffer();
            else
                updateQuadsIndicesBuffer();
        else if (quads.size() > 0)
            createQuadsIndicesBuffer();
    }
    m_oldPositionSize = vertices.size();
    m_oldNormalSize = normals.size();
    m_oldEdgeSize = edges.size();
    m_oldTriangleSize = triangles.size();
    m_oldQuadSize = quads.size();

}

SOFA_DECL_CLASS(Qt3DModel)

int Qt3DModelClass = core::RegisterObject("Qt3DModel")
.add< Qt3DModel>();

} // namespace qt3d

} // namespace sofa
