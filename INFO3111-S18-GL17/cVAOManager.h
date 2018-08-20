#ifndef _cVAOManager_HG_
#define _cVAOManager_HG_

// Will load the models and place them 
// into the vertex and index buffers to be drawn

#include <string>
#include <map>
#include <vector>

// The vertex structure 
//	that's ON THE GPU (eventually) 
// So dictated from THE SHADER
struct sVert
{
	float x, y, z, w;			// vPosition
	float red, green, blue, alpha;		// vColourRGBA
	float nx, ny, nz, n_discard;		// vNormal
	float u1, v1, u2, v2;	// vTexUV
	sVert() : 
		x(0.0f), y(0.0f), z(0.0f), w(0.0f), 
		red(0.0f), green(0.0f), blue(0.0f), alpha(0.0f),
		nx(0.0f), ny(0.0f), nz(0.0f), n_discard(0.0f),
		u1(0.f), v1(0.0f), u2(0.0f), v2(0.0f) 
	{};
};


struct sModelDrawInfo
{
	sModelDrawInfo(); 

	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	// The "local" (i.e. "CPU side" temporary array)
	sVert* pVertices;	//  = 0;
	// The index buffer (CPU side)
	unsigned int* pIndices;

	// You could store the max and min values of the 
	//  vertices here (determined when you load them):
	float maxX, maxY, maxZ;
	float minX, minY, minZ;
	float extentX, extentY, extentZ;
	float maxExtent;

};


class cVAOManager
{
public:

	bool LoadModelIntoVAO(std::string fileName, 
						  sModelDrawInfo &drawInfo, 
						  unsigned int shaderProgramID);

	// This uses the more flexible (and faster and very old) nPlyFile5t class
	// The method is deliberately different to make sure we don't accidentally
	//	call the "old" method.
	struct sLoadParamsINFO3111S2018
	{
		// Default is generate everything if not present
		sLoadParamsINFO3111S2018()
		{
			this->Clear();
			return;
		};
		sLoadParamsINFO3111S2018(std::string modelFile)
		{
			this->Clear();
			this->modelFileToLoad = modelFile;
			return;
		};
		void Clear(void)
		{
			this->ifAbsent_GenerateNormals = true;
			this->Force_Normal_Regeneration = false;
			this->textureCoordGenerationMode = sLoadParamsINFO3111S2018::GENERATE_UVs_IF_NOT_PRESENT;
			this->textureCoordGenerationType = sLoadParamsINFO3111S2018::SPHERICAL_UV;
			this->textureGenerationScale = 1.0f;
			this->ifAbsent_GenerateRGB = true;
			this->RGB_red = 1.0f;
			this->RGB_green = 1.0f;
			this->RGB_blue = 1.0f;
			this->ifAbsent_GenerateAlpha = true;
			this->RGB_alpha = 1.0f;
			this->bLoadedOK = true;
			return;
		}
		// Added.
		std::string modelFileToLoad;
		std::string loadErrors;			// Holds loading errors (if any)
		bool bLoadedOK;					// true if it's in the VAO

		bool ifAbsent_GenerateNormals;
		bool Force_Normal_Regeneration;

		enum eUVGenerationMode
		{
			DONT_GENERATE_UVs,
			FORCE_UV_GENERATION,
			GENERATE_UVs_IF_NOT_PRESENT		// Default
		};
		eUVGenerationMode textureCoordGenerationMode;	// = GENERATE_UVs_IF_NOT_PRESENT

		enum eUVGenerationType
		{
			SPHERICAL_UV,		// Default
			PLANAR_XY, PLANAR_XZ, PLANAR_YZ,
			PLANAR_ON_WIDEST_AXES	// If object is 'flat', it will pick that planar mode
		};
		eUVGenerationType textureCoordGenerationType;		// = SPHERICAL_UV
		float textureGenerationScale;		// default is 1.0 (so 0.0 to 1.0)

		bool ifAbsent_GenerateRGB;
		bool ifAbsent_GenerateAlpha;
		float RGB_red;
		float RGB_green;
		float RGB_blue;
		float RGB_alpha;
	};

	bool LoadModelInfoVAO_PlyFile5t( std::string fileName, 
									 unsigned int shaderProgramID, 
									 sModelDrawInfo &drawInfo, 
									 std::string &errors,
									 sLoadParamsINFO3111S2018 loadParams = sLoadParamsINFO3111S2018() );
	// This allows you to load multiple models. 
	// The file to load (and the errors) are passed through sLoadParamsINFO3111S2018.
	// NOTE: Will return false if ANY file didn't load. 
	// (If that's the case, then check errors in the sLoadParamsINFO3111S2018 vector.)
	bool LoadModelsInfoVAO_PlyFile5t( unsigned int shaderProgramID, 
									  std::vector<sModelDrawInfo> &vecDrawInfo, 
									  std::vector<sLoadParamsINFO3111S2018> &vecModelsToLoad );

	bool LoadPlyThenSaveAsGDPFile( std::string plyFileName, std::string GDPFileName );

	// We don't want to return an int, likely
	bool FindDrawInfoByModelName(std::string filename,
								 sModelDrawInfo &drawInfo);

	std::string getLastError(bool bAndClear = true);

	void ShutDown(void);

private:

	std::map< std::string /*model name*/,
		      sModelDrawInfo /* info needed to draw*/ >
		m_map_ModelName_to_VAOID;

	// Loads the ply model file into a temporary array
	bool m_LoadTheModel( std::string fileName, 
						 sModelDrawInfo &drawInfo);

	bool m_LoadTheModel_PlyFile5t( std::string fileName, 
	                               unsigned int shaderProgramID, 
	                               sModelDrawInfo &drawInfo, 
	                               std::string &errors,
	                               const sLoadParamsINFO3111S2018 &loadParams );

	// Takes a sModelDrawInfo and loads it into the VAO
	bool m_Load_ModelDrawInfo_IntoVAO( std::string fileName, 
	                                   sModelDrawInfo &drawInfo, 
	                                   unsigned int shaderProgramID );


	std::string m_lastErrorString;
	void m_AppendTextToLastError(std::string text, bool addNewLineBefore = true);
};

#endif	// _cVAOManager_HG_
