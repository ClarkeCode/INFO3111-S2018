#include "cLight.h"

//static 
const float cLight::MAX_ATTENUATION = 100.0f;

cLight::cLight()
{
	this->position = glm::vec3(0.0f);
	this->direction = glm::vec3(0.0f);

	this->diffuse = glm::vec3(0.0f);
	this->specular = glm::vec3(0.0f);
	this->specularPower = 0.0f;

	this->attenConst = 0.0f;
	this->attenLinear = 0.1f;
	this->attenQuad = 0.1f;

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

