#pragma once

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib") 
#pragma comment(lib, "Xinput.lib")

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>    
#include <memory>
#include <DirectXMath.h>
#include <d3dcompiler.h> 
#include <Xinput.h>

using namespace DirectX;
using namespace Microsoft::WRL;

#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <array>
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <functional>
#include <map>


#include "3DEngine_Core.h"

#define WIN_HEIGHT						720
#define WIN_WIDTH						1280

//Core
#include "Transform.h"
#include "Vertex.h"
#include "CommandContext.h"
#include "device.h"
#include "RootSignature.h"
#include "window.h"
#include "Timer.h"
#include "Math_Utils.h"
#include "InputManager.h"


//	using
///////////
using namespace _Math;
using namespace _Utils;
using namespace _3DE;

// std
template <class T>
using Vector							= std::vector<T>;

template <class Key, class Value>
using UnorderedMap						= std::unordered_map<Key, Value>;

template<class Key, class Value>
using Map								= std::map<Key, Value>;

template <class T>
using Function							= std::function<T>;

using String							= std::string;
using WString							= std::wstring;

using WStringView						= std::wstring;
using StringView						= std::string_view;

#define _COUT							std::cout <<
#define _END							<< std::endl;

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


//	Macro
////////////
//#define INIT_WIN						Window _w
//#define WIN_SECU						if (!_w.Initialize()) return -1;
#define STATE_ID(s)						GetStateID<s>()


//	define
////////////
#define _ZERO							1e-20f

//	color
inline XMFLOAT3 _WHITE					= { 1.0f, 1.0f, 1.0f };
inline XMFLOAT3 _BLACK					= { 0.0f, 0.0f, 0.0f };
inline XMFLOAT3 _GRAY					= { 0.5f, 0.5f, 0.5f };

inline XMFLOAT3 _RED					= { 1.0f, 0.0f, 0.0f };
inline XMFLOAT3 _GREEN					= { 0.0f, 1.0f, 0.0f };
inline XMFLOAT3 _BLUE					= { 0.0f, 0.0f, 1.0f };

inline XMFLOAT3 _YELLOW					= { 1.0f, 1.0f, 0.0f };
inline XMFLOAT3 _MAGENTA				= { 1.0f, 0.0f, 1.0f };
inline XMFLOAT3 _CYAN					= { 0.0f, 1.0f, 1.0f };

inline XMFLOAT3 _ORANGE					= { 1.0f, 0.5f, 0.0f };
inline XMFLOAT3 _BROWN					= { 0.5f, 0.5f, 0.0f };


//	inline
////////////
inline XMFLOAT3 _FORWARD				= { 0.f, 0.f, 1.f };
inline XMFLOAT3 _UP						= { 0.f, 1.f, 0.f };
inline XMFLOAT3 _RIGHT					= { 1.f, 0.f, 0.f };

inline XMFLOAT3 _VECT_ZERO				= { 0.f, 0.f, 0.f };
inline XMFLOAT3 _VECT_ONE				= { 1.f ,1.f, 1.f };
