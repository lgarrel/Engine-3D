#pragma once

#include "../Render/render.h"

#include "resource.h"

//	y'a des truc que j'ai besoin
//////////////////////////////////
#include "S_Rail.h"

//	scenes
///////////
#include "TestScene.h"//include les scene dedant*


#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DEBUG_NEW
#endif
