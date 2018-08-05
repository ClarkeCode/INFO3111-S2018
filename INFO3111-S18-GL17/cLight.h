#ifndef _sLight_HG_
#define _sLight_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <float.h>			// FLT_MAX
#include <string>

// This will get more involved 
// (changed to a class as it's now got a lot of methods.
//  It used to have basically data, so was a struct.)
// (class and struct are the same thing)
class cLight
{
public:
	cLight();

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 diffuse;	
private:
	bool m_bIsOn;				// Passed to w param: w = 0 means ligth is off
public:
	// *******************************************************************
	// To simplify the lighting, we'll limit the specular and ambient 
	//  to the object only, rather than the light+object interaction

	//glm::vec3 ambient;	
	//void setAmbientFromDiffuse( float ambientToDiffuseRation = 0.2f )
	//{
	//	this->ambient = ambientToDiffuseRation * this->diffuse;
	//	return;
	//};		
	//glm::vec3 specular;	// rgb = colour, 
	//float specularPower;        // w = shininess (or specular 'power')
	//                            // (this value outside of the 0-1 range, 
	//                            //  and can get quite high, like 10,000)
	// *******************************************************************
	float attenConst;
	float attenLinear;
	float attenQuad;
	float distanceCutOff;

	float spotConeAngleInner;
	float spotConeAngleOuter;

	static const float MAX_ATTENUATION;// = 100.0f;

	// These set the w (4th) value of the diffuse component 
	bool IsOn(void);
	void TurnLightOff(void);
	void TurnLightOn(void);

	// The light types
	enum eLightType
	{
		POINT_LIGHT,
		SPOT_LIGHT,
		DIRECTIONAL_LIGHT
	};
private:
	eLightType m_lightType;

public:
	eLightType getLightType(void);
	std::string getLightTypeString(void);

	void SetLightType( eLightType type );

	// The different parts of the different lights
	//	are split up here. These structs are passed
	//  to some of the functions below. 
	// This allows us to change the type of light
	//	without sending everything each time
	struct sLightColourDesc
	{
		sLightColourDesc() : 
			diffuse( glm::vec3(0.0f) )/*, 
			ambient( glm::vec3(0.0f) ),
			specular( glm::vec3(0.0) ),
			shininess(0.0f)*/ {};
		sLightColourDesc( glm::vec3 diffuseColour ) :
			diffuse(diffuseColour) {};
		glm::vec3 diffuse;
		//glm::vec3 ambient;
		// helper function	
		//void setAmbientFromDiffuse( float ambientToDiffuseRation = 0.2f )
		//{
		//	this->ambient = ambientToDiffuseRation * this->diffuse;
		//	return;
		//};	
		//glm::vec3 specular;	// highlight colour
		//float shininess;	// (or specular 'power')
	};

	void setLightColour( const sLightColourDesc &lightColour );
	void getLightColour( sLightColourDesc &lightColour );

	struct sLightAtten
	{
		sLightAtten() : 
			attenConst(0.0f), attenLinear(0.0f), attenQuad(0.0f), distanceCutOff(FLT_MAX) {};
		sLightAtten( float constAtt, float linearAtt, float quadAtt ) : 
			attenConst(constAtt), attenLinear(linearAtt), attenQuad(quadAtt),
			distanceCutOff(FLT_MAX) {};
		float attenConst;
		float attenLinear;
		float attenQuad;
		float distanceCutOff;
	};

	void setLightAtten( const sLightAtten &lightAtten );
	void getLightAtten ( sLightAtten &lightAtten );


	struct sPointLightDesc
	{
		sPointLightDesc() : 
			position( glm::vec3(0.0f) ) {};
		glm::vec3 position;
		// Same as any light
		sLightColourDesc colour;
		sLightAtten attenuation;
	};
	void SetAsPoint(void);	// Assumes position has been set
	void SetAsPoint( glm::vec3 position );
	void SetAsPoint( const sPointLightDesc &lightDesc );
	void getPointLightDesc( sPointLightDesc &lightDesc );
	
	struct sSpotLightDesc
	{
		sSpotLightDesc() : 
			position( glm::vec3(0.0f) ),
			direction( glm::vec3(0.0f) ),
			innerConeAngle(0.0f), outerConeAngle(0.0f) {};
		glm::vec3 position;
		glm::vec3 direction;
		float innerConeAngle;
		float outerConeAngle;
		// Same as any light
		sLightColourDesc colour;
		sLightAtten attenuation;
	};
	void SetAsSpot(void);	// Assumes spot specific params have been set
	void SetAsSpot( glm::vec3 position, glm::vec3 direction, 
					float innerConeAngle, float outerConeAngle );
	void SetAsSpot( const sSpotLightDesc &lightDesc );
	void getSpotLightDesc( sSpotLightDesc &lightDesc );

	struct sDirectionalLightDesc
	{
		sDirectionalLightDesc() : 
			direction( glm::vec3(0.0f) ) {};
		glm::vec3 direction;

		// Same as any light
		sLightColourDesc colour;
		sLightAtten attenuation;
	};
	void SetAsDirectional(void);	// Assumes direction has been set
	void SetAsDirectional( glm::vec3 direction );
	void SetAsDirectional( const sDirectionalLightDesc &lightDesc );
	void getDirectinoalDesc( sDirectionalLightDesc &lightDesc );

};



#endif
