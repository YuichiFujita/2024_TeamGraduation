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
#include "reporter.h"
#include "resultManager.h"
#include "gauge2D.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TOP_LINE = "#==============================================================================";	// テキストのライン
	const std::string TEXT_LINE = "#------------------------------------------------------------------------------";	// テキストのライン

	const float END_HYPE_TIME[] = { 9999.0f, 0.0f };	// 勝利チーム決定時の盛り上がり時間

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

	// タイマー情報
	namespace Timer
	{
		const float RATE = 0.7f;
		const MyLib::Vector3 POS = MyLib::Vector3(640.0f, 80.0f, 0.0f);	// タイマー位置
		const MyLib::Vector3 POS_BG = Timer::POS + MyLib::Vector3(0.0f, -20.0f, 0.0f);	// タイマー位置
		const MyLib::Vector2 VAL_SIZE = MyLib::Vector2(0.0f, 28.0f) * RATE;	// タイマー数字大きさ
		const MyLib::Vector2 PART_SIZE = MyLib::Vector2(0.0f, 18.0f) * RATE;	// タイマー区切り大きさ
		const MyLib::Vector2 VAL_SPACE = MyLib::Vector2(53.0f, 0.0f) * RATE;	// タイマー数字空白
		const MyLib::Vector2 PART_SPACE = MyLib::Vector2(35.0f, 0.0f) * RATE;	// タイマー区切り空白
		const char* TEXTURE_VALUE = "data\\TEXTURE\\number\\AKR.png";	// 数字テクスチャパス
		const char* TEXTURE_PART = "data\\TEXTURE\\timepart001.png";	// 区切りテクスチャパス
		const char* TEXTURE_BG = "data\\TEXTURE\\timer\\bg.png";		// 背景テクスチャパス
		const float HEIGHT_BG = 64.0f;									// 背景縦幅
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
	m_pTimerBG = nullptr;				// タイマー背景

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
	CPlayerManager::Create(CScene::MODE_GAME);

	// 追従カメラの設定
	CCamera* pCamera = GET_MANAGER->GetCamera();	// カメラ情報
	pCamera->SetState(CCamera::STATE_FOLLOW);
#endif

	// チームステータスの生成
	CreateTeamStatus();

	// 外部ファイルから設定されたタイムを読込
	CEntryRuleManager::SRule rule;		// ルール
	CEntryRuleManager::LoadRule(&rule);	// ルール読込
	if (rule.fTime > 0.0f)
	{ // 時間制限が無限じゃない場合

		CTexture* pTexture = CTexture::GetInstance();	// テクスチャ情報

		// タイマーUIの生成
		m_pTimerUI = CTimerUI::Create
		( // 引数
			rule.fTime,			// 開始時間
			rule.fTime,			// 制限時間
			Timer::POS,			// 位置
			Timer::VAL_SIZE.y,	// 数字の大きさ
			Timer::PART_SIZE.y,	// 区切りの大きさ
			Timer::VAL_SPACE,	// 数字の空白
			Timer::PART_SPACE	// 区切りの空白
		);
		if (m_pTimerUI == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// 数字テクスチャの再割当
		m_pTimerUI->BindTextureValue(pTexture->Regist(Timer::TEXTURE_VALUE));

		// 区切りテクスチャの再割当
		m_pTimerUI->BindTexturePart(pTexture->Regist(Timer::TEXTURE_PART));

		// 色設定
		m_pTimerUI->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// タイマーUIの非表示
		m_pTimerUI->SetEnableDisp(false);

		// タイマー背景の生成
		m_pTimerBG = CObject2D::Create();
		if (m_pTimerBG == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// 位置を設定
		m_pTimerBG->SetPosition(Timer::POS_BG);

		// テクスチャを登録・割当
		int texID = pTexture->Regist(Timer::TEXTURE_BG);
		m_pTimerBG->BindTexture(texID);

		// 画像比率から大きさを設定
		MyLib::Vector2 size = pTexture->GetImageSize(texID);
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, Timer::HEIGHT_BG);
		m_pTimerBG->SetSize(size);
		m_pTimerBG->SetSizeOrigin(size);

		// タイマー背景の非表示
		m_pTimerBG->SetEnableDisp(false);
	}

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

	// タイマーUI
	SAFE_UNINIT(m_pTimerUI);

	// タイマー背景
	SAFE_UNINIT(m_pTimerBG);

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

	if (m_pCharmManager != nullptr)
	{
		// モテマネージャー更新
		m_pCharmManager->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// ゲージ時間更新
	UpdateGaugeTime();

// 撮影用
#if 0
	if (m_pCharmManager != nullptr)
	{
		m_pCharmManager->SetHypeTime(CGameManager::ETeamSide::SIDE_LEFT, 10.0f);
	}
#endif

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
		StartTeamStatus();

		// タイマーの計測開始
		if (m_pTimerUI != nullptr)
		{
			m_pTimerUI->Start();
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
	SAFE_KILL(pManager);

	// ドア閉める
	for (const auto& door : m_apGymDoor)
	{
		door->SetEnableOpen(false);
	}

	if (m_pTimerUI != nullptr)
	{ // 生成されている場合

		// タイマーUIの表示
		m_pTimerUI->SetEnableDisp(true);
	}

	if (m_pTimerBG != nullptr)
	{ // 生成されている場合

		// タイマー背景の表示
		m_pTimerBG->SetEnableDisp(true);
	}

	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->SetEnableGaugeDisp(true);
		}
	}

	// プレイヤーマネージャーの生成
	CPlayerManager::Create(CPlayerManager::EType::TYPE_GAME);

	// 実況生成
	CReporter::Create(ETeamSide::SIDE_LEFT);
	CReporter::Create(ETeamSide::SIDE_RIGHT);

	// 追従カメラの設定
	CCamera* pCamera = GET_MANAGER->GetCamera();	// カメラ情報
	pCamera->SetState(CCamera::STATE_FOLLOW);

	// カメラモーションの終了
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// カメラモーション情報
	pCameraMotion->SetFinish(true);

	// 開始演出へ遷移
	SetSceneType(ESceneType::SCENE_START);

	// BGM再生
	CSound* pSound = CSound::GetInstance();
	pSound->StopSound();
	pSound->PlaySound(CSound::ELabel::LABEL_BGM_GAME);
}

//==========================================================================
// 観客更新
//==========================================================================
void CGameManager::UpdateAudience()
{
#if 1
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
#endif	// 観客を出さない
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
	bool bAllDead[ETeamSide::SIDE_MAX] = { false, false };	// 敗北フラグ

	// 各チーム情報
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
		bAllDead[i] = m_pTeamStatus[i]->CheckAllDead();
	}

	if (bAllDead[ETeamSide::SIDE_LEFT] != bAllDead[ETeamSide::SIDE_RIGHT])
	{ // どちらかが全滅した場合

		for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
		{ // チーム数分繰り返す

			// 盛り上がり時間の設定
			int nDead = (int)bAllDead[i];	// 整数変換した死亡フラグ
			m_pCharmManager->SetHypeTime((ETeamSide)i, END_HYPE_TIME[i]);
		}
	}
}

//==========================================================================
// ゲージ時間更新
//==========================================================================
void CGameManager::UpdateGaugeTime()
{
	// スペシャルゲージの時間更新
	float fBrightTime = CGauge2D::GetBrightTime();
	fBrightTime += GET_MANAGER->GetDeltaTime() * GET_MANAGER->GetSlowRate();

	if (fBrightTime >= CGauge2D::GetBrightTimeEnd())
	{// ループ
		fBrightTime = 0.0f;
	}
	CGauge2D::SetBrightTime(fBrightTime);
	ImGui::Text("fBrightTime : % .2f", fBrightTime);
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

//==========================================================================
// タイマー停止フラグ設定
//==========================================================================
void CGameManager::SetEnableTimerStop(const bool bStop)
{
	m_pTimerUI->EnableStop(bStop);
}

//==========================================================================
// @brief	コートサイズ取得(チーム)
// @param	team[in]	取得したいチーム
// @param	pos[out]	取得したコートの中心点
// @return	コートサイズ
//==========================================================================
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
// @brief	コート中心地取得(チーム)
// @param	team[in]	取得したいチーム
// @return	取得したコートの中心点
//==========================================================================
MyLib::Vector3 CGameManager::GetCourtMiddle(const ETeamSide team)
{
	// 片側コートサイズ
	MyLib::Vector3 size = m_courtSize;
	size.x *= 0.5f;

	// チームに応じた位置
	MyLib::Vector3 pos = MyLib::Vector3();
	switch (team)
	{
	case SIDE_LEFT:
		pos.x = -size.x;
		break;

	case SIDE_RIGHT:
		pos.x = size.x;
		break;

	default:
		pos.x = 0.0f;
		break;
	}

	return pos;
}

//==========================================================================
// @brief	コートの何パー位置か取得(チーム)
// @param	team[in]	取得したいチーム
// @param	pos[out]	取得したコートの中心点
// @return	コートサイズ
//==========================================================================
float CGameManager::GetCourtPosPercentage(const ETeamSide team, MyLib::Vector3 pos)
{
	MyLib::Vector3 posCourt = MyLib::Vector3();
	MyLib::Vector3 sizeCourt = GetCourtSize(team, posCourt);

	// 最小大値
	float min = posCourt.x - sizeCourt.x;
	float max = posCourt.x + sizeCourt.x;

	// 割合
	float fRate = UtilFunc::Transformation::ValueToRate(pos.x, min, max);

	if (team == ETeamSide::SIDE_LEFT)
	{// 左は割合反転
		fRate = 1.0f - fRate;
	}

	return fRate;
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
	MyAssert::CustomAssert(side != CGameManager::ETeamSide::SIDE_NONE, "モテたチームisどこ？？");

	// チームステータス
	float value = CCharmValueManager::GetInstance()->GetAddValue(charmType);
	m_pTeamStatus[(int)side]->AddCharmValue(value);

	assert(m_pCharmManager != nullptr);
	if (m_pCharmManager->GetPrisetHypeTime(charmType) > m_pCharmManager->GetHypeTime(side))
	{ // 設定予定の盛り上がり時間が今の盛り上がり時間より長い場合

		// 盛り上がり時間の設定
		m_pCharmManager->SetHypeTime(side, charmType);
	}

	// モテ文字生成
	CCharmText::Create(side);
}

//==========================================================================
// モテ減算
//==========================================================================
void CGameManager::SubCharmValue(ETeamSide side, CCharmValueManager::ETypeSub charmType)
{
	MyAssert::CustomAssert(side != CGameManager::ETeamSide::SIDE_NONE, "モテたチームisどこ？？");

	// チームステータス
	float value = CCharmValueManager::GetInstance()->GetSubValue(charmType);
	m_pTeamStatus[(int)side]->SubCharmValue(value);

	// 盛り上がり時間の初期化
	assert(m_pCharmManager != nullptr);
	m_pCharmManager->SetHypeTime(side, 0.0f);
}

//==========================================================================
// スペシャル加算
//==========================================================================
void CGameManager::AddSpecialValue(ETeamSide side, CSpecialValueManager::ETypeAdd ValueType)
{
	MyAssert::CustomAssert(side != CGameManager::ETeamSide::SIDE_NONE, "モテたチームisどこ？？");

	// チームステータス
	float value = CSpecialValueManager::GetInstance()->GetAddValue(ValueType);
	m_pTeamStatus[(int)side]->AddSpecialValue(value);
}

//==========================================================================
// ライバルチームに変換
//==========================================================================
CGameManager::ETeamSide CGameManager::RivalTeam(ETeamSide team)
{
	switch (team)
	{
	case CGameManager::SIDE_LEFT:
		return SIDE_RIGHT;
		break;

	case CGameManager::SIDE_RIGHT:
		return SIDE_LEFT;
		break;

	default:
		return SIDE_NONE;
		break;
	}
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
		m_pTeamStatus[i]->SetEnableGaugeDisp(false);

		// チーム設定
		side = static_cast<ETeamSide>(i);
		m_pTeamStatus[i]->TeamSetting(side);
	}
}

//==========================================================================
// チームステータス開始時
//==========================================================================
void CGameManager::StartTeamStatus()
{
	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->SetEnableGaugeDisp(true);
		}
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
	int m_aAlive[ETeamSide::SIDE_MAX] = {};		// 総人数
	int m_aLife[ETeamSide::SIDE_MAX] = {};		// 総体力

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

			// 総体力加算
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

			m_pTeamStatus[i]->SetSpecialValue(fValue);

			// 位置設定
			ImGui::TreePop();
		}

		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->Debug();
		}
	}

	// 盛り上がり状態(一瞬？)
	if (ImGui::TreeNode("ExciteSetting"))
	{
		if (m_pCharmManager != nullptr)
		{
			if (ImGui::Button("Left"))
			{
				m_pCharmManager->SetHypeTime(CGameManager::ETeamSide::SIDE_LEFT, 10.0f);
			}
			if (ImGui::Button("Right"))
			{
				m_pCharmManager->SetHypeTime(CGameManager::ETeamSide::SIDE_RIGHT, 10.0f);
			}
		}
	
		ImGui::TreePop();
	}

	// ゲージのmax時間
	if (ImGui::TreeNode("GaugeBright"))
	{
		float fBright = CGauge2D::GetBrightTime();
		float fBrightEnd = CGauge2D::GetBrightTimeEnd();

		ImGui::Text("BrightTime: [%.2f]", fBright);
		ImGui::DragFloat("fBrightEnd", &fBrightEnd, 0.01f, 0.0f, 10.0f, "%.2f");

		CGauge2D::SetBrightTimeEnd(fBrightEnd);

		ImGui::TreePop();
	}

	// モテ値マネージャー
	if (m_pCharmValueManager != nullptr)
	{
		m_pCharmValueManager->Debug();
	}

	// 終了
	if (ImGui::Button("end"))
	{
		SetSceneType(ESceneType::SCENE_END);
	}

	// 警戒(L)
	if (ImGui::Button("(Caution: L)"))
	{
		CPlayerManager* pPlrMgr = CPlayerManager::GetInstance();
		pPlrMgr->CautionInAll(CGameManager::ETeamSide::SIDE_LEFT);
	}

	// 警戒(R)
	if (ImGui::Button("(Caution: R)"))
	{
		CPlayerManager* pPlrMgr = CPlayerManager::GetInstance();
		pPlrMgr->CautionInAll(CGameManager::ETeamSide::SIDE_RIGHT);
	}

#endif
}
