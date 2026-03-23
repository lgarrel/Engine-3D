#pragma once

class RailFirstChunk : public RailBaseChunk
{

public:
	void Create(Device* _pDevice, Scene* _pScene, XMFLOAT3 _enterPos, Entity* _player, Vector<XMFLOAT3> _previousEnemyPositions = {}) override;

	void OnUpdate(float _dt) override;
};

