/** @file main.cpp
 *  @brief core code for the sample, init, freeGlut setup, etc, etc
 *
 *	Effects in Games, June 2012
 */

#include "stdafx.h"
#include "main.h"
#include "utils.h"

#include "Shader.h"
#include "ShaderProgram.h"
#include "ShaderUtils.h"


#ifdef DO_NOT_SHOW_CONSOLE
	#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif

//#define USE_ONLY_CORE_OPENGL_PROFILE

using namespace std;

#pragma region globals

///////////////////////////////////////////////////////////////////////////////

// time:
double gAppTime = 0.0;	// global app time in seconds
float gFps = 0.0f;

unsigned int gMainWindowWidth = WND_START_WIDTH;
unsigned int gMainWindowHeight = WND_START_HEIGHT;

TwBar *gMainTweakBar = NULL;

#pragma endregion


///////////////////////////////////////////////////////////////////////////////
bool initApp();
void cleanUp();

// window:
void mainChangeSize(int w, int h);

// mouse:
void mainProcessMouse(int button, int state, int x, int y);
void mainProcessMouseMotion(int x, int y);
void mainProcessMousePassiveMotion(int x, int y);

// update & rendering
void mainIdle();

///////////////////////////////////////////////////////////////////////////////
// entry point
int main(int argc, char **argv)
{
	//
	// init GLUT
	//
	glutInit(&argc, argv);

#ifdef USE_ONLY_CORE_OPENGL_PROFILE
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
#endif

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(WND_START_WIDTH, WND_START_HEIGHT);
	// glutMainLoop returns and we can release our resources
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	if (glutCreateWindow(WND_TITLE) < 1)
	{
		LOG_ERROR("Cannot create the main GLUT window!");
        WAIT_FOR_CONSOLE_INPUT();
		return 1;
	}

	// set callbacks:
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressSpecialKey);
	glutSpecialUpFunc(releaseSpecialKey);
	glutMouseFunc(mainProcessMouse);
	glutMotionFunc(mainProcessMouseMotion);
	glutPassiveMotionFunc(mainProcessMousePassiveMotion);
	glutDisplayFunc(renderScene);
	glutIdleFunc(mainIdle);
	glutReshapeFunc(mainChangeSize);


	//
	// 'init' OpenGL: GLEW... and print some basic info
	//
	if (utils::initGL(true) == false) 
    {
        WAIT_FOR_CONSOLE_INPUT();
		return 1;
    }

	
	// Initialize AntTweakBar
#ifdef USE_ONLY_CORE_OPENGL_PROFILE
    TwInit(TW_OPENGL_CORE, NULL);
#else
	TwInit(TW_OPENGL, NULL);
#endif
	gMainTweakBar = TwNewBar("TweakBar");
	TwAddVarRO(gMainTweakBar, "FPS", TW_TYPE_FLOAT, &gFps, NULL);

	//
	// init whole application:
	//
	if (initApp() == false)
	{
		LOG_ERROR("cannot init application...");
        WAIT_FOR_CONSOLE_INPUT();
		return 1;
	}

	//
	// start rendering and animation:
	//
	glutMainLoop();

	//
	// finish:
	//
	cleanUp();
	TwTerminate();

	return 0;
}


#pragma region input

///////////////////////////////////////////////////////////////////////////////
void mainChangeSize(int w, int h)
{
	gMainWindowWidth = w;
	gMainWindowHeight = h;		

    changeSize(w, h);

	// Send the new window size to AntTweakBar
    TwWindowSize(w, h);
}

///////////////////////////////////////////////////////////////////////////////
void mainProcessMouse(int button, int state, int x, int y)
{
	processMouse(button, state, x, y);
	
	// send message to antTweakBar
	TwEventMouseButtonGLUT(button, state, x, y);
}

///////////////////////////////////////////////////////////////////////////////
void mainProcessMouseMotion(int x, int y)
{
	processMouseMotion(x, y);
	
	// send message to antTweakBar
	TwEventMouseMotionGLUT(x, y);		
}

///////////////////////////////////////////////////////////////////////////////
void mainProcessMousePassiveMotion(int x, int y)
{
	processMousePassiveMotion(x, y);
	
	// send message to antTweakBar
	TwEventMouseMotionGLUT(x, y);		
}

#pragma endregion

#pragma region update & render

///////////////////////////////////////////////////////////////////////////////
void mainIdle()
{
	double deltaTime;
	utils::updateTimer(&deltaTime, &gAppTime);
	
	utils::calculateFps(&gFps);

	// call Update:
	updateScene(deltaTime);

	// render frame:
	renderScene();

	TwDraw();

	glutSwapBuffers();
}

#pragma endregion
