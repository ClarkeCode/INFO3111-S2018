#include "cVAOManager.h"

#include "Ply/CPlyFile5nt.h"

std::string FILELOCAL_StripPath( std::string fileAndPath  );
std::wstring FILELOCAL_ASCII_to_Unicode_BASIC(std::string ASCIIString);
std::string FILELOCAL_Unicode_to_ASCII_BASIC(std::wstring UnicodeString);


bool cVAOManager::LoadModelInfoVAO_PlyFile5t( std::string fileName, 
									unsigned int shaderProgramID, 
									sModelDrawInfo &drawInfo, 
									std::string &errors,
									cVAOManager::sLoadParamsINFO3111S2018 loadParams /*=sLoadParamsINFO3111S2018()*/ )
{
	if ( ! this->m_LoadTheModel_PlyFile5t( fileName, shaderProgramID, drawInfo, errors, loadParams ) )
	{
		this->m_AppendTextToLastError( errors );
		return false;
	}

	return this->m_Load_ModelDrawInfo_IntoVAO( fileName, drawInfo, shaderProgramID );
}


bool cVAOManager::m_LoadTheModel_PlyFile5t( std::string fileName, 
                                            unsigned int shaderProgramID, 
                                            sModelDrawInfo &drawInfo, 
                                            std::string &errors,
                                            const sLoadParamsINFO3111S2018 &loadParams )
{
	CPlyFile5nt thePlyFile;

	std::wstring error;
	std::wstring wFileName = FILELOCAL_ASCII_to_Unicode_BASIC( fileName );
	if ( ! thePlyFile.OpenPLYFile2( wFileName, error ) )
	{
		errors = FILELOCAL_Unicode_to_ASCII_BASIC( error );
		return false;
	}

	// file is openned.
	if ( loadParams.Force_Normal_Regeneration )
	{
		thePlyFile.normalizeTheModelBaby();
	}
	else if ( ( ! thePlyFile.bHasNormalsInFile() ) && loadParams.ifAbsent_GenerateNormals )
	{
		thePlyFile.normalizeTheModelBaby();
	}//if (loadParams.Force_Normal_Regeneration...

	if ( loadParams.Force_UV_Regeneration )
	{
		// Here the "scale" is the UV repeat value.
		// 1.0 --> UVs-STs go from 0.0 to 1.0
		// 2.0 --> 0.0 to 2.0;
		// 4.0 --> 0.0 to 4.0;
		thePlyFile.GenTextureCoordsSpherical( CPlyFile5nt::enumTEXCOORDBIAS::POSITIVE_X, 
											  CPlyFile5nt::enumTEXCOORDBIAS::POSITIVE_Y, 
											  false,	// Based on normals
											  1.0,		// Scale
											  false );	// Faster
	}
	else if ( ( ! thePlyFile.bHasTextureCoordinatesInFile() ) && loadParams.ifAbsent_GenerateUVs )
	{
		thePlyFile.GenTextureCoordsSpherical( CPlyFile5nt::enumTEXCOORDBIAS::POSITIVE_X, 
											  CPlyFile5nt::enumTEXCOORDBIAS::POSITIVE_Y, 
											  false,	// Based on normals
											  1.0,		// Scale
											  false );	// Faster
	}//if(loadParams.Force_UV_Regeneration...
	
	drawInfo.meshName = fileName;

	drawInfo.numberOfVertices = thePlyFile.GetNumberOfVerticies();
	drawInfo.numberOfTriangles = thePlyFile.GetNumberOfElements();
	// We only load triangles, so x3
	drawInfo.numberOfIndices = thePlyFile.GetNumberOfElements() * 3;

	drawInfo.minX = thePlyFile.getMinX();
	drawInfo.minY = thePlyFile.getMinY();
	drawInfo.minZ = thePlyFile.getMinZ();
	drawInfo.maxX = thePlyFile.getMaxX();
	drawInfo.maxY = thePlyFile.getMaxY();
	drawInfo.maxZ = thePlyFile.getMaxZ();
	drawInfo.extentX = thePlyFile.getDeltaX();
	drawInfo.extentY = thePlyFile.getDeltaY();
	drawInfo.extentZ = thePlyFile.getDeltaZ();
	drawInfo.maxExtent = thePlyFile.getMaxExtent();


	// Create a local vertex array
	drawInfo.pVertices = new sVert[drawInfo.numberOfVertices];

	// Copy data from ply loader to this format
	// Why? Because the format of the ply loader is NOT the same
	//	as the drawInfo vertex array (which matches the SHADER).
	for ( unsigned int vertIndex = 0; vertIndex != thePlyFile.GetNumberOfVerticies(); vertIndex++ )
	{
		PlyVertex curVertex = thePlyFile.getVertex_at(vertIndex);

		drawInfo.pVertices[vertIndex].x = curVertex.xyz.x;
		drawInfo.pVertices[vertIndex].y = curVertex.xyz.y;
		drawInfo.pVertices[vertIndex].z = curVertex.xyz.z;
		drawInfo.pVertices[vertIndex].w = 1.0f;

		drawInfo.pVertices[vertIndex].nx = curVertex.nx;
		drawInfo.pVertices[vertIndex].ny = curVertex.ny;
		drawInfo.pVertices[vertIndex].nz = curVertex.nz;
		drawInfo.pVertices[vertIndex].n_discard = 1.0f;

		drawInfo.pVertices[vertIndex].u1 = curVertex.tex0u;
		drawInfo.pVertices[vertIndex].v1 = curVertex.tex0v;
		// We're not using the 2nd set of texture coordinates
		drawInfo.pVertices[vertIndex].u2 = 0.0f;
		drawInfo.pVertices[vertIndex].v2 = 0.0f;
		
		if ( ! thePlyFile.bHasRGBAColoursInFile() ) 
		{
			if ( loadParams.ifAbsent_GenerateRGB )
			{
				drawInfo.pVertices[vertIndex].red = loadParams.RGB_red;
				drawInfo.pVertices[vertIndex].green = loadParams.RGB_green;
				drawInfo.pVertices[vertIndex].blue = loadParams.RGB_blue;
			}
			if ( loadParams.ifAbsent_GenerateAlpha )
			{
				drawInfo.pVertices[vertIndex].alpha = loadParams.RGB_alpha;
			}
		}//if (!thePlyFile.bHasRGBAColoursInFile()) 

	}//for ( unsigned int vertIndex = 0


	// Do the same for the index (triangle or element) values

	drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];

	unsigned int indexBufferIndex = 0;
	for ( unsigned int triIndex = 0; 
		  triIndex != drawInfo.numberOfTriangles;		// ::g_NumberOfTriangles; 
		  triIndex++, indexBufferIndex += 3 )
	{
		PlyElement curElement = thePlyFile.getElement_at( triIndex );

		// This is taking an array of triangles vertex indices 
		// (which is a 2D array of indices, really) and converting
		//  it into a 1D array of indices...)
		drawInfo.pIndices[indexBufferIndex + 0] = curElement.vertex_index_1;
		drawInfo.pIndices[indexBufferIndex + 1] = curElement.vertex_index_2;
		drawInfo.pIndices[indexBufferIndex + 2] = curElement.vertex_index_3;

	}// for ( unsigned int triIndex = 0...


	return true;
}


bool cVAOManager::LoadPlyThenSaveAsGDPFile( std::string plyFileName, std::string GDPFileName )
{
	// TODO: This.


	return true;
}


std::string FILELOCAL_StripPath( std::string fileAndPath  )
{
	std::stringstream ssFileName;

	// Scan until you get to a forward and reverse slash
	bool bKeepScanning = true;
	for ( std::string::reverse_iterator itChar = fileAndPath.rbegin();
		  (itChar != fileAndPath.rend() && bKeepScanning); itChar++ )
	{
		if ( (*itChar == '/') || (*itChar == '\\') )
		{
			bKeepScanning = false;
		}
		else
		{
			ssFileName << *itChar;
		}
	}

	std::string fileName = ssFileName.str();
	std::reverse( fileName.begin(), fileName.end() );

	return fileName;
}

// NOTE: This is NOT a great way to do this UNLESS
//	you're sure that you'll only be using English... 
// Like it 'gets the job done', but there will be many issues
//  if you have to use this for actually international languages.
std::wstring FILELOCAL_ASCII_to_Unicode_BASIC(std::string ASCIIString)
{
	std::wstringstream ssUnicodeString;

	for ( int charIndex = 0; charIndex != ASCIIString.size(); charIndex++ )
	{
		wchar_t curChar = (wchar_t)ASCIIString[charIndex];
		ssUnicodeString << curChar;
	}

	return ssUnicodeString.str();
}

std::string FILELOCAL_Unicode_to_ASCII_BASIC(std::wstring UnicodeString)
{
	std::stringstream ssASCIIString;

	for ( int charIndex = 0; charIndex != UnicodeString.size(); charIndex++ )
	{
		// Here's where the real issues happen... 
		char curChar = (char)UnicodeString[charIndex];
		ssASCIIString << curChar;
	}

	return ssASCIIString.str();
}
