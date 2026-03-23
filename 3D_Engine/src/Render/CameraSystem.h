#pragma once

class CameraSystem : public System
{
public:
	CameraSystem();

	void Update(Scene* currentScene, float deltaTime) override;

	bool GetMainCameraMatrices(Scene* currentScene, XMFLOAT4X4& outView, XMFLOAT4X4& outProj);
};

