#pragma once

struct Transform
{
	//POS
	XMFLOAT3 m_localPosition;
	XMFLOAT3 m_worldPosition;

	//SCA
	XMFLOAT3 m_localScale;
	XMFLOAT3 m_worldScale;

	//ROTA
	XMFLOAT3 m_forward;
	XMFLOAT3 m_up;
	XMFLOAT3 m_right;

	XMFLOAT4 m_localQuaternion;
	XMFLOAT4 m_worldQuaternion;

	XMFLOAT4X4 m_localRotation;
	XMFLOAT4X4 m_worldRotation;

	//RES
	XMFLOAT4X4 m_localMatrix;
	XMFLOAT4X4 m_world;
	bool m_dirtyWorld;

public:
	Transform();
	void UpdateLocal();
	void UpdateWorld();

	void Identity();
	void ResetFlag();

	//Move
	void MoveLocal(float x, float y, float z);
	void MoveLocal(XMFLOAT3 _move);
	void MoveForwardLocal(float d);
	void MoveRightLocal(float d);
	void MoveUpLocal(float d);

	// Rotation 
	void ResetLocalRotation();
	void SetLocalRotation(Transform* transform);

	// Scale
	void SetLocalScale(XMFLOAT3 _scale);
	void LocalScale(float _scale);
	void SetWorldScale(float _scale);
	void WorldScale(float _scale);

	// Rotation 
	void ResetWorldRotation();
	void SetWorldRotation(Transform* transfrom);
	void SetRotationFromAxes();
	void SetRotationFromMatrix();
	void SetRotationFromQuaternion();

	void RotateLocalX(float radians);
	void RotateLocalY(float radians);
	void RotateLocalZ(float radians);

	void RotateLocalAxis(XMFLOAT3 axis, float radians);

	void SetYPR(float _yaw, float _pitch = 0.f, float _roll = 0.f);
	void AddYPR(float _yaw, float _pitch = 0.f, float _roll = 0.f);

	void LookAt(float _x, float _y, float _z, const XMFLOAT3& _up);
	void LookTo(float _x, float _y, float _z, const XMFLOAT3& _up);

	//Parent/Enfant
	void ApplyParentMatrix(const XMFLOAT4X4& parentWorld);

	//test
	void LookFrom(float _x, float _y, float _z, const XMFLOAT3& _up);

private :

	void ApplyLocalRotation(XMMATRIX rot);
};