//=============================================================================
// 
//  審判用プレイヤー処理 [playerReferee.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerReferee.h"
#include "playerManager.h"
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "calculation.h"
#include "sound.h"
#include "fade.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string CHARAFILE = "data\\TEXT\\character\\referee\\setup_player.txt";	// キャラクターファイル
}

namespace StateTime
{
	const float TOSSWAIT = 3.0f;	// トス待ち
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerReferee::STATE_FUNC CPlayerReferee::m_StateFunc[] =	// 状態関数
{
	&CPlayerReferee::StateNone,		// なし
	&CPlayerReferee::StateTossWait,	// トス待機
	&CPlayerReferee::StateToss,		// トス
};

//==========================================================================
// 静的メンバ変数
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerReferee::CPlayerReferee(const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase, int nPriority) :
	CPlayer(typeTeam, typeArea, typeBase, nPriority),
	m_state			(EState::STATE_NONE),	// 状態
	m_fStateTime	(0.0f)					// 状態時間
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerReferee::~CPlayerReferee()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerReferee::Init()
{
	// 種類の設定
	CObject::SetType(CObject::TYPE_PLAYER);

	// キャラ作成
	HRESULT hr = SetCharacter(CHARAFILE);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// ボール取得
	CListManager<CBall> list = CBall::GetListObj();	// リスト
	CBall* pBall = list.GetData(0);

	// ボールを即キャッチ
	if (pBall != nullptr)
	{
		pBall->CatchLand(this);
	}

	// カメラの方向向きにする
	SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));

	// 待機にしておく
	SetState(EState::STATE_TOSSWAIT);
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerReferee::Uninit()
{
	// 終了処理
	CPlayer::Uninit();
}

//==========================================================================
// 動的削除処理
//==========================================================================
void CPlayerReferee::Kill()
{
	// 動的削除処理
	CPlayer::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerReferee::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 過去の位置保存
	SetOldPosition(GetPosition());

	// 親の更新処理
	CObjectChara::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 状態更新
//==========================================================================
void CPlayerReferee::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態タイマー加算
	m_fStateTime += fDeltaTime * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// なし
//==========================================================================
void CPlayerReferee::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイマーリセット
	m_fStateTime = 0.0f;

	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	// ニュートラル設定
	pMotion->Set(EMotion::MOTION_DEF);
}

//==========================================================================
// トス待ち
//==========================================================================
void CPlayerReferee::StateTossWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	if (pMotion->GetType() != EMotion::MOTION_WAIT)
	{// 待機してなかったら待機にする
		pMotion->Set(EMotion::MOTION_WAIT);
	}

	if (m_fStateTime >= StateTime::TOSSWAIT)
	{
		// トスへ遷移
		pMotion->Set(EMotion::MOTION_TOSS);
		SetState(EState::STATE_TOSS);
	}
}

//==========================================================================
// トス
//==========================================================================
void CPlayerReferee::StateToss(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	if (pMotion->GetType() == EMotion::MOTION_TOSS &&
		pMotion->IsFinish())
	{// トスモーションが終了してたら

		// NONEにする
		pMotion->Set(EMotion::MOTION_DEF);
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// 攻撃時処理
//==========================================================================
void CPlayerReferee::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	int nType = pMotion->GetType();

	// 武器の位置
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), ATKInfo);
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	switch (nType)
	{
	case EMotion::MOTION_TOSS:
		if (GetBall() != nullptr)
		{// トス
			GetBall()->Spawn(this);
		}

		// スタートにする
		CGameManager::GetInstance()->StartSetting();
		break;

	default:
		break;
	}
}


//==========================================================================
// 描画処理
//==========================================================================
void CPlayerReferee::Draw()
{
	// 描画処理
	CPlayer::Draw();
}

//==========================================================================
// 状態設定
//==========================================================================
void CPlayerReferee::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}
