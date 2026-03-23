#pragma once

class CameraComponent : public Component
{
	Camera m_camera;

public:
	CameraComponent() = default;
	CameraComponent(float fovY, float aspectRatio, float nearZ, float farZ);

	const char* GetTypeName() const override { return "CameraComponent"; }
	Camera& GetCamera() { return m_camera; }


	void SetPerspective(float fovY, float aspectRatio, float nearZ, float farZ);
	void SetAspectRatio(float aspectRatio);

	//GETTER

	const XMFLOAT4X4& GetView() { return m_camera.GetView(); }
	const XMFLOAT4X4& GetProj() { return m_camera.GetProj(); }

	float GetFov() { return m_camera.GetFOV(); }
	float GetAspectRatio() { return m_camera.GetAspect(); }
	float GetNear() { return m_camera.GetNear(); }
	float GetFar() { return m_camera.GetFar(); }
};

