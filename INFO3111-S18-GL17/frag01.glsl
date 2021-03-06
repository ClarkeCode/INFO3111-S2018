#version 420
// FRAGMENT SHADER

// From the vertex shader:
in vec4 vertColourRGBA;			
in vec4 vertWorldPosXYZ;
in vec4 vertNormal;
in vec4 vertTexUV;	
in vec4 vertOriginalVertexColourRGBA;

uniform bool bDontLightObject;

// rgb of the colour HIGHLIGHT
// a (w, 4th value) is power or 'shininess' of object
uniform vec4 objectSpecularColour;

// Often, the ambient value is in both the object
//  and the light, but to simply things, we will
//  use a "global" ambient value. 
// This his how bright things are when outside of the light.
uniform float globalAmbientToDiffuseRatio;		

uniform bool bUse_vColourRGBA_AlphaValue;		

// Often the 4th value of the diffuse is being used, 
// but we'll just explicity set it here...
// Set this from 0 (transparent) to 1 (solid)
uniform float alphaTransparency;

// Same variable as in vertex shader
uniform vec4 meshColourRGBA; 		

// *************************************************************
// This is the OPTIONAL vertex colour source mixing code, 
// so that you can choose the vertex colour from:
// - the vertex (i.e. from the model file)
// - the meshColourRGBA uniform
// - the texture
// 
// Since they are 3 floats that 'mix' together, you 
// can combine all 3 sources, if you want. 
// (ensure that they sum to 1.0 or it'll look odd)
// 
// NOTE: Setting this bool uniform ENABLES this!
// (so if you IGNORE it, it behaves like it did before)
// ALSO NOTE: If enabled, you must set at least one of these
//	to 1.0, or you'll get a BLACK SCREEN (i.e. NO colour)
// 
uniform bool bEnableVertexSourceMixing;
// ("VCS" stort for "Vertex Colour Source")
uniform float VCS_FromVertex_Mix;
uniform float VCS_FromMesh_Mix;
uniform float VCS_FromTexture_Mix;
// *************************************************************

const int NUMLIGHTS = 10;


uniform sampler2D texture00;		// Grass
uniform sampler2D texture01;		// Jellybean
uniform sampler2D texture02;
uniform sampler2D texture03;		// Brenda R.
uniform sampler2D texture04;		// Jellybean
uniform sampler2D texture05;		// Grass
uniform sampler2D texture06;
uniform sampler2D texture07;		// Brenda R.

// This is used for a skybox.
// A 'cube map' is a set of 6 2D images, one for each
//	side of a virtual cube. Unlike a 2D image, you need
//	3 coordinates to look up the image. Often, you use
//	the normal on the surface of the object as the lookup
uniform samplerCube texCubeSkyboxTexture;
uniform bool bSampleFromSkyboxTexture;


uniform float textureMix00;	// grass
uniform float textureMix01;
uniform float textureMix02;
uniform float textureMix03;	// brenda
uniform float textureMix04;
uniform float textureMix05;
uniform float textureMix06;
uniform float textureMix07;


struct sLight
{
	vec3 Position;
	vec3 Direction;
	vec4 Diffuse;		// If w value = 0.0, then light isn't 'on'
	                    // (the 4th value doesn't have a use for RGB colour)
	// To simplify the lighting, we'll limit the specular and ambient 
	//  to the object, rather than the light+object interaction
	//vec3 Ambient;
	//vec4 Specular; 		// rgb = colour, w = intensity
	
	vec4 AttenAndType;	// x = constant, y = linear, z = quadratic, w = “type”
	                    // w = “type”: 0 = point, 1 = spot, 2 = directional
	vec4 LightAttribs;	// x = spot outer cone, y = spot inner cone
						// z = distance cut off (not currently used)
						// w = undefined
};
uniform sLight theLights[NUMLIGHTS];
//uniform int numberOfLightsImReallyUsing;


uniform vec3 eyeLocation;		// Camera location IN WORLD

// Going to the frame buffer (back buffer)
// (aka "the screen")  RGBA
out vec4 outputColour;

vec3 CalculateDirectionalLightContrib( uint lightIndex, vec3 vertWorldNormal );
float CalculateAttenuation( uint lightIndex, vec3 vertexWorldPosition );

void CalculateDiffuseAndSpecularContrib( uint lightIndex, 
                                         vec3 vertexWorldPosition, vec3 vertexNormal, 
                                         vec3 eyeLocation, 
                                         vec3 objectSpecularColour, float objectSpecularPower,
                                         out vec3 diffuseContrib, out vec3 specularContrib );

void main()
{
	// Skybox? This doesn't have lighting, and we only sample cube map, so we return immediately
	if ( bSampleFromSkyboxTexture )
	{
		// A Cube Map needs 3 coords to lookup, so you don't use UV(or ST).
		// Often, like this case, you use the vertex normal.
		outputColour.rgb = texture(texCubeSkyboxTexture, vertNormal.xyz).rgb;
		
		// Alternatively, you could also add some sort of attenuation
		//	to the skybox, to simulate "getting dark".
		outputColour.a = 1.0f;
		//outputColour.rgb = vec3(1.0f, 0.0, 0.0f);
		
		return;
	}
	
	// Assume object is black
	outputColour.rgba = vec4(0.0f, 0.0f, 0.0f, alphaTransparency);	// Start with black 
	

	// ...unless we are using the 'per vertex' alpha values from the ply model
	if ( bUse_vColourRGBA_AlphaValue )
	{
		outputColour.a = vertColourRGBA.a;			// Set transparency ('alpha transparency')
	}

	
	
	// Calculate the texture colour of the object
	// "Sample" the colour of the texture at these texture coordinates
	//uniform sampler2D texBrick;
	vec3 texture00_SampleRGB = texture(texture00, vertTexUV.st).rgb;
	vec3 texture01_SampleRGB = texture(texture01, vertTexUV.st).rgb;
	vec3 texture02_SampleRGB = texture(texture02, vertTexUV.st).rgb;
	vec3 texture03_SampleRGB = texture(texture03, vertTexUV.st).rgb;
	vec3 texture04_SampleRGB = texture(texture04, vertTexUV.st).rgb;
	vec3 texture05_SampleRGB = texture(texture05, vertTexUV.st).rgb;
	vec3 texture06_SampleRGB = texture(texture06, vertTexUV.st).rgb;
	vec3 texture07_SampleRGB = texture(texture07, vertTexUV.st).rgb;


	// Why is he doing this?
	vec3 textureSampleRGB = 
			( textureMix00 * texture00_SampleRGB )		// 0
		  + ( textureMix01 * texture01_SampleRGB )		// 0 
		  + ( textureMix02 * texture02_SampleRGB )		// 0 
		  + ( textureMix03 * texture03_SampleRGB )		// 0
		  + ( textureMix04 * texture04_SampleRGB )		// 0 
		  + ( textureMix05 * texture05_SampleRGB )		// 1   chooses this texture
		  + ( textureMix06 * texture06_SampleRGB )		// 0 
		  + ( textureMix07 * texture07_SampleRGB );		// 0
	// **********************************************************************
		  
	
	// Assume colour is coming from the texture...
	vec3 meshMaterialDiffuse = textureSampleRGB.rgb;
	
	/// ALTERNATIVE mixing of vertex colours
	// (if this is FALSE, by default, then this isn't being used)
	if ( bEnableVertexSourceMixing == true )
	{
		meshMaterialDiffuse.rgb = ( VCS_FromVertex_Mix * vertOriginalVertexColourRGBA.rgb ) 
		                        + ( VCS_FromMesh_Mix * meshColourRGBA.rgb )
		                        + ( VCS_FromTexture_Mix * textureSampleRGB.rgb );
		

		// Clamp in case we go above 1.0
		meshMaterialDiffuse = clamp( meshMaterialDiffuse.rgb, vec3(0.0f), vec3(1.0) );
		
	}//if ( bEnableVertexSourceMixing )
	
	
	
	if ( bDontLightObject )  
	{
		if ( bEnableVertexSourceMixing == true )
		{
			outputColour.rgb = meshMaterialDiffuse.rgb;
		}
		else 
		{	
			outputColour.rgb = meshColourRGBA.rgb;
		}
		// early exit
		return;	
	}//if ( bDontLightObject )  
	

	
	
	for ( int lightIndex = 0; lightIndex != NUMLIGHTS; lightIndex++ )
	{
		// Is this light 'in use'?
		// (is Diffuse.w != 0.0?)
		if ( theLights[lightIndex].Diffuse.w == 0.0f )
		{
			// Skip this light
			continue;
		}
	
		// Directional light? 
		// If so, we do this BEFORE calculating any attenuation (because it's not needed for directional)
		if ( theLights[lightIndex].AttenAndType.w == 2.0f )
		{
			vec3 lightContrib = CalculateDirectionalLightContrib( lightIndex, normalize(vertNormal.xyz) );
			outputColour.rgb += ( meshMaterialDiffuse.rgb * lightContrib );												 
			continue;
		}//if ( theLights[lightIndex]. )
	
		// The light ISN'T a directional light, 
		//  so continue with the more involved calculations
		
		// Note that we pass the object specular colour since this is often 
		//  different from the vertex (diffuse) colour. 		
		vec3 lightDiffuseContrib = vec3(0.0f);
		vec3 lightSpecularContrib = vec3(0.0f);
		
		CalculateDiffuseAndSpecularContrib( lightIndex, 
		                                    vertWorldPosXYZ.xyz, vertNormal.xyz, 
		                                    eyeLocation.xyz, 
		                                    objectSpecularColour.rgb, objectSpecularColour.w,
		                                    lightDiffuseContrib, lightSpecularContrib );

		
		// Point, Spot, or directional
		switch( int(theLights[lightIndex].AttenAndType.w) )
		{
			case 0:	//  = point;	
				break;
			case 1: // = spot, 
				// See if the direction of the light is 
				// 	outside of the cone of the light.
				// Calculate the line between the light and the vertex
				// Which is the "lightVector" we calculated earlier
				vec3 vertexToLight = vertWorldPosXYZ.xyz - theLights[lightIndex].Position.xyz;
				
				vertexToLight = normalize(vertexToLight);
				
				// Calculate the line between the "direction" and light.
				// (but that's already the direction, because it's relative
				//	to the light)
		
				float currentLightRayAngle
						= dot( vertexToLight.xyz, theLights[lightIndex].Direction.xyz );
				currentLightRayAngle = max(0.0f, currentLightRayAngle);
				
				// Is this inside the cone? 
				float outerConeAngleCos = cos(radians(theLights[lightIndex].LightAttribs.x));
				float innerConeAngleCos = cos(radians(theLights[lightIndex].LightAttribs.y));
								
				// Is it completely outside of the spot?
				if ( currentLightRayAngle < outerConeAngleCos )
				{
					// Nope. so it's in the dark
					lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
					lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
				}
				else if ( currentLightRayAngle < innerConeAngleCos )
				{
					// Angle is between the inner and outer cone
					// (this is called the penumbra of the spot light, by the way)
					// 
					// This blends the brightness from full brightness, near the inner cone
					//	to black, near the outter cone
					float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
					                      (innerConeAngleCos - outerConeAngleCos);
										  
					lightDiffuseContrib *= penumbraRatio;
					lightSpecularContrib *= penumbraRatio;
				}
				// else it's 'in' the spotlight, so don't change
				// reduce the amount of light hitting the object
			
				break;
			case 2:	//  = directional  
				// This is supposed to simulate sunlight. 
				// SO: 
				// -- There's ONLY direction, no position
				// -- Almost always, there's only 1 of these in a scene
				// Cheapest light to calculate. 
				
				// This is happening at the START of the shader to 
				//	avoid doing expensive calculations for nothing

				break;
				
		}// select (AttenAndType.w)
		
		// To simplify, we aren't using the light ambient value, just the object
		// Sample the texture to get the colour:

		
//		outputColour.rgb += (vertColourRGBA.rgb * lightDiffuseContrib.rgb)
//		                     + lightSpecularContrib.rgb;
		
		// textureSampleRGB is sampled BEFORE the light for loop
		outputColour.rgb += (meshMaterialDiffuse * lightDiffuseContrib.rgb)
		                     + lightSpecularContrib.rgb;
									
	}// for ( int index = 0
	
	
	// Calculate the global ambient (i.e. no matter how many lights, this will be the same)
	// (So we are adding the ambient 'light' to the object, simulating the light
	//  that's just 'around' the scene, but not being directly lit.)
	vec3 ambientObjectColour = ( globalAmbientToDiffuseRatio 
	                             * meshMaterialDiffuse );
	                               
	// Another simplification: if the colour is darker than 
	//  the ambient, pick the ambient colour. 
	// (or pick the brighter colour, either the ambient (no light) or the lit surface)
	outputColour.rgb = max(ambientObjectColour, outputColour.rgb);
	
	outputColour.rgb = clamp( outputColour.rgb, vec3(0.0f,0.0f,0.0f), vec3(1.0f,1.0f,1.0f) );

	outputColour.a = alphaTransparency;
	
	
	
	// Bump colour output as projector is dark
	//outputColour.rgb *= 1.2f;
	
};

// Caulate the specular contribution from the light
// Returns: 
// - Diffuse contribution (from LIGHT colour only)
// - Specular contribution (from light AND object spec colour)
//   (because specular highlight colour could be different)
void CalculateDiffuseAndSpecularContrib( uint lightIndex, 
                                         vec3 vertexWorldPosition, vec3 vertexNormal, 
                                         vec3 eyeLocation, 
                                         vec3 objectSpecularColour, float objectSpecularPower,
                                         out vec3 diffuseContrib, out vec3 specularContrib )
{
	//    ___  _  __  __                           _       _ _    
	//   |   \(_)/ _|/ _|_  _ ___ ___   __ ___ _ _| |_ _ _(_) |__ 
	//   | |) | |  _|  _| || (_-</ -_) / _/ _ \ ' \  _| '_| | '_ \
	//   |___/|_|_| |_|  \_,_/__/\___| \__\___/_||_\__|_| |_|_.__/
	//                                                            
	
	diffuseContrib = theLights[lightIndex].Diffuse.rgb;
	
	vec3 lightVector = theLights[lightIndex].Position - vertexWorldPosition.xyz;
	// 'normalize' means a vector of unit (1.0) length
	// (to make the math not screw up)
	lightVector = normalize(lightVector);
	
	// Get the dot product of the light and normalize
	float dotProduct = dot( lightVector, normalize(vertexNormal) );	// -1 to 1
	
	dotProduct = max( 0.0f, dotProduct );		// 0 to 1
	
	diffuseContrib.rgb *= dotProduct;
	
	float attenuation = CalculateAttenuation( lightIndex, vertexWorldPosition.xyz );

// Optional clamp of attenuation, so the light won't get 'too' dark			
//		attenuation = clamp( attenuation, 0.0f, 1000.0f );
		
	diffuseContrib *= attenuation;
	

	//    ___                   _                       _       _ _    
	//   / __|_ __  ___ __ _  _| |__ _ _ _   __ ___ _ _| |_ _ _(_) |__ 
	//   \__ \ '_ \/ -_) _| || | / _` | '_| / _/ _ \ ' \  _| '_| | '_ \
	//   |___/ .__/\___\__|\_,_|_\__,_|_|   \__\___/_||_\__|_| |_|_.__/
	//       |_|                                                       
		
	specularContrib = vec3(0.0f);
		
	vec3 reflectVector = reflect( -lightVector, normalize(vertexNormal) );

	// Get eye or view vector
	// The location of the vertex in the world to your eye
	vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPosition.xyz);
	
	
	// To simplify, we are NOT using the light specular value, just the objects.
	specularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), 
									 objectSpecularPower )
									 * objectSpecularColour.rgb;	//* theLights[lightIndex].Specular.rgb
						   

	specularContrib *= attenuation;

	return;
}


float CalculateAttenuation( uint lightIndex, vec3 vertexWorldPosition )
{
	// Calculate the distance between the light 
	// and the vertex that this fragment is using
	float lightDistance = distance( vertWorldPosXYZ.xyz, theLights[lightIndex].Position );
	lightDistance = abs(lightDistance);

	float attenuation = 1.0f / 
		( theLights[lightIndex].AttenAndType.x 						// 0  
		+ theLights[lightIndex].AttenAndType.y * lightDistance					// Linear
		+ theLights[lightIndex].AttenAndType.z * lightDistance * lightDistance );	// Quad
	
	return attenuation;
}


// Returns directinoal light contribution
vec3 CalculateDirectionalLightContrib( uint lightIndex, vec3 vertexWorldNormal )
{
	// This is supposed to simulate sunlight. 
	// SO: 
	// -- There's ONLY direction, no position
	// -- Almost always, there's only 1 of these in a scene
	// Cheapest light to calculate. 

	vec3 lightContrib = theLights[lightIndex].Diffuse.rgb;
	
	// Get the dot product of the light and normalize
	float dotProduct = dot( -theLights[lightIndex].Direction.xyz, normalize(vertexWorldNormal.xyz) );	// -1 to 1

	dotProduct = max( 0.0f, dotProduct );		// 0 to 1
	
	lightContrib *= dotProduct;		
	
	// NOTE: There isn't any attenuation, like with sunlight.
	// (This is part of the reason directional lights are fast to calculate)

	// TODO: Still need to do specular, but this gives you an idea

	return lightContrib;
}
