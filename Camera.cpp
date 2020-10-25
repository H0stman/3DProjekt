#include "Camera.hpp"

Camera::Camera() : lookat(XMVectorZero())
{
	position = XMVectorSet(0.0f, 0.0f, -50.0f, 1.0f);
	updirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	view = XMMatrixLookAtLH(position, lookat, updirection);
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1424.0f / 720.0f, 0.1f, 1000.0f);
	fpitch = fyaw = pitch = yaw = 0.0f;
	rotation = 0.005f;
	movement = 0.7f;
	forward = XMVector4Normalize(lookat - position);

	//XMMATRIX viewprojection = view * projection;
	//frustum = BoundingFrustum(viewprojection);

	frustum = BoundingFrustum(projection);
	frustum.Transform(frustum, view);

	//XMStoreFloat3(&frustum.Origin, position);
	//XMVECTOR qangle = XMVector3AngleBetweenNormals(forward, XMVectorSet(1.0, 0.0, 0.0, 1.0));
	//XMVECTOR qorient = XMQuaternionRotationRollPitchYawFromVector(qangle);
	//XMStoreFloat4(&frustum.Orientation, qorient);
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
	return XMMatrixOrthographicLH(1424, 720, 0.1f, 1000.0f);
}

XMVECTOR Camera::GetPosition()
{
	return position;
}

XMVECTOR Camera::GetForward()
{
	return forward;
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
		position = XMVectorSet(0.0f, 0.0f, -50.0f, 1.0f);
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
		projection = XMMatrixOrthographicLH(1424, 720, 0.1f, 1000.0f);

	if (kb.P)
		projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1424.0f / 720.0f, 0.1f, 1000.0f);

	view = XMMatrixLookToLH(position, forward, updirection);
	
	//fpitch += 0.01f;
	//if (fpitch > 2.0 * 3.14) {
	//	fyaw += 0.01;
	//	fpitch = 0.0;
	//}

	//XMVECTOR qangle = XMVector3AngleBetweenNormals(forward, XMVectorSet(1.0, 0.0, 0.0, 1.0));
	//XMVECTOR qorient = XMQuaternionRotationRollPitchYawFromVector(qangle);
	//XMVECTOR qorient = XMQuaternionRotationRollPitchYaw(fpitch, fyaw, 0.0f);
	//XMStoreFloat4(&frustum.Orientation, qorient);
	frustum = BoundingFrustum(projection);
	XMVECTOR det = XMMatrixDeterminant(view);
	frustum.Transform(frustum, XMMatrixInverse(&det, view));
	//frustum.Transform(frustum, view);

	//XMMATRIX viewprojection = view * projection;
	
}

VOID Camera::Update(XMVECTOR parentpos, XMVECTOR parentforward)
{
	XMVECTOR focus = parentpos + parentforward * 3;
	position = focus + XMVectorSet(4.0, 3.0, 0.0, 1.0);
	view = XMMatrixLookAtLH(position, focus, updirection);
}

VOID Camera::SetPositionY(float value)
{
	position = XMVectorSetY(this->position, value);
}

BoundingFrustum Camera::GetFrustum()
{
	return frustum;
}
