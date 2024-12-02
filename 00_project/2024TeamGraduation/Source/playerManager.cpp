//==========================================================================
// 
//  プレイヤーマネージャー処理 [playerManager.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerManager.h"
#include "playerUserOut.h"
#include "bindKeyFront.h"
#include "bindKeyBack.h"
#include "bindKeyLeft.h"
#include "bindKeyRight.h"

// 派生クラス
#include "playerManager_game.h"
#include "playerManager_result.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TOP_LINE = "#==============================================================================";	// テキストのライン
	const std::string TEXT_LINE = "#------------------------------------------------------------------------------";	// テキストのライン
}

//==========================================================================
// 関数ポインタ
//==========================================================================
// 外野情報関数
CPlayerManager::INFO_FUNC CPlayerManager::m_InfoFuncList[] =
{
	&CPlayerManager::GetInfoLeftFar,	// 左奥の外野情報
	&CPlayerManager::GetInfoLeft,		// 左の外野情報
	&CPlayerManager::GetInfoLeftNear,	// 左手前の外野情報
	&CPlayerManager::GetInfoRightFar,	// 右奥の外野情報
	&CPlayerManager::GetInfoRight,		// 右の外野情報
	&CPlayerManager::GetInfoRightNear,	// 右手前の外野情報
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CPlayerManager* CPlayerManager::m_pInstance = nullptr;	// 自身のインスタンス
std::vector<CPlayerManager::LoadInfo> CPlayerManager::m_vecLoadInfo[CGameManager::ETeamSide::SIDE_MAX] = {};	// 読み込み情報

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerManager::CPlayerManager() : m_bUserChange(false)	// ユーザー変更操作フラグ
{
	// メンバ変数をクリア
	memset(&m_apOut[0], 0, sizeof(m_apOut));	// 外野プレイヤー

	for (int i= 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		m_vecLoadInfo[i].clear();
	}
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerManager::~CPlayerManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CPlayerManager* CPlayerManager::Create(CScene::MODE mode)
{
	// 既に生成済みの場合抜ける
	if (m_pInstance != nullptr) { return m_pInstance; }

	// インスタンスの生成
	switch (mode)
	{
	case CScene::MODE_GAME:
		m_pInstance = DEBUG_NEW CPlayerManager_Game;
		break;

	case CScene::MODE_RESULT:
		m_pInstance = DEBUG_NEW CPlayerManager_Result;
		break;

	default:
		m_pInstance = DEBUG_NEW CPlayerManager;
		break;
	}

	if (m_pInstance != nullptr)
	{ // メモリの確保が出来た場合

		// インスタンスの初期化
		if (FAILED(m_pInstance->Init()))
		{ // 初期化に失敗した場合

			return nullptr;
		}
	}

	return m_pInstance;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerManager::Init()
{
#ifdef ENTRYSTART

	// FUJITA：エントリーで外部に保存されたプレイヤー情報を取得しプレイヤーを生成
	// 読み込み処理
	Load();

	// 読み込んだ情報をもとにプレイヤー生成
	int nLeft = 0, nRight = 0;
	for (int j = 0; j < CGameManager::MAX_SIDEPLAYER; j++)
	{
		// 左のプレイヤー生成
		if (j < static_cast<int>(m_vecLoadInfo[CGameManager::ETeamSide::SIDE_LEFT].size()))
		{
			CreateLeftPlayer(nLeft, m_vecLoadInfo[CGameManager::ETeamSide::SIDE_LEFT][j]);
			nLeft++;
		}

		// 右のプレイヤー生成
		if (j < static_cast<int>(m_vecLoadInfo[CGameManager::ETeamSide::SIDE_RIGHT].size()))
		{
			CreateRightPlayer(nRight, m_vecLoadInfo[CGameManager::ETeamSide::SIDE_RIGHT][j]);
			nRight++;
		}
	}

#else

	//----------------------------------------------------------------------
	// プレイヤー内野生成
	//----------------------------------------------------------------------
	// プレイヤーUser四人生成(左右)
#if 0
	// プレイヤーUser二人生成(右)
	for (int i = 0; i < 2; i++)
	{
		MyLib::Vector3 pos = MyLib::Vector3(200.0f, 0.0f, -100.0f);
		MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, 200.0f * (float)i);
		CPlayer* pUser = CPlayer::Create
		(
			pos + offset, 					// 位置
			CGameManager::SIDE_RIGHT,		// チームサイド
			CPlayer::EFieldArea::FIELD_IN,	// ポジション
			(CPlayer::EBaseType)i,			// ベースタイプ
			CPlayer::EBody::BODY_DEBU,		// 体系
			CPlayer::EHandedness::HAND_R	// 利き手
		);
		if (pUser == nullptr)
		{
			return E_FAIL;
		}
		pUser->SetRotation(MyLib::Vector3(0.0f, HALF_PI, 0.0f));
		pUser->SetRotDest(HALF_PI);
	}

	// プレイヤーUser二人生成(左)
	for (int i = 0; i < 2; i++)
	{
		MyLib::Vector3 pos = MyLib::Vector3(-200.0f, 0.0f, -100.0f);
		MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, 200.0f * (float)i);
		CPlayer* pUser = CPlayer::Create
		(
			pos + offset, 					// 位置
			CGameManager::SIDE_LEFT,		// チームサイド
			CPlayer::EFieldArea::FIELD_IN,	// ポジション
			(CPlayer::EBaseType)i,			// ベースタイプ
			CPlayer::EBody::BODY_GARI,		// 体系
			CPlayer::EHandedness::HAND_L	// 利き手
		);
		if (pUser == nullptr)
		{
			return E_FAIL;
		}
		pUser->SetRotation(MyLib::Vector3(0.0f, -HALF_PI, 0.0f));
		pUser->SetRotDest(-HALF_PI);
	}
#endif

	// プレイヤーUser生成(左)
#if 1
	CPlayer* pUser = CPlayer::Create(MyLib::Vector3(-200.0f, 0.0f, 0.0f), CGameManager::SIDE_LEFT, CPlayer::EFieldArea::FIELD_IN, CPlayer::EBaseType::TYPE_USER);
	if (pUser == nullptr)
	{
		return E_FAIL;
	}
	pUser->SetRotation(MyLib::Vector3(0.0f, -HALF_PI, 0.0f));
	pUser->SetRotDest(-HALF_PI);
#endif

	// プレイヤーUser二世生成(右)
#if 0
	CPlayer* pUser2 = CPlayer::Create(MyLib::Vector3(200.0f, 0.0f, 0.0f), CGameManager::SIDE_RIGHT, CPlayer::EFieldArea::FIELD_IN, CPlayer::EBaseType::TYPE_USER);
	if (pUser2 == nullptr)
	{
		return E_FAIL;
	}
	pUser2->SetRotation(MyLib::Vector3(0.0f, HALF_PI, 0.0f));
	pUser2->SetRotDest(HALF_PI);
#endif

	// プレイヤーAI一人生成(右)
#if 1
	CPlayer* pAI = CPlayer::Create(MyLib::Vector3(200.0f, 0.0f, 0.0f), CGameManager::SIDE_RIGHT, CPlayer::EFieldArea::FIELD_IN, CPlayer::EBaseType::TYPE_AI);
	if (pAI == nullptr)
	{
		return E_FAIL;
	}
	pAI->SetRotation(MyLib::Vector3(0.0f, HALF_PI, 0.0f));
	pAI->SetRotDest(HALF_PI);
#endif

	// プレイヤーAI一人生成(左)
#if 0
	CPlayer* pAI2 = CPlayer::Create(MyLib::Vector3(-200.0f, 0.0f, 200.0f), CGameManager::SIDE_LEFT, CPlayer::EFieldArea::FIELD_IN, CPlayer::EBaseType::TYPE_AI);
	if (pAI2 == nullptr)
	{
		return E_FAIL;
	}
	pAI2->SetRotation(MyLib::Vector3(0.0f, -HALF_PI, 0.0f));
	pAI2->SetRotDest(-HALF_PI);
#endif

	// プレイヤーAI四人生成(右)
#if 0
	for (int i = 0; i < 3; i++)
	{
		MyLib::Vector3 pos = MyLib::Vector3(200.0f, 0.0f, 0.0f) + MyLib::Vector3(0.0f, 0.0f, -150.0f);
		MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, 100.0f * (float)i);
		CPlayer* pAI = CPlayer::Create(pos + offset, CGameManager::SIDE_RIGHT, CPlayer::EFieldArea::FIELD_IN, CPlayer::EBaseType::TYPE_AI);
		if (pAI == nullptr)
		{
			return E_FAIL;
		}
		pAI->SetRotation(MyLib::Vector3(0.0f, HALF_PI, 0.0f));
		pAI->SetRotDest(HALF_PI);
	}
#endif

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

#endif // ENTRY


	return S_OK;
}

//==========================================================================
// 左のプレイヤー生成
//==========================================================================
HRESULT CPlayerManager::CreateLeftPlayer(int i, const LoadInfo& info)
{
	MyLib::Vector3 pos = MyLib::Vector3(200.0f, 0.0f, -100.0f);

	CPlayer::EBaseType baseType = (info.nControllIdx >= 0) ? CPlayer::EBaseType::TYPE_USER : CPlayer::EBaseType::TYPE_AI;	// ベースタイプ

	CPlayer* pPlayer = CPlayer::Create
	(
		pos, 								// 位置
		CGameManager::ETeamSide::SIDE_LEFT,	// チームサイド
		CPlayer::EFieldArea::FIELD_IN,		// ポジション
		baseType,							// ベースタイプ
		info.eBody,							// 体系
		info.eHanded						// 利き手
	);
	if (pPlayer == nullptr)
	{
		return E_FAIL;
	}
	pPlayer->SetRotation(MyLib::Vector3(0.0f, HALF_PI, 0.0f));
	pPlayer->SetRotDest(HALF_PI);
	
	// インデックス反映
	pPlayer->SetMyPlayerIdx(info.nControllIdx);

	// ドレスアップ反映
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);

	return S_OK;
}

//==========================================================================
// 右のプレイヤー生成
//==========================================================================
HRESULT CPlayerManager::CreateRightPlayer(int i, const LoadInfo& info)
{
	MyLib::Vector3 pos = MyLib::Vector3(200.0f, 0.0f, -100.0f);

	CPlayer::EBaseType baseType = (info.nControllIdx >= 0) ? CPlayer::EBaseType::TYPE_USER : CPlayer::EBaseType::TYPE_AI;	// ベースタイプ

	CPlayer* pPlayer = CPlayer::Create
	(
		pos, 								// 位置
		CGameManager::ETeamSide::SIDE_RIGHT,	// チームサイド
		CPlayer::EFieldArea::FIELD_IN,		// ポジション
		baseType,							// ベースタイプ
		info.eBody,							// 体系
		info.eHanded						// 利き手
	);
	if (pPlayer == nullptr)
	{
		return E_FAIL;
	}
	pPlayer->SetRotation(MyLib::Vector3(0.0f, -HALF_PI, 0.0f));
	pPlayer->SetRotDest(-HALF_PI);

	// インデックス反映
	pPlayer->SetMyPlayerIdx(info.nControllIdx);

	// ドレスアップ反映
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerManager::Uninit()
{
	// 自身のインスタンスの破棄
	SAFE_DELETE(m_pInstance);
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ユーザー変更操作を初期化
	m_bUserChange = false;
}

//==========================================================================
// プレイヤー登録処理
//==========================================================================
int CPlayerManager::RegistPlayer(CPlayer* pPlayer, const int nPosIdx)
{
	switch (pPlayer->GetAreaType())
	{ // ポジションごとの処理
	case CPlayer::EFieldArea::FIELD_IN:

		// リストマネージャーに登録
		return RegistInPlayer(pPlayer);

	case CPlayer::EFieldArea::FIELD_OUT:

		// 配列に登録
		if (nPosIdx <= -1)	{ return RegistOutPlayer(pPlayer); }			// ポジション指定がない場合空き配列に登録
		else				{ return RegistOutPlayer(pPlayer, nPosIdx); }	// ポジション指定がある場合指定配列に登録

	default:
		assert(false);
		break;
	}

	return -1;
}

//==========================================================================
// プレイヤー削除処理
//==========================================================================
void CPlayerManager::DeletePlayer(CPlayer* pPlayer)
{
	switch (pPlayer->GetAreaType())
	{ // ポジションごとの処理
	case CPlayer::EFieldArea::FIELD_IN:

		// リストマネージャーから削除
		DeleteInPlayer(pPlayer);
		break;

	case CPlayer::EFieldArea::FIELD_OUT:

		// 配列から削除
		DeleteOutPlayer(pPlayer);
		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// 内野プレイヤーリストの取得処理
//==========================================================================
CListManager<CPlayer> CPlayerManager::GetInList(const CGameManager::ETeamSide team)
{
	switch (team)
	{ // チームサイドごとの処理
	case CGameManager::ETeamSide::SIDE_LEFT:
		return m_listInLeft;

	case CGameManager::ETeamSide::SIDE_RIGHT:
		return m_listInRight;

	default:
		assert(false);
		break;
	}

	assert(false);
	return {};
}

//==========================================================================
// 外野プレイヤーの取得処理
//==========================================================================
CPlayer* CPlayerManager::GetOutPlayer(const EOutPos out)
{
	// 外野プレイヤー情報を返す
	return m_apOut[out];
}

//==========================================================================
// 外野ポジションの取得処理
//==========================================================================
CPlayerManager::EOutPos CPlayerManager::GetOutPosition(const CPlayer* pPlayer)
{
	for (int i = 0; i < EOutPos::OUT_MAX; i++)
	{ // 外野の総数分繰り返す

		// 引数プレイヤーと同じアドレスの場合そのポジションを返す
		if (m_apOut[i] == pPlayer) { return (EOutPos)i; }
	}

	return (EOutPos)-1;
}

//==========================================================================
// 外野情報の取得処理
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetOutInfo(const EOutPos out)
{
	// 外野情報の取得
	return (this->*(m_InfoFuncList[out]))();
}

//==========================================================================
// ユーザーベースのAI変更
//--------------------------------------------------------------------------
// ※ どのポジションも変更可能
//==========================================================================
void CPlayerManager::ChangeAIToUser(CPlayer* pPlayer)
{
	// 自身がユーザーの場合は抜ける
	if (pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER) { return; }

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	CPlayer* pChange = nullptr;	// 変更するユーザープレイヤー
	float fMinDis = 0.0f;		// ユーザーとの最小距離

	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pItrPlayer = (*itr);	// プレイヤー情報
		
		// 自身と別チームのプレイヤーの場合次へ
		if (pItrPlayer->GetTeam() != pPlayer->GetTeam()) { continue; }

		// ユーザーではない場合次へ
		if (pItrPlayer->GetBaseType() != CPlayer::EBaseType::TYPE_USER) { continue; }

		// ユーザーとの距離を計算
		float fCurDis = pItrPlayer->GetPosition().DistanceXZ(pPlayer->GetPosition());
		if (pChange == nullptr || fCurDis < fMinDis)
		{ // まだユーザーが設定されていない、またはユーザー間の距離がより短かった場合

			// 変更ユーザーを保存
			pChange = pItrPlayer;

			// より短いユーザー間の距離を保存
			fMinDis = fCurDis;
		}
	}

	if (pChange != nullptr)
	{ // 変更可能なユーザーがいた場合

		// 元のユーザーと自身のベースを交換
		assert(pChange->GetBaseType() == CPlayer::EBaseType::TYPE_USER);
		pChange->GetBase()->SetNewBase(CPlayer::EBaseType::TYPE_AI);	// 元のユーザーをAIに変更
		pPlayer->GetBase()->SetNewBase(CPlayer::EBaseType::TYPE_USER);	// 自身をユーザーベースに変更

		// 元のユーザーの操作権インデックスを保存
		int nOldUserIdx = pChange->GetMyPlayerIdx();
		assert(nOldUserIdx != -1);

		// 元のユーザーと自身の操作権を交換
		pChange->SetMyPlayerIdx(pPlayer->GetMyPlayerIdx());	// 元のユーザーの操作権を自身の操作権に変更
		pPlayer->SetMyPlayerIdx(nOldUserIdx);				// 自身の操作権を元のユーザーの操作権に変更
	}
}

//==========================================================================
// AIベースのユーザー変更
//--------------------------------------------------------------------------
// ※ 外野はユーザーに変更不可
//==========================================================================
void CPlayerManager::ChangeUserToAI(CPlayer* pPlayer)
{
	// 自身がAIの場合は抜ける
	if (pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_AI) { return; }

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	CPlayer* pChange = nullptr;	// 変更するAIプレイヤー
	float fMinDis = 0.0f;		// AIとの最小距離

	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pItrPlayer = (*itr);	// プレイヤー情報
		
		// 自身と別チームのプレイヤーの場合次へ
		if (pItrPlayer->GetTeam() != pPlayer->GetTeam()) { continue; }

		// 内野ではない場合次へ
		if (pItrPlayer->GetAreaType() != CPlayer::EFieldArea::FIELD_IN) { continue; }

		// AIではない場合次へ
		if (pItrPlayer->GetBaseType() != CPlayer::EBaseType::TYPE_AI) { continue; }

		// AIとの距離を計算
		float fCurDis = pItrPlayer->GetPosition().DistanceXZ(pPlayer->GetPosition());
		if (pChange == nullptr || fCurDis < fMinDis)
		{ // まだAIが設定されていない、またはAI間の距離がより短かった場合

			// 変更AIを保存
			pChange = pItrPlayer;

			// より短いAI間の距離を保存
			fMinDis = fCurDis;
		}
	}

	if (pChange != nullptr)
	{ // 変更可能なAIがいた場合

		// 元のAIと自身のベースを交換
		assert(pChange->GetBaseType() == CPlayer::EBaseType::TYPE_AI);
		pChange->GetBase()->SetNewBase(CPlayer::EBaseType::TYPE_USER);	// 元のAIをユーザーに変更
		pPlayer->GetBase()->SetNewBase(CPlayer::EBaseType::TYPE_AI);	// 自身をAIベースに変更

		// 元のAIの操作権インデックスを保存
		int nOldUserIdx = pChange->GetMyPlayerIdx();
		assert(nOldUserIdx == -1);

		// 元のAIと自身の操作権を交換
		pChange->SetMyPlayerIdx(pPlayer->GetMyPlayerIdx());	// 元のAIの操作権を自身の操作権に変更
		pPlayer->SetMyPlayerIdx(nOldUserIdx);				// 自身の操作権を元のAIの操作権に変更
	}
}

//==========================================================================
// ベース入替処理
//==========================================================================
void CPlayerManager::SwapBase(CPlayer* pPlayer, CPlayer* pTarget)
{
	// 二人のプレイヤーのベースが同じ場合は抜ける
	if (pPlayer->GetBaseType() == pTarget->GetBaseType()) { return; }

	// ターゲットのベースを保存
	CPlayer::EBaseType oldBase = pTarget->GetBaseType();

	// ターゲットと自身のベースを交換
	pTarget->GetBase()->SetNewBase(pPlayer->GetBaseType());
	pPlayer->GetBase()->SetNewBase(oldBase);

	// ターゲットの操作権インデックスを保存
	int nOldUserIdx = pTarget->GetMyPlayerIdx();

	// ターゲットと自身の操作権を交換
	pTarget->SetMyPlayerIdx(pPlayer->GetMyPlayerIdx());
	pPlayer->SetMyPlayerIdx(nOldUserIdx);
}

//==========================================================================
// 外野プレイヤー登録処理 (ポジション指定)
//==========================================================================
int CPlayerManager::RegistOutPlayer(CPlayer* pPlayer, const int nPosIdx)
{
	CGameManager::ETeamSide team = pPlayer->GetTeam();		// チーム
	int nHalfMax = EOutPos::OUT_MAX / 2;					// チームごとの外野総数
	int nStart = nHalfMax * std::abs(team - 1);				// 配列先頭
	int nEnd = nHalfMax + (nHalfMax * std::abs(team - 1));	// 配列最後尾

	// ポジション指定インデックスが範囲外の場合エラー
	if (nPosIdx <= -1 || nPosIdx >= EOutPos::OUT_MAX) { assert(false); return -1; }

	// 指定のポジションに外野が登録済みの場合エラー
	if (m_apOut[nPosIdx] != nullptr) { assert(false); return -1; }

	// 指定のポジションが別チームだった場合エラー
	if ((int)team != ((nPosIdx / nHalfMax) - 1) * -1) { assert(false); return -1; }

	// 外野を登録
	m_apOut[nPosIdx] = pPlayer;

	// 現在インデックスの外野情報を取得
	SOutInfo info = GetOutInfo((EOutPos)nPosIdx);

	// 左右位置を設定
	CPlayerOut* pBase = pPlayer->GetBase()->GetPlayerOut();	// 外野プレイヤー情報
	pBase->SetPosLeft(info.posLeft);
	pBase->SetPosRight(info.posRight);

	if (pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER)
	{ // ベースがユーザーの場合

		// 左右操作の割当
		CPlayerUserOut* pBaseUser = pPlayer->GetBase()->GetPlayerUserOut();	// ユーザー外野プレイヤー情報
		pBaseUser->BindLeftKey(info.pKeyLeft);
		pBaseUser->BindRightKey(info.pKeyRight);
	}
	else
	{ // ベースがユーザーではない場合

		// 左右操作の破棄
		SAFE_DELETE(info.pKeyLeft);
		SAFE_DELETE(info.pKeyRight);
	}

	return nPosIdx;
}

//==========================================================================
// 外野プレイヤー登録処理
//==========================================================================
int CPlayerManager::RegistOutPlayer(CPlayer* pPlayer)
{
	CGameManager::ETeamSide team = pPlayer->GetTeam();		// チーム
	int nHalfMax = EOutPos::OUT_MAX / 2;					// チームごとの外野総数
	int nStart = nHalfMax * std::abs(team - 1);				// 配列先頭
	int nEnd = nHalfMax + (nHalfMax * std::abs(team - 1));	// 配列最後尾

	for (int i = nStart; i < nEnd; i++)
	{ // 同チームの外野ポジションの総数分繰り返す

		if (m_apOut[i] == nullptr)
		{ // 外野が未設定の場合

			// 外野を登録
			m_apOut[i] = pPlayer;

			// 現在インデックスの外野情報を取得
			SOutInfo info = GetOutInfo((EOutPos)i);

			// 左右位置を設定
			CPlayerOut* pBase = pPlayer->GetBase()->GetPlayerOut();	// 外野プレイヤー情報
			pBase->SetPosLeft(info.posLeft);
			pBase->SetPosRight(info.posRight);

			if (pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER)
			{ // ベースがユーザーの場合

				// 左右操作の割当
				CPlayerUserOut* pBaseUser = pPlayer->GetBase()->GetPlayerUserOut();	// ユーザー外野プレイヤー情報
				pBaseUser->BindLeftKey(info.pKeyLeft);
				pBaseUser->BindRightKey(info.pKeyRight);
			}
			else
			{ // ベースがユーザーではない場合

				// 左右操作の破棄
				SAFE_DELETE(info.pKeyLeft);
				SAFE_DELETE(info.pKeyRight);
			}

			return i;
		}
	}

	return -1;
}

//==========================================================================
// 外野プレイヤー削除処理
//==========================================================================
void CPlayerManager::DeleteOutPlayer(CPlayer* pPlayer)
{
	CGameManager::ETeamSide team = pPlayer->GetTeam();		// チーム
	int nHalfMax = EOutPos::OUT_MAX / 2;					// チームごとの外野総数
	int nStart = nHalfMax * std::abs(team - 1);				// 配列先頭
	int nEnd = nHalfMax + (nHalfMax * std::abs(team - 1));	// 配列最後尾

	for (int i = nStart; i < nEnd; i++)
	{ // 同チームの外野ポジションの総数分繰り返す

		if (m_apOut[i] == pPlayer)
		{ // 外野が設定済みの場合

			// 外野を削除し抜ける
			m_apOut[i] = nullptr;
			return;
		}
	}
}

//==========================================================================
// 内野プレイヤー登録処理
//==========================================================================
int CPlayerManager::RegistInPlayer(CPlayer* pPlayer)
{
	switch (pPlayer->GetTeam())
	{ // チームサイドごとの処理
	case CGameManager::ETeamSide::SIDE_LEFT:

		// 内野左プレイヤーリストに登録
		m_listInLeft.Regist(pPlayer);
		return m_listInLeft.GetNumAll() - 1;

	case CGameManager::ETeamSide::SIDE_RIGHT:

		// 内野右プレイヤーリストに登録
		m_listInRight.Regist(pPlayer);
		return m_listInRight.GetNumAll() - 1;

	default:
		assert(false);
		break;
	}

	return -1;
}

//==========================================================================
// 内野プレイヤー削除処理
//==========================================================================
void CPlayerManager::DeleteInPlayer(CPlayer* pPlayer)
{
	switch (pPlayer->GetTeam())
	{ // チームサイドごとの処理
	case CGameManager::ETeamSide::SIDE_LEFT:

		// 内野左プレイヤーリストから削除
		m_listInLeft.Delete(pPlayer);
		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:

		// 内野右プレイヤーリストから削除
		m_listInRight.Delete(pPlayer);
		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// 左奥の外野情報の取得処理
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetInfoLeftFar()
{
	SOutInfo info;	// 外野情報
	info.posLeft	= MyLib::Vector3(-900.0f, 0.0f, 650.0f);	// 移動可能左位置
	info.posRight	= MyLib::Vector3(-50.0f, 0.0f, 650.0f);		// 移動可能右位置
	info.pKeyLeft	= DEBUG_NEW CBindKeyLeft;	// 左移動キー
	info.pKeyRight	= DEBUG_NEW CBindKeyRight;	// 右移動キー
	return info;
}

//==========================================================================
// 左の外野情報の取得処理
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetInfoLeft()
{
	SOutInfo info;	// 外野情報
	info.posLeft	= MyLib::Vector3(-1040.0f, 0.0f, -650.0f);	// 移動可能左位置
	info.posRight	= MyLib::Vector3(-1040.0f, 0.0f, 650.0f);	// 移動可能右位置
	info.pKeyLeft	= DEBUG_NEW CBindKeyBack;	// 左移動キー
	info.pKeyRight	= DEBUG_NEW CBindKeyFront;	// 右移動キー
	return info;
}

//==========================================================================
// 左手前の外野情報の取得処理
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetInfoLeftNear()
{
	SOutInfo info;	// 外野情報
	info.posLeft	= MyLib::Vector3(-50.0f, 0.0f, -650.0f);	// 移動可能左位置
	info.posRight	= MyLib::Vector3(-900.0f, 0.0f, -650.0f);	// 移動可能右位置
	info.pKeyLeft	= DEBUG_NEW CBindKeyRight;	// 左移動キー
	info.pKeyRight	= DEBUG_NEW CBindKeyLeft;	// 右移動キー
	return info;
}

//==========================================================================
// 右奥の外野情報の取得処理
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetInfoRightFar()
{
	SOutInfo info;	// 外野情報
	info.posLeft	= MyLib::Vector3(50.0f, 0.0f, 650.0f);		// 移動可能左位置
	info.posRight	= MyLib::Vector3(900.0f, 0.0f, 650.0f);		// 移動可能右位置
	info.pKeyLeft	= DEBUG_NEW CBindKeyLeft;	// 左移動キー
	info.pKeyRight	= DEBUG_NEW CBindKeyRight;	// 右移動キー
	return info;
}

//==========================================================================
// 右の外野情報の取得処理
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetInfoRight()
{
	SOutInfo info;	// 外野情報
	info.posLeft	= MyLib::Vector3(1040.0f, 0.0f, 650.0f);	// 移動可能左位置
	info.posRight	= MyLib::Vector3(1040.0f, 0.0f, -650.0f);	// 移動可能右位置
	info.pKeyLeft	= DEBUG_NEW CBindKeyFront;	// 左移動キー
	info.pKeyRight	= DEBUG_NEW CBindKeyBack;	// 右移動キー
	return info;
}

//==========================================================================
// 右手前の外野情報の取得処理
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetInfoRightNear()
{
	SOutInfo info;	// 外野情報
	info.posLeft	= MyLib::Vector3(900.0f, 0.0f, -650.0f);	// 移動可能左位置
	info.posRight	= MyLib::Vector3(50.0f, 0.0f, -650.0f);		// 移動可能右位置
	info.pKeyLeft	= DEBUG_NEW CBindKeyRight;	// 左移動キー
	info.pKeyRight	= DEBUG_NEW CBindKeyLeft;	// 右移動キー
	return info;
}

//==========================================================================
// セーブ処理
//==========================================================================
void CPlayerManager::Save(const std::vector<LoadInfo>& LeftInfo, const std::vector<LoadInfo>& RightInfo)
{
	// ファイルを開く
	std::ofstream File(PlayerManager::TEXT_PLAYERINFO);
	if (!File.is_open()) return;

	// 引数情報渡す
	m_vecLoadInfo[CGameManager::ETeamSide::SIDE_LEFT] = LeftInfo;
	m_vecLoadInfo[CGameManager::ETeamSide::SIDE_RIGHT] = RightInfo;

	// テキストファイル名目次
	File << TOP_LINE << std::endl;
	File << "# チーム情報" << std::endl;
	File << TOP_LINE << std::endl;

	//--------------------------
	// 左チーム
	//--------------------------
	File << TEXT_LINE << std::endl;
	File << "# チーム" << CGameManager::ETeamSide::SIDE_LEFT << std::endl;
	File << TEXT_LINE << std::endl;
	File << "SETTEAM" << std::endl;

	// プレイヤー情報セーブ
	SavePlayerInfo(&File, m_vecLoadInfo[CGameManager::ETeamSide::SIDE_LEFT]);
	File << "END_SETTEAM" << std::endl;

	//--------------------------
	// 右チーム
	//--------------------------
	File << TEXT_LINE << std::endl;
	File << "# チーム" << CGameManager::ETeamSide::SIDE_RIGHT << std::endl;
	File << TEXT_LINE << std::endl;
	File << "SETTEAM" << std::endl;

	// プレイヤー情報セーブ
	SavePlayerInfo(&File, m_vecLoadInfo[CGameManager::ETeamSide::SIDE_RIGHT]);

	File << "END_SETTEAM" << std::endl;

	// ファイルを閉じる
	File.close();
}

//==========================================================================
// プレイヤー情報セーブ
//==========================================================================
void CPlayerManager::SavePlayerInfo(std::ofstream* File, const std::vector<LoadInfo>& Info)
{
	for (const auto& info : Info)
	{
		(*File) << "\tSETPLAYER" << std::endl;

		(*File) << "\t\tID_CONTROLL = " << info.nControllIdx << std::endl;	// 操作するインデックス番号
		(*File) << "\t\tHAIR = " << info.nHair << std::endl;				// 髪着せ替え
		(*File) << "\t\tACCESSORY = " << info.nAccessory << std::endl;		// アクセ着せ替え
		(*File) << "\t\tFACE = " << info.nFace << std::endl;				// 顔着せ替え
		(*File) << "\t\tBODY = " << info.eBody << std::endl;				// 体型
		(*File) << "\t\tHANDED = " << info.eHanded << std::endl;			// 利き手

		(*File) << "\tEND_SETPLAYER" << std::endl;
		(*File) << std::endl;
	}
}

//==========================================================================
// 読み込み
//==========================================================================
void CPlayerManager::Load()
{
	// ファイルを開く
	std::ifstream File(PlayerManager::TEXT_PLAYERINFO);
	if (!File.is_open()) return;

	// コメント用
	std::string hoge;

	// データ読み込み
	std::string line;

	// チームの最大数
	int nTeam = 0;

	while (std::getline(File, line))
	{
		// コメントはスキップ
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("SETTEAM") != std::string::npos)
		{// チーム情報

			// 読み込み情報クリア
			m_vecLoadInfo[nTeam].clear();

			int nPlayerNum = 0;	// プレイヤー数

			while (line.find("END_SETTEAM") == std::string::npos)
			{// END_SETTEAMが来るまで繰り返し

				// 確認する
				std::getline(File, line);

				if (line.find("SETPLAYER") != std::string::npos)
				{// プレイヤー情報読み込み

					// プレイヤー情報読み込み
					LoadPlayerInfo(&File, nTeam, nPlayerNum);

					// プレイヤー数加算
					nPlayerNum++;
				}
			}

			// チーム数加算
			nTeam++;
		}

		if (nTeam >= CGameManager::ETeamSide::SIDE_MAX)
		{// チームの最大数まで読み込んでいたら終了
			break;
		}
	}
}

//==========================================================================
// プレイヤー情報読み込み
//==========================================================================
void CPlayerManager::LoadPlayerInfo(std::ifstream* File, int nTeam, int nIdxPlayer)
{
	// コメント用
	std::string hoge;

	// チーム
	CGameManager::ETeamSide team = static_cast<CGameManager::ETeamSide>(nTeam);

	// 読み込み用変数
	LoadInfo info;	// 読み込み情報

	// データ読み込み
	std::string line;
	while (std::getline(*File, line))
	{
		// コメントはスキップ
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("END_SETPLAYER") != std::string::npos)
		{// 終了
			break;
		}

		if (line.find("ID_CONTROLL") != std::string::npos)
		{// 操作するインデックス番号

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>			// ＝
				info.nControllIdx;	// 操作するインデックス番号
		}

		if (line.find("HAIR") != std::string::npos)
		{// 髪

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				info.nHair;		// 髪
		}

		if (line.find("ACCESSORY") != std::string::npos)
		{// アクセサリー

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				info.nAccessory;		// アクセサリー
		}

		if (line.find("FACE") != std::string::npos)
		{// 顔

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				info.nFace;		// 顔
		}

		if (line.find("BODY") != std::string::npos)
		{// 体型

			int body = -1;

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				body;		// 体型

			info.eBody = static_cast<CPlayer::EBody>(body);
		}

		if (line.find("HANDED") != std::string::npos)
		{// 利き手

			int hand = -1;

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				hand;		// 利き手

			info.eHanded = static_cast<CPlayer::EHandedness>(hand);
		}
	}

	// 読み込んだ情報を格納
	m_vecLoadInfo[nTeam].push_back(info);
}