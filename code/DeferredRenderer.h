/** @file deferredRenderer.h
 *  @brief 
 *
 *	Effects in Games, June 2012
 */

struct DeferredLight
{
    glm::vec3 mPos;
    glm::vec3 mDiffuse;
    glm::vec4 mSpecular;    // intensity in the fourth component

    DeferredLight() : mPos(0.0f), mDiffuse(0.4f), mSpecular(0.6f, 0.6f, 0.6f, 64.0f) { }
    DeferredLight(float dr, float dg, float db, float sr, float sg, float sb, float si) : mPos(0.0f), mDiffuse(dr, dg, db), mSpecular(sr, sg, sb, si) { }
};

class DeferredRenderer
{
public:
    enum TexType { ttNormal, ttPos, ttColor };

protected:
    ShaderProgram mGeometryStageProg;
    ShaderProgram mFinalStageProg;
    GLuint mQuadVao;
	GLuint mQuadVbo;

    GLuint mTexPos;
    GLuint mTexDepth;
    GLuint mTexNormals;
    GLuint mTexColor;
    FrameBuffer mGeometryFbo;

    ShaderProgram mDisplayTexProg;
public:
	DeferredRenderer();
	~DeferredRenderer();

	bool init(); 
    void beginGeometryStage(glm::mat4 &projMat);
    void endGeometryStage();
    void finalStage(glm::mat4 &mvMat, const std::vector<DeferredLight> &lights);

    ShaderProgram *geometryStageProgram() { return &mGeometryStageProg; }

    void displayTex(TexType tt, const glm::vec4 &rect);
};