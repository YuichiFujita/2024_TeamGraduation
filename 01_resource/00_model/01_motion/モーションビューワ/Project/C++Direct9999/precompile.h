//=============================================================================
// 
//  �v���R���t�@�C���w�b�_�[ [precompile.h]
//  Author : ���n�Ή끕���搶
// 
//=============================================================================
#ifndef _PRECOMPIRE_H_
#define _PRECOMPIRE_H_	// ��d�C���N���[�h�h�~

#include <Windows.h>
#include <stdio.h>
#include <time.h>


// C++��std�p
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

#include <d3dx9.h>						// �`�揈���ɕK�v
#define DIRECTINPUT_VERSION	(0x0800)	// �r���h���̌x���p�}�N��
#include <dinput.h>						// ���͏����ɕK�v
#include <xaudio2.h>					// �T�E���h�����ɕK�v
#include <dsound.h>						// �T�E���h���������ɕK�v
#include <Xinput.h>

// ���������[�N�o�͗p�}�N��
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>		// ���������[�N���o�ɕK�v

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

// crtdbg.h�̃o�O�΍��p�}�N��
#if _DEBUG
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else

#define DEBUG_NEW new
#endif

#endif	// _PRECOMPIRE_H_