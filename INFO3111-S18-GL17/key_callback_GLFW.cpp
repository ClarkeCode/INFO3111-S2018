#include "globalStuff.h"
#include <iostream> //for debugging

// This one is connected to the regular "keyboard" handler in Winders.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if ((key == GLFW_KEY_F1) && (action == GLFW_PRESS)) {
		::debug_CopyToClipboard(::debug_serializeCMeshObjectToString(::g_vec_pMeshObjects[::g_SelectedModelID]));
	}

	if ((key == GLFW_KEY_F8) && (action == GLFW_PRESS)) {
		//::g_myCamera.lookAtModelByFName("Cow1");
	}
	if ((key == GLFW_KEY_F9) && (action == GLFW_PRESS)) {
		//::g_myCamera.lookAtModelByFName("Cow1");
		::g_myCamera.cam_eye_position = glm::vec3(0.63f, 3.1f, 7.1f);
	}
	if ((key == GLFW_KEY_F10) && (action == GLFW_PRESS)) {
		::g_myCamera.lookAtModelByFName("Cow1");
	}
	if ((key == GLFW_KEY_F11) && (action == GLFW_PRESS)) {
		::g_myCamera.lookAtModelByFName("Moon");
		::g_myCamera.cam_target_position.y /= 3.0f;
	}

	// Is the shift key pressed at the same time?
	if ( mods == GLFW_MOD_SHIFT )	// ONLY shift and nothing else
	{
		if ( ( key == GLFW_KEY_L) && (action == GLFW_PRESS) )
		{
			::g_bTurnOnDebugLightSpheres =  ! ::g_bTurnOnDebugLightSpheres;
		}

		// Select the next light:
		if ( ( key == GLFW_KEY_9 ) && ( action == GLFW_PRESS ) )
		{
			::g_SelectedLightID--;
		}
		if ( ( key == GLFW_KEY_0 ) && ( action == GLFW_PRESS ) )
		{
			::g_SelectedLightID++;
		}
		//Check for wrap around...
		if ( ::g_SelectedLightID >= NUMLIGHTS )
		{
			::g_SelectedLightID = NUMLIGHTS - 1;
		}

		if ((key == GLFW_KEY_LEFT_BRACKET) && (action == GLFW_PRESS)) {
			::g_SelectedModelID = ::g_SelectedModelID == 0 ? ::g_vec_pMeshObjects.size() - 1 : ::g_SelectedModelID - 1;
		}
		if ((key == GLFW_KEY_RIGHT_BRACKET) && (action == GLFW_PRESS)) {
			::g_SelectedModelID = ::g_SelectedModelID >= g_vec_pMeshObjects.size() ? 0 : ::g_SelectedModelID + 1;
		}

	}//if ( mods == GLFW_MOD_SHIFT )


	// Shift, and any other modifier, too.
	if ( ( mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT ) {
		//DestroyAllHumans();

		if ((key == GLFW_KEY_GRAVE_ACCENT) && (action == GLFW_PRESS)) {
			::g_incrementSubMode();
			std::cout << ::g_editMode << " " << ::g_editSubMode << "\n";
		}
		
	}
	else {
		if ((key == GLFW_KEY_GRAVE_ACCENT) && (action == GLFW_PRESS)) {
			//::g_editMode = ::g_editMode == E_EDIT_LIGHT ? E_EDIT_CAMERA : static_cast<e_edit_mode>(static_cast<int>(::g_editMode) + 1);
			::g_incrementEditMode();
			std::cout << ::g_editMode << " " << ::g_editSubMode << "\n";
		}
	}

	// STARTOF: Turn the physics updater on or off
	if ( ( mods & GLFW_MOD_CONTROL ) == GLFW_MOD_CONTROL )
	{
		if ( ( key == GLFW_KEY_P ) && ( action == GLFW_PRESS ) )
		{
			::g_bDoEulerPhysicsUpdate = true;
		}
		else if ( ( key == GLFW_KEY_O ) && ( action == GLFW_PRESS ) )
		{
			::g_bDoEulerPhysicsUpdate = false;
		}
	}// ENDOF: Turn the physics updater on or off

	return;
}
