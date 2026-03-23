#pragma once

class Scene;
class Device;
class Entity;

class Enemy
{
public:
    Enemy(Scene* scene, Device* device, Entity* playerTarget, int wagonTextureIndex = -1);
    ~Enemy();

    void Update(float deltaTime);
    void TakeDamage(int damage);
    void Shoot();
    Entity* GetEntity() { return m_enemyBase; }
    bool IsDead() const;

private:
    Scene* m_scene = nullptr;
    Device* m_device = nullptr;

    Entity* m_enemyBase = nullptr;
    Entity* m_turretSphere = nullptr;
    Entity* m_cannonCylinder = nullptr;
    Entity* m_playerTarget = nullptr;

    std::vector<std::unique_ptr<Bullet>> mv_bullets;

    float m_shootTimer;
    float m_shootCooldown;
    int m_hp;
    bool isDead;
};