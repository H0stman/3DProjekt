#include "Camera.hpp"

Camera::Camera(INT windowWidth, INT windowHeight)
{
		position = XMFLOAT3(0.0f, 7.0f, 5.0f);
		updirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		view = XMMatrixIdentity();
		projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.f), windowWidth / windowHeight, 0.1f, 1000.0f);
		orthographic = XMMatrixOrthographicLH(windowWidth, windowHeight, 0.1f, 1000.0f);
		pitch = 0.0f;
		yaw = 0.0f;
}

XMMATRIX Camera::GetViewMatrix()
{
	return view;
}

XMMATRIX Camera::GetProjectionMatrix()
{
	return projection;
}

XMFLOAT3 Camera::GetPosition()
{
	return position;
}
