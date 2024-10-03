//=============================================================================
// 
//  プリコンパイルヘッダー [precompile.h]
//  Author : 相馬靜雅＆東先生
// 
//=============================================================================
#ifndef _PRECOMPIRE_H_
#define _PRECOMPIRE_H_	// 二重インクルード防止

//#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <DirectXMath.h>
#include <D3DX9Math.h>
#include <assert.h>

// C++のstd用
#include <vector>
#include <algorithm>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iterator>
#include <codecvt>
#include <fstream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <stack>
#include <filesystem>

#include <d3dx9.h>						// 描画処理に必要
#define DIRECTINPUT_VERSION	(0x0800)	// ビルド時の警告用マクロ
#include <dinput.h>						// 入力処理に必要
#include <xaudio2.h>					// サウンド処理に必要
#include <dsound.h>						// サウンド内部処理に必要
#include <Xinput.h>

// マイライブラリ
#include "mylib.h"
#include "myassert.h"

// エフェクシア
#ifndef _DEBUG
#pragma comment(lib, "Effekseer.lib")
#else
#pragma comment(lib, "Effekseerd.lib")
#endif
#include <Effekseer.h>

#ifndef _DEBUG
#pragma comment(lib, "EffekseerRendererDX9.lib")
#else
#pragma comment(lib, "EffekseerRendererDX9d.lib")
#endif
#include <EffekseerRendererDX9.h>

// json
#include <json.hpp>
using json = nlohmann::json;

// Imgui
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <implot.h>
#include <implot_internal.h>

// メモリリーク出力用マクロ
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>		// メモリリーク検出に必要

// crtdbg.hのバグ対策用マクロ
#if _DEBUG
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else

#define DEBUG_NEW new
#endif

#endif	// _PRECOMPIRE_H_