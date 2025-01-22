//=============================================================================
// 
//  審判用プレイヤー処理 [playerReferee_result.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerReferee_result.h"
#include "playerManager.h"
#include "camera.h"
#include "manager.h"
#include "calculation.h"
#include "sound.h"
#include "fade.h"
#include "resultmanager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string CHARAFILE = "data\\TEXT\\character\\referee\\result\\setup_player.txt";	// キャラクターファイル
	const MyLib::Vector3 POSITION_START = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// 初期位置
}

namespace StateTime
{
	const float TOSSWAIT = 3.0f;			// トス待ち
	const float CREATE_STARTTEXT = 2.0f;	// スタート文字生成
	const float RETURN = 1.0f;				// 戻る
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerReferee_Result::STATE_FUNC CPlayerReferee_Result::m_StateFunc[] =	// 状態関数
{
	&CPlayerReferee_Result::StateNone,	// なし
	&CPlayerReferee_Result::StateWait,	// 待ち
	&CPlayerReferee_Result::StateTalk,	// 喋り
	&CPlayerReferee_Result::StateWin,	// 勝ち
};

//==========================================================================
// 静的メンバ変数
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerReferee_Result::CPlayerReferee_Result(const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase, int nPriority) :
	CPlayerReferee(typeTeam, typeArea, typeBase, nPriority),
	m_state			(EState::STATE_NONE),	// 状態
	m_fStateTime	(0.0f),					// 状態時間
	m_winTeam		(CGameManager::ETeamSide::SIDE_NONE),	// 勝利チーム
	m_pWin			(nullptr)				// 勝利
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerReferee_Result::~CPlayerReferee_Result()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CPlayerReferee_Result* CPlayerReferee_Result::Create()
{
	// メモリの確保
	CPlayerReferee_Result* pObj = DEBUG_NEW CPlayerReferee_Result;

	if (pObj != nullptr)
	{
		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerReferee_Result::Init()
{
	// 種類の設定
	CObject::SetType(CObject::TYPE_PLAYER);

	// キャラ作成
	HRESULT hr = SetCharacter(CHARAFILE);
	if (FAILED(hr))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 影の生成
	if (FAILED(CreateShadow()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 中央付近に置く
	SetPosition(POSITION_START);

	// カメラの方向向きにする
	SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));

	// 待機にしておく
	SetState(EState::STATE_WAIT);
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerReferee_Result::Uninit()
{
	// 終了処理
	CPlayerReferee::Uninit();
}

//==========================================================================
// 動的削除処理
//==========================================================================
void CPlayerReferee_Result::Kill()
{
	if (m_pWin != nullptr)
	{
		// 勝利エフェクト停止
		m_pWin->Uninit();
		m_pWin = nullptr;
	}

	// 動的削除処理
	CPlayerReferee::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerReferee_Result::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 更新処理
	CPlayerReferee::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 状態更新
//==========================================================================
void CPlayerReferee_Result::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態タイマー加算
	m_fStateTime += fDeltaTime * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// なし
//==========================================================================
void CPlayerReferee_Result::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
// 待ち
//==========================================================================
void CPlayerReferee_Result::StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	if (pMotion->GetType() != EMotion::MOTION_WAIT)
	{// 待機してなかったら待機にする
		pMotion->Set(EMotion::MOTION_WAIT);
	}
}

//==========================================================================
// 喋り
//==========================================================================
void CPlayerReferee_Result::StateTalk(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pWin != nullptr)
	{
		// 勝利エフェクト停止
		m_pWin->Uninit();
		m_pWin = nullptr;
	}

	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	if (pMotion->GetType() != EMotion::MOTION_TALK)
	{// 喋りなかったら喋りにする
		pMotion->Set(EMotion::MOTION_TALK);
	}
}

//==========================================================================
// 勝ち
//==========================================================================
void CPlayerReferee_Result::StateWin(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pWin == nullptr)
	{
		// 勝利エフェクト生成
		m_pWin = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_WINREFEREE,
			GetPosition(),
			MyLib::Vector3(0.0f, (m_winTeam == CGameManager::ETeamSide::SIDE_LEFT) ? 0.0f : D3DX_PI, 0.0f),	// 向き
			MyLib::Vector3(),
			15.0f, false);
	}

	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	// セットするモーション
	int motionIdx = m_winTeam + EMotion::MOTION_WIN_L;

	if (pMotion->GetType() != motionIdx)
	{// 喋りなかったら喋りにする
		pMotion->Set(motionIdx);
	}
}

//==========================================================================
// 攻撃時処理
//==========================================================================
void CPlayerReferee_Result::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
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
	case EMotion::MOTION_DEF:
		break;

	default:
		break;
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CPlayerReferee_Result::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}
