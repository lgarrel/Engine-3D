#pragma once

class Enemy;

class RailBaseChunk
{
protected:
	Vector<Rail*> mv_rails;
	Vector<Enemy*> mv_enemies;

	Rail* mp_currentRail = nullptr;

	Entity* mp_floor = nullptr;

	XMFLOAT3 m_exitPoint;

	Device* mp_Device = nullptr;
	Scene* mp_Scene = nullptr;

public:
	virtual ~RailBaseChunk();

	virtual void Create(Device* _pDevice, Scene* _pScene, XMFLOAT3 _enterPos, Entity* _player, Vector<XMFLOAT3> _previousEnemyPositions = {}) = 0;

	virtual void OnUpdate(float _dt) = 0;

	void MoveChunk(XMFLOAT3 _move);

	void UpdateEnemies(float _dt);

	Rail* GetCurrentRail() { return mp_currentRail; }

	XMFLOAT3 GetExitPoint() { return m_exitPoint; }

	Vector<XMFLOAT3> GetEnemyPositions();
};
