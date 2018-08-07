#ifndef _cLightManager_HG_
#define _cLightManager_HG_

// This is a helper class that helps manage the light classes
//	used on the application (C++) side, with the way the 
//	shader needs the lights to be set up

#include "cLight.h"
#include <vector>
#include <string>

class cLightManager
{
public:
	cLightManager();
	~cLightManager();

	// This will load the uniforms for this light.
	// The 'lightArrayName' is the name of the array in the shader, so 
	//		uniform sLight theLights[NUMLIGHTS];
	//  would be passed 'theLights'.
	// If you change the names of the elements in the light struct
	//  in the shader, then you'd have to change this. 
	// Note: There's a number of functions in OpenGL that can query this kind
	//	of thing, or with a lot of lights, you would use something called 
	//	a Named Uniform Block, but this is beyond what we need to do for now. 
	// Returns true if all the uniforms were loaded (i.e. were NOT -1)
	// WARNING: This will ERASE the old values, so really should only be called once.
	bool InitilizeUniformLocations( unsigned int shaderID, std::string lightArrayName, 
									unsigned int numberOfLights /*size of array*/, 
	                                std::string &errors );

	void GenerateLights(unsigned int numberOfLights, bool bDeleteOldLights = true);

	// Likely zero (0), but you never know
	unsigned int GetNumberOfLights(void);


	void CopyLightInfoToShader(void);

	// Don't keep this pointer around...
	// Returns NULL (0) if invalid index
	cLight* pGetLightAtIndex(unsigned int lightIndex);


	struct sUniformLocations
	{
		struct sNameLocationPair
		{
			sNameLocationPair() : location(-1) {};
			std::string name;
			int location;
		};
		//sUniformLocations();

		// Each one of these has a name (variable name in the shader)
		//	and a location (ID), for that variable in that shader.
		// (Note: we are only using one shader)
		sNameLocationPair Position;
		sNameLocationPair Direction;
		sNameLocationPair Diffuse;
		// Ambient and Specular FOR THE LIGHT have been removed to simplify the lighting
		//sNameLocationPair Ambient;
		//sNameLocationPair Specular;
		sNameLocationPair AttenAndLightType;
		sNameLocationPair LightAttribs;
	};

	std::vector<cLight*> vec_pTheLights;
	std::vector<sUniformLocations> vecLightUniforms;

private:
	// Helper function used in InitilizeUniformLocations()
	bool m_LoadUniformVariableHelper( unsigned int shaderID, sUniformLocations::sNameLocationPair &nameLocPair );

	unsigned int m_numberOfLights;

	void m_DeleteTheLights(void);
};

#endif // cLightManager
