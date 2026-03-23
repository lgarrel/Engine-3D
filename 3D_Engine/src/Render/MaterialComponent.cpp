#include "pch.h"
#include "MaterialComponent.h"

MaterialComponent::MaterialComponent()
{
    m_material.m_diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_material.m_schlick = XMFLOAT3(0.01f, 0.01f, 0.01f);
    m_material.m_shininess = 0.5f;
    m_textureIndex = -1;
}

const char* MaterialComponent::GetTypeName() const
{
    return "MaterialComponent";
}

Material& MaterialComponent::GetMaterial()
{
    return m_material;
}

void MaterialComponent::SetMaterial(const Material& mat)
{
    m_material = mat;
}

int MaterialComponent::GetTextureIndex() const
{
    return m_textureIndex;
}

void MaterialComponent::SetTextureIndex(int index)
{
    m_textureIndex = index;
}


void MaterialComponent::SetColor(float _r, float _g, float _b, float _a)
{
	XMFLOAT4 _rgba{ _r, _g, _b, _a };
	SetColor(_rgba);
}

void MaterialComponent::SetColor(XMFLOAT3 _rgb, float _a)
{
	XMFLOAT4 _rgba{ _rgb.x, _rgb.y, _rgb.z, _a };
	SetColor(_rgba);
}

void MaterialComponent::SetColor(XMFLOAT4 _rgba)
{
	m_material.m_diffuse = _rgba;
}
