#pragma once

class StateMachineComponent : public Component
{
	int m_currentStateIndex = -1;
	Vector<State*> mv_states;

	GlobalState* mp_globalState = nullptr;

public:
	const char* GetTypeName() const override { return "StateMachineComponent"; }

	void SetGlobalState(GlobalState* _pGlobal);
	GlobalState* GetGlobalState() { return mp_globalState; };

	template <typename Type>
	Type* AddState();

	void ToState(int _to);
private:
	//friend class StateMachineSystem;
};

#include "StateMachineComponent.inl"

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
