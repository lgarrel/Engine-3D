#include "pch.h"

template <typename T>
bool GameManager::Initialize(int width, int height, const std::wstring& name)
{
    m_window = std::make_unique<Window>(width, height, name);
    if (!m_window->Initialize()) {
        return false;
    }

    m_engine = std::make_unique<GraphicsEngine>();
    m_engine->Initialize<T>(m_window->GetHandle(), m_window->GetWidth(), m_window->GetHeight());

    return true;
}