#pragma once

class TransformComponent : public Component
{
private:
	Transform m_transform;

public:
	TransformComponent();
	TransformComponent(const Transform& trans);

	const char* GetTypeName() const override { return "TransformComponent"; }

	// Getter
	Transform& GetTransform() { return m_transform; }
	const Transform& GetTransform() const { return m_transform; }

	XMFLOAT3 GetLocalPosition() { return m_transform.m_localPosition; }

	void SetLocalPosition(const XMFLOAT3& pos);
	void SetLocalScale(XMFLOAT3 scale);
	void LocalScale(float scale);

	void SetLocalForward(XMFLOAT3 _forw);

	void RotateLocalX(float radians);
	void RotateLocalY(float radians);
	void RotateLocalZ(float radians);

	void MoveLocal(float x, float y, float z);
	void MoveLocalForward(float d);
	void MoveLocalRight(float d);
	void MoveLocalUp(float d);

	const XMFLOAT4X4& GetWorldMatrix() { return m_transform.m_world; }
	
private:
	//friend class TransformSystem;

};

