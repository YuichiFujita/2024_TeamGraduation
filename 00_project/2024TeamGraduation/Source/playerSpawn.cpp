//==========================================================================
// 
//  登場演出用プレイヤー処理 [playerSpawn.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerSpawn.h"
#include "manager.h"
#include "playerSpawnManager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerSpawn::CPlayerSpawn(const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase, int nPriority) :
	CPlayer(typeTeam, typeArea, typeBase, nPriority)
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerSpawn::~CPlayerSpawn()
{
	
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerSpawn::Init()
{
	// 親クラスの初期化
	HRESULT hr = CPlayer::Init();
	if (FAILED(hr)) { return E_FAIL; }

	// プレイヤー登場演出マネージャーに割当
	CPlayerSpawnManager* pManager = CPlayerSpawnManager::GetInstance();	// プレイヤー登場演出マネージャー
	if (pManager != nullptr) { pManager->RegistPlayer(this); }			// マネージャーがある場合登録

	// モーション設定
	GetMotion()->Set(EMotion::MOTION_WALK_SPAWN);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerSpawn::Uninit()
{
	// 親クラスの終了
	CPlayer::Uninit();

	// プレイヤー登場演出マネージャーから削除
	CPlayerSpawnManager* pManager = CPlayerSpawnManager::GetInstance();	// プレイヤー登場演出マネージャー
	if (pManager != nullptr) { pManager->DeletePlayer(this); }			// マネージャーがある場合削除
}

//==========================================================================
// 削除
//==========================================================================
void CPlayerSpawn::Kill()
{
	// 親クラスの終了
	CPlayer::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerSpawn::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 過去の位置保存
	SetOldPosition(GetPosition());

	// 親クラスの更新処理
	CObjectChara::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayerSpawn::Draw()
{
	// 親クラスの描画
	CPlayer::Draw();
}
