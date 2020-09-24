#include "Camera.hpp"

Camera::Camera()
{
	position = XMFLOAT3(0.0f, 7.0f, 5.0f);
	updirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	viewmatrix = XMMatrixIdentity();
	projectionmatrix = XMMatrixPerspectiveFovLH(90.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
}
