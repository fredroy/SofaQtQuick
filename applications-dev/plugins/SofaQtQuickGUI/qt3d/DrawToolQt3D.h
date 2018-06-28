#ifndef SOFAQTQUICKGUI_HELPER_DRAWTOOQT3DL_H
#define SOFAQTQUICKGUI_HELPER_DRAWTOOQT3DL_H

#include <sofa/core/visual/DrawTool.h>

namespace sofaqtquickgui
{

namespace qt3d
{

class DrawToolQt3D : public sofa::core::visual::DrawTool
{
    typedef sofa::core::visual::DrawTool Inherited;

public:
    DrawToolQt3D();
    virtual ~DrawToolQt3D() {}

    virtual void init();

    /// @name Primitive rendering methods
    /// @{
    virtual void drawPoints(const std::vector<Vector3> &points, float size, const  Vec4f& colour);
    virtual void drawPoints(const std::vector<Vector3> &points, float size, const std::vector<Vec4f>& colour);

    virtual void drawLine(const Vector3 &p1, const Vector3 &p2, const Vec4f& colour);
    virtual void drawLines(const std::vector<Vector3> &points, float size, const Vec4f& colour);
    virtual void drawLines(const std::vector<Vector3> &points, float size, const std::vector<Vec4f>& colours);
    virtual void drawLines(const std::vector<Vector3> &points, const std::vector< Vec2i > &index, float size, const Vec4f& colour);

    virtual void drawLineStrip(const std::vector<Vector3> &points, float size, const Vec4f& colour);
    virtual void drawLineLoop(const std::vector<Vector3> &points, float size, const Vec4f& colour);

    virtual void drawTriangles(const std::vector<Vector3> &points, const Vec4f& colour);
    virtual void drawTriangles(const std::vector<Vector3> &points, const Vector3& normal, const Vec4f& colour);
    virtual void drawTriangles(const std::vector<Vector3> &points,
        const std::vector< Vec3i > &index,
        const std::vector<Vector3>  &normal,
        const Vec4f& colour);
    virtual void drawTriangles(const std::vector<Vector3> &points,
        const std::vector< Vec4f > &colour);
    virtual void drawTriangles(const std::vector<Vector3> &points,
        const std::vector<Vector3>  &normal,
        const std::vector< Vec4f > &colour);
    virtual void drawTriangleStrip(const std::vector<Vector3> &points,
        const std::vector<Vector3>  &normal,
        const Vec4f& colour);
    virtual void drawTriangleFan(const std::vector<Vector3> &points,
        const std::vector<Vector3>  &normal,
        const Vec4f& colour);

    virtual void drawFrame(const Vector3& position, const Quaternion &orientation, const Vec3f &size);
    virtual void drawFrame(const Vector3& position, const Quaternion &orientation, const Vec3f &size, const Vec4f &colour);

    virtual void drawSpheres(const std::vector<Vector3> &points, const std::vector<float>& radius, const Vec4f& colour);
    virtual void drawSpheres(const std::vector<Vector3> &points, float radius, const Vec4f& colour);
    virtual void drawFakeSpheres(const std::vector<Vector3> &points, const std::vector<float>& radius, const Vec4f& colour);
    virtual void drawFakeSpheres(const std::vector<Vector3> &points, float radius, const Vec4f& colour);

    virtual void drawCone(const Vector3& p1, const Vector3 &p2, float radius1, float radius2, const Vec4f& colour, int subd = 16);

    /// Draw a cube of size one centered on the current point.
    virtual void drawCube(const float& radius, const Vec4f& colour, const int& subd = 16);

    virtual void drawCylinder(const Vector3& p1, const Vector3 &p2, float radius, const Vec4f& colour, int subd = 16);

    virtual void drawCapsule(const Vector3& p1, const Vector3 &p2, float radius, const Vec4f& colour, int subd = 16);

    virtual void drawArrow(const Vector3& p1, const Vector3 &p2, float radius, const Vec4f& colour, int subd = 16);
    virtual void drawArrow(const Vector3& p1, const Vector3 &p2, float radius, float coneLength, const Vec4f& colour, int subd = 16);

    /// Draw a cross (3 lines) centered on p
    virtual void drawCross(const Vector3&p, float length, const Vec4f& colour);

    /// Draw a plus sign of size one centered on the current point.
    virtual void drawPlus(const float& radius, const Vec4f& colour, const int& subd = 16);

    virtual void drawPoint(const Vector3 &p, const Vec4f &c);
    virtual void drawPoint(const Vector3 &p, const Vector3 &n, const Vec4f &c);

    virtual void drawTriangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3,
        const Vector3 &normal);
    virtual void drawTriangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3,
        const Vector3 &normal, const Vec4f &c);
    virtual void drawTriangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3,
        const Vector3 &normal,
        const Vec4f &c1, const Vec4f &c2, const Vec4f &c3);
    virtual void drawTriangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3,
        const Vector3 &normal1, const Vector3 &normal2, const Vector3 &normal3,
        const Vec4f &c1, const Vec4f &c2, const Vec4f &c3);

    virtual void drawQuad(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &p4,
        const Vector3 &normal);
    virtual void drawQuad(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &p4,
        const Vector3 &normal, const Vec4f &c);
    virtual void drawQuad(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &p4,
        const Vector3 &normal,
        const Vec4f &c1, const Vec4f &c2, const Vec4f &c3, const Vec4f &c4);
    virtual void drawQuad(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &p4,
        const Vector3 &normal1, const Vector3 &normal2, const Vector3 &normal3, const Vector3 &normal4,
        const Vec4f &c1, const Vec4f &c2, const Vec4f &c3, const Vec4f &c4);
    virtual void drawQuads(const std::vector<Vector3> &points, const Vec4f& colour);
    virtual void drawQuads(const std::vector<Vector3> &points, const std::vector<Vec4f>& colours);

    virtual void drawTetrahedron(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vec4f &colour);
    virtual void drawTetrahedra(const std::vector<Vector3> &points, const Vec4f& colour);
    //Scale each tetrahedron
    virtual void drawScaledTetrahedra(const std::vector<Vector3> &points, const Vec4f& colour, const float scale);

    virtual void drawHexahedron(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3,
        const Vector3 &p4, const Vector3 &p5, const Vector3 &p6, const Vector3 &p7, const Vec4f &colour);
    virtual void drawHexahedra(const std::vector<Vector3> &points, const Vec4f& colour);
    //Scale each hexahedron
    virtual void drawScaledHexahedra(const std::vector<Vector3> &points, const Vec4f& colour, const float scale);

    virtual void drawSphere(const Vector3 &p, float radius);
    virtual void drawEllipsoid(const Vector3 &p, const Vector3 &radii);

    virtual void drawBoundingBox(const Vector3 &min, const Vector3 &max, float size = 1.0);

    virtual void draw3DText(const Vector3 &p, float scale, const Vec4f &color, const char* text);
    virtual void draw3DText_Indices(const std::vector<Vector3> &positions, float scale, const Vec4f &color);
    /// @}

    /// @name Transformation methods.
    /// @{
    virtual void pushMatrix();

    virtual void popMatrix();

    virtual void multMatrix(float*);

    virtual void scale(float);
    virtual void translate(float x, float y, float z);
    /// @}

    /// @name Drawing style methods.
    virtual void setMaterial(const Vec4f &colour);

    virtual void resetMaterial(const Vec4f &colour);
    virtual void resetMaterial();

    virtual void setPolygonMode(int _mode, bool _wireframe);

    virtual void setLightingEnabled(bool _isAnabled);
    /// @}

    virtual void enableBlending();
    virtual void disableBlending();

    virtual void enableLighting();
    virtual void disableLighting();

    virtual void enableDepthTest();
    virtual void disableDepthTest();

    /// @name States (save/restore)
    virtual void saveLastState();
    virtual void restoreLastState();

    /// @name Overlay methods

    /// draw 2D text at position (x,y) from top-left corner
    virtual void writeOverlayText(int x, int y, unsigned fontSize, const Vec4f &color, const char* text);

    // @name Color Buffer method
    virtual void readPixels(int x, int y, int w, int h, float* rgb, float* z = NULL);
    /// @}

    virtual void clear();
};

} // namespace qt3d

} // namespace sofaqtquickgui

#endif // SOFAQTQUICKGUI_HELPER_DRAWTOOQT3DL_H
