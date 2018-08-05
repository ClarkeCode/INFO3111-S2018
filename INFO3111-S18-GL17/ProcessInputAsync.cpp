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


	cLight* pSelectedLight = ::g_pLightManager->pGetLightAtIndex(::g_SelectedLightID);

	if ( glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS )
	{	// Decrease linear atten by 1%
		pSelectedLight->attenLinear *= 0.99f;
	}

	if ( glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS )
	{	// Increase linear atten by 1%
		if ( pSelectedLight->attenLinear <= 0.0f )	 
		{ 
			pSelectedLight->attenLinear = 0.001f;		// Make it a tiny value
		}
		else
		{
			pSelectedLight->attenLinear *= 1.01f;		// + 1%
			if ( pSelectedLight->attenLinear >= cLight::MAX_ATTENUATION )
			{
				pSelectedLight->attenLinear = cLight::MAX_ATTENUATION;		
			}
		}
	}
	if ( glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS )
	{	// Decrease quadratic atten by 1%
		pSelectedLight->attenQuad *= 0.99f;			// +1%
	}

	if ( glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS )
	{	// Increase quadratic atten by 1%
		if ( pSelectedLight->attenQuad <= 0.0f )	 
		{ 
			pSelectedLight->attenQuad = 0.0001f;		// Make it a tiny value
		}
		else
		{
			pSelectedLight->attenQuad *= 1.01f;		// + 1%
			if ( pSelectedLight->attenQuad >= cLight::MAX_ATTENUATION )
			{
				pSelectedLight->attenQuad = cLight::MAX_ATTENUATION;		
			}
		}
	}	

	// OUTER spot cone angle
	if ( glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS )		// Make spot BIGGER
	{	// 
		if ( pSelectedLight->spotConeAngleOuter < 90.0f )	{ pSelectedLight->spotConeAngleOuter += 0.1f; } 
	}	
	if ( glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS )		// Make spot SMALLER
	{	// 
		if ( pSelectedLight->spotConeAngleOuter >= 0.0f )	{ pSelectedLight->spotConeAngleOuter -= 0.1f; } 
		// If outer is LT inner, then adjust inner
		if ( pSelectedLight->spotConeAngleOuter < pSelectedLight->spotConeAngleInner )
		{
			pSelectedLight->spotConeAngleInner = pSelectedLight->spotConeAngleOuter;
		}
	}	
	// INNER spot cone angle
	if ( glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS )		// Make spot BIGGER
	{	// 
		if ( pSelectedLight->spotConeAngleInner < 90.0f )	{ pSelectedLight->spotConeAngleInner += 0.1f; } 
		// If inner is GT outer, then adjust outer
		if ( pSelectedLight->spotConeAngleInner > pSelectedLight->spotConeAngleOuter )
		{
			pSelectedLight->spotConeAngleOuter = pSelectedLight->spotConeAngleInner;
		}
	}	
	if ( glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS )		// Make spot SMALLER
	{	// 
		if ( pSelectedLight->spotConeAngleInner >= 0.0f )	{ pSelectedLight->spotConeAngleInner -= 0.1f; } 
	}	


	// Also move the light around
	if ( glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS )
	{
		pSelectedLight->position.z += cameraSpeed;		
	}
	if ( glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS )
	{
		pSelectedLight->position.z -= cameraSpeed;		
	}		
	if ( glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS )
	{
		pSelectedLight->position.x -= cameraSpeed;		
	}	
	if ( glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS )
	{
		pSelectedLight->position.x += cameraSpeed;		
	}		
	if ( glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS )
	{
		pSelectedLight->position.y += cameraSpeed;		
	}		
	if ( glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS )
	{
		pSelectedLight->position.y -= cameraSpeed;		
	}		

	// 
	//if ( glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS )
	//{
	//	std::cout << "Hey!@" << std::endl;
	//	::g_bTurnOnDebugLightSpheres = false;
	//}


	std::stringstream ssTitle;
	ssTitle << "Light[" << ::g_SelectedLightID << "]: xyz("
		<< pSelectedLight->position.x << ", "
		<< pSelectedLight->position.y << ", "
		<< pSelectedLight->position.z << ") "
		<< "Lin: " 
		<< pSelectedLight->attenLinear << " "
		<< "Quad: " 
		<< pSelectedLight->attenQuad;

	glfwSetWindowTitle( ::g_window, ssTitle.str().c_str() );

	//std::cout << "Camera (xyz): "  
	//		<<cameraEye.x << ", " 
	//		<< cameraEye.y << ", "
	//		<< cameraEye.z << std::endl;
	return;
}