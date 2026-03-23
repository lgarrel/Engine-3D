#pragma once

class Bullet
{
public:
	Bullet(Scene* scene, Device* device);
	~Bullet() = default;

	void Update(float deltaTime);
	void Destroy();


	void SetPosition(XMFLOAT3 pos);
	void SetVelocity(XMFLOAT3 vel);
	void SetRotation(XMFLOAT4 quat);

	bool ShouldDestroy();


private:

	Entity* m_bullet = nullptr;
	Scene* m_scene = nullptr;
};

