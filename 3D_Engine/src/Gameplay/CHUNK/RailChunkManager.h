#pragma once

class RailChunkManager
{
private:
	Vector<RailBaseChunk*> mv_railChunk;

	Scene* mp_scene = nullptr;
	Device* mp_device = nullptr;
	Entity* mp_player = nullptr;


	float m_speed = 5.0f;        
	float m_currentSpeed = 0.f;
	float m_acceleration = 0.5f; // accélération progressive
	float m_maxSpeed = 10.0f;    // Vitesse maximale

	int m_nbChunkToGenerate = 10;

	RailBaseChunk* mp_currentChunk = nullptr;

	Vector<XMFLOAT3> m_lastEnemyPositions;


	////////////////////////////////////////////

	void GenerateChunk(XMFLOAT3 _pos);

	void DeleteChunk();

	void Init();

public:

	void Init(Device* _pDevice, Scene* _pScene, Entity* _pPlayer);

	Rail* GetCurrentRail() { return mp_currentChunk ? mp_currentChunk->GetCurrentRail() : nullptr; }
	RailBaseChunk* GetCurrentChunk() { return mp_currentChunk; }

	void Update(float _dt);

	void Reset();
};

