#include "Camera.hpp"

Camera::Camera()
{
	position = XMFLOAT3(0.0f, 7.0f, 5.0f);
	updirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	viewmatrix = XMMatrixIdentity();
	projectionmatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.f), 16.0f / 9.0f, 0.1f, 1000.0f);
	pitch = 0.0f;
	yaw = 0.0f;
}

XMMATRIX Camera::GetViewMatrix()
{
	return viewmatrix;
}

XMMATRIX Camera::GetProjectionMatrix()
{
	return projectionmatrix;
}

XMFLOAT3 Camera::GetPosition()
{
	return position;
}
