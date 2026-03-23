#include "pch.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	m_transform.Identity();
}

TransformComponent::TransformComponent(const Transform& trans)
	: m_transform(trans)
{
}

void TransformComponent::SetLocalPosition(const XMFLOAT3& pos)
{
	m_transform.m_localPosition = pos;
}

void TransformComponent::SetLocalScale(XMFLOAT3 scale)
{
	m_transform.SetLocalScale(scale);
}

void TransformComponent::LocalScale(float scale)
{
	m_transform.LocalScale(scale);
}

void TransformComponent::SetLocalForward(XMFLOAT3 _forw)
{
	m_transform.m_forward = _forw;
}

void TransformComponent::RotateLocalX(float radians)
{
	m_transform.RotateLocalX(radians);
}

void TransformComponent::RotateLocalY(float radians)
{
	m_transform.RotateLocalY(radians);
}

void TransformComponent::RotateLocalZ(float radians)
{
	m_transform.RotateLocalZ(radians);
}

void TransformComponent::MoveLocal(float x, float y, float z)
{
	m_transform.MoveLocal(x, y, z);
}

void TransformComponent::MoveLocalForward(float d)
{
	m_transform.MoveForwardLocal(d);
}

void TransformComponent::MoveLocalRight(float d)
{
	m_transform.MoveRightLocal(d);
}

void TransformComponent::MoveLocalUp(float d)
{
	m_transform.MoveUpLocal(d);
}