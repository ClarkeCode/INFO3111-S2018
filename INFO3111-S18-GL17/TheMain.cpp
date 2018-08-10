
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


void ShutErDown(void);


GLFWwindow* g_window = 0;



unsigned int g_NumberOfVertices = 0;		// From file
unsigned int g_NumberOfTriangles = 0;		// From file

bool LoadModelTypes(GLint shaderID, std::string &errors);


//void LoadObjectsIntoScene(void);

cShaderManager* g_pTheShaderManager = 0;	// NULL, 0, nullptr

cVAOManager* g_pTheVAOManager = 0;

// When true, the DoPhysicsUpdate is called.
//bool g_bDoEulerPhysicsUpdate = false;		


// Moved
//struct sLight


// moved
//const unsigned int NUMLIGHTS = 3;
//std::vector<sLight> g_vecLights;

// These functions happen in the cLightManager class, now
//void SetUpTheLights(GLint shaderProgID);
//void CopyLightInfoToShader( unsigned int numberOfLightsToCopy );

//void DrawDebugLightSpheres(void);
void DrawDebugLightSpheres(cShaderManager::cShaderProgram* pShaderProgram);
void DrawDebugLightSpheres2(cShaderManager::cShaderProgram* pShaderProgram);

// This will draw a line of spheres, to indicate the direction and 'size' of the spot
void DrawDebugSpotLightSpheres(cShaderManager::cShaderProgram* pShaderProgram);

//// A more general draw sphere
//void DrawDebugSphere( cShaderManager::cShaderProgram* pShaderProgram, 
//					  glm::vec3 position, glm::vec4 colour, float scale);


static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

// Moved
//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)


//// Processes input (defined below)
//void ProcessInput( glm::vec3 &cameraEye, 
//                   glm::vec3 &cameraTarget, 
//                   GLFWwindow* &window );


void HACK_EXAMPLE_Update_Spot_Angle_OverTime(void);


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

	//mvp_location = glGetUniformLocation(shadProgID, "MVP");		// program

//	GLint matModel_UniLoc = glGetUniformLocation(shadProgID, "matModel");
//	GLint matView_Uniloc = glGetUniformLocation(shadProgID, "matView");
//	GLint matProj_Uniloc = glGetUniformLocation(shadProgID, "matProjection");

	// If it returns -1, then it didn't find it.
//	GLint meshColourRGBA_UniLoc = glGetUniformLocation(shadProgID, "meshColourRGBA");
//	GLint bUse_vColourRGBA_AlphaValue_UniLoc = glGetUniformLocation(shadProgID, "bUse_vColourRGBA_AlphaValue");
//	GLint bUseVertexColour_UniLoc = glGetUniformLocation(shadProgID, "bUseVertexColour");

//	GLint bDontLightObject_UniLoc = glGetUniformLocation(shadProgID, "bDontLightObject" );
	
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

	

	

	//::g_pLightManager->pGetLightAtIndex(1)->SetAsPoint();
	//::g_pLightManager->pGetLightAtIndex(1)->position = glm::vec3(2.0f,2.0f,2.0f);
	//::g_pLightManager->pGetLightAtIndex(1)->diffuse = glm::vec3(1.0f,1.0f,1.0f);
	//::g_pLightManager->pGetLightAtIndex(1)->attenLinear = 0.324f;
	//::g_pLightManager->pGetLightAtIndex(1)->attenQuad = 0.0115f;
	//::g_pLightManager->pGetLightAtIndex(1)->specular = glm::vec3(1.0f,1.0f,1.0f);
	//::g_pLightManager->pGetLightAtIndex(1)->specularPower = 1.0f;
	//::g_pLightManager->pGetLightAtIndex(1)->setAmbientFromDiffuse(0.2f);
	//::g_pLightManager->pGetLightAtIndex(1)->TurnLightOn();

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






	::g_pTheVAOManager = new cVAOManager();
	
	// Load objects into scene...

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


	std::string errors;
//	if ( ! LoadModelTypes(shadProgID, errors) )
//	if ( ! LoadModelTypes( shadProgID, vecModelFilesToLoad, errors ) )
	if ( ! LoadModelTypes_PlyLoader( shadProgID, vecModelFilesToLoad, errors ) )
	{
		std::cout << "There were errors loading the model files..." << std::endl;
		std::cout << errors << std::endl;
	}



	LoadObjectsIntoScene();

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
		


		//    ___          _                                      _   
		//   | _ ) __ _ __(_)__   _ __  _____ _____ _ __  ___ _ _| |_ 
		//   | _ \/ _` (_-< / _| | '  \/ _ \ V / -_) '  \/ -_) ' \  _|
		//   |___/\__,_/__/_\__| |_|_|_\___/\_/\___|_|_|_\___|_||_\__|
		//                                                            
		//if ( pBunny )
		//{
		//	//pBunny->pos.y += 0.01f;		// contin. motion

		//	double currentTime = glfwGetTime();
		//	const double TIME_WE_SHOULD_WAIT = 0.5;		// 500 ms

		//	// How much time has passed? 
		//	if ( (currentTime - lastTime) > TIME_WE_SHOULD_WAIT )
		//	{
		//		// Do the thing
		//		pBunny->pos.y += 0.01f;
		//		lastTime = glfwGetTime();		// "reset" the "last" time

		//	}//if ( (currentTime..

		//}//if ( pBunny )


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




		//matView = glm::lookAt( cameraEye,		// position (3d space)
		//					   cameraTarget,	// looking at
		//					   upVector );		// Up vector

		// Look at Luke's ship
		cMeshObject* pLuke = ::g_pFindObjectByFriendlyName("Luke");
		cMeshObject* pBugs = ::g_pFindObjectByFriendlyName("Bugs");

		matView = glm::lookAt( cameraEye,		// position (3d space)
							   pLuke->pos,		// looking at
							   upVector );		// Up vector


		// Start of the objects in the scene... 

		cShaderManager::cShaderProgram* pShaderProgram = 
			::g_pTheShaderManager->pGetShaderProgramFromFriendlyName("simpleshader");


		//CopyLightInfoToShader(NUMLIGHTS);
		::g_pLightManager->CopyLightInfoToShader();

		// **************************************
		// MOVED: if ( ::g_bTurnOnDebugLightSpheres )
		//DrawDebugLightSpheres(pShaderProgram);
		DrawDebugLightSpheres2(pShaderProgram);

		//DrawDebugSphere( pShaderProgram, 
		//				 glm::vec3(0.0f, 1.0f, 0.0f), 
		//				 glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 
		//				 0.1f );
		//DrawDebugSphere( pShaderProgram, 
		//				 glm::vec3(0.0f, 1.0f, 0.0f), 
		//				 glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 
		//				 0.3f );







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


		unsigned int numberOfObjects = 
			static_cast<unsigned int>(::g_vec_pMeshObjects.size() );

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
	::g_pDebugSphere->colourSource = cMeshObject::USE_OBJECT_COLOUR;


	// Draw centre of light (the position)
	::g_pDebugSphere->scale = scale;
	::g_pDebugSphere->diffuseColour = colour;

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

//void DrawDebugLightSpheres(cShaderManager::cShaderProgram* pShaderProgram)
//{
//	// Static so it's created one (not over and over again)
//	// (before main is called, not that it matters with this object)
//	static cLightHelper theLightHelper;
//
//
//	// **************************************
//	if ( ::g_bTurnOnDebugLightSpheres )
//	{
//		::g_pDebugSphere->pos = ::g_vecLights[::g_SelectedLightID].position;
//		::g_pDebugSphere->bIsVisible = true;
//		::g_pDebugSphere->bDontLightObject = true;
//		::g_pDebugSphere->bUseColourAlphaValue = true;
//		::g_pDebugSphere->isWireframe = true;
//		::g_pDebugSphere->colourSource = cMeshObject::USE_OBJECT_COLOUR;
//
//
//
//		// Draw centre of light (the position)
//		::g_pDebugSphere->scale = 0.01f;
//		::g_pDebugSphere->colour = glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::WHITE ), 1.0f);
//		DrawObject( ::g_pDebugSphere, pShaderProgram, ::g_pTheVAOManager, glm::mat4(1.0f) );
//
//
//		// Draw sphere 0 at 1% brightness
//		float distance = theLightHelper.calcApproxDistFromAtten
//			( 0.01f,   
//				cLightHelper::DEFAULDIFFUSEACCURACYTHRESHOLD, 
//				cLightHelper::DEFAULTINFINITEDISTANCE, 
//				::g_vecLights[::g_SelectedLightID].attenConst,
//				::g_vecLights[::g_SelectedLightID].attenLinear, 
//				::g_vecLights[::g_SelectedLightID].attenQuad );
//		::g_pDebugSphere->scale = distance;
//		::g_pDebugSphere->colour = glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::DARK_GRAY ), 1.0f);
//		DrawObject( ::g_pDebugSphere, pShaderProgram, ::g_pTheVAOManager, glm::mat4(1.0f) );
//
//
//
//
//		// Draw sphere 0 at 50% brightness
//		distance = theLightHelper.calcApproxDistFromAtten
//			( 0.50f,   
//				cLightHelper::DEFAULDIFFUSEACCURACYTHRESHOLD, 
//				cLightHelper::DEFAULTINFINITEDISTANCE, 
//				::g_vecLights[::g_SelectedLightID].attenConst,
//				::g_vecLights[::g_SelectedLightID].attenLinear, 
//				::g_vecLights[::g_SelectedLightID].attenQuad );
//		::g_pDebugSphere->scale = distance;
//		::g_pDebugSphere->colour = glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::LIGHT_SALMON ), 1.0f);
//		DrawObject( ::g_pDebugSphere, pShaderProgram, ::g_pTheVAOManager, glm::mat4(1.0f) );
//
//
//
//		// Draw sphere 0 at 90% brightness
//		distance = theLightHelper.calcApproxDistFromAtten
//			( 0.90f,   
//				cLightHelper::DEFAULDIFFUSEACCURACYTHRESHOLD, 
//				cLightHelper::DEFAULTINFINITEDISTANCE, 
//				::g_vecLights[::g_SelectedLightID].attenConst,
//				::g_vecLights[::g_SelectedLightID].attenLinear, 
//				::g_vecLights[::g_SelectedLightID].attenQuad );
//		::g_pDebugSphere->scale = distance;
//		::g_pDebugSphere->colour = glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::CYAN ), 1.0f);
//		DrawObject( ::g_pDebugSphere, pShaderProgram, ::g_pTheVAOManager, glm::mat4(1.0f) );
//
//
//	}//if ( ::g_bTurnOnDebugLightSpheres )
//	else
//	{
//		// Don't Draw the light spheres
//
//		// Now they aren't in the vector of objects, so if we don't draw it, then it won't appear
//
//		//::g_pTheLightAttenMesh[0]->bIsVisible  = 
//		//::g_pTheLightAttenMesh[1]->bIsVisible = 
//		//::g_pTheLightAttenMesh[2]->bIsVisible = false;
//		//::g_pTheLightMesh->bIsVisible = false;
//	}//if ( ::g_bTurnOnDebugLightSpheres )
//
//
//	return;
//}

//// OLD draw debug light spheres...
//void DrawDebugLightSpheres(void)
//{
//	// Static so it's created one (not over and over again)
//	// (before main is called, not that it matters with this object)
//	static cLightHelper theLightHelper;
//
//
//	// **************************************
//	if ( ::g_bTurnOnDebugLightSpheres )
//	{
//		::g_pTheLightAttenMesh[0]->bIsVisible  = 
//		::g_pTheLightAttenMesh[1]->bIsVisible = 
//		::g_pTheLightAttenMesh[2]->bIsVisible = true;
//
//		::g_pTheLightMesh->bIsVisible = true;
//
//		// Take the other 4 meshes and change the location to where the light is
//		::g_pTheLightMesh->pos 
//			= ::g_pTheLightAttenMesh[1]->pos 
//			= ::g_pTheLightAttenMesh[2]->pos
//			= ::g_pTheLightAttenMesh[3]->pos = ::g_vecLights[::g_SelectedLightID].position;
//
//
//		//// move that light mesh to where the light is at, yo
//		::g_pTheLightMesh->pos = ::g_vecLights[::g_SelectedLightID].position;
//		::g_pTheLightMesh->scale = 0.01f;
//		::g_pTheLightMesh->colour = glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::WHITE ), 1.0f);
//
//
//		// Draw sphere 0 at 1% brightness
//		float distance = theLightHelper.calcApproxDistFromAtten
//			( 0.01f,   
//				cLightHelper::DEFAULDIFFUSEACCURACYTHRESHOLD, 
//				cLightHelper::DEFAULTINFINITEDISTANCE, 
//				::g_vecLights[::g_SelectedLightID].attenConst,
//				::g_vecLights[::g_SelectedLightID].attenLinear, 
//				::g_vecLights[::g_SelectedLightID].attenQuad );
//		::g_pTheLightAttenMesh[0]->scale = distance;
//
//		::g_pTheLightAttenMesh[0]->bDontLightObject = true;
//		::g_pTheLightAttenMesh[0]->bUseColourAlphaValue = true;
//		::g_pTheLightAttenMesh[0]->colourSource = cMeshObject::USE_OBJECT_COLOUR;
//		::g_pTheLightAttenMesh[0]->colour = glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::DARK_GRAY ), 1.0f);
//
//		// Draw sphere 0 at 50% brightness
//		distance = theLightHelper.calcApproxDistFromAtten
//			( 0.50f,   
//				cLightHelper::DEFAULDIFFUSEACCURACYTHRESHOLD, 
//				cLightHelper::DEFAULTINFINITEDISTANCE, 
//				::g_vecLights[::g_SelectedLightID].attenConst,
//				::g_vecLights[::g_SelectedLightID].attenLinear, 
//				::g_vecLights[::g_SelectedLightID].attenQuad );
//		::g_pTheLightAttenMesh[1]->scale = distance;
//
//		::g_pTheLightAttenMesh[1]->bDontLightObject = true;
//		::g_pTheLightAttenMesh[1]->bUseColourAlphaValue = true;
//		::g_pTheLightAttenMesh[1]->colourSource = cMeshObject::USE_OBJECT_COLOUR;
//		::g_pTheLightAttenMesh[1]->colour = glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::LIGHT_SALMON ), 1.0f);
//
//
//		// Draw sphere 0 at 90% brightness
//		distance = theLightHelper.calcApproxDistFromAtten
//			( 0.90f,   
//				cLightHelper::DEFAULDIFFUSEACCURACYTHRESHOLD, 
//				cLightHelper::DEFAULTINFINITEDISTANCE, 
//				::g_vecLights[::g_SelectedLightID].attenConst,
//				::g_vecLights[::g_SelectedLightID].attenLinear, 
//				::g_vecLights[::g_SelectedLightID].attenQuad );
//
//		::g_pTheLightAttenMesh[2]->scale = distance;
//
//		::g_pTheLightAttenMesh[2]->bDontLightObject = true;
//		::g_pTheLightAttenMesh[2]->bUseColourAlphaValue = true;
//		::g_pTheLightAttenMesh[2]->colourSource = cMeshObject::USE_OBJECT_COLOUR;
//		::g_pTheLightAttenMesh[2]->colour = glm::vec4( CGLColourHelper::getInstance()->getColourRGB( CGLColourHelper::CYAN ), 1.0f);
//
//	}//if ( ::g_bTurnOnDebugLightSpheres )
//	else
//	{
//		// Don't Draw the light spheres
//		::g_pTheLightAttenMesh[0]->bIsVisible  = 
//		::g_pTheLightAttenMesh[1]->bIsVisible = 
//		::g_pTheLightAttenMesh[2]->bIsVisible = false;
//		::g_pTheLightMesh->bIsVisible = false;
//	}//if ( ::g_bTurnOnDebugLightSpheres )
//
//
//	return;
//}
