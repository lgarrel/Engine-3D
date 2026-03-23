#pragma once

class Rail
{
	Entity* mp_leftRail = nullptr;
	Entity* mp_centerRail = nullptr;
	Entity* mp_rightRail = nullptr;

	Device* mp_device = nullptr;
	Scene* mp_scene = nullptr;

public: 
	Rail();
	~Rail();

	void Init(Scene* _pScene, XMFLOAT3 _enterPos = _VECT_ZERO, float _rotY = 0.0f);

	XMFLOAT3 GetFowrward() { return mp_centerRail->GetComponent<TransformComponent>()->GetTransform().m_forward; }
	XMFLOAT3 GetPosition() { return mp_centerRail->GetComponent<TransformComponent>()->GetTransform().m_worldPosition; }

	void SetDev(Device* _dev) { mp_device = _dev; }

	void Move(XMFLOAT3 _delta);

	void Update(float _dt);
};

