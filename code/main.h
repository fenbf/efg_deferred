/** @file main.h
 *  @brief common includes, function prototypes for samples
 *
 *	Effects in Games, June 2012
 */

#pragma once

#define WND_TITLE "OpenGL Sample"

#define WND_START_WIDTH  800
#define WND_START_HEIGHT 600

//#define DO_NOT_SHOW_CONSOLE

///////////////////////////////////////////////////////////////////////////////
extern double gAppTime;	// global app time in seconds
extern float gFps;

extern unsigned int gMainWindowWidth;
extern unsigned int gMainWindowHeight;

extern TwBar *gMainTweakBar;

///////////////////////////////////////////////////////////////////////////////
bool initApp();
void cleanUp();

// window:
void changeSize(int w, int h);

// keyboard:
void processNormalKeys(unsigned char key, int x, int y);
void pressSpecialKey(int key, int x, int y);
void releaseSpecialKey(int key, int x, int y);

// mouse:
void processMouse(int button, int state, int x, int y);
void processMouseMotion(int x, int y);
void processMousePassiveMotion(int x, int y);

// update & rendering
void updateScene(double deltaTime);
void renderScene();
