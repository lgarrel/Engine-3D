#pragma once

class Vertex
{
public:

	XMFLOAT3 m_pos;
	XMFLOAT3 m_color;
	XMFLOAT3 m_normal;
	XMFLOAT2 m_uv;

	Vertex();

	void Identity();
};

