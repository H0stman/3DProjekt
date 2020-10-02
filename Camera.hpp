#pragma once
#include <DirectXMath.h>
#include "Window.hpp"

using namespace DirectX;

class Camera
{
	XMFLOAT3 position;
	XMVECTOR updirection;
	XMMATRIX view, projection, orthographic, perspective;
	float pitch, yaw;

public:
	Camera(INT windowWidth, INT windowHeight);
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	XMFLOAT3 GetPosition();

};

