//==========================================================================
// 
//  プレイヤーマネージャー処理 [playerManager_result.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "playerManager_result.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

// プレイヤー
namespace Player
{
	const float INIT_IN_X(100.0f);	// 初期位置(内野)
	const float INIT_IN_Z(100.0f);	// 初期位置(内野)
	const float INIT_OUT_X(100.0f);	// 初期位置(外野)
	const float INIT_OUT_Z(300.0f);	// 初期位置(外野)
	const float SHIFT(50.0f);	// ずらし値

	const MyLib::Vector3 POS_IN[CGameManager::ETeamSide::SIDE_MAX] =
	{// 内野位置
		MyLib::Vector3(-INIT_IN_X, 0.0f, +INIT_IN_Z),
		MyLib::Vector3(+INIT_IN_X, 0.0f, +INIT_IN_Z),
	};

	const MyLib::Vector3 POS_OUT[CGameManager::ETeamSide::SIDE_MAX] =
	{// 外野位置
		MyLib::Vector3(-INIT_OUT_X, 0.0f, +INIT_OUT_Z),
		MyLib::Vector3(+INIT_OUT_X, 0.0f, +INIT_OUT_Z),
	};

	const MyLib::Vector3 POS_SHIFT[CGameManager::ETeamSide::SIDE_MAX] =
	{// ずらし値
		MyLib::Vector3(-SHIFT, 0.0f, -SHIFT),
		MyLib::Vector3(-SHIFT, 0.0f, -SHIFT),
	};
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerManager_Result::CPlayerManager_Result() : CPlayerManager()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerManager_Result::~CPlayerManager_Result()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerManager_Result::Init()
{
#ifdef ENTRYSTART
	// 初期化処理
	CPlayerManager::Init();
#else

#endif

	return S_OK;
}

//==========================================================================
// 左のプレイヤー生成
//==========================================================================
HRESULT CPlayerManager_Result::CreateLeftPlayer(int i, const LoadInfo& info)
{
	// 位置
	MyLib::Vector3 pos = Player::POS_OUT[CGameManager::ETeamSide::SIDE_LEFT];
	pos += (Player::POS_SHIFT[CGameManager::ETeamSide::SIDE_LEFT] * i);

	// プレイヤー生成
	CPlayer* pPlayer = CPlayer::Create(
		pos,
		CGameManager::ETeamSide::SIDE_LEFT,
		CPlayer::EHuman::HUMAN_RESULT,
		info.eBody,
		info.eHanded);

	// インデックス反映
	pPlayer->SetMyPlayerIdx(info.nControllIdx);

	// ドレスアップ反映
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);

	return S_OK;
}

//==========================================================================
// 右のプレイヤー生成
//==========================================================================
HRESULT CPlayerManager_Result::CreateRightPlayer(int i, const LoadInfo& info)
{
	// 位置
	MyLib::Vector3 pos = Player::POS_OUT[CGameManager::ETeamSide::SIDE_RIGHT];
	pos += (Player::POS_SHIFT[CGameManager::ETeamSide::SIDE_RIGHT] * i);

	// プレイヤー生成
	CPlayer* pPlayer = CPlayer::Create(
		pos,
		CGameManager::ETeamSide::SIDE_RIGHT,
		CPlayer::EHuman::HUMAN_RESULT,
		info.eBody,
		info.eHanded);

	// インデックス反映
	pPlayer->SetMyPlayerIdx(info.nControllIdx);

	// ドレスアップ反映
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerManager_Result::Uninit()
{
	// 自身のインスタンスの破棄
	SAFE_DELETE(m_pInstance);
}
