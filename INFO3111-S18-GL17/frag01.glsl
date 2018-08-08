#version 420
// FRAGMENT SHADER

// From the vertex shader:
in vec4 vertColourRGBA;
in vec4 vertWorldPosXYZ;
in vec4 vertNormal;
in vec4 vertTexUV;	

uniform bool bDontLightObject;

// rgb of the colour HIGHLIGHT
// a (w, 4th value) is power or 'shininess' of object
uniform vec4 objectSpecularColour;

// Often, the ambient value is in both the object
//  and the light, but to simply things, we will
//  use a "global" ambient value. 
// This his how bright things are when outside of the light.
uniform float globalAmbientToDiffuseRatio;		

const int NUMLIGHTS = 10;


//uniform sampler2D texCuteBunnyTexure;
//uniform sampler2D texBrick;
//uniform sampler2D texObama;
//uniform samplerCube mySexySkyBox;


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
	
	if ( bDontLightObject )
	{
		outputColour = vertColourRGBA;
		// early exit
		return;	
	}

	// Assume object is black
	outputColour.rgb = vec3(0.0f, 0.0f, 0.0f);	// Start with black 
	
	outputColour.a = vertColourRGBA.a;			// Set transparency ('alpha transparency')
	
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
			outputColour.rgb += ( vertColourRGBA.rgb * lightContrib );												 
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

		
		outputColour.rgb += (vertColourRGBA.rgb * lightDiffuseContrib.rgb)
		                     + lightSpecularContrib.rgb;
									
	}// for ( int index = 0
	
	
	// Calculate the global ambient (i.e. no matter how many lights, this will be the same)
	// (So we are adding the ambient 'light' to the object, simulating the light
	//  that's just 'around' the scene, but not being directly lit.)
	vec3 ambientObjectColour = (globalAmbientToDiffuseRatio * vertColourRGBA.rgb);
	                               
	// Another simplification: if the colour is darker than 
	//  the ambient, pick the ambient colour. 
	// (or pick the brighter colour, either the ambient (no light) or the lit surface)
	outputColour.rgb = max(ambientObjectColour, outputColour.rgb);
	
	outputColour.rgb = clamp( outputColour.rgb, vec3(0.0f,0.0f,0.0f), vec3(1.0f,1.0f,1.0f) );
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
