#include <SofaQt3D/Qt3dModel.h>

#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QTextureImage>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QLineWidth>
#include <Qt3DExtras/QDiffuseSpecularMaterial>

#include <sofa/core/visual/VisualParams.h>
#include <sofa/defaulttype/RGBAColor.h>
#include <sofa/helper/types/Material.h>
#include <sofa/core/ObjectFactory.h>

#include <sofa/helper/system/FileRepository.h>


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
    , m_isReady(false)
    , m_vertexPositionBuffer(nullptr)
    , m_indexTriangleBuffer(nullptr)
    , m_indexQuadBuffer(nullptr)
    , m_indexEdgeBuffer(nullptr)
    , m_indexWireframeBuffer(nullptr)
{
    m_dataTracker.trackData(this->materials);
    m_dataTracker.trackData(this->material);
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

        createGeometryEntity(fg);
    }
    else
    {
        for (const FaceGroup &fg : groups)
            createGeometryEntity(fg);
    }   

    createPointEntity();
    createWireframeEntity();

    setReady(true);
}

bool Qt3DModel::findTextureFile(std::string& textureFilename)
{
    std::string textureFile(textureFilename);

    if (!sofa::helper::system::DataRepository.findFile(textureFile, "", nullptr))
    {
        textureFile = this->fileMesh.getFullPath();
        std::size_t position = textureFile.rfind("/");
        textureFile.replace(position + 1, textureFile.length() - position, textureFilename);

        if (!sofa::helper::system::DataRepository.findFile(textureFile))
        {
            return false;
        }
    }
    textureFilename = textureFile;

    return true;
}

void Qt3DModel::setDiffuseSpecularMaterial(const sofa::helper::types::Material* mat, Qt3DExtras::QDiffuseSpecularMaterial* qtMaterial)
{
    RGBAColor ambient = mat->useAmbient ? mat->ambient : RGBAColor::black();
    RGBAColor diffuse = mat->useDiffuse ? mat->diffuse : RGBAColor::black();
    RGBAColor specular = mat->useSpecular ? mat->specular : RGBAColor::black();
    float shininess = mat->useShininess ? mat->shininess : 45;
    if (shininess > 128.0f) shininess = 128.0f;

    if (shininess == 0.0f)
    {
        specular = RGBAColor::black();
        shininess = 1;
    }

    bool isTransparent = (mat->useDiffuse && mat->diffuse[3] < 1.0);
    if (isTransparent)
    {
        ambient[3] = 0; //diffuse[3];
                        //diffuse[3] = 0;
        specular[3] = 0;
        qtMaterial->setAlphaBlendingEnabled(true);
    }
    if (mat->useTexture)
    {
        if (m_mapDiffuseTextureMaterial.find(mat) == m_mapDiffuseTextureMaterial.end())
        {
            std::string textureFile(mat->textureFilename);

            if (findTextureFile(textureFile) )
            {
                Qt3DRender::QTextureImage* texImage = new Qt3DRender::QTextureImage();
                texImage->setSource(QUrl::fromLocalFile(textureFile.c_str()));
                Qt3DRender::QAbstractTexture* tex2d = new Qt3DRender::QTexture2D();
                tex2d->addTextureImage(texImage);
                m_mapDiffuseTextureMaterial[mat] = tex2d;
            }
            else 
                msg_error(this) << "Error (not found) while loading " << mat->textureFilename;
        }
        if (m_mapDiffuseTextureMaterial.find(mat) != m_mapDiffuseTextureMaterial.end())
        {
            qtMaterial->setDiffuse(QVariant::fromValue(m_mapDiffuseTextureMaterial[mat]));
            qtMaterial->setTextureScale(1.0);
        }
        else
        {
            qtMaterial->setDiffuse(QColor(diffuse.r() * 255, diffuse.g() * 255, diffuse.b() * 255, diffuse.a() * 255));
        }
    }
    else
    {
        qtMaterial->setDiffuse(QColor(diffuse.r() * 255, diffuse.g() * 255, diffuse.b() * 255, diffuse.a() * 255));
    }

    qtMaterial->setAmbient(QColor(ambient.r() * 255, ambient.g() * 255, ambient.b() * 255, ambient.a() * 255));
    qtMaterial->setSpecular(QColor(specular.r() * 255, specular.g() * 255, specular.b() * 255, specular.a() * 255));
    qtMaterial->setShininess(shininess);

    if (mat->useBumpMapping)
    {
        if (m_mapNormalTextureMaterial.find(mat) == m_mapNormalTextureMaterial.end())
        {
            std::string textureFile(mat->bumpTextureFilename);

            if (findTextureFile(textureFile))
            {
                Qt3DRender::QTextureImage* texImage = new Qt3DRender::QTextureImage();
                texImage->setSource(QUrl::fromLocalFile(textureFile.c_str()));
                Qt3DRender::QAbstractTexture* tex2d = new Qt3DRender::QTexture2D();
                tex2d->addTextureImage(texImage);
                m_mapNormalTextureMaterial[mat] = tex2d;
            }
            else
                msg_error(this) << "Error (not found) while loading " << mat->bumpTextureFilename;
        }
        if (m_mapNormalTextureMaterial.find(mat) != m_mapNormalTextureMaterial.end())
        {
            qtMaterial->setNormal(QVariant::fromValue(m_mapNormalTextureMaterial[mat]));
        }
    }
}

Qt3DRender::QMaterial* Qt3DModel::buildMaterial(const sofa::helper::types::Material* mat)
{
    Qt3DExtras::QDiffuseSpecularMaterial* qtMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();
    setDiffuseSpecularMaterial(mat, qtMaterial);
    m_mapEntityMaterial[mat] = qtMaterial;

    return qtMaterial;
}

void Qt3DModel::setGeometriesEnabled(bool enabled)
{
    for (Qt3DCore::QEntity* e : m_pointEntities)
    {
        e->setEnabled(enabled);
    }
    for (Qt3DCore::QEntity* e : m_edgeEntities)
    {
        e->setEnabled(enabled);
    }
    for (Qt3DCore::QEntity* e : m_triangleEntities)
    {
        e->setEnabled(enabled);
    }
    for (Qt3DCore::QEntity* e : m_quadEntities)
    {
        e->setEnabled(enabled);
    }
}

void Qt3DModel::updateVisual()
{
    if(ready())
    {
        //update geometry
        Inherited::updateVisual();

        //update other stuff
        //materials
        if (m_dataTracker.isDirty(this->materials))
        {
            for (const sofa::helper::types::Material& mat : this->materials.getValue())
            {
                Qt3DRender::QMaterial* qtMaterial = m_mapEntityMaterial[&mat];
                Qt3DExtras::QDiffuseSpecularMaterial* dsMaterial = static_cast<Qt3DExtras::QDiffuseSpecularMaterial*> (qtMaterial);
                if (dsMaterial)
                {
                    this->setDiffuseSpecularMaterial(&mat, dsMaterial);
                }
            }
        }
        if (m_dataTracker.isDirty(this->material))
        {
            const sofa::helper::types::Material& mat = this->material.getValue();
            Qt3DRender::QMaterial* qtMaterial = m_mapEntityMaterial[&mat];
            Qt3DExtras::QDiffuseSpecularMaterial* dsMaterial = static_cast<Qt3DExtras::QDiffuseSpecularMaterial*> (qtMaterial);
            if (dsMaterial)
            {
                this->setDiffuseSpecularMaterial(&mat, dsMaterial);
            }
        }
        m_dataTracker.clean();

        //states
        sofa::core::visual::VisualParams* vparams = sofa::core::visual::VisualParams::defaultInstance();
        if(vparams->displayFlags().getShowWireFrame())
        {
            m_wireframeEntity->setEnabled(true);
            setGeometriesEnabled(false);
        }
        else
        {
            m_wireframeEntity->setEnabled(false);
            setGeometriesEnabled(true);
        }
    }

}

void Qt3DModel::createPointEntity()
{
    const VecCoord& vertices = this->getVertices();

    Qt3DCore::QEntity* entity = new Qt3DCore::QEntity(m_rootEntity);

    Qt3DRender::QGeometryRenderer* geometryRenderer = new Qt3DRender::QGeometryRenderer(entity);
    Qt3DRender::QGeometry* geometry = new Qt3DRender::QGeometry(entity);

    geometry->addAttribute(m_positionAttribute);

    geometryRenderer->setInstanceCount(1);
    geometryRenderer->setFirstVertex(0);
    geometryRenderer->setFirstInstance(0);
    geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Points);
    geometryRenderer->setGeometry(geometry);
    geometryRenderer->setVertexCount(vertices.size());

    Qt3DExtras::QDiffuseSpecularMaterial* qtMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();
    qtMaterial->setParent(entity);

    entity->addComponent(geometryRenderer);
    entity->addComponent(qtMaterial);
    entity->addComponent(m_transform);

    m_pointEntity = entity; 
    m_pointEntity->setEnabled(false);
}

void Qt3DModel::createWireframeEntity()
{
    const VecCoord& vertices = this->getVertices();
    const ResizableExtVector<Edge>& edges = this->getEdges();
    const ResizableExtVector<Triangle>& triangles = this->getTriangles();
    const ResizableExtVector<Quad>& quads = this->getQuads();

    unsigned int count = (edges.size() + triangles.size() * 3 + quads.size() * 4) * 2;

    Qt3DCore::QEntity* entity = new Qt3DCore::QEntity(m_rootEntity);

    Qt3DRender::QGeometryRenderer* geometryRenderer = new Qt3DRender::QGeometryRenderer(entity);
    Qt3DRender::QGeometry* geometry = new Qt3DRender::QGeometry(entity);

    Qt3DRender::QAttribute *indexAttribute = new Qt3DRender::QAttribute();
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(m_indexWireframeBuffer);
    indexAttribute->setDataType(Qt3DRender::QAttribute::UnsignedInt);
    indexAttribute->setDataSize(1);
    indexAttribute->setByteOffset(0);
    indexAttribute->setByteStride(0);
    indexAttribute->setCount(count);

    geometry->addAttribute(m_positionAttribute);
    geometry->addAttribute(indexAttribute);

    geometryRenderer->setInstanceCount(1);
    geometryRenderer->setFirstVertex(0);
    geometryRenderer->setFirstInstance(0);
    geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    geometryRenderer->setGeometry(geometry);
    geometryRenderer->setVertexCount(count);

    Qt3DExtras::QDiffuseSpecularMaterial* qtMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();
    qtMaterial->setParent(entity);
    //Qt3DRender::QLineWidth* qtLineWidth = new Qt3DRender::QLineWidth(entity);
    //qtLineWidth->setValue(10.0);

    entity->addComponent(geometryRenderer);
    entity->addComponent(qtMaterial);
    entity->addComponent(m_transform);

    m_wireframeEntity = entity;
    m_wireframeEntity->setEnabled(false);
}

void Qt3DModel::createGeometryEntity(const FaceGroup& faceGroup)
{
    const VecCoord& vertices = this->getVertices();
    const sofa::helper::types::Material* material;
    if (faceGroup.materialId < 0)
        material = &this->material.getValue();
    else
        material = &this->materials.getValue()[faceGroup.materialId];

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
        geometry->addAttribute(m_texcoordAttribute);
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
        m_edgeEntities.push_back(entity);
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
        geometry->addAttribute(m_texcoordAttribute);
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
        m_triangleEntities.push_back(entity);
    }
    if (faceGroup.nbq > 0)
    {
        Qt3DCore::QEntity* entity = new Qt3DCore::QEntity(m_rootEntity);

        Qt3DRender::QGeometryRenderer* geometryRenderer = new Qt3DRender::QGeometryRenderer(entity);
        Qt3DRender::QGeometry* geometry = new Qt3DRender::QGeometry(entity);

        Qt3DRender::QAttribute *indexAttribute = new Qt3DRender::QAttribute();
        indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
        indexAttribute->setBuffer(m_indexQuadBuffer);
        indexAttribute->setDataType(Qt3DRender::QAttribute::UnsignedInt);
        indexAttribute->setDataSize(1);
        indexAttribute->setByteOffset(faceGroup.quad0 * 2);
        indexAttribute->setByteStride(0);
        indexAttribute->setCount(faceGroup.nbq * 3 * 2);

        geometry->addAttribute(m_positionAttribute);
        geometry->addAttribute(m_normalAttribute);
        geometry->addAttribute(m_texcoordAttribute);
        geometry->addAttribute(indexAttribute);

        geometryRenderer->setInstanceCount(1);
        geometryRenderer->setFirstVertex(0);
        geometryRenderer->setFirstInstance(0);
        geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);
        geometryRenderer->setGeometry(geometry);
        geometryRenderer->setVertexCount(faceGroup.nbq * 3 * 2);

        Qt3DRender::QMaterial* qtMaterial = buildMaterial(material);
        qtMaterial->setParent(entity);

        entity->addComponent(geometryRenderer);
        entity->addComponent(qtMaterial);
        entity->addComponent(m_transform);
        m_triangleEntities.push_back(entity);

    }
}

void Qt3DModel::createVertexBuffer()
{
    m_vertexPositionBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, m_rootEntity);
    m_vertexNormalBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, m_rootEntity);
    m_vertexTexcoordBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, m_rootEntity);
    initVertexBuffer();

}

void Qt3DModel::createEdgesIndicesBuffer()
{
    m_indexEdgeBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, m_rootEntity);
    updateEdgesIndicesBuffer();
}

void Qt3DModel::createTrianglesIndicesBuffer()
{
    m_indexTriangleBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, m_rootEntity);
    updateTrianglesIndicesBuffer();
}


void Qt3DModel::createQuadsIndicesBuffer()
{
    m_indexQuadBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, m_rootEntity);
    updateQuadsIndicesBuffer();
}

void Qt3DModel::createWireframeIndicesBuffer()
{
    m_indexWireframeBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, m_rootEntity);
    updateWireframeIndicesBuffer();
}

void Qt3DModel::initVertexBuffer()
{
    const VecCoord& vertices = this->getVertices();
    const VecCoord& vnormals = this->getVnormals();
    const VecTexCoord& vtexcoords = this->getVtexcoords();
    updateVertexBuffer();

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

    m_texcoordAttribute = new Qt3DRender::QAttribute();
    m_texcoordAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    m_texcoordAttribute->setBuffer(m_vertexTexcoordBuffer);
    m_texcoordAttribute->setDataType(Qt3DRender::QAttribute::Float);
    m_texcoordAttribute->setDataSize(2);
    m_texcoordAttribute->setByteOffset(0);
    m_texcoordAttribute->setByteStride(0);
    m_texcoordAttribute->setCount(vtexcoords.size());
    m_texcoordAttribute->setName(Qt3DRender::QAttribute::defaultTextureCoordinateAttributeName());
}

void Qt3DModel::updateVertexBuffer()
{
    const VecCoord& vertices = this->getVertices();
    const VecCoord& vnormals = this->getVnormals();
    const VecTexCoord& vtexcoords = this->getVtexcoords();

    unsigned positionsBufferSize, normalsBufferSize;
    unsigned textureCoordsBufferSize = 0;

    positionsBufferSize = (vertices.size() * sizeof(vertices[0]));
    normalsBufferSize = (vnormals.size() * sizeof(vnormals[0]));
    textureCoordsBufferSize = (vtexcoords.size() * sizeof(vtexcoords[0]));

    QByteArray qbaPosition(reinterpret_cast<const char*>(vertices.getData()), positionsBufferSize);
    m_vertexPositionBuffer->setData(qbaPosition);
    QByteArray qbaNormal(reinterpret_cast<const char*>(vnormals.getData()), normalsBufferSize);
    m_vertexNormalBuffer->setData(qbaNormal);
    QByteArray qbaTexcoord(reinterpret_cast<const char*>(vtexcoords.getData()), textureCoordsBufferSize);
    m_vertexTexcoordBuffer->setData(qbaTexcoord);
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
    //convert to triangles
    ResizableExtVector<Triangle> quadTriangles;
    for (const Quad& q : quads)
    {
        quadTriangles.push_back(Triangle(q[0], q[1], q[2]));
        quadTriangles.push_back(Triangle(q[2], q[3], q[0]));
    }

    QByteArray qbaQuads(reinterpret_cast<const char*>(quadTriangles.getData()), quadTriangles.size() * 3 * sizeof(unsigned int));
    m_indexQuadBuffer->setData(qbaQuads);
}


void Qt3DModel::updateWireframeIndicesBuffer()
{
    const ResizableExtVector<Edge>& edges = this->getEdges();
    const ResizableExtVector<Triangle>& triangles = this->getTriangles();
    const ResizableExtVector<Quad>& quads = this->getQuads();

    ResizableExtVector<Edge> wireframe;
    for (const Edge& e : edges)
        wireframe.push_back(e);
    for (const Triangle& t : triangles)
    {
        wireframe.push_back(Edge(t[0], t[1]));
        wireframe.push_back(Edge(t[1], t[2]));
        wireframe.push_back(Edge(t[2], t[0]));
    }
    for (const Quad& q : quads)
    {
        wireframe.push_back(Edge(q[0], q[1]));
        wireframe.push_back(Edge(q[1], q[2]));
        wireframe.push_back(Edge(q[2], q[3]));
        wireframe.push_back(Edge(q[3], q[0]));
    }

    QByteArray qbaWireframe(reinterpret_cast<const char*>(wireframe.getData()), wireframe.size() * 2 * sizeof(unsigned int));
    m_indexWireframeBuffer->setData(qbaWireframe);
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

        createWireframeIndicesBuffer();
    }
    // Update VBO & IBO
    // TODO: Change topological change detection
    else
    {
        updateVertexBuffer();

        //Indices
        //Edges
        if (m_indexEdgeBuffer != nullptr)
            if (m_oldEdgeSize != edges.size())
                updateEdgesIndicesBuffer();
        else if (edges.size() > 0)
            createEdgesIndicesBuffer();

        //Triangles
        if (m_indexTriangleBuffer != nullptr)
            if (m_oldTriangleSize != triangles.size())
                updateTrianglesIndicesBuffer();
        else if (triangles.size() > 0)
            createTrianglesIndicesBuffer();

        //Quads
        if (m_indexQuadBuffer != nullptr)
            if (m_oldQuadSize != quads.size())
                updateQuadsIndicesBuffer();
        else if (quads.size() > 0)
            createQuadsIndicesBuffer();

        //Wireframe
        if (m_indexWireframeBuffer != nullptr)
            if(m_oldEdgeSize != edges.size() || m_oldTriangleSize != triangles.size() || m_oldQuadSize != quads.size() )
                updateWireframeIndicesBuffer();
        else 
            createWireframeIndicesBuffer();

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
