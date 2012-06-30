/** @file SimpleScene.cpp
 *  @brief simple scene object
 *
 *	Effects in Games, June 2012
 */

#include "stdafx.h"
#include "main.h"
#include "utils.h"

#include "Shader.h"
#include "ShaderProgram.h"
#include "ShaderUtils.h"
#include "GeometryUtils.h"

#include "SimpleScene.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
glm::mat4 SimpleScene::sProjectionMatrix;
glm::mat4 SimpleScene::sModelviewMatrix;

///////////////////////////////////////////////////////////////////////////////
bool SimpleScene::init()
{
    //
    // geometry
    //
    geometryUtils::createQuad(&mPlaneVbo, &mPlaneVao, 
                              glm::vec3(0.0f, 0.0f, 0.0f), 
                              glm::vec3(0.0f, 1.0f, 0.0f), 
                              glm::vec3(0.0f, 0.0f, -1.0f), 
                              40.0f, 40.0f,
                              10.0f); 
    
    geometryUtils::createCube(&mCubeVbo, &mCubeVao, glm::vec3(0.0f, 0.5001f, 0.0f ), glm::vec3(0.5f));

	//
	// textures
	//
	mPlaneTexture = SOIL_load_OGL_texture("data/plane.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
                                          SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);

    mCubeTexture = SOIL_load_OGL_texture("data/stripes.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
                                         SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);

	if (mPlaneTexture == 0 || mCubeTexture == 0)
	{
		LOG_ERROR("cannot load texture!");
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
void SimpleScene::render(geometryUtils::TransformState transformState)
{
    //
    // plane
    //
    glBindTexture(GL_TEXTURE_2D, mPlaneTexture);   
    glBindVertexArray(mPlaneVao);    
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //
    // cubes
    //
    glBindTexture(GL_TEXTURE_2D, mCubeTexture);
    glBindVertexArray(mCubeVao);    

    float rad = 1.0f;
    float ang = 0.0f;
    for (int i = 0; i < 32; ++i)
    {
        transformState.translate(rad*sinf(ang), 0.0f, rad*cosf(ang));    
        glDrawArrays(GL_TRIANGLES, 0, geometryUtils::CUBE_VERTS_COUNT);

        if (i%2)
        {
            transformState.translate(rad*sinf(ang), 1.0f, rad*cosf(ang));    
            glDrawArrays(GL_TRIANGLES, 0, geometryUtils::CUBE_VERTS_COUNT);
        }

        rad += 0.5f;
        ang += 0.4f;
    }
    
    glBindVertexArray(0);
}