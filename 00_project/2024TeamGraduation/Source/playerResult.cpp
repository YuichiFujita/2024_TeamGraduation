//=============================================================================
// 
//  リザルト用プレイヤー処理 [playerResult.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "playerResult.h"
#include "resultmanager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string CHARAFILE[CPlayer::EBody::BODY_MAX] =		// キャラクターファイル
	{
		"data\\TEXT\\character\\result\\setup_player.txt",
		"data\\TEXT\\character\\result\\setup_player_fat.txt",
		"data\\TEXT\\character\\result\\setup_player_gari.txt",
	};
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerResult::STATE_FUNC CPlayerResult::m_StateFunc[] =	// 状態関数
{
	&CPlayerResult::StateNone,		// なし
	&CPlayerResult::StateWin,		// 勝利
	&CPlayerResult::StateLose,		// 敗北
	&CPlayerResult::StateDraw,		// 引き分け
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CPlayerResult> CPlayerResult::m_List;		// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerResult::CPlayerResult(const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase, int nPriority) :
	CPlayer(typeTeam, typeArea, typeBase, nPriority),
	m_state			(EState::STATE_NONE),	// 状態
	m_fStateTime	(0.0f)					// 状態時間
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerResult::~CPlayerResult()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerResult::Init()
{
	// 種類の設定
	CObject::SetType(CObject::TYPE_PLAYER);

	// キャラ作成
	HRESULT hr = SetCharacter(CHARAFILE[GetBodyType()]);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// カメラの方向向きにする
	SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));

	// 待機にしておく
	SetState(EState::STATE_NONE);

	// ドレスアップ生成
	CreateDressUp();

	// プレイヤーリストに割当
	m_List.Regist(this);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerResult::Uninit()
{
	// 終了処理
	CPlayer::Uninit();

	// プレイヤーリストに割当
	m_List.Delete(this);
}

//==========================================================================
// 動的削除処理
//==========================================================================
void CPlayerResult::Kill()
{
	// 動的削除処理
	CPlayer::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerResult::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerResult::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態タイマー加算
	m_fStateTime += fDeltaTime * fSlowRate;

	// 状態更新
	if (m_StateFunc[m_state] != nullptr)
	{
		(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// なし
//==========================================================================
void CPlayerResult::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
// 勝利状態
//==========================================================================
void CPlayerResult::StateWin(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	if (pMotion->GetType() != EMotion::MOTION_WIN)
	{// 勝利モーション以外なら勝利にする

		// 勝利モーション設定
		CMotion::Info info = GetMotion()->GetInfo(EMotion::MOTION_WIN);	// モーション情報
		int nKey = rand() % info.nNumKey;														// 開始キー
		float fFrame = (float)(rand() % info.aKey[nKey].nFrame);								// 開始フレーム
		pMotion->Set(EMotion::MOTION_WIN, 0, true, fFrame);
	}
}

//==========================================================================
// 敗北状態
//==========================================================================
void CPlayerResult::StateLose(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	if (pMotion->GetType() != EMotion::MOTION_LOSE)
	{// 敗北モーション以外なら敗北にする
		pMotion->Set(EMotion::MOTION_LOSE);
	}
}

//==========================================================================
// 引き分け状態
//==========================================================================
void CPlayerResult::StateDraw(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	if (pMotion->GetType() != EMotion::MOTION_DRAW)
	{// 敗北モーション以外なら敗北にする
		pMotion->Set(EMotion::MOTION_DRAW);
	}
}

//==========================================================================
// 勝敗チェック
//==========================================================================
void CPlayerResult::CheckVictoryNone()
{
	SetState(EState::STATE_NONE);
}

//==========================================================================
// 勝敗チェック(前座)
//==========================================================================
void CPlayerResult::CheckVictoryPrelude()
{
	CResultManager* pRsltMgr = CResultManager::GetInstance();
	CGameManager::ETeamSide teamWin = pRsltMgr->GetTeamPreludeWin();

	if (GetTeam() == teamWin)
	{// 勝利チームだったら
		SetState(EState::STATE_WIN);
	}
	else if(teamWin == CGameManager::ETeamSide::SIDE_NONE)
	{// 引き分けなら
		SetState(EState::STATE_DRAW);
	}
	else
	{// 敗北者なら
		SetState(EState::STATE_LOSE);
	}
}

//==========================================================================
// 勝敗チェック(モテ)
//==========================================================================
void CPlayerResult::CheckVictoryContest()
{
	CResultManager* pRsltMgr = CResultManager::GetInstance();
	CGameManager::ETeamSide teamWin = pRsltMgr->GetTeamContestWin();
	
	if (GetTeam() == teamWin)
	{// 勝利チームだったら
		SetState(EState::STATE_WIN);
	}
	else if (teamWin == CGameManager::ETeamSide::SIDE_NONE)
	{// 引き分けなら
		SetState(EState::STATE_DRAW);
	}
	else
	{// 敗北者なら
		SetState(EState::STATE_LOSE);
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayerResult::Draw()
{
	// 描画処理
	CPlayer::Draw();
}

//==========================================================================
// 状態設定
//==========================================================================
void CPlayerResult::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}
