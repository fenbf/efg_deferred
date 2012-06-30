/** @file DeferredRenderer.cpp
 *  @brief 
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

#include "DeferredRenderer.h"

GLuint createEmptyTexture(int w, int h, int internalFormat, int format, GLenum dataType);

DeferredRenderer::DeferredRenderer()
{
    mQuadVbo = mQuadVao = 0;
    mTexPos = mTexDepth = mTexNormals = mTexColor = 0;
}

DeferredRenderer::~DeferredRenderer()
{
    glDeleteBuffers(1, &mQuadVbo); 
    glDeleteVertexArrays(1, &mQuadVao);

    glDeleteTextures(1, &mTexPos);
    glDeleteTextures(1, &mTexDepth);
    glDeleteTextures(1, &mTexNormals);
    glDeleteTextures(1, &mTexColor);
}

bool DeferredRenderer::init()
{
	//
	// shaders
	//
	if (!shaderUtils::loadAndBuildShaderPairFromFile(&mGeometryStageProg, "shaders/geometryStage.vert", "shaders/geometryStage.frag"))
		return false;

    if (!shaderUtils::loadAndBuildShaderPairFromFile(&mFinalStageProg, "shaders/finalStage.vert", "shaders/finalStage.frag"))
        return false;

    if (!shaderUtils::loadAndBuildShaderPairFromFile(&mDisplayTexProg, "shaders/displayTex.vert", "shaders/displayTex.frag"))
        return false;

    //
    // framebuffer
    //
    mTexPos     = createEmptyTexture(gMainWindowWidth, gMainWindowHeight, GL_RGB16F, GL_RGB, GL_FLOAT);
    mTexDepth   = createEmptyTexture(gMainWindowWidth, gMainWindowHeight, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_FLOAT);
    mTexNormals = createEmptyTexture(gMainWindowWidth, gMainWindowHeight, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
    mTexColor   = createEmptyTexture(gMainWindowWidth, gMainWindowHeight, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);

    mGeometryFbo.createAndBind();
    mGeometryFbo.attachTextureAsColorTarget(0, mTexPos,  gMainWindowWidth, gMainWindowHeight, GL_TEXTURE_2D);
    mGeometryFbo.attachTextureAsColorTarget(1, mTexNormals,  gMainWindowWidth, gMainWindowHeight, GL_TEXTURE_2D);
    mGeometryFbo.attachTextureAsColorTarget(2, mTexColor,    gMainWindowWidth, gMainWindowHeight, GL_TEXTURE_2D);
    mGeometryFbo.attachTextureAsDepthTarget(mTexDepth,       gMainWindowWidth, gMainWindowHeight, GL_TEXTURE_2D);
    mGeometryFbo.setDrawBuffers();

    if (mGeometryFbo.check() == false)
        return false;

    FrameBuffer::bindSystemFrameBuffer();

	//
	// geometry
	//
    geometryUtils::create2DUniformQuad(&mQuadVbo, &mQuadVao);	

	//
	// tweaks
	//


	return true;
}

void DeferredRenderer::beginGeometryStage(glm::mat4 &projMat)
{
    mGeometryFbo.bind(true);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mGeometryStageProg.use();
    mGeometryStageProg.uniformMatrix4f("projectionMatrix", glm::value_ptr(projMat));
}

void DeferredRenderer::endGeometryStage()
{

}

void DeferredRenderer::finalStage(glm::mat4 &mvMat, const std::vector<DeferredLight> &lights)
{
    FrameBuffer::bindSystemFrameBuffer(gMainWindowWidth, gMainWindowHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mFinalStageProg.use();
	
	//mFinalStageProg.uniform2f("texelSize", 1.0f/(float)gMainWindowWidth, 1.0f/(float)gMainWindowHeight);
	mFinalStageProg.uniform1i("texPos",  0);
    //mFinalStageProg.uniform1i("texDepth",  0);
    mFinalStageProg.uniform1i("texNormal", 1);
    mFinalStageProg.uniform1i("texColor",  2);

   /* mFinalStageProg.uniform1f("nearPlane", 0.1f);
    mFinalStageProg.uniform1f("farPlane", 1000.0f);
    mFinalStageProg.uniform1f("viewWidth", (float)gMainWindowWidth);
    mFinalStageProg.uniform1f("viewHeight", (float)gMainWindowHeight);*/

    glActiveTexture(GL_TEXTURE2);
	mGeometryFbo.bindColorTargetAsTexture(2);
    glActiveTexture(GL_TEXTURE1);
    mGeometryFbo.bindColorTargetAsTexture(1);
    glActiveTexture(GL_TEXTURE0);
    //mGeometryFbo.bindDepthAsTexture();
    mGeometryFbo.bindColorTargetAsTexture(0);

	glBindVertexArray(mQuadVao);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    
    glm::vec4 light;
    
    for (size_t i = 0; i < lights.size(); ++i)
    {
        light = mvMat * glm::vec4(lights[i].mPos, 1.0f);
        mFinalStageProg.uniform3f("lightPos", light.x, light.y, light.z);
        mFinalStageProg.uniform3f("lightDiffuse", lights[i].mDiffuse.x, lights[i].mDiffuse.y, lights[i].mDiffuse.z);
        mFinalStageProg.uniform4f("lightSpecular", lights[i].mDiffuse.x, lights[i].mDiffuse.y, lights[i].mDiffuse.z, lights[i].mSpecular.w);
	    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

	glBindVertexArray(0);

	mFinalStageProg.disable();
}

void DeferredRenderer::displayTex(DeferredRenderer::TexType tt, const glm::vec4 &rect )
{
    mDisplayTexProg.use();
    mDisplayTexProg.uniform4f("rect", rect.x, rect.y, rect.z, rect.w);
    mDisplayTexProg.uniform1i("tex", 0);

    glBindTexture(GL_TEXTURE_2D, (tt == ttNormal ? mTexNormals : tt == ttColor ? mTexColor : mTexPos));
    glBindVertexArray(mQuadVao);

    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(0);   
    mDisplayTexProg.disable();
}

GLuint createEmptyTexture(int w, int h, int internalFormat, int format, GLenum dataType)
{
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);		
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, dataType, NULL); 
    return id;
}