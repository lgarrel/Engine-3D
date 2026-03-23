#pragma once

class Entity;

class Component
{
protected:
	bool m_needToBeDestroyed;

	Entity* mp_owner = nullptr;

public:
	Component();
	virtual ~Component() = default;

	bool GetNeedToBeDestroyed() const;
	void SetNeedToBeDestroyed(bool state);

	void SetOwner(Entity* _pOwner) { mp_owner = _pOwner; }
	//Entity* GetOwner() { return mp_owner; }

	virtual const char* GetTypeName() const = 0;
};




