#include "Camera.hpp"

Camera::Camera() : lookat(XMVectorZero())
{
	position = XMVectorSet(0.0f, 70.0f, -70.0f, 1.0f);
	updirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	lookat = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	view = XMMatrixLookAtLH(position, lookat, updirection);
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	pitch = yaw = 0.0f;
	rotation = 0.009f;
	movement = 0.7f;
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
	auto m = Mouse::Get().GetState();
	auto kb = Keyboard::Get().GetState();
	

	FLOAT y = -sinf(pitch);
	FLOAT r = cosf(pitch);
	FLOAT z = r * cosf(yaw);
	FLOAT x = -r * sinf(yaw);


	if (m.positionMode == Mouse::MODE_RELATIVE)
	{
		Vector3 delta = Vector3(FLOAT(m.x), FLOAT(m.y), 0.f) * rotation;

		pitch -= delta.y;
		yaw -= delta.x;

		// limit pitch to straight up or straight down
		// with a little fudge-factor to avoid gimbal lock
		FLOAT limit = XM_PI / 2.0f - 0.01f;
		pitch = fmaxf(-limit, pitch);
		pitch = fminf(+limit, pitch);
		
		// keep longitude in sane range by wrapping
		if (yaw > XM_PI)
			yaw -= XM_PI * 2.0f;
		else if (yaw < -XM_PI)
			yaw += XM_PI * 2.0f;
	}
	
	Mouse::Get().SetMode(m.leftButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);

	if (kb.Home)
	{
		position = XMVectorZero();
		pitch = yaw = 0;
	}

	Vector3 move = Vector3::Zero;

	if (kb.Up || kb.W)
		move.y += 1.f;

	if (kb.Down || kb.S)
		move.y -= 1.f;

	if (kb.Left || kb.A)
		move.x += 1.f;

	if (kb.Right || kb.D)
		move.x -= 1.f;

	if (kb.PageUp || kb.Space)
		move.z += 1.f;

	if (kb.LeftShift || kb.X)
		move.z -= 1.f;


	Quaternion q = Quaternion::CreateFromYawPitchRoll(yaw, pitch, 0.0f);

	move = Vector3::Transform(move, q);

	move *= movement;

	position += move;

	lookat = position + Vector3(x, y, z);
	view = XMMatrixLookAtRH(position, lookat, Vector3::Up);

}
