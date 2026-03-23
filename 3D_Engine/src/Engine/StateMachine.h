#pragma once

class Entity;

class State
{
protected:
	Entity* mp_owner;

public:
	virtual void OnStart() {}
	virtual void OnUpdate() {}
	virtual void OnEnd() {}

	void SetOwner(Entity* _pOwner) { mp_owner = _pOwner; }
};

class GlobalState : public State
{
public:
	virtual void OnCollisionEnter(Entity* other) {};
	virtual void OnCollisionStay(Entity* other) {};
	virtual void OnCollisionExit(Entity* other) {};
};

///*#ifndef STATE_MACHINE_COMPONENT_H_DEFINED
//#define STATE_MACHINE_COMPONENT_H_DEFINED
//
//#include "Component.h"
//
//class Entity;
//
//class State
//{
//public:
//	virtual void OnStart() {};
//	virtual void OnUpdate() {};
//	virtual void OnEnd() {};
//
//	void SetOwner(Entity* _pOwner) { m_pOwner = _pOwner; }
//
//protected:
//	Entity* m_pOwner = nullptr;
//
//};
//
//class StateGlobal : public State
//{
//public:
//	virtual void OnCollisionEnter(Entity* other) {};
//	virtual void OnCollisionStay(Entity* other) {};
//	virtual void OnCollisionExit(Entity* other) {};
//};
//
//class StateMachineComponent : public Component
//{
//public:
//	inline static bool hasBeenRegister = false;
//	inline static uint64_t typeMask = 0;
//
//	void SetStateGlobal(StateGlobal* _pState);
//	StateGlobal* GetStateGlobal() { return m_pStateGlobal; }
//
//	template <typename StateType>
//	StateType* AddState();
//
//	void ToState(int to);
//
//	std::vector<State*> m_vStates;
//	int m_currentIndex = -1;
//
//private:
//	StateGlobal* m_pStateGlobal = nullptr;
//
//	friend class StateMachineSystem;
//};*/

//#include "StateMachineComponent.inl"

//#endif