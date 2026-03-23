#include "pch.h"
#include "S_Player.h"



S_Player::S_Player(Scene* scene, Device* device, int wagonTextureIndex)
{
    m_scene = scene;
    m_device = device;

    // Create Wagon
    m_wagon = scene->CreateEntity(); 
    m_wagon->SetTag("Player");

    TransformComponent* wagonTransform = m_wagon->AddComponent<TransformComponent>();
    wagonTransform->SetLocalPosition(XMFLOAT3(0.0f, 1.f, 5.0f));
    wagonTransform->SetLocalScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

    std::unique_ptr<Mesh> wagonMesh = ShapeFactory::CreateBox(device, 2.0f, 2.0f, 2.0f);
    RendererComponent* wagonRenderer = m_wagon->AddComponent<RendererComponent>();
    wagonRenderer->SetMesh(std::move(wagonMesh));

    MaterialComponent* wagonMat = m_wagon->AddComponent<MaterialComponent>();

    wagonMat->GetMaterial().m_diffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.0f);

    wagonMat->SetTextureIndex(wagonTextureIndex);
    

    m_wagon->AddComponent<PlayerControllerComponent>();

    ColliderComponent* wagonCollider = m_wagon->AddComponent<ColliderComponent>();
    wagonCollider->extents = XMFLOAT3(1.0f, 1.f, 1.55f);
    wagonCollider->offset = XMFLOAT3(0.0f, 0.0f, 0.5f);

    m_wagon->AddComponent<RigidBodyComponent>();

    // Create Cannon
    m_cannon = scene->CreateEntity();
    m_cannon->SetTag("Cannon");

    TransformComponent* cannonTransform = m_cannon->AddComponent<TransformComponent>();
    cannonTransform->SetLocalPosition(XMFLOAT3(0.0f, 0.9f, 1.4f));
    cannonTransform->SetLocalScale(XMFLOAT3(0.3f, 0.7f, 0.3f));
    cannonTransform->RotateLocalX(XM_PIDIV4);

    std::unique_ptr<Mesh> cannonMesh = ShapeFactory::CreateCylinder(device, 0.5f, 0.5f, 2.0f, 20, 20);
    RendererComponent* cannonRenderer = m_cannon->AddComponent<RendererComponent>();
    cannonRenderer->SetMesh(std::move(cannonMesh));

    MaterialComponent* cannonMat = m_cannon->AddComponent<MaterialComponent>();
    cannonMat->SetColor(_GRAY);

    m_wagon->AddChild(m_cannon);
}

void S_Player::Update(float deltaTime) 
{
    InputManager* input = InputManager::GetInstance();
    if (input->IsKeyPressed('A'))
    {
        if (m_timeToShoot >= m_cooldown)
        {
            Shoot();
            m_timeToShoot = 0.f;
        }
    }

    for (int i = 0; i < mv_bullets.size(); i++)
    {
        mv_bullets[i]->Update(deltaTime);
        if (mv_bullets[i]->ShouldDestroy())
        {
            mv_bullets.erase(mv_bullets.begin() + i);
            i--;
        }
    }
}

void S_Player::Shoot()
{
    Bullet* bullet = new Bullet(m_scene, m_device);

    TransformComponent* cannonTransform = m_cannon->GetComponent<TransformComponent>();
    XMMATRIX cannonWorld = XMLoadFloat4x4(&cannonTransform->GetWorldMatrix());

    XMVECTOR tipLocal = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
    XMVECTOR tipWorld = XMVector4Transform(tipLocal, cannonWorld);

    XMFLOAT3 tipPos;
    XMStoreFloat3(&tipPos, tipWorld);

    XMVECTOR forwardWorld = XMVector3Normalize(XMVectorSet(cannonTransform->GetTransform().m_world.m[1][0], cannonTransform->GetTransform().m_world.m[1][1], cannonTransform->GetTransform().m_world.m[1][2], 0.0f));
    XMFLOAT3 velocity;
    XMStoreFloat3(&velocity, forwardWorld * 30.0f);

    XMVECTOR cannonQuat = XMQuaternionRotationMatrix(cannonWorld);
    XMFLOAT4 rotation;
    XMStoreFloat4(&rotation, cannonQuat);

    bullet->SetPosition(tipPos);
    bullet->SetVelocity(velocity);
    bullet->SetRotation(rotation);

    mv_bullets.push_back(std::unique_ptr<Bullet>(bullet));
}
