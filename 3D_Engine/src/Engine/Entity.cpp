#include "pch.h"
#include "Entity.h"

Entity::Entity(int id)
{
	m_ID = id;
}

Entity::~Entity()
{
	for (auto& pair : m_components) {
		delete pair.second;
	}
	m_components.clear();
}

void Entity::SetTag(const std::string& tag)
{
	m_tag = tag;
}

const std::string& Entity::GetTag() const
{
	return m_tag;
}

bool Entity::CompareTag(const std::string& tag) const
{
	return m_tag == tag;
}

void Entity::SetParent(Entity* parent)
{
	mp_parent = parent;
	parent->mv_children.push_back(this);
	std::cout << "ID PARENT : " << parent->m_ID << std::endl;
}

bool Entity::HasParent()
{
	if (mp_parent != nullptr)
		return true;
	
	return false;
}

Entity* Entity::GetParent()
{
	return mp_parent;
}

bool Entity::HasChild()
{
	if(mv_children.empty() == false)
		return true;

	return false;
}

std::vector<Entity*> Entity::GetChilds()
{
	return mv_children;
}

void Entity::AddChild(Entity* child)
{
	if (child->mp_parent == this)
		return;

	if (child->mp_parent != nullptr)
	{
		child->mp_parent->RemoveChild(child);
	}

	mv_children.push_back(child);
	child->mp_parent = this;
	std::cout << "ID PARENT : " << m_ID << std::endl;
}

void Entity::RemoveChild(Entity* child)
{
	if (child->mp_parent != this) 
		return;

	for (int i = 0; i < mv_children.size(); i++)
	{
		if (mv_children[i] == child)
		{
			mv_children.erase(mv_children.begin() + i);
			break;
		}
	}
	child->mp_parent = nullptr;
}

