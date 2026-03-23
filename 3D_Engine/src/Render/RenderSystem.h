#pragma once
#include "pch.h"
#include "../Engine/SYSTEM/System.h"
#include "Frustum.h"

class Scene;
class CameraSystem;
class CommandContext;
class Device;
class LightSystem;
struct ObjectConstants;
struct PassConstants;
template<typename T> class UploadBuffer; 
struct ID3D12DescriptorHeap;


class RenderSystem : public System
{
public:
	RenderSystem();

	void Initialize(CommandContext* context, Device* device, UploadBuffer<ObjectConstants>* objectCB, UploadBuffer<PassConstants>* passCB, ID3D12DescriptorHeap* cbvHeap, LightSystem* lightSystem);
	void Update(Scene* currentScene, float deltaTime) override;
	void RenderScene(Scene* scene, CameraSystem* cameraSystem);

private:
	void RenderEntity(Entity* entity, int cbIndex);

	CommandContext* m_context = nullptr;
	Device* m_device = nullptr;
	UploadBuffer<ObjectConstants>* m_objectCB = nullptr;
	UploadBuffer<PassConstants>* m_passCB = nullptr;
	ID3D12DescriptorHeap* m_cbvHeap = nullptr;
	LightSystem* m_lightSystem = nullptr;

	Frustum m_frustum;  
};