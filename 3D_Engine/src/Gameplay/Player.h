#pragma once
#include "Bullet.h"

class Player
{
public:
    Player(Scene* scene, Device* device, int wagonTextureIndex = -1);
    ~Player() = default;

    void Update(float deltaTime);
    void Shoot();

    void TakeDamage(int damage);
    int GetHP() const;
    void SetHP(int hp);

    Entity* GetWagon() const { return m_wagon; }
    Entity* GetCannon() const { return m_cannon; }
    Entity* GetTurrel() const { return m_turrel; }

private:
    Entity* m_wagon = nullptr;
    Entity* m_turrel = nullptr;
    Entity* m_cannon = nullptr;
    Scene* m_scene = nullptr;
    Device* m_device = nullptr;

    std::vector<std::unique_ptr<Bullet>> mv_bullets;

    float m_cooldown = 1.f;
    float m_timeToShoot = 0.f;

    int m_hp = 3;
    float m_invincibilityTimer = 0.0f;

    float m_turrelPitch = XM_PIDIV2;

    static constexpr float m_turrelPitchMin = XM_PIDIV2 - 0.6f;
    static constexpr float m_turrelPitchMax = XM_PIDIV2;
};