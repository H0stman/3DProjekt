#pragma once
#include <DirectXMath.h>
class PointLight
{
private:
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_DiffuseColor;
	float m_AttenuationConstant;
	float m_AttenuationLinear;
	float m_AttenuationQuadratic;
	float m_DiffuseIntensity;
	float m_SpecularIntensity;
private:

public:
	PointLight();
	virtual ~PointLight() = default;

	[[nodiscard]] const DirectX::XMFLOAT3& GetPosition() const;
	[[nodiscard]] const DirectX::XMFLOAT3& GetColor() const;
	[[nodiscard]] const float& GetAttenuationConstant() const;
	[[nodiscard]] const float& GetAttenuationLinear() const;
	[[nodiscard]] const float& GetAttenuationQuadratic() const;
	[[nodiscard]] const float& GetDiffuseIntensity() const;
	[[nodiscard]] const float& GetSpecularIntensity() const;
	[[nodiscard]] const DirectX::XMMATRIX GetViewMatrix() const;
	[[nodiscard]] const DirectX::XMMATRIX GetProjectionMatrix() const;

	void SetPosition(const float& x, const float& y, const float& z);
	void SetColor(const float& r, const float& g, const float& b);
	void SetAttenuationConstant(const float& attenuationConstant);
	void SetAttenuationLinear(const float& attenuationLinear);
	void SetAttenuationQuadratic(const float& attenuationQuadratic);
	void SetDiffuseIntensity(const float& diffuseIntensity);
	void SetSpecularIntensity(const float& specularIntensity);
};