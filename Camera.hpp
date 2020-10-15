#pragma once
#include <Windows.h>
#include <DirectXTK/Mouse.h>
#include <DirectXTK/Keyboard.h>
#include <DirectXTK/SimpleMath.h>
#include <cmath>
#include <DirectXMath.h>

using namespace DirectX;
using namespace SimpleMath;

class Camera
{
	Vector3 updirection, lookat, position;
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

