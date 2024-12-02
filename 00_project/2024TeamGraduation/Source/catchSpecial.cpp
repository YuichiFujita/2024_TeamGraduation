//=============================================================================
// 
//  キャッチスペシャル処理 [catchSpecial.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "catchSpecial.h"
#include "player.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace StateTime
{
	const float KAMEHAME_SUCC(5.0f);	// かめはめ波？キャッチ成功
	const float KAMEHAME_FAIL(5.0f);	// かめはめ波？キャッチ成功
}

// かめはめ波
namespace Kamehameha
{
	const float FAN_ROT(5.0f);		// 成功判定の扇の角度(度数法)
	const float RAD_JUST(0.5f);		// 成功判定のの許容割合(ジャスト時)
	const float RAD_NORMAL(0.2f);	// 成功判定のの許容割合(平常時)
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CCatchSpecial::STATE_FUNC CCatchSpecial::m_StateFunc[] =	// スペシャルキャッチ状態関数
{
	&CCatchSpecial::StateNone,				// なし
	&CCatchSpecial::State_Kamehame_Succ,	// かめはめ波？キャッチ成功
	&CCatchSpecial::State_Kamehame_Fail,	// かめはめ波？キャッチ失敗
};

// 関数リスト
std::vector<std::function<CCatchSpecial::EState(const CPlayer*, const bool)>> CCatchSpecial::s_CheckFunc =
{
	nullptr,								// NONE
	CCatchSpecial::Check_Kamehameha,						// かめはめ波
	nullptr,								// MAX
};

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CCatchSpecial::CCatchSpecial(CPlayer* pPlayer, EState state) :
	m_pPlayer(pPlayer), m_state(state)
{
	// 値のクリア
	m_fStateTime = 0.0f;	// 状態時間
	m_bSuccess = false;		// 成功フラグ
}

//==========================================================================
// デストラクタ
//==========================================================================
CCatchSpecial::~CCatchSpecial()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCatchSpecial *CCatchSpecial::Create(CPlayer* pPlayer, EState state)
{
	// メモリの確保
	CCatchSpecial* pCatchSP = DEBUG_NEW CCatchSpecial(pPlayer, state);
	if (pCatchSP != nullptr)
	{
		// クラスの初期化
		if (FAILED(pCatchSP->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pCatchSP);
			return nullptr;
		}
	}

	return pCatchSP;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCatchSpecial::Init()
{

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCatchSpecial::Uninit()
{
	m_pPlayer = nullptr;			// プレイヤー
}

//==========================================================================
// 更新処理
//==========================================================================
void CCatchSpecial::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_fStateTime += fDeltaTime * fSlowRate;

	// 状態更新
	if (m_StateFunc[m_state] != nullptr)
	{
		(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// なし状態
//==========================================================================
void CCatchSpecial::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 終了
	m_pPlayer->SetCatchSpecial(nullptr);
	Uninit();
}

//==========================================================================
// かめはめ波？キャッチ成功
//==========================================================================
void CCatchSpecial::State_Kamehame_Succ(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fStateTime > StateTime::KAMEHAME_SUCC)
	{// 終了したら
		
		// 成功効果
		Success();

		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// かめはめ波？キャッチ失敗
//==========================================================================
void CCatchSpecial::State_Kamehame_Fail(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fStateTime > StateTime::KAMEHAME_SUCC)
	{// 終了したら

		// 失敗効果
		Failure();

		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CCatchSpecial::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 成功時共通
//==========================================================================
void CCatchSpecial::Success()
{
	CGameManager* pGameMgr = CGameManager::GetInstance();
	CGameManager::ETeamSide team = m_pPlayer->GetTeam();
	CGameManager::ETeamSide rivalTeam = pGameMgr->RivalTeam(team);
	
	// 成功フラグ
	m_bSuccess = true;

	// 自陣
	// モテ加算
	pGameMgr->AddCharmValue(team, CCharmValueManager::ETypeAdd::ADD_CATCH_SPECIAL);
	// スペシャル加算
	pGameMgr->AddSpecialValue(team, CSpecialValueManager::ETypeAdd::ADD_CATCH_SPECIAL);

	// 敵陣
	pGameMgr->SubCharmValue(rivalTeam, CCharmValueManager::ETypeSub::SUB_SPECIAL_CATCH);
}

//==========================================================================
// 失敗時共通
//==========================================================================
void CCatchSpecial::Failure()
{
	// 成功フラグ
	m_bSuccess = false;
}

//==========================================================================
// チェック
//==========================================================================
CCatchSpecial::EState CCatchSpecial::Check_Kamehameha(const CPlayer* pPlayer, const bool bJust)
{
	// ・ジャストならコートの前50％
	// ・ノーマルならコートの前20％
	// ・スペシャル発動君の角度内(扇)
	CGameManager* pGameMgr = CGameManager::GetInstance();
	CGameManager::ETeamSide team = pPlayer->GetTeam();
	MyLib::Vector3 pos = pPlayer->GetPosition();

	// 規定割合
	float fRad = 0.0f;
	bJust ? fRad = Kamehameha::RAD_JUST : fRad = Kamehameha::RAD_NORMAL;

	// コート内割合取得
	float fRate = pGameMgr->GetCourtPosPercentage(team, pos);

	if (fRate <= fRad)
	{// (規定内)なら成功

		return EState::STATE_BEAM_SUCC;
	}

	return EState::STATE_BEAM_FAIL;
}

//==========================================================================
// チェック
//==========================================================================
CCatchSpecial::EState CCatchSpecial::Check(CPlayer* pPlayer, bool bJust, CBall::ESpecial state)
{
	CCatchSpecial::EState catchState = EState::STATE_NONE;

	// 静的関数ポインタ
	if (s_CheckFunc[state] != nullptr)
	{
		catchState = CCatchSpecial::s_CheckFunc[state](pPlayer, bJust);
	}

	return catchState;
}
