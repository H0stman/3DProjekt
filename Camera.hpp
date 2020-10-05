#pragma once
#include <DirectXMath.h>
#include "Engine.hpp"


using namespace DirectX;

class Camera
{
	XMFLOAT3 position;
	XMVECTOR updirection;
	XMMATRIX view, projection;
	FLOAT pitch, yaw;

public:
	Camera();
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	XMFLOAT3 GetPosition();
	VOID SetOrthographicProjection();
	VOID SetPerspectiveProjection();
};

