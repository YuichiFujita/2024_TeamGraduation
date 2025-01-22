//==========================================================================
// 
//  プレイヤーマネージャー処理 [playerManager_spawn.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "playerManager_spawn.h"
#include "resultmanager.h"
#include "playerResult.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float	ZLINE_OFFSET = 200.0f;	// Z軸オフセット
	const MyLib::Vector3 POS_LEFT = MyLib::Vector3(-972.85f, 0.0f, 1817.35f);	// 左先頭プレイヤー初期位置
	const MyLib::Vector3 POS_RIGHT = MyLib::Vector3(972.85f, 0.0f, 1817.35f);	// 右先頭プレイヤー初期位置
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerManager_Spawn::CPlayerManager_Spawn() : CPlayerManager()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerManager_Spawn::~CPlayerManager_Spawn()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerManager_Spawn::Init()
{
	// 親クラスの初期化処理
	CPlayerManager::Init();

	return S_OK;
}

//==========================================================================
// プレイヤー生成
//==========================================================================
HRESULT CPlayerManager_Spawn::CreatePlayer()
{
	std::vector<LoadInfo> vecInLeftInfo		= GetLoadInInfo(CGameManager::ETeamSide::SIDE_LEFT);	// 左内野情報
	std::vector<LoadInfo> vecInRightInfo	= GetLoadInInfo(CGameManager::ETeamSide::SIDE_RIGHT);	// 右内野情報
	std::vector<LoadInfo> vecOutLeftInfo	= GetLoadOutInfo(CGameManager::ETeamSide::SIDE_LEFT);	// 左外野情報
	std::vector<LoadInfo> vecOutRightInfo	= GetLoadOutInfo(CGameManager::ETeamSide::SIDE_RIGHT);	// 右外野情報

	// 読み込んだ情報をもとにプレイヤー生成
	int nInMaxLeft = static_cast<int>(vecInLeftInfo.size());	// 左チーム人数
	int nInMaxRight = static_cast<int>(vecInRightInfo.size());	// 右チーム人数
	int nCntLeft = 0, nCntRight = 0;	// 人数カウント
	for (int j = 0; j < CGameManager::MAX_SIDEPLAYER; j++)
	{
		if (j < nInMaxLeft)
		{
			// 左チームプレイヤー生成
			if (FAILED(CreateLeftPlayer(nCntLeft, vecInLeftInfo[j])))
			{ // 生成に失敗した場合

				return E_FAIL;
			}

			// 左人数加算
			nCntLeft++;
		}

		if (j < nInMaxRight)
		{
			// 右チームプレイヤー生成
			if (FAILED(CreateRightPlayer(nCntRight, vecInRightInfo[j])))
			{ // 生成に失敗した場合

				return E_FAIL;
			}

			// 右人数加算
			nCntRight++;
		}
	}

	for (const auto& rOutLeft : vecOutLeftInfo)
	{ // 左外野人数分繰り返す

		// 左チームプレイヤー生成
		if (FAILED(CreateLeftPlayer(nCntLeft, rOutLeft)))
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// 左人数加算
		nCntLeft++;
	}

	for (const auto& rOutRight : vecOutRightInfo)
	{ // 右外野人数分繰り返す

		// 右チームプレイヤー生成
		if (FAILED(CreateRightPlayer(nCntRight, rOutRight)))
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// 右人数加算
		nCntRight++;
	}

	return S_OK;
}

//==========================================================================
// 左のプレイヤー生成
//==========================================================================
HRESULT CPlayerManager_Spawn::CreateLeftPlayer(int i, const LoadInfo& info)
{
	// プレイヤー生成
	MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, ZLINE_OFFSET * (float)i);
	CPlayer* pPlayer = CPlayer::Create
	(
		POS_LEFT + offset, 				// 位置
		CGameManager::SIDE_LEFT,		// チームサイド
		CPlayer::EHuman::HUMAN_SPAWN,	// 人
		info.eBody,		// 体系
		info.eHanded	// 利き手
	);
	if (pPlayer == nullptr)
	{
		return E_FAIL;
	}

	// インデックス反映
	pPlayer->SetMyPlayerIdx(info.nControllIdx);

	// ドレスアップ反映
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);

	// 先頭だけドア開け
	if (i == 0)
	{
		pPlayer->SetMotion(CPlayer::EMotion::MOTION_DOOR);
	}

	return S_OK;
}

//==========================================================================
// 右のプレイヤー生成
//==========================================================================
HRESULT CPlayerManager_Spawn::CreateRightPlayer(int i, const LoadInfo& info)
{
	// プレイヤー生成
	MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, ZLINE_OFFSET * (float)i);
	CPlayer* pPlayer = CPlayer::Create
	(
		POS_RIGHT + offset, 			// 位置
		CGameManager::SIDE_RIGHT,		// チームサイド
		CPlayer::EHuman::HUMAN_SPAWN,	// 人
		info.eBody,		// 体系
		info.eHanded	// 利き手
	);
	if (pPlayer == nullptr)
	{
		return E_FAIL;
	}

	// インデックス反映
	pPlayer->SetMyPlayerIdx(info.nControllIdx);

	// ドレスアップ反映
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);

	// 先頭だけドア開け
	if (i == 0)
	{
		pPlayer->SetMotion(CPlayer::EMotion::MOTION_DOOR);
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerManager_Spawn::Uninit()
{
	// 自身のインスタンスの破棄
	SAFE_DELETE(m_pInstance);
}
