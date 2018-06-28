#include <SofaQtQuickGUI/qt3d/DrawToolQt3D.h>

namespace sofaqtquickgui
{

namespace qt3d
{

DrawToolQt3D::DrawToolQt3D() {}
void DrawToolQt3D::init() {}

void DrawToolQt3D::drawPoints(const std::vector<Vector3> &points, float size, const  Vec4f& colour){}
void DrawToolQt3D::drawPoints(const std::vector<Vector3> &points, float size, const std::vector<Vec4f>& colour){}

void DrawToolQt3D::drawLine(const Vector3 &p1, const Vector3 &p2, const Vec4f& colour){}
void DrawToolQt3D::drawLines(const std::vector<Vector3> &points, float size, const Vec4f& colour){}
void DrawToolQt3D::drawLines(const std::vector<Vector3> &points, float size, const std::vector<Vec4f>& colours){}
void DrawToolQt3D::drawLines(const std::vector<Vector3> &points, const std::vector< Vec2i > &index, float size, const Vec4f& colour){}

void DrawToolQt3D::drawLineStrip(const std::vector<Vector3> &points, float size, const Vec4f& colour){}
void DrawToolQt3D::drawLineLoop(const std::vector<Vector3> &points, float size, const Vec4f& colour){}

void DrawToolQt3D::drawTriangles(const std::vector<Vector3> &points, const Vec4f& colour){}
void DrawToolQt3D::drawTriangles(const std::vector<Vector3> &points, const Vector3& normal, const Vec4f& colour){}
void DrawToolQt3D::drawTriangles(const std::vector<Vector3> &points,
    const std::vector< Vec3i > &index,
    const std::vector<Vector3>  &normal,
    const Vec4f& colour){}
void DrawToolQt3D::drawTriangles(const std::vector<Vector3> &points,
    const std::vector< Vec4f > &colour){}
void DrawToolQt3D::drawTriangles(const std::vector<Vector3> &points,
    const std::vector<Vector3>  &normal,
    const std::vector< Vec4f > &colour){}
void DrawToolQt3D::drawTriangleStrip(const std::vector<Vector3> &points,
    const std::vector<Vector3>  &normal,
    const Vec4f& colour){}
void DrawToolQt3D::drawTriangleFan(const std::vector<Vector3> &points,
    const std::vector<Vector3>  &normal,
    const Vec4f& colour){}



void DrawToolQt3D::drawFrame(const Vector3& position, const Quaternion &orientation, const Vec3f &size){}
void DrawToolQt3D::drawFrame(const Vector3& position, const Quaternion &orientation, const Vec3f &size, const Vec4f &colour){}

void DrawToolQt3D::drawSpheres(const std::vector<Vector3> &points, const std::vector<float>& radius, const Vec4f& colour){}
void DrawToolQt3D::drawSpheres(const std::vector<Vector3> &points, float radius, const Vec4f& colour){}
void DrawToolQt3D::drawFakeSpheres(const std::vector<Vector3> &points, const std::vector<float>& radius, const Vec4f& colour){}
void DrawToolQt3D::drawFakeSpheres(const std::vector<Vector3> &points, float radius, const Vec4f& colour){}

void DrawToolQt3D::drawCone(const Vector3& p1, const Vector3 &p2, float radius1, float radius2, const Vec4f& colour, int subd){}

/// Draw a cube of size one centered on the current point.
void DrawToolQt3D::drawCube(const float& radius, const Vec4f& colour, const int& subd){}

void DrawToolQt3D::drawCylinder(const Vector3& p1, const Vector3 &p2, float radius, const Vec4f& colour, int subd){}

void DrawToolQt3D::drawCapsule(const Vector3& p1, const Vector3 &p2, float radius, const Vec4f& colour, int subd){}

void DrawToolQt3D::drawArrow(const Vector3& p1, const Vector3 &p2, float radius, const Vec4f& colour, int subd){}
void DrawToolQt3D::drawArrow(const Vector3& p1, const Vector3 &p2, float radius, float coneLength, const Vec4f& colour, int subd){}

/// Draw a cross (3 lines) centered on p
void DrawToolQt3D::drawCross(const Vector3&p, float length, const Vec4f& colour){}

/// Draw a plus sign of size one centered on the current point.
void DrawToolQt3D::drawPlus(const float& radius, const Vec4f& colour, const int& subd ){}

void DrawToolQt3D::drawPoint(const Vector3 &p, const Vec4f &c){}
void DrawToolQt3D::drawPoint(const Vector3 &p, const Vector3 &n, const Vec4f &c){}

void DrawToolQt3D::drawTriangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3,
    const Vector3 &normal){}
void DrawToolQt3D::drawTriangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3,
    const Vector3 &normal, const Vec4f &c){}
void DrawToolQt3D::drawTriangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3,
    const Vector3 &normal,
    const Vec4f &c1, const Vec4f &c2, const Vec4f &c3){}
void DrawToolQt3D::drawTriangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3,
    const Vector3 &normal1, const Vector3 &normal2, const Vector3 &normal3,
    const Vec4f &c1, const Vec4f &c2, const Vec4f &c3){}

void DrawToolQt3D::drawQuad(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &p4,
    const Vector3 &normal){}
void DrawToolQt3D::drawQuad(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &p4,
    const Vector3 &normal, const Vec4f &c){}
void DrawToolQt3D::drawQuad(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &p4,
    const Vector3 &normal,
    const Vec4f &c1, const Vec4f &c2, const Vec4f &c3, const Vec4f &c4){}
void DrawToolQt3D::drawQuad(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &p4,
    const Vector3 &normal1, const Vector3 &normal2, const Vector3 &normal3, const Vector3 &normal4,
    const Vec4f &c1, const Vec4f &c2, const Vec4f &c3, const Vec4f &c4){}
void DrawToolQt3D::drawQuads(const std::vector<Vector3> &points, const Vec4f& colour){}
void DrawToolQt3D::drawQuads(const std::vector<Vector3> &points, const std::vector<Vec4f>& colours){}

void DrawToolQt3D::drawTetrahedron(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vec4f &colour){}
void DrawToolQt3D::drawTetrahedra(const std::vector<Vector3> &points, const Vec4f& colour){}
//Scale each tetrahedron
void DrawToolQt3D::drawScaledTetrahedra(const std::vector<Vector3> &points, const Vec4f& colour, const float scale){}

void DrawToolQt3D::drawHexahedron(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3,
    const Vector3 &p4, const Vector3 &p5, const Vector3 &p6, const Vector3 &p7, const Vec4f &colour){}
void DrawToolQt3D::drawHexahedra(const std::vector<Vector3> &points, const Vec4f& colour){}
//Scale each hexahedron
void DrawToolQt3D::drawScaledHexahedra(const std::vector<Vector3> &points, const Vec4f& colour, const float scale){}

void DrawToolQt3D::drawSphere(const Vector3 &p, float radius){}
void DrawToolQt3D::drawEllipsoid(const Vector3 &p, const Vector3 &radii){}

void DrawToolQt3D::drawBoundingBox(const Vector3 &min, const Vector3 &max, float size){}

void DrawToolQt3D::draw3DText(const Vector3 &p, float scale, const Vec4f &color, const char* text){}
void DrawToolQt3D::draw3DText_Indices(const std::vector<Vector3> &positions, float scale, const Vec4f &color){}

void DrawToolQt3D::pushMatrix(){}

void DrawToolQt3D::popMatrix(){}

void DrawToolQt3D::multMatrix(float*){}

void DrawToolQt3D::scale(float){}
void DrawToolQt3D::translate(float x, float y, float z){}
void DrawToolQt3D::setMaterial(const Vec4f &colour){}

void DrawToolQt3D::resetMaterial(const Vec4f &colour){}
void DrawToolQt3D::resetMaterial(){}

void DrawToolQt3D::setPolygonMode(int _mode, bool _wireframe){}

void DrawToolQt3D::setLightingEnabled(bool _isAnabled){}

void DrawToolQt3D::enableBlending(){}
void DrawToolQt3D::disableBlending(){}

void DrawToolQt3D::enableLighting(){}
void DrawToolQt3D::disableLighting(){}

void DrawToolQt3D::enableDepthTest(){}
void DrawToolQt3D::disableDepthTest(){}

void DrawToolQt3D::saveLastState(){}
void DrawToolQt3D::restoreLastState(){}

void DrawToolQt3D::writeOverlayText(int x, int y, unsigned fontSize, const Vec4f &color, const char* text){}

void DrawToolQt3D::readPixels(int x, int y, int w, int h, float* rgb, float* z){}

void DrawToolQt3D::clear() {}

} // namespace qt3d

} // namespace sofaqtquickgui
