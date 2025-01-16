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
	namespace player
	{
		namespace left
		{
			const MyLib::Vector3 POS = MyLib::Vector3(-300.0f, 0.0f, 0.0f);	// 左中心位置
			const float OFFSET_Z = 300.0f;	// Z座標オフセット
		}

		namespace right
		{
			const MyLib::Vector3 POS = MyLib::Vector3(300.0f, 0.0f, 0.0f);	// 右中心位置
			const float OFFSET_Z = 300.0f;	// Z座標オフセット
		}
	}
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

	// 審判生成
	CPlayer::Create
	(
		MyLib::Vector3(0.0f, 0.0f, CGameManager::GetInstance()->GetCourtSize().z),			// 位置
		CGameManager::ETeamSide::SIDE_NONE,			// チームサイド
		CPlayer::EHuman::HUMAN_REFEREE,				// 人
		CPlayer::EBody::BODY_NORMAL,				// 体型
		CPlayer::EHandedness::HAND_R				// 利き手
	);

#if 0
	for (int i = 0; i < 10; i++)
	{
		CPlayer* pAI = CPlayer::Create(MyLib::Vector3(200.0f, 0.0f, 0.0f), CGameManager::SIDE_RIGHT, CPlayer::EFieldArea::FIELD_IN, CPlayer::EBaseType::TYPE_AI);
		if (pAI == nullptr)
		{
			return E_FAIL;
		}
		pAI->SetRotation(MyLib::Vector3(0.0f, HALF_PI, 0.0f));
		pAI->SetRotDest(HALF_PI);
	}
#endif
	return S_OK;
}

//==========================================================================
// プレイヤー生成
//==========================================================================
HRESULT CPlayerManager_Game::CreatePlayer()
{
	// 読み込み情報
	std::vector<LoadInfo> leftLoadInfo = CPlayerManager::GetLoadInfo(CGameManager::ETeamSide::SIDE_LEFT);
	std::vector<LoadInfo> rightLoadInfo = CPlayerManager::GetLoadInfo(CGameManager::ETeamSide::SIDE_RIGHT);

	// 読み込んだ情報をもとにプレイヤー生成
	int nMaxLeft = static_cast<int>(leftLoadInfo.size());		// 左チーム人数
	int nMaxRight = static_cast<int>(rightLoadInfo.size());	// 右チーム人数
	int nCntLeft = 0, nCntRight = 0;	// 人数カウント
	for (int j = 0; j < CGameManager::MAX_SIDEPLAYER; j++)
	{
		// 左のプレイヤー生成
		if (j < nMaxLeft)
		{
			MyLib::Vector3 pos = player::left::POS;	// 生成位置

			// Z座標を動かす
			pos.z = player::left::POS.z - (player::left::OFFSET_Z * (float)(nMaxLeft - 1)) * 0.5f + (player::left::OFFSET_Z * (float)nCntLeft);

			// 左チームプレイヤー生成
			CreateLeftPlayer(nCntLeft, leftLoadInfo[j], pos);

			// 左人数加算
			nCntLeft++;
		}

		// 右のプレイヤー生成
		if (j < nMaxRight)
		{
			MyLib::Vector3 pos = player::right::POS;	// 生成位置

			// Z座標を動かす
			pos.z = player::right::POS.z - (player::right::OFFSET_Z * (float)(nMaxRight - 1)) * 0.5f + (player::right::OFFSET_Z * (float)nCntRight);

			// 右チームプレイヤー生成
			CreateRightPlayer(nCntRight, rightLoadInfo[j], pos);

			// 右人数加算
			nCntRight++;
		}
	}

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
