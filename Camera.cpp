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

XMVECTOR Camera::GetPosition()
{
	return position;
}

VOID Camera::SetOrthographicProjection()
{
	projection = XMMatrixOrthographicLH(1280, 720, 0.1f, 1000.0f);
}

VOID Camera::SetPerspectiveProjection()
{
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(85.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
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

		// limit pitch to straight up or straight down
		// with a little fudge-factor to avoid gimbal lock
		FLOAT limit = XM_PI / 2.0f - 0.01f;
		pitch = max(-limit, pitch);
		pitch = min(+limit, pitch);
		
		// keep longitude in sane range by wrapping
		if (yaw < XM_PI)
			yaw -= XM_PI * 2.0f;
		else if (yaw > -XM_PI)
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

	if (kb.Up || kb.W)
		position += forward;

	if (kb.Down || kb.S)
		position -= forward;

	if (kb.Left || kb.A)
		position += XMVector3Normalize(XMVector3Cross(forward, updirection));

	if (kb.Right || kb.D)
		position -= XMVector3Normalize(XMVector3Cross(forward, updirection));

	if (kb.PageUp || kb.Space)
		position += updirection;

	if (kb.LeftShift || kb.X)
		position -= updirection;

	if (kb.O)
		SetOrthographicProjection();

	if (kb.P)
		SetPerspectiveProjection();

	view = XMMatrixLookToLH(position, forward, updirection);

}
