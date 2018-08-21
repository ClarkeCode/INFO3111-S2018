#include "globalStuff.h"

#include <glm/glm.hpp>
#include <sstream>
#include <iostream>
#include <fstream>

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

std::string partialMeshSerialize (cMeshObject* curMesh) {
	using namespace std;
	stringstream ss;
	string(*stringifyVec3)(glm::vec3 const&) = [](glm::vec3 const& get) -> string {
		stringstream ss; 
		ss << get.x << " " << get.y << " " << get.z;
		return ss.str();
	};
	string(*stringifyVec4)(glm::vec4 const&) = [](glm::vec4 const& get) -> string {
		stringstream ss;
		ss << get.x << " " << get.y << " " << get.z << " "<< get.w;
		return ss.str();
	};

	ss << "MESHNAME " << curMesh->meshName << endl;
	sModelDrawInfo modelInfo;
	::g_pTheVAOManager->FindDrawInfoByModelName(curMesh->meshName, modelInfo);
	//pCornerRoad->scale = 10.0f / modelInfo.maxExtent;
	ss << "TRUESCALE " << curMesh->scale * modelInfo.maxExtent << endl;
	ss << "DIFFUSECOLOUR " << stringifyVec4(curMesh->diffuseColour) << endl;

	ss << "AMBIENTDIFFUSE " << curMesh->ambientToDiffuseRatio << endl;
	ss << "SPECULAR-HLCOLOUR " << stringifyVec3(curMesh->specularHighlightColour) << endl;
	ss << "SHININESS " << curMesh->specularShininess << endl;
	

	ss << "WIREFRAME " << curMesh->isWireframe << endl;
	ss << "USE-NONUNIFORM " << curMesh->bUseNonUniformScaling << endl;
	ss << "NONUNIFORM-SCALING " << stringifyVec3(curMesh->nonUniformScale) << endl;

	ss << "ORIENTATION " << stringifyVec3(curMesh->orientation) << endl;

	ss << "DONTLIGHT " << curMesh->bDontLightObject << endl;
	ss << "ISVISIBLE " << curMesh->bIsVisible << endl;
	ss << "COLOURSOURCE " << curMesh->colourSource << endl;
	ss << "USECOLOUR-ALPHAVALUE " << curMesh->bUseColourAlphaValue << endl;
	ss << "ISSKYBOX " << curMesh->bIsSkyBoxObject << endl;

	for (int x = 0; x < curMesh->MAXNUMBEROFTEXTURES; x++) {
		ss << "TEXTURENAME " << (curMesh->textureNames[x] == "" ? "NOTEXTURE" : curMesh->textureNames[x]) << endl;
	}
	for (int x = 0; x < curMesh->MAXNUMBEROFTEXTURES; x++) {
		ss << "TEXTUREMIX " << curMesh->textureMixRatios[x] << endl;
	}

	ss << "VERTEXSOURCEMIXING " << curMesh->bEnableVertexSourceMixing << " " << curMesh->fVCS_FromVertex_Mix << " " << curMesh->fVCS_FromMesh_Mix << " " << curMesh->fVCS_FromTexture_Mix;

	return ss.str();
}

void SaveMeshInfoToFile () {
	using namespace std;
	std::ofstream myFile;
	myFile.open(g_meshInfoFileName);
	if (!myFile.is_open()) { std::cout << "Error saving mesh info" << std::endl; return; };

	string(*stringifyVec3)(glm::vec3 const&) = [](glm::vec3 const& get) -> string {
		stringstream ss;
		ss << get.x << " " << get.y << " " << get.z;
		return ss.str();
	};
	double timer = glfwGetTime();
	int modelsSaved = 0;
	for (cMeshObject* curMesh : g_vec_pMeshObjects) {
		myFile << "MESHSTART INDEPENDANT" << endl;
		myFile << "FNAME " << (curMesh->friendlyName == "" ? "NONAME" : curMesh->friendlyName) << endl;
		myFile << "POSITION " << stringifyVec3(curMesh->pos) << endl;
		myFile << partialMeshSerialize(curMesh) << endl;
		myFile << "MESHEND" << endl << endl;
		modelsSaved++;

		for (cMeshObject* child : curMesh->vec_pChildObjects) {
			myFile << "MESHSTART DEPENDANT " << (curMesh->friendlyName == "" ? "NONAME" : curMesh->friendlyName) << endl;
			myFile << "FNAME " << (child->friendlyName == "" ? "" + child->uniqueID : child->friendlyName) << endl;
			//glm::vec3 relPos = curMesh->pos + child->pos;
			myFile << "RELPOSITION " << stringifyVec3(child->pos) << endl;
			myFile << partialMeshSerialize(child) << endl;
			myFile << "MESHEND" << endl << endl;
			modelsSaved++;
		}
	}
	myFile.close();
	cout << "Saved information of " << modelsSaved << " object meshes in " << glfwGetTime() - timer << " seconds ";
	cout << "to file '" << g_meshInfoFileName << "'" << endl;
}

void LoadMeshInfoFromFile () {
	{
		// The "debug sphere" that replaces all of the other spheres for the lights, etc.
		{// Add an object into the "scene"
			::g_pDebugSphere = new cMeshObject();

			::g_pDebugSphere->meshName = "isosphere_smooth_xyz_n_rgba_uv.ply";

			::g_pDebugSphere->pos = glm::vec3(0.0f, 0.0f, 0.0f);
			::g_pDebugSphere->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			// Get the "unit" scale from the VAOManager
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(::g_pDebugSphere->meshName,
				modelInfo);
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

			::g_pDebugSphereSmall->pos = glm::vec3(0.0f, 0.0f, 0.0f);
			::g_pDebugSphereSmall->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			// Get the "unit" scale from the VAOManager
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(::g_pDebugSphereSmall->meshName,
				modelInfo);
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

			::g_pDebugCone->pos = glm::vec3(0.0f, 0.0f, 0.0f);
			::g_pDebugCone->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			// Get the "unit" scale from the VAOManager
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(::g_pDebugCone->meshName,
				modelInfo);
			//::g_pTheLightMesh->scale = 0.1f;
			::g_pDebugCone->scale = 1.0f / modelInfo.maxExtent;

			::g_pDebugCone->isWireframe = false;

			::g_pDebugCone->bDontLightObject = true;

			// DON'T put this into the "scene" object (the "vector of things to draw")
			//::g_vec_pMeshObjects.push_back( ::g_pTheLightMesh );
		}
	}

	using namespace std;
	ifstream myFile;
	myFile.open(g_meshInfoFileName);
	if (!myFile.is_open()) { std::cout << "Error saving mesh info" << std::endl; return; };

	glm::vec3(*unloadVec3)(stringstream& ss) = [](stringstream& ss) -> glm::vec3 {
		float a, b, c; ss >> a >> b >> c; return glm::vec3(a, b, c);
	};
	glm::vec4(*unloadVec4)(stringstream& ss) = [](stringstream& ss) -> glm::vec4 {
		float a, b, c, d; ss >> a >> b >> c >> d; return glm::vec4(a, b, c, d);
	};

	double timer = glfwGetTime();
	int modelsLoaded = 0;
	cout << "Loading information from file '" << g_meshInfoFileName << "': ";

	cMeshObject* curMesh;
	string tempLine;
	bool isDependant = false;
	string dependantName = "";
	int texNameCounter, texMixCounter;
	texNameCounter = 0;
	texMixCounter = 0;
	while (getline(myFile, tempLine)) {
		stringstream ss(tempLine);
		if (tempLine.empty() ||(tempLine.size() >= 2 && tempLine[0] == '/' && tempLine[1] == '/')) continue;
		//ignore any lines that start with // or are empty

		string id;
		ss >> id;

		if (id == "MESHSTART") {
			curMesh = new cMeshObject();
			string dependant;
			ss >> dependant;
			texNameCounter = 0;
			texMixCounter = 0;
			if (dependant == "DEPENDANT") {
				ss >> dependantName;
				isDependant = true;
			}
		}
		if (id == "FNAME") { ss >> curMesh->friendlyName; }
		if (id == "POSITION") { curMesh->pos = unloadVec3(ss); }
		if (id == "RELPOSITION") { curMesh->pos = //g_pFindObjectByFriendlyName(dependantName)->pos + 
			unloadVec3(ss); }
		if (id == "MESHNAME") { //ss >> 
			string remainder = ss.str();
			curMesh->meshName = remainder.erase(0,9); 
		}
		if (id == "TRUESCALE") {
			sModelDrawInfo modelInfo;
			::g_pTheVAOManager->FindDrawInfoByModelName(curMesh->meshName, modelInfo);
			ss >> curMesh->scale;
			curMesh->scale /= modelInfo.maxExtent;
		}
		if (id == "DIFFUSECOLOUR") { curMesh->diffuseColour = unloadVec4(ss); }

		if (id == "AMBIENTDIFFUSE") { ss >> curMesh->ambientToDiffuseRatio; }
		if (id == "SPECULAR-HLCOLOUR") { curMesh->specularHighlightColour = unloadVec3(ss); }

		if (id == "WIREFRAME") { ss >> curMesh->isWireframe; }
		if (id == "USE-NONUNIFORM") { ss >> curMesh->bUseNonUniformScaling; }
		if (id == "NONUNIFORM-SCALING") { curMesh->nonUniformScale = unloadVec3(ss); }

		if (id == "ORIENTATION") { curMesh->orientation = unloadVec3(ss); }

		if (id == "DONTLIGHT") { ss >> curMesh->bDontLightObject; }
		if (id == "ISVISIBLE") { ss >> curMesh->bIsVisible; }
		if (id == "COLOURSOURCE") { int temp; ss >> temp; curMesh->colourSource = (cMeshObject::eColourSource)temp; }
		if (id == "USECOLOUR-ALPHAVALUE") { ss >> curMesh->bUseColourAlphaValue; }
		if (id == "ISSKYBOX") { ss >> curMesh->bIsSkyBoxObject; }

		if (id == "TEXTURENAME") {
			string temp;
			ss >> temp;
			curMesh->textureNames[texNameCounter] = temp == "NOTEXTURE" ? "" : temp;
			texNameCounter++;
		}
		if (id == "TEXTUREMIX") { ss >> curMesh->textureMixRatios[texMixCounter]; texMixCounter++; }
		if (id == "VERTEXSOURCEMIXING") { ss >> curMesh->bEnableVertexSourceMixing >> curMesh->fVCS_FromVertex_Mix >> curMesh->fVCS_FromMesh_Mix >> curMesh->fVCS_FromTexture_Mix; }

		if (id == "MESHEND") {
			if (isDependant) { g_pFindObjectByFriendlyName(dependantName)->vec_pChildObjects.push_back(curMesh); }
			else			 { ::g_vec_pMeshObjects.push_back(curMesh); }
			isDependant = false;
			dependantName = "";
			modelsLoaded++;
		}
	}
	cout << modelsLoaded << " objects in " << glfwGetTime() - timer << " seconds" << endl;
}

void ClearLoadMesh() {
	g_vec_pMeshObjects.clear();
	LoadMeshInfoFromFile();
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
