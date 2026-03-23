#pragma once

class UIRendererComponent : public Component
{
public:
    XMFLOAT2 position;
    XMFLOAT2 size;
    XMFLOAT4 uvRect;
     
    std::string text;
    XMFLOAT4 color;
    int textureIndex;  

    UIRendererComponent(XMFLOAT2 pos = { 0.f, 0.f },
        XMFLOAT2 sz = { 100.f, 100.f },
        XMFLOAT4 uvs = { 0.f, 0.f, 1.f, 1.f },
        std::string txt = "",
        XMFLOAT4 col = { 1.f, 1.f, 1.f, 1.f },
        int texIdx = -1);

    const char* GetTypeName() const override { return "UIRendererComponent"; }
};
