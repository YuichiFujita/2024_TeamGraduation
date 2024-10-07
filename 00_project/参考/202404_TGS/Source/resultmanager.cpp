//=============================================================================
// 
//  リザルトマネージャ処理 [resultmanager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "debugproc.h"
#include "resultmanager.h"
#include "calculation.h"

#include "timer_result.h"
#include "clearrank.h"
#include "toatalrank.h"
#include "scroll.h"
#include "sound.h"
#include "ranking.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CResultManager* CResultManager::m_pThisPtr = nullptr;	// 自身のポインタ
float CResultManager::m_fClearTime = 0.0f;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CResultManager::CResultManager()
{
	// 値のクリア
	m_state = State::STATE_SCORE;				// 状態
	m_JudgeRank = CJudge::JUDGE::JUDGE_DDD;	// 最終評価
	m_pScroll = nullptr;		// 巻き物のオブジェクト
	m_pTimer = nullptr;			// タイマーのオブジェクト
	m_pClearRank = nullptr;		// クリア時のランク
	m_pToatalRank = nullptr;	// 総合評価

}

//==========================================================================
// デストラクタ
//==========================================================================
CResultManager::~CResultManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CResultManager *CResultManager::Create()
{
	if (m_pThisPtr != nullptr)
	{
		return m_pThisPtr;
	}

	// メモリの確保
	m_pThisPtr = DEBUG_NEW CResultManager;
	if (m_pThisPtr == nullptr)
	{
		return nullptr;
	}

	// 初期化処理
	HRESULT hr = m_pThisPtr->Init();
	if (FAILED(hr))
	{// 失敗していたら
		return nullptr;
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CResultManager::Init()
{
	return S_OK;
}

//==========================================================================
// リザルト画面生成
//==========================================================================
void CResultManager::CreateResultScreen()
{
	// 巻き物
	m_pScroll = CScroll::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f), 1.5f, 350.0f, 1000.0f, false, true, 1);

	// タイマー
	m_pTimer = static_cast<CTimer_Result*>(CTimer::Create(CTimer::Type::TYPE_RESULT, 4));
	m_pTimer->SetTime(m_fClearTime);

	// クリアランク
	m_pClearRank = CClearRank::Create(m_JudgeRank);

	// トータルランク
	m_pToatalRank = CToatalRank::Create(m_JudgeRank, m_fClearTime);

	// ランキングデータに保存
	CRanking::SRankdata nowdata = CRanking::SRankdata();
	nowdata.allrank = m_pToatalRank->GetRank();
	nowdata.minutes = static_cast<int>(m_fClearTime / 60);
	nowdata.seconds = static_cast<int>(m_fClearTime) % 60;
	nowdata.milliSeconds = static_cast<int>((m_fClearTime - static_cast<int>(m_fClearTime)) * 1000);
	nowdata.milliSeconds /= 10;
	CRanking::SetNowData(nowdata);
}

//==========================================================================
// 終了処理
//==========================================================================
void CResultManager::Uninit()
{
	// タイマーの破棄
	if (m_pTimer != nullptr)
	{
		// 終了処理
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

	delete m_pThisPtr;
	m_pThisPtr = nullptr;

	m_fClearTime = 0.0f;
}

//==========================================================================
// リセット
//==========================================================================
void CResultManager::Reset()
{
	// タイマーの破棄
	if (m_pTimer != nullptr)
	{
		// 終了処理
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

	m_state = State::STATE_SCORE;			// 状態
	m_JudgeRank = CJudge::JUDGE::JUDGE_DDD;	// 最終評価
	m_fClearTime = 0.0f;					// クリア時間

	m_pScroll = nullptr;		// 巻き物のオブジェクト
	m_pTimer = nullptr;			// タイマーのオブジェクト
	m_pClearRank = nullptr;		// クリア時のランク
	m_pToatalRank = nullptr;	// 総合評価
}

//==========================================================================
// 更新処理
//==========================================================================
void CResultManager::Update()
{

	// クリアのランク
	if (m_pClearRank == nullptr) return;

	if (m_pScroll->GetState() == CScroll::STATE::STATE_WAIT &&
		m_pClearRank->GetState() == CClearRank::State::STATE_NONE &&
		!m_pClearRank->IsFinish())
	{// 巻き物開ききった後
		m_pClearRank->SetState(CClearRank::State::STATE_SCROLL_TEXT);

		// サウンド再生
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING);
	}

	if (m_pClearRank->GetState() == CClearRank::State::STATE_FINISH &&
		m_pTimer->GetState() == CTimer_Result::State::STATE_NONE &&
		!m_pTimer->IsFinish())
	{// クリアランク終了時

		m_pTimer->CTimer_Result::SetState(CTimer_Result::State::STATE_SCROLL_TEXT);

		// サウンド再生
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING);
	}

	if (m_pTimer->GetState() == CTimer_Result::State::STATE_FINISH &&
		m_pToatalRank->GetState() == CToatalRank::State::STATE_NONE &&
		!m_pToatalRank->IsFinish())
	{// クリアタイム終了時

		m_pToatalRank->SetState(CToatalRank::State::STATE_SCROLL_TEXT);

		// サウンド再生
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING);
	}


	if (m_pToatalRank->IsFinish())
	{// 総合評価も出た

		// 押下待ち
		m_state = State::STATE_PRESSENTER;
	}

	// タイマー更新
	if (m_pTimer != nullptr)
	{
		m_pTimer->Update();
		m_pTimer->SetTime(m_fClearTime);
	}
}
