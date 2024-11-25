//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerAIOut_control.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "teamStatus.h"

// デバッグ用
#include "3D_effect.h"

// player関連
#include "playerBase.h"
#include "playerStatus.h"

#include "playerAIOut_controlAction.h"
#include "playerAIOut_controlMove.h"

#include "playerAIOut.h"

#include "playerManager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace timing
{
	// タイミング
	const float TIMING_NORMAL = 1.0f;	// 通常
	const int TIMING_RAND_MAX = 100;	// randMAX値
	const int TIMING_RAND_MIN = -80;	// randMIN値
}

namespace
{
	// ジャンプ投げの最大位置(ジャンプ力MAX：150)
	const float JUMP_END_POS = 140.0f;

	// 距離間(デフォルト)
	const float LENGTH_IN = 500.0f;
	const float LENGTH_SPACE = 10.0f;
	const float LENGTH_OUT = 200.0f;
	const float LENGTH_LINE = 100.0f;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerAIOutControl::MODE_FUNC CPlayerAIOutControl::m_ModeFunc[] =	// モード関数
{
	&CPlayerAIOutControl::ModeNone,				// なし
	&CPlayerAIOutControl::ModeThrowManager,		// 投げ
	&CPlayerAIOutControl::ModeMoveManager,		// キャッチ
};

CPlayerAIOutControl::THROW_FUNC CPlayerAIOutControl::m_ThrowFunc[] =	// 投げ関数
{
	&CPlayerAIOutControl::ThrowNone,
	&CPlayerAIOutControl::ThrowNormal,
	&CPlayerAIOutControl::ThrowPass,
};

CPlayerAIOutControl::MOVE_FUNC CPlayerAIOutControl::m_MoveFunc[] =	// 行動関数
{
	&CPlayerAIOutControl::MoveNone,			// なし
	&CPlayerAIOutControl::MoveRetrieve,		// 取りに行く
	&CPlayerAIOutControl::MoveMeeting,		// 対面
};


//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIOutControl::CPlayerAIOutControl()
{
	// 値の初期化
	m_eMode = EMode::MODE_NONE;
	m_eThrow = EThrow::THROW_NONE;
	m_eMove = EMove::MOVE_NONE;
	m_pAIOut = nullptr;
	m_bStart = false;
	m_bEnd = false;
}

//==========================================================================
// 生成
//==========================================================================
CPlayerAIOutControl* CPlayerAIOutControl::Create(CPlayer* player)
{
	// メモリの確保
	CPlayerAIOutControl* pObj = DEBUG_NEW CPlayerAIOutControl;
	if (pObj != nullptr)
	{
		pObj->m_pAIOut = player;

		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerAIOutControl::Init()
{
	// 値の初期化


	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAIOutControl::Uninit()
{
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIOutControl::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ボールを見る
	LookBall();

	// モード管理
	ModeManager();

	// 状態更新
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// モード管理
//==========================================================================
void CPlayerAIOutControl::ModeManager()
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() == nullptr || pBall->GetTypeTeam() != m_pAIOut->GetTeam())
	{// ボールが取得されていない場合||自分とボールを持っているチームが違う場合
		m_eMode = EMode::MODE_MOVE;
	}
	else if (pBall->GetPlayer() == m_pAIOut)
	{// ボールを持っているのが自分だった場合
		m_eMode = EMode::MODE_THROW;
	}
}

//==========================================================================
// 投げ管理
//==========================================================================
void CPlayerAIOutControl::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報(外野)の取得
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();
	CPlayerControlMove* pControlMove = m_pAIOut->GetBase()->GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// 歩きオフ
	pControlAIOutMove->SetIsWalk(false);

	if (m_eThrow == EThrow::THROW_NONE)
	{// タイプ無の場合
		int n = rand() % 10;
		//int n = 1;

		if (n < 3)
		{
			m_eThrow = EThrow::THROW_NORMAL;
		}
		else
		{
			m_eThrow = EThrow::THROW_PASS;
		}
	}
	
	// 投げる種類更新
	(this->*(m_ThrowFunc[m_eThrow]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 通常投げ
//==========================================================================
void CPlayerAIOutControl::ThrowNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報(外野)の取得
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();

	if (!pControlAIOutAction->GetIsFlag().bThrow)
	{// フラグがオフの時
		m_eThrow = EThrow::THROW_NONE;
	}

	// 投げる
	Throw();
}

//==========================================================================
// パス
//==========================================================================
void CPlayerAIOutControl::ThrowPass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報(外野)の取得
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();

	if (!pControlAIOutAction->GetIsFlag().bPass)
	{// フラグがオフの時
		m_eThrow = EThrow::THROW_NONE;
	}

	// パス
	Pass();
}

//==========================================================================
// パス処理
//==========================================================================
void CPlayerAIOutControl::Pass()
{
	// AIコントロール情報(外野)の取得
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();

	// 投げる
	pControlAIOutAction->SetIsPass(true);
}

//==========================================================================
// ボールを投げる
//==========================================================================
void CPlayerAIOutControl::Throw()
{
	// AIコントロール情報(外野)の取得
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();

	// ターゲット設定
	m_pAIOut->SetRotDest(m_pAIOut->GetPosition().AngleXZ(GetThrowTarget()->GetPosition()));

	// 投げる
	pControlAIOutAction->SetIsThrow(true);
}

//==========================================================================
// ボールを見る
//==========================================================================
void CPlayerAIOutControl::LookBall()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	if (m_pAIOut->GetBall()) return;

	// 自分からボールの位置を見る
	float rot = m_pAIOut->GetPosition().AngleXZ(pBall->GetPosition());

	// 設定
	m_pAIOut->SetRotDest(rot);
}

//==========================================================================
// ボールのエリア判定
//==========================================================================
void CPlayerAIOutControl::AreaCheck()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	MyLib::Vector3 ballPos = pBall->GetPosition();

	// 自分のチームを取得
	CGameManager::ETeamSide team = m_pAIOut->GetTeam();
	
	if (team == CGameManager::ETeamSide::SIDE_LEFT)
	{// 左チーム
		if (ballPos.x < 0)
		{
			m_eMove = EMove::MOVE_NONE;
			return;
		}
	}
	else if (team == CGameManager::ETeamSide::SIDE_RIGHT)
	{// 右チーム
		if (ballPos.x > 0)
		{
			m_eMove = EMove::MOVE_NONE;
			return;
		}
	}
}

//==========================================================================
// 行動管理
//==========================================================================
void CPlayerAIOutControl::ModeMoveManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() != nullptr)
	{// 誰かがボールを持っている場合
		m_eMove = EMove::MOVE_NONE;
	}
	else if (pBall->GetPlayer() == nullptr)
	{// 誰もボールを持っていない場合

		// ボールを取りに行く
		m_eMove = EMove::MOVE_FIND;

		// エリア
		AreaCheck();
	}

	// 投げる種類更新
	(this->*(m_MoveFunc[m_eMove]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// なし
//==========================================================================
void CPlayerAIOutControl::MoveNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報(外野)の取得
	CPlayerControlMove* pControlMove = m_pAIOut->GetBase()->GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// 歩きオフ
	pControlAIOutMove->SetIsWalk(false);

	// キャッチ無
	m_eMove = EMove::MOVE_NONE;
}

//==========================================================================
// ボールの回収
//==========================================================================
void CPlayerAIOutControl::MoveRetrieve(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報(外野)の取得
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();
	CPlayerControlMove* pControlMove = m_pAIOut->GetBase()->GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall == nullptr || pBall->GetPlayer() != nullptr)
	{// ボールがnullptr||プレイヤーがボールを取っている||エリア外の場合

		// 歩きオフ！
		pControlAIOutMove->SetIsWalk(false);

		return;
	}

	// 角度を求める(playerからみたボール)
	float fAngle = m_pAIOut->GetPosition().AngleXZ(pBall->GetPosition());

	// 外野ポジションの取得
	CPlayerManager::EOutPos eOutPos = CPlayerManager::GetInstance()->GetOutPosition(m_pAIOut);

	float fDest = fAngle;

	if ((eOutPos == CPlayerManager::EOutPos::OUT_LEFT_FAR) ||
		(eOutPos == CPlayerManager::EOutPos::OUT_RIGHT_FAR))
	{// ポジションが奥


	}
	else if (
		(eOutPos == CPlayerManager::EOutPos::OUT_LEFT_NEAR) ||
		(eOutPos == CPlayerManager::EOutPos::OUT_RIGHT_NEAR))
	{// ポジションが手前

		fDest *= -1.0f;

	}
	else if (eOutPos == CPlayerManager::EOutPos::OUT_LEFT)
	{// 左
		if (fAngle > -D3DX_PI * 0.5f)
		{
			fDest *= -1.0f;
		}
	}
	else if (eOutPos == CPlayerManager::EOutPos::OUT_RIGHT)
	{// 右
		if (fAngle < D3DX_PI * 0.5f)
		{
			fDest *= -1.0f;
		}
	}

	// 向きの設定
	pControlAIOutMove->SetVecRot(fDest);

	// 歩きオン!
	pControlAIOutMove->SetIsWalk(true);

	// 方向設定
	m_pAIOut->SetRotDest(fAngle);
}

//==========================================================================
// 対面
//==========================================================================
void CPlayerAIOutControl::MoveMeeting(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報(外野)の取得
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();
	CPlayerControlMove* pControlMove = m_pAIOut->GetBase()->GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr)return;

	// ボールの持ち主情報の取得
	CPlayer* pBallPlayer = pBall->GetPlayer();

	if (pBallPlayer == nullptr) {// ボールを取っていない
		pControlAIOutMove->SetIsWalk(false);
		return;
	}

	// ボールの持ち主と違うチームの場合
	if (pBallPlayer->GetTeam() != m_pAIOut->GetTeam()) return;

	// 角度を求める(playerからみたボール)
	float fAngle = m_pAIOut->GetPosition().AngleXZ(pBallPlayer->GetPosition());

	// 外野ポジションの取得
	CPlayerManager::EOutPos eOutPos = CPlayerManager::GetInstance()->GetOutPosition(m_pAIOut);

	float fDest = fAngle;

	if (eOutPos == CPlayerManager::EOutPos::OUT_LEFT)
	{// 左
		if (fAngle > -D3DX_PI * 0.5f)
		{
			fDest *= -1.0f;
		}
	}
	else if (eOutPos == CPlayerManager::EOutPos::OUT_RIGHT)
	{// 右
		if (fAngle < D3DX_PI * 0.5f)
		{
			fDest *= -1.0f;
		}
	}

	// 自分から見たボールの持ち主
	float rot = m_pAIOut->GetPosition().AngleXZ(pBallPlayer->GetPosition());

	if (rot > 0)
	{// カニ歩き設定
		m_pAIOut->GetBase()->GetPlayerOut()->SetIsClab(true);
	}
	else
	{
		m_pAIOut->GetBase()->GetPlayerOut()->SetIsClab(false);
	}

	// 歩きオン!
	pControlAIOutMove->SetIsWalk(true);

	// 向きの設定
	pControlAIOutMove->SetVecRot(fDest);

	// 方向設定
	m_pAIOut->SetRotDest(fAngle);
}

//==========================================================================
// パス行動の判定処理
//==========================================================================
bool CPlayerAIOutControl::IsPass()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return false;

	// パス状態&&ターゲットが自分の場合
	if (pBall->IsPass() && pBall->GetTarget() == m_pAIOut) return false;

	return true;
}

//==========================================================================
// ターゲット設定(投げる時)
//==========================================================================
CPlayer* CPlayerAIOutControl::GetThrowTarget()
{
	CPlayer* pTarget = nullptr;	// 目標ターゲット
	float fMinDis = 1000000.0f;	// 近いプレイヤー

	MyLib::Vector3 Mypos = m_pAIOut->GetPosition();	// 位置情報の取得

	// ボールの取得
	CBall* pBall = m_pAIOut->GetBall();
	if (pBall == nullptr) { return pTarget; }
	CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// チームタイプの取得

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// プレイヤー位置

		// 味方チーム||外野の場合
		if ((typeTeam == pPlayer->GetTeam()) ||
			(pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT))
		{
			continue;
		}

		// 味方との距離を求める
		float fLength = Mypos.DistanceXZ(posPlayer);

		if (fLength < fMinDis)
		{ // より近い味方がいた場合

			// 最小距離の更新
			fMinDis = fLength;

			// ターゲットを更新
			pTarget = pPlayer;

			// 方向設定(そっちを向く)
			m_pAIOut->SetRotDest(Mypos.AngleXZ(pTarget->GetPosition()));
		}
	}

	return pTarget;
}

