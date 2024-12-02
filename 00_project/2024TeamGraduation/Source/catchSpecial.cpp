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
// かめはめ波
namespace Kamehameha
{
	const float FAN_ROT(5.0f);		// 成功判定の扇の角度(度数法)
	const float RAD_JUST(0.5f);		// 成功判定のの許容割合(ジャスト時)
	const float RAD_NORMAL(0.2f);	// 成功判定のの許容割合(平常時)
	
	const float MOMENTUM_TIME[] =
	{
		0.5f,		// なし
		1.0f,		// ズザザー
		2.0f,		// 耐える
		1.0f,		// 結果(成功)(失敗)
		0.0f,
	};

	// 状態内状態定義(勢い)
	enum EMomentumState
	{
		STATE_NONE = 0,			// なし
		STATE_SLIDE,			// ズザザー
		STATE_BRAKE,			// 耐える
		STATE_RESULT,			// 結果(成功)(失敗)
		STATE_MAX
	};
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CCatchSpecial::STATE_FUNC CCatchSpecial::m_StateFunc[] =	// スペシャルキャッチ状態関数
{
	&CCatchSpecial::StateNone,				// なし
	&CCatchSpecial::StateKamehameSucc,		// かめはめ波？キャッチ成功
	&CCatchSpecial::StateKamehameFail,		// かめはめ波？キャッチ失敗
};

CCatchSpecial::MOMENTUM_FUNC CCatchSpecial::m_MomentumFunc[] =	// 勢い状態関数
{
	&CCatchSpecial::MomentumStateNone,		// なし
	&CCatchSpecial::MomentumStateSlide,		// ズザザ
	&CCatchSpecial::MomentumStateBrake,		// 耐える
	&CCatchSpecial::MomentumStateResult,	// 結果
	&CCatchSpecial::MomentumStateEnd,		// 終了
};

CCatchSpecial::START_FUNC CCatchSpecial::m_StartFunc[] =	// 勢い状態開始関数
{
	nullptr,								// なし
	&CCatchSpecial::MomentumStartSlide,		// ズザザ
	nullptr,								// 耐える
	nullptr,								// 結果
	nullptr,								// 終了
};

// 関数リスト
std::vector<std::function<CCatchSpecial::EState(const CPlayer*, const bool)>> CCatchSpecial::s_CheckFunc =
{
	nullptr,								// NONE
	CCatchSpecial::Check_Kamehameha,		// かめはめ波
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
	m_fStateTime = 0.0f;							// 状態時間
	m_momentumState = EMomentumState::MOMENTUM_NONE;	// 状態内状態
	m_fMomentumStateTime = 0.0f;					// 状態内状態時間
	m_bSuccess = false;								// 成功フラグ
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
	MyAssert::CustomAssert(m_pPlayer != nullptr, "catchSpecial: プレイヤーどこ行ったん？");
	m_pPlayer->SetEnableMove(false);
	m_pPlayer->SetEnableAction(false);

	m_fStateTime += fDeltaTime * fSlowRate;
	m_fMomentumStateTime += fDeltaTime * fSlowRate;

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

	// 行動可能に
	m_pPlayer->SetEnableMove(true);
	m_pPlayer->SetEnableAction(true);
	m_pPlayer->SetCatchSpecial(nullptr);
	Uninit();
}

//==========================================================================
// かめはめ波？キャッチ成功
//==========================================================================
void CCatchSpecial::StateKamehameSucc(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 成功フラグ
	m_bSuccess = true;

	// 勢い更新
	if (m_MomentumFunc[m_momentumState] != nullptr)
	{
		(this->*(m_MomentumFunc[m_momentumState]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// モーション設定
	m_pPlayer->SetMotion(CPlayer::EMotion::MOTION_CATCHSPECIAL_SUCC);

	if (m_momentumState == EMomentumState::MOMENTUM_END)
	{// 終了したら(結果時間が終わった)
		
		// 成功効果
		Success();

		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// かめはめ波？キャッチ失敗
//==========================================================================
void CCatchSpecial::StateKamehameFail(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 成功フラグ
	m_bSuccess = false;

	// 勢い更新
	if (m_MomentumFunc[m_momentumState] != nullptr)
	{
		(this->*(m_MomentumFunc[m_momentumState]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// モーション設定
	m_pPlayer->SetMotion(CPlayer::EMotion::MOTION_CATCHSPECIAL_FAIL);

	if (m_momentumState == EMomentumState::MOMENTUM_END)
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
// [勢い] なし
//==========================================================================
void CCatchSpecial::MomentumStateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// スローかける

	if (m_fMomentumStateTime > Kamehameha::MOMENTUM_TIME[m_momentumState])
	{// 終了

		// スロー戻す


		SetMomentumState(EMomentumState::MOMENTUM_SLIDE);
	}
}

//==========================================================================
// [勢い] ズザザー
//==========================================================================
void CCatchSpecial::MomentumStateSlide(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ズザザでコート奥まで行く
	MyLib::Vector3 pos = MyLib::Vector3();
	float fBaseTime = Kamehameha::MOMENTUM_TIME[m_momentumState];
	CPlayer::SKnockbackInfo knockback = m_pPlayer->GetKnockBackInfo();

	// 移動量精製
	MyLib::Vector3 move = knockback.posStart - knockback.posEnd;
	float fAngle = atan2f(move.x, move.z) + D3DX_PI;		//目標の向き
	UtilFunc::Transformation::RotNormalize(fAngle);

	move = m_pPlayer->GetMove();
	move.x = sinf(fAngle) * 1.0f;
	move.z = cosf(fAngle) * 1.0f;

	pos += move;

	m_pPlayer->SetPosition(pos);

	if (m_fMomentumStateTime > fBaseTime)
	{// 終了

		SetMomentumState(EMomentumState::MOMENTUM_BRAKE);
	}
}

//==========================================================================
// [勢い] 耐える
//==========================================================================
void CCatchSpecial::MomentumStateBrake(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fMomentumStateTime > Kamehameha::MOMENTUM_TIME[m_momentumState])
	{// 終了

		SetMomentumState(EMomentumState::MOMENTUM_RESULT);
	}
}

//==========================================================================
// [勢い] 結果(成功)(失敗)
//==========================================================================
void CCatchSpecial::MomentumStateResult(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fMomentumStateTime > Kamehameha::MOMENTUM_TIME[m_momentumState])
	{// 終了

		SetMomentumState(EMomentumState::MOMENTUM_END);
	}
}

//==========================================================================
// [勢い] 終了
//==========================================================================
void CCatchSpecial::MomentumStateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_fMomentumStateTime = 0.0f;
}

//==========================================================================
// 勢い状態設定
//==========================================================================
void CCatchSpecial::SetMomentumState(EMomentumState state)
{
	m_momentumState = state;
	m_fMomentumStateTime = 0.0f;

	// 勢い更新
	if (m_StartFunc[m_momentumState] != nullptr)
	{
		(this->*(m_StartFunc[m_momentumState]))();
	}
}

//==========================================================================
// ズザザ開始
//==========================================================================
void CCatchSpecial::MomentumStartSlide()
{
	CPlayer::SKnockbackInfo knockback = m_pPlayer->GetKnockBackInfo();

	// 開始位置
	MyLib::Vector3 pos = m_pPlayer->GetPosition();
	MyLib::Vector3 rot = m_pPlayer->GetRotation();
	knockback.posStart = pos;

	// コート端で判定取ってその際を設定
	CGameManager* pGmMgr = CGameManager::GetInstance();
	
	while(!pGmMgr->SetPosLimit(pos, m_pPlayer->GetRadius()))
	{// 端に当たっていなかったら
	
		pos.x += sinf(rot.y) * 10.0f;
		pos.z += cosf(rot.y) * 10.0f;
	}
	
	// 終了位置
	knockback.posEnd = pos;

	// 設定
	m_pPlayer->SetKnockBackInfo(knockback);
}

//==========================================================================
// 成功時共通
//==========================================================================
void CCatchSpecial::Success()
{
	CGameManager* pGameMgr = CGameManager::GetInstance();
	CGameManager::ETeamSide team = m_pPlayer->GetTeam();
	CGameManager::ETeamSide rivalTeam = pGameMgr->RivalTeam(team);

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
	//スペシャル時ライン越え判定
	MyLib::Vector3 pos = m_pPlayer->GetPosition();

	if (m_state != CPlayer::EState::STATE_OUTCOURT &&
		m_state != CPlayer::EState::STATE_OUTCOURT_RETURN)
	{
		m_pPlayer->OutCourtSetting();
	}
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
