#pragma once

#include "Scene.h"


class SceneManager
{
	static SceneManager* mp_instance;
	Scene* mp_currentScene;
	std::vector<Scene*> mv_mScenes;

public:

	SceneManager();
	~SceneManager() = default;
	void Init();

	template <typename S>
	void SetCurrentScene();
	template <typename S>
	S* Add();
	template <typename S>
	void DeleteScene();
	template <typename S>
	S* Get();
	template <typename S>
	std::vector<S*> GetAll();

	static SceneManager* GetInstance();
	Scene* GetCurrentScene();
};

template <typename S>
void SceneManager::SetCurrentScene()
{
	for (int i = 0; i < mv_mScenes.size(); i++)
	{
		if (dynamic_cast<S*>(mv_mScenes[i]))
		{
			mp_currentScene = mv_mScenes[i];
			mp_currentScene->Init();
			return;
		}
	}
}

template <typename S>
S* SceneManager::Add()
{
	S* s = new S();
	mv_mScenes.push_back(s);
	return s;
}

template <typename S>
void SceneManager::DeleteScene()
{
	mv_mScenes.erase(std::remove_if(mv_mScenes.begin(), mv_mScenes.end(),
		[](Scene* e) {
			return dynamic_cast<S*>(e);
		}),
		mv_mScenes.end());

	for (auto* e : mv_mScenes) {
		if (dynamic_cast<S*>(e)) {
			delete e;
		}
	}
}
template <typename S>
S* SceneManager::Get()
{
	for (int i = 0; i < mv_mScenes.size(); i++)
	{
		if (S* elem = dynamic_cast<S*>(mv_mScenes[i]))
		{
			return elem;
		}
	}

	return nullptr;
}

template <typename S>
std::vector<S*> SceneManager::GetAll()
{
	std::vector<S*> v;
	for (Scene* scene : mv_mScenes)
	{
		if (S* casted = dynamic_cast<S*>(scene))
		{
			v.push_back(casted);
		}
	}
	return v;
}