#version 420
// VERTEX SHADER

in vec4 vPosition; 
in vec4 vColourRGBA;
in vec4 vNormal;
in vec4 vTexUV;


//uniform mat4 MVP;		// Model View Projection
uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProjection;

uniform vec4 meshColourRGBA; 		// Now a vec4!!

// glUniform1f()  pass in 0 or 1 
uniform bool bUseVertexColour;		// 0 or 1 Really a float

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
//	to 1.0, or might get no colour at all (black screen).
// 
uniform bool bEnableVertexSourceMixing;
// ("VCS" stort for "Vertex Colour Source")
uniform float VCS_FromVertex_Mix;
uniform float VCS_FromMesh_Mix;
uniform float VCS_FromTexture_Mix;		// cMeshObject sets this to 1.0 by default
// *************************************************************

// If true, the A value in vColourRGBA
// will be used as the 'a' value for the colour
uniform bool bUse_vColourRGBA_AlphaValue;		

// Being passed to next shader stage... 
out vec4 vertColourRGBA;
out vec4 vertWorldPosXYZ;
out vec4 vertNormal;
out vec4 vertTexUV;
out vec4 vertOriginalVertexColourRGBA;


void main()
{
    vec4 newVertex = vPosition;		
	newVertex.w = 1.0f;
	
//    gl_Position = MVP * vec4(newVertex, 1.0);

	// Check the main render to see a similar line of code
	mat4 matMVP = matProjection * matView * matModel;
	// This is the "screen space" location
	gl_Position = matMVP * newVertex;
	
	// Calculate the "world" location of the  matrix.
	// (is vec4 because 4x4 times 4x1 is a 4x1)
	vertWorldPosXYZ = matModel * newVertex;

	// Calculate the "rotation only" transformation for the normals;
	// NO scale
	// NO tranlations (moves)sq
	// ONLY rotation
	vertNormal = inverse(transpose(matModel)) * normalize(vNormal);
	
	vertTexUV = vTexUV;
	
	// Assume we're using the (overall) object colour
	vertColourRGBA.rgb = meshColourRGBA.rgb;
	
	// By default, we use the 'whole object' 
	// alpha (transparency) value...
	vertColourRGBA.a = meshColourRGBA.a;
	
	if ( bUseVertexColour )
	{
		vertColourRGBA.rgb = vColourRGBA.rgb;	
	}
	// This will use the model file alpha value, instead
	// (Default is to NOT do this, though)
	if ( bUse_vColourRGBA_AlphaValue )
	{
		vertColourRGBA.a = vColourRGBA.a;
	}
	
	// Copy the ORIGINAL vertex colour in case we want that
	vertOriginalVertexColourRGBA = vColourRGBA;
};
