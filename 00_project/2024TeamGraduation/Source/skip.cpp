//============================================================
//
//	スキップ操作処理 [skip.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "skip.h"
#include "renderer.h"
#include "manager.h"
#include "sound.h"
#include "fade.h"
#include "texture.h"
#include "object2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *TEXTURE_FILE = "data\\TEXTURE\\skip000.png";	// スキップ操作表示テクスチャ
	const int	PRIORITY	 = 6;		// スキップ操作表示の優先順位
	const float	DISP_TIME	 = 4.0f;	// 完全不透明フレーム
	const float	FADE_LEVEL	 = 0.05f;	// 透明度のフレーム変動量
	const float WIDTH		 = 77.0f * 2.4f;	// スキップ操作の表示大きさ
	const MyLib::Vector3 POS = MyLib::Vector3(1092.0f, 720.0f - 673.0f, 0.0f);	// スキップ操作の表示位置
}

//************************************************************
//	親クラス [CSkip] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSkip::CSkip() : 
	m_pControl	(nullptr),		// 操作の情報
	m_state		(STATE_NONE),	// 状態
	m_fCurTime	(0.0f)			// 状態管理カウンター
{

}

//============================================================
//	デストラクタ
//============================================================
CSkip::~CSkip()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSkip::Init()
{
	// メンバ変数を初期化
	m_pControl	= nullptr;		// 操作の情報
	m_state		= STATE_NONE;	// 状態
	m_fCurTime	= 0.0f;			// 状態管理カウンター

	// 操作情報の生成
	m_pControl = CObject2D::Create(PRIORITY);
	if (m_pControl == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();	// テクスチャ情報
	int texID = pTexture->Regist(TEXTURE_FILE);
	m_pControl->BindTexture(texID);

	// 位置を設定
	m_pControl->SetPosition(POS);

	// 横幅を元に大きさ設定
	MyLib::Vector2 size = pTexture->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, WIDTH);
	m_pControl->SetSize(size);

	// 透明度を設定
	m_pControl->SetAlpha(0.0f);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSkip::Uninit()
{
	// 操作情報の終了
	SAFE_UNINIT(m_pControl);
}

//============================================================
//	更新処理
//============================================================
void CSkip::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_state != STATE_NONE)
	{ // 何もしない状態ではない場合

		D3DXCOLOR colCont = m_pControl->GetColor();	// 操作表示の色
		switch (m_state)
		{ // 状態ごとの処理
		case STATE_FADEOUT:

			// α値を加算
			colCont.a += FADE_LEVEL * fDeltaRate * fSlowRate;
			if (colCont.a >= 1.0f)
			{ // 不透明になった場合

				// 透明度を補正
				colCont.a = 1.0f;

				// 表示状態にする
				m_state = STATE_DISP;
			}

			break;

		case STATE_DISP:

			// カウンターを加算
			m_fCurTime += fDeltaTime * fSlowRate;
			if (m_fCurTime > DISP_TIME)
			{ // 表示時間を超えた場合

				// カウンター初期化
				m_fCurTime = 0;

				// フェードイン状態にする
				m_state = STATE_FADEIN;
			}

			break;

		case STATE_FADEIN:

			// α値を減算
			colCont.a -= FADE_LEVEL * fDeltaRate * fSlowRate;
			if (colCont.a <= 0.0f)
			{ // 透明になった場合

				// 透明度を補正
				colCont.a = 0.0f;

				// 何もしない状態にする
				m_state = STATE_NONE;
			}

			break;

		default:
			assert(false);
			break;
		}

		// 色を反映
		m_pControl->SetColor(colCont);
	}
}

//============================================================
//	描画処理
//============================================================
void CSkip::Draw()
{

}

//============================================================
//	表示設定処理
//============================================================
void CSkip::SetDisp()
{
	// 既に表示中の場合抜ける
	if (m_state == STATE_DISP) { return; }

	// カウンターを初期化
	m_fCurTime = 0.0f;

	// フェードアウト状態にする
	m_state = STATE_FADEOUT;
}

//============================================================
//	表示取得処理
//============================================================
bool CSkip::IsDisp()
{
	// 表示状況を設定
	bool bDisp = (m_state == STATE_DISP || m_state == EState::STATE_FADEOUT);

	// 表示状況を返す
	return bDisp;
}

//============================================================
//	生成処理
//============================================================
CSkip *CSkip::Create()
{
	// スキップ操作の生成
	CSkip *pSkip = DEBUG_NEW CSkip;
	if (pSkip == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// スキップ操作の初期化
		if (FAILED(pSkip->Init()))
		{ // 初期化に失敗した場合

			// スキップ操作の破棄
			SAFE_DELETE(pSkip);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pSkip;
	}
}

//============================================================
//	破棄処理
//============================================================
void CSkip::Release(CSkip *&prSkip)
{
	// スキップ操作の終了
	assert(prSkip != nullptr);
	prSkip->Uninit();

	// メモリ開放
	SAFE_DELETE(prSkip);
}
