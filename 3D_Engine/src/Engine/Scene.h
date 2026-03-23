#pragma once

#include "Entity.h"

class Component;

class Scene
{
public:
    Scene();
    virtual ~Scene();


    Entity* CreateEntity();
    void DestroyEntity(Entity* entity);
    void DestroyEntity(int entityID);

    Entity* GetEntityByID(int id);
    const std::vector<Entity*>& GetAllEntities() const { return m_entities; }


    template<typename T>
    std::vector<Entity*> GetEntitiesWithComponent();

    template<typename T>
    std::vector<T*> GetAllComponents();



    virtual void Init() {};
    virtual void Update(float deltaTime);
    virtual void Render() {}

    virtual Entity* GetCamera() const;

protected:
    std::vector<Entity*> m_entities;
    int m_nextId;
};

template<typename T>
std::vector<Entity*> Scene::GetEntitiesWithComponent()
{
    std::vector<Entity*> result;
    for (Entity* entity : m_entities)
    {
        if (entity->HasComponent<T>())
        {
            result.push_back(entity);
        }
    }
    return result;
}

template<typename T>
std::vector<T*> Scene::GetAllComponents()
{
    std::vector<T*> result;
    for (Entity* entity : m_entities)
    {
        if (T* comp = entity->GetComponent<T>())
        {
            result.push_back(comp);
        }
    }
    return result;
}