#include "pch.h"
#include "TestSceneRail.h"

TestSceneRail* TestSceneRail::mp_instance = nullptr;

TestSceneRail* TestSceneRail::GetInstance()
{
    if (mp_instance == nullptr)
        mp_instance = new TestSceneRail;

    return mp_instance;
}

void TestSceneRail::Init()
{
    //  floor
    /*Entity* refCubeEntity = CreateEntity();
    TransformComponent* refTransform = refCubeEntity->AddComponent<TransformComponent>();
    refTransform->SetLocalPosition(XMFLOAT3(0.0f, -2.0f, 10.0f));
    refTransform->SetLocalScale(XMFLOAT3(10.f, 0.25f, 10.f));

    std::unique_ptr<Mesh> refMesh = ShapeFactory::CreateBox(m_device, 2.0f, 2.0f, 2.0f);
    RendererComponent* refRenderer = refCubeEntity->AddComponent<RendererComponent>();
    refRenderer->SetMesh(std::move(refMesh));*/

    // Create Camera
    /*mp_cameraEntity = CreateEntity();
    TransformComponent* camTransform = mp_cameraEntity->AddComponent<TransformComponent>();
    camTransform->SetLocalPosition(XMFLOAT3(0.0f, 1.0f, .0f));

    CameraComponent* camComp = mp_cameraEntity->AddComponent<CameraComponent>();
    camComp->SetPerspective(XM_PIDIV4, 1280.0f / 720.0f, 0.1f, 1000.0f);*/

    mp_cameraEntity = CreateEntity();
    TransformComponent* camTransform = mp_cameraEntity->AddComponent<TransformComponent>();
    camTransform->SetLocalPosition(XMFLOAT3(0.0f, 2.0f, 0.0f));

    CameraComponent* camComp = mp_cameraEntity->AddComponent<CameraComponent>();
    camComp->SetPerspective(XM_PIDIV4, 1280.0f / 720.0f, 0.1f, 1000.0f);

    //mp_cameraEntity->AddComponent<PlayerControllerComponent>();

    //  Create Rails
    S_Rail* _pFirstRail = new S_Rail;
    _pFirstRail->SetDev(m_device);
    _pFirstRail->Init(this);
    mv_rails.push_back(_pFirstRail);
    mp_currentRail = _pFirstRail;

    XMFLOAT3 _enterPnt = _pFirstRail->GetExitPoint();
    XMFLOAT3 _forw = _FORWARD;

    for (int i = 1; i <= m_railNB; i++)
    { 
        if (i != 1)
        {
            _enterPnt = mv_rails[i - 1]->GetExitPoint();
            _forw = mv_rails[i - 1]->GetForward();
        }
    
        S_Rail* _pnextRail = new S_Rail;
        _pnextRail->SetDev(m_device);   
        _pnextRail->Init(this, _enterPnt, _forw);
        mv_rails.push_back(_pnextRail);

        _COUT i _END
        _COUT mv_rails.size() - 1 _END
    }

}

void TestSceneRail::Update(float deltaTime)
{
    mp_cameraEntity->GetComponent<TransformComponent>()->GetTransform().MoveForwardLocal(5.f * deltaTime);
    XMFLOAT3 _camPos = mp_cameraEntity->GetComponent<TransformComponent>()->GetTransform().m_worldPosition;

    if (mp_currentRail->GetExitPoint().z < _camPos.z )
    {
        DeleteRail();

        S_Rail* _pnextRail = new S_Rail;
        _pnextRail->SetDev(m_device);
        _pnextRail->Init(this
            , mv_rails[mv_rails.size() - 1]->GetExitPoint()
            , mv_rails[mv_rails.size() - 1]->GetForward());
        mv_rails.push_back(_pnextRail);
    }
}

void TestSceneRail::Render()
{
}

void TestSceneRail::DeleteRail()
{
    S_Rail* _toDel = mv_rails[0];
    mv_rails.erase(mv_rails.begin());

    delete _toDel;
}

//void ChunkManager::DeleteChunk()
//{
//    Chunk* chunkToDelete = m_vChunk[0];
//    m_vChunk.erase(m_vChunk.begin());
//
//    delete chunkToDelete;
//
//    m_pGameManager->AddToScore(100);
//}