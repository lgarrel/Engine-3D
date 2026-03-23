#pragma once

struct Light
{
	XMFLOAT3 m_strength;
	float m_fallOffStart;
	XMFLOAT3 m_direction;
	float m_fallOffEnd;
	XMFLOAT3 m_position;
	float m_spotPower;
};


struct Material
{
	XMFLOAT4 m_diffuse;
	XMFLOAT3 m_schlick;
	float m_shininess;
};