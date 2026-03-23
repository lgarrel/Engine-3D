#include "pch.h"
#include "Transform.h"

Transform::Transform()
{
	Identity();
	m_dirtyWorld = true;
}

void Transform::UpdateLocal()
{
	XMFLOAT4X4 mat = m_localRotation;

	float scaX = m_localScale.x;
	float scaY = m_localScale.y;
	float scaZ = m_localScale.z;

	XMFLOAT4 matRow1 = { mat.m[0][0] * scaX, mat.m[0][1] * scaX, mat.m[0][2] * scaX, mat.m[0][3] * scaX };
	XMFLOAT4 matRow2 = { mat.m[1][0] * scaY, mat.m[1][1] * scaY, mat.m[1][2] * scaY, mat.m[1][3] * scaY };
	XMFLOAT4 matRow3 = { mat.m[2][0] * scaZ, mat.m[2][1] * scaZ, mat.m[2][2] * scaZ, mat.m[2][3] * scaZ };
	XMFLOAT4 matRow4 = { m_localPosition.x, m_localPosition.y, m_localPosition.z, 1.f };

	m_localMatrix = _Utils::XMFLT4ToXMFLT4X4(matRow1, matRow2, matRow3, matRow4);

	m_world = m_localMatrix;

	m_worldPosition = m_localPosition;
	m_worldScale = m_localScale;
}

void Transform::UpdateWorld()
{
	XMFLOAT4X4 mat = m_worldRotation;

	float scaX = m_worldScale.x;
	float scaY = m_worldScale.y;
	float scaZ = m_worldScale.z;

	XMFLOAT4 matRow1 = { mat.m[0][0] * scaX, mat.m[0][1] * scaX, mat.m[0][2] * scaX, mat.m[0][3] * scaX };
	XMFLOAT4 matRow2 = { mat.m[1][0] * scaY, mat.m[1][1] * scaY, mat.m[1][2] * scaY, mat.m[1][3] * scaY };
	XMFLOAT4 matRow3 = { mat.m[2][0] * scaZ, mat.m[2][1] * scaZ, mat.m[2][2] * scaZ, mat.m[2][3] * scaZ };
	XMFLOAT4 matRow4 = { m_worldPosition.x,  m_worldPosition.y,  m_worldPosition.z,  1.f };

	m_world = _Utils::XMFLT4ToXMFLT4X4(matRow1, matRow2, matRow3, matRow4);
}

void Transform::Identity()
{
	m_localPosition = { 0.f, 0.f, 0.f };
	m_worldPosition = m_localPosition;

	m_localScale = { 1.f, 1.f, 1.f };
	m_worldScale = m_localScale;

	ResetLocalRotation();
	ResetWorldRotation();

	m_world = _Math::Identity4x4();
}

void Transform::ResetFlag()
{
	m_dirtyWorld = true;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void Transform::MoveLocal(float x, float y, float z)
{
	m_localPosition.x += m_forward.x * x;
	m_localPosition.y += m_forward.y * y;
	m_localPosition.z += m_forward.z * z;
}

void Transform::MoveLocal(XMFLOAT3 _move)
{
	float _x = _move.x;
	float _y = _move.y;
	float _z = _move.z;

	MoveLocal(_x, _y, _z);
}

void Transform::MoveForwardLocal(float d)
{
	XMVECTOR p = XMLoadFloat3(&m_localPosition);
	XMVECTOR f = XMLoadFloat3(&m_forward);
	XMStoreFloat3(&m_localPosition, XMVectorMultiplyAdd(XMVectorReplicate(d), f, p));
}

void Transform::MoveRightLocal(float d)
{
	XMVECTOR p = XMLoadFloat3(&m_localPosition);
	XMVECTOR r = XMLoadFloat3(&m_right);
	XMStoreFloat3(&m_localPosition, XMVectorMultiplyAdd(XMVectorReplicate(d), r, p));
}

void Transform::MoveUpLocal(float d)
{
	XMVECTOR p = XMLoadFloat3(&m_localPosition);
	XMVECTOR u = XMLoadFloat3(&m_up);
	XMStoreFloat3(&m_localPosition, XMVectorMultiplyAdd(XMVectorReplicate(d), u, p));
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void Transform::SetLocalScale(XMFLOAT3 _scale)
{
	m_localScale.x = _scale.x;
	m_localScale.y = _scale.y;
	m_localScale.z = _scale.z;
}

void Transform::LocalScale(float _scale)
{
	m_localScale.x += _scale;
	m_localScale.y += _scale;
	m_localScale.z += _scale;

	if (m_localScale.x <= .1f)
		m_localScale.x = .1f;

	if (m_localScale.y <= .1f)
		m_localScale.y = .1f;

	if (m_localScale.z <= .1f)
		m_localScale.z = .1f;
}

void Transform::SetWorldScale(float _scale)
{
	m_localScale.x = _scale;
	m_localScale.y = _scale;
	m_localScale.z = _scale;
}

void Transform::WorldScale(float _scale)
{
	m_worldScale.x += _scale;
	m_worldScale.y += _scale;
	m_worldScale.z += _scale;

	if (m_worldScale.x <= .1f)
		m_worldScale.x = .1f;

	if (m_worldScale.y <= .1f)
		m_worldScale.y = .1f;

	if (m_worldScale.z <= .1f)
		m_worldScale.z = .1f;
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void Transform::ResetLocalRotation()
{
	m_forward = { 0.0f, 0.0f, 1.0f };
	m_right = { 1.0f, 0.0f, 0.0f };
	m_up = { 0.0f, 1.0f, 0.0f };

	m_localQuaternion = { 0.f, 0.f, 0.f, 1.f };

	XMStoreFloat4x4(&m_localRotation, XMMatrixIdentity());
}

void Transform::SetLocalRotation(Transform* transform)
{
	m_forward = transform->m_forward;
	m_right = transform->m_right;
	m_up = transform->m_up;
	m_localQuaternion = transform->m_localQuaternion;
	m_localRotation = transform->m_localRotation;
}

void Transform::SetRotationFromAxes()
{
	AxisRotation4X4(m_localRotation, m_right, m_up, m_forward);
	XMStoreFloat4(&m_localQuaternion, XMQuaternionNormalize(XMQuaternionRotationMatrix(XMLoadFloat4x4(&m_localRotation))));
}

void Transform::SetRotationFromMatrix()
{
	XMStoreFloat4(&m_localQuaternion, XMQuaternionNormalize(XMQuaternionRotationMatrix(XMLoadFloat4x4(&m_localRotation))));
	MatrixRotation4X4(m_localRotation, m_right, m_up, m_forward);
}

void Transform::SetRotationFromQuaternion()
{
	XMStoreFloat4x4(&m_localRotation, XMMatrixRotationQuaternion(XMLoadFloat4(&m_localQuaternion)));
	QuaternionRotation4X4(m_localRotation, m_right, m_up, m_forward);
}

void Transform::ResetWorldRotation()
{
	m_worldQuaternion = { 0.f, 0.f, 0.f, 1.f };

	XMStoreFloat4x4(&m_worldRotation, XMMatrixIdentity());
}

void Transform::SetWorldRotation(Transform* transform)
{
	m_worldQuaternion = transform->m_worldQuaternion;
	m_worldRotation = transform->m_worldRotation;
}

void Transform::RotateLocalX(float radians)
{
	XMMATRIX rot = XMMatrixRotationAxis(XMLoadFloat3(&m_right), radians);
	ApplyLocalRotation(rot);
}

void Transform::RotateLocalY(float radians)
{
	XMMATRIX rot = XMMatrixRotationY(radians);
	ApplyLocalRotation(rot);
}

void Transform::RotateLocalZ(float radians)
{
	XMMATRIX rot = XMMatrixRotationAxis(XMLoadFloat3(&m_forward), radians);
	ApplyLocalRotation(rot);
}

void Transform::RotateLocalAxis(XMFLOAT3 axis, float radians)
{
	XMMATRIX rot = XMMatrixRotationAxis(XMLoadFloat3(&axis), radians);
    ApplyLocalRotation(rot);
}

void Transform::SetYPR(float _yaw, float _pitch, float _roll)
{
	ResetLocalRotation();
	AddYPR(_yaw, _pitch, _roll);
}

void Transform::AddYPR(float _yaw, float _pitch, float _roll)
{
	XMVECTOR _axisForward = XMLoadFloat3(&m_forward);
	XMVECTOR _axisRight = XMLoadFloat3(&m_right);
	XMVECTOR _axisUp = XMLoadFloat3(&m_up);

	XMVECTOR _quatRota = XMLoadFloat4(&m_localQuaternion);
	if (_roll)
		_quatRota = XMQuaternionMultiply(_quatRota, XMQuaternionRotationAxis(_axisForward, _roll));
	if (_pitch)
		_quatRota = XMQuaternionMultiply(_quatRota, XMQuaternionRotationAxis(_axisRight, _pitch));
	if (_yaw)
		_quatRota = XMQuaternionMultiply(_quatRota, XMQuaternionRotationAxis(_axisUp, _yaw));

	_quatRota = XMQuaternionNormalize(_quatRota);
	XMStoreFloat4(&m_localQuaternion, _quatRota);

	SetRotationFromQuaternion();
}

void Transform::LookAt(float _x, float _y, float _z, const XMFLOAT3& _up)
{
	XMVECTOR _vPos = XMLoadFloat3(&m_localPosition);
	XMVECTOR _vVar = XMVectorSet(_x, _y, _z, 1.0f);
	XMVECTOR _vForw = XMVectorSubtract(_vVar, _vPos);

	if (XMVectorGetX(XMVector3LengthSq(_vForw)) <= _ZERO)
		return;

	const XMMATRIX _cam = XMMatrixTranspose(XMMatrixLookAtLH(XMVectorZero(), _vForw, XMLoadFloat3(&_up)));
	XMStoreFloat4x4(&m_localRotation, _cam);
	SetRotationFromMatrix();
}

void Transform::LookTo(float _x, float _y, float _z, const XMFLOAT3& _up)
{
	XMVECTOR vDir = XMVectorSet(_x, _y, _z, 0.0f);
	if (XMVectorGetX(XMVector3LengthSq(vDir)) <= _ZERO)
		return;
	XMMATRIX _cam = XMMatrixTranspose(XMMatrixLookToLH(XMVectorZero(), vDir, XMLoadFloat3(&_up)));
	XMStoreFloat4x4(&m_localRotation, _cam);
	SetRotationFromMatrix();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void Transform::ApplyLocalRotation(XMMATRIX rot)
{
	XMVECTOR F = XMVector3TransformNormal(XMLoadFloat3(&m_forward), rot);
	XMVECTOR R = XMVector3TransformNormal(XMLoadFloat3(&m_right), rot);

	F = XMVector3Normalize(F);
	R = XMVector3Normalize(R);
	XMVECTOR U = XMVector3Normalize(XMVector3Cross(F, R)); 
	R = XMVector3Cross(U, F);                               

	XMStoreFloat3(&m_forward, F);
	XMStoreFloat3(&m_right, R);
	XMStoreFloat3(&m_up, U);

	XMFLOAT4 rotRow1 = { m_right.x	, m_right.y		, m_right.z  , 0.f };
	XMFLOAT4 rotRow2 = { m_up.x		, m_up.y		, m_up.z	 , 0.f };
	XMFLOAT4 rotRow3 = { m_forward.x, m_forward.y	, m_forward.z, 0.f };
	XMFLOAT4 rotRow4 = { 0.f		, 0.f			, 0.f		 , 1.f };
	m_localRotation = _Utils::XMFLT4ToXMFLT4X4(rotRow1, rotRow2, rotRow3, rotRow4);
}

void Transform::ApplyParentMatrix(const XMFLOAT4X4& parentWorld)
{
	XMMATRIX parent = XMLoadFloat4x4(&parentWorld);
	XMMATRIX local = XMLoadFloat4x4(&m_localMatrix);

	XMMATRIX world = XMMatrixMultiply(local, parent);

	XMStoreFloat4x4(&m_world, world);

	m_worldPosition = { m_world.m[3][0], m_world.m[3][1], m_world.m[3][2] };
}

void Transform::LookFrom(float _x, float _y, float _z, const XMFLOAT3& _up)
{
	// Direction depuis la source (_x,_y,_z) vers la position locale de l'objet
	XMVECTOR vSource = XMVectorSet(_x, _y, _z, 0.0f);
	XMVECTOR vPos = XMLoadFloat3(&m_localPosition);
	XMVECTOR vDir = XMVectorSubtract(vPos, vSource);

	if (XMVectorGetX(XMVector3LengthSq(vDir)) <= _ZERO)
		return;

	XMMATRIX _cam = XMMatrixTranspose(
		XMMatrixLookToLH(XMVectorZero(), vDir, XMLoadFloat3(&_up))
	);
	XMStoreFloat4x4(&m_localRotation, _cam);
	SetRotationFromMatrix();
}
