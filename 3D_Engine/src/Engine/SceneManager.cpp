
#include "pch.h"
#include "SceneManager.h"

SceneManager* SceneManager::mp_instance = nullptr;

SceneManager::SceneManager()
{
    mp_currentScene = nullptr;
}

void SceneManager::Init()
{

}

SceneManager* SceneManager::GetInstance()
{
    if (mp_instance == nullptr)
    {
        mp_instance = new SceneManager();
    }
    return mp_instance;
}

Scene* SceneManager::GetCurrentScene()
{

    if (!mp_currentScene) {
        std::cerr << "Erreur : currentScene est nullptr !" << std::endl;
    }
    return mp_currentScene;

}
