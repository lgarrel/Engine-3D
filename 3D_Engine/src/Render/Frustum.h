#pragma once
#include "pch.h"

struct Sphere
{
	XMFLOAT3 center;
	float radius;
};

class Frustum
{
public:
	XMFLOAT4 m_planes[6];

	Frustum();

	void FromViewProj(const XMFLOAT4X4& viewProj);
	bool Intersect(const Sphere& sphere);

};

