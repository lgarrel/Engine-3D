#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class Component;

class Entity
{
public:
    Entity(int id);
    ~Entity();

    int GetID() const { return m_ID; }
     
    void SetTag(const std::string& tag);
    const std::string& GetTag() const;
    bool CompareTag(const std::string& tag) const;

    template<typename T>
    T* _AddComponent(T* component);

    template<typename T>
    T* AddComponent();

    template<typename T>
    T* GetComponent();

    template<typename T>
    bool HasComponent() const;

    template<typename T>
    void RemoveComponent();

    const std::unordered_map<size_t, Component*>& GetAllComponents() const { return m_components; }

    void SetParent(Entity* parent);
    bool HasParent();
    Entity* GetParent();

    bool HasChild();
    std::vector<Entity*> GetChilds();
    void AddChild(Entity* child);
    void RemoveChild(Entity* child);

    void Destroy() { m_toDestroy = true; }
    bool ShouldDestroy() const { return m_toDestroy; }

private:
    int m_ID;
    bool m_toDestroy = false;
    std::string m_tag = "Untagged";
    std::unordered_map<size_t, Component*> m_components;
    Entity* mp_parent = nullptr;
    std::vector<Entity*> mv_children;

};


template<typename T>
T* Entity::_AddComponent(T* component)
{
    size_t typeHash = typeid(T).hash_code();

    m_components[typeHash] = component;
    return component;
}

template<typename T>
T* Entity::AddComponent()
{
    T* component = new T();
    return _AddComponent(component);
}

template<typename T>
T* Entity::GetComponent()
{
    auto it = m_components.find(typeid(T).hash_code());
    if (it != m_components.end()) {
        return static_cast<T*>(it->second);
    }
    return nullptr;
}

template<typename T>
bool Entity::HasComponent() const
{
    return m_components.find(typeid(T).hash_code()) != m_components.end();
}

template<typename T>
void Entity::RemoveComponent()
{
    auto it = m_components.find(typeid(T).hash_code());
    if (it != m_components.end()) {
        delete it->second;
        m_components.erase(it);
    }
}