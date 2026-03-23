#include "pch.h"
#include "Player.h"



Player::Player(Scene* scene, Device* device, int wagonTextureIndex)
{
    m_scene = scene;
    m_device = device;

    // Create Wagon
    m_wagon = scene->CreateEntity(); 
    m_wagon->SetTag("Player");

    TransformComponent* wagonTransform = m_wagon->AddComponent<TransformComponent>();
    wagonTransform->SetLocalPosition(XMFLOAT3(0.0f, 1.f, 0.0f));
    wagonTransform->SetLocalScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

    /*std::unique_ptr<Mesh> wagonMesh = ShapeFactory::CreateBox(device, 2.0f, 2.0f, 2.0f);
    RendererComponent* wagonRenderer = m_wagon->AddComponent<RendererComponent>();
    wagonRenderer->SetMesh(std::move(wagonMesh));*/

    MaterialComponent* wagonMat = m_wagon->AddComponent<MaterialComponent>();
    wagonMat->SetColor(_WHITE);

    wagonMat->SetTextureIndex(wagonTextureIndex);

    PlayerControllerComponent* playerControl = m_wagon->AddComponent<PlayerControllerComponent>();
    playerControl->m_movementTarget = m_wagon;



    ColliderComponent* wagonCollider = m_wagon->AddComponent<ColliderComponent>();
    wagonCollider->extents = XMFLOAT3(1.0f, 1.f, 1.55f);
    wagonCollider->offset = XMFLOAT3(0.0f, 0.0f, 0.5f);

    RigidBodyComponent* wagonRB = m_wagon->AddComponent<RigidBodyComponent>();
    wagonRB->isStatic = false;

    // Create Turrel
    m_turrel = scene->CreateEntity();
    m_turrel->SetTag("Turrel");

    TransformComponent* turrelTransform = m_turrel->AddComponent<TransformComponent>();
    turrelTransform->SetLocalPosition(XMFLOAT3(0.0f, 1.f, 0.f));

    /*std::unique_ptr<Mesh> turrelMesh = ShapeFactory::CreateSphere(device, 0.5f, 20.f, 20.f);
    RendererComponent* turrelRenderer = m_turrel->AddComponent<RendererComponent>();
    turrelRenderer->SetMesh(std::move(turrelMesh));*/

    MaterialComponent* turrelMat = m_turrel->AddComponent<MaterialComponent>();
    turrelMat->SetColor(_WHITE);
    turrelMat->SetTextureIndex(wagonTextureIndex);

    playerControl->m_rotationTarget = m_turrel;


    // Create Cannon
    m_cannon = scene->CreateEntity();
    m_cannon->SetTag("Cannon");

    TransformComponent* cannonTransform = m_cannon->AddComponent<TransformComponent>();
    cannonTransform->SetLocalPosition(XMFLOAT3(0.0f, 0.2f, 1.f));
    cannonTransform->SetLocalScale(XMFLOAT3(0.3f, 0.7f, 0.3f));
    cannonTransform->RotateLocalX(XM_PIDIV2);

    /*std::unique_ptr<Mesh> cannonMesh = ShapeFactory::CreateCylinder(device, 0.5f, 0.5f, 2.0f, 20, 20);
    RendererComponent* cannonRenderer = m_cannon->AddComponent<RendererComponent>();
    cannonRenderer->SetMesh(std::move(cannonMesh));*/

    MaterialComponent* cannonMat = m_cannon->AddComponent<MaterialComponent>();
    cannonMat->SetColor(_WHITE);

    cannonMat->SetTextureIndex(wagonTextureIndex);

    m_wagon->AddChild(m_turrel);
    m_turrel->AddChild(m_cannon);

}

void Player::Update(float deltaTime)
{

    m_timeToShoot += deltaTime;

    if (m_invincibilityTimer > 0.0f)
    {
        m_invincibilityTimer -= deltaTime;
    }

    ColliderComponent* wagonCollider = m_wagon->GetComponent<ColliderComponent>();
    if (wagonCollider != nullptr)
    {
        if (wagonCollider->isColliding)
        {
            for (Entity* entity : wagonCollider->collidedEntities)
            {
                if (entity->GetTag() == "BulletEnemy")
                {
                    TakeDamage(1);
                }
            }
        }
    }
    m_timeToShoot += deltaTime;

    InputManager* input = InputManager::GetInstance();
    if (input->IsKeyPressed('A') || input->IsMouseButtonDown(0) || input->IsRightTriggerPressed())
    {
        if (m_timeToShoot >= m_cooldown)
        {
            Shoot();
            m_timeToShoot = 0.f;
        }
    }

    auto tc = m_turrel->GetComponent<TransformComponent>();

    float delta = 0.f;

    if (input->IsKeyDown(VK_SHIFT) || input->GetRightStickY() < -0.1f)
        delta = deltaTime; 

    if (input->IsKeyDown(VK_SPACE) || input->GetRightStickY() > 0.1f)
        delta = -deltaTime;  

    if (delta != 0.f)
    {
        float newPitch = m_turrelPitch + delta;

        newPitch = XMMax(m_turrelPitchMin, XMMin(m_turrelPitchMax, newPitch));

        float realDelta = newPitch - m_turrelPitch;
        m_turrelPitch = newPitch;

        if (realDelta != 0.f)
            tc->RotateLocalX(realDelta);
    }

    auto wagonTc = m_wagon->GetComponent<TransformComponent>();
    XMFLOAT3 pos = wagonTc->GetLocalPosition();
    //wagonTc->SetLocalPosition({ 0.0f, 1.0f, 0.f });

    if (wagonTc->GetLocalPosition().x < -0.5f && wagonTc->GetLocalPosition().x > 0.5f)
        wagonTc->SetLocalPosition({0.0f, 1.0f, 0.0f});

    if (wagonTc->GetLocalPosition().z < 0.1f)
        wagonTc->SetLocalPosition({0.0f, 1.0f, 0.1f});

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

void Player::Shoot()
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
    bullet->SetOwner(m_wagon);

    mv_bullets.push_back(std::unique_ptr<Bullet>(bullet));
}

void Player::TakeDamage(int damage)
{
    if (m_invincibilityTimer <= 0.0f)
    {
        m_hp -= damage;
        if (m_hp < 0)
        {
            m_hp = 0;
        }
        m_invincibilityTimer = 1.0f; 
    }
}

int Player::GetHP() const
{
    return m_hp;
}

void Player::SetHP(int hp)
{
    m_hp = hp;
}
