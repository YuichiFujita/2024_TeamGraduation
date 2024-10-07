//=============================================================================
// 
//  �v���R���p�C���w�b�_�[ [precompile.h]
//  Author : ���n�Ή끕���搶
// 
//=============================================================================
#ifndef _PRECOMPIRE_H_
#define _PRECOMPIRE_H_	// ��d�C���N���[�h�h�~

//#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <DirectXMath.h>
#include <D3DX9Math.h>
#include <assert.h>

// C++��std�p
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

#include <d3dx9.h>						// �`�揈���ɕK�v
#define DIRECTINPUT_VERSION	(0x0800)	// �r���h���̌x���p�}�N��
#include <dinput.h>						// ���͏����ɕK�v
#include <xaudio2.h>					// �T�E���h�����ɕK�v
#include <dsound.h>						// �T�E���h���������ɕK�v
#include <Xinput.h>

// �}�C���C�u����
#include "mylib.h"
#include "myassert.h"

// �G�t�F�N�V�A
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

// ���������[�N�o�͗p�}�N��
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>		// ���������[�N���o�ɕK�v

// crtdbg.h�̃o�O�΍��p�}�N��
#if _DEBUG
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else

#define DEBUG_NEW new
#endif

#endif	// _PRECOMPIRE_H_