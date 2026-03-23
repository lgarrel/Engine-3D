#pragma once

class TransformSystem : public System
{
public:
	TransformSystem();

	void UpdateEntity(Entity* entity);
	void Update(Scene* currentScene, float deltaTime) override;
};

