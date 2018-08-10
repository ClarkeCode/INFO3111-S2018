#include "globalStuff.h"
#include <sstream> //for debug_ functions

std::vector< cMeshObject* > g_vec_pMeshObjects;

//cMeshObject* g_pTheLightMesh = 0;		// or NULL
// Will show the attenuation of the lights
//  1% brightness
// 25% brightness
// 50% brightness
// 90% brightness
//cMeshObject* g_pTheLightAttenMesh[4] = {0};		// or NULL
cMeshObject* g_pDebugSphere = NULL; 
cMeshObject* g_pDebugSphereSmall = NULL;	// Same size, but fewer triangles
cMeshObject* g_pDebugCone = NULL;

// If true, the spheres around the lights are there
bool g_bTurnOnDebugLightSpheres = true;
// Use can do this to "select" different lights
unsigned int g_SelectedLightID = 0;


cMeshObject* g_pFindObjectByFriendlyName(std::string name)
{
	unsigned int numObject = (unsigned int) ::g_vec_pMeshObjects.size();
	for ( unsigned int index = 0; index != numObject; index++ )
	{
		// Is this zee one? 
		if ( ::g_vec_pMeshObjects[index]->friendlyName == name )
		{ 
			// Yup
			return ::g_vec_pMeshObjects[index];
		}
	}//for ( unsigned int index = 0

	return NULL;
}

//const unsigned int NUMLIGHTS = 10;
//std::vector<sLight> g_vecLights;

cLightManager* g_pLightManager = NULL;


// When true, the DoPhysicsUpdate is called.
bool g_bDoEulerPhysicsUpdate = false;	

float g_globalAmbientToDiffuseRatio = 0.2f;	//0.2f;









/*
______      _               _     _____           _
| ___ \    | |             | |   /  __ \         | |
| |_/ /___ | |__   ___ _ __| |_  | /  \/_   _ ___| |_ ___  _ __ ___
|    // _ \| '_ \ / _ \ '__| __| | |   | | | / __| __/ _ \| '_ ` _ \
| |\ \ (_) | |_) |  __/ |  | |_  | \__/\ |_| \__ \ || (_) | | | | | |
\_| \_\___/|_.__/ \___|_|   \__|  \____/\__,_|___/\__\___/|_| |_| |_|

Here lies my global-level changes
*/

e_edit_mode		g_editMode = E_EDIT_CAMERA;
e_edit_sub_mode g_editSubMode = E_CAMERA_RELATIVE;

void g_setSubModeToDefault() {
	switch (g_editMode) {
	case E_EDIT_CAMERA: g_editSubMode = E_CAMERA_RELATIVE; break;
	case E_EDIT_MODEL:  g_editSubMode = E_NO_SUB_MODE; break;
	case E_EDIT_LIGHT:	g_editSubMode = E_LIGHT_DIRECTION; break;
	}
}

void g_incrementEditMode() {
	static std::vector<e_edit_mode> topEditModes { E_EDIT_CAMERA, E_EDIT_MODEL, E_EDIT_LIGHT };
	if (g_editMode == topEditModes.back()) {
		g_editMode = topEditModes.front();
	}
	else {
		g_editMode = *++std::find(topEditModes.begin(), topEditModes.end(), g_editMode);
	}
	g_setSubModeToDefault();
};

void g_incrementSubMode() {
	static std::vector<e_edit_sub_mode> cameraModes	{ E_CAMERA_RELATIVE, E_CAMERA_FIXED_TARGET, E_CAMERA_FIXED_EYE };
	static std::vector<e_edit_sub_mode> modelModes	{ E_NO_SUB_MODE };
	static std::vector<e_edit_sub_mode> lightModes	{ E_LIGHT_POSITION, E_LIGHT_DIRECTION };
	
	std::vector<e_edit_sub_mode> * selectedList = nullptr;
	switch (g_editMode) {
	case E_EDIT_CAMERA:	selectedList = &cameraModes;	break;
	case E_EDIT_MODEL:	selectedList = &modelModes;		break;
	case E_EDIT_LIGHT:	selectedList = &lightModes;		break;
	}
	
	//If the current sub mode selected is at the last item of the selected list, move to the front
	if (std::find(selectedList->begin(), selectedList->end(), g_editSubMode) == (selectedList->end() - 1)) {
		g_editSubMode = selectedList->front();
	}
	else {
		g_editSubMode = *++std::find(selectedList->begin(), selectedList->end(), g_editSubMode);
	}
};



myCamera g_myCamera = { 0.04f, 10.0f ,
						glm::vec3(-10.0f, 3.2f, -9.05f),
						glm::vec3(0.0f, 0.0f, 0.0f), false, false };


void debug_CopyToClipboard(std::string s) {
	glfwSetClipboardString(::g_window, s.c_str());
}

std::string debug_vec3ToString(glm::vec3 v) {
	std::stringstream ss;
	ss << "( " << v.x << ", " << v.y << ", " << v.z << " )";
	return ss.str();
}

std::string debug_vec4ToString(glm::vec4 v) {
	std::stringstream ss;
	ss << "( " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " )";
	return ss.str();
}