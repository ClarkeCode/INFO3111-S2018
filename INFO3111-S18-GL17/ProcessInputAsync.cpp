#include "globalStuff.h"

#include <sstream>

void ProcessInputAsync( glm::vec3 &cameraEye, glm::vec3 &cameraTarget, GLFWwindow* &window )
{
	float cameraSpeed = 0.05f; 

	int state = glfwGetKey(window, GLFW_KEY_D);
	if (state == GLFW_PRESS) { cameraEye.x += cameraSpeed; }

	state = glfwGetKey(window, GLFW_KEY_A);
	if (state == GLFW_PRESS) { cameraEye.x -= cameraSpeed; }

	state = glfwGetKey(window, GLFW_KEY_W);
	if (state == GLFW_PRESS) { cameraEye.z += cameraSpeed; }

	state = glfwGetKey(window, GLFW_KEY_S);
	if (state == GLFW_PRESS) { cameraEye.z -= cameraSpeed; }

	state = glfwGetKey(window, GLFW_KEY_Q);	// Up
	if (state == GLFW_PRESS) { cameraEye.y += cameraSpeed; }

	state = glfwGetKey(window, GLFW_KEY_E);	// Down
	if (state == GLFW_PRESS) { cameraEye.y -= cameraSpeed; }



	if ( glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS )
	{	// Decrease linear atten by 1%
		::g_vecLights[0].attenLinear *= 0.99f;
	}

	if ( glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS )
	{	// Increase linear atten by 1%
		if ( ::g_vecLights[::g_SelectedLightID].attenLinear <= 0.0f )	 
		{ 
			::g_vecLights[::g_SelectedLightID].attenLinear = 0.001f;		// Make it a tiny value
		}
		else
		{
			::g_vecLights[::g_SelectedLightID].attenLinear *= 1.01f;		// + 1%
			if ( ::g_vecLights[::g_SelectedLightID].attenLinear >= sLight::MAX_ATTENUATION )
			{
				::g_vecLights[::g_SelectedLightID].attenLinear = sLight::MAX_ATTENUATION;		
			}
		}
	}
	if ( glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS )
	{	// Decrease quadratic atten by 1%
		::g_vecLights[::g_SelectedLightID].attenQuad *= 0.99f;			// +1%
	}

	if ( glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS )
	{	// Increase quadratic atten by 1%
		if ( ::g_vecLights[::g_SelectedLightID].attenQuad <= 0.0f )	 
		{ 
			::g_vecLights[::g_SelectedLightID].attenQuad = 0.0001f;		// Make it a tiny value
		}
		else
		{
			::g_vecLights[::g_SelectedLightID].attenQuad *= 1.01f;		// + 1%
			if ( ::g_vecLights[::g_SelectedLightID].attenQuad >= sLight::MAX_ATTENUATION )
			{
				::g_vecLights[::g_SelectedLightID].attenQuad = sLight::MAX_ATTENUATION;		
			}
		}
	}	

	// Also move the light around
	if ( glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS )
	{
		::g_vecLights[::g_SelectedLightID].position.z += cameraSpeed;		
	}
	if ( glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS )
	{
		::g_vecLights[::g_SelectedLightID].position.z -= cameraSpeed;		
	}		
	if ( glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS )
	{
		::g_vecLights[::g_SelectedLightID].position.x -= cameraSpeed;		
	}	
	if ( glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS )
	{
		::g_vecLights[::g_SelectedLightID].position.x += cameraSpeed;		
	}		
	if ( glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS )
	{
		::g_vecLights[::g_SelectedLightID].position.y += cameraSpeed;		
	}		
	if ( glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS )
	{
		::g_vecLights[::g_SelectedLightID].position.y -= cameraSpeed;		
	}		

	// 
	//if ( glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS )
	//{
	//	std::cout << "Hey!@" << std::endl;
	//	::g_bTurnOnDebugLightSpheres = false;
	//}


	std::stringstream ssTitle;
	ssTitle << "Light[" << ::g_SelectedLightID << "]: xyz("
		<< ::g_vecLights[::g_SelectedLightID].position.x << ", "
		<< ::g_vecLights[::g_SelectedLightID].position.y << ", "
		<< ::g_vecLights[::g_SelectedLightID].position.z << ") "
		<< "Lin: " 
		<< ::g_vecLights[::g_SelectedLightID].attenLinear << " "
		<< "Quad: " 
		<< ::g_vecLights[::g_SelectedLightID].attenQuad;

	glfwSetWindowTitle( ::g_window, ssTitle.str().c_str() );

	//std::cout << "Camera (xyz): "  
	//		<<cameraEye.x << ", " 
	//		<< cameraEye.y << ", "
	//		<< cameraEye.z << std::endl;
	return;
}