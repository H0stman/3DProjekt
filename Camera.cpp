#include "Camera.hpp"

Camera::Camera() : lookat(XMVectorZero())
{
	position = XMVectorSet(0.0f, 0.0f, -70.0f, 1.0f);
	updirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	view = XMMatrixLookAtLH(position, lookat, updirection);
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	pitch = yaw = 0.0f;
	rotation = 0.005f;
	movement = 0.7f;
	forward = XMVector4Normalize(lookat - position);
}

XMMATRIX Camera::GetViewMatrix()
{
	return view;
}

XMMATRIX Camera::GetProjectionMatrix()
{
	return projection;
}

XMMATRIX Camera::GetOrthoMatrix()
{
	return XMMatrixOrthographicLH(1280, 720, 0.0f, 1000.0f);
}

XMVECTOR Camera::GetPosition()
{
	return position;
}

VOID Camera::Update()
{
	auto m = Mouse::Get().GetState();
	auto kb = Keyboard::Get().GetState();

	if (m.positionMode == Mouse::MODE_RELATIVE)
	{
		pitch = yaw = 0;

		pitch += m.y * rotation;
		yaw += m.x * rotation;
		
		// keep longitude in sane range by wrapping
		if (yaw > XM_PI)
			yaw -= XM_PI * 2.0f;
		else if (yaw < -XM_PI)
			yaw += XM_PI * 2.0f;

		forward = XMVector4Normalize(XMVector4Transform(forward, XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f)));
	}
	
	Mouse::Get().SetMode(m.leftButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);

	if (kb.Home)
	{
		position = XMVectorZero();
		pitch = yaw = 0;
	}

	XMVECTOR move = XMVectorZero();

	if (kb.W)
		position += forward;

	if (kb.S)
		position -= forward;

	if (kb.A)
		position += XMVector3Normalize(XMVector3Cross(forward, updirection));

	if (kb.D)
		position -= XMVector3Normalize(XMVector3Cross(forward, updirection));

	if (kb.Space)
		position += updirection;

	if (kb.LeftShift)
		position -= updirection;

	if (kb.O)
		projection = XMMatrixOrthographicLH(1280, 720, 0.1f, 1000.0f);

	if (kb.P)
		projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);

	view = XMMatrixLookToLH(position, forward, updirection);

}
