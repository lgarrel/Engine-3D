#pragma once
#include "pch.h"


class Camera
{
	float m_nearPlane;
	float m_farPlane;
	float m_aspect;
	float m_FOV;

	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_proj;

	bool m_dirtyView;

public:

	Camera();
	~Camera();

	void UpdateView(Transform* transform);
	void OnUpdate(Transform* transform);

	void SetLens(float _fov = 70.f, float _aspect = 1.f,
		float _near = 1.f, float _far = 500.f);

	void Walk(Transform* transform, float _dist);

	void LookAt(Transform* tranform, XMFLOAT3 _target, XMFLOAT3 _up);

	//	YPR
	void Yaw(float _angle);
	void Pitch(float _angle);
	void Roll(float _angle);

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	//	Get
	//////////

	//	frustum
	float GetNear();
	float GetFar();
	float GetAspect();
	float GetFOV();

	//	matrix
	XMFLOAT4X4 GetView();
	XMFLOAT4X4 GetProj();

	//SET
	void SetView(XMFLOAT4X4 view) { m_view = view; }

};


