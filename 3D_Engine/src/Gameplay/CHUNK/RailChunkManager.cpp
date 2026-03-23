#include "pch.h"
#include "RailChunkManager.h"

#define Chunk_Defined 2

void RailChunkManager::GenerateChunk(XMFLOAT3 _pos)
{
	int _nextChunk = GenerateRandomNumber(0, Chunk_Defined);
	RailBaseChunk* _pNewChunk = nullptr;

	switch (_nextChunk)
	{
	default:
		_pNewChunk = new RailFirstChunk;
		break;

	}

	_pNewChunk->Create(mp_device, mp_scene, _pos, mp_player, m_lastEnemyPositions);
	m_lastEnemyPositions = _pNewChunk->GetEnemyPositions();
	mv_railChunk.push_back(_pNewChunk);
}

void RailChunkManager::DeleteChunk()
{
	RailBaseChunk* chunkToDelete = mv_railChunk[0];
	mv_railChunk.erase(mv_railChunk.begin());

	delete chunkToDelete;
}

void RailChunkManager::Init()
{
	m_currentSpeed = m_speed;
	m_lastEnemyPositions.clear();

	XMFLOAT3 nextPos = { 0.f, 0.f, 0.f };
	for (int i = 0; i < m_nbChunkToGenerate; i++)
	{
		GenerateChunk(nextPos);
		nextPos = mv_railChunk.back()->GetExitPoint();
	}
}

void RailChunkManager::Init(Device* _pDevice, Scene* _pScene, Entity* _pPlayer)
{
	mp_scene = _pScene;
	mp_device = _pDevice;
	mp_player = _pPlayer;

	Init();
}

void RailChunkManager::Update(float _dt)
{
	XMFLOAT3 _move = { 0.0f, 0.0f, -m_currentSpeed * _dt };

	for (RailBaseChunk* _pChunk : mv_railChunk)
	{
		_pChunk->MoveChunk(_move);
		_pChunk->UpdateEnemies(_dt);

	}

	RailBaseChunk* _pFirstChunk = mv_railChunk[0];
	if (_pFirstChunk->GetExitPoint().z <= -10.0f)
	{
		GenerateChunk(mv_railChunk[mv_railChunk.size() - 1]->GetExitPoint());
		DeleteChunk();
	}

	
	m_currentSpeed += m_acceleration * _dt;
}

void RailChunkManager::Reset()
{
	for (int i = mv_railChunk.size(); i > 0; i--)
	{
		DeleteChunk();
	}

	m_currentSpeed = m_speed;

	Init();
}