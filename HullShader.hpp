#pragma once
#include "IShader.hpp"

class HullShader : public IShader
{
private:
	ID3D11HullShader *hullshader;
private:
	HullShader(const HullShader& origShader) = delete;
	HullShader& operator=(const HullShader& origShader) = delete;
public:
	HullShader() = default;
	virtual ~HullShader() = default;
	virtual const bool Initialize(Shader_Setup_Details &setup);

	virtual void SetShader() const;
	virtual void UnSetShader() const override;
};
