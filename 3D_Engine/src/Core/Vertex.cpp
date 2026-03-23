#include "pch.h"
#include "Vertex.h"

Vertex::Vertex()
{
	Identity();
}

void Vertex::Identity()
{
	m_pos = { 0.f, 0.f, 0.f };
	m_color = {1.f, 1.f, 1.f};
	m_normal = { 0.0f, 0.0f, 1.0f };
	m_uv = { 0.0f, 0.0f };
}
