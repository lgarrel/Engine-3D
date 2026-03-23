#include "pch.h"
#include "Scene.h"

Scene::Scene()
{
	m_nextId = 0;
}

Scene::~Scene()
{
	for (Entity* entity : m_entities) 
	{
		delete entity;
	}
	m_entities.clear();
}

Entity* Scene::CreateEntity()
{
	Entity* e = new Entity(m_nextId++);
	m_entities.push_back(e);
	return e;
}

void Scene::DestroyEntity(Entity* entity)
{
	if (entity == nullptr) 
		return;

	if (entity->GetParent())
	{
		entity->GetParent()->RemoveChild(entity);
	}

	std::vector<Entity*> children = entity->GetChilds();
	for (Entity* child : children)
	{
		DestroyEntity(child);
	}


	auto it = std::find(m_entities.begin(), m_entities.end(), entity);

	if (it != m_entities.end()) 
	{
		delete* it;
		m_entities.erase(it);
	}
}

Entity* Scene::GetCamera() const
{
	return nullptr;
}

void Scene::DestroyEntity(int entityID)
{
	for (auto it = m_entities.begin(); it != m_entities.end(); it++) 
	{
		if ((*it)->GetID() == entityID) 
		{
			delete* it;
			m_entities.erase(it);
			return;
		}
	}
}

Entity* Scene::GetEntityByID(int id)
{
	for (Entity* entity : m_entities) 
	{
		if (entity->GetID() == id) 
		{
			return entity;
		}
	}
	return nullptr;
}

void Scene::Update(float deltaTime)
{
	std::vector<Entity*> toDestroy;
	for (Entity* entity : m_entities)
	{
		if (entity->ShouldDestroy())
		{
			toDestroy.push_back(entity);
		}
	}

	for (Entity* entity : toDestroy)
	{
		DestroyEntity(entity);
	}
}
