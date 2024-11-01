//=============================================================================
// 
//  プレコンファイルヘッダー [precompile.h]
//  Author : 相馬靜雅＆東先生
// 
//=============================================================================
#ifndef _PRECOMPIRE_H_
#define _PRECOMPIRE_H_	// 二重インクルード防止

#include <Windows.h>
#include <stdio.h>
#include <time.h>


// C++のstd用
#include <vector>
#include <algorithm>
#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <iterator>
#include <codecvt>
#include <fstream>
#include <filesystem>
#include <shlwapi.h> // PathCombine, PathCanonicalize
#pragma comment(lib, "shlwapi.lib")

#include <d3dx9.h>						// 描画処理に必要
#define DIRECTINPUT_VERSION	(0x0800)	// ビルド時の警告用マクロ
#include <dinput.h>						// 入力処理に必要
#include <xaudio2.h>					// サウンド処理に必要
#include <dsound.h>						// サウンド内部処理に必要
#include <Xinput.h>

// メモリリーク出力用マクロ
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>		// メモリリーク検出に必要

#include "mylib.h"

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

// crtdbg.hのバグ対策用マクロ
#if _DEBUG
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else

#define DEBUG_NEW new
#endif

#endif	// _PRECOMPIRE_H_