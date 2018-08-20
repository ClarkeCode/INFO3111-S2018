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

// Got this from here: https://stackoverflow.com/questions/686353/c-random-float-number-generation
float getRandInRange(float LO, float HI)
{
	return LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
}


//std::vector< cMeshObject* > g_vec_pMeshObjects;
void LoadObjectsIntoScene(void)
{	

	// Helpful define to shorten typing:
	#define CH CGLColourHelper::getInstance()

	{// Add an object into the "scene"
		cMeshObject* pCornerRoad = new cMeshObject(); 
		pCornerRoad->meshName = "Road-2-Lane-Corner_xyz_uv_100c100.ply";
		pCornerRoad->friendlyName = "CornerRoad";
		pCornerRoad->pos = glm::vec3( 0.0f, 0.0f, 0.0f );
		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName ( pCornerRoad->meshName, modelInfo );
		pCornerRoad->scale = 10.0f / modelInfo.maxExtent;

		// Matching texture is set, too
		pCornerRoad->textureNames[0] = "Road-2-Lane-Corner-bmp.bmp";
		pCornerRoad->textureMixRatios[0] = 1.0f;

		::g_vec_pMeshObjects.push_back( pCornerRoad );
	}		
	
	
	{// Add an object into the "scene"
		cMeshObject* pSkyBox = new cMeshObject(); 
		pSkyBox->meshName = "isosphere_smooth_xyz_n_rgba_uv.ply";
		//pSkyBox->meshName = "cow_xyz_n_rgba_uv.ply";
		pSkyBox->friendlyName = "SkyBoxSpere";
		pSkyBox->pos = glm::vec3( 0.0f, 0.0f, 0.0f );
		pSkyBox->diffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
		pSkyBox->colourSource = cMeshObject::USE_OBJECT_COLOUR;
		// Get the "unit" scale from the VAOManager
		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName ( pSkyBox->meshName, modelInfo );
		// Make this sphere big enough to contain the 
		//	entire scene (be careful about the far clipping plane)
		pSkyBox->scale = 5000.0f / modelInfo.maxExtent;
		pSkyBox->isWireframe = false;
		//pSkyBox->bDontLightObject = true;
		pSkyBox->bIsVisible = true;
		pSkyBox->bIsSkyBoxObject = true;
		::g_vec_pMeshObjects.push_back( pSkyBox );
	}	

	// Load a bunch of asteroids
	std::vector<std::string> vecAssNames;
	vecAssNames.push_back("Asteroid_001.ply");
	vecAssNames.push_back("Asteroid_002.ply");
	vecAssNames.push_back("Asteroid_003.ply");
	vecAssNames.push_back("Asteroid_004.ply");
	vecAssNames.push_back("Asteroid_005.ply");
	vecAssNames.push_back("Asteroid_006.ply");
	vecAssNames.push_back("Asteroid_007.ply");
	vecAssNames.push_back("Asteroid_008.ply");
	vecAssNames.push_back("Asteroid_009.ply");
	vecAssNames.push_back("Asteroid_010.ply");
	vecAssNames.push_back("Asteroid_011.ply");
	vecAssNames.push_back("Asteroid_012.ply");
	vecAssNames.push_back("Asteroid_013.ply");
	vecAssNames.push_back("Asteroid_014.ply");
	vecAssNames.push_back("Asteroid_015.ply");
	vecAssNames.push_back("Asteroid_016.ply");

	const unsigned int NUMBEROFASTEROIDS = 150;
	for ( unsigned int count = 0; count != 150; count++ )
	{// Add an object into the "scene"
		cMeshObject* pTemp = new cMeshObject(); 
		// Pick a random asteroid
		int asteroidIndex = rand() % (vecAssNames.size() + 1);
		if ( asteroidIndex > vecAssNames.size() - 1 ) 
		{
			asteroidIndex = 0;
		}
		pTemp->meshName = vecAssNames[asteroidIndex];

		float range = 10.0f;
		pTemp->pos = glm::vec3( getRandInRange(-range, range), 
								getRandInRange(-range, range), 
								getRandInRange(-range, range) );

//		std::cout << pTemp->meshName << ":" 
//			<< pTemp->pos.x << ", " << pTemp->pos.y << ", " << pTemp->pos.z << std::endl;
		
		pTemp->diffuseColour = glm::vec4( 244.0f/255.0f,  223.0f/255.0f,33.0f/255.0f, 1.0f );		// Transparency 'alpha'
		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName ( pTemp->meshName, modelInfo );
		pTemp->scale = 1.0f / modelInfo.maxExtent;
		pTemp->isWireframe = false;
		pTemp->colourSource = cMeshObject::USE_OBJECT_COLOUR;
		pTemp->bDontLightObject = false;

		pTemp->textureNames[0] = "_original.bmp";
		pTemp->textureMixRatios[0] = 0.25f;

		pTemp->textureNames[1] = "Aerographe albedo.bmp";
		pTemp->textureMixRatios[2] = 0.35f;

		pTemp->textureNames[3] = "depositphotos_14127078-Seamless-texture-surface-of-the.bmp";
		pTemp->textureMixRatios[3] = 0.60f;

		::g_vec_pMeshObjects.push_back( pTemp );
	}//for(unsigned int count...
	std::cout << "Placed " << NUMBEROFASTEROIDS << " asteroids in the scene." << std::endl;


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

		
		pTemp->textureNames[0] = "Brenda.bmp";
		pTemp->textureMixRatios[0] = 1.0f;


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

		pTemp->textureNames[3] = "Grass.bmp";
		pTemp->textureMixRatios[3] = 1.0f;


		pTemp->isWireframe = false;

		::g_vec_pMeshObjects.push_back( pTemp );
	}
	{// Add an object into the "scene"
		cMeshObject* pTemp = new cMeshObject(); 

		pTemp->meshName = "CrappyTerrain_xyz_n_rgba_uv.ply";
		pTemp->friendlyName = "TheGround";

		pTemp->pos = glm::vec3( 0.0f, -15.0f, 0.0f );
		//pTemp->diffuseColour = glm::vec4( 142.0f/255.0f, 205.0f/255.0f, 49.0f/255.0f, 1.0f );
		// "Medium Sea Green": 60,179,113
		pTemp->diffuseColour = glm::vec4( CH->getColourRGB(CGLColourHelper::SEA_GREEN), 1.0f );

		// ********************************************
		// OPTIONAL "vertex colour source" mode
		pTemp->bEnableVertexSourceMixing = true;
		pTemp->fVCS_FromVertex_Mix = 1.0f;		// <-----**** SHOW VERTEX (ply file) COLOURS
		pTemp->fVCS_FromMesh_Mix = 0.0f;
		pTemp->fVCS_FromTexture_Mix = 0.0f;
		// ********************************************

		// ********************************************
//		// This setting will make the object show texture colours, which is the "default"
//		// (i.e. like the code was on Friday, August 17th)
//		pTemp->bEnableVertexSourceMixing = true;
//		pTemp->fVCS_FromVertex_Mix = 0.0f;
//		pTemp->fVCS_FromMesh_Mix = 0.0f;
//		pTemp->fVCS_FromTexture_Mix = 1.0f;		// <-----****
//		// ********************************************


		pTemp->textureNames[0] = "Grass.bmp";
		pTemp->textureMixRatios[0] = 1.0f;


		pTemp->ambientToDiffuseRatio = 0.2f;

		pTemp->specularHighlightColour = glm::vec3(1.0f,1.0f,1.0f);
		pTemp->specularShininess = 1.0f;

		pTemp->scale = 1.0f;

		//pTemp->bDontLightObject = true;
		//pTemp->isWireframe = true;

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

		pTemp->textureNames[0] = "Brenda.bmp";
		pTemp->textureMixRatios[0] = 1.0f;


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

			pCalf->specularHighlightColour = glm::vec3(1.0f, 1.0f, 1.0f );
			pCalf->specularShininess = 100.0f;

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



		pXWing->textureNames[2] = "JellyBeans.bmp";
		pXWing->textureMixRatios[2] = 1.00f;



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

			// This is the object with Brenda R's face. 
			// I'm so very sorry...

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

			cLukesCow->setAlphaValue(0.5f);

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


	// Helpful define to shorten typing:
	//#define CH CGLColourHelper::getInstance() 
	#undef CH 


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
		loadParams.textureCoordGenerationMode = cVAOManager::sLoadParamsINFO3111S2018::FORCE_UV_GENERATION;
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

bool LoadModelTypes_PlyLoader_2( GLint shadProgID, 
								 std::vector<cVAOManager::sLoadParamsINFO3111S2018> vecModelFilesToLoad, 
								 std::string &errors )
{
	std::stringstream ssError;

	std::cout << "Loading " << vecModelFilesToLoad.size() << " models...";

	std::vector<sModelDrawInfo> vecModelInfo;
	bool bAllGood = ::g_pTheVAOManager->LoadModelsInfoVAO_PlyFile5t( shadProgID, vecModelInfo, vecModelFilesToLoad );

	std::cout << "done " << (bAllGood ? "OK" : "(there were errors)") << std::endl;

	std::stringstream ssErrors;
	unsigned int index = 0;
	for ( ; index != vecModelFilesToLoad.size(); index++ )
	{
		cVAOManager::sLoadParamsINFO3111S2018 &loadParams = vecModelFilesToLoad[index];

		if ( ! loadParams.bLoadedOK )
		{
			std::cout << loadParams.modelFileToLoad << " wasn't loaded." << std::endl;
			ssErrors << loadParams.modelFileToLoad << " wasn't loaded." << std::endl;
		}
		else
		{
			std::cout 
				<< "Loaded " << loadParams.modelFileToLoad << " OK: " << std::endl;
			std::cout 
				<< "\t" << vecModelInfo[index].numberOfVertices << " vertices, "
				<< vecModelInfo[index].numberOfTriangles << " triangles, " << std::endl;
			std::cout  
				<< "\t" << "V.Buff.ID = " << vecModelInfo[index].VertexBufferID << ", "
				<< "I.Buff.ID = " << vecModelInfo[index].IndexBufferID << ", "
				<< "VAOID = " << vecModelInfo[index].VAO_ID << std::endl;
		}//if(!loadParams.bLoadedOK
	}//for(std::vector<cVAOManager::sLoadParamsINFO3111S2018>::iterator...

	errors = ssErrors.str();

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
