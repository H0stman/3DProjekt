#pragma once
#include <Windows.h>
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
	XMVECTOR updirection, lookat, position;
	XMMATRIX view, projection;
	FLOAT pitch, yaw, rotation, movement;

public:
	Camera();
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	XMVECTOR GetPosition();
	VOID SetOrthographicProjection();
	VOID SetPerspectiveProjection();
	VOID Update();
};

