#include "pch.h"
#include "Bullet.h"

Bullet::Bullet(Scene* scene, Device* device)
{
	m_scene = scene;

	// Create Bullet
    m_bullet = scene->CreateEntity();
    m_bullet->SetTag("Bullet");

    TransformComponent* bulletTransform = m_bullet->AddComponent<TransformComponent>();
    bulletTransform->SetLocalScale(XMFLOAT3(0.3f, 0.3f, 0.3f));

    std::unique_ptr<Mesh> bulletMesh = ShapeFactory::CreateCylinder(device, 0.5f, 0.2f, 2.0f, 20, 20);
    RendererComponent* bulletRenderer = m_bullet->AddComponent<RendererComponent>();
    bulletRenderer->SetMesh(std::move(bulletMesh));

    MaterialComponent* bulletMat = m_bullet->AddComponent<MaterialComponent>();
    bulletMat->GetMaterial().m_diffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);

    ColliderComponent* bulletCollider = m_bullet->AddComponent<ColliderComponent>();
    bulletCollider->extents = XMFLOAT3(0.25f, 0.25f, 0.1f);

    RigidBodyComponent* bulletRB = m_bullet->AddComponent<RigidBodyComponent>();
    bulletRB->isStatic = false;
    bulletRB->useGravity = false;
}

void Bullet::SetOwner(Entity* shoot)
{
    m_owner = shoot;
    m_bullet->SetTag(m_bullet->GetTag() + m_owner->GetTag());
    std::cout << m_bullet->GetTag();

    if (m_owner->GetTag() == "Player")
    {
        MaterialComponent* bulletMat = m_bullet->GetComponent<MaterialComponent>();
        bulletMat->SetColor(_MAGENTA);
    }

    if (m_owner->GetTag() == "Enemy")
    {
        MaterialComponent* bulletMat = m_bullet->GetComponent<MaterialComponent>();
        bulletMat->SetColor(_RED);
    }
}

void Bullet::Update(float deltaTime)
{
    if (m_bullet == nullptr)
        return;

    ColliderComponent* bulletCollider = m_bullet->GetComponent<ColliderComponent>();
    if(bulletCollider->isColliding)
    {
        for (Entity* entity : bulletCollider->collidedEntities)
        {
            if (entity->GetTag() != m_owner->GetTag())
            {
                Destroy();
                break;
            }
        }
    }

    TransformComponent* bTrans = m_bullet->GetComponent<TransformComponent>();
    if(bTrans->GetTransform().m_localPosition.y <= -3.f)
    {
        Destroy();
    }
}

void Bullet::Destroy()
{
    if (m_bullet)
    {
        std::cout << "Bullet Detruite" << std::endl;
        m_bullet->Destroy();
    }
}

void Bullet::SetPosition(XMFLOAT3 pos)
{
    if (m_bullet)
    {
        m_bullet->GetComponent<TransformComponent>()->SetLocalPosition(pos);
    }
}

void Bullet::SetVelocity(XMFLOAT3 vel)
{
    if (m_bullet)
    {
        m_bullet->GetComponent<RigidBodyComponent>()->velocity = vel;
    }
}

void Bullet::SetRotation(XMFLOAT4 quat)
{
    if (m_bullet)
    {
        m_bullet->GetComponent<TransformComponent>()->GetTransform().m_localQuaternion = quat;
        m_bullet->GetComponent<TransformComponent>()->GetTransform().SetRotationFromQuaternion();
    }
}


bool Bullet::ShouldDestroy()
{
    if (m_bullet == nullptr)
    {
        return true;
    }

    if (m_bullet->ShouldDestroy())
    {
        return true;
    }

    return false;
}
