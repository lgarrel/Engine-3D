#include "pch.h"
#include "UIRendererComponent.h"

UIRendererComponent::UIRendererComponent(XMFLOAT2 pos, XMFLOAT2 sz, XMFLOAT4 uvs, std::string txt, XMFLOAT4 col, int texIdx) : position(pos), size(sz), uvRect(uvs), text(txt), color(col), textureIndex(texIdx)
{

}
