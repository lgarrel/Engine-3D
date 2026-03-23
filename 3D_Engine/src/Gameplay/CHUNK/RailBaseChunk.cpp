#include "pch.h"
#include "RailBaseChunk.h"

RailBaseChunk::~RailBaseChunk()
{
    for (Rail* rail : mv_rails)
    {
        delete rail;
    }
    mv_rails.clear();

    for (Enemy* enemy : mv_enemies)
    {
        delete enemy;
    }
    mv_enemies.clear();

    if (mp_floor && mp_Scene)
    {
        mp_Scene->DestroyEntity(mp_floor);
    }
}

void RailBaseChunk::MoveChunk(XMFLOAT3 _move)
{
    for (Rail* rail : mv_rails)
    {
        rail->Move(_move);
    }

    if (mp_floor)
    {
        auto tc = mp_floor->GetComponent<TransformComponent>();
        XMFLOAT3 pos = tc->GetLocalPosition();
        pos.x += _move.x;
        pos.y += _move.y;
        pos.z += _move.z;
        tc->SetLocalPosition(pos);
    }

    for (Enemy* enemy : mv_enemies)
    {
        if (enemy->GetEntity())
        {
            auto tc = enemy->GetEntity()->GetComponent<TransformComponent>();
            XMFLOAT3 pos = tc->GetLocalPosition();
            pos.x += _move.x;
            pos.y += _move.y;
            pos.z += _move.z;
            tc->SetLocalPosition(pos);
        }
    }

    m_exitPoint.x += _move.x;
    m_exitPoint.y += _move.y;
    m_exitPoint.z += _move.z;
}

void RailBaseChunk::UpdateEnemies(float _dt)
{
    for (int i = 0; i < mv_enemies.size(); i++)
    {
        mv_enemies[i]->Update(_dt);
        if (mv_enemies[i]->GetEntity() == nullptr)
        {
            delete mv_enemies[i];
            mv_enemies.erase(mv_enemies.begin() + i);
            i--;
        }
    }
}

Vector<XMFLOAT3> RailBaseChunk::GetEnemyPositions()
{
    Vector<XMFLOAT3> positions;
    for (Enemy* enemy : mv_enemies)
    {
        if (enemy && enemy->GetEntity())
        {
            auto tc = enemy->GetEntity()->GetComponent<TransformComponent>();
            if (tc)
                positions.push_back(tc->GetLocalPosition());
        }
    }
    return positions;
}