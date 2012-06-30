/** @file renderables.cpp
 *  @brief classes that represent renderable objects
 *
 *	Effects in Games, June 2012
 */

#include "stdafx.h"
#include "utils.h"

#include "Shader.h"
#include "ShaderProgram.h"
#include "ShaderUtils.h"

#include "Renderable.h"

///////////////////////////////////////////////////////////////////////////////
glm::mat4 Renderable::sProjectionMatrix;
glm::mat4 Renderable::sModelviewMatrix;

