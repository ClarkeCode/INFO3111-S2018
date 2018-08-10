#ifndef _globalStuff_HG_
#define _globalStuff_HG_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include "cMeshObject.h"

#include "cLightManager.h"

#include "CGLColourHelper.h"

#include "cShaderManager.h"

#include "cVAOManager.h"

// Put common stuff in the header 
// ALWAYS use header variables as extern
// NEVER "regular" variables


extern GLFWwindow* g_window;// = 0;

extern std::vector< cMeshObject* > g_vec_pMeshObjects;


// And common functions that are called.
// (signatures here)
void LoadObjectsIntoScene(void);
bool LoadModelTypes(GLint shadProgID, std::string &errors);
bool LoadModelTypes(GLint shadProgID, std::vector<std::string> vecModelNames, std::string &errors);
bool LoadModelTypes_PlyLoader(GLint shadProgID, std::vector<std::string> vecModelNames, std::string &errors);

// If true, the spheres around the lights are there
extern bool g_bTurnOnDebugLightSpheres;	// = false
// Use can do this to "select" different lights
extern unsigned int g_SelectedLightID;	// = 0

// Returns NULL if not found
cMeshObject* g_pFindObjectByFriendlyName(std::string name);
cMeshObject* g_pFindObjectByUniqueID(unsigned int uniqueID);

void DoPhysicsIntegrationUpdate(double deltaTime);
void ProcessInputAsync( glm::vec3 &cameraEye, glm::vec3 &cameraTarget, GLFWwindow* &window );

//static was removed because a 'static function' is only visible in that obj file (not outside)
// Once we moved the callback to another cpp file, we had to remove the static.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

const unsigned int NUMLIGHTS = 10;
//extern std::vector<cLight> g_vecLights;

extern cLightManager* g_pLightManager;// = NULL;

// When true, the DoPhysicsUpdate is called.
extern bool g_bDoEulerPhysicsUpdate;// = false;		

void DrawObject( cMeshObject* pCurMesh, 
				 cShaderManager::cShaderProgram* pShaderProg,	// To get at uniforms
				 cVAOManager* pVAOManager,
				 glm::mat4 matParentModel);

// A more general draw sphere
// These are no longer being used...
//extern cMeshObject* g_pTheLightMesh;		// or NULL
//extern cMeshObject* g_pTheLightAttenMesh[4];		// or NULL

extern cMeshObject* g_pDebugSphere; 
extern cMeshObject* g_pDebugSphereSmall;	// Same size, but fewer triangles
extern cMeshObject* g_pDebugCone;


// Used mainly for debug objects, so we can pass explicitly pass the model matrix
void DrawObject_ExplicitModelMatrix( cMeshObject* pCurMesh, 
                                     cShaderManager::cShaderProgram* pShaderProg,	// To get at uniforms
                                     cVAOManager* pVAOManager,
                                     glm::mat4 matModelMatrix );

void DrawDebugSphere( cShaderManager::cShaderProgram* pShaderProgram, 
					  glm::vec3 position, glm::vec4 colour, float scale );


// A simplification to the lighting:
// This is the 'global ambient' value, as a ratio of full brightness.
// (i.e. how 'dark' the non-lit areas of the scene are.)
// Often, the ambient value is 20-30% of the lit scene, for indoors. 
// Set this to 0.0 for no ambient light.
// Set to 1.0 if you want it to look you're on the surface of the sun (everthing lit everywhere).
extern float g_globalAmbientToDiffuseRatio;	// = 0.2f;



#endif
