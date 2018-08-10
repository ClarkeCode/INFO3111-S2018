#include "globalStuff.h"

#include <glm/glm.hpp>
#include <sstream>

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
	

	std::string bunnyMeshName = "bun_zipper_xyz_rgba_n_uv.ply";
	std::string cowMeshName = "cow_xyz_rgba_n_uv.ply";
	std::string carMeshName = "AUDI_TT_COUPE_xyz_rgba_n_uv.ply";
	std::string buildingMeshName = ("building_xyz_rgba_n_uv.ply");
	std::string treeMeshName = ("palm-tree_xyz_rgba_n_uv.ply");
	std::string terrainMeshName = ("rob-terrain4-xyz-rgba-n-uv.ply");
	std::string moonMeshName = ("isosphere_smooth_xyz_n_rgba_uv.ply");

	{// Add an object into the "scene"
		cMeshObject* pTemp = new cMeshObject(); 
	
		pTemp->meshName = terrainMeshName;
		pTemp->friendlyName = "Terrain";
	
		pTemp->pos = glm::vec3( 0.0f, -2.0f, 0.0f );
		pTemp->orientation = glm::vec3( 0.0f, glm::radians(90.0f), 0.0f);
		pTemp->diffuseColour = glm::vec4( 142.0f/255.0f, 
								          205.0f/255.0f,
									     49.0f/255.0f,
									      1.0f );		// Transparency 'alpha'
	
		pTemp->ambientToDiffuseRatio = 0.2f;
	
		pTemp->specularHighlightColour = glm::vec3(1.0f,1.0f,1.0f);
		pTemp->specularShininess = 0.0f;
	
		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
		pTemp->scale = 50.0f / modelInfo.maxExtent;
		//pTemp->isWireframe = true;
		//pTemp->bDontLightObject = true;
	
		pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
	
		::g_vec_pMeshObjects.push_back( pTemp );
	}	

	{// Add an object into the "scene"
		cMeshObject* pTemp = new cMeshObject();

		pTemp->meshName = carMeshName;

		pTemp->friendlyName = "Car";

		pTemp->pos = glm::vec3(-0.10000f, -0.38333f, -0.63333f);
		pTemp->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);		// Transparency 'alpha'
		pTemp->orientation = glm::vec3(0.15000f, 3.14159f, 0.00000f);

		// 1.0 unit per second
		//		pTemp->velocity.z = 0.1f;
		pTemp->acceleration.z = 0.01f;

		pTemp->specularShininess = 10.0f;
		pTemp->specularHighlightColour = glm::vec3(1.0f, 1.0f, 1.0f);
		pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;

		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName,
			modelInfo);
		//pTemp->scale = 2.0f;
		pTemp->scale = 2.0f / modelInfo.maxExtent;
		pTemp->scale *= 1.0f;			// a bit bigger

		pTemp->isWireframe = false;

		::g_vec_pMeshObjects.push_back(pTemp);


	}

	{// Add an object into the "scene"
		cMeshObject* pTemp = new cMeshObject();

		pTemp->meshName = moonMeshName;

		pTemp->friendlyName = "Moon";

		pTemp->pos = glm::vec3(-0.95000f, 7.36670f, -17.59990f);
		pTemp->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);		// Transparency 'alpha'
		pTemp->orientation = glm::vec3(0.15000f, 3.14159f, 0.00000f);

		// 1.0 unit per second
		//		pTemp->velocity.z = 0.1f;
		pTemp->acceleration.z = 0.01f;

		pTemp->specularShininess = 10.0f;
		pTemp->specularHighlightColour = glm::vec3(1.0f, 1.0f, 1.0f);
		pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;

		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName,
			modelInfo);
		//pTemp->scale = 2.0f;
		pTemp->scale = 3.0f / modelInfo.maxExtent;
		pTemp->scale *= 1.0f;			// a bit bigger

		pTemp->isWireframe = false;

		::g_vec_pMeshObjects.push_back(pTemp);


	}

	{// Add an object into the "scene"
		cMeshObject* pTemp = new cMeshObject();

		pTemp->meshName = buildingMeshName;

		pTemp->friendlyName = "Building";

		pTemp->pos = glm::vec3(4.71667f, -2.56666f, -12.83331f);
		pTemp->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);		// Transparency 'alpha'
		pTemp->orientation = glm::vec3(0.0000f, 3.69159f, 0.00000f);


		pTemp->specularShininess = 10.0f;
		pTemp->specularHighlightColour = glm::vec3(1.0f, 1.0f, 1.0f);
		pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;

		sModelDrawInfo modelInfo;
		::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName,
			modelInfo);
		//pTemp->scale = 2.0f;
		pTemp->scale = 6.0f / modelInfo.maxExtent;

		pTemp->isWireframe = false;

		::g_vec_pMeshObjects.push_back(pTemp);


	}

	
	/*
		  ___        _                 _		
		 / _ \      (_)               | |    
		/ /_\ \_ __  _ _ __ ___   __ _| |___ 
		|  _  | '_ \| | '_ ` _ \ / _` | / __|
		| | | | | | | | | | | | | (_| | \__ \
		\_| |_/_| |_|_|_| |_| |_|\__,_|_|___/

	*/
	{
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = cowMeshName;
			pTemp->friendlyName = "Cow1";

			pTemp->pos = glm::vec3(-0.76667f, -0.73333f, -4.30000f);
			pTemp->orientation = glm::vec3(0.0000f, -0.45000f, 0.00000f);

			pTemp->diffuseColour = glm::vec4(243.0f / 255.0f,
				9.0f / 255.0f,
				25.0f / 255.0f,
				1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName,
				modelInfo);
			//pTemp->scale = 0.05f;
			pTemp->scale = 1.0f / modelInfo.maxExtent;

			pTemp->isWireframe = false;

			::g_vec_pMeshObjects.push_back(pTemp);
		}

		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = cowMeshName;
			pTemp->friendlyName = "Cow2";

			pTemp->pos = glm::vec3(2.06667f, -0.63333f, -5.00001f);
			pTemp->orientation = glm::vec3(0.0000f, 0.0f, 0.00000f);

			pTemp->diffuseColour = glm::vec4(243.0f / 255.0f,
				9.0f / 255.0f,
				25.0f / 255.0f,
				1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName,
				modelInfo);
			//pTemp->scale = 0.05f;
			pTemp->scale = 1.1f / modelInfo.maxExtent;

			pTemp->isWireframe = false;

			::g_vec_pMeshObjects.push_back(pTemp);
		}

		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = bunnyMeshName;

			pTemp->friendlyName = "Bugs1";		// as in Bugs Bunny

			pTemp->pos = glm::vec3(-3.58333f, -1.40000f, -4.73334f);
			pTemp->orientation = glm::vec3(0.0000f, 0.0f, 0.00000f);

			pTemp->diffuseColour = glm::vec4(1.0f,
				1.0f,
				1.0f,
				1.0f);		// Transparency 'alpha'
			pTemp->ambientToDiffuseRatio = 0.2f;
			pTemp->specularHighlightColour = glm::vec3(1.0f, 1.0f, 1.0f);
			pTemp->specularShininess = 10.0f;

			pTemp->scale = 2.0f;
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName,
				modelInfo);
			//pTemp->scale = 2.0f;
			pTemp->scale = 0.3f / modelInfo.maxExtent;

			pTemp->isWireframe = false;

			::g_vec_pMeshObjects.push_back(pTemp);
		}

		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = bunnyMeshName;
			pTemp->friendlyName = "Bugs2";

			pTemp->pos = glm::vec3(-0.15000f, -1.15000f, -6.46670f);
			pTemp->orientation = glm::vec3(-0.00000f, -2.35000f, 0.30000f);

			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f,
				205.0f / 255.0f,
				248.0f / 255.0f,
				1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName,
				modelInfo);
			//pTemp->scale = 0.1f;
			pTemp->scale = 0.34f / modelInfo.maxExtent;

			pTemp->isWireframe = false;

			::g_vec_pMeshObjects.push_back(pTemp);
		}

		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = bunnyMeshName;

			pTemp->friendlyName = "Bugs3";		// as in Bugs Bunny

			pTemp->pos = glm::vec3(-3.40000f, -1.33333f, -11.68333f);
			pTemp->orientation = glm::vec3(0.00000f, 1.65000f, -0.30000f);

			pTemp->diffuseColour = glm::vec4(1.0f,
				1.0f,
				1.0f,
				1.0f);		// Transparency 'alpha'
			pTemp->ambientToDiffuseRatio = 0.2f;
			pTemp->specularHighlightColour = glm::vec3(1.0f, 1.0f, 1.0f);
			pTemp->specularShininess = 10.0f;

			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName,
				modelInfo);
			//pTemp->scale = 2.0f;
			pTemp->scale = 0.26f / modelInfo.maxExtent;

			pTemp->isWireframe = false;

			::g_vec_pMeshObjects.push_back(pTemp);
		}
		
	}

	/*
					 ____  ____  ____  ____    __     __   __ _  ____ 
					(_  _)(  _ \(  __)(  __)  (  )   / _\ (  ( \(    \
					  )(   )   / ) _)  ) _)   / (_/\/    \/    / ) D (
					 (__) (__\_)(____)(____)  \____/\_/\_/\_)__)(____/

							"Welcome to the jungle!"
	*/
	if (true) {
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = treeMeshName;
			pTemp->friendlyName = "Tree01";

			pTemp->pos = glm::vec3(-1.0f, 0.0f, 0.0f);
			pTemp->orientation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f, 49.0f / 255.0f, 205.0f / 255.0f, 1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
			pTemp->scale = 3.0f / modelInfo.maxExtent;

			pTemp->isWireframe = false;
			pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			::g_vec_pMeshObjects.push_back(pTemp);
		}
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = treeMeshName;
			pTemp->friendlyName = "Tree02";

			pTemp->pos = glm::vec3(-2.71667f, 0.71667f, -2.56666f);
			pTemp->orientation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f, 49.0f / 255.0f, 205.0f / 255.0f, 1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
			pTemp->scale = 4.5f / modelInfo.maxExtent;

			pTemp->isWireframe = false;
			pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			::g_vec_pMeshObjects.push_back(pTemp);
		}
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = treeMeshName;
			pTemp->friendlyName = "Tree03";

			pTemp->pos = glm::vec3(1.36667f, 0.66667f, -2.91666f);
			pTemp->orientation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f, 49.0f / 255.0f, 205.0f / 255.0f, 1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
			pTemp->scale = 4.2f / modelInfo.maxExtent;

			pTemp->isWireframe = false;
			pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			::g_vec_pMeshObjects.push_back(pTemp);
		}
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = treeMeshName;
			pTemp->friendlyName = "Tree04";

			pTemp->pos = glm::vec3(-1.91667f, -0.00000f, -6.90004f);
			pTemp->orientation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f, 49.0f / 255.0f, 205.0f / 255.0f, 1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
			pTemp->scale = 2.5f / modelInfo.maxExtent;

			pTemp->isWireframe = false;
			pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			::g_vec_pMeshObjects.push_back(pTemp);
		}
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = treeMeshName;
			pTemp->friendlyName = "Tree05";

			pTemp->pos = glm::vec3(0.76667f, -0.08333f, -8.20005f);
			pTemp->orientation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f, 49.0f / 255.0f, 205.0f / 255.0f, 1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
			pTemp->scale = 2.8f / modelInfo.maxExtent;

			pTemp->isWireframe = false;
			pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			::g_vec_pMeshObjects.push_back(pTemp);
		}
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = treeMeshName;
			pTemp->friendlyName = "Tree06";

			pTemp->pos = glm::vec3(-4.40000f, 0.50000f, -8.25005f);
			pTemp->orientation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f, 49.0f / 255.0f, 205.0f / 255.0f, 1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
			pTemp->scale = 4.1f / modelInfo.maxExtent;

			pTemp->isWireframe = false;
			pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			::g_vec_pMeshObjects.push_back(pTemp);
		}
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = treeMeshName;
			pTemp->friendlyName = "Tree07";

			pTemp->pos = glm::vec3(-1.46667f, 0.15000f, -10.36668f);
			pTemp->orientation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f, 49.0f / 255.0f, 205.0f / 255.0f, 1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
			pTemp->scale = 3.5f / modelInfo.maxExtent;

			pTemp->isWireframe = false;
			pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			::g_vec_pMeshObjects.push_back(pTemp);
		}
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = treeMeshName;
			pTemp->friendlyName = "Tree08";

			pTemp->pos = glm::vec3(-3.75000f, 0.00000f, -1.23333f);
			pTemp->orientation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f, 49.0f / 255.0f, 205.0f / 255.0f, 1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
			pTemp->scale = 3.7f / modelInfo.maxExtent;

			pTemp->isWireframe = false;
			pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			::g_vec_pMeshObjects.push_back(pTemp);
		}
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = treeMeshName;
			pTemp->friendlyName = "Tree09";

			pTemp->pos = glm::vec3(3.88333f, -0.16667f, -7.60004f);
			pTemp->orientation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f, 49.0f / 255.0f, 205.0f / 255.0f, 1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
			pTemp->scale = 3.5f / modelInfo.maxExtent;

			pTemp->isWireframe = false;
			pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			::g_vec_pMeshObjects.push_back(pTemp);
		}
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = treeMeshName;
			pTemp->friendlyName = "Tree10";

			pTemp->pos = glm::vec3(-5.43335f, -0.16667f, -13.46663f);
			pTemp->orientation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f, 49.0f / 255.0f, 205.0f / 255.0f, 1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
			pTemp->scale = 3.0f / modelInfo.maxExtent;

			pTemp->isWireframe = false;
			pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			::g_vec_pMeshObjects.push_back(pTemp);
		}
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = treeMeshName;
			pTemp->friendlyName = "Tree11";

			pTemp->pos = glm::vec3(-0.21667f, -0.55000f, -13.11664f);
			pTemp->orientation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f, 49.0f / 255.0f, 205.0f / 255.0f, 1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
			pTemp->scale = 3.0f / modelInfo.maxExtent;

			pTemp->isWireframe = false;
			pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			::g_vec_pMeshObjects.push_back(pTemp);
		}
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = treeMeshName;
			pTemp->friendlyName = "Tree12";

			pTemp->pos = glm::vec3(3.46666f, 0.00000f, -0.28333f);
			pTemp->orientation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f, 49.0f / 255.0f, 205.0f / 255.0f, 1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
			pTemp->scale = 3.0f / modelInfo.maxExtent;

			pTemp->isWireframe = false;
			pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			::g_vec_pMeshObjects.push_back(pTemp);
		}
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = treeMeshName;
			pTemp->friendlyName = "Tree13";

			pTemp->pos = glm::vec3(5.88336f, 0.33333f, -5.71669f);
			pTemp->orientation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f, 49.0f / 255.0f, 205.0f / 255.0f, 1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
			pTemp->scale = 4.2f / modelInfo.maxExtent;

			pTemp->isWireframe = false;
			pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			::g_vec_pMeshObjects.push_back(pTemp);
		}
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = treeMeshName;
			pTemp->friendlyName = "Tree14";

			pTemp->pos = glm::vec3(-2.31667f, 0.00000f, 2.30000f);
			pTemp->orientation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f, 49.0f / 255.0f, 205.0f / 255.0f, 1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
			pTemp->scale = 2.4f / modelInfo.maxExtent;

			pTemp->isWireframe = false;
			pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			::g_vec_pMeshObjects.push_back(pTemp);
		}
		{// Add an object into the "scene"
			cMeshObject* pTemp = new cMeshObject();

			pTemp->meshName = treeMeshName;
			pTemp->friendlyName = "Tree15";

			pTemp->pos = glm::vec3(2.01667f, -0.03333f, 3.38333f);
			pTemp->orientation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
			pTemp->diffuseColour = glm::vec4(142.0f / 255.0f, 49.0f / 255.0f, 205.0f / 255.0f, 1.0f);		// Transparency 'alpha'
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(pTemp->meshName, modelInfo);
			pTemp->scale = 2.9f / modelInfo.maxExtent;

			pTemp->isWireframe = false;
			pTemp->colourSource = cMeshObject::USE_VERTEX_COLOURS;
			::g_vec_pMeshObjects.push_back(pTemp);
		}
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


//old version
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
