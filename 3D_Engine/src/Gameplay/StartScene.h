#pragma once
#include "pch.h"
#include "Scene.h"

class StartScene : public Scene
{
private:
    Entity* m_uiStartScreen;
    Entity* m_cameraEntity;  
    Device* m_device;
    GraphicsEngine* m_graphicsEngine;

public:
    StartScene();
    ~StartScene();

    void SetDevice(Device* device);
    void SetGraphicsEngine(GraphicsEngine* engine);

    void Init() override;
    void Update(float deltaTime) override;
    void Render() override;


    Entity* GetCamera() const;
};