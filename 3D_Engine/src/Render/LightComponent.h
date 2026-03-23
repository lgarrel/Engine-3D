#pragma once


class LightComponent : public Component
{
public:
    enum class LightType
    {
        Directional,
        Point,
        Spot
    };

    LightComponent()
    {
        m_light.m_strength = { 1.0f, 1.0f, 1.0f };
        m_light.m_direction = { 0.0f, -1.0f, 0.0f };
        m_light.m_position = { 0.0f, 10.0f, 0.0f };
        m_light.m_fallOffStart = 1.0f;
        m_light.m_fallOffEnd = 10.0f;
        m_light.m_spotPower = 64.0f;
        m_type = LightType::Directional;
    }

    const char* GetTypeName() const override { return "LightComponent"; }

    void SetType(LightType type) { m_type = type; }
    LightType GetType() const { return m_type; }

    void SetStrength(const XMFLOAT3& strength) { m_light.m_strength = strength; }
    const XMFLOAT3& GetStrength() const { return m_light.m_strength; }

    void SetDirection(const XMFLOAT3& direction) { m_light.m_direction = direction; }
    const XMFLOAT3& GetDirection() const { return m_light.m_direction; }

    void SetFalloffStart(float start) { m_light.m_fallOffStart = start; }
    float GetFalloffStart() const { return m_light.m_fallOffStart; }

    void SetFalloffEnd(float end) { m_light.m_fallOffEnd = end; }
    float GetFalloffEnd() const { return m_light.m_fallOffEnd; }

    void SetSpotPower(float power) { m_light.m_spotPower = power; }
    float GetSpotPower() const { return m_light.m_spotPower; }

    const Light& GetLightData() const { return m_light; }
    Light& GetLightData() { return m_light; }

private:
    Light m_light;
    LightType m_type;
};