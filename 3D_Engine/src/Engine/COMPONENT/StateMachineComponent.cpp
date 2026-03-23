#include "pch.h"
#include "StateMachineComponent.h"


/*#ifndef STATE_MACHINE_COMPONENT_CPP_DEFINED
#define STATE_MACHINE_COMPONENT_CPP_DEFINED

#include "StateMachineComponent.h"

#endif

void StateMachineComponent::SetStateGlobal(StateGlobal* _pState)
{
	m_pStateGlobal = _pState;
	m_pStateGlobal->SetOwner(m_pEntity);
	m_pStateGlobal->OnStart();
}

void StateMachineComponent::ToState(int to)
{
	if (m_currentIndex != -1)
		m_vStates[m_currentIndex]->OnEnd();

	m_currentIndex = to;

	if (m_currentIndex != -1)
		m_vStates[m_currentIndex]->OnStart();
}*/

void StateMachineComponent::SetGlobalState(GlobalState* _pGlobal)
{
	mp_globalState = _pGlobal;
	mp_globalState->SetOwner(mp_owner);
	mp_globalState->OnStart();
}

void StateMachineComponent::ToState(int _to)
{
	if (m_currentStateIndex != -1)
		mv_states[m_currentStateIndex]->OnEnd();

	m_currentStateIndex = _to;

	if (m_currentStateIndex != -1)
		mv_states[m_currentStateIndex]->OnStart();
}
