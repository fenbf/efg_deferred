/** @file SimpleScene.h
 *  @brief simple scene object
 *
 *	Effects in Games, June 2012
 */


class SimpleScene 
{
public:
    /// current proj matrix, used when rendering objects
    static glm::mat4 sProjectionMatrix;
    /// current modelview matrix, used when rendering objects
    static glm::mat4 sModelviewMatrix;

protected:
    GLuint mPlaneTexture;
    GLuint mCubeTexture;
	GLuint mPlaneVao;
    GLuint mCubeVao;
    GLuint mPlaneVbo;
    GLuint mCubeVbo;
    GLint  mConeVertCount;
public:
	SimpleScene() { mPlaneTexture = mCubeTexture = mPlaneVao = mCubeVao = mPlaneVbo = mCubeVbo = 0; mConeVertCount = 0; }
	~SimpleScene() { glDeleteTextures(1, &mPlaneTexture); glDeleteTextures(1, &mCubeTexture); glDeleteVertexArrays(1, &mPlaneVao); glDeleteVertexArrays(1, &mCubeVao); glDeleteBuffers(1, &mPlaneVbo); glDeleteBuffers(1, &mCubeVbo);  }

	virtual bool init(); 
	virtual void render(geometryUtils::TransformState transformState);
};
