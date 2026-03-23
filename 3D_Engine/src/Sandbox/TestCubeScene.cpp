#include "pch.h"
#include "TestCubeScene.h"
#include "S_Player.h"

void TestCubeScene::SetDevice(Device* device)
{
    mp_device = device;
}

void TestCubeScene::SetGraphicsEngine(GraphicsEngine* engine)
{
    m_graphicsEngine = engine;
}

void TestCubeScene::Init()
{
    int indexBasicTexture = m_graphicsEngine->Load3DTexture(L"../../res/Texture/White_Basic.png");
    int indexTextureCaisse = m_graphicsEngine->Load3DTexture(L"../../res/Texture/caisseTexture2.png");

    m_player = new S_Player(this, mp_device, indexTextureCaisse);
    Entity* wagon = m_player->GetWagon();

    //Create refCube
    Entity* refCubeEntity = CreateEntity();
    refCubeEntity->SetTag("RefCube");

    TransformComponent* refTransform = refCubeEntity->AddComponent<TransformComponent>();
    refTransform->SetLocalPosition(XMFLOAT3(0.0f, 0.5f, 10.0f));
    refTransform->SetLocalScale(XMFLOAT3(1.f, 1.f, 1.f));

    std::unique_ptr<Mesh> refMesh = ShapeFactory::CreateBox(mp_device, 2.0f, 2.0f, 2.0f);
    RendererComponent* refRenderer = refCubeEntity->AddComponent<RendererComponent>();
    refRenderer->SetMesh(std::move(refMesh));

    MaterialComponent* refMat = refCubeEntity->AddComponent<MaterialComponent>();

    refMat->GetMaterial().m_diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

    RigidBodyComponent* refCubeBody = refCubeEntity->AddComponent<RigidBodyComponent>();
    refCubeBody->isStatic = true;

    ColliderComponent* refCubeCollider = refCubeEntity->AddComponent<ColliderComponent>();
    refCubeCollider->extents = XMFLOAT3(1.0f, 1.0f, 1.0f);
    


    // Create Camera
    m_cameraEntity = CreateEntity();
    TransformComponent* camTransform = m_cameraEntity->AddComponent<TransformComponent>();
    camTransform->SetLocalPosition(XMFLOAT3(0.0f, 3.0f, -10.0f));

    CameraComponent* camComp = m_cameraEntity->AddComponent<CameraComponent>();
    camComp->SetPerspective(XM_PIDIV4, 1280.0f / 720.0f, 0.1f, 1000.0f);


    //Create obstacle
    // Entity* obstacleEntity = CreateEntity();

    // TransformComponent* obsTransform = obstacleEntity->AddComponent<TransformComponent>();

    // obsTransform->SetLocalPosition(XMFLOAT3(0.0f, -1.0f, 20.0f));
    // obsTransform->SetLocalScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

    // std::unique_ptr<Mesh> obsMesh = ShapeFactory::CreateBox(m_device, 2.0f, 2.0f, 2.0f);

    // RendererComponent* obsRenderer = obstacleEntity->AddComponent<RendererComponent>();
    // obsRenderer->SetMesh(std::move(obsMesh));

    // ColliderComponent* obsCollider = obstacleEntity->AddComponent<ColliderComponent>();
    // obsCollider->extents = XMFLOAT3(1.0f, 1.0f, 1.0f);
     
    // obstacleEntity->AddComponent<RigidBodyComponent>();

    // --- UI ---
    Entity* uiEntityText = CreateEntity();
    UIRendererComponent* uiTextComp = uiEntityText->AddComponent<UIRendererComponent>();
    uiTextComp->position = XMFLOAT2(50.f, 50.f);
    uiTextComp->size = XMFLOAT2(32.f, 32.f);
    uiTextComp->text = "POURQUOI CA MARCHE";
    uiTextComp->color = XMFLOAT4(1.f, 0.f, 0.f, 1.f);


    int indexTextureHeart = m_graphicsEngine->Load3DTexture(L"../../res/Texture/Heart.png");

    for (int i = 0; i < 3; i++ )
    {
        Entity* uiHeartEntity = CreateEntity();
        UIRendererComponent* uiHeartComp = uiHeartEntity->AddComponent<UIRendererComponent>();
        uiHeartComp->position = XMFLOAT2(1100.f + 40*i, 50.f);
        uiHeartComp->size = XMFLOAT2(50.f, 32.f);
        uiHeartComp->text = "";
        uiHeartComp->textureIndex = indexTextureHeart;
        uiHeartComp->color = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
    }


    // Create Light
    Entity* lightEntity = CreateEntity();
    LightComponent* lightComp = lightEntity->AddComponent<LightComponent>();
    lightComp->SetType(LightComponent::LightType::Directional);
    lightComp->SetStrength(XMFLOAT3(1.0f, 1.0f, 1.0f));
    lightComp->SetDirection({ 0.f, 0.f, 1.f });

    Entity* lightEntity2 = CreateEntity();
    LightComponent* lightComp2 = lightEntity2->AddComponent<LightComponent>();
    lightComp2->SetType(LightComponent::LightType::Directional);
    lightComp2->SetStrength(XMFLOAT3(1.0f, 1.0f, 1.0f));

    //Create Floor
    Entity* floorEntity = CreateEntity();
    floorEntity->SetTag("Floor");

    TransformComponent* floorTransform = floorEntity->AddComponent<TransformComponent>();
    floorTransform->SetLocalPosition(XMFLOAT3(0.0f, -1.0f, 0.0f));
    floorTransform->SetLocalScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

    std::unique_ptr<Mesh> floorMesh = ShapeFactory::CreateBox(mp_device, 200.0f, 1.f, 200.0f);

    RendererComponent* floorRenderer = floorEntity->AddComponent<RendererComponent>();
    floorRenderer->SetMesh(std::move(floorMesh));

    MaterialComponent* floorMat = floorEntity->AddComponent<MaterialComponent>();
    floorMat->GetMaterial().m_diffuse = XMFLOAT4(0.f, 0.5f,0.5f, 1.0f);


    ColliderComponent* floorCollider = floorEntity->AddComponent<ColliderComponent>();

    floorCollider->extents = XMFLOAT3(100.0f, 0.5f, 100.0f);


    //Create Wall
    Entity* wallEntity = CreateEntity();
    wallEntity->SetTag("Wall");

    TransformComponent* wallTransform = wallEntity->AddComponent<TransformComponent>();
    wallTransform->SetLocalPosition(XMFLOAT3(0.0f, -1.0f, 50.f));
    wallTransform->SetLocalScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

    std::unique_ptr<Mesh> wallMesh = ShapeFactory::CreateBox(mp_device, 20.0f, 20.f, 1.0f);

    RendererComponent* wallRenderer = wallEntity->AddComponent<RendererComponent>();
    wallRenderer->SetMesh(std::move(wallMesh));

    MaterialComponent* wallMat = wallEntity->AddComponent<MaterialComponent>();
    wallMat->GetMaterial().m_diffuse = XMFLOAT4(0.f, .6f, 0.5f, 1.0f);

    ColliderComponent* wallCollider = wallEntity->AddComponent<ColliderComponent>();
    wallCollider->extents = XMFLOAT3(10.0f, 10.0f, 0.5f);



    //  Create Rails
    S_Rail* _pFirstRail = new S_Rail;
    _pFirstRail->SetDev(mp_device);
    _pFirstRail->Init(this);
    mv_rails.push_back(_pFirstRail);
    //mp_currentRail = _pFirstRail;

    XMFLOAT3 _enterPnt = _pFirstRail->GetExitPoint();
    XMFLOAT3 _forw = _FORWARD;
    //XMFLOAT3 _forw = {0.0f, 0.0f, -1.0f};

    for (int i = 1; i <= m_railNB; i++)
    {
        if (i != 1)
        {
            _enterPnt = mv_rails[i - 1]->GetExitPoint();
            _forw = mv_rails[i - 1]->GetForward();

            //_forw.z += GenerateRandomNumber(-3, 3);
        }

        S_Rail* _pnextRail = new S_Rail;
        _pnextRail->SetDev(mp_device);
        _pnextRail->Init(this, _enterPnt, _forw);
        mv_rails.push_back(_pnextRail);

        _COUT i _END
            _COUT mv_rails.size() - 1 _END
    }

    /*std::cout << "Cube entity created with ID: " << refCubeEntity->GetID() << std::endl;
    std::cout << "Camera entity created with ID: " << m_cameraEntity->GetID() << std::endl;
    std::cout << "Player entity created with ID: " << wagon->GetID() << std::endl;
    //std::cout << "Obstacle entity created with ID: " << obstacleEntity->GetID() << std::endl;
    std::cout << "UI entity created with ID: " << uiEntity->GetID() << std::endl;
    std::cout << "Light entity created with ID: " << lightEntity->GetID() << std::endl;*/

    //SET CHILD
    wagon->AddChild(m_cameraEntity);
    //wagon->AddChild(refCubeEntity);

}

void TestCubeScene::Update(float deltaTime)
{
    if (m_player) {
        m_player->Update(deltaTime);
    }
    Scene::Update(deltaTime);
}

void TestCubeScene::Render()
{
}
