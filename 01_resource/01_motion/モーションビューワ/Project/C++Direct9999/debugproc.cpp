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
LPD3DXFONT CDebugProc::m_pFont = NULL;	// フォントのポインタ

//==========================================================================
// マクロ定義
//==========================================================================
#define MAX_FLOATNUM	(2)		//小数点以下の表示桁数

//==========================================================================
// コンストラクタ
//==========================================================================
CDebugProc::CDebugProc()
{
	// デバッグ表示情報のクリア
	m_bDisp = false;
	m_pFont = NULL;
	memset(&m_aStr[0], NULL, sizeof(m_aStr));
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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	HRESULT hr;

	//デバッグ表示用フォントの生成
	hr = D3DXCreateFont(pDevice, 20, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);

	if (FAILED(hr))
	{// 失敗していたら

		return E_FAIL;
	}

	//初期表示設定
//#if _DEBUG
//
//	// ON
//	m_bDisp = true;
//
//#else
//
//	// OFF
//	m_bDisp = false;
//
//#endif
	
	// ON
	m_bDisp = true;

	return S_OK;
}

//==========================================================================
// デバッグ表示の終了処理
//==========================================================================
void CDebugProc::Uninit(void)
{
	// フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//==========================================================================
// デバッグ表示の更新処理
//==========================================================================
void CDebugProc::Update(void)
{
	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	if(pInputKeyboard->GetTrigger(DIK_F1) == true)
	{//F1キーが押されたとき

		// ON,OFF切り替え
		m_bDisp = m_bDisp ? false : true;
	}
}

//==========================================================================
// デバッグ表示の描画処理
//==========================================================================
void CDebugProc::Draw(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	if (m_bDisp == true)
	{// デバックモードがオンの時

		// テキストの描画
		m_pFont->DrawText(NULL, &m_aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// デバッグ表示情報のクリア
	memset(&m_aStr[0], NULL, sizeof(m_aStr));
}

//==========================================================================
// デバッグ表示の設定処理
//==========================================================================
void CDebugProc::Print(const char *fmt, ...)
{
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
void CDebugProc::SetText(void)
{
	// デバッグの情報取得
	CDebugProc *pDebug = CManager::GetDebugProc();

	// エディットタイプ取得
	CManager::EDITTYPE editType = CManager::GetEditType();

	pDebug->Print("FPS：%d\n", GetFPS());

	pDebug->Print("\n--------------------------------[ F系 ]--------------------------------\n");

	switch (editType)
	{
	case CManager::EDITTYPE_EDIT:
		pDebug->Print("[F2]：モード切り替え [エディット状態]\n");
		break;

	case CManager::EDITTYPE_PLAY:
		pDebug->Print("[F2]：モード切り替え [実行状態]\n");
		break;

	case CManager::EDITTYPE_SETUP:
		pDebug->Print("[F2]：モード切り替え [セットアップ状態]\n");
		break;

	default:
		break;
	}
}
