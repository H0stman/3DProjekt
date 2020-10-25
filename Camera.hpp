#pragma once
#include <Windows.h>
#include <DirectXTK/Mouse.h>
#include <DirectXTK/Keyboard.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;

class Camera
{
	XMVECTOR updirection, lookat, position, forward;
	XMMATRIX view, projection, orthographic;
	FLOAT pitch, yaw, rotation, movement, fpitch, fyaw;
	BoundingFrustum frustum;

public:
	Camera();
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	XMMATRIX GetOrthoMatrix();
	XMVECTOR GetPosition();
	XMVECTOR GetForward();
	VOID Update();
	VOID Update(XMVECTOR parentpos, XMVECTOR parentforward);
	BoundingFrustum GetFrustum();
	VOID SetPositionY(float value);
};

