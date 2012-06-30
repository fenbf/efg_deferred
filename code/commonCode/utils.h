/** @file utils.h
 *  @brief useful utilities for opengl based apps
 *
 *	Effects in Games, June 2012
 */

#pragma once

/** logging will be by using simple "printf" function */
#define UTILS_LOG_WITH_PRINTF

#ifdef UTILS_LOG_WITH_PRINTF
	#define LOG(msg, ...) { printf(msg, __VA_ARGS__); printf("\n"); }
	#define LOG_SUCCESS(msg, ...) { printf("SUCCESS: "); printf(msg, __VA_ARGS__); printf("\n"); }
	#define LOG_ERROR(msg, ...) { printf("ERR in %s at line %d: ", __FUNCTION__, __LINE__); printf(msg, __VA_ARGS__); printf("\n"); }
#else
	#define LOG(msg, ...) { }
	#define LOG_SUCCESS(msg, ...) { }
	#define LOG_ERROR(msg, ...) { }
#endif

#define WAIT_FOR_CONSOLE_INPUT() { printf("\npress ENTER/RETURN to exit..."); fflush(stdout); getchar(); }

#ifdef _DEBUG
	#define CHECK_OPENGL_ERRORS() utils::checkGLErrors(__FUNCTION__, __FILE__, __LINE__);
#else
	#define CHECK_OPENGL_ERRORS() 
#endif

namespace utils
{

///////////////////////////////////////////////////////////////////////////////
bool initGL(bool vsync);

void calculateFps(float *fps);

void updateTimer(double *deltaTime, double *appTime, const double REFRESH_TIME = 1.0/60.0);

bool checkGLErrors(const char *func, const char * file, int line);

inline float randFloatRange(float a, float b);

inline const char *fileNameFromPath(const char *filePath);

} // namespce utils	

///////////////////////////////////////////////////////////////////////////////
// inline

inline float utils::randFloatRange(float a, float b)
{
	return a + (float)rand()/(float)RAND_MAX * (b-a);
}

inline const char *utils::fileNameFromPath(const char *filePath)
{
	const char *s = strrchr(filePath, '\\');
	if (s == NULL) 
		s = strrchr(filePath, '/');
	return s == NULL ? filePath : s+1;
}