#pragma once
#include <Windows.h>
#include <DirectXMath.h>

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

