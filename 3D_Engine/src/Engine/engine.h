#pragma once
 
#include "../Core/core.h"

#include "StateMachine.h"

//	ECS
////////

//	entity
#include "Entity.h"
#include "COMPONENT/Component.h"
#include "SYSTEM/System.h"

//	component
#include "COMPONENT/TransformComponent.h"
#include "COMPONENT/PlayerControllerComponent.h"
#include "COMPONENT/StateMachineComponent.h"
#include "COMPONENT/ColliderComponent.h"
#include "COMPONENT/RigidBodyComponent.h"

//	system
#include "SYSTEM/PlayerControllerSystem.h"
#include "SYSTEM/TransformSystem.h"
#include "SYSTEM/StateMachineSystem.h"
#include "SYSTEM/PhysicsSystem.h"

//	SCENE / MANAGER
///////////
#include "Scene.h"
#include "SceneManager.h"
#include "GameManager.h"