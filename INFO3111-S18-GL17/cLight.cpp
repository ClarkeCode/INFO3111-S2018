#include "cLight.h"

//static 
const float cLight::MAX_ATTENUATION = 100.0f;

cLight::cLight()
{
	this->position = glm::vec3(0.0f);
	this->direction = glm::vec3(0.0f);

	this->diffuse = glm::vec3(0.0f);
	//this->ambient = glm::vec3(0.0f);
	//this->specular = glm::vec3(0.0f);
	//this->specularPower = 0.0f;

	this->attenConst = 0.0f;
	this->attenLinear = 0.1f;
	this->attenQuad = 0.1f;
	this->distanceCutOff = FLT_MAX;

	this->position = glm::vec3(0.0f);
	this->direction = glm::vec3(0.0f);

	this->spotConeAngleInner = 0.0f;
	this->spotConeAngleOuter = 0.0f;


	this->TurnLightOff();

	return;
}

// These set the w (4th) value of the diffuse component 
void cLight::TurnLightOff(void)
{
	this->m_bIsOn = false;
	return;
}

void cLight::TurnLightOn(void)
{
	this->m_bIsOn = true;
	return;
}

bool cLight::IsOn(void)
{
	return this->m_bIsOn;
}

cLight::eLightType cLight::getLightType(void)
{
	return this->m_lightType;
}

std::string cLight::getLightTypeString(void)
{
	switch( this->m_lightType )
	{
	case cLight::DIRECTIONAL_LIGHT:
		return "DIRECTIONAL_LIGHT";
		break;
	case cLight::POINT_LIGHT:
		return "POINT_LIGHT";
		break;
	case cLight::SPOT_LIGHT:
		return "SPOT_LIGHT";
		break;
	default:
		// Should never happen
		return "UNKNOWN_LIGHT_TYPE";
	};
	// Should never happen
	return "UNKNOWN_LIGHT_TYPE";
}

void cLight::SetLightType( cLight::eLightType type )
{
	this->m_lightType = type;
	return;
}

void cLight::setLightColour( const cLight::sLightColourDesc &lightColour )
{
	this->diffuse = lightColour.diffuse;
	//this->ambient = lightColour.ambient;
	//this->specular = lightColour.specular;
	//this->shininess = lightColour.shininess;
	return;
}

void cLight::getLightColour( cLight::sLightColourDesc &lightColour )
{
	lightColour.diffuse = this->diffuse;
	//lightColour.ambient = this->ambient;
	//lightColour.specular = this->specular;
	//lightColour.shininess = this->shininess;
	return;
}

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

void cLight::setLightAtten( const sLightAtten &lightAtten )
{
	this->attenConst = lightAtten.attenConst;
	this->attenLinear = lightAtten.attenLinear;
	this->attenQuad = lightAtten.attenQuad;
	this->distanceCutOff = lightAtten.distanceCutOff;
	return;
}

void cLight::getLightAtten ( sLightAtten &lightAtten )
{
	lightAtten.attenConst = this->attenConst;
	lightAtten.attenLinear = this->attenLinear;
	lightAtten.attenQuad = this->attenQuad;
	lightAtten.distanceCutOff = this->distanceCutOff;
	return;
}


//    ___     _     _     _    _      _   _    
//   | _ \___(_)_ _| |_  | |  (_)__ _| |_| |_  
//   |  _/ _ \ | ' \  _| | |__| / _` | ' \  _| 
//   |_| \___/_|_||_\__| |____|_\__, |_||_\__| 
//                              |___/          
void cLight::SetAsPoint(void)
{
	this->m_lightType = cLight::POINT_LIGHT;
	return;
}

void cLight::SetAsPoint( glm::vec3 position )
{
	this->m_lightType = cLight::POINT_LIGHT;
	this->position = position;
	return;
}


void cLight::SetAsPoint( const sPointLightDesc &lightDesc )
{
	this->position = lightDesc.position;

	this->attenConst = lightDesc.attenuation.attenConst;
	this->attenLinear = lightDesc.attenuation.attenLinear;
	this->attenQuad = lightDesc.attenuation.attenQuad;

	this->distanceCutOff = lightDesc.attenuation.distanceCutOff;

	this->diffuse = lightDesc.colour.diffuse;
	//this->ambient = lightDesc.colour.ambient;
	//this->specular = lightDesc.colour.specular;
	//this->specularPower = lightDesc.colour.shininess;

	return;
}


void cLight::getPointLightDesc( sPointLightDesc &lightDesc )
{
	lightDesc.position = this->position;

	lightDesc.attenuation.attenConst = this->attenConst;
	lightDesc.attenuation.attenLinear = this->attenLinear;
	lightDesc.attenuation.attenQuad	= this->attenQuad;

	lightDesc.attenuation.distanceCutOff = this->distanceCutOff;

	lightDesc.colour.diffuse = this->diffuse;
	//lightDesc.colour.ambient = this->ambient;
	//lightDesc.colour.specular = this->specular;
	//lightDesc.colour.shininess = this->specularPower;

	return;
}


//    ___           _     _    _      _   _    
//   / __|_ __  ___| |_  | |  (_)__ _| |_| |_  
//   \__ \ '_ \/ _ \  _| | |__| / _` | ' \  _| 
//   |___/ .__/\___/\__| |____|_\__, |_||_\__| 
//       |_|                    |___/          
void cLight::SetAsSpot(void)
{
	this->m_lightType = cLight::SPOT_LIGHT;
	return;
}

void cLight::SetAsSpot( glm::vec3 position, glm::vec3 direction, 
                        float innerConeAngle, float outerConeAngle )
{
	this->m_lightType = cLight::SPOT_LIGHT;

	this->position = position;
	this->direction = direction;
	this->spotConeAngleInner = innerConeAngle;
	this->spotConeAngleOuter = outerConeAngle;

	return;
}

//    ___  _            _   _               _   _    _      _   _    
//   |   \(_)_ _ ___ __| |_(_)___ _ _  __ _| | | |  (_)__ _| |_| |_  
//   | |) | | '_/ -_) _|  _| / _ \ ' \/ _` | | | |__| / _` | ' \  _| 
//   |___/|_|_| \___\__|\__|_\___/_||_\__,_|_| |____|_\__, |_||_\__| 
//                                                    |___/          

void cLight::SetAsDirectional(void)
{
	this->m_lightType = cLight::DIRECTIONAL_LIGHT;
	return;
}

void cLight::SetAsDirectional( glm::vec3 direction )
{
	this->m_lightType = cLight::DIRECTIONAL_LIGHT;
	this->direction = direction;
	return;
}

