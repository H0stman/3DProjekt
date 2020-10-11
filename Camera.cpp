#include "Camera.hpp"

Camera::Camera()
{
	position = XMVectorSet(0.0f, 7.0f, 5.0f, 1.0f);
	updirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	view = XMMatrixIdentity();
	projection = XMMatrixPerspectiveFovRH(XMConvertToRadians(85.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	pitch = yaw = 0.0f;
	rotation = 0.004f;
	movement = 0.07f;
}

XMMATRIX Camera::GetViewMatrix()
{
	return view;
}

XMMATRIX Camera::GetProjectionMatrix()
{
	return projection;
}

XMVECTOR Camera::GetPosition()
{
	return position;
}

VOID Camera::SetOrthographicProjection()
{
	projection = XMMatrixOrthographicRH(1280, 720, 0.1f, 1000.0f);
}

VOID Camera::SetPerspectiveProjection()
{
	projection = XMMatrixPerspectiveFovRH(XMConvertToRadians(85.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
}

VOID Camera::Update()
{
	FLOAT y = sinf(pitch);
	FLOAT r = cosf(pitch);
	FLOAT z = r * cosf(yaw);
	FLOAT x = r * sinf(yaw);

	lookat = position + XMVectorSet(x, y , z, 1);
	view = XMMatrixLookAtRH(position, lookat, updirection);
	
}
