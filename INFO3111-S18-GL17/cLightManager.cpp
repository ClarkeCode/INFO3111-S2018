#include "globalStuff.h"	// For the OpenGL calls
#include "cLightManager.h"
#include <sstream>

cLightManager::cLightManager()

{
	this->m_numberOfLights = 0;

	return;
}

cLightManager::~cLightManager()
{
	this->m_DeleteTheLights();

	return;
}

void cLightManager::GenerateLights(unsigned int numberOfLights, bool bDeleteOldLights /*=true*/)
{
	if ( ! this->vec_pTheLights.empty() )
	{
		if ( ! bDeleteOldLights )
		{
			return;
		}
	}//if ( ! this->vecTheLights.empty() )

	// Either there's no lights, or we're supposed to delete them, anyway
	this->m_DeleteTheLights();


	for ( unsigned int count = 0; count != numberOfLights; count++ )
	{
		cLight* pTempLight = new cLight();

		this->vec_pTheLights.push_back( pTempLight );
	}
	
	return;
}


//cLightManager::sUniformLocations::sUniformLocations()
//{
//	// Clear all the variables
//	memset( this, 0, sizeof(cLightManager::sUniformLocations) );
//	return;
//}

	
bool cLightManager::InitilizeUniformLocations( 
		unsigned int shaderID, std::string lightArrayName, 
		unsigned int numberOfLights /*size of array*/, 
	    std::string &errors )
{
	// All these temporary variables are here so that you can see them in debug.
	// 
	// The structure for the light in our shader looks like this:
	// 
	//	struct sLight
	//	{
	//		vec3 Position;
	//		vec3 Direction;
	//		vec4 Diffuse;		// If w value = 0.0, then light isn't 'on'
	//							// (the 4th value doesn't have a use for RGB colour)
	//		vec3 Ambient;
	//		vec4 Specular; 		// rgb = colour, w = intensity
	//	
	//		vec4 AttenAndType;	// x = constant, y = linear, z = quadratic, w = “type”
	//							// w = “type”: 0 = point, 1 = spot, 2 = directional
	//		vec4 LightAttribs;	// x = spot outer cone, y = spot inner cone
	//							// z, w = undefined
	//	};
	//	uniform sLight theLights[NUMLIGHTS];
	// 	
	
	bool bNoErrors = true;
	std::stringstream ssErrors;

	// Clear any old values
	this->vecLightUniforms.clear();

	// Set up space for the uniforms
	this->vecLightUniforms.reserve( numberOfLights ); 

	for ( unsigned int lightIndex = 0; lightIndex != numberOfLights; lightIndex++ )
	{
		std::stringstream ssLightPrefix;
		// Make the first part of the uniform, like
		//  'myLightArray[3].'
		// ...so the rest of the unform name can be added to the end
		ssLightPrefix << lightArrayName << "[" << lightIndex << "].";

		sUniformLocations curLightInShader;

		curLightInShader.Position.name		= ssLightPrefix.str() + "Position";
		curLightInShader.Direction.name		= ssLightPrefix.str() + "Direction";
		curLightInShader.Diffuse.name		= ssLightPrefix.str() + "Diffuse";
		//curLightInShader.Ambient.name		= ssLightPrefix.str() + "Ambient";
		//curLightInShader.Specular.name		= ssLightPrefix.str() + "Specular";
		curLightInShader.AttenAndLightType.name	= ssLightPrefix.str() + "AttenAndType";
		curLightInShader.LightAttribs.name	= ssLightPrefix.str() + "LightAttribs";

		// Now look up the uniform locations...
		if ( ! this->m_LoadUniformVariableHelper( shaderID, curLightInShader.Position ) )
		{
			ssErrors << "Didn't find " << curLightInShader.Position.name << std::endl;
			bNoErrors = false;
		}

		if ( ! this->m_LoadUniformVariableHelper( shaderID, curLightInShader.Direction ) )
		{
			ssErrors << "Didn't find " << curLightInShader.Direction.name << std::endl;
			bNoErrors = false;
		}

		if ( ! this->m_LoadUniformVariableHelper( shaderID, curLightInShader.Diffuse ) )
		{
			ssErrors << "Didn't find " << curLightInShader.Diffuse.name << std::endl;
			bNoErrors = false;
		}

//		if ( ! this->m_LoadUniformVariableHelper( shaderID, curLightInShader.Ambient ) )
//		{
//			ssErrors << "Didn't find " << curLightInShader.Ambient.name << std::endl;
//			bNoErrors = false;
//		}
//	
//		if ( ! this->m_LoadUniformVariableHelper( shaderID, curLightInShader.Specular ) )
//		{
//			ssErrors << "Didn't find " << curLightInShader.Specular.name << std::endl;
//			bNoErrors = false;
//		}

		if ( ! this->m_LoadUniformVariableHelper( shaderID, curLightInShader.AttenAndLightType ) )
		{
			ssErrors << "Didn't find " << curLightInShader.AttenAndLightType.name << std::endl;
			bNoErrors = false;
		}

		if ( ! this->m_LoadUniformVariableHelper( shaderID, curLightInShader.LightAttribs ) )
		{
			ssErrors << "Didn't find " << curLightInShader.LightAttribs.name << std::endl;
			bNoErrors = false;
		}

		this->vecLightUniforms.push_back( curLightInShader );

	}//for ( unsigned int lightIndex


	return bNoErrors;
}

bool cLightManager::m_LoadUniformVariableHelper( unsigned int shaderID, 
												 sUniformLocations::sNameLocationPair &nameLocPair )
{
	nameLocPair.location = glGetUniformLocation( shaderID, nameLocPair.name.c_str() );
	
	if ( nameLocPair.location == -1 )
	{	
		// Didn't find it
		return false;
	}

	// Found the uniform. We're good. 
	return true;
}

unsigned int cLightManager::GetNumberOfLights(void)
{
	return static_cast<unsigned int>( this->vec_pTheLights.size() );
}

void cLightManager::m_DeleteTheLights(void)
{
	for ( unsigned int index = 0; index != this->vec_pTheLights.size(); index++ )
	{
		cLight* pCurLight = this->vec_pTheLights[index];

		delete pCurLight;

		// Or just:
		// delete this->vec_pTheLights[index];
	}
	this->vec_pTheLights.clear();

	return;
}

// Don't keep this pointer around...
cLight* cLightManager::pGetLightAtIndex(unsigned int lightIndex)
{
	if ( lightIndex >= this->GetNumberOfLights() )
	{
		return NULL;
	}

	return this->vec_pTheLights[lightIndex];
}


void cLightManager::CopyLightInfoToShader(void)
{
	// Are the lights and uniform vectors the same size?
	if ( this->vecLightUniforms.size() != this->vec_pTheLights.size() )
	{	
		// Nope.
		return;
	}

	// Get the information from the lights, and pass them into the 
	//	uniform locations in the shader.
	// (IF the light is "on", that is, otherwise skip it)
	unsigned int numLights = this->GetNumberOfLights();

	for ( unsigned int lightIndex = 0; lightIndex != numLights; lightIndex++ )
	{

		cLight* pCurLight = this->vec_pTheLights[lightIndex];
		sUniformLocations& curUniDesc = this->vecLightUniforms[lightIndex];

		// Our shader light looks like this:
		//
		//	struct sLight
		//	{
		//		vec3 Position;
		//		vec3 Direction;
		//		vec4 Diffuse;		// If w value = 0.0, then light isn't 'on'
		//		                    // (the 4th value doesn't have a use for RGB colour)
		// REMOVED vec3 Ambient;
		// REMOVED vec4 Specular; 		// rgb = colour, w = intensity
		//		
		//		vec4 AttenAndType;	// x = constant, y = linear, z = quadratic, w = “type”
		//		                    // w = “type”: 0 = point, 1 = spot, 2 = directional
		//		vec4 LightAttribs;	// x = spot outer cone, y = spot inner cone
		//							// z, w = undefined
		//	};
		// 
		if ( pCurLight->IsOn() )
		{
			// vec3 Position;
			glUniform3f( curUniDesc.Position.location, 
						 pCurLight->position.x, pCurLight->position.y, pCurLight->position.z );

			//		vec3 Direction;
			glUniform3f( curUniDesc.Direction.location, 
						 pCurLight->direction.x, pCurLight->direction.y, pCurLight->direction.z );

			//		vec4 Diffuse;		// If w value = 0.0, then light isn't 'on'
			//		                    // (the 4th value doesn't have a use for RGB colour)
			glUniform4f( curUniDesc.Diffuse.location, 
						 pCurLight->diffuse.r, pCurLight->diffuse.g, pCurLight->diffuse.b, 
						 1.0f );	// w value is non zero if "on"

//			//		vec4 Ambient;
//			glUniform3f( curUniDesc.Ambient.location, 
//						 pCurLight->ambient.r, pCurLight->ambient.g, pCurLight->ambient.b );
//
//			//		vec4 Specular; 		// rgb = colour, w = intensity
//			glUniform4f( curUniDesc.Specular.location, 
//						 pCurLight->specular.r, pCurLight->specular.g, pCurLight->specular.b, 
//						 pCurLight->specularPower );	// Spec "power" or shininess
//			//		
//			//		vec4 AttenAndType;	// x = constant, y = linear, z = quadratic, w = “type”
//			//		                    // w = “type”: 0 = point, 1 = spot, 2 = directional

			float lightTypeFloat = 0.0f;		// 0 = point
			switch( pCurLight->getLightType() )
			{
			case cLight::SPOT_LIGHT:
				lightTypeFloat = 1.0f;			// 1 = spot
				break;
			case cLight::DIRECTIONAL_LIGHT:
				lightTypeFloat = 2.0f;			// 2 = directional
				break;
			};
			glUniform4f( curUniDesc.AttenAndLightType.location, 
						 pCurLight->attenConst, pCurLight->attenLinear, pCurLight->attenQuad, 
						 lightTypeFloat );

			//		vec4 LightAttribs;	// x = spot outer cone, y = spot inner cone
			//							// z, w = undefined
			glUniform4f( curUniDesc.LightAttribs.location, 
						 pCurLight->spotConeAngleOuter, pCurLight->spotConeAngleInner, 
						 0.0f,		// Not used
						 0.0f );	// Not used

		}
		else
		{
			// Light is "OFF"
			glUniform4f( curUniDesc.Diffuse.location, 
						 pCurLight->diffuse.r, pCurLight->diffuse.g, pCurLight->diffuse.b, 
						 0.0f );	// w value is zero (0.0) if "off"

		}//if ( pCurLight->IsOn() )



	}//for ( unsigned int lightIndex


	return;
}
