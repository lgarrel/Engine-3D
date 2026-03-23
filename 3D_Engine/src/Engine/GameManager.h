#pragma once

#include "../Core/window.h"
#include "../Render/GraphicsEngine.h"

class GameManager
{
public:
    static GameManager* GetInstance();

    template <typename T>
    bool Initialize(int width, int height, const std::wstring& name);

    void Run();

    GraphicsEngine* GetEngine() { return m_engine.get(); }
    Window* GetWindow() { return m_window.get(); }

private:
    GameManager() = default;
    ~GameManager() = default;

    static GameManager* m_instance;

    std::unique_ptr<Window> m_window;
    std::unique_ptr<GraphicsEngine> m_engine;
};

#include "GameManager.inl"

