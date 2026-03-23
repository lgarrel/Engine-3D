#include "pch.h"
#include "Component.h"

Component::Component()
{
	m_needToBeDestroyed = false;
}

bool Component::GetNeedToBeDestroyed() const
{
	return m_needToBeDestroyed;
}

void Component::SetNeedToBeDestroyed(bool states)
{
	m_needToBeDestroyed = states;
}


