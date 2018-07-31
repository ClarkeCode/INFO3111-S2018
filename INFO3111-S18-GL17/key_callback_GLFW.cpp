#include "globalStuff.h"

// This one is connected to the regular "keyboard" handler in Winders.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
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


	}//if ( mods == GLFW_MOD_SHIFT )

	if ( ( mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT )
	{
		// Shift, and any other modifier, too.
		//DestroyAllHumans();
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
