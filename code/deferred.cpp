/** @file deferred.cpp
 *  @brief deferred sample application
 *
 *	Effects in Games, June 2012
 */

#include "stdafx.h"
#include "main.h"
#include "utils.h"

#include "Shader.h"
#include "ShaderProgram.h"
#include "ShaderUtils.h"
#include "Framebuffer.h"
#include "GeometryUtils.h"

#include "SimpleScene.h"

#include "DeferredRenderer.h"

using namespace std;

struct Cam
{
    glm::vec3 mTarget;
    glm::vec3 mDir;
    glm::vec3 mRight;
    glm::vec3 mFront;
    float     mZoom;
};

///////////////////////////////////////////////////////////////////////////////
// globals

// transformations & camera
Cam gCam;
glm::mat4 gModelViewMatrix;
glm::mat4 gProjectionMatrix;

SimpleScene gScene;
DeferredRenderer gRenderer;
std::vector<DeferredLight> gLights;
std::vector<glm::vec4> gLightAngles;
const int NUM_LIGHTS = 16;

bool gAnimate;

///////////////////////////////////////////////////////////////////////////////

bool initApp() 
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	srand((unsigned int)time(NULL));   

	if (gScene.init() == false)
		return false;	

    if (gRenderer.init() == false)
        return false;

    for (int i = 0; i < NUM_LIGHTS; ++i)
    {
        float col = (float)(rand())/(float)RAND_MAX;
        float col2 = (float)(rand())/(float)RAND_MAX;
        float rad = (float)(rand())/(float)RAND_MAX;
        float speed = (float)(rand())/(float)RAND_MAX;
        gLights.push_back(DeferredLight(col, col2, col*col2,  col, col, col2, col*100.0f));
        gLightAngles.push_back(glm::vec4(5.0f+rad*20.0f, 5.0f+col*20.0f, col2*5.0f, speed+0.5f));
    }

    //
    // tweaks
    //
    gCam.mTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    gCam.mDir    = glm::vec3(0.0f, -0.5f, -1.0f);
    TwAddVarRW(gMainTweakBar, "camera dir", TW_TYPE_DIR3F, &gCam.mDir.x, "");
    gCam.mZoom = 7.0f;
    TwAddVarRW(gMainTweakBar, "zoom", TW_TYPE_FLOAT, &gCam.mZoom, "min=0.1 max=30.0 step=0.05");

    gAnimate = true;
    TwAddVarRW(gMainTweakBar, "animate", TW_TYPE_BOOLCPP, &gAnimate, "");
	return true;
}

///////////////////////////////////////////////////////////////////////////////
void cleanUp()
{

}

///////////////////////////////////////////////////////////////////////////////
void changeSize(int w, int h)
{
    float aspect = (float)w/(float)h;

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // setup projection matrix
	gProjectionMatrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
}

///////////////////////////////////////////////////////////////////////////////
void processNormalKeys(unsigned char key, int x, int y) 
{
	if (key == 27) 
		exit(0);
    else if (key == ' ')
    {
        gCam.mTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        gCam.mDir    = glm::vec3(0.0f, -0.5f, -1.0f);
        gCam.mZoom   = 7.0f;
    }
}

///////////////////////////////////////////////////////////////////////////////
void pressSpecialKey(int key, int x, int y) 
{
    if (key == GLUT_KEY_UP)
        gCam.mTarget += gCam.mFront;
    else if (key == GLUT_KEY_DOWN)
        gCam.mTarget -= gCam.mFront;

	else if (key == GLUT_KEY_LEFT)
        gCam.mTarget -= gCam.mRight;
    else if (key == GLUT_KEY_RIGHT)
        gCam.mTarget += gCam.mRight;
}


///////////////////////////////////////////////////////////////////////////////
void releaseSpecialKey(int key, int x, int y) 
{
	
}

///////////////////////////////////////////////////////////////////////////////
void processMouse(int button, int state, int x, int y)
{
	// todo: add some custom code...
}

///////////////////////////////////////////////////////////////////////////////
void processMouseMotion(int x, int y)
{
	// todo: add some custom code...	
}

///////////////////////////////////////////////////////////////////////////////
void processMousePassiveMotion(int x, int y)
{
	// todo: add some custom code...	
}

///////////////////////////////////////////////////////////////////////////////
void updateScene(double deltaTime) 
{
	if (!gAnimate) 
        return;

    for (int i = 0; i < NUM_LIGHTS; ++i)
    {
        gLightAngles[i].z += (float)deltaTime*gLightAngles[i].w;
        gLights[i].mPos.x = sinf(gLightAngles[i].z) * gLightAngles[i].x;
        gLights[i].mPos.y = 3.0f+1.0f*sinf(gLightAngles[i].z);
        gLights[i].mPos.z = cosf(gLightAngles[i].z) * gLightAngles[i].y;
    }
}

///////////////////////////////////////////////////////////////////////////////
void renderScene() 
{	
	//
	// camera
	//

    gCam.mFront = glm::vec3(gCam.mDir.x, 0.0f, gCam.mDir.z);
    gCam.mRight = glm::vec3(-gCam.mFront.z, 0.0f, gCam.mFront.x);
	gModelViewMatrix = glm::lookAt(gCam.mTarget - gCam.mDir*gCam.mZoom, gCam.mTarget, glm::vec3(0.0f, 1.0f, 0.0f)); 

    gRenderer.beginGeometryStage(gProjectionMatrix);	    

    GLint mvLoc = gRenderer.geometryStageProgram()->getUniformLocation("modelviewMatrix");
    GLint normLoc = gRenderer.geometryStageProgram()->getUniformLocation("normalMatrix");
    geometryUtils::TransformState ts = geometryUtils::TransformState(mvLoc, normLoc, &gModelViewMatrix);
    ts.setState();
    gScene.render(ts);

    gRenderer.endGeometryStage();
    
    gRenderer.finalStage(gModelViewMatrix, gLights);

    gRenderer.displayTex(DeferredRenderer::ttColor,  glm::vec4(0.8f, 0.8f, 0.15, 0.15));
    gRenderer.displayTex(DeferredRenderer::ttNormal, glm::vec4(0.8f, 0.5f, 0.15, 0.15));
    gRenderer.displayTex(DeferredRenderer::ttPos,    glm::vec4(0.8f, 0.2f, 0.15, 0.15));
}