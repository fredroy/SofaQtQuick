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
using sofa::helper::types::Material;
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
    m_material = new Qt3DExtras::QPhongMaterial(m_rootEntity); 
    m_transform = new Qt3DCore::QTransform(m_rootEntity);
    m_transform->setScale3D(QVector3D(0.0,0.0,0.0));
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


void Qt3DModel::initGeometryGroup(const FaceGroup& faceGroup)
{
    const VecCoord& vertices = this->getVertices();

    if (true) // replace with test to draw point
    {
        Qt3DCore::QEntity* entity = new Qt3DCore::QEntity(m_rootEntity);

        Qt3DRender::QGeometryRenderer* geometryRenderer = new Qt3DRender::QGeometryRenderer(entity);
        Qt3DRender::QGeometry* geometry = new Qt3DRender::QGeometry(entity);

        geometry->addAttribute(m_positionAttribute);
        geometry->addAttribute(m_normalAttribute);

        geometryRenderer->setInstanceCount(1);
        geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Points);
        geometryRenderer->setGeometry(geometry);
        geometryRenderer->setVertexCount(vertices.size() );

        entity->addComponent(geometryRenderer);
        entity->addComponent(m_material);
        entity->addComponent(m_transform);
    }
/*
    if (faceGroup.nbe > 0)
    {
        Qt3DCore::QEntity* entity = new Qt3DCore::QEntity(m_rootEntity);

        Qt3DRender::QGeometryRenderer* geometryRenderer = new Qt3DRender::QGeometryRenderer(entity);
        Qt3DRender::QGeometry* geometry = new Qt3DRender::QGeometry(entity);

        Qt3DRender::QAttribute *indexAttribute = new Qt3DRender::QAttribute();
        indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
        indexAttribute->setBuffer(m_indexEdgeBuffer);
        indexAttribute->setDataType(Qt3DRender::QAttribute::UnsignedShort);
        indexAttribute->setDataSize(1);
        indexAttribute->setByteOffset(faceGroup.edge0);
        indexAttribute->setByteStride(0);
        indexAttribute->setCount(faceGroup.nbe * 2);

        geometry->addAttribute(m_positionAttribute);
        geometry->addAttribute(m_normalAttribute);
        geometry->addAttribute(indexAttribute);

        geometryRenderer->setInstanceCount(1);
        geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
        geometryRenderer->setGeometry(geometry);

        entity->addComponent(geometryRenderer);
        entity->addComponent(m_material);
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
        indexAttribute->setDataType(Qt3DRender::QAttribute::UnsignedShort);
        indexAttribute->setDataSize(1);
        indexAttribute->setByteOffset(faceGroup.tri0);
        indexAttribute->setByteStride(0);
        indexAttribute->setCount(faceGroup.nbt * 2);

        geometry->addAttribute(m_positionAttribute);
        geometry->addAttribute(m_normalAttribute);
        geometry->addAttribute(indexAttribute);

        geometryRenderer->setInstanceCount(1);
        geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);
        geometryRenderer->setGeometry(geometry);

        entity->addComponent(geometryRenderer);
        entity->addComponent(m_material);
        entity->addComponent(m_transform);
    }*/
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

void Qt3DModel::drawGroups(bool transparent)
{
    sofa::helper::ReadAccessor< Data< helper::vector<FaceGroup> > > groups = this->groups;

    if (groups.empty())
    {
        drawGroup(-1, transparent);
    }
    else
    {
        for (unsigned int i = 0; i<groups.size(); ++i)
            drawGroup(i, transparent);
    }
}

void Qt3DModel::drawGroup(int ig, bool transparent)
{
    const ResizableExtVector<Edge>& edges = this->getEdges();
    const ResizableExtVector<Triangle>& triangles = this->getTriangles();
    const ResizableExtVector<Quad>& quads = this->getQuads();
    const VecCoord& vertices = this->getVertices();
    const ResizableExtVector<Deriv>& vnormals = this->getVnormals();

    //FaceGroup g;
    //if (ig < 0)
    //{
    //    g.materialId = -1;
    //    g.edge0 = 0;
    //    g.nbe = edges.size();
    //    g.tri0 = 0;
    //    g.nbt = triangles.size();
    //    g.quad0 = 0;
    //    g.nbq = quads.size();
    //}
    //else
    //{
    //    g = this->groups.getValue()[ig];
    //}
    //Material m;
    //if (g.materialId < 0)
    //    m = this->material.getValue();
    //else
    //    m = this->materials.getValue()[g.materialId];

    //bool isTransparent = (m.useDiffuse && m.diffuse[3] < 1.0) || hasTransparent();
    //if (transparent ^ isTransparent) return;


    //RGBAColor ambient = m.useAmbient ? m.ambient : RGBAColor::black();
    //RGBAColor diffuse = m.useDiffuse ? m.diffuse : RGBAColor::black();
    //RGBAColor specular = m.useSpecular ? m.specular : RGBAColor::black();
    //RGBAColor emissive = m.useEmissive ? m.emissive : RGBAColor::black();
    //float shininess = m.useShininess ? m.shininess : 45;
    //if (shininess > 128.0f) shininess = 128.0f;

    //if (shininess == 0.0f)
    //{
    //    specular = RGBAColor::black();
    //    shininess = 1;
    //}

    //if (isTransparent)
    //{
    //    emissive[3] = 0; //diffuse[3];
    //    ambient[3] = 0; //diffuse[3];
    //                    //diffuse[3] = 0;
    //    specular[3] = 0;
    //}

}

void Qt3DModel::internalDraw(const sofa::core::visual::VisualParams* vparams, bool transparent)
{
    if (!vparams->displayFlags().getShowVisualModels()) return;

    const VecCoord& vertices = this->getVertices();
    const sofa::defaulttype::ResizableExtVector<Deriv>& vnormals = this->getVnormals();
    const VecTexCoord& vtexcoords = this->getVtexcoords();
    const VecCoord& vtangents = this->getVtangents();
    const VecCoord& vbitangents = this->getVbitangents();
    bool hasTangents = vtangents.size() && vbitangents.size();
    
    //

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
    const ResizableExtVector<Edge>& edges = this->getEdges();
    m_indexEdgeBuffer->setData(reinterpret_cast<const char*>(edges.getData()));
}

void Qt3DModel::initTrianglesIndicesBuffer()
{
    const ResizableExtVector<Triangle>& triangles = this->getTriangles();
    m_indexTriangleBuffer->setData(reinterpret_cast<const char*>(triangles.getData()));

    Qt3DRender::QAttribute *indexAttribute = new Qt3DRender::QAttribute();
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(m_indexEdgeBuffer);
    indexAttribute->setDataType(Qt3DRender::QAttribute::UnsignedShort);
    indexAttribute->setDataSize(1);
    indexAttribute->setByteOffset(0);
    indexAttribute->setByteStride(0);
    indexAttribute->setCount(triangles.size() * 3);
}

void Qt3DModel::initQuadsIndicesBuffer()
{
    const ResizableExtVector<Quad>& quads = this->getQuads();
    m_indexQuadBuffer->setData(reinterpret_cast<const char*>(quads.getData()));

    Qt3DRender::QAttribute *indexAttribute = new Qt3DRender::QAttribute();
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(m_indexEdgeBuffer);
    indexAttribute->setDataType(Qt3DRender::QAttribute::UnsignedShort);
    indexAttribute->setDataSize(1);
    indexAttribute->setByteOffset(0);
    indexAttribute->setByteStride(0);
    indexAttribute->setCount(quads.size() * 4);
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
    m_indexEdgeBuffer->setData(reinterpret_cast<const char*>(edges.getData()));
}

void Qt3DModel::updateTrianglesIndicesBuffer()
{
    const ResizableExtVector<Triangle>& triangles = this->getTriangles();
    m_indexTriangleBuffer->setData(reinterpret_cast<const char*>(triangles.getData()));
}

void Qt3DModel::updateQuadsIndicesBuffer()
{
    const ResizableExtVector<Quad>& quads = this->getQuads();
    m_indexQuadBuffer->setData(reinterpret_cast<const char*>(quads.getData()));
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
