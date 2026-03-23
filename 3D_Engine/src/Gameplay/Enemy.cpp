#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(Scene* scene, Device* device, Entity* playerTarget, int wagonTextureIndex)
{
    m_scene = scene;
    m_device = device;
    m_playerTarget = playerTarget;
    m_shootTimer = 0.0f;
    m_hp = 1;
    isDead = false;


    m_enemyBase = scene->CreateEntity();
    m_enemyBase->SetTag("Enemy");

    TransformComponent* baseTransform = m_enemyBase->AddComponent<TransformComponent>();
    baseTransform->SetLocalPosition(XMFLOAT3(5.0f, 0.5f, 15.0f)); 
    baseTransform->SetLocalScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

    std::unique_ptr<Mesh> baseMesh = ShapeFactory::CreateBox(device, 2.0f, 2.0f, 2.0f);
    RendererComponent* baseRenderer = m_enemyBase->AddComponent<RendererComponent>();
    baseRenderer->SetMesh(std::move(baseMesh));

    MaterialComponent* baseMat = m_enemyBase->AddComponent<MaterialComponent>();
    baseMat->SetColor(_RED); 

    baseMat->SetTextureIndex(wagonTextureIndex);

    ColliderComponent* baseCollider = m_enemyBase->AddComponent<ColliderComponent>();
    baseCollider->extents = XMFLOAT3(1.0f, 1.0f, 1.0f);

    RigidBodyComponent* baseRB = m_enemyBase->AddComponent<RigidBodyComponent>();
    baseRB->isStatic = true; 
     
    m_turretSphere = scene->CreateEntity();
    m_turretSphere->SetTag("EnemyTurret");

    TransformComponent* turretTransform = m_turretSphere->AddComponent<TransformComponent>();
    turretTransform->SetLocalPosition(XMFLOAT3(0.0f, 1.0f, 0.0f));

    std::unique_ptr<Mesh> turretMesh = ShapeFactory::CreateSphere(device, 0.5f, 20.f, 20.f);
    RendererComponent* turretRenderer = m_turretSphere->AddComponent<RendererComponent>();
    turretRenderer->SetMesh(std::move(turretMesh));

    MaterialComponent* turretMat = m_turretSphere->AddComponent<MaterialComponent>();
    turretMat->SetColor(_WHITE);

    turretMat->SetTextureIndex(wagonTextureIndex);
     
    m_cannonCylinder = scene->CreateEntity();
    m_cannonCylinder->SetTag("EnemyCannon");

    TransformComponent* cannonTransform = m_cannonCylinder->AddComponent<TransformComponent>();
    cannonTransform->SetLocalPosition(XMFLOAT3(0.0f, 0.2f, 0.5f));
    cannonTransform->SetLocalScale(XMFLOAT3(0.3f, 0.7f, 0.3f));
    cannonTransform->RotateLocalX(XM_PIDIV2);

    std::unique_ptr<Mesh> cannonMesh = ShapeFactory::CreateCylinder(device, 0.5f, 0.5f, 2.0f, 20, 20);
    RendererComponent* cannonRenderer = m_cannonCylinder->AddComponent<RendererComponent>();
    cannonRenderer->SetMesh(std::move(cannonMesh));

    MaterialComponent* cannonMat = m_cannonCylinder->AddComponent<MaterialComponent>();
    cannonMat->SetColor(_WHITE);

    cannonMat->SetTextureIndex(wagonTextureIndex);


    m_enemyBase->AddChild(m_turretSphere);
    m_turretSphere->AddChild(m_cannonCylinder);
}

Enemy::~Enemy()
{
}

void Enemy::Update(float deltaTime)
{
    if (m_hp <= 0)
    {
        if (m_enemyBase != nullptr)
        {
            m_enemyBase->Destroy();
            m_enemyBase = nullptr;
            isDead = true;
        }
    }

    else
    {
        if (m_playerTarget != nullptr)
        {
            TransformComponent* playerTc = m_playerTarget->GetComponent<TransformComponent>();
            TransformComponent* turretTc = m_turretSphere->GetComponent<TransformComponent>();

            if (playerTc != nullptr)
            {
                if (turretTc != nullptr)
                { 
                    XMFLOAT3 targetPos = playerTc->GetTransform().m_worldPosition;
                    XMFLOAT3 turretPos = turretTc->GetTransform().m_worldPosition;
                     
                    XMFLOAT3 direction;
                    direction.x = targetPos.x - turretPos.x;
                    direction.y = targetPos.y - turretPos.y;
                    direction.z = targetPos.z - turretPos.z;
                     
                    turretTc->GetTransform().LookTo(direction.x, direction.y, direction.z, XMFLOAT3(0.0f, 1.0f, 0.0f));
                }
            }
        }

        m_shootTimer += deltaTime;

        if (m_shootTimer >= 3.0f)
        {
            Shoot();
            m_shootTimer = 0.0f;
            m_shootCooldown = (float)GenerateRandomNumber(20, 50) / 10.0f;

        }

        ColliderComponent* enemyCollider = m_enemyBase->GetComponent<ColliderComponent>();
        if (enemyCollider->isColliding)
        {
            for (Entity* entity : enemyCollider->collidedEntities)
            {
                if (entity->GetTag() == "BulletPlayer")
                {
                    TakeDamage(1);
                    break;
                }
            }
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

void Enemy::TakeDamage(int damage)
{
    m_hp -= damage;
}

void Enemy::Shoot()
{
    Bullet* bullet = new Bullet(m_scene, m_device);

    TransformComponent* cannonTransform = m_cannonCylinder->GetComponent<TransformComponent>();
    XMMATRIX cannonWorld = XMLoadFloat4x4(&cannonTransform->GetWorldMatrix());

    XMVECTOR tipLocal = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
    XMVECTOR tipWorld = XMVector4Transform(tipLocal, cannonWorld);

    XMFLOAT3 tipPos;
    XMStoreFloat3(&tipPos, tipWorld);

    XMVECTOR forwardWorld = XMVector3Normalize(XMVectorSet(cannonTransform->GetTransform().m_world.m[1][0], cannonTransform->GetTransform().m_world.m[1][1], cannonTransform->GetTransform().m_world.m[1][2], 0.0f));
    XMFLOAT3 velocity;
    XMStoreFloat3(&velocity, forwardWorld * 20.0f); 

    XMVECTOR cannonQuat = XMQuaternionRotationMatrix(cannonWorld);
    XMFLOAT4 rotation;
    XMStoreFloat4(&rotation, cannonQuat);

    bullet->SetPosition(tipPos);
    bullet->SetVelocity(velocity);
    bullet->SetRotation(rotation);
    bullet->SetOwner(m_enemyBase);

    mv_bullets.push_back(std::unique_ptr<Bullet>(bullet));
}

bool Enemy::IsDead() const
{
    return isDead;
}
