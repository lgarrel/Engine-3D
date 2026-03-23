#include "pch.h"
#include "RailFirstChunk.h"

static bool IsTooClose(XMFLOAT3 _pos, const Vector<XMFLOAT3>& _list, float minDist = 3.0f)
{
    for (const XMFLOAT3& other : _list)
    {
        float dx = _pos.x - other.x;
        float dz = _pos.z - other.z;
        float dy = _pos.y - other.y;
        float distSq = dx * dx + dz * dz + dy * dy;
        if (distSq < minDist * minDist)
            return true;
    }
    return false;
}

void RailFirstChunk::Create(Device* _pDevice, Scene* _pScene, XMFLOAT3 _enterPos, Entity* _player, Vector<XMFLOAT3> _previousEnemyPositions)
{
    mp_Device = _pDevice;
    mp_Scene = _pScene;

    mp_floor = _pScene->CreateEntity();
    mp_floor->SetTag("ChunkFloor");

    TransformComponent* floorTransform = mp_floor->AddComponent<TransformComponent>();
    floorTransform->SetLocalPosition(XMFLOAT3(_enterPos.x, _enterPos.y - 1.0f, _enterPos.z + 5.5f));
    floorTransform->SetLocalScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

    std::unique_ptr<Mesh> floorMesh = ShapeFactory::CreateBox(_pDevice, 50.0f, 0.2f, 22.0f);
    RendererComponent* floorRenderer = mp_floor->AddComponent<RendererComponent>();
    floorRenderer->SetMesh(std::move(floorMesh));

    MaterialComponent* floorMat = mp_floor->AddComponent<MaterialComponent>();
    floorMat->SetColor(_BLACK);

    ColliderComponent* floorCollider = mp_floor->AddComponent<ColliderComponent>();
    floorCollider->extents = XMFLOAT3(25.0f, 0.1f, 11.f);


    Vector<XMFLOAT3> _vPos{
        {_enterPos.x, _enterPos.y, _enterPos.z},
        {_enterPos.x, _enterPos.y, _enterPos.z + 2.2f},
        {_enterPos.x, _enterPos.y, _enterPos.z + 4.4f},
        {_enterPos.x, _enterPos.y, _enterPos.z + 6.6f},
        {_enterPos.x, _enterPos.y, _enterPos.z + 8.8f},
        {_enterPos.x, _enterPos.y, _enterPos.z + 11.0f},
        {_enterPos.x, _enterPos.y, _enterPos.z + 13.2f},
        {_enterPos.x, _enterPos.y, _enterPos.z + 15.4f},
        {_enterPos.x, _enterPos.y, _enterPos.z + 17.6f},
        {_enterPos.x, _enterPos.y, _enterPos.z + 19.8f}
    };

    for (int i = 0; i < _vPos.size(); i++)
    {
        Rail* _pnextRail = new Rail;
        _pnextRail->SetDev(_pDevice);
        _pnextRail->Init(_pScene, _vPos[i]);
        mv_rails.push_back(_pnextRail);
    }

    XMFLOAT3 _exit = {
        _vPos[_vPos.size() - 1].x, 
        _vPos[_vPos.size() - 1].y, 
        _vPos[_vPos.size() - 1].z + 2.2f, 
    };

    m_exitPoint = _exit;

    mp_currentRail = mv_rails[0];

    Entity* floorEntity = mp_floor;
    auto floorTc = floorEntity->GetComponent<TransformComponent>();
    XMFLOAT3 floorPos = floorTc->GetLocalPosition();
    float floorSurface = floorPos.y;

    float sideXRanges[2][2] = {
        { -15.0f, -2.0f },  // gauche
        {   2.0f, 15.0f }   // droite
    };

    float minY = floorSurface + 2.0f;
    float maxY = floorSurface + 4.0f;

    float minZ = floorPos.z - 5.5f + 1.0f;
    float maxZ = floorPos.z + 5.5f - 1.0f;

    Vector<XMFLOAT3> usedPositions = _previousEnemyPositions;
    const int maxAttempts = 20;


    // Create Enemies
    for (int i = 0; i < 2; i++)
    {
        int side = i % 2; // 0 = gauche, 1 = droite

        XMFLOAT3 spawnPos = { 0.f, 0.f, 0.f };
        bool found = false;

        for (int attempt = 0; attempt < maxAttempts; attempt++)
        {
            float xMin = sideXRanges[side][0];
            float xMax = sideXRanges[side][1];

            float x = floorPos.x + (float)GenerateRandomNumber((int)(xMin * 10), (int)(xMax * 10)) / 10.0f;
            float y = (float)GenerateRandomNumber((int)(minY * 10), (int)(maxY * 10)) / 10.0f;
            float z = (float)GenerateRandomNumber((int)(minZ * 10), (int)(maxZ * 10)) / 10.0f;

            spawnPos = { x, y, z };

            if (!IsTooClose(spawnPos, usedPositions, 3.0f))
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            float x = floorPos.x + sideXRanges[side][0] + (float)(i * 5);
            float y = minY + 1.0f;
            float z = floorPos.z + (float)(i * 2);
            spawnPos = { x, y, z };
        }

        usedPositions.push_back(spawnPos);

        Enemy* enemy = new Enemy(_pScene, _pDevice, _player);
        enemy->GetEntity()->GetComponent<TransformComponent>()->SetLocalPosition(spawnPos);
        mv_enemies.push_back(enemy);
    }
}

void RailFirstChunk::OnUpdate(float _dt)
{
     
}
