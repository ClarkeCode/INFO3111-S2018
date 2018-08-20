#include "globalStuff.h"
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "cMeshObject.h"
#include "cShaderManager.h"
#include "cVAOManager.h"

void SetUpTextureBindingsPerObject( cShaderManager::cShaderProgram* pShaderProgram, cMeshObject* pCurMesh );


void DrawObject( cMeshObject* pCurMesh, 
				 cShaderManager::cShaderProgram* pShaderProg,	// To get at uniforms
				 cVAOManager* pVAOManager,
				 glm::mat4 matParentModel )
{
	// Is this visible
	if ( ! pCurMesh->bIsVisible )
	{
		// Skip it.
		return;
	}

	//mat4x4_identity(m);
	//matModel = glm::mat4(1.0f);		// because "math"

	glm::mat4 matModel = matParentModel;		// because "math"
			


	// Place the object in the world at 'this' location
	glm::mat4 matTranslation 
		= glm::translate( glm::mat4(1.0f), 
							glm::vec3( pCurMesh->pos.x,
										pCurMesh->pos.y, 
										pCurMesh->pos.z ) );
	matModel = matModel * matTranslation;


	//mat4x4_rotate_Z(m, m, );
	//*************************************
	// ROTATE around Z
	glm::mat4 matRotateZ = glm::rotate( glm::mat4(1.0f), 
										pCurMesh->orientation.z, // (float) glfwGetTime(), 
										glm::vec3( 0.0f, 0.0f, 1.0f ) );
	matModel = matModel * matRotateZ;
	//*************************************

	//*************************************
	// ROTATE around Y
	glm::mat4 matRotateY = glm::rotate( glm::mat4(1.0f), 
										pCurMesh->orientation.y, // (float) glfwGetTime(), 
										glm::vec3( 0.0f, 1.0f, 0.0f ) );
	matModel = matModel * matRotateY;
	//*************************************

	//*************************************
	// ROTATE around X
	glm::mat4 rotateX = glm::rotate( glm::mat4(1.0f), 
										pCurMesh->orientation.x, // (float) glfwGetTime(), 
										glm::vec3( 1.0f, 0.0, 0.0f ) );
	matModel = matModel * rotateX;
	//*************************************

		
	// Set up a scaling matrix
	glm::mat4 matScale = glm::mat4(1.0f);	

	float theScale = pCurMesh->scale;		// 1.0f;		
	matScale = glm::scale( glm::mat4(1.0f), 
		                   glm::vec3( theScale, theScale, theScale ) );

	if ( pCurMesh->bUseNonUniformScaling )
	{
		// This is multiplied the scale we loaded with the model
		matScale = glm::scale( glm::mat4(1.0f), 
		                       glm::vec3( pCurMesh->nonUniformScale.x * theScale, 
							              pCurMesh->nonUniformScale.y * theScale, 
							              pCurMesh->nonUniformScale.z * theScale) );
	}//if ( pCurMesh->bUseNonUniformScaling )

	// Apply (multiply) the scaling matrix to 
	// the existing "model" (or "world") matrix
	matModel = matModel * matScale;


	// Are we taking colour from the model file (vertex values)
	//	or as a single colour (the 'colour' value in the mesh object)
	//	passed to the vertex shader
	switch ( pCurMesh->colourSource )
	{
		case cMeshObject::USE_VERTEX_COLOURS:
			glUniform1f( pShaderProg->getUniformID_From_Name("bUseVertexColour"), 
						 (float)GL_TRUE );
//			glUniform1f( bUseVertexColour_UniLoc, GL_TRUE );
			break;

		case cMeshObject::USE_OBJECT_COLOUR:
			glUniform1f( pShaderProg->getUniformID_From_Name("bUseVertexColour"), (float)GL_FALSE );
			//glUniform1f( bUseVertexColour_UniLoc, GL_FALSE );
			glUniform4f( pShaderProg->getUniformID_From_Name("meshColourRGBA"),
							pCurMesh->diffuseColour.x, 
							pCurMesh->diffuseColour.y,
							pCurMesh->diffuseColour.z,
							pCurMesh->diffuseColour.a );
			//glUniform4f( meshColourRGBA_UniLoc,			
			//				pCurMesh->colour.x, 
			//				pCurMesh->colour.y,
			//				pCurMesh->colour.z,
			//				pCurMesh->colour.a );
			break;

		default:	
			// This shouldn't happen, so set it to "hot pink" to show error
			glUniform1f( pShaderProg->getUniformID_From_Name("bUseVertexColour"), (float)GL_FALSE );
			glUniform4f( pShaderProg->getUniformID_From_Name("meshColourRGBA"),	255.0f, 105.0f/255.0f, 180.0f/255.0f, 1.0f );
			//glUniform1f( bUseVertexColour_UniLoc, GL_FALSE );
			//glUniform4f( meshColourRGBA_UniLoc,	255.0f, 105.0f/255.0f, 180.0f/255.0f, 1.0f );
			break;

	}//switch ( pCurMesh->colourSource )


	// STARTOF: OPTIONAL vertex colour mode
	if ( pCurMesh->bEnableVertexSourceMixing )
	{
		glUniform1f( pShaderProg->getUniformID_From_Name("bEnableVertexSourceMixing"), (GLfloat)GL_TRUE );
			
		glUniform1f( pShaderProg->getUniformID_From_Name("VCS_FromVertex_Mix"), pCurMesh->fVCS_FromVertex_Mix );
		glUniform1f( pShaderProg->getUniformID_From_Name("VCS_FromMesh_Mix"), pCurMesh->fVCS_FromMesh_Mix );
		if ( pCurMesh->fVCS_FromMesh_Mix > 0.0f )
		{	// Set the mesh colour if it's being used
			glUniform4f( pShaderProg->getUniformID_From_Name("meshColourRGBA"),			
							pCurMesh->diffuseColour.x, 
							pCurMesh->diffuseColour.y,
							pCurMesh->diffuseColour.z,
							pCurMesh->diffuseColour.a );
		}
		glUniform1f( pShaderProg->getUniformID_From_Name("VCS_FromTexture_Mix"), pCurMesh->fVCS_FromTexture_Mix);
	}
	else
	{
		glUniform1f( pShaderProg->getUniformID_From_Name("bEnableVertexSourceMixing"), (GLfloat)GL_FALSE );
	}
	// ENDOF: optional vertex colour mode

	// Alpha (transparency) taken from vertex values or mesh 'colour' value?
	glUniform1f( pShaderProg->getUniformID_From_Name("bUse_vColourRGBA_AlphaValue"),
					(pCurMesh->bUseColourAlphaValue ? (float)GL_TRUE : (float)GL_FALSE) );
	//glUniform1f( bUse_vColourRGBA_AlphaValue_UniLoc,
	//				(pCurMesh->bUseColourAlphaValue ? (float)GL_TRUE : (float)GL_FALSE) );

	glUniform4f( pShaderProg->getUniformID_From_Name("objectSpecularColour"), 
				 pCurMesh->specularHighlightColour.r, 
				 pCurMesh->specularHighlightColour.g,
				 pCurMesh->specularHighlightColour.b, 
				 pCurMesh->specularShininess );
	glUniform1f( pShaderProg->getUniformID_From_Name("objectAmbientToDiffuseRatio"), 
				 pCurMesh->ambientToDiffuseRatio );

	// Use vertex (model) colours or overall (mesh 'colour') value for diffuse

	if ( pCurMesh->bDontLightObject )
	{
		glUniform1f( pShaderProg->getUniformID_From_Name("bDontLightObject"), (float)GL_TRUE );
	}
	else
	{
		glUniform1f( pShaderProg->getUniformID_From_Name("bDontLightObject"), (float)GL_FALSE);
	}
	//glUniform1f( bDontLightObject_UniLoc,
	//				(pCurMesh->bDontLightObject ? (float)GL_TRUE : (float)GL_FALSE) );


	// Is it wireframe? 
	if ( pCurMesh->isWireframe )
	{	// Yuppers.
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		//glDisable( GL_DEPTH_TEST );
		glDisable( GL_CULL_FACE );
	}
	else
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_CULL_FACE );
	}

	if ( pCurMesh->bIsSkyBoxObject )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glDisable( GL_CULL_FACE );
		glUniform1f( pShaderProg->getUniformID_From_Name("bSampleFromSkyboxTexture"),
					(float)GL_TRUE );
	}
	else
	{
		glUniform1f( pShaderProg->getUniformID_From_Name("bSampleFromSkyboxTexture"),
					(float)GL_FALSE );
		glEnable( GL_CULL_FACE );
	}




	glUniformMatrix4fv( pShaderProg->getUniformID_From_Name("matModel"),	//matModel_UniLoc, 
						1, 
						GL_FALSE, 
						glm::value_ptr(matModel));	


	//glUniformMatrix4fv( pShaderProg->getUniformID_From_Name("matView"),		//matView_Uniloc, 
	//					1, 
	//					GL_FALSE, 
	//					glm::value_ptr(matView));	

	//glUniformMatrix4fv( pShaderProg->getUniformID_From_Name("matProj"),		//matProj_Uniloc, 
	//					1, 
	//					GL_FALSE, 
	//					glm::value_ptr(matProjection));	


	//		glDrawArrays(GL_TRIANGLES, 0, 3);
	//		glDrawArrays(GL_TRIANGLES, 0, ::g_NumberOfVertsToDraw);

	SetUpTextureBindingsPerObject( pShaderProg, pCurMesh );


	// Getting uniforms in the draw call is really stupid..
	glUniform1f( pShaderProg->getUniformID_From_Name( "textureMix00" ), 
				                                      pCurMesh->textureMixRatios[0] );
	glUniform1f( pShaderProg->getUniformID_From_Name( "textureMix01" ), 
				                                      pCurMesh->textureMixRatios[1] );
	glUniform1f( pShaderProg->getUniformID_From_Name( "textureMix02" ), 
				                                      pCurMesh->textureMixRatios[2] );
	glUniform1f( pShaderProg->getUniformID_From_Name( "textureMix03" ), 
				                                      pCurMesh->textureMixRatios[3] );
	glUniform1f( pShaderProg->getUniformID_From_Name( "textureMix04" ), 
				                                      pCurMesh->textureMixRatios[4] );
	glUniform1f( pShaderProg->getUniformID_From_Name( "textureMix05" ), 
				                                      pCurMesh->textureMixRatios[5] );
	glUniform1f( pShaderProg->getUniformID_From_Name( "textureMix06" ), 
				                                      pCurMesh->textureMixRatios[6] );
	glUniform1f( pShaderProg->getUniformID_From_Name( "textureMix07" ), 
				                                      pCurMesh->textureMixRatios[7] );

	//******************************************
//	// Transparency thing
//	if ( pCurMesh->getAlphaValue() < 1.0f )
//	{
		glEnable(GL_BLEND);		// Enables "blending"
					//glDisable( GL_BLEND );
					// Source == already on framebuffer
					// Dest == what you're about to draw
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUniform1f( pShaderProg->getUniformID_From_Name("alphaTransparency"), 
		                                                 pCurMesh->getAlphaValue() );			        

//	}
//	//******************************************


	// Figure out what model we are loading
	sModelDrawInfo modelInfo;
	if ( pVAOManager->FindDrawInfoByModelName( pCurMesh->meshName, modelInfo ) )
	{	// We found something

		// Connect the buffers + vertex attribs to this particular model
		glBindVertexArray( modelInfo.VAO_ID ); 

		// Draw whatever is in that buffer
		glDrawElements( GL_TRIANGLES, 
						modelInfo.numberOfIndices,
						GL_UNSIGNED_INT,			// What type is the index array. Ours is "unsigned int"
						0 );

		// Unbind the buffers + vertex attribs
		glBindVertexArray( 0 );
	}
	// Else we DON'T draw it


	// Draw the children... 
	// If only there was a way to call DrawObject...
	if ( ! pCurMesh->vec_pChildObjects.empty() )
	{
		// Apply the INVERSE of the parent scale matrix
		float scaleBack = 1.0f / pCurMesh->scale;

		glm::mat4 matScaleBack = glm::scale( glm::mat4(1.0f), 
											 glm::vec3(scaleBack, scaleBack, scaleBack) );

		std::vector< cMeshObject* >::iterator itChildMesh = pCurMesh->vec_pChildObjects.begin();

		for ( ; itChildMesh != pCurMesh->vec_pChildObjects.end(); itChildMesh++ )
		{
			cMeshObject* pChild = *itChildMesh;

			matModel = matModel * matScaleBack;

			DrawObject( pChild, pShaderProg, pVAOManager,
						matModel );		// Key to this, starts where parent is.
		}		
	}//if ( ! pCurMesh->vec_pChildObjects.empty() )

	return;
}


// Slight speed up by caching the sampler uniform locations
// This is so we don't have to call 
// pShaderProgram->getUniformID_From_Name() 8 times for
//	every single object at every single call.
namespace nsSamplerLocCache
{
	bool bUniformsLoaded = false;
	GLint samplerUniformLocation[cMeshObject::MAXNUMBEROFTEXTURES] = {0};
};

void SetUpTextureBindingsPerObject( cShaderManager::cShaderProgram* pShaderProgram, cMeshObject* pCurMesh )
{
	// Alreay got the unifrom locations for the samplers? 
	if ( ! nsSamplerLocCache::bUniformsLoaded )
	{
		// Nope. So do this (just once)
		nsSamplerLocCache::samplerUniformLocation[0] = pShaderProgram->getUniformID_From_Name("texture00");
		nsSamplerLocCache::samplerUniformLocation[1] = pShaderProgram->getUniformID_From_Name("texture01");
		nsSamplerLocCache::samplerUniformLocation[2] = pShaderProgram->getUniformID_From_Name("texture02");
		nsSamplerLocCache::samplerUniformLocation[3] = pShaderProgram->getUniformID_From_Name("texture03");
		nsSamplerLocCache::samplerUniformLocation[4] = pShaderProgram->getUniformID_From_Name("texture04");
		nsSamplerLocCache::samplerUniformLocation[5] = pShaderProgram->getUniformID_From_Name("texture05");
		nsSamplerLocCache::samplerUniformLocation[6] = pShaderProgram->getUniformID_From_Name("texture06");
		nsSamplerLocCache::samplerUniformLocation[7] = pShaderProgram->getUniformID_From_Name("texture07");

		nsSamplerLocCache::bUniformsLoaded = true;
	}//if(!nsSamplerLocCache::bUniformsLoaded)
	

	// Set the textures once per frame 
	// ('cause Feeney just decided to do that)
	// ************************************
	// For each object...
	// ...Loop through the texture names...
	// .....Look up the texture ID, 
	// .....Bind them IN LOCATION THEY ARE IN THE MESHOBJECT
	//		pTemp->textureNames[0] = "Brenda.bmp";
	//		pTemp->textureMixRatios[0] = 0.25f;
	//
	//		pTemp->textureNames[1] = "Grass.bmp";
	//		pTemp->textureMixRatios[2] = 0.35f;
	//
	//		pTemp->textureNames[3] = "JellyBeans.bmp";
	//		pTemp->textureMixRatios[3] = 0.60f;

	for ( unsigned int textureBindingUnitLocationIndex = 0;
		  textureBindingUnitLocationIndex != cMeshObject::MAXNUMBEROFTEXTURES;
		  textureBindingUnitLocationIndex++ )
	{
		std::string meshTexture = pCurMesh->textureNames[textureBindingUnitLocationIndex];
		// Is there a texture? 
		if ( meshTexture == "" )
		{	
			// Skip the rest.
			continue;
		}
		// Get GL texture "name" (or ID)
		GLuint textureID = ::g_pTextureManager->getTextureIDFromName(meshTexture);

		// Choose a texture unit to bind to... 
		glActiveTexture( GL_TEXTURE0 +  textureBindingUnitLocationIndex );	// Selects 'texture unit'

		// Choose an active texture to bind to this unit
		glBindTexture( GL_TEXTURE_2D, textureID );	// Selects 'texture'
		
		// If we are looking up the uniform variable name, and were building 
		//	up the name using, say, a stringstream, it would be quicker to use 
		//	a switch statement...
//		switch( textureBindingUnitLocationIndex )
//		{
//		case 0:
//			glUniform1i( pShaderProgram->getUniformID_From_Name("texture00"), textureBindingUnitLocationIndex );
//			break;
//		case 1:
//			glUniform1i( pShaderProgram->getUniformID_From_Name("texture01"), textureBindingUnitLocationIndex );
//			break;
//		case 2:
//			glUniform1i( nsSamplerLocCache::samplerUniformLocation[2], textureBindingUnitLocationIndex );
//			break;
//		// ... and so on
//		}
		// Set the sampler to the "Texture UNIT"

		// But now, the lookup is done just once, and placed into an array in the 
		//	sSamplerLocCache::samplerUniformLocation[] array.
		// Because the index into that array is the same as the binding location, 
		//	we can just pass this information directly. 
		//
		// To be clear, there is NOTHING WRONG with using the switch (as above), 
		//	it's just once the values are cached, then there's no advantage to it.
		// (like everything doesn't have to be a loop or an array, is my point)
		glUniform1i( nsSamplerLocCache::samplerUniformLocation[textureBindingUnitLocationIndex], textureBindingUnitLocationIndex );
	
	}//for ( unsigned int textureBindingUnitLocationIndex = 0;

	return;
}

// Used mainly for debug objects, so we can pass explicitly pass the model matrix
void DrawObject_ExplicitModelMatrix( cMeshObject* pCurMesh, 
                                     cShaderManager::cShaderProgram* pShaderProg,	// To get at uniforms
                                     cVAOManager* pVAOManager,
                                     glm::mat4 matModelMatrix )
{
	// Is this visible
	if ( ! pCurMesh->bIsVisible )
	{
		// Skip it.
		return;
	}

	// Are we taking colour from the model file (vertex values)
	//	or as a single colour (the 'colour' value in the mesh object)
	//	passed to the vertex shader
	switch ( pCurMesh->colourSource )
	{
		case cMeshObject::USE_VERTEX_COLOURS:
			glUniform1f( pShaderProg->getUniformID_From_Name("bUseVertexColour"), 
						 (float)GL_TRUE );
//			glUniform1f( bUseVertexColour_UniLoc, GL_TRUE );
			break;

		case cMeshObject::USE_OBJECT_COLOUR:
			glUniform1f( pShaderProg->getUniformID_From_Name("bUseVertexColour"), (float)GL_FALSE );
			//glUniform1f( bUseVertexColour_UniLoc, GL_FALSE );
			glUniform4f( pShaderProg->getUniformID_From_Name("meshColourRGBA"),			
							pCurMesh->diffuseColour.x, 
							pCurMesh->diffuseColour.y,
							pCurMesh->diffuseColour.z,
							pCurMesh->diffuseColour.a );
			break;

		default:	
			// This shouldn't happen, so set it to "hot pink" to show error
			glUniform1f( pShaderProg->getUniformID_From_Name("bUseVertexColour"), (float)GL_FALSE );
			glUniform4f( pShaderProg->getUniformID_From_Name("meshColourRGBA"),	255.0f, 105.0f/255.0f, 180.0f/255.0f, 1.0f );
			break;

	}//switch ( pCurMesh->colourSource )

	// STARTOF: OPTIONAL vertex colour mode
	if ( pCurMesh->bEnableVertexSourceMixing )
	{
		glUniform1f( pShaderProg->getUniformID_From_Name("bEnableVertexSourceMixing"), (GLfloat)GL_TRUE );
			
		glUniform1f( pShaderProg->getUniformID_From_Name("VCS_FromVertex_Mix"), pCurMesh->fVCS_FromVertex_Mix );
		glUniform1f( pShaderProg->getUniformID_From_Name("VCS_FromMesh_Mix"), pCurMesh->fVCS_FromMesh_Mix );
		if ( pCurMesh->fVCS_FromMesh_Mix > 0.0f )
		{	// Set the mesh colour if it's being used
			glUniform4f( pShaderProg->getUniformID_From_Name("meshColourRGBA"),			
							pCurMesh->diffuseColour.x, 
							pCurMesh->diffuseColour.y,
							pCurMesh->diffuseColour.z,
							pCurMesh->diffuseColour.a );
		}
		glUniform1f( pShaderProg->getUniformID_From_Name("VCS_FromTexture_Mix"), pCurMesh->fVCS_FromTexture_Mix);
	}
	else
	{
		glUniform1f( pShaderProg->getUniformID_From_Name("bEnableVertexSourceMixing"), (GLfloat)GL_FALSE );
	}
	// ENDOF: optional vertex colour mode


	// Alpha (transparency) taken from vertex values or mesh 'colour' value?
	glUniform1f( pShaderProg->getUniformID_From_Name("bUse_vColourRGBA_AlphaValue"),
					(pCurMesh->bUseColourAlphaValue ? (float)GL_TRUE : (float)GL_FALSE) );
	//glUniform1f( bUse_vColourRGBA_AlphaValue_UniLoc,
	//				(pCurMesh->bUseColourAlphaValue ? (float)GL_TRUE : (float)GL_FALSE) );

	glUniform4f( pShaderProg->getUniformID_From_Name("objectSpecularColour"), 
				 pCurMesh->specularHighlightColour.r, 
				 pCurMesh->specularHighlightColour.g,
				 pCurMesh->specularHighlightColour.b, 
				 pCurMesh->specularShininess );
	glUniform1f( pShaderProg->getUniformID_From_Name("objectAmbientToDiffuseRatio"), 
				 pCurMesh->ambientToDiffuseRatio );

	// Use vertex (model) colours or overall (mesh 'colour') value for diffuse


	glUniform1f( pShaderProg->getUniformID_From_Name("bDontLightObject"),
					(pCurMesh->bDontLightObject ? (float)GL_TRUE : (float)GL_FALSE) );


	// Is it wireframe? 
	if ( pCurMesh->isWireframe )
	{	// Yuppers.
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		//glDisable( GL_DEPTH_TEST );
		glDisable( GL_CULL_FACE );
	}
	else
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_CULL_FACE );
	}


	glUniformMatrix4fv( pShaderProg->getUniformID_From_Name("matModel"),	//matModel_UniLoc, 
						1, 
						GL_FALSE, 
						glm::value_ptr(matModelMatrix));	



	// Figure out what model we are loading
	sModelDrawInfo modelInfo;
	if ( pVAOManager->FindDrawInfoByModelName( pCurMesh->meshName, modelInfo ) )
	{	// We found something

		// Connect the buffers + vertex attribs to this particular model
		glBindVertexArray( modelInfo.VAO_ID ); 

		// Draw whatever is in that buffer
		glDrawElements( GL_TRIANGLES, 
						modelInfo.numberOfIndices,
						GL_UNSIGNED_INT,			// What type is the index array. Ours is "unsigned int"
						0 );

		// Unbind the buffers + vertex attribs
		glBindVertexArray( 0 );
	}
	// Else we DON'T draw it


	// Draw the children... 
	// If only there was a way to call DrawObject...
	if ( ! pCurMesh->vec_pChildObjects.empty() )
	{
		std::vector< cMeshObject* >::iterator itChildMesh = pCurMesh->vec_pChildObjects.begin();

		for ( ; itChildMesh != pCurMesh->vec_pChildObjects.end(); itChildMesh++ )
		{
			cMeshObject* pChild = *itChildMesh;

			DrawObject( pChild, pShaderProg, pVAOManager,
						matModelMatrix );		// Key to this, starts where parent is.
		}		
	}//if ( ! pCurMesh->vec_pChildObjects.empty() )

	return;
}



