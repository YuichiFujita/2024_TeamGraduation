//==========================================================================
// 
//  プレイヤーマネージャー処理 [playerManager_game.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "playerManager_game.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerManager_Game::CPlayerManager_Game() : CPlayerManager()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerManager_Game::~CPlayerManager_Game()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerManager_Game::Init()
{
	// 初期化処理
	CPlayerManager::Init();

	//----------------------------------------------------------------------
	// プレイヤー外野生成
	//----------------------------------------------------------------------
	int nHalfMax = EOutPos::OUT_MAX / 2;	// チームごとの外野総数

	// プレイヤー外野生成 (右サイド)
#if 1
	for (int i = 0; i < nHalfMax; i++)
	{ // チームごとの外野人数分繰り返す

		// 左チームの外野プレイヤー生成
		CPlayer* pOutLeft = CPlayer::Create
		(
			VEC3_ZERO,						// 位置
			CGameManager::SIDE_RIGHT,		// チームサイド
			CPlayer::EFieldArea::FIELD_OUT,	// ポジション
			CPlayer::EBaseType::TYPE_AI		// ベースタイプ
		);
		if (pOutLeft == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}
	}
#endif

	// プレイヤー外野生成 (左サイド)
#if 1
	for (int i = 0; i < nHalfMax; i++)
	{ // チームごとの外野人数分繰り返す

		// 左チームの外野プレイヤー生成
		CPlayer* pOutLeft = CPlayer::Create
		(
			VEC3_ZERO,						// 位置
			CGameManager::SIDE_LEFT,		// チームサイド
			CPlayer::EFieldArea::FIELD_OUT,	// ポジション
			CPlayer::EBaseType::TYPE_AI		// ベースタイプ
		);
		if (pOutLeft == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}
	}
#endif

	// 審判生成
	CPlayer::Create
	(
		MyLib::Vector3(0.0f, 0.0f, CGameManager::GetInstance()->GetCourtSize().z),			// 位置
		CGameManager::ETeamSide::SIDE_NONE,			// チームサイド
		CPlayer::EHuman::HUMAN_REFEREE,				// 人
		CPlayer::EBody::BODY_NORMAL,				// 体型
		CPlayer::EHandedness::HAND_R				// 利き手
	);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerManager_Game::Uninit()
{
	// 自身のインスタンスの破棄
	SAFE_DELETE(m_pInstance);
}
