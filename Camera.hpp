#pragma once
#include <Windows.h>
#include <DirectXTK/Mouse.h>
#include <DirectXTK/Keyboard.h>
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
	XMVECTOR updirection, lookat, position, forward;
	XMMATRIX view, projection, orthographic;
	FLOAT pitch, yaw, rotation, movement;

public:
	Camera();
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	XMMATRIX GetOrthoMatrix();
	XMVECTOR GetPosition();
	VOID Update();
};

