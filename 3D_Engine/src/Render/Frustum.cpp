#include "pch.h"

Frustum::Frustum()
{
	for (int i = 0; i < 6; i++)
		m_planes[i] = { 0.0f, 0.0f, 0.0f, 0.0f };
}

void Frustum::FromViewProj(const DirectX::XMFLOAT4X4& viewProj)
{
    //Pour calculer le frustum (en gros ce que la caméra affiche )

    XMVECTOR colX = XMVectorSet(viewProj._11, viewProj._21, viewProj._31, viewProj._41);
    XMVECTOR colY = XMVectorSet(viewProj._12, viewProj._22, viewProj._32, viewProj._42);
    XMVECTOR colZ = XMVectorSet(viewProj._13, viewProj._23, viewProj._33, viewProj._43);
    XMVECTOR colW = XMVectorSet(viewProj._14, viewProj._24, viewProj._34, viewProj._44);

    XMStoreFloat4(&m_planes[0], XMPlaneNormalize(XMVectorAdd(colW, colX)));      // Left
    XMStoreFloat4(&m_planes[1], XMPlaneNormalize(XMVectorSubtract(colW, colX))); // Right
    XMStoreFloat4(&m_planes[2], XMPlaneNormalize(XMVectorAdd(colW, colY)));      // Bottom
    XMStoreFloat4(&m_planes[3], XMPlaneNormalize(XMVectorSubtract(colW, colY))); // Top

    XMStoreFloat4(&m_planes[4], XMPlaneNormalize(colZ));                         // Near

    XMStoreFloat4(&m_planes[5], XMPlaneNormalize(XMVectorSubtract(colW, colZ))); // Far
}

bool Frustum::Intersect(const Sphere& sphere)
{ 
    //Calcule si c'est visible ou non (dans le champ de la cam ou pas) en calculant si il est bien devant chaque plan et pas cachée derrière

    XMVECTOR sphereCenter = XMVectorSet(sphere.center.x, sphere.center.y, sphere.center.z, 1.0f);

    for (const auto& plane : m_planes)
    {
        XMVECTOR p = XMLoadFloat4(&plane);

        XMVECTOR distVec = XMPlaneDotCoord(p, sphereCenter);

        if (XMVectorGetX(distVec) < -sphere.radius)
        {
            return false; // Pas dans le champ
        }
    }

    return true; // dans le champ
}
