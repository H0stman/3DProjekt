#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
	XMFLOAT3 position;
	XMVECTOR updirection;
	XMMATRIX viewmatrix;
	XMMATRIX projectionmatrix;
	

public:
	Camera();


};

