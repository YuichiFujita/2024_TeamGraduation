//=============================================================================
// 
// ゲームマネージャ処理 [gamemanager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "gamemanager.h"
#include "game.h"
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "blackframe.h"
#include "map.h"
#include "calculation.h"
#include "fog.h"
#include "fade.h"
#include "input.h"
#include "ball.h"
#include "collisionLine_Box.h"
#include "teamStatus.h"
#include "audience.h"
#include "gymDoor.h"
#include "gymWallManager.h"
#include "playerSpawnManager.h"
#include "playerManager.h"
#include "charmManager.h"
#include "charmText.h"
#include "timerUI.h"

#include "resultManager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TOP_LINE = "#==============================================================================";	// テキストのライン
	const std::string TEXT_LINE = "#------------------------------------------------------------------------------";	// テキストのライン

	// ドッジボールコート情報
	namespace Gym
	{
		const MyLib::Vector3 POS_LEFT = MyLib::Vector3(-972.85f, 0.0f, 1717.35f);	// ドア左位置
		const MyLib::Vector3 POS_RIGHT = MyLib::Vector3(972.85f, 0.0f, 1717.35f);	// ドア右位置
		const MyLib::Vector3 POS[] = { POS_LEFT, POS_RIGHT };	// ドア位置
	}

	// ドッジボールコート情報
	namespace Court
	{
		const MyLib::Vector3 SIZE = MyLib::Vector3(950.0f, 100.0f, 560.0f);	// サイズ
		const float HALF_DIAGONAL = sqrtf(SIZE.x * SIZE.x + (SIZE.z * 2.0f) * (SIZE.z * 2.0f));	// チーム内コートの対角線
	}
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CGameManager::SCENE_FUNC CGameManager::m_SceneFunc[] =	// シーン関数
{
	&CGameManager::SceneMain,		// メイン
	&CGameManager::SceneSpawn,		// 登場演出
	&CGameManager::SceneStart,		// 開始演出
	&CGameManager::SceneSpecial,	// スペシャル演出
	&CGameManager::SceneEnd,		// 終了演出
	&CGameManager::SceneDebug,		// デバッグ
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CGameManager* CGameManager::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CGameManager::CGameManager()
{
	// 値のクリア
	m_SceneType = SCENE_MAIN;		// シーンの種類
	m_OldSceneType = SCENE_MAIN;	// シーンの種類(過去)
	m_bControll = false;			// 操作できるか
	m_fSceneTimer = 0.0f;			// シーンタイマー
	m_courtSize = MyLib::Vector3();	// コートサイズ
	m_endInfo = SEndInfo();			// 終了時情報

	m_pGymWallManager = nullptr;		// ジム壁マネジャー
	m_pCharmManager = nullptr;			// モテマネージャ
	m_pCharmValueManager = nullptr;		// モテ値マネージャ
	m_pSpecialValueManager = nullptr;	// スぺ値マネージャ
	m_pTimerUI = nullptr;				// タイマーUI

	memset(&m_apGymDoor[0], 0, sizeof(m_apGymDoor));		// 体育館のドア
	memset(&m_pTeamStatus[0], 0, sizeof(m_pTeamStatus));	// チームステータス
}

//==========================================================================
// デストラクタ
//==========================================================================
CGameManager::~CGameManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CGameManager* CGameManager::Create(CScene::MODE mode)
{
	if (m_pThisPtr != nullptr)
	{// 既にある場合
		return m_pThisPtr;
	}

	// インスタンス生成
	switch (mode)
	{
	case CScene::MODE_GAME:
		m_pThisPtr = DEBUG_NEW CGameManager;
		break;
	}

	if (m_pThisPtr != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		HRESULT hr = m_pThisPtr->Init();

		if (FAILED(hr))
		{// 失敗していたら
			return nullptr;
		}
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CGameManager::Init()
{
	m_bControll = true;	// 操作できるか

	// コートサイズ
	m_courtSize = Court::SIZE;

	for (int i = 0; i < EDoor::DOOR_MAX; i++)
	{ // ドアの配置数分繰り返す

		// 体育館ドア生成
		m_apGymDoor[i] = CGymDoor::Create(Gym::POS[i]);
		if (m_apGymDoor[i] == nullptr)
		{ // 生成に失敗した場合
	
			return E_FAIL;
		}
	}

	// ジム壁マネージャ生成
	m_pGymWallManager = CGymWallManager::Create();
	if (m_pGymWallManager == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

#if _DEBUG
	// コートサイズのボックス
	if (m_pCourtSizeBox == nullptr)
	{
		m_pCourtSizeBox = CCollisionLine_Box::Create(MyLib::AABB(-m_courtSize, m_courtSize), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
#endif

#if 1	// TODO : 藤田用に戻す
	// 開始シーンの設定
	SetSceneType(ESceneType::SCENE_SPAWN);	// 登場演出

	// プレイヤー登場演出マネージャー生成
	if (CPlayerSpawnManager::Create() == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 通常カメラの設定
	CCamera* pCamera = GET_MANAGER->GetCamera();	// カメラ情報
	pCamera->SetState(CCamera::STATE_NONE);
#else
	// 開始シーンの設定
	SetSceneType(ESceneType::SCENE_START);

	// プレイヤーマネージャーの生成
	CPlayerManager::Create();

	// 追従カメラの設定
	CCamera* pCamera = GET_MANAGER->GetCamera();	// カメラ情報
	pCamera->SetState(CCamera::STATE_FOLLOW);
#endif
	// チームステータスの生成
	CreateTeamStatus();

	// モテマネージャ生成
	m_pCharmManager = CCharmManager::Create();
	if (m_pCharmManager == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// モテ値マネージャ生成
	m_pCharmValueManager = CCharmValueManager::Create();
	if (m_pCharmValueManager == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// スぺ値マネージャ生成
	m_pSpecialValueManager = CSpecialValueManager::Create();
	if (m_pSpecialValueManager == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CGameManager::Uninit()
{
	// チームステータス
	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		SAFE_UNINIT(m_pTeamStatus[i]);
	}

#if _DEBUG
	// 終了
	if (m_pCourtSizeBox != nullptr)
	{
		m_pCourtSizeBox = nullptr;
	}
#endif

	// プレイヤーマネージャー
	CPlayerManager* pManager = CPlayerManager::GetInstance();
	SAFE_UNINIT(pManager);

	// モテマネージャ
	SAFE_UNINIT(m_pCharmManager);

	// モテ値マネージャ
	SAFE_UNINIT(m_pCharmValueManager);

	// スぺ値マネージャ
	SAFE_UNINIT(m_pSpecialValueManager);

	// 観客全消し
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		CAudience::SetDespawnAll(static_cast<CGameManager::ETeamSide>(i));
	}

	// 自身の開放
	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CGameManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// TODO : UpdateScene的なのでまとめたい
	m_fSceneTimer += fDeltaTime * fSlowRate;	// シーンタイマー
	
	// シーン別更新
	(this->*(m_SceneFunc[m_SceneType]))();

	CPlayerManager* pManager = CPlayerManager::GetInstance();
	if (pManager != nullptr)
	{
		// プレイヤーマネージャー更新
		pManager->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

#if _DEBUG	// デバッグ処理

	if (ImGui::TreeNode("GameManager"))
	{
		Debug();
		ImGui::TreePop();
	}
#endif
}

//==========================================================================
// ドア開放フラグの設定
//==========================================================================
void CGameManager::SetEnableOpen(const bool bOpen, const float fMoveTime)
{
	for (int i = 0; i < EDoor::DOOR_MAX; i++)
	{ // ドアの配置数分繰り返す

		// 体育館ドア開放フラグの設定
		m_apGymDoor[i]->SetEnableOpen(bOpen, fMoveTime);
	}
}

//==========================================================================
// ドア開放フラグの取得
//==========================================================================
bool CGameManager::IsOpen() const
{
	for (int i = 0; i < EDoor::DOOR_MAX; i++)
	{ // ドアの配置数分繰り返す

		// ドアが開放されていない場合抜ける
		if (!m_apGymDoor[i]->IsOpen()) { return false; }
	}

	return true;
}

//==========================================================================
// ドア稼働中フラグの取得
//==========================================================================
bool CGameManager::IsMove() const
{
	for (int i = 0; i < EDoor::DOOR_MAX; i++)
	{ // ドアの配置数分繰り返す

		// ドアが稼働中ではない場合抜ける
		if (!m_apGymDoor[i]->IsMove()) { return false; }
	}

	return true;
}

//==========================================================================
// スタート時の設定
//==========================================================================
void CGameManager::StartSetting()
{
	// メインへ遷移
	SetSceneType(ESceneType::SCENE_MAIN);

	// TODO：開始演出が終わったら遷移！
	{
#if _DEBUG
		// コートサイズのボックス
		if (m_pCourtSizeBox == nullptr)
		{
			m_pCourtSizeBox = CCollisionLine_Box::Create(MyLib::AABB(-m_courtSize, m_courtSize), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
#endif

		// チームステータス
		CreateTeamStatus();

		// タイマーUI
		if (m_pTimerUI == nullptr)
		{
			m_pTimerUI = CTimerUI::Create(
				90.0f,
				90.0f,
				MyLib::Vector3(640.0f, 70.0f, 0.0f),
				D3DXVECTOR2(40.0f, 40.0f),
				D3DXVECTOR2(30.0f, 40.0f),
				D3DXVECTOR2(40.0f, 0.0f),
				D3DXVECTOR2(60.0f, 0.0f),
				CTimerUI::EAlignX::XALIGN_CENTER,
				CTimerUI::EAlignY::YALIGN_CENTER,
				MyLib::Vector3(0.0f)
			);

			// 開始
#if _NDEBUG
			m_pTimerUI->Start();
#endif

		}

		// メインへ遷移
		SetSceneType(ESceneType::SCENE_MAIN);
	}
}

//==========================================================================
// メイン
//==========================================================================
void CGameManager::SceneMain()
{
	// 操作出来る
	m_bControll = true;

	// 観客更新
	UpdateAudience();

	// 制限時間更新
	UpdateLimitTimer();

	// チームステータス更新
	UpdateTeamStatus();
}

//==========================================================================
// 制限時間更新
//==========================================================================
void CGameManager::UpdateLimitTimer()
{
	// タイマー無いと抜ける
	if (m_pTimerUI == nullptr) return;

	// 終了したら
	if (m_pTimerUI->IsEnd())
	{
		SetSceneType(ESceneType::SCENE_END);
	}
}

//==========================================================================
// 登場演出
//==========================================================================
void CGameManager::SceneSpawn()
{
	// プレイヤー登場演出マネージャーの取得
	CPlayerSpawnManager* pManager = CPlayerSpawnManager::GetInstance();
	assert(pManager != nullptr);

	// 入力情報の取得
	CInputKeyboard* pKey = GET_INPUTKEY;
	CInputGamepad* pPad = GET_INPUTPAD;

	// 操作出来ない
	m_bControll = false;

	bool bInput = pKey->GetTrigger(DIK_RETURN)
			   || pKey->GetTrigger(DIK_SPACE)
			   || pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0)
			   || pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, 0)
			   || pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, 0)
			   || pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_Y, 0);

	if (bInput || pManager->GetState() == CPlayerSpawnManager::EState::STATE_END)
	{ // スキップ操作、または登場演出が終わった場合

		// 登場演出のスキップ
		SkipSpawn();
	}
}

//==========================================================================
// 開始演出
//==========================================================================
void CGameManager::SceneStart()
{
	// 操作出来ない
	m_bControll = false;
}

//==========================================================================
// スペシャル演出
//==========================================================================
void CGameManager::SceneSpecial()
{
	// 操作出来ない
	m_bControll = false;

	// スペシャル演出更新
	UpdateSpecial();
}

//==========================================================================
// 終了演出
//==========================================================================
void CGameManager::SceneEnd()
{
	// 操作出来ない
	m_bControll = false;

	// 遷移
	if (GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_RESULT))
	{// 一度だけ

		// 試合終了処理
		EndGame();
	}
}

//==========================================================================
// デバッグ
//==========================================================================
void CGameManager::SceneDebug()
{
	// 操作出来る
	m_bControll = true;
}

//==========================================================================
// 登場演出スキップ
//==========================================================================
void CGameManager::SkipSpawn()
{
	CPlayerSpawnManager* pManager = CPlayerSpawnManager::GetInstance();	// プレイヤー登場演出マネージャー
	assert(pManager != nullptr);

	// プレイヤー登場演出マネージャーの終了
	SAFE_UNINIT(pManager);

	// プレイヤーマネージャーの生成
	CPlayerManager::Create();

	// 追従カメラの設定
	CCamera* pCamera = GET_MANAGER->GetCamera();	// カメラ情報
	pCamera->SetState(CCamera::STATE_FOLLOW);

	// カメラモーションの終了
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// カメラモーション情報
	pCameraMotion->SetFinish(true);

	// 開始演出へ遷移
	SetSceneType(ESceneType::SCENE_START);
}

//==========================================================================
// 観客更新
//==========================================================================
void CGameManager::UpdateAudience()
{
	GET_MANAGER->GetDebugProc()->Print("\n----------------- 観客情報 -----------------\n");
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		CTeamStatus::SCharmInfo info = m_pTeamStatus[i]->GetCharmInfo();	// モテ情報
		float fMoteRate = info.fValue / info.fValueMax;				// モテ割合
		int nNumAudience = (int)(CAudience::MAX_WATCH * fMoteRate);	// 現在の観客数

		// 観客数を設定
		CAudience::SetNumWatch(nNumAudience, (CGameManager::ETeamSide)(i));
		GET_MANAGER->GetDebugProc()->Print("【チーム0%d観客】[%d]\n", i, nNumAudience);
	}
}

//==========================================================================
// スペシャル演出更新
//==========================================================================
void CGameManager::UpdateSpecial()
{

}

//==========================================================================
// チームステータス更新
//==========================================================================
void CGameManager::UpdateTeamStatus()
{
	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		if (m_pTeamStatus[i] == nullptr) continue;

		// スペシャル加算
		AddSpecialValue(m_pTeamStatus[i]->GetTeam(), CSpecialValueManager::ETypeAdd::ADD_ALLWAYS);

#if _DEBUG
#if 0
		return;
#endif	// デバッグ用
#endif

		// 全滅判定
		m_pTeamStatus[i]->CheckAllDead();
	}
}

//==========================================================================
// ボールの取得
//==========================================================================
CBall* CGameManager::GetBall()
{
	CListManager<CBall> list = CBall::GetListObj();	// ボールリスト
	assert(list.GetNumAll() == 1);	// 複数ボールがある場合エラー
	return *list.GetBegin();		// ボール情報を返す
}

//==========================================================================
// シーンの種類設定
//==========================================================================
void CGameManager::SetSceneType(ESceneType type)
{
	m_OldSceneType = m_SceneType;
	m_SceneType = type;
	m_fSceneTimer = 0.0f;
}

/**
@brief	コートサイズ取得(チーム)
@param	team[in]	取得したいチーム
@param	pos[out]	取得したコートの中心点
@return	コートサイズ
*/
MyLib::Vector3 CGameManager::GetCourtSize(const ETeamSide team, MyLib::Vector3& pos)
{
	// 片側コートサイズ
	MyLib::Vector3 size = m_courtSize;
	size.x *= 0.5f;

	// チームに応じた位置
	pos = MyLib::Vector3();
	switch (team)
	{
	case SIDE_LEFT:
		pos.x = -size.x;
		break;

	case SIDE_RIGHT:
		pos.x = size.x;
		break;

	default:
		break;
	}

	return size;
}

//==========================================================================
// チームコートの対角線取得
//==========================================================================
float CGameManager::GetHalfCourtDiagonal()
{
	// チームコートの対角線を返す
	return Court::HALF_DIAGONAL;
}

//==========================================================================
// コート移動制限
//==========================================================================
bool CGameManager::SetPosLimit(MyLib::Vector3& pos, const float fPlusRadius)
{
	bool bHit = false;

	if (pos.x > m_courtSize.x + fPlusRadius)
	{
		pos.x = m_courtSize.x + fPlusRadius;
		bHit = true;
	}
	else if (pos.x < -m_courtSize.x - fPlusRadius)
	{
		pos.x = -m_courtSize.x - fPlusRadius;
		bHit = true;
	}

	if (pos.z > m_courtSize.z + fPlusRadius)
	{
		pos.z = m_courtSize.z + fPlusRadius;
		bHit = true;
	}
	else if (pos.z < -m_courtSize.z - fPlusRadius)
	{
		pos.z = -m_courtSize.z - fPlusRadius;
		bHit = true;
	}

	return bHit;
}

//==========================================================================
// モテ加算
//==========================================================================
void CGameManager::AddCharmValue(ETeamSide side, CCharmValueManager::ETypeAdd charmType)
{
	// チームステータス
	float value = CCharmValueManager::GetInstance()->GetAddValue(charmType);
	m_pTeamStatus[side]->AddCharmValue(value);

	// モテ文字生成
	CCharmText::Create(side);
}

//==========================================================================
// モテ減算
//==========================================================================
void CGameManager::SubCharmValue(ETeamSide side, CCharmValueManager::ETypeSub charmType)
{
	// チームステータス
	float value = CCharmValueManager::GetInstance()->GetSubValue(charmType);
	m_pTeamStatus[side]->SubCharmValue(value);
}

//==========================================================================
// スペシャル加算
//==========================================================================
void CGameManager::AddSpecialValue(ETeamSide side, CSpecialValueManager::ETypeAdd ValueType)
{
	// チームステータス
	float value = CSpecialValueManager::GetInstance()->GetAddValue(ValueType);
	m_pTeamStatus[side]->AddSpecialValue(value);
}

//==========================================================================
// チームステータス生成
//==========================================================================
void CGameManager::CreateTeamStatus()
{
	ETeamSide side = CGameManager::SIDE_NONE;

	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->Uninit();
			m_pTeamStatus[i] = nullptr;
		}

		m_pTeamStatus[i] = CTeamStatus::Create();

		// チーム設定
		side = static_cast<ETeamSide>(i);
		m_pTeamStatus[i]->TeamSetting(side);
	}
}

//==========================================================================
// チームステータス保存
//==========================================================================
void CGameManager::Save()
{
	// ファイルを開く
	std::ofstream File(ResultManager::TEXTFILE);
	if (!File.is_open()) {
		return;
	}

	// 勝利
	int winteam = 0;
	float tension = 10.0f;

	// テキストファイル名目次
	File << TOP_LINE << std::endl;
	File << "# チーム情報" << std::endl;
	File << TOP_LINE << std::endl;
	
	File << TEXT_LINE << std::endl;
	File << "# 試合情報" << std::endl;
	File << TEXT_LINE << std::endl;
	File << "PRELUDE_WIN = " << m_endInfo.m_winteamPrelude << std::endl;
	File << "CONTEST_WIN = " << m_endInfo.m_winteamCharm << std::endl;
	File << "TENSION = "<< m_endInfo.m_fTension << std::endl;
	File << std::endl;

	int i = 0;
	for (const auto& team : m_pTeamStatus)
	{
		if (team == nullptr) continue;

		File << TEXT_LINE << std::endl;
		File << "# チーム" << i << std::endl;
		File << TEXT_LINE << std::endl;
		File << "SETTEAM" << std::endl;

		File << "	CHARMVALUE = " << team->GetCharmInfo().fValue << std::endl;
		File << std::endl;

		// リストループ
		CListManager<CPlayer> list = CPlayerManager::GetInstance()->GetInList(static_cast<ETeamSide>(i));
		std::list<CPlayer*>::iterator itr = list.GetEnd();
		CPlayer* pObj = nullptr;

		while (list.ListLoop(itr))
		{
			pObj = (*itr);
		
			File << "	SETPLAYER"  << std::endl;

			File << "		HANDED = "		<< pObj->GetHandedness() << std::endl;		// 利き手
			File << "		BODY = "		<< pObj->GetBodyType() << std::endl;		// 体型
			File << "		HAIR = "		<< 1 << std::endl;		// 髪着せ替え
			File << "		ACCESSORY= "	<< 1 << std::endl;		// アクセ着せ替え
			File << "		FACE = "		<< 1 << std::endl;		// 顔着せ替え

			File << "	END_SETPLAYER" << std::endl;
			File << std::endl;
		}

		File << "END_SETTEAM" << std::endl;
		File << std::endl;

		i++;
	}

	// ファイルを閉じる
	File.close();
}

//==========================================================================
// 試合終了
//==========================================================================
void CGameManager::EndGame()
{
	// TODO: テンションを求める


	// 勝利チーム決定
	CheckVictory();

	// 試合内容保存
	Save();
}

//==========================================================================
// 勝利チーム決定
//==========================================================================
void CGameManager::CheckVictory()
{
	int m_aAlive[ETeamSide::SIDE_MAX] = {};
	int m_aLife[ETeamSide::SIDE_MAX] = {};

	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		// リストループ
		CListManager<CPlayer> list = CPlayerManager::GetInstance()->GetInList(static_cast<ETeamSide>(i));
		std::list<CPlayer*>::iterator itr = list.GetEnd();
		CPlayer* pObj = nullptr;

		while (list.ListLoop(itr))
		{
			pObj = (*itr);
		
			if (pObj->GetState() != CPlayer::EState::STATE_DEAD ||
				pObj->GetState() != CPlayer::EState::STATE_DEAD)
			{// 人数加算
				m_aAlive[i]++;
			}

			// 体力加算
			m_aLife[i] += pObj->GetLife();
		}
	}

	if (m_aAlive[ETeamSide::SIDE_LEFT] > m_aAlive[ETeamSide::SIDE_RIGHT])
	{// 左が多い
		m_endInfo.m_winteamPrelude = ETeamSide::SIDE_LEFT;
	}
	else if(m_aAlive[ETeamSide::SIDE_LEFT] < m_aAlive[ETeamSide::SIDE_RIGHT])
	{// 右が多い
		m_endInfo.m_winteamPrelude = ETeamSide::SIDE_RIGHT;
	}
	else
	{// 左右同数

		// 総合体力差
		if (m_aLife[ETeamSide::SIDE_LEFT] > m_aLife[ETeamSide::SIDE_RIGHT])
		{// 左が多い
			m_endInfo.m_winteamPrelude = ETeamSide::SIDE_LEFT;
		}
		else if (m_aLife[ETeamSide::SIDE_LEFT] < m_aLife[ETeamSide::SIDE_RIGHT])
		{// 右が多い
			m_endInfo.m_winteamPrelude = ETeamSide::SIDE_RIGHT;
		}
		else
		{// 引き分け
			m_endInfo.m_winteamPrelude = ETeamSide::SIDE_NONE;
		}
	}

	//--------------------------
	//モテ
	
	float fCharmL = m_pTeamStatus[CGameManager::ETeamSide::SIDE_LEFT]->GetCharmInfo().fValue;
	float fCharmR = m_pTeamStatus[CGameManager::ETeamSide::SIDE_RIGHT]->GetCharmInfo().fValue;


	// モテ勝利
	if (fCharmL > fCharmR)
	{// 左が多い
		m_endInfo.m_winteamCharm = ETeamSide::SIDE_LEFT;
	}
	else if (fCharmL < fCharmR)
	{// 右が多い
		m_endInfo.m_winteamCharm = ETeamSide::SIDE_RIGHT;
	}
	else
	{
		m_endInfo.m_winteamCharm = ETeamSide::SIDE_NONE;
	}
}

//==========================================================================
// デバッグ
//==========================================================================
void CGameManager::Debug()
{
#if _DEBUG

	//-----------------------------
	// コート
	//-----------------------------
	if (ImGui::TreeNode("Court"))
	{
		if (m_pCourtSizeBox == nullptr)
		{
			m_pCourtSizeBox = CCollisionLine_Box::Create(MyLib::AABB(-m_courtSize, m_courtSize), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		if (ImGui::Button("Reset"))
		{// リセット
			m_courtSize = Court::SIZE;
		}
		ImGui::SameLine();

		ImGui::DragFloat3("size", (float*)&m_courtSize, 10.0f, 0.0f, 10000.0f, "%.2f");

		// サイズ設定
		if (m_pCourtSizeBox != nullptr)
		{
			m_pCourtSizeBox->SetAABB(MyLib::AABB(-m_courtSize, m_courtSize));
		}

		// 位置設定
		ImGui::TreePop();
	}

	// タイマー
	if (ImGui::TreeNode("Timer"))
	{

		if (m_pTimerUI != nullptr)
		{
			// 停止状況切り替え
			bool bStop = m_pTimerUI->IsStop();
			ImGui::Checkbox("bStop", &bStop);
			m_pTimerUI->EnableStop(bStop);
		}

		ImGui::TreePop();
	}

	// チームステータス
	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		//-----------------------------
		// コート
		//-----------------------------
		std::string treename = "Special" + std::to_string(i);	// ツリー名
		if (ImGui::TreeNode(treename.c_str()))
		{
			float fValue = m_pTeamStatus[i]->GetSpecialValue();

			ImGui::DragFloat("GaugeValue", (float*)&fValue, 1.0f, 0.0f, m_pTeamStatus[i]->GetSpecialInfo().fValueMax, "%.2f");

			// 位置設定
			ImGui::TreePop();

			m_pTeamStatus[i]->SetSpecialValue(fValue);
		}

		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->Debug();
		}
	}

	// 終了
	if (ImGui::Button("end"))
	{
		SetSceneType(ESceneType::SCENE_END);
	}

#endif
}
