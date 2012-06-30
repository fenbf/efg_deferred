/** @file ShaderUtils.h
 *  @brief useful functions for shader management
 *
 *	Effects in Games, June 2012
 */


#pragma once

class Shader;
class ShaderProgram;

namespace shaderUtils
{
/// by default it is false and only the filename is printed
extern bool gPrintFullPathToShaderFile;

bool loadAndBuildShaderPairFromFile(ShaderProgram *outProg, const char *vs, const char *fs);
bool loadAndBuildShaderPairFromSource(ShaderProgram *outProg, const char *vsSource, const char *fsSource);
void disableAllShaders();


} // namespace shaderUtils
