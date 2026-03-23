#pragma once

#include "../Engine/COMPONENT/Component.h"

class MaterialComponent : public Component
{
public:
    MaterialComponent();

    const char* GetTypeName() const override;

    Material& GetMaterial();
    void SetMaterial(const Material& mat);

    int GetTextureIndex() const;
    void SetTextureIndex(int index);

    //color
    void SetColor(float _r, float _g, float _b, float _a = 1.0f);
    void SetColor(XMFLOAT3 _rgb, float _a = 1.0f);
    void SetColor(XMFLOAT4 _rgba);

private:
    Material m_material;
    int m_textureIndex;
};