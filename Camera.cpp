#include "Camera.hpp"

Camera::Camera()
{
	position = XMFLOAT3(0.0f, 7.0f, 5.0f);
	updirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	view = XMMatrixIdentity();
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(85.0f), Engine::GetInstance()->window.GetWidth() / Engine::GetInstance()->window.GetHeight(), 0.1f, 1000.0f);
	pitch = yaw = 0.0f;
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

VOID Camera::SetOrthographicProjection()
{
	projection = XMMatrixOrthographicLH(Engine::GetInstance()->window.GetWidth(), Engine::GetInstance()->window.GetHeight(), 0.1f, 1000.0f);
}

VOID Camera::SetPerspectiveProjection()
{
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(85.0f), Engine::GetInstance()->window.GetWidth() / Engine::GetInstance()->window.GetHeight(), 0.1f, 1000.0f);
}
