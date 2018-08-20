#include "cMeshObject.h"

cMeshObject::cMeshObject()
{
	this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
//    this->colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->diffuseColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	this->ambientToDiffuseRatio = 0.2f;
	
	// Specular colour is often the colour of the lights
	// Specular power can go well beyond 1.0
	this->specularHighlightColour = glm::vec3( 1.0f, 1.0f, 1.0f );
	this->specularShininess = 1.0f;

    this->scale = 1.0f;

    this->isWireframe = false;
	this->bUseNonUniformScaling = false;
	this->nonUniformScale = glm::vec3(1.0f,1.0f,1.0f);

	this->orientation = glm::vec3(0.0f,0.0f,0.0f);

	this->m_AssignUniqueID();

	this->bDontLightObject = false;

	this->bIsVisible = true;

	this->colourSource = cMeshObject::USE_VERTEX_COLOURS;

	this->bUseColourAlphaValue = false;


	this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	this->acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	this->textureMixRatios[0] = 0.0f;
	this->textureMixRatios[1] = 0.0f;
	this->textureMixRatios[2] = 0.0f;
	this->textureMixRatios[3] = 0.0f;
	this->textureMixRatios[4] = 0.0f;
	this->textureMixRatios[5] = 0.0f;
	this->textureMixRatios[6] = 0.0f;
	this->textureMixRatios[7] = 0.0f;
	// ...and so on...

	this->bIsSkyBoxObject = false;


	this->bEnableVertexSourceMixing = false;
	this->fVCS_FromVertex_Mix = 0.0f;		// matches: uniform float VCS_FromVertex_Mix
	this->fVCS_FromMesh_Mix = 0.0f;			// matches: uniform float VCS_FromMesh_Mix
	this->fVCS_FromTexture_Mix = 1.0f;		// matches: VCS_FromTexture_Mix


	return;
}

cMeshObject::~cMeshObject()
{

	return;
}

//static 
unsigned int cMeshObject::m_nextID = 1000;

	//mutable unsigned int uniqueID;
	//const unsigned int uniqueID = 0;

void cMeshObject::m_AssignUniqueID(void) 
{
	unsigned int* pUniqeID = const_cast<unsigned int*>(&this->uniqueID);

	(*pUniqeID) = cMeshObject::m_nextID;

	cMeshObject::m_nextID++;
	return;
}
