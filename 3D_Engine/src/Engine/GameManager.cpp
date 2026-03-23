#include "pch.h"
#include "GameManager.h"

GameManager* GameManager::m_instance = nullptr;

GameManager* GameManager::GetInstance()
{
    if (m_instance == nullptr) {
        m_instance = new GameManager();
    }
    return m_instance;
}

void GameManager::Run()
{
    if (m_window && m_engine) {
        m_window->Run(m_engine.get());
    }
}
