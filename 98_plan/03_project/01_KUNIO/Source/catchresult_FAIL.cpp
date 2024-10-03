//=============================================================================
// 
// キャッチ結果処理 [catchresult_FAIL.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "catchresult_FAIL.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
#include "camera.h"
#include "game.h"
#include "player.h"
#include "fade.h"
#include "timer.h"
#include "resultmanager.h"
#include "gamemanager.h"

//==========================================================================
// マクロ定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\result\\FAIL.png";
	const float TIME_EXPANSION = 0.3f;			// 拡大
	const float TIME_EXPNONE = 1.0f;			// 拡大後何もしない
	const float TIME_FADEOUT = 0.4f;			// フェードアウト時間
}

//==========================================================================
// コンストラクタ
//==========================================================================
CCatchResult_FAIL::CCatchResult_FAIL(int nPriority) : CCatchResult(nPriority)
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CCatchResult_FAIL::~CCatchResult_FAIL()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCatchResult_FAIL::Init()
{
	HRESULT hr;

	// 初期化処理
	hr = CObject2D::Init();
	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}

	// 種類の設定
	SetType(TYPE_OBJECT2D);

	// テクスチャの割り当て
	int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nTexIdx);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nTexIdx);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 400.0f);
	SetSize(size);
	SetSizeOrigin(size);

	// 開始
	m_fStateTimer = 0.0f;
	m_state = State::STATE_EXPANSION;

	// 向き設定
	SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));
	SetOriginRotation(GetRotation());

	return S_OK;
}

//==========================================================================
// 拡大
//==========================================================================
void CCatchResult_FAIL::StateExpansion()
{
	if (m_fStateTimer >= TIME_EXPANSION)
	{
		m_fStateTimer = 0.0f;
		m_state = STATE_EXPNONE;
		return;
	}

	// 状態遷移カウンター加算
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	float ratio = m_fStateTimer / TIME_EXPANSION;

	// サイズ設定
	D3DXVECTOR2 size = GetSize();
	size.x = UtilFunc::Correction::EasingEaseIn(0.0f, GetSizeOrigin().x, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(0.0f, GetSizeOrigin().y, ratio);
	SetSize(size);

}

//==========================================================================
// 整い状態
//==========================================================================
void CCatchResult_FAIL::StateExpNone()
{
	// 状態遷移カウンター加算
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	if (m_fStateTimer >= TIME_EXPNONE)
	{
		m_fStateTimer = 0.0f;
		m_state = State::STATE_FADEOUT;
		return;
	}
}

//==========================================================================
// フェードアウト状態
//==========================================================================
void CCatchResult_FAIL::StateFadeOut()
{
	// 状態遷移カウンター加算
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// 不透明度更新
	float alpha = 1.0f - m_fStateTimer / TIME_FADEOUT;
	SetAlpha(alpha);

	if (m_fStateTimer >= TIME_FADEOUT)
	{
		m_fStateTimer = 0.0f;
		Uninit();

		CGameManager* pManager = CGame::GetInstance()->GetGameManager();
		CManager::GetInstance()->GetResultManager()->SetJudgeRank(static_cast<CJudge::JUDGE>(pManager->GetJudgeRank()));
		CManager::GetInstance()->GetResultManager()->SetClearTime(CTimer::GetInstance()->GetTime());
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RESULT);
		return;
	}
}
