#pragma once

struct EER
{
	XMFLOAT3 m_exitPoint;
	XMFLOAT3 m_enterPoint;
	XMFLOAT3 m_forward;
};

class S_Rail
{
	Entity* mp_leftRail = nullptr;
	Entity* mp_centerRail = nullptr;
	Entity* mp_rightRail = nullptr;

	Device* mp_device = nullptr;

	EER m_eef;

	//float m_speed = 5.f;


public: 
	S_Rail();
	~S_Rail();

	void Init(Scene* _pScene, XMFLOAT3 _enterPnt = _VECT_ZERO, XMFLOAT3 _forw = _FORWARD);

	XMFLOAT3& GetExitPoint() { return m_eef.m_exitPoint; }
	XMFLOAT3& GetForward() { return m_eef.m_forward; }

	void SetDev(Device* _dev) { mp_device = _dev; }

	void Update(float _dt);
};

