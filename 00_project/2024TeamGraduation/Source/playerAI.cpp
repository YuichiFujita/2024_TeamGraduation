//=============================================================================
// 
//  AIプレイヤー処理 [playerAI.cpp]
//  Author : 藤田勇一
// 
//=============================================================================
#include "playerAI.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "teamStatus.h"


// 使用クラス
#include "playerStatus.h"
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace timing
{
	// タイミング
	const float TIMING_NONE = 0.0f;	// 通常
	const float TIMING_NORMAL = 1.0f;	// 通常
	const float TIMING_QUICK = 0.2f;	// 速く
	const float TIMING_DELAY = 2.0f;	// 遅く
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerAI::MODE_FUNC CPlayerAI::m_ModeFunc[] =	// モード関数
{
	&CPlayerAI::ModeNone,				// なし
	&CPlayerAI::ModeThrowManager,		// 投げマネージャー
	&CPlayerAI::ModeCatchManager,		// キャッチマネージャー
};

CPlayerAI::TYPE_FUNC CPlayerAI::m_ThrowTypeFunc[] =	// 投げ種類関数
{
	&CPlayerAI::TypeNone,				// なし
	&CPlayerAI::TypeThrow,				// 通常投げ
	&CPlayerAI::TypeJumpThrow,			// ジャンプ投げ
	&CPlayerAI::TypeSpecialThrow,		// スペシャル投げ
};

CPlayerAI::MOVE_FUNC CPlayerAI::m_ThrowMoveFunc[] =	// 投げ状態関数
{
	&CPlayerAI::MoveNormal,			// 通常(その場)
	&CPlayerAI::MoveWalk,				// 歩き
	&CPlayerAI::MoveDash,				// 走り
};

CPlayerAI::TIMING_FUNC CPlayerAI::m_ThrowTimingFunc[] =	// タイミング関数
{
	&CPlayerAI::TimingNormal,			// 通常
	&CPlayerAI::TimingQuick,			// 速く
	&CPlayerAI::TimingDelay,			// 遅く
	& CPlayerAI::TimingFeint,			// フェイント
};


//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAI::CPlayerAI()
{
	m_eMode = EMode::MODE_NONE;
	m_eThrowType = EThrowType::TYPE_NONE;
	m_eThrowMove = EThrowMove::MOVE_NORMAL;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;

	m_fTimingCount = 0.0f;
	m_bTiming = false;
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAI::~CPlayerAI()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CPlayerAI *CPlayerAI::Create(const CGameManager::TeamSide team, const MyLib::Vector3& rPos)
{
	// メモリの確保
	CPlayerAI* pPlayer = DEBUG_NEW CPlayerAI;
	if (pPlayer != nullptr)
	{
		// クラスの初期化
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pPlayer);
			return nullptr;
		}

		// チームサイドを設定
		pPlayer->GetStatus()->SetTeam(team);

		// 位置を設定
		pPlayer->SetPosition(rPos);
	}

	return pPlayer;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerAI::Init()
{
	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 親クラスの初期化
	HRESULT hr = CPlayer::Init();
	if (FAILED(hr)) { return E_FAIL; }

	// 操作関連
	ChangeMoveControl(DEBUG_NEW CPlayerAIControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());

	m_fTimingCount = timing::TIMING_QUICK;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAI::Uninit()
{
	// 親クラスの終了
	CPlayer::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CPlayerAI::Kill()
{
	// 親クラスの終了
	CPlayer::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親クラスの更新
	CPlayer::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 状態更新
	UpdateMode(fDeltaTime, fDeltaRate, fSlowRate);

	// 向き
	MyLib::Vector3 rot = GetRotation();

	float fRange = D3DX_PI * 0.25f;

	float fRangeMinMax[] =
	{
		D3DX_PI * 1.0f + fRange,	// 上
		D3DX_PI * 1.0f - fRange,	// 上
		D3DX_PI * 0.0f + fRange,	// 下
		D3DX_PI * 0.0f - fRange,	// 下
		D3DX_PI * -0.5f + fRange,	// 右
		D3DX_PI * -0.5f - fRange,	// 右
		D3DX_PI * 0.5f + fRange,	// 左
		D3DX_PI * 0.5f - fRange,	// 左
	};

	for (int i = 0; i < sizeof(fRangeMinMax) / sizeof(fRangeMinMax[0]); i++)
	{
		UtilFunc::Transformation::RotNormalize(fRangeMinMax[i]);

		MyLib::Vector3 move(
			sinf(fRangeMinMax[i] + D3DX_PI) * 15.0f,
			0.0f,
			cosf(fRangeMinMax[i] + D3DX_PI) * 15.0f
		);

		CEffect3D::Create(
			GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f),
			move,
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 0.5f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}

	{
		MyLib::Vector3 move(
			sinf(fRangeMinMax[0] + D3DX_PI) * 15.0f,
			0.0f,
			cosf(fRangeMinMax[0] + D3DX_PI) * 15.0f
		);

		CEffect3D::Create(
			GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f),
			move,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			20.0f, 0.5f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}

	{
		MyLib::Vector3 move(
			sinf(fRangeMinMax[1] + D3DX_PI) * 15.0f,
			0.0f,
			cosf(fRangeMinMax[1] + D3DX_PI) * 15.0f
		);

		CEffect3D::Create(
			GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f),
			move,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			20.0f, 0.5f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayerAI::Draw()
{
	// 親クラスの描画
	CPlayer::Draw();
}

//==========================================================================
// ヒット処理
//==========================================================================
CPlayerAI::SHitInfo CPlayerAI::Hit(CBall* pBall)
{
	CPlayer::SHitInfo hitInfo = CPlayer::Hit(pBall);	// ヒット情報

	if (hitInfo.eHit == EHit::HIT_NONE){
		return hitInfo;
	}

	if (hitInfo.eHit == EHit::HIT_CATCH)
	{// キャッチ状態

		// 投げモード
		m_eMode = EMode::MODE_THROW;
	}

	return hitInfo;
}

//==========================================================================
// 投げの管理
//==========================================================================
void CPlayerAI::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CGameManager* pGameManager = CGame::GetInstance()->GetGameManager();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(this->GetCharStatus()->GetTeam());

	if (pTeamStatus->IsMaxSpecial())
	{// スペシャル
		m_eThrowType = EThrowType::TYPE_SPECIAL;
	}
	else
	{
		int n = rand() % 2;

		/*switch (n)
		{
		case 0:
			m_eThrowType = EThrowType::TYPE_NORMAL;
			break;

		case 1:*/
			m_eThrowType = EThrowType::TYPE_JUMP;
			/*break;

		default:
			break;
		}*/

		// 投げる種類更新
		(this->*(m_ThrowTypeFunc[m_eThrowType]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// キャッチの管理
//==========================================================================
void CPlayerAI::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 通常投げ処理
//==========================================================================
void CPlayerAI::TypeThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 動きの設定 : 現在(その場)
	m_eThrowMove = EThrowMove::MOVE_NORMAL;

	// 投げる動き更新
	(this->*(m_ThrowMoveFunc[m_eThrowMove]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ジャンプ投げ処理
//==========================================================================
void CPlayerAI::TypeJumpThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 動きの設定 : 現在(その場)
	m_eThrowMove = EThrowMove::MOVE_NORMAL;

	// 投げる動き更新
	(this->*(m_ThrowMoveFunc[m_eThrowMove]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// スペシャル投げ処理
//==========================================================================
void CPlayerAI::TypeSpecialThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// スペシャル投げ
	pControlAIAction->SetIsSpecial(true);

	// それぞれの状態を戻す
	m_eMode = EMode::MODE_NONE;
	m_eThrowType = EThrowType::TYPE_NONE;
}

//==========================================================================
// 通常投げ(その場)
//==========================================================================
void CPlayerAI::MoveNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイミング管理
	TimingManager(fDeltaTime, fDeltaRate, fSlowRate);

	// 投げる動き更新
	(this->*(m_ThrowTimingFunc[m_eThrowTiming]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 歩き投げ
//==========================================================================
void CPlayerAI::MoveWalk(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 走り投げ
//==========================================================================
void CPlayerAI::MoveDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// タイミング管理
//==========================================================================
void CPlayerAI::TimingManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_bTiming)
	{
		// タイミングの設定
		int n = rand() % 3;

		switch (n)
		{
		case 0:
			m_eThrowTiming = EThrowTiming::TIMING_NORMAL;
			m_fTimingCount = timing::TIMING_NORMAL;
			m_bTiming = true;
			break;

		case 1:
			m_eThrowTiming = EThrowTiming::TIMING_QUICK;
			m_fTimingCount = timing::TIMING_QUICK;
			m_bTiming = true;
			break;

		case 2:
			m_eThrowTiming = EThrowTiming::TIMING_DELAY;
			m_fTimingCount = timing::TIMING_DELAY;
			m_bTiming = true;
			break;
		default:
			break;
		}
	}
}

//==========================================================================
// 通常タイミング
//==========================================================================
void CPlayerAI::TimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	if (m_eThrowType == EThrowType::TYPE_JUMP)
	{
		// 投げる
		pControlAIAction->SetIsJump(true);
		m_eThrowType = EThrowType::TYPE_NONE;
	}

	if (m_fTimingCount > 0)
	{// カウントダウン
		m_fTimingCount -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// 投げる
	pControlAIAction->SetIsThrow(true);

	// それぞれの状態のリセット
	m_eMode = EMode::MODE_NONE;
	m_eThrowType = EThrowType::TYPE_NONE;
	m_eThrowMove = EThrowMove::MOVE_NORMAL;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;

	// タイミングリセット
	m_fTimingCount = timing::TIMING_NONE;
	m_bTiming = false;

	pControlAIAction->SetIsJump(false);
}

//==========================================================================
// 速いタイミング
//==========================================================================
void CPlayerAI::TimingQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	if (m_eThrowType == EThrowType::TYPE_JUMP)
	{
		// 投げる
		pControlAIAction->SetIsJump(true);
		m_eThrowType = EThrowType::TYPE_NONE;
	}

	if (m_fTimingCount > 0)
	{// カウントダウン
		m_fTimingCount -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// 投げる
	pControlAIAction->SetIsThrow(true);

	// それぞれの状態のリセット
	m_eMode = EMode::MODE_NONE;
	m_eThrowType = EThrowType::TYPE_NONE;
	m_eThrowMove = EThrowMove::MOVE_NORMAL;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;

	// カウントのリセット
	m_fTimingCount = timing::TIMING_NONE;
	m_bTiming = false;

	pControlAIAction->SetIsJump(false);
}

//==========================================================================
// 遅いタイミング
//==========================================================================
void CPlayerAI::TimingDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	if (m_eThrowType == EThrowType::TYPE_JUMP)
	{
		// 投げる
		pControlAIAction->SetIsJump(true);
		m_eThrowType = EThrowType::TYPE_NONE;
	}

	if (m_fTimingCount > 0)
	{// カウントダウン
		m_fTimingCount -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// 投げる
	pControlAIAction->SetIsThrow(true);

	// それぞれの状態のリセット
	m_eMode = EMode::MODE_NONE;
	m_eThrowType = EThrowType::TYPE_NONE;
	m_eThrowMove = EThrowMove::MOVE_NORMAL;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;

	// カウントのリセット
	m_fTimingCount = timing::TIMING_NONE;
	m_bTiming = false;

	pControlAIAction->SetIsJump(false);
}

//==========================================================================
// フェイント
//==========================================================================
void CPlayerAI::TimingFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 状態更新
//==========================================================================
void CPlayerAI::UpdateMode(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態更新
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 操作処理
//==========================================================================
void CPlayerAI::Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// 移動操作	// TODO：AIむじ～
	//m_pControlMove->Move(this, fDeltaTime, fDeltaRate, fSlowRate);
	pControlAction->Action(this, fDeltaTime, fDeltaRate, fSlowRate);

	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayerAI::ChangeMoveControl(CPlayerAIControlMove* control)
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();

	delete pControlMove;
	pControlMove = control;

	SetPlayerControlMove(pControlMove);
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayerAI::ChangeActionControl(CPlayerAIControlAction* control)
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	delete pControlAction;
	pControlAction = control;

	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// 操作関連削除
//==========================================================================
void CPlayerAI::DeleteControl()
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	if (pControlMove != nullptr)
	{// 移動操作
		delete pControlMove;
		pControlMove = nullptr;
	}

	if (pControlAction != nullptr)
	{// アクション操作
		delete pControlAction;
		pControlAction = nullptr;
	}

	// 操作関連削除
	CPlayer::DeleteControl();

	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}


//==========================================================================
// デバッグ処理
//==========================================================================
void CPlayerAI::Debug()
{
#if _DEBUG
	// デバッグ処理
	CPlayer::Debug();

	CPlayerAIControlAction* pControlAction = dynamic_cast<CPlayerAIControlAction*> (GetPlayerControlAction());
	if (pControlAction == nullptr) return;

	// 自動投げフラグ設定
	bool autoThrow = pControlAction->IsAutoThrow();
	if (ImGui::Checkbox("Change Auto Throw", &autoThrow))
	{ 
		pControlAction->SetEnableAutoThrow(autoThrow);
	}
#endif
}