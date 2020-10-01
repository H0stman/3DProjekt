#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
	XMFLOAT3 position;
	XMVECTOR updirection;
	XMMATRIX viewmatrix, projectionmatrix;
	float pitch, yaw;

public:
	Camera();
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	XMFLOAT3 GetPosition();

};

