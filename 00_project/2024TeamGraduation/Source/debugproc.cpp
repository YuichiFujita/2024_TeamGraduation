//=============================================================================
//
// デバッグ表示処理[debugproc.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "debugproc.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "Xload.h"

#include "player.h"
#include "sound.h"
#include "objectX.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
LPD3DXFONT CDebugProc::m_pFont = nullptr;	// フォントのポインタ

//==========================================================================
// マクロ定義
//==========================================================================
#define MAX_FLOATNUM	(2)		//小数点以下の表示桁数

//==========================================================================
// デバッグ表示 ON/OFF
//==========================================================================
#ifndef DEBUG_EXE
#define DEBUGPROC
#endif

//==========================================================================
// コンストラクタ
//==========================================================================
CDebugProc::CDebugProc()
{
	// デバッグ表示情報のクリア
	m_bDisp = false;
	m_pFont = nullptr;
	memset(&m_aStr[0], 0, sizeof(m_aStr));
}

//==========================================================================
// デストラクタ
//==========================================================================
CDebugProc::~CDebugProc()
{
	
}

//==========================================================================
// デバッグ表示の初期化処理
//==========================================================================
HRESULT CDebugProc::Init(HINSTANCE hInstance, HWND hWnd)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	HRESULT hr;

	//デバッグ表示用フォントの生成
	hr = D3DXCreateFont(pDevice, 20, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);

	if (FAILED(hr))
	{// 失敗していたら

		return E_FAIL;
	}

	//初期表示設定
#if _DEBUG

	// ON
	m_bDisp = true;

#else

	// OFF
	m_bDisp = false;

#endif
	
	// ON
	//m_bDisp = true;

	return S_OK;
}

//==========================================================================
// デバッグ表示の終了処理
//==========================================================================
void CDebugProc::Uninit()
{
	// フォントの破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
	//CLostResourceManager::GetInstance()->Remove(this);
}

//==========================================================================
// デバッグ表示の更新処理
//==========================================================================
void CDebugProc::Update()
{
	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

#if _DEBUG
	if(pInputKeyboard->GetTrigger(DIK_F1))
	{//F1キーが押されたとき

		// ON,OFF切り替え
		m_bDisp = m_bDisp ? false : true;
	}
#endif
}

//==========================================================================
// デバッグ表示の描画処理
//==========================================================================
void CDebugProc::Draw()
{
	//return;
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	if (m_bDisp)
	{// デバックモードがオンの時

#ifdef DEBUGPROC
		// テキストの描画
		m_pFont->DrawText(nullptr, &m_aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif
	}

	// デバッグ表示情報のクリア
	memset(&m_aStr[0], 0, sizeof(m_aStr));
}

//==========================================================================
// デバッグ表示の設定処理
//==========================================================================
void CDebugProc::Print(const char *fmt, ...)
{
	//return;
	va_list args;
	char aString[MAX_DEBUGSTRING];		// 指定文字格納用
	char aSaveString[MAX_DEBUGSTRING];	// 可変引数中身格納用
	int nLength = 0;	// 可変引数内の文字の長さ
	int nStopLength;	// 可変引数挿入場所より

	// 文字列の代入
	strcpy(&aString[0], fmt);

	va_start(args, fmt);

	for (char *p = &aString[0]; *p != '\0'; p++)
	{
		if (*p == '%')
		{// 確認文字がある場合

			p++;			// 確認文字まで進める

			nLength = 0;	// 文字の長さリセット

			// 指定された種類ごとに変数の値を抜き出す(va_arg)
			switch (*p)
			{
			case 'd':	// 整数

				sprintf(&aSaveString[0], "%d", va_arg(args, int));

				break;
			case 'f':	// 小数

				sprintf(&aSaveString[0], "%f", va_arg(args, double));

				// 文字列の長さを取得
				nLength = (int)strlen(&aSaveString[0]);

				// 小数点以下の1文字目まで確認
				for (int nCntlength = 0; aSaveString[nCntlength] != '\0'; nCntlength++)
				{
					if (aSaveString[nCntlength] == '.')
					{// 小数点があった場合

						// 小数点以下の桁数を求める
						int nMin = (int)strlen(&aSaveString[nCntlength + 1]);

						// 小数点より先の文字数と指定した値までの差分を求める
						nMin -= MAX_FLOATNUM;

						// 文字列の長さを小数点以下第二位までに補正
						nLength -= nMin;
					}
				}

				break;

			case 'c':	// 文字

				sprintf(&aSaveString[0], "%c", va_arg(args, char));
				break;

			case 's':	// 文字列

				sprintf(&aSaveString[0], "%s", va_arg(args, const char*));
				break;
			}

			if (nLength == 0)
			{// 文字の長さを取得していない場合

				// 文字列の長さを取得
				nLength = (int)strlen(&aSaveString[0]);
			}

			nStopLength = (int)strlen(p) + 1;

			// メモリ内の確認文字(%〜)より後ろの文字列をずらす
			memmove(p + nLength - 2, p, nStopLength);

			p--;	// ポインタを%まで戻す

			// 可変引数を指定された場所に挿入
			memcpy(p, &aSaveString[0], nLength);
		}
	}

	va_end(args);

	// 文字列を連結する
	strcat(&m_aStr[0], &aString[0]);
}

//==========================================================================
// 文字設定処理
//==========================================================================
void CDebugProc::SetText()
{
	// デバッグの情報取得
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();

	pDebug->Print("--------------- システム情報 ---------------\n");
	pDebug->Print("【FPS】      [%d]\n", GetFPS());
#ifdef _DEBUG
	pDebug->Print("【目標FPS】  [%d]\n", *GetDebugFps());
#endif
	pDebug->Print("【DeltaTime】[%f]\n", CManager::GetInstance()->GetDeltaTime());
	pDebug->Print("【SlowRate】 [%f]\n", CManager::GetInstance()->GetSlowRate());
	pDebug->Print("【オブジェクト総数】[%d]\n", CObject::GetNumAll());

	pDebug->Print("\n--------------- デバッグ操作 ---------------\n");
	pDebug->Print("[F2]：ワイヤーフレーム切り替え\n");
	pDebug->Print("[F3]：起伏のエディット切り替え\n");
	pDebug->Print("[F4]：マップ制御点エディット切り替え\n");
	pDebug->Print("[F5]：プレイヤー配置リセット\n");
	pDebug->Print("[F6]：フォグ切り替え\n");
	pDebug->Print("[F7]：カメラ追従切り替え\n");
	pDebug->Print("[F8]：エディットモード切り替え\n");

	pDebug->Print("\n");
}

//========================
//バックアップ
//========================
void CDebugProc::Backup()
{
	//フォント破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

//========================
//復元
//========================
void CDebugProc::Restore()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイスの取得

	//デバッグ表示用フォント生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &m_pFont);

	//バッファクリア
	memset(&m_aStr, 0, sizeof m_aStr);
}
