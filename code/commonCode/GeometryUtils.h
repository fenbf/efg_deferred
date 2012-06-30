/** @file GeometryUtils.h
 *  @brief utils for geometry creation
 *
 *	Effects in Games, June 2012
 */

#pragma once

namespace geometryUtils
{
    static const int CUBE_VERTS_COUNT = 36;
    
    class TransformState
    {
    private:
        GLint mLocationModelView;
        GLint mLocationNormalMatrix;
        const glm::mat4 *mModelViewMatrix;

    public:
        TransformState(GLint locationModelView, GLint locationNormalMatrix, const glm::mat4 *modelViewMatrix);
        ~TransformState() { }

        void translate(float x, float y, float z);
        void setState();
    };
    
    /// creates quad with the size -1 to 1 (x, and y, z is 0.0), and UV tex coords
    /// creates new VBO buffer (single one)
    /// layout: 3 floats for position and 2 for UV
    /// draw using GL_TRIANGLE_SRTIPE
	void create2DUniformQuad(GLuint* vbo, GLuint *vao);

    /// creates a cube, pos with normals and texcoords
    /// layout: 3 floats for position, 3 floats for normals and 2 for UV
    /// draw using GL_TRIANGLES
    void createCube( GLuint* vbo, GLuint *vao, const glm::vec3 &center, const glm::vec3 &size );

    /// creates a cone, pos with normals and texcoords
    /// layout: 3 floats for position, 3 floats for normals and 2 for UV
    /// draw using GL_TRIANGLES
    /// \return number of vertices for triangles to draw
    int createCone( GLuint* vbo, GLuint *vao, float radius, float h, const glm::vec3 &center, int steps );

    /// creates a quad, pos with normals and texcoords
    /// w and h is the width and height/deph of the quad
    /// layout: 3 floats for position, 3 floats for normals and 2 for UV
    /// draw using GL_TRIANGLES
    void createQuad(GLuint* vbo, GLuint *vao, const glm::vec3 &point, const glm::vec3 &normal, const glm::vec3 &up, float w, float h, float texScale);
}