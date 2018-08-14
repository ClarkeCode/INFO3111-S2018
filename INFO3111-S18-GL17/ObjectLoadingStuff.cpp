#include "globalStuff.h"

#include <glm/glm.hpp>
#include <sstream>
#include <iostream>

//extern std::vector< cMeshObject* > g_vec_pMeshObjects;
//
//extern cMeshObject* g_pTheLightMesh = 0;		// or NULL

// To handle the scaling of the object
#include "cVAOManager.h"	
// Should be in a header somewhere... 
extern cVAOManager* g_pTheVAOManager;// = 0;



//std::vector< cMeshObject* > g_vec_pMeshObjects;
void LoadObjectsIntoScene(void)
{
	// The "debug sphere" that replaces all of the other spheres for the lights, etc.
	{// Add an object into the "scene"
		::g_pDebugSphere = new cMeshObject(); 

		::g_pDebugSphere->meshName = "isosphere_smooth_xyz_n_rgba_uv.ply";

		::g_pDebugSphere->pos = glm::vec3( 0.0f, 0.0f, 0.0f );
		::g_pDebugSphere->diffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );

		// Get the "unit" scale from the VAOManager
		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName ( ::g_pDebugSphere->meshName, 
														modelInfo );
		//::g_pTheLightMesh->scale = 0.1f;
		::g_pDebugSphere->scale = 1.0f / modelInfo.maxExtent;

		::g_pDebugSphere->isWireframe = false;

		::g_pDebugSphere->bDontLightObject = true;

		// DON'T put this into the "scene" object (the "vector of things to draw")
		//::g_vec_pMeshObjects.push_back( ::g_pTheLightMesh );
	}

	{// Add an object into the "scene"
		::g_pDebugSphereSmall = new cMeshObject(); 

		::g_pDebugSphereSmall->meshName = "Isoshphere_Small_InvertedNormals_xyz_n_rgba_uv.ply";

		::g_pDebugSphereSmall->pos = glm::vec3( 0.0f, 0.0f, 0.0f );
		::g_pDebugSphereSmall->diffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );

		// Get the "unit" scale from the VAOManager
		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName ( ::g_pDebugSphereSmall->meshName, 
														modelInfo );
		//::g_pTheLightMesh->scale = 0.1f;
		::g_pDebugSphereSmall->scale = 1.0f / modelInfo.maxExtent;

		::g_pDebugSphereSmall->isWireframe = false;

		::g_pDebugSphereSmall->bDontLightObject = true;

		// DON'T put this into the "scene" object (the "vector of things to draw")
		//::g_vec_pMeshObjects.push_back( ::g_pTheLightMesh );
	}		
	
	{// Add an object into the "scene"
		::g_pDebugCone = new cMeshObject(); 

		::g_pDebugCone->meshName = "SpotCone_xyz_n_rgba_uv.ply";
		::g_pDebugCone->friendlyName = "SpotCone";

		::g_pDebugCone->pos = glm::vec3( 0.0f, 0.0f, 0.0f );
		::g_pDebugCone->diffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );

		// Get the "unit" scale from the VAOManager
		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName ( ::g_pDebugCone->meshName, 
														modelInfo );
		//::g_pTheLightMesh->scale = 0.1f;
		::g_pDebugCone->scale = 1.0f / modelInfo.maxExtent;

		::g_pDebugCone->isWireframe = false;

		::g_pDebugCone->bDontLightObject = true;

		// DON'T put this into the "scene" object (the "vector of things to draw")
		//::g_vec_pMeshObjects.push_back( ::g_pTheLightMesh );
	}	
	

	//{// Add an object into the "scene"
	//	::g_pTheLightMesh = new cMeshObject(); 
	//
	//	::g_pTheLightMesh->meshName = "isosphere_smooth_xyz_n_rgba_uv.ply";
	//
	//	::g_pTheLightMesh->pos = glm::vec3( 0.0f, 0.0f, 0.0f );
	//	::g_pTheLightMesh->colour = glm::vec4( 142.0f/255.0f, 
	//							   205.0f/255.0f,
	//								49.0f/255.0f,
	//								 1.0f );		// Transparency 'alpha'
	//
	//	// Get the "unit" scale from the VAOManager
	//	sModelDrawInfo modelInfo;
	//	::g_pTheVAOManager->FindDrawInfoByModelName ( ::g_pTheLightMesh->meshName, 
	//												  modelInfo );
	//	//::g_pTheLightMesh->scale = 0.1f;
	//	::g_pTheLightMesh->scale = 1.0f / modelInfo.maxExtent;
	//
	//
	//	::g_pTheLightMesh->isWireframe = false;
	//
	//	::g_pTheLightMesh->bDontLightObject = true;
	//
	//	::g_vec_pMeshObjects.push_back( ::g_pTheLightMesh );
	//}	
	//// Load the spheres to show the attenuation
	//for ( unsigned int index = 0; index != 4; index++ )
	//{
	//	::g_pTheLightAttenMesh[index] = new cMeshObject(); 
	//
	//	::g_pTheLightAttenMesh[index]->meshName = "isosphere_smooth_xyz_n_rgba_uv.ply";
	//
	//	::g_pTheLightAttenMesh[index]->pos = glm::vec3( 0.0f, 0.0f, 0.0f );
	//	::g_pTheLightAttenMesh[index]->colour = glm::vec4( 142.0f/255.0f, 
	//							   205.0f/255.0f,
	//								49.0f/255.0f,
	//								 1.0f );		// Transparency 'alpha'
	//	::g_pTheLightAttenMesh[index]->scale = 0.1f;
	//	::g_pTheLightAttenMesh[index]->isWireframe = true;
	//
	//	::g_pTheLightAttenMesh[index]->bDontLightObject = true;
	//
	//	::g_vec_pMeshObjects.push_back( ::g_pTheLightAttenMesh[index] );
	//}

	//{// Add an object into the "scene"
	//	cMeshObject* pTemp = new cMeshObject(); 
	//
	//	pTemp->meshName = "DockingBay_allOne_xyz_n_rgba_uv_quarter_size.ply";
	//
	//	pTemp->pos = glm::vec3( 0.0f, -1.0f, 0.0f );
	//	pTemp->orientation = glm::vec3( 0.0f, glm::radians(90.0f), 0.0f);
	//	pTemp->diffuseColour = glm::vec4( 142.0f/255.0f, 
	//							          205.0f/255.0f,
	//								     49.0f/255.0f,
	//								      1.0f );		// Transparency 'alpha'
	//
	//	pTemp->ambientToDiffuseRatio = 0.2f;
	//
	//	pTemp->specularHighlightColour = glm::vec3(1.0f,1.0f,1.0f);
	//	pTemp->specularShininess = 1.0f;
	//
	//	pTemp->scale = 2.5f;
	//	//pTemp->isWireframe = true;
	//	//pTemp->bDontLightObject = true;
	//
	//	pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
	//
	//	::g_vec_pMeshObjects.push_back( pTemp );
	//}	
	{// Add an object into the "scene"
		cMeshObject* pTemp = new cMeshObject(); 

		pTemp->meshName = "Cube_xyz_n_rgba_uv.ply";

		pTemp->friendlyName = "Cube";		// as in Bugs Bunny

		pTemp->textureMixRatios[0] = 0.0f;
		pTemp->textureMixRatios[1] = 0.0f;
		pTemp->textureMixRatios[2] = 0.0f;
		pTemp->textureMixRatios[3] = 1.0f;


		pTemp->velocity.y = 2.0f;

		pTemp->pos = glm::vec3( 0.0f, 0.0f, 0.0f );
		pTemp->diffuseColour = glm::vec4( 1.0f,
								   1.0f, 
									1.0f,
									 1.0f );		// Transparency 'alpha'
		pTemp->colourSource = cMeshObject::USE_OBJECT_COLOUR;

		pTemp->ambientToDiffuseRatio = 0.2f;
		pTemp->specularHighlightColour = glm::vec3(1.0f,1.0f,1.0f);
		// This goes from 1 to whatver (like 10,000 is fine)
		pTemp->specularShininess = 100.0f;

		pTemp->scale = 10.0f;
		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName ( pTemp->meshName, 
													  modelInfo );
		//pTemp->scale = 2.0f;
		pTemp->scale = 1.0f / modelInfo.maxExtent;

		pTemp->isWireframe = false;

		::g_vec_pMeshObjects.push_back( pTemp );
	}	

	{// Add an object into the "scene"
		cMeshObject* pTemp = new cMeshObject(); 

		pTemp->meshName = "bun_zipper_res2_xyz_n_rgba_uv.ply";

		pTemp->friendlyName = "Bugs";		// as in Bugs Bunny

		pTemp->textureMixRatios[0] = 0.0f;
		pTemp->textureMixRatios[1] = 0.0f;
		pTemp->textureMixRatios[2] = 0.0f;
		pTemp->textureMixRatios[3] = 1.0f;


		pTemp->velocity.y = 2.0f;

		pTemp->pos = glm::vec3( 0.0f, 0.0f, 0.0f );
		pTemp->diffuseColour = glm::vec4( 1.0f,
								   1.0f, 
									1.0f,
									 1.0f );		// Transparency 'alpha'
		pTemp->colourSource = cMeshObject::USE_OBJECT_COLOUR;

		pTemp->ambientToDiffuseRatio = 0.2f;
		pTemp->specularHighlightColour = glm::vec3(1.0f,1.0f,1.0f);
		// This goes from 1 to whatver (like 10,000 is fine)
		pTemp->specularShininess = 100.0f;

		pTemp->scale = 2.0f;
		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName ( pTemp->meshName, 
													  modelInfo );
		//pTemp->scale = 2.0f;
		pTemp->scale = 1.0f / modelInfo.maxExtent;

		pTemp->isWireframe = false;

		::g_vec_pMeshObjects.push_back( pTemp );
	}
	{// Add an object into the "scene"
		cMeshObject* pTemp = new cMeshObject(); 

		pTemp->meshName = "CrappyTerrain_xyz_n_rgba_uv.ply";

		pTemp->pos = glm::vec3( 0.0f, -15.0f, 0.0f );
		pTemp->diffuseColour = glm::vec4( 142.0f/255.0f, 
								          205.0f/255.0f,
									     49.0f/255.0f,
									      1.0f );		// Transparency 'alpha'

		pTemp->textureMixRatios[0] = 0.0f;
		pTemp->textureMixRatios[1] = 1.0f;		// Grass
		pTemp->textureMixRatios[2] = 0.0f;
		pTemp->textureMixRatios[3] = 0.0f;		// Select texture 3 (Brenda R.)

		pTemp->ambientToDiffuseRatio = 0.2f;

		pTemp->specularHighlightColour = glm::vec3(1.0f,1.0f,1.0f);
		pTemp->specularShininess = 1.0f;

		pTemp->scale = 1.0f;
		pTemp->isWireframe = false;
		//pTemp->bDontLightObject = true;

		pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;

		::g_vec_pMeshObjects.push_back( pTemp );
	}	
	
	{// Add an object into the "scene"
		cMeshObject* pTemp = new cMeshObject(); 

		pTemp->meshName = "cow_xyz_n_rgba_uv.ply";

		pTemp->pos = glm::vec3( 1.0f, 0.0f, 0.0f );
		pTemp->diffuseColour = glm::vec4( 243.0f/255.0f,		
								     9.0f/255.0f,
								    25.0f/255.0f,
									 1.0f );		// Transparency 'alpha'
		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName ( pTemp->meshName, 
													  modelInfo );
		//pTemp->scale = 0.05f;
		pTemp->scale = 1.0f / modelInfo.maxExtent;

		pTemp->isWireframe = false;

		::g_vec_pMeshObjects.push_back( pTemp );
	}

	{// Add an object into the "scene"
		cMeshObject* pCow = new cMeshObject(); 

		pCow->meshName = "cow_xyz_n_rgba_uv.ply";
		pCow->friendlyName = "Cow, with calf";

		pCow->pos = glm::vec3( 2.0f, 1.0f, 0.0f );
		pCow->diffuseColour = glm::vec4( 142.0f/255.0f,	
								   205.0f/255.0f,
								   248.0f/255.0f,
									 1.0f );		// Transparency 'alpha'
		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName ( pCow->meshName, 
													  modelInfo );
		//pTemp->scale = 0.1f;
		pCow->scale = 1.0f / modelInfo.maxExtent;

		pCow->isWireframe = true;


		{// Add an object into the "scene"
			cMeshObject* pCalf = new cMeshObject(); 

			pCalf->meshName = "cow_xyz_n_rgba_uv.ply";
			pCalf->friendlyName = "Calf";

			pCalf->pos = glm::vec3( 0.0f, 1.0f, 0.0f );
			pCalf->diffuseColour = glm::vec4( 142.0f/255.0f,	
										205.0f/255.0f,
										248.0f/255.0f,
											1.0f );		// Transparency 'alpha'

			pCalf->colourSource = cMeshObject::USE_OBJECT_COLOUR;

			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName ( pCalf->meshName, 
															modelInfo );
			//pTemp->scale = 0.1f;
			pCalf->scale = 0.5f / modelInfo.maxExtent;
			//pCalf->scale = 1.0f;

			pCalf->isWireframe = false;

			// Calf will move with cow
			pCow->vec_pChildObjects.push_back( pCalf );
		}
		::g_vec_pMeshObjects.push_back( pCow );
	}



	{// Add an object into the "scene"
		cMeshObject* pXWing = new cMeshObject(); 

		pXWing->meshName = "X-Wing_Attack_(33569 faces)_xyz_n_rgba_uv.ply";

		pXWing->friendlyName = "Luke";		

		pXWing->pos = glm::vec3( 0.0f, 2.0f, 0.0f );
		pXWing->diffuseColour = glm::vec4( 1.0f,  1.0f, 1.0f, 1.0f );		// Transparency 'alpha'
		pXWing->orientation = glm::vec3(0.0f, glm::radians(180.0f), 0.0f );

		// 1.0 unit per second
//		pTemp->velocity.z = 0.1f;
		pXWing->acceleration.z = 0.01f;


		pXWing->textureMixRatios[0] = 0.0f;
		pXWing->textureMixRatios[1] = 1.0f;



		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName ( pXWing->meshName, 
													  modelInfo );
		//pTemp->scale = 2.0f;
		pXWing->scale = 1.0f / modelInfo.maxExtent;
		pXWing->scale *= 2.0f;			// a bit bigger

		pXWing->isWireframe = false;

		// Add the X-Wing to the "things I'm drawing" vector...
		::g_vec_pMeshObjects.push_back( pXWing );

		//// Add 4 cMeshObjects... 
		//lightMeshLeftBrake.isVisible = false;
		//lightMeshLeftBrake.friendlyName = "LeftBrakeLight";
		//pCar->vecChild.Push_back( lightMeshLeftBrake );
		//pCar->vecChild.Push_back( lightMeshRightBrake );


		// Have a cow track with Luke...
		{// Load a cow!!!
			cMeshObject* cLukesCow = new cMeshObject(); 
			cLukesCow->meshName = "cow_xyz_n_rgba_uv.ply";
			// Because this is added as a "child" of another object,
			//  this location is RELATIVE to the 'parent' object
			cLukesCow->pos = glm::vec3( 1.0f, 0.0f, 0.0f );
			cLukesCow->diffuseColour = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
			cLukesCow->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName ( cLukesCow->meshName, modelInfo );
			cLukesCow->scale = 5.0f / modelInfo.maxExtent;
			cLukesCow->isWireframe = false;
			//::g_vec_pMeshObjects.push_back( pTemp );


			cLukesCow->textureMixRatios[0] = 0.0f;
			cLukesCow->textureMixRatios[1] = 0.0f;
			cLukesCow->textureMixRatios[2] = 0.0f;
			cLukesCow->textureMixRatios[3] = 1.0f;

			pXWing->vec_pChildObjects.push_back( cLukesCow );

		}// ENDOF: Load a cow!!

	}


	{// Add an object into the "scene"
		cMeshObject* pTemp = new cMeshObject(); 

		pTemp->meshName = "ssj100_xyz_n_rgba_uv.ply";

		// 2 * PI   
		// 1 PI = 180
		// 0.5 = 90 
		// 0.25 = 45


//		pTemp->orientation.x = 3.14159f * 0.25f;		// YOLO, right? 
		pTemp->orientation.y = 3.14159f * 0.5f;		// YOLO, right? 

		pTemp->pos = glm::vec3( -1.0f, 0.0f, 0.0f );
		pTemp->diffuseColour = glm::vec4( 142.0f/255.0f, 
									49.0f/255.0f,
								   205.0f/255.0f,
									 1.0f );		// Transparency 'alpha'
		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName ( pTemp->meshName, 
													  modelInfo );
		//pTemp->scale = 1.5f;
		pTemp->scale = 1.0f / modelInfo.maxExtent;

		pTemp->textureMixRatios[0] = 1.0f;
		pTemp->textureMixRatios[1] = 0.0f;

		pTemp->isWireframe = false;

		::g_vec_pMeshObjects.push_back( pTemp );
	}

	{// Add an object into the "scene"
		cMeshObject* pTemp = new cMeshObject(); 

		pTemp->meshName = "free_arena_ASCII_xyz_n_rgba_uv.ply";

		pTemp->pos = glm::vec3( 0.0f, 0.0f, 0.0f );
		pTemp->diffuseColour = glm::vec4( 244.0f/255.0f,  
									223.0f/255.0f,
								    33.0f/255.0f,
									 1.0f );		// Transparency 'alpha'
		// Largest "extent" in this model
		// is 40.2828 
		
		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName ( pTemp->meshName, 
													  modelInfo );
		//pTemp->scale = 1.0f/40.2828f;
		pTemp->scale = 1.0f / modelInfo.maxExtent;


		// Now my model 1.0 unit in size (-1 to 1)

		pTemp->isWireframe = false;

		::g_vec_pMeshObjects.push_back( pTemp );
	}


	return;
}

bool LoadModelTypes(GLint shadProgID, std::vector<std::string> vecModelNames, std::string &errors)
{
	bool bAllGood = true;

	std::stringstream ssError;

	for ( std::vector<std::string>::iterator itModelName = vecModelNames.begin(); 
		  itModelName != vecModelNames.end(); itModelName++ )
	{
		sModelDrawInfo theModel;
		if ( ! ::g_pTheVAOManager->LoadModelIntoVAO( *itModelName, theModel, shadProgID ) )
		{
			ssError << "ERROR: " << *itModelName << " wasn't loaded" << std::endl;
			bAllGood = false;
		}
	}// for ( ...iterator itModelName...

	errors = ssError.str();

	return bAllGood;
}

bool LoadModelTypes_PlyLoader(GLint shadProgID, std::vector<std::string> vecModelNames, std::string &errors)
{
	bool bAllGood = true;

	std::stringstream ssError;

	for ( std::vector<std::string>::iterator itModelName = vecModelNames.begin(); 
		  itModelName != vecModelNames.end(); itModelName++ )
	{
		sModelDrawInfo theModel;
		cVAOManager::sLoadParamsINFO3111S2018 loadParams;
		//
		loadParams.Force_UV_Regeneration = true;
		// 
		std::string plyLoadErrors;
		//if ( ! ::g_pTheVAOManager->LoadModelIntoVAO( *itModelName, theModel, shadProgID ) )
		if ( ! ::g_pTheVAOManager->LoadModelInfoVAO_PlyFile5t( *itModelName, shadProgID, 
			                                                    theModel, plyLoadErrors, 
		                                                        loadParams) )
		{
			ssError << "ERROR: " << *itModelName << " wasn't loaded because: " << plyLoadErrors << std::endl;
			bAllGood = false;
		}
		else
		{
			std::cout 
				<< "Loaded " << *itModelName << " OK: " << std::endl;
			std::cout 
				<< "\t" << theModel.numberOfVertices << " vertices, "
				<< theModel.numberOfTriangles << " triangles, " << std::endl;
			std::cout  
				<< "\t" << "V.Buff.ID = " << theModel.VertexBufferID << ", "
				<< "I.Buff.ID = " << theModel.IndexBufferID << ", "
				<< "VAOID = " << theModel.VAO_ID << std::endl;
		}

	}// for ( ...iterator itModelName...

	errors = ssError.str();

	return bAllGood;
}


bool LoadModelTypes(GLint shadProgID, std::string &errors)
{
	std::stringstream ssError;

	bool bAllGood = true;

//	"bun_zipper_res2_xyz.ply", "ssj100_xyz.ply", "building_xyz.ply"
	sModelDrawInfo bunny;
	if ( ! ::g_pTheVAOManager->LoadModelIntoVAO( "bun_zipper_res2_xyz_n_rgba_uv.ply", bunny, shadProgID ) )
	{
		ssError << "ERROR: bun_zipper_res2_xyz_n_rgba_uv.ply wasn't loaded" << std::endl;
		bAllGood = false;
	}

	sModelDrawInfo cow;
	if ( ! ::g_pTheVAOManager->LoadModelIntoVAO( "cow_xyz_n_rgba_uv.ply", cow, shadProgID ) )
	{
		ssError << "ERROR: cow_xyz_n_rgba_uv.ply wasn't loaded" << std::endl;
		bAllGood = false;
	}	

	sModelDrawInfo airplane;
	if ( ! ::g_pTheVAOManager->LoadModelIntoVAO( "ssj100_xyz_n_rgba_uv.ply", airplane, shadProgID ) )
	{
		ssError << "ERROR: ssj100_xyz_n_rgba_uv.ply wasn't loaded" << std::endl;
		bAllGood = false;
	}

	sModelDrawInfo arena;
	if ( ! ::g_pTheVAOManager->LoadModelIntoVAO( "free_arena_ASCII_xyz_n_rgba_uv.ply", arena, shadProgID ) )
	{
		ssError << "ERROR: free_arena_ASCII_xyz_n_rgba_uv.ply wasn't loaded" << std::endl;
		bAllGood = false;
	}

	sModelDrawInfo terrain;
	if ( ! ::g_pTheVAOManager->LoadModelIntoVAO( "CrappyTerrain_xyz_n_rgba_uv.ply", terrain, shadProgID ) )
	{
		ssError << "ERROR: CrappyTerrain_xyz_n_rgba_uv.ply wasn't loaded" << std::endl;
		bAllGood = false;
	}

	sModelDrawInfo sphere;
	if ( ! ::g_pTheVAOManager->LoadModelIntoVAO( "isosphere_smooth_xyz_n_rgba_uv.ply", sphere, shadProgID ) )
	{
		ssError << "ERROR: isosphere_smooth_xyz_n_rgba_uv.ply wasn't loaded" << std::endl;
		bAllGood = false;
	}

	sModelDrawInfo UseTheForceLuke;
	if ( ! ::g_pTheVAOManager->LoadModelIntoVAO( "X-Wing_Attack_(33569 faces)_xyz_n_rgba_uv.ply", UseTheForceLuke, shadProgID ) )
	{
		ssError << "ERROR: X-Wing_Attack_(33569 faces)_xyz_n_rgba_uv.ply wasn't loaded" << std::endl;
		bAllGood = false;
	}

	errors = ssError.str();

	return bAllGood;
}
