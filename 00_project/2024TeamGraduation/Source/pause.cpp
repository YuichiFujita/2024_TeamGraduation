//=============================================================================
// 
//  ポーズ処理 [pause.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "pause.h"
#include "object2D.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "calculation.h"

// 派生クラス
#include "pause_game.h"
#include "pause_tutorial.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE[] =	// テクスチャファイル
	{
		"",
		"data\\TEXTURE\\pause\\window.png",
		"data\\TEXTURE\\pause\\resume.png",
		"data\\TEXTURE\\pause\\retry.png",
		"data\\TEXTURE\\pause\\end.png",
	};
	const float DIS_POSY = 130.0f;	// 選択肢の間隔
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPause::CPause()
{
	// 値のクリア
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		m_aObject2D[nCntVtx] = nullptr;	// オブジェクト2Dのオブジェクト
	}
	m_bPause = false;	// ポーズの判定
	m_nSelect = 0;									// 選択肢
	m_fFlashTime = 0.0f;	// 点滅時間
}

//==========================================================================
// デストラクタ
//==========================================================================
CPause::~CPause()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CPause* CPause::Create(CScene::MODE mode)
{
	// メモリ確保
	CPause* pFade = nullptr;

	switch (mode)
	{
	case CScene::MODE_GAME:
		pFade = DEBUG_NEW CPause_Game;
		break;

	case CScene::MODE::MODE_TUTORIAL:
		pFade = DEBUG_NEW CPause_Tutorial;
		break;

	default:
		pFade = DEBUG_NEW CPause_Game;
		break;
	}

	if (pFade != nullptr)
	{
		// 初期化処理
		if (FAILED(pFade->Init()))
		{
			return nullptr;
		}
	}

	return pFade;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPause::Init()
{
	// 生成処理
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		// 生成処理
		m_aObject2D[nCntVtx] = CObject2D::Create(7);

		if (m_aObject2D[nCntVtx] == nullptr)
		{// 失敗していたら
			return E_FAIL;
		}
		m_aObject2D[nCntVtx]->SetType(CObject::TYPE::TYPE_NONE);

		// テクスチャの割り当て
		int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE[nCntVtx]);

		// テクスチャの割り当て
		m_aObject2D[nCntVtx]->BindTexture(nTexIdx);

		if (nCntVtx == VTX_FADE)
		{// 黒幕の時
			m_aObject2D[nCntVtx]->SetSize(D3DXVECTOR2(640.0f, 360.0f));					// サイズ
			m_aObject2D[nCntVtx]->SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));	// 位置
			m_aObject2D[nCntVtx]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.4f));			// 色設定
		}
		else if (nCntVtx == VTX_WINDOW)
		{// ウィンドウの時
			m_aObject2D[nCntVtx]->SetSize(D3DXVECTOR2(640.0f, 360.0f));					// サイズ
			m_aObject2D[nCntVtx]->SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));	// 位置
			m_aObject2D[nCntVtx]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));			// 色設定
		}
		else
		{// 選択肢

			// 幅を基準にサイズ設定
			D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nTexIdx);
			size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
			m_aObject2D[nCntVtx]->SetSize(size);	// サイズ
			m_aObject2D[nCntVtx]->SetPosition(MyLib::Vector3(640.0f, 430.0f + ((nCntVtx - VTX_RETRY) * DIS_POSY), 0.0f));	// 位置
		}
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPause::Uninit()
{
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_aObject2D[nCntVtx] == nullptr) continue;

		// 終了処理
		m_aObject2D[nCntVtx]->Uninit();
		delete m_aObject2D[nCntVtx];
		m_aObject2D[nCntVtx] = nullptr;
	}
}

//==========================================================================
// 削除
//==========================================================================
void CPause::Kill()
{
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_aObject2D[nCntVtx] == nullptr) continue;

		// 終了処理
		m_aObject2D[nCntVtx]->Uninit();
		m_aObject2D[nCntVtx] = nullptr;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CPause::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// キーボード情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();

	// ゲームパッド情報取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_P) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_START, 0))
	{// ポーズ
		m_bPause = !m_bPause;
	}

	if (!m_bPause)
	{// ポーズ中じゃなかったら
		return;
	}

	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// フェード中じゃない時
		return;
	}

	// 点滅時間更新
	m_fFlashTime += fDeltaTime * fSlowRate;

	// 色更新
	UpdateColor();

	// 選択肢更新
	UpdateSelect();

	// 更新処理
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_aObject2D[nCntVtx] != nullptr)
		{
			// 更新処理
			m_aObject2D[nCntVtx]->Update(fDeltaTime, fDeltaRate, fSlowRate);

			// 頂点座標更新
			m_aObject2D[nCntVtx]->SetVtx();
		}
	}
}

//==========================================================================
// 色更新
//==========================================================================
void CPause::UpdateColor()
{
	for (int i = VTX_CONTINUE; i < VTX_MAX; i++)
	{
		D3DXCOLOR col = m_aObject2D[i]->GetColor();
		if (m_nSelect + VTX_CONTINUE == i)
		{// チカチカ
			col = UtilFunc::Transformation::HSVtoRGB(0.0f, 0.0f, 0.7f + fabsf(sinf(D3DX_PI * (m_fFlashTime / 1.0f)) * 0.3f));
		}
		else
		{
			// 黒っぽくする
			col = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
		}
		m_aObject2D[i]->SetColor(col);
	}
}

//==========================================================================
// 選択肢更新
//==========================================================================
void CPause::UpdateSelect()
{
	// キーボード情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();

	// ゲームパッド情報取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_W) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
		(pPad->GetLStickTrigger(0, CInputGamepad::STICK_AXIS::STICK_Y) && pPad->GetStickMoveL(0).y > 0))
	{// 上系が押された

		// パターンNo.を更新
		m_nSelect = (m_nSelect + (MENU_MAX - 1)) % MENU_MAX;

		// サウンド再生

	}
	else if (pKey->GetTrigger(DIK_S) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_DOWN, 0) ||
		(pPad->GetLStickTrigger(0, CInputGamepad::STICK_AXIS::STICK_Y) && pPad->GetStickMoveL(0).y < 0))
	{// 下系が押された

		// パターンNo.を更新
		m_nSelect = (m_nSelect + 1) % MENU_MAX;

		// サウンド再生

	}


	if (pKey->GetTrigger(DIK_RETURN) || 
		pPad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{// 決定が押された

		Decide();
	}
	
}

//==========================================================================
// 決定処理
//==========================================================================
void CPause::Decide()
{
	switch (m_nSelect)
	{
	case MENU_RETURNGAME:
		SetPause(false);
		break;

	case MENU_RETRY:

		// モード別決定処理
		DecideByMode();
		break;

	case MENU_RETURNTITLE:

		// サウンド再生

		// タイトルに戻る
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
		break;
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CPause::Draw()
{
	if (!m_bPause)
	{// ポーズ中じゃなかったら
		return;
	}

	// 描画処理
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_aObject2D[nCntVtx] != nullptr)
		{
			// 描画処理
			m_aObject2D[nCntVtx]->Draw();
		}
	}
}

