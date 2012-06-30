/** @file GeometryUtils.cpp
 *  @brief utils for geometry creation
 *
 *	Effects in Games, June 2012
 */

#include "stdafx.h"
#include "GeometryUtils.h"

namespace geometryUtils
{

TransformState::TransformState(GLint locationModelView, GLint locationNormalMatrix, const glm::mat4 *modelViewMatrix ) :
    mLocationModelView(locationModelView),
    mLocationNormalMatrix(locationNormalMatrix),
    mModelViewMatrix(modelViewMatrix)
{

}

void TransformState::translate(float x, float y, float z)
{
    glm::mat4 tempMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    tempMatrix = (*mModelViewMatrix) * tempMatrix;
    glUniformMatrix4fv(mLocationModelView, 1, false, glm::value_ptr(tempMatrix));
    if (mLocationNormalMatrix != -1)
    {
        glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(tempMatrix));
        glUniformMatrix3fv(mLocationNormalMatrix, 1, false, glm::value_ptr(normalMatrix));
    }
}

void TransformState::setState()
{
    glUniformMatrix4fv(mLocationModelView, 1, false, glm::value_ptr(*mModelViewMatrix));
    glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(*mModelViewMatrix));
    if (mLocationNormalMatrix != -1)
    {
        glUniformMatrix3fv(mLocationNormalMatrix, 1, false, glm::value_ptr(normalMatrix));
    }
}

#define VEC_VERTEX(vec, x, y, z, nx, ny, nz, u, v)       \
    vec.push_back(x); vec.push_back(y); vec.push_back(z);    \
    vec.push_back(nx); vec.push_back(ny); vec.push_back(nz); \
    vec.push_back(u); vec.push_back(v);


///////////////////////////////////////////////////////////////////////////////
void create2DUniformQuad(GLuint* vbo, GLuint *vao)
{
	assert(vbo && "vbo is null");
    assert(vao && "vao is null");

	// triangle strip
	static const float quadData[] = {
		-1.0f, -1.0f, 0.0f, // pos
		0.0f, 0.0f,         // tex

		-1.0f, 1.0f, 0.0f,
		0.0f, 1.0f,

		1.0f, -1.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
	};

	glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadData), quadData, GL_STATIC_DRAW);

    const GLsizei STRIDE = sizeof(float)*5;
    glGenVertexArrays(1, vao); 
	glBindVertexArray(*vao);

	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glEnableVertexAttribArray(0);		
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE, 0);
	glEnableVertexAttribArray(1);		
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, STRIDE, (const void *)(sizeof(float)*3));

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void createCube( GLuint* vbo, GLuint *vao, const glm::vec3 &center, const glm::vec3 &size )
{
    assert(vbo && "vbo is null");
    assert(vao && "vao is null");

    std::vector <float> data;
    
    // front
    VEC_VERTEX(data, center.x-size.x, center.y+size.y, center.z+size.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    VEC_VERTEX(data, center.x-size.x, center.y-size.y, center.z+size.z, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    VEC_VERTEX(data, center.x+size.x, center.y-size.y, center.z+size.z, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

    VEC_VERTEX(data, center.x-size.x, center.y+size.y, center.z+size.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    VEC_VERTEX(data, center.x+size.x, center.y-size.y, center.z+size.z, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    VEC_VERTEX(data, center.x+size.x, center.y+size.y, center.z+size.z, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
    
    // right
    VEC_VERTEX(data, center.x+size.x, center.y+size.y, center.z+size.z, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    VEC_VERTEX(data, center.x+size.x, center.y-size.y, center.z+size.z, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    VEC_VERTEX(data, center.x+size.x, center.y-size.y, center.z-size.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    VEC_VERTEX(data, center.x+size.x, center.y+size.y, center.z+size.z, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    VEC_VERTEX(data, center.x+size.x, center.y-size.y, center.z-size.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    VEC_VERTEX(data, center.x+size.x, center.y+size.y, center.z-size.z, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // back
    VEC_VERTEX(data, center.x+size.x, center.y+size.y, center.z-size.z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
    VEC_VERTEX(data, center.x+size.x, center.y-size.y, center.z-size.z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
    VEC_VERTEX(data, center.x-size.x, center.y-size.y, center.z-size.z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

    VEC_VERTEX(data, center.x+size.x, center.y+size.y, center.z-size.z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
    VEC_VERTEX(data, center.x-size.x, center.y-size.y, center.z-size.z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
    VEC_VERTEX(data, center.x-size.x, center.y+size.y, center.z-size.z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
    
    // left
    VEC_VERTEX(data, center.x-size.x, center.y+size.y, center.z-size.z, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    VEC_VERTEX(data, center.x-size.x, center.y-size.y, center.z-size.z, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    VEC_VERTEX(data, center.x-size.x, center.y-size.y, center.z+size.z, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    VEC_VERTEX(data, center.x-size.x, center.y+size.y, center.z-size.z, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    VEC_VERTEX(data, center.x-size.x, center.y-size.y, center.z+size.z, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    VEC_VERTEX(data, center.x-size.x, center.y+size.y, center.z+size.z, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // bottom
    VEC_VERTEX(data, center.x-size.x, center.y-size.y, center.z-size.z, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
    VEC_VERTEX(data, center.x+size.x, center.y-size.y, center.z+size.z, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
    VEC_VERTEX(data, center.x-size.x, center.y-size.y, center.z+size.z, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);

    VEC_VERTEX(data, center.x-size.x, center.y-size.y, center.z-size.z, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
    VEC_VERTEX(data, center.x+size.x, center.y-size.y, center.z-size.z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
    VEC_VERTEX(data, center.x+size.x, center.y-size.y, center.z+size.z, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);    
    
    // top
    VEC_VERTEX(data, center.x-size.x, center.y+size.y, center.z-size.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
    VEC_VERTEX(data, center.x-size.x, center.y+size.y, center.z+size.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    VEC_VERTEX(data, center.x+size.x, center.y+size.y, center.z+size.z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

    VEC_VERTEX(data, center.x-size.x, center.y+size.y, center.z-size.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
    VEC_VERTEX(data, center.x+size.x, center.y+size.y, center.z+size.z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
    VEC_VERTEX(data, center.x+size.x, center.y+size.y, center.z-size.z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*data.size(), &data[0], GL_STATIC_DRAW);

    // vao
    glGenVertexArrays(1, vao); 
    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glEnableVertexAttribArray(0);		
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3+3+2)*sizeof(float), 0);
    glEnableVertexAttribArray(1);		
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3+3+2)*sizeof(float), (const void *)(sizeof(float)*3));
    glEnableVertexAttribArray(2);		
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (3+3+2)*sizeof(float), (const void *)(sizeof(float)*(3+3)));

    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

///////////////////////////////////////////////////////////////////////////////
int createCone( GLuint* vbo, GLuint *vao, float radius, float h, const glm::vec3 &center, int steps )
{    
    assert(vbo && "vbo is null");
    assert(vao && "vao is null");

    glm::vec3 top = center + glm::vec3(0.0f, h, 0.0f);
    glm::vec3 temp, norm;

    float y = radius/h;
    float t = 1.0f/sqrt(y*y+1);
    y *= t;

    std::vector <float> data;

    const float da = (float)M_PI*2.0f/(float)steps;
    const float du = 1.0f/(float)steps;

    for (int i = 0; i < steps; ++i)
    {
        VEC_VERTEX(data, top.x, top.y, top.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
        
        temp = center;
        norm.x = t * cosf((float)i*da);
        norm.y = y;
        norm.z = t * sinf((float)i*da);
        temp.x += radius * cosf((float)i*da);
        temp.z += radius * sinf((float)i*da);
        VEC_VERTEX(data, temp.x, temp.y, temp.z, norm.x, norm.y, norm.z, (float)i*du, 1.0f);

        temp = center;
        norm.x = t * cosf((float)(i+1)*da);
        norm.y = y;
        norm.z = t * sinf((float)(i+1)*da);
        temp.x += radius * cosf((float)(i+1)*da);
        temp.z += radius * sinf((float)(i+1)*da);
        VEC_VERTEX(data, temp.x, temp.y, temp.z, norm.x, norm.y, norm.z, (float)(i+1)*du, 1.0f);
    }

    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*data.size(), &data[0], GL_STATIC_DRAW);

    // vao
    glGenVertexArrays(1, vao); 
    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glEnableVertexAttribArray(0);		
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3+3+2)*sizeof(float), 0);
    glEnableVertexAttribArray(1);		
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3+3+2)*sizeof(float), (const void *)(sizeof(float)*3));
    glEnableVertexAttribArray(2);		
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (3+3+2)*sizeof(float), (const void *)(sizeof(float)*(3+3)));

    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return (int)data.size()/(3+3+2);
}

///////////////////////////////////////////////////////////////////////////////
void createQuad( GLuint* vbo, GLuint *vao, const glm::vec3 &center, const glm::vec3 &normal, const glm::vec3 &up, float w, float h, float texScale )
{
    assert(vbo && "vbo is null");
    assert(vao && "vao is null");

    glm::vec3 left = glm::cross(up, normal);

    glm::vec3 topLeft  = center - (left * w * 0.5f) + (up * h * 0.5f);
    glm::vec3 downLeft = center - (left * w * 0.5f) - (up * h * 0.5f);
    glm::vec3 topRight = center + (left * w * 0.5f) + (up * h * 0.5f);
    glm::vec3 downRight  = center + (left * w *  0.5f) - (up * h * 0.5f);

    // two triangles
    const float quadData[] = {
        // first triangle
        topLeft.x, topLeft.y, topLeft.z, 
        normal.x, normal.y, normal.z,
        0.0f, 0.0f,  

        downLeft.x, downLeft.y, downLeft.z, 
        normal.x, normal.y, normal.z,
        0.0f, texScale,

        downRight.x, downRight.y, downRight.z, 
        normal.x, normal.y, normal.z,
        texScale, texScale,

        // second triangle
        topLeft.x, topLeft.y, topLeft.z, 
        normal.x, normal.y, normal.z,
        0.0f, 0.0f,  

        downRight.x, downRight.y, downRight.z, 
        normal.x, normal.y, normal.z,
        texScale, texScale,

        topRight.x, topRight.y, topRight.z, 
        normal.x, normal.y, normal.z,
        texScale, 0.0f,
    };

	glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadData), quadData, GL_STATIC_DRAW);

    // vao
    glGenVertexArrays(1, vao); 
    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glEnableVertexAttribArray(0);		
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3+3+2)*sizeof(float), 0);
    glEnableVertexAttribArray(1);		
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3+3+2)*sizeof(float), (const void *)(sizeof(float)*3));
    glEnableVertexAttribArray(2);		
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (3+3+2)*sizeof(float), (const void *)(sizeof(float)*(3+3)));

    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}