/** @file renderables.h
 *  @brief classes that represent renderable objects
 *
 *	Effects in Games, June 2012
 */

class Renderable
{
public:
	/// current proj matrix, used when rendering 'Renderable' objects
	static glm::mat4 sProjectionMatrix;
	/// current modelview matrix, used when rendering 'Renderable' objects
	static glm::mat4 sModelviewMatrix;
protected:
	ShaderProgram mProgram;
	/// vertex array for this object
	GLuint mVao;
	/// texture if needed
	GLuint mTextureID;
public:
	Renderable() { mVao = 0; mTextureID = 0; }
	virtual ~Renderable() { glDeleteTextures(1, &mTextureID); glDeleteVertexArrays(1, &mVao); }

	virtual bool init() { return true; }
	virtual void render() = 0;
};