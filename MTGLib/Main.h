#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>

#define TARGET_FPS ( 60 )

#include "CommonWinApp.h"
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "xaudio2.lib")

HINSTANCE GetHInstance( void );
int GetCmdShow( void );