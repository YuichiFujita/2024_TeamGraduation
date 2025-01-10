//==========================================================================
// 
//  プレイヤーマネージャー処理 [playerManager_result.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "playerManager_result.h"
#include "resultmanager.h"
#include "playerResult.h"

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
	// 親クラスの初期化処理
	CPlayerManager::Init();

	//プレイヤー初期化
	InitPlayer();

	return S_OK;
}

//==========================================================================
// 左のプレイヤー生成
//==========================================================================
HRESULT CPlayerManager_Result::CreateLeftPlayer(int i, const LoadInfo& info)
{
	// 位置
	MyLib::Vector3 posCreate = CResultManager::GetInstance()->GetPosMid(CGameManager::ETeamSide::SIDE_LEFT);
	posCreate += Player::POS_IN[CGameManager::ETeamSide::SIDE_LEFT];
	posCreate += (Player::POS_SHIFT[CGameManager::ETeamSide::SIDE_LEFT] * static_cast<float>(i));

	// プレイヤー生成
	CPlayer* pPlayer = CPlayer::Create(
		posCreate,
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
	MyLib::Vector3 posCreate = CResultManager::GetInstance()->GetPosMid(CGameManager::ETeamSide::SIDE_RIGHT);
	posCreate += Player::POS_IN[CGameManager::ETeamSide::SIDE_RIGHT];
	posCreate += (Player::POS_SHIFT[CGameManager::ETeamSide::SIDE_RIGHT] * static_cast<float>(i));

	// プレイヤー生成
	CPlayer* pPlayer = CPlayer::Create(
		posCreate,
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
// プレイヤー初期化
//==========================================================================
void CPlayerManager_Result::InitPlayer()
{
	//----------------------------------------------------------------------
	// プレイヤー外野生成
	//----------------------------------------------------------------------
	int nHalfMax = EOutPos::OUT_MAX / 2;	// チームごとの外野総数

	// プレイヤー外野生成 (右サイド)
#if 1
	for (int i = 0; i < nHalfMax; i++)
	{ // チームごとの外野人数分繰り返す

		// 右チームの外野プレイヤー生成
		CPlayer* pOutRight= CPlayer::Create
		(
			VEC3_ZERO,
			CGameManager::ETeamSide::SIDE_RIGHT,
			CPlayer::EHuman::HUMAN_RESULT,
			CPlayer::BODY_NORMAL,
			CPlayer::HAND_R,
			CPlayer::EFieldArea::FIELD_OUT
		);
		if (pOutRight == nullptr)
		{ // 生成に失敗した場合

			assert(false);
		}
	}

	// プレイヤー外野生成 (左サイド)
	for (int i = 0; i < nHalfMax; i++)
	{ // チームごとの外野人数分繰り返す

		// 左チームの外野プレイヤー生成
		CPlayer* pOutLeft = CPlayer::Create
		(
			VEC3_ZERO,
			CGameManager::ETeamSide::SIDE_LEFT,
			CPlayer::EHuman::HUMAN_RESULT,
			CPlayer::BODY_NORMAL,
			CPlayer::HAND_R,
			CPlayer::EFieldArea::FIELD_OUT
		);
		if (pOutLeft == nullptr)
		{ // 生成に失敗した場合

			assert(false);
		}
	}
#endif

	// 位置
	MyLib::Vector3 pos = VEC3_ZERO;

	// 位置設定
	int nNumRight = 0, nNumLeft = 0;
	CListManager<CPlayerResult> sampleList = CPlayerResult::GetList();
	std::list<CPlayerResult*>::iterator itr = sampleList.GetEnd();
	CPlayerResult* pObj = nullptr;

	while (sampleList.ListLoop(itr))
	{
		pObj = (*itr);
		// pObjか(*itr)を使って処理
	
		// 内野排除
		if (pObj->GetAreaType() == CPlayer::EFieldArea::FIELD_IN) continue;

		CGameManager::ETeamSide team = pObj->GetTeam();
		pos = CResultManager::GetInstance()->GetPosMid(team);
		pos += Player::POS_OUT[team];

		// カウンター
		switch (team)
		{
		case CGameManager::ETeamSide::SIDE_LEFT:

			pos += (Player::POS_SHIFT[team] * static_cast<float>(nNumLeft));
			nNumLeft++;
			break;
		
		case CGameManager::ETeamSide::SIDE_RIGHT:

			pos += (Player::POS_SHIFT[team] * static_cast<float>(nNumRight));
			nNumRight++;
			break;
	
		default:
			MyAssert::CustomAssert(false, "playerManager_result: どっちチームだよ");
			break;
		}

		pObj->SetPosition(pos);
	}
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerManager_Result::Uninit()
{
	// 自身のインスタンスの破棄
	SAFE_DELETE(m_pInstance);
}
