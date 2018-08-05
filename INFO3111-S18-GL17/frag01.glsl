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
	
	for ( int index = 0; index != NUMLIGHTS; index++ )
	{
		// Is this light 'in use'?
		// (is Diffuse.w != 0.0?)
		if ( theLights[index].Diffuse.w == 0.0f )
		{
			// Skip this light
			continue;
		}
	
		// Directional light? 
		if ( theLights[index].AttenAndType.w == 2.0f )
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			// Get the dot product of the light and normalize
			float dotProduct = dot( -theLights[index].Direction, normalize(vertNormal.xyz) );	// -1 to 1

			dotProduct = max( 0.0f, dotProduct );		// 0 to 1

			vec3 lightContrib = theLights[index].Diffuse.rgb;

			lightContrib *= dotProduct;		

			// Still need to do specular, but this gives you an idea
			outputColour.rgb += ( vertColourRGBA.rgb * lightContrib );
			return;
		}//if ( theLights[index]. )
	
	
		// The Diffuse component (aka how much light is reflecting off the surface?)
		
		vec3 lightVector = theLights[index].Position - vertWorldPosXYZ.xyz;
		// 'normalize' means a vector of unit (1.0) length
		// (to make the math not screw up)
		lightVector = normalize(lightVector);
		
		// Get the dot product of the light and normalize
		float dotProduct = dot( lightVector, normalize(vertNormal.xyz) );	// -1 to 1
		
		dotProduct = max( 0.0f, dotProduct );		// 0 to 1
		
		vec3 lightContrib = theLights[index].Diffuse.rgb;
	
		lightContrib *= dotProduct;

		// Calculate the distance between the light 
		// and the vertex that this fragment is using
		float lightDistance = distance( vertWorldPosXYZ.xyz, theLights[index].Position );
		lightDistance = abs(lightDistance);

		float attenuation = 1.0f / 
			( theLights[index].AttenAndType.x 						// 0  
			+ theLights[index].AttenAndType.y * lightDistance					// Linear
    		+ theLights[index].AttenAndType.z * lightDistance * lightDistance );	// Quad

// Optional clamp of attenuation, so the light won't get 'too' dark			
//		attenuation = clamp( attenuation, 0.0f, 1000.0f );


		
		lightContrib *= attenuation;
		
		
		// Specular component
		vec3 reflectVector = reflect( -lightVector, normalize(vertNormal.xyz) );

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertWorldPosXYZ.xyz);
		
		vec3 specularColour = objectSpecularColour.rgb;
		float specularShininess = objectSpecularColour.w;
		
		// To simplify, we are NOT using the light specular value, just the object.
		vec3 specularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), 
		                            specularShininess )
						       * objectSpecularColour.rgb;	//* theLights[index].Specular.rgb
							   
		
//		vec3 specularContrib = vec3(0.0f);	// DELETE
		specularContrib *= attenuation;
		//specularContrib *= 0.001f;
		

							
		// Point, Spot, or directional
		switch( int(theLights[index].AttenAndType.w) )
		{
			case 0:	//  = point;	
				break;
			case 1: // = spot, 
				// See if the direction of the light is 
				// 	outside of the cone of the light.
				// Calculate the line between the light and the vertex
				// Which is the "lightVector" we calculated earlier
				vec3 vertexToLight = vertWorldPosXYZ.xyz - theLights[index].Position.xyz;
				
				vertexToLight = normalize(vertexToLight);
				
				// Calculate the line between the "direction" and light.
				// (but that's already the direction, because it's relative
				//	to the light)
		
				float currentLightRayAngle
						= dot( vertexToLight.xyz, theLights[index].Direction.xyz );
				currentLightRayAngle = max(0.0f, currentLightRayAngle);
				
				// Is this inside the cone? 
				float outerConeAngleCos = cos(radians(theLights[index].LightAttribs.x));
				float innerConeAngleCos = cos(radians(theLights[index].LightAttribs.y));
								
				// Is it completely outside of the spot?
				if ( currentLightRayAngle < outerConeAngleCos )
				{
					// Nope. so it's in the dark
					lightContrib = vec3(0.0f, 0.0f, 0.0f);
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
										  
					lightContrib *= penumbraRatio;
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
		outputColour.rgb += ( vertColourRGBA.rgb + specularContrib.rgb )
		                      * lightContrib;
		
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






//	gl_FragColor.rgb *= 0.0001f;		// Almost zero	
//	gl_FragColor.rgb += vec3( 0.23f, 0.420f, 0.69f );