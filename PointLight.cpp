#include "PointLight.h"

PointLight::PointLight()
	: m_Position{ DirectX::XMFLOAT3(32.0f, 30.0f, 50.0f) },
	m_DiffuseColor{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) },
	m_AttenuationConstant{ 1.0f },
	m_AttenuationLinear{ 0.045f },
	m_AttenuationQuadratic{ 0.0075f },
	m_DiffuseIntensity{ 1.0f },
	m_SpecularIntensity{ 0.9f }
{
}

const DirectX::XMFLOAT3& PointLight::GetPosition() const
{
	return m_Position;
}

const DirectX::XMFLOAT3& PointLight::GetColor() const
{
	return m_DiffuseColor;
}

const float& PointLight::GetAttenuationConstant() const
{
	return m_AttenuationConstant;
}

const float& PointLight::GetAttenuationLinear() const
{
	return m_AttenuationLinear;
}

const float& PointLight::GetAttenuationQuadratic() const
{
	return m_AttenuationQuadratic;
}

const float& PointLight::GetDiffuseIntensity() const
{
	return m_DiffuseIntensity;
}

const float& PointLight::GetSpecularIntensity() const
{
	return m_SpecularIntensity;
}

void PointLight::SetPosition(const float& x, const float& y, const float& z)
{
	m_Position = DirectX::XMFLOAT3(x, y, z);
}

void PointLight::SetColor(const float& r, const float& g, const float& b)
{
	m_DiffuseColor = DirectX::XMFLOAT3(r, g, b);
}

void PointLight::SetAttenuationConstant(const float& attenuationConstant)
{
	m_AttenuationConstant = attenuationConstant;
}

void PointLight::SetAttenuationLinear(const float& attenuationLinear)
{
	m_AttenuationLinear = attenuationLinear;
}

void PointLight::SetAttenuationQuadratic(const float& attenuationQuadratic)
{
	m_AttenuationQuadratic = attenuationQuadratic;
}

void PointLight::SetDiffuseIntensity(const float& diffuseIntensity)
{
	m_DiffuseIntensity = diffuseIntensity;
}

void PointLight::SetSpecularIntensity(const float& specularIntensity)
{
	m_SpecularIntensity = specularIntensity;
}