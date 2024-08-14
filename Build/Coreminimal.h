#pragma once
#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf ‚Ìwarning–h~
#include <stdio.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>

#include <time.h>

#include <vector>
#include <string>
#include <algorithm>

#include<fbxsdk.h>

//#include<DirectXTex.h>
//
//#include"ScreenGrab11.h"
//
//#include <wincodec.h>

// –{—ˆ‚Íƒwƒbƒ_‚É‘‚©‚È‚¢•û‚ª—Ç‚¢
using namespace DirectX;
using namespace std;

#define DIRECTINPUT_VERSION 0x0800		// Œx‘Îˆ
#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)


//*****************************************************************************
// ƒ‰ƒCƒuƒ‰ƒŠ‚ÌƒŠƒ“ƒN
//*****************************************************************************
#pragma comment (lib, "d3d11.lib")		
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")	
#pragma comment (lib, "d3dx9.lib")	
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

#pragma comment (lib,"libfbxsdk-mt.lib")
#pragma comment (lib,"libxml2-mt.lib")
#pragma comment (lib,"zlib-mt.lib")

#include"math.h"
#include"collision.h"