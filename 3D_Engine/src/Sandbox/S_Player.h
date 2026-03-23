#pragma once
#include "Bullet.h"

class S_Player
{
public:
    S_Player(Scene* scene, Device* device, int wagonTextureIndex = -1);
    ~S_Player() = default;

    void Update(float deltaTime);
    void Shoot();

    Entity* GetWagon() const { return m_wagon; }
    Entity* GetCannon() const { return m_cannon; }

private:
    Entity* m_wagon = nullptr;
    Entity* m_turrel = nullptr;
    Entity* m_cannon = nullptr;
    Scene* m_scene = nullptr;
    Device* m_device = nullptr;

    std::vector<std::unique_ptr<Bullet>> mv_bullets;

    float m_cooldown = 2.f;
    float m_timeToShoot = 0.f;

    float m_turrelPitch = XM_PIDIV2;

    static constexpr float m_turrelPitchMin = XM_PIDIV2 - 0.6f;
    static constexpr float m_turrelPitchMax = XM_PIDIV2;
};
