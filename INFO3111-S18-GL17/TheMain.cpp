
#include "globalStuff.h"
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "cShaderManager.h"

#include "cVAOManager.h"

//#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include <fstream>		// for the ply model loader
#include <vector>		// for the ply model loader
#include <sstream>

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


#include "cMeshObject.h"
// A vector of POINTERS to mesh objects...
//std::vector< cMeshObject* > g_vec_pMeshObjects;
//cMeshObject* g_pTheLightMesh = 0;		// or NULL

//#include "sLight.h"

#include "cLightHelper.h"

void SetUpTextureBindings( GLint shaderID );



void ShutErDown(void);


GLFWwindow* g_window = 0;



unsigned int g_NumberOfVertices = 0;		// From file
unsigned int g_NumberOfTriangles = 0;		// From file

bool LoadModelTypes(GLint shaderID, std::string &errors);


cShaderManager* g_pTheShaderManager = 0;	// NULL, 0, nullptr

cVAOManager* g_pTheVAOManager = 0;


//void DrawDebugLightSpheres(void);
void DrawDebugLightSpheres(cShaderManager::cShaderProgram* pShaderProgram);
void DrawDebugLightSpheres2(cShaderManager::cShaderProgram* pShaderProgram);

// This will draw a line of spheres, to indicate the direction and 'size' of the spot
void DrawDebugSpotLightSpheres(cShaderManager::cShaderProgram* pShaderProgram);



// NOTE: This is for the CUBE MAP (skybox) NOT the 2d textures...
void SetUpCubeMapTextureBinding( GLint shaderID );


static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void HACK_EXAMPLE_Update_Spot_Angle_OverTime(void);


unsigned int g_RockAndRollChangeTextureCounter = 0;


int main(void)
{

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	::g_window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (! ::g_window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(::g_window, key_callback);
	glfwMakeContextCurrent(::g_window);
	gladLoadGLLoader(( GLADloadproc )glfwGetProcAddress);
	glfwSwapInterval(1);



	// Shader manager thing
	::g_pTheShaderManager = new cShaderManager();

	// A shader "program"
	cShaderManager::cShaderProgram myShader;

	//... which has at least a vertex and a fragment shader
	cShaderManager::cShader vertexShader;	
	cShaderManager::cShader fragmentShader;

	vertexShader.fileName = "vert01.glsl";
	vertexShader.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	fragmentShader.fileName = "frag01.glsl"; 
	fragmentShader.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if ( ! ::g_pTheShaderManager->createProgramFromFile(
			"simpleshader", 
			vertexShader, 
			fragmentShader ) )
	{
		std::cout << "Oh no! Call Coby Briant! The shaders didn't work!" << std::endl;
		// What do you do now? Like seriously, what?
		// Likely just quit
		std::cout << ::g_pTheShaderManager->getLastError() << std::endl;
		return(-1);
	}
	else
	{
		std::cout << "The shader compiled OK." << std::endl;
	}

	GLuint shadProgID
		= ::g_pTheShaderManager->getIDFromFriendlyName("simpleshader");



	::g_pTheShaderManager->useShaderProgram(shadProgID);

	
	// Shader uniform variables
	// Get the shader by name (so we can load the unforms)
	cShaderManager::cShaderProgram* pShader = 
		::g_pTheShaderManager->pGetShaderProgramFromFriendlyName("simpleshader");

	pShader->LoadUniformLocation( "bDontLightObject" );
	pShader->LoadUniformLocation( "bUseVertexColour" );
	pShader->LoadUniformLocation( "bUse_vColourRGBA_AlphaValue" );
	pShader->LoadUniformLocation( "meshColourRGBA" );
	pShader->LoadUniformLocation( "matProjection" );
	pShader->LoadUniformLocation( "matView" );
	pShader->LoadUniformLocation( "matModel" );
	// Added
	pShader->LoadUniformLocation( "objectSpecularColour" );
	pShader->LoadUniformLocation( "globalAmbientToDiffuseRatio" );
	pShader->LoadUniformLocation( "eyeLocation" );
	pShader->LoadUniformLocation( "alphaTransparency" );

	pShader->LoadUniformLocation( "texture00" );
	pShader->LoadUniformLocation( "texture01" );
	pShader->LoadUniformLocation( "texture02" );
	pShader->LoadUniformLocation( "texture03" );
	pShader->LoadUniformLocation( "texture04" );
	pShader->LoadUniformLocation( "texture05" );
	pShader->LoadUniformLocation( "texture06" );
	pShader->LoadUniformLocation( "texture07" );

	pShader->LoadUniformLocation( "textureMix00" );
	pShader->LoadUniformLocation( "textureMix01" );
	pShader->LoadUniformLocation( "textureMix02" );
	pShader->LoadUniformLocation( "textureMix03" );
	pShader->LoadUniformLocation( "textureMix04" );
	pShader->LoadUniformLocation( "textureMix05" );
	pShader->LoadUniformLocation( "textureMix06" );
	pShader->LoadUniformLocation( "textureMix07" );

	// Cube map uniforms
	pShader->LoadUniformLocation( "bSampleFromSkyboxTexture" );
	pShader->LoadUniformLocation( "texCubeSkyboxTexture" );

	// These are for the OPTIONAL 'vertex colour source' mix.
	// If the 1st uniform is FALSE (which is default, and also set in the draw)
	// then the code behaves as it did at the end of the Friday, August 17th class. 
	//
	// If this is ENABLED, then the following uniforms are ignored:
	// - bUseVertexColour
	// It also means that the cMeshObject "eColourSource colourSource" works differently:
	// - While the "out vec4 vertColourRGBA" from the vertex shader is still being set, 
	//   there is an additional vertOriginalVertexColourRGBA being passed, so you 
	//   still have access to the ORIGINAL vertex (model) colour, if you want. 
	// 
	// cMeshObject now has corresponding variables to these, too.
	pShader->LoadUniformLocation( "bEnableVertexSourceMixing" );
	pShader->LoadUniformLocation( "VCS_FromVertex_Mix" );
	pShader->LoadUniformLocation( "VCS_FromMesh_Mix" );
	pShader->LoadUniformLocation( "VCS_FromTexture_Mix" );


	// The light values...
	//SetUpTheLights(shadProgID);
	::g_pLightManager = new cLightManager();

	::g_pLightManager->GenerateLights( NUMLIGHTS, true );
	std::string lightErrors;
	if ( ! ::g_pLightManager->InitilizeUniformLocations( pShader->ID, "theLights", NUMLIGHTS, lightErrors ) )
	{
		std::cout << lightErrors << std::endl;
	}
	else
	{
		std::cout << "Light uniforms set up OK." << std::endl;
	}

	//cLight* pTheLight = ::g_pLightManager->pGetLightAtIndex(0);

	::g_pLightManager->pGetLightAtIndex(0)->SetAsPoint();
	::g_pLightManager->pGetLightAtIndex(0)->position = glm::vec3(2.0f,2.0f,2.0f);
	::g_pLightManager->pGetLightAtIndex(0)->diffuse = glm::vec3(1.0f,1.0f,1.0f);
	::g_pLightManager->pGetLightAtIndex(0)->attenLinear = 0.324f;
	::g_pLightManager->pGetLightAtIndex(0)->attenQuad = 0.0115f;
	::g_pLightManager->pGetLightAtIndex(0)->TurnLightOn();

	//::g_pLightManager->pGetLightAtIndex(0)->SetAsSpot();
	//// Straight down: glm::vec3(0.0f, -1.0f, 0.0f);
	//::g_pLightManager->pGetLightAtIndex(0)->direction = glm::vec3( 0.0f, -1.0f, 0.0f );
	//::g_pLightManager->pGetLightAtIndex(0)->spotConeAngleOuter = 35.0f;
	//::g_pLightManager->pGetLightAtIndex(0)->spotConeAngleInner = 25.0f;

	//// Spin the light around the x axis
	//// vec4 = mat4 * vec4
	////gl_Position = matMVP * newVertex;
	//glm::vec4 spotDirection = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );

	//glm::mat4 matSpotDirAdj = glm::mat4(1.0f);	// Identity matrix

	//matSpotDirAdj = glm::rotate( glm::mat4(1.0f), 
	//							 glm::radians( 180.0f ),			// Angle to rotate
	//							 glm::vec3( 1.0f, 0.0f, 0.0f ) );	// Only x

	//spotDirection = matSpotDirAdj * spotDirection;

	//::g_pLightManager->pGetLightAtIndex(0)->direction 
	//											= glm::vec3( spotDirection );



	
//	// Try a directional light, yo
//	::g_pLightManager->pGetLightAtIndex(0)->SetAsDirectional();
//	// +ve X (shining to the left)
//	// -ve Y (shining down)
//	::g_pLightManager->pGetLightAtIndex(0)->direction = 
//			glm::normalize( glm::vec3( -1.0f, -1.0f, 0.0f ) ); 

	

	

	::g_pLightManager->pGetLightAtIndex(1)->SetAsSpot();
	::g_pLightManager->pGetLightAtIndex(1)->position = glm::vec3(2.0f,2.0f,2.0f);
	::g_pLightManager->pGetLightAtIndex(1)->diffuse = glm::vec3(1.0f,0.0f,0.0f);
	::g_pLightManager->pGetLightAtIndex(1)->attenLinear = 0.324f;
	::g_pLightManager->pGetLightAtIndex(1)->attenQuad = 0.0115f;
	//::g_pLightManager->pGetLightAtIndex(1)->specular = glm::vec3(1.0f,1.0f,1.0f);
	//::g_pLightManager->pGetLightAtIndex(1)->specularPower = 1.0f;
	//::g_pLightManager->pGetLightAtIndex(1)->setAmbientFromDiffuse(0.2f);
	::g_pLightManager->pGetLightAtIndex(1)->direction = glm::vec3(0.0f, -1.0f, 0.0f);
	::g_pLightManager->pGetLightAtIndex(1)->spotConeAngleInner = 25.0f;
	::g_pLightManager->pGetLightAtIndex(1)->spotConeAngleOuter = 30.2f;
	::g_pLightManager->pGetLightAtIndex(1)->TurnLightOn();

	//::g_pLightManager->pGetLightAtIndex(2)->SetAsPoint();
	//::g_pLightManager->pGetLightAtIndex(2)->position = glm::vec3(2.0f,2.0f,2.0f);
	//::g_pLightManager->pGetLightAtIndex(1)->diffuse = glm::vec3(1.0f,1.0f,1.0f);
	//::g_pLightManager->pGetLightAtIndex(2)->attenLinear = 0.324f;
	//::g_pLightManager->pGetLightAtIndex(2)->attenQuad = 0.0115f;
	//::g_pLightManager->pGetLightAtIndex(2)->specular = glm::vec3(1.0f,1.0f,1.0f);
	//::g_pLightManager->pGetLightAtIndex(2)->specularPower = 1.0f;
	//::g_pLightManager->pGetLightAtIndex(2)->setAmbientFromDiffuse(0.2f);
	//::g_pLightManager->pGetLightAtIndex(2)->TurnLightOn();


	// TEXTURE STUFF
	::g_pTextureManager = new cBasicTextureManager();

	::g_pTextureManager->SetBasePath( "textures" );
	// "textures\800px_COLOURBOX15652566.bmp"

	// Have to be square and BMP 
	// (Doesn't have to be a "power of 2" in size, but you'll hear that)
	if ( ::g_pTextureManager->Create2DTextureFromBMPFile( "800px_COLOURBOX15652566.bmp", true ) )
	{
		std::cout << "Loaded the texture OK." << std::endl;
	}
	else 
	{
		std::cout << "Texture DIDN'T load." << std::endl;
	}

	::g_pTextureManager->Create2DTextureFromBMPFile( "emma-watson_gettyimages-619546914.bmp", true );
	::g_pTextureManager->Create2DTextureFromBMPFile( "3487.alphabet-letter-noodles-lrg-450x300.bmp", true );
	::g_pTextureManager->Create2DTextureFromBMPFile( "13982479-jelly-beans-seamless-texture-tile.bmp", true );
	::g_pTextureManager->Create2DTextureFromBMPFile( "Grass.bmp", true );
	::g_pTextureManager->Create2DTextureFromBMPFile( "water_texture__by_heavensinyoureyes-d75vg6r.bmp", true );
	::g_pTextureManager->Create2DTextureFromBMPFile( "Brenda.bmp", true );

	// These textures are from the 2016 final exam, too. 
	// NOTE: they had to be converted to 24-bit BMP files
	::g_pTextureManager->Create2DTextureFromBMPFile( "_original.bmp", true );
	::g_pTextureManager->Create2DTextureFromBMPFile( "Aerographe albedo.bmp", true );
	::g_pTextureManager->Create2DTextureFromBMPFile( "depositphotos_14127078-Seamless-texture-surface-of-the.bmp", true );

	// The texture that matches the "corner road" model
	::g_pTextureManager->Create2DTextureFromBMPFile( "Road-2-Lane-Corner-bmp.bmp", true );
	
		//bool CreateCubeTextureFromBMPFiles( std::string cubeMapName, 
		//                                std::string posX_fileName, std::string negX_fileName, 
		//                                std::string posY_fileName, std::string negY_fileName, 
		//								std::string posZ_fileName, std::string negZ_fileName, 
		//								bool bIsSeamless, std::string &errorString );
	std::string errorString;
	if ( ! ::g_pTextureManager->CreateCubeTextureFromBMPFiles( "Sky", 
					"TropicalSunnyDayLeft2048_rot_180.bmp",		// "TropicalSunnyDayRight2048.bmp", 
					"TropicalSunnyDayRight2048_rot_180.bmp",	// "TropicalSunnyDayLeft2048.bmp"
					"TropicalSunnyDayUp2048.bmp",
					"TropicalSunnyDayDown2048.bmp",
					"TropicalSunnyDayBack2048_rot_180.bmp",	// "TropicalSunnyDayBack2048.bmp"
					"TropicalSunnyDayFront2048_rot_180.bmp",		// "TropicalSunnyDayFront2048.bmp"
					true, errorString ) )
	{
		std::cout << "Didn't load the cube map" << std::endl;
		std::cout << errorString << std::endl;
	}
	else
	{
		std::cout << "Cube map loaded OK." << std::endl;
	}

	::g_pTheVAOManager = new cVAOManager();
	
	// Load objects into scene...

//#define USE_OLDER_LOADER
#define USE_NEWER_LOADER

#ifdef USE_OLDER_LOADER
	std::vector<std::string> vecModelFilesToLoad;

	vecModelFilesToLoad.push_back("bun_zipper_res2_xyz_n_rgba_uv.ply");
	vecModelFilesToLoad.push_back("cow_xyz_n_rgba_uv.ply");
	vecModelFilesToLoad.push_back("ssj100_xyz_n_rgba_uv.ply");
	vecModelFilesToLoad.push_back("free_arena_ASCII_xyz_n_rgba_uv.ply");
	vecModelFilesToLoad.push_back("CrappyTerrain_xyz_n_rgba_uv.ply");
	vecModelFilesToLoad.push_back("isosphere_smooth_xyz_n_rgba_uv.ply");
	vecModelFilesToLoad.push_back("X-Wing_Attack_(33569 faces)_xyz_n_rgba_uv.ply");
	vecModelFilesToLoad.push_back("DockingBay_allOne_xyz_n_rgba_uv_quarter_size.ply");
	vecModelFilesToLoad.push_back("Isoshphere_Small_InvertedNormals_xyz_n_rgba_uv.ply");
	vecModelFilesToLoad.push_back("Cube_xyz_n_rgba_uv.ply");
	// These models are from the 2016 final exam:
	// NOTE: Now that there's a "fancier" ply file loader, 
	// it will load these files, even though they might not have UVs, normals, etc.
	// Also note that there is a struct that sets the way that it generates
	//	the "extra" vertex attributes: sLoadParamsINFO3111S2018
	vecModelFilesToLoad.push_back("Asteroid_001.ply");
	vecModelFilesToLoad.push_back("Asteroid_002.ply");
	vecModelFilesToLoad.push_back("Asteroid_003.ply");
	vecModelFilesToLoad.push_back("Asteroid_004.ply");
	vecModelFilesToLoad.push_back("Asteroid_005.ply");
	vecModelFilesToLoad.push_back("Asteroid_006.ply");
	vecModelFilesToLoad.push_back("Asteroid_007.ply");
	vecModelFilesToLoad.push_back("Asteroid_008.ply");
	vecModelFilesToLoad.push_back("Asteroid_009.ply");
	vecModelFilesToLoad.push_back("Asteroid_010.ply");
	vecModelFilesToLoad.push_back("Asteroid_011.ply");
	vecModelFilesToLoad.push_back("Asteroid_012.ply");
	vecModelFilesToLoad.push_back("Asteroid_013.ply");
	vecModelFilesToLoad.push_back("Asteroid_014.ply");
	vecModelFilesToLoad.push_back("Asteroid_015.ply");
	vecModelFilesToLoad.push_back("Asteroid_016.ply");

	std::string errors;
//	if ( ! LoadModelTypes(shadProgID, errors) )
//	if ( ! LoadModelTypes( shadProgID, vecModelFilesToLoad, errors ) )
	if ( ! LoadModelTypes_PlyLoader( shadProgID, vecModelFilesToLoad, errors ) )
	{
		std::cout << "There were errors loading the model files..." << std::endl;
		std::cout << errors << std::endl;
	}
#endif //USE_OLDER_LOADER
#ifdef USE_NEWER_LOADER
	std::vector<cVAOManager::sLoadParamsINFO3111S2018> vecModelFilesToLoad;

	typedef cVAOManager::sLoadParamsINFO3111S2018 LD;

	LD terrain("CrappyTerrain_xyz_n_rgba_uv.ply");
	terrain.textureCoordGenerationType = LD::PLANAR_ON_WIDEST_AXES;
	terrain.textureCoordGenerationMode = LD::FORCE_UV_GENERATION;
	terrain.textureGenerationScale = 8.0f;
	vecModelFilesToLoad.push_back( terrain );

	LD cornerRoad("Road-2-Lane-Corner_xyz_uv_100c100.ply");
	// DON'T overwrite the UV coordinates
	//cornerRoad.textureCoordGenerationMode = LD::DONT_GENERATE_UVs;
	vecModelFilesToLoad.push_back( cornerRoad) ;

	vecModelFilesToLoad.push_back( LD("bun_zipper_res2_xyz_n_rgba_uv.ply") );
	vecModelFilesToLoad.push_back( LD("cow_xyz_n_rgba_uv.ply") );
	vecModelFilesToLoad.push_back( LD("ssj100_xyz_n_rgba_uv.ply") );
	vecModelFilesToLoad.push_back( LD("free_arena_ASCII_xyz_n_rgba_uv.ply") );
	vecModelFilesToLoad.push_back( LD("isosphere_smooth_xyz_n_rgba_uv.ply") );
	vecModelFilesToLoad.push_back( LD("X-Wing_Attack_(33569 faces)_xyz_n_rgba_uv.ply") );
	vecModelFilesToLoad.push_back( LD("DockingBay_allOne_xyz_n_rgba_uv_quarter_size.ply") );
	vecModelFilesToLoad.push_back( LD("Isoshphere_Small_InvertedNormals_xyz_n_rgba_uv.ply") );
	vecModelFilesToLoad.push_back( LD("Cube_xyz_n_rgba_uv.ply") );


	// These models are from the 2016 final exam:
	// NOTE: Now that there's a "fancier" ply file loader, 
	// it will load these files, even though they might not have UVs, normals, etc.
	// Also note that there is a struct that sets the way that it generates
	//	the "extra" vertex attributes: sLoadParamsINFO3111S2018
	vecModelFilesToLoad.push_back( LD("Asteroid_001.ply") );
	vecModelFilesToLoad.push_back( LD("Asteroid_002.ply") );
	vecModelFilesToLoad.push_back( LD("Asteroid_003.ply") );
	vecModelFilesToLoad.push_back( LD("Asteroid_004.ply") );
	vecModelFilesToLoad.push_back( LD("Asteroid_005.ply") );
	vecModelFilesToLoad.push_back( LD("Asteroid_006.ply") );
	vecModelFilesToLoad.push_back( LD("Asteroid_007.ply") );
	vecModelFilesToLoad.push_back( LD("Asteroid_008.ply") );
	vecModelFilesToLoad.push_back( LD("Asteroid_009.ply") );
	vecModelFilesToLoad.push_back( LD("Asteroid_010.ply") );
	vecModelFilesToLoad.push_back( LD("Asteroid_011.ply") );
	vecModelFilesToLoad.push_back( LD("Asteroid_012.ply") );
	vecModelFilesToLoad.push_back( LD("Asteroid_013.ply") );
	vecModelFilesToLoad.push_back( LD("Asteroid_014.ply") );
	vecModelFilesToLoad.push_back( LD("Asteroid_015.ply") );
	vecModelFilesToLoad.push_back( LD("Asteroid_016.ply") );


	std::string errors;
	if ( ! LoadModelTypes_PlyLoader_2( shadProgID, vecModelFilesToLoad, errors ) )
	{
		std::cout << "There were errors loading the model files..." << std::endl;
		std::cout << errors << std::endl;
	}
#endif //USE_NEWER_LOADER


	//LoadObjectsIntoScene();
	ClearLoadMesh();

	// Print out the GL version
	std::cout << glGetString( GL_VERSION ) << std::endl;

	glm::vec3 cameraEye = glm::vec3( 0.0, 0.0, -2.0f ); 
	glm::vec3 cameraTarget = glm::vec3( 0.0f, 0.0f, 0.0f ); 
	glm::vec3 upVector = glm::vec3( 0.0f, 0.1f, 0.0f );


	glEnable( GL_DEPTH_TEST );

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );


	// The main "frame" loop, like each time through this
	//	renders the entire scene. (60Hz, 140 Hz)

	double lastTime = glfwGetTime();

	// Used for physics integration update
	double globalLastTimeStamp = glfwGetTime();




//    ___                    ___                    _               
//   |   \ _ _ __ ___ __ __ / __| __ ___ _ _  ___  | |___  ___ _ __ 
//   | |) | '_/ _` \ V  V / \__ \/ _/ -_) ' \/ -_) | / _ \/ _ \ '_ \
//   |___/|_| \__,_|\_/\_/  |___/\__\___|_||_\___| |_\___/\___/ .__/
//                                                            |_|   
	while (!glfwWindowShouldClose(::g_window))
	{

//glm::vec4 spotDirection = glm::vec4( 0.0f, -1.0f, 0.0f, 1.0f );
//
//glm::mat4 matSpotDirAdj = glm::mat4(1.0f);	// Identity matrix
//
//double theTime = glfwGetTime() * 100.0f;
//
//matSpotDirAdj = glm::rotate( glm::mat4(1.0f), 
//						glm::radians( (float)theTime ),			// Angle to rotate
//						glm::vec3( 1.0f, 0.0f, 0.0f ) );	// Only x
//
//spotDirection = matSpotDirAdj * spotDirection;
//
//::g_pLightManager->pGetLightAtIndex(0)->direction 
//									= glm::vec3( spotDirection );


//		HACK_EXAMPLE_Update_Spot_Angle_OverTime();


		// Set the optional "vertex colour source" mode to "off" by default.
		glUniform1f( pShader->getUniformID_From_Name("bEnableVertexSourceMixing"), 
					 (GLfloat)::g_bGlobalDefault_EnableVertexSourceMixing );


		//    ___                    ___ _           _         _   _          _      _            
		//   | __|_ _ _ _  __ _  _  | _ \ |_ _  _ __(_)__ ___ | | | |_ __  __| |__ _| |_ ___ _ _  
		//   | _/ _` | ' \/ _| || | |  _/ ' \ || (_-< / _(_-< | |_| | '_ \/ _` / _` |  _/ -_) '_| 
		//   |_|\__,_|_||_\__|\_, | |_| |_||_\_, /__/_\__/__/  \___/| .__/\__,_\__,_|\__\___|_|   
		//                    |__/           |__/                   |_|                           
		if ( ::g_bDoEulerPhysicsUpdate )
		{
			double currentTime = glfwGetTime();
			// Aka "frame time"
			double deltaTime = currentTime - globalLastTimeStamp;
			DoPhysicsIntegrationUpdate(deltaTime);
			globalLastTimeStamp = currentTime;


			// Move the bunny every 0.5 second
			cMeshObject* pBunny = ::g_pFindObjectByFriendlyName("Bugs");

			// Apply gravity to bunny
			pBunny->acceleration.y = -1.0f; 
			std::cout 
				<< "a = " << pBunny->acceleration.y << ", " 
				<< "v = " << pBunny->velocity.y << std::endl;

			if ( pBunny->pos.y <= -2.0f )
			{
				pBunny->velocity.y = fabs(pBunny->velocity.y);
				pBunny->pos.y = -2.0f;
			}

			cMeshObject* pLuke = ::g_pFindObjectByFriendlyName("Luke");
			pLuke->pos.z += 0.01f;
	
			pLuke->orientation.z += 0.01f;	// Do a barrel roll!!


		}//if ( ::g_bDoEulerPhysicsUpdate )
		



		float ratio;
		int width, height;
		
		//mat4x4 m, p, mvp;
		//glm::mat4 matModel;			// was "m" (also "world")
		glm::mat4 matProjection;	// was "p"
		glm::mat4 matView;			// was "view"

//		glm::mat4 mvp;

		glfwGetFramebufferSize(::g_window, &width, &height);
		ratio = width / ( float )height;
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		matProjection 
	      = glm::perspective( 0.6f, 
							  ratio, 
							  0.1f,				// Near plane	
							  100000.0f );		// Far plane

		matView = glm::mat4(1.0f);

		// Deal with the keyboard, etc.
		ProcessInputAsync( cameraEye, cameraTarget, ::g_window );

		// Look at Luke's ship
		cMeshObject* pLuke = ::g_pFindObjectByFriendlyName("Luke");
		cMeshObject* pBugs = ::g_pFindObjectByFriendlyName("Bugs");

		matView = glm::lookAt( g_myCamera.cam_eye_position,//cameraEye,		// position (3d space)
							   g_myCamera.cam_target_position,//pLuke->pos,		// looking at
							   upVector );		// Up vector




		// Start of the objects in the scene... 

		cShaderManager::cShaderProgram* pShaderProgram = 
			::g_pTheShaderManager->pGetShaderProgramFromFriendlyName("simpleshader");


		// Assume everything is using the 'per object' alpha value
		glUniform1f( pShaderProgram->getUniformID_From_Name("bUse_vColourRGBA_AlphaValue"), (float)GL_FALSE);

		//CopyLightInfoToShader(NUMLIGHTS);
		::g_pLightManager->CopyLightInfoToShader();

		DrawDebugLightSpheres2(pShaderProgram);


		// Fancy textrue thingy
		cMeshObject* pTerrain = ::g_pFindObjectByFriendlyName("TheGround");

//	g_RockAndRollChangeTextureCounter++;
//	if ( g_RockAndRollChangeTextureCounter > 15 )
//	{
//		if ( pTerrain->textureNames[0] == "Brenda.bmp" )
//		{
//			pTerrain->textureNames[0] = "Grass.bmp";
//		}
//		else
//		{
//			pTerrain->textureNames[0] = "Brenda.bmp";
//		}
//		g_RockAndRollChangeTextureCounter = 0;
//	}

		glUniformMatrix4fv( pShaderProgram->getUniformID_From_Name("matView"),		//matView_Uniloc, 
				1, 
				GL_FALSE, 
				glm::value_ptr(matView));	

		glUniformMatrix4fv( pShaderProgram->getUniformID_From_Name("matProjection"),		//matProj_Uniloc, 
				1, 
				GL_FALSE, 
				glm::value_ptr(matProjection));	

		glUniform3f( pShaderProgram->getUniformID_From_Name("eyeLocation"), 
					 cameraEye.x, cameraEye.y, cameraEye.z );

		glUniform1f( pShaderProgram->getUniformID_From_Name("globalAmbientToDiffuseRatio"), 
					 ::g_globalAmbientToDiffuseRatio );

		
// ********************
		// Position and draw skybox
		// Place skybox at camera location
		cMeshObject* pSkyBoxSphere = ::g_pFindObjectByFriendlyName("SkyBoxSpere");
		pSkyBoxSphere->pos = cameraEye;

		//uniform bool bSampleFromSkyboxTexture;

		SetUpCubeMapTextureBinding( pShaderProgram->ID );

		//pSkyBoxSphere->bIsVisible = true;
		//pSkyBoxSphere->isWireframe = false;
		//pSkyBoxSphere->bDontLightObject = false;
		//glDisable( GL_CULL_FACE );

		//glUniform1f( pShaderProgram->getUniformID_From_Name("bSampleFromSkyboxTexture"),
		//			 (float)GL_TRUE );

		//DrawObject( pSkyBoxSphere, pShaderProgram, ::g_pTheVAOManager, glm::mat4(1.0f) );

		//glEnable( GL_CULL_FACE );
		//pSkyBoxSphere->bIsVisible = false;

		//glUniform1f( pShaderProgram->getUniformID_From_Name("bSampleFromSkyboxTexture"),
		//			 (float)GL_FALSE );
// ********************



		unsigned int numberOfObjects = static_cast<unsigned int>(::g_vec_pMeshObjects.size() );

		for ( unsigned int meshIndex = 0;
			  meshIndex != numberOfObjects; meshIndex++ )
		{
			cMeshObject* pCurMesh = ::g_vec_pMeshObjects[meshIndex];

			// And.... draw the mesh:

			glm::mat4 matParentModel = glm::mat4(1.0f);		


			DrawObject( pCurMesh, 
						pShaderProgram,
						::g_pTheVAOManager,
						matParentModel);

		}// for(...
		// At this point, all the objects in the "main scene" 
		// are drawn... 

		// Present the "frame buffer" to the screen
		glfwSwapBuffers(::g_window);
		glfwPollEvents();


	} // while (!glfwWindowShouldClose(window))


	ShutErDown();

	glfwDestroyWindow(::g_window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

// Moved
//void ProcessInput( glm::vec3 &cameraEye, glm::vec3 &cameraTarget, GLFWwindow* &window )


float currentAngleY = 0.0f;
void HACK_EXAMPLE_Update_Spot_Angle_OverTime(void)
{
	// Assume that the "default" direction of that imaginary 
	//	point in front of the spot light is 0,0,1.
	glm::vec3 defaultSpotDir = glm::vec3(0.0f, 0.0f, 1.0f);	// For "reasons"

	// Just like the cMeshObject orientation... 
	glm::vec3 orientationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);	// Angle around various axes

	glm::mat4 matSpotLightOrientation = glm::mat4(1.0f);	// 

	// Remember when this spun the Triangle of Death??? 
	// (What fun times they were...)

	orientationXYZ.x = (float)glfwGetTime();

	//orientationXYZ.y = glm::radians(45.0f);;
	//orientationXYZ.y = glm::radians(currentAngleY);
	//currentAngleY += 1.0f;

	//*************************************
	// ROTATE around Z
	glm::mat4 matRotateZ = glm::rotate( glm::mat4(1.0f), 
										orientationXYZ.z, 
										glm::vec3( 0.0f, 0.0f, 1.0f ) );
	matSpotLightOrientation = matSpotLightOrientation * matRotateZ;

	// ROTATE around Y
	glm::mat4 matRotateY = glm::rotate( glm::mat4(1.0f), 
										orientationXYZ.y, 
										glm::vec3( 0.0f, 1.0f, 0.0f ) );
	matSpotLightOrientation = matSpotLightOrientation * matRotateY;

	// ROTATE around X
	glm::mat4 rotateX = glm::rotate( glm::mat4(1.0f), 
										orientationXYZ.x, 
										glm::vec3( 1.0f, 0.0, 0.0f ) );
	matSpotLightOrientation = matSpotLightOrientation * rotateX;
	//*************************************

	//matSpotLightOrientation  = glm::lookAt( SpotPosition, TheBunnyRabit, glm::vec3(0,1,1) )

	// Do the magic OF MATH!!
//	glm::vec3 defaultSpotDir
//	glm::mat4 matSpotLightOrientation
//
//	vec4                    = mat4                    * vec4
//	gl_Position             = matMVP                  * newVertex;
	glm::vec4 calcDirection = matSpotLightOrientation * glm::vec4(defaultSpotDir, 1.0f);



	::g_pLightManager->pGetLightAtIndex(0)->direction = glm::vec3(calcDirection);
	//::g_pLightManager->pGetLightAtIndex(0)->direction = defaultSpotDir;

	return;
}

void ShutErDown(void)
{
	// 
	if ( ::g_pTheShaderManager )
	{
		delete ::g_pTheShaderManager;
	}

	if ( ::g_pTheVAOManager )
	{
		::g_pTheVAOManager->ShutDown();
		delete ::g_pTheVAOManager;
	}

	if ( ::g_pTextureManager )
	{
		delete ::g_pTextureManager;
	}

	unsigned int vectorSize = (unsigned int)::g_vec_pMeshObjects.size();
	for ( unsigned int index = 0; index != vectorSize; index++ )
	{
		delete ::g_vec_pMeshObjects[index];
	}

	if ( ::g_pLightManager )	
	{ 
		delete ::g_pLightManager; 
	}

	return;
}

void DoPhysicsIntegrationUpdate(double deltaTime)
{
	// Check if too big
	const double BIGGEST_TIME_STEP_ALLOWED = 0.1f;	// 100 ms is pretty big...
	
	if ( deltaTime > BIGGEST_TIME_STEP_ALLOWED )
	{
		deltaTime = BIGGEST_TIME_STEP_ALLOWED;
	}



	unsigned int numberOfObjects = (unsigned int)::g_vec_pMeshObjects.size();
	for ( unsigned int meshIndex = 0;
		meshIndex != numberOfObjects; meshIndex++ )
	{
		cMeshObject* pCurMesh = ::g_vec_pMeshObjects[meshIndex];

		if ( pCurMesh->friendlyName != "Bugs" )
		{
			continue;
		}

		// new position = old postion + (velocity * time)
		// new velocity = old velocity + (acceleration * time)

		pCurMesh->velocity += (float)deltaTime * pCurMesh->acceleration;

		pCurMesh->pos += (float)deltaTime * pCurMesh->velocity;


		pCurMesh = NULL;
	}


	return;
}

void DrawDebugSphere( cShaderManager::cShaderProgram* pShaderProgram, 
					  glm::vec3 position, glm::vec4 colour, float scale)
{
	bool bOldState = ::g_pDebugSphere->bIsVisible;

	::g_pDebugSphere->pos = position;
	::g_pDebugSphere->bIsVisible = true;
	::g_pDebugSphere->bDontLightObject = true;
	::g_pDebugSphere->bUseColourAlphaValue = true;
	::g_pDebugSphere->isWireframe = true;


	// Draw centre of light (the position)
	::g_pDebugSphere->scale = scale;
	::g_pDebugSphere->diffuseColour = colour;
	::g_pDebugSphere->colourSource = cMeshObject::USE_OBJECT_COLOUR;

	DrawObject( ::g_pDebugSphere, pShaderProgram, ::g_pTheVAOManager, glm::mat4(1.0f) );


	// Restore it
	::g_pDebugSphere->bIsVisible = bOldState;

	return;
}
void DrawDebugLightSpheres2(cShaderManager::cShaderProgram* pShaderProgram)
{
	// Static so it's created one (not over and over again)
	// (before main is called, not that it matters with this object)
	static cLightHelper theLightHelper;


	// **************************************
	if ( ::g_bTurnOnDebugLightSpheres )
	{
		cLight* pCurLight = ::g_pLightManager->pGetLightAtIndex(::g_SelectedLightID);

		if ( ! pCurLight )
		{
			// Light index is invalid
			return;
		}

		// Draw centre of light (the position)
		//DrawDebugSphere( pShaderProgram, 
		//				  ::g_vecLights[::g_SelectedLightID].position, 
		//				  glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::WHITE ), 1.0f),
		//				  0.01f );
		DrawDebugSphere( pShaderProgram, 
						  pCurLight->position, 
						  glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::WHITE ), 1.0f),
						  0.05f );



		// Draw sphere 0 at 1% brightness
		//float distance = theLightHelper.calcApproxDistFromAtten
		//	( 0.01f,   
		//		cLightHelper::DEFAULDIFFUSEACCURACYTHRESHOLD, 
		//		cLightHelper::DEFAULTINFINITEDISTANCE, 
		//		::g_vecLights[::g_SelectedLightID].attenConst,
		//		::g_vecLights[::g_SelectedLightID].attenLinear, 
		//		::g_vecLights[::g_SelectedLightID].attenQuad );
		//DrawDebugSphere( pShaderProgram, 
		//				  ::g_vecLights[::g_SelectedLightID].position, 
		//				  glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::DARK_GRAY ), 1.0f),
		//				  distance );
		float distance = theLightHelper.calcApproxDistFromAtten
			( 0.01f,   
				cLightHelper::DEFAULDIFFUSEACCURACYTHRESHOLD, 
				cLightHelper::DEFAULTINFINITEDISTANCE, 
				pCurLight->attenConst,
				pCurLight->attenLinear, 
				pCurLight->attenQuad );
		DrawDebugSphere( pShaderProgram, 
						  pCurLight->position, 
						  glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::DARK_GRAY ), 1.0f),
						  distance );




		// Draw sphere 0 at 50% brightness
		//distance = theLightHelper.calcApproxDistFromAtten
		//	( 0.50f,   
		//		cLightHelper::DEFAULDIFFUSEACCURACYTHRESHOLD, 
		//		cLightHelper::DEFAULTINFINITEDISTANCE, 
		//		::g_vecLights[::g_SelectedLightID].attenConst,
		//		::g_vecLights[::g_SelectedLightID].attenLinear, 
		//		::g_vecLights[::g_SelectedLightID].attenQuad );
		//DrawDebugSphere( pShaderProgram, 
		//				  ::g_vecLights[::g_SelectedLightID].position, 
		//				  glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::LIGHT_SALMON ), 1.0f),
		//				  distance );
		distance = theLightHelper.calcApproxDistFromAtten
			( 0.50f,   
				cLightHelper::DEFAULDIFFUSEACCURACYTHRESHOLD, 
				cLightHelper::DEFAULTINFINITEDISTANCE, 
				pCurLight->attenConst,
				pCurLight->attenLinear, 
				pCurLight->attenQuad );
		DrawDebugSphere( pShaderProgram, 
						  pCurLight->position, 
						  glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::LIGHT_SALMON ), 1.0f),
						  distance );


		// Draw sphere 0 at 90% brightness
		//distance = theLightHelper.calcApproxDistFromAtten
		//	( 0.90f,   
		//		cLightHelper::DEFAULDIFFUSEACCURACYTHRESHOLD, 
		//		cLightHelper::DEFAULTINFINITEDISTANCE, 
		//		::g_vecLights[::g_SelectedLightID].attenConst,
		//		::g_vecLights[::g_SelectedLightID].attenLinear, 
		//		::g_vecLights[::g_SelectedLightID].attenQuad );
		//DrawDebugSphere( pShaderProgram, 
		//				  ::g_vecLights[::g_SelectedLightID].position, 
		//				  glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::CYAN ), 1.0f),
		//				  distance );
		distance = theLightHelper.calcApproxDistFromAtten
			( 0.90f,   
				cLightHelper::DEFAULDIFFUSEACCURACYTHRESHOLD, 
				cLightHelper::DEFAULTINFINITEDISTANCE, 
				pCurLight->attenConst,
				pCurLight->attenLinear, 
				pCurLight->attenQuad );
		DrawDebugSphere( pShaderProgram, 
						  pCurLight->position, 
						  glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::CYAN ), 1.0f),
						  distance );

	}// if ( ::g_bTurnOnDebugLightSpheres )


	return;
}

void SetUpCubeMapTextureBinding( GLint shaderID )
{
	GLuint skyBoxTextureID = ::g_pTextureManager->getTextureIDFromName("Sky");

	GLuint skyboxTexUnit = 20;		// Arbitrary... 0

	// 0x84C0 is texture unit #0;
	glActiveTexture( skyboxTexUnit + GL_TEXTURE0 );	// Selects 'texture unit'
//	glBindTexture( GL_TEXTURE_2D, skyBoxTextureID );	// Selects 'texture'
	glBindTexture( GL_TEXTURE_CUBE_MAP, skyBoxTextureID );	// Selects 'texture'

	//uniform samplerCube texCubeSkyboxTexture
	GLint texture00_UniformLocID = 
			glGetUniformLocation( shaderID, "texCubeSkyboxTexture" );

	// Set the sampler to the "Texture UNIT"
	glUniform1i( texture00_UniformLocID, skyboxTexUnit );
	// ************************************

	return;
}

void SetUpTextureBindings( GLint shaderID )
{

	// texture01
	GLuint jellyBeanTextureID = 
		::g_pTextureManager->getTextureIDFromName("13982479-jelly-beans-seamless-texture-tile.bmp");

	// There are likely AT LEAST 80 texture units
	// We are starting at 25 just because... 
	GLuint jellyBeanTextureUnit = 25;		// Arbitrary... 0

	// 0x84C0 is texture unit #0;
	glActiveTexture( jellyBeanTextureUnit + GL_TEXTURE0 );	// Selects 'texture unit'
	glBindTexture( GL_TEXTURE_2D, jellyBeanTextureID );	// Selects 'texture'

	GLint texture00_UniformLocID = 
			glGetUniformLocation( shaderID, "texture00" );

	// Set the sampler to the "Texture UNIT"
	glUniform1i( texture00_UniformLocID, jellyBeanTextureUnit );
	// ************************************

	// ************************************
	// texture02
	GLuint grassTextureID = 
		::g_pTextureManager->getTextureIDFromName("Grass.bmp");

	GLuint grassTextureUnit = 26;		// Arbitrary... 0

	// 0x84C0 is texture unit #0;
	glActiveTexture( grassTextureUnit + GL_TEXTURE0 );	// Selects 'texture unit'
	glBindTexture( GL_TEXTURE_2D, grassTextureID );	// Selects 'texture'

	GLint texture01_UniformLocID = 
			glGetUniformLocation( shaderID, "texture01" );

	// Set the sampler to the "Texture UNIT"
	glUniform1i( texture01_UniformLocID, grassTextureUnit );
	// ************************************

	GLuint brendaTextureID = 
		::g_pTextureManager->getTextureIDFromName("Brenda.bmp");
	// Pick a texture UNIT for Brenda's texture. We'll pick 8
	glActiveTexture( GL_TEXTURE0 + 8 );	// Selects 'texture unit'
	glBindTexture( GL_TEXTURE_2D, brendaTextureID );
	GLint texture03Sampler_LocationID = glGetUniformLocation( shaderID, "texture03" );
	glUniform1i( texture03Sampler_LocationID, 8 );
	// OpenGL 4.5 and later...
	//glBindTextureUnit( 8, texture03Sampler_LocationID );

	return;
}
