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
	const float INIT_IN_X(900.0f);	// 初期位置(内野)
	const float INIT_IN_Z(-400.0f);	// 初期位置(内野)
	const float INIT_OUT_X(900.0f);	// 初期位置(外野)
	const float INIT_OUT_Z(100.0f);	// 初期位置(外野)
	const float SHIFT(50.0f);	// ずらし値

	const float RANGE = D3DX_PI * 0.6f;	// 生成範囲
	const float RANGE_OUT = D3DX_PI * 1.0f;	// 生成範囲
	const float DISTANCE = 400.0f;	// 間隔

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
	CListManager<CPlayerResult> sampleList = CPlayerResult::GetList();
	std::list<CPlayerResult*>::iterator itr = sampleList.GetEnd();
	CPlayerResult* pObj = nullptr;

	// 左右のリスト
	std::vector<CPlayer*> leftList, rightList;
	std::vector<CPlayer*> outLeftList, outRightList;

	while (sampleList.ListLoop(itr))
	{
		pObj = (*itr);
		// pObjか(*itr)を使って処理
	
		// チーム取得
		CGameManager::ETeamSide team = pObj->GetTeam();

		// カウンター
		switch (team)
		{
		case CGameManager::ETeamSide::SIDE_LEFT:

			// 内野
			if (pObj->GetAreaType() == CPlayer::EFieldArea::FIELD_IN)
			{
				// リスト追加
				leftList.push_back(pObj);
			}
			else
			{
				// リスト追加
				outLeftList.push_back(pObj);
			}
			break;
		
		case CGameManager::ETeamSide::SIDE_RIGHT:

			// 内野
			if (pObj->GetAreaType() == CPlayer::EFieldArea::FIELD_IN)
			{
				// リスト追加
				rightList.push_back(pObj);
			}
			else
			{
				// リスト追加
				outRightList.push_back(pObj);
			}
			break;
	
		default:
			MyAssert::CustomAssert(false, "playerManager_result: どっちチームだよ");
			break;
		}

		pObj->SetPosition(pos);
	}




	// プレイヤー数
	float division = Player::RANGE / leftList.size();	// 1分割あたりの向き
	float fRot = Player::RANGE / (float)(leftList.size() + 1);

	MyLib::Vector3 setpos, setrot;

	// 左
	for (int i = 0; i < leftList.size(); i++)
	{
		// 位置計算
		setpos = Player::POS_IN[CGameManager::ETeamSide::SIDE_LEFT];
		setpos.x += sinf(fRot * (i + 1) - Player::RANGE * 0.5f) * Player::DISTANCE;
		setpos.z += Player::DISTANCE * 0.25f + cosf(fRot * (i + 1) - Player::RANGE * 0.5f) * Player::DISTANCE;

		// 位置設定
		leftList[i]->SetPosition(setpos);

		// 向き設定
		setrot.y = setpos.AngleXZ(Player::POS_IN[CGameManager::ETeamSide::SIDE_LEFT]);
		leftList[i]->SetRotation(setrot);
	}

	// 左外野
	division = Player::RANGE_OUT / outLeftList.size();	// 1分割あたりの向き
	fRot = Player::RANGE_OUT / (float)(outLeftList.size() + 1);
	for (int i = 0; i < outLeftList.size(); i++)
	{
		// 位置計算
		setpos = Player::POS_OUT[CGameManager::ETeamSide::SIDE_LEFT];
		setpos.x += sinf(fRot * (i + 1) - Player::RANGE_OUT * 0.5f) * Player::DISTANCE;
		setpos.z += Player::DISTANCE * 0.25f + cosf(fRot * (i + 1) - Player::RANGE_OUT * 0.5f) * Player::DISTANCE;

		// カーブを緩やかにする
		setpos.z -= cosf(fRot * (i + 1) - Player::RANGE_OUT * 0.5f) * Player::DISTANCE * 0.75f;

		// 位置設定
		outLeftList[i]->SetPosition(setpos);

		// 向き設定
		setrot.y = setpos.AngleXZ(Player::POS_IN[CGameManager::ETeamSide::SIDE_LEFT]);
		outLeftList[i]->SetRotation(setrot);
	}

	// 右
	division = Player::RANGE / rightList.size();	// 1分割あたりの向き
	fRot = Player::RANGE / (float)(rightList.size() + 1);
	for (int i = 0; i < rightList.size(); i++)
	{
		// 位置計算
		setpos = Player::POS_IN[CGameManager::ETeamSide::SIDE_RIGHT];
		setpos.x += sinf(fRot * (i + 1) - Player::RANGE * 0.5f) * Player::DISTANCE;
		setpos.z += Player::DISTANCE * 0.25f + cosf(fRot * (i + 1) - Player::RANGE * 0.5f) * Player::DISTANCE;

		// 位置設定
		rightList[i]->SetPosition(setpos);

		// 向き設定
		setrot.y = setpos.AngleXZ(Player::POS_IN[CGameManager::ETeamSide::SIDE_RIGHT]);
		rightList[i]->SetRotation(setrot);
	}

	// 右外野
	division = Player::RANGE_OUT / outRightList.size();	// 1分割あたりの向き
	fRot = Player::RANGE_OUT / (float)(outRightList.size() + 1);
	for (int i = 0; i < outRightList.size(); i++)
	{
		// 位置計算
		setpos = Player::POS_OUT[CGameManager::ETeamSide::SIDE_RIGHT];
		setpos.x += sinf(fRot * (i + 1) - Player::RANGE_OUT * 0.5f) * Player::DISTANCE;
		setpos.z += Player::DISTANCE * 0.25f + cosf(fRot * (i + 1) - Player::RANGE_OUT * 0.5f) * Player::DISTANCE;

		// カーブを緩やかにする
		setpos.z -= cosf(fRot * (i + 1) - Player::RANGE_OUT * 0.5f) * Player::DISTANCE * 0.75f;

		// 位置設定
		outRightList[i]->SetPosition(setpos);

		// 向き設定
		setrot.y = setpos.AngleXZ(Player::POS_IN[CGameManager::ETeamSide::SIDE_RIGHT]);
		outRightList[i]->SetRotation(setrot);
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
