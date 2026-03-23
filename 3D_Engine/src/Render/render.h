#pragma once

#include "../Engine/engine.h"

// UTILITAIRE
#include "ShaderUtils.h"
#include "PSOUtils.h"
#include "UploadBuffer.h" 
#include "SwapChain.h"
#include "Light.h"         

// G�OM�TRIE
#include "GeomVertex.h" 
#include "MeshData.h"
#include "Mesh.h"
#include "GeometryGenerator.h"
#include "ShapeFactory.h"

// CAM�RA
#include "Frustum.h"      
#include "Camera.h"

// COMPOSANTS
#include "UIRendererComponent.h"
#include "MaterialComponent.h"  
#include "CameraComponent.h"
#include "RenderComponent.h"
#include "LightComponent.h"

// MOTEUR  
#include "GraphicsEngine.h"

// SYST�MES 
#include "LightSystem.h"
#include "CameraSystem.h"
#include "RenderSystem.h"
#include "RenderUI.h"
