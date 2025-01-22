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
		MyLib::Vector3(0.0f, 0.0f, CGameManager::GetInstance()->GetCourtSize().z),	// 位置
		CGameManager::ETeamSide::SIDE_NONE,	// チームサイド
		CPlayer::EHuman::HUMAN_REFEREE,		// 人
		CPlayer::EBody::BODY_NORMAL,		// 体型
		CPlayer::EHandedness::HAND_R		// 利き手
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
	//----------------------------------------------------------------------
	// プレイヤー内野生成
	//----------------------------------------------------------------------
	std::vector<LoadInfo> vecInLeftInfo		= GetLoadInInfo(CGameManager::ETeamSide::SIDE_LEFT);	// 左内野情報
	std::vector<LoadInfo> vecInRightInfo	= GetLoadInInfo(CGameManager::ETeamSide::SIDE_RIGHT);	// 右内野情報

	// 読み込んだ情報をもとにプレイヤー生成
	int nInMaxLeft = static_cast<int>(vecInLeftInfo.size());	// 左チーム人数
	int nInMaxRight = static_cast<int>(vecInRightInfo.size());	// 右チーム人数
	int nCntLeft = 0, nCntRight = 0;	// 人数カウント
	for (int j = 0; j < CGameManager::MAX_SIDEPLAYER; j++)
	{
		if (j < nInMaxLeft)
		{
			MyLib::Vector3 pos = player::left::POS;	// 生成位置

			// Z座標を動かす
			pos.z = player::left::POS.z - (player::left::OFFSET_Z * (float)(nInMaxLeft - 1)) * 0.5f + (player::left::OFFSET_Z * (float)nCntLeft);

			// 左チームプレイヤー生成
			if (FAILED(CreateLeftPlayer(nCntLeft, vecInLeftInfo[j], pos)))
			{ // 生成に失敗した場合

				return E_FAIL;
			}

			// 左人数加算
			nCntLeft++;
		}

		if (j < nInMaxRight)
		{
			MyLib::Vector3 pos = player::right::POS;	// 生成位置

			// Z座標を動かす
			pos.z = player::right::POS.z - (player::right::OFFSET_Z * (float)(nInMaxRight - 1)) * 0.5f + (player::right::OFFSET_Z * (float)nCntRight);

			// 右チームプレイヤー生成
			if (FAILED(CreateRightPlayer(nCntRight, vecInRightInfo[j], pos)))
			{ // 生成に失敗した場合

				return E_FAIL;
			}

			// 右人数加算
			nCntRight++;
		}
	}

	//----------------------------------------------------------------------
	// プレイヤー外野生成
	//----------------------------------------------------------------------
	std::vector<LoadInfo> vecOutLeftInfo	= GetLoadOutInfo(CGameManager::ETeamSide::SIDE_LEFT);	// 左外野情報
	std::vector<LoadInfo> vecOutRightInfo	= GetLoadOutInfo(CGameManager::ETeamSide::SIDE_RIGHT);	// 右外野情報

	// プレイヤー外野生成 (左サイド)
	for (const auto& rOutLeft : vecOutLeftInfo)
	{ // 左外野人数分繰り返す

		// 左チーム外野プレイヤー生成
		if (FAILED(CreateOutPlayer(CGameManager::ETeamSide::SIDE_LEFT, rOutLeft)))
		{ // 生成に失敗した場合

			return E_FAIL;
		}
	}

	// プレイヤー外野生成 (右サイド)
	for (const auto& rOutRight : vecOutRightInfo)
	{ // 右外野人数分繰り返す

		// 右チーム外野プレイヤー生成
		if (FAILED(CreateOutPlayer(CGameManager::ETeamSide::SIDE_RIGHT, rOutRight)))
		{ // 生成に失敗した場合

			return E_FAIL;
		}
	}

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

//==========================================================================
// 外野プレイヤー生成
//==========================================================================
HRESULT CPlayerManager_Game::CreateOutPlayer(CGameManager::ETeamSide team, const LoadInfo& info)
{
	// プレイヤー生成
	CPlayer* pPlayer = CPlayer::Create
	(
		MyLib::Vector3(), 				// 位置
		team,							// チームサイド
		CPlayer::EFieldArea::FIELD_OUT,	// ポジション
		CPlayer::EBaseType::TYPE_AI,	// ベースタイプ
		info.eBody,						// 体系
		info.eHanded					// 利き手
	);
	if (pPlayer == nullptr)
	{
		return E_FAIL;
	}

	// インデックス反映
	pPlayer->SetMyPlayerIdx(info.nControllIdx);

	// ドレスアップ反映
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);

	return S_OK;
}
