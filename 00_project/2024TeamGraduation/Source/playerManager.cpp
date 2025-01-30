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
#include "playerManager_spawn.h"
#include "playerManager_result.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TOP_LINE	= "#==============================================================================";	// テキストのライン
	const std::string TEXT_LINE	= "#------------------------------------------------------------------------------";	// テキストのライン
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
std::vector<CPlayerManager::LoadInfo> CPlayerManager::m_vecInLoadInfo[CGameManager::ETeamSide::SIDE_MAX] = {};	// 内野読み込み情報
std::vector<CPlayerManager::LoadInfo> CPlayerManager::m_vecOutLoadInfo[CGameManager::ETeamSide::SIDE_MAX] = {};	// 外野読み込み情報

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerManager::CPlayerManager() : m_bUserChange(false)	// ユーザー変更操作フラグ
{
	// メンバ変数をクリア
	memset(&m_apOut[0], 0, sizeof(m_apOut));	// 外野プレイヤー

	for (int i= 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		m_vecInLoadInfo[i].clear();
		m_vecOutLoadInfo[i].clear();
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
CPlayerManager* CPlayerManager::Create(EType type)
{
	// 既に生成済みの場合抜ける
	if (m_pInstance != nullptr) { return m_pInstance; }

	// インスタンスの生成
	switch (type)
	{
	case EType::TYPE_GAME:
		m_pInstance = DEBUG_NEW CPlayerManager_Game;
		break;

	case EType::TYPE_SPAWN:
		m_pInstance = DEBUG_NEW CPlayerManager_Spawn;
		break;

	case EType::TYPE_RESULT:
		m_pInstance = DEBUG_NEW CPlayerManager_Result;
		break;

	default:
		assert(false);
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
	// 読み込み処理
	Load();

	// プレイヤー生成
	CreatePlayer();

	return S_OK;
}

//==========================================================================
// 外野プレイヤー生成
//==========================================================================
HRESULT CPlayerManager::CreateOutPlayer(CGameManager::ETeamSide team, const LoadInfo& info)
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

//==========================================================================
// 左のプレイヤー生成
//==========================================================================
HRESULT CPlayerManager::CreateLeftPlayer(int i, const LoadInfo& info)
{
	CPlayer::EBaseType baseType = (info.nControllIdx >= 0) ? CPlayer::EBaseType::TYPE_USER : CPlayer::EBaseType::TYPE_AI;	// ベースタイプ
	CPlayer* pPlayer = CPlayer::Create
	(
		MyLib::Vector3(), 					// 位置
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
	pPlayer->SetRotation(MyLib::Vector3(0.0f, -HALF_PI, 0.0f));
	pPlayer->SetRotDest(-HALF_PI);
	
	// インデックス反映
	pPlayer->SetMyPlayerIdx(info.nControllIdx);
	pPlayer->SetMyPlayerIdxFace(info.nControllIdx);

	// ドレスアップ反映
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);
	pPlayer->BindDressUpFace(info.nHair, info.nAccessory, info.nFace);

	return S_OK;
}

//==========================================================================
// 左のプレイヤー生成(位置あり)
//==========================================================================
HRESULT CPlayerManager::CreateLeftPlayer(int i, const LoadInfo& info, const MyLib::Vector3& pos)
{
	// 左のプレイヤー生成
	HRESULT hr = CreateLeftPlayer(i, info);

	// リスト取得
	std::list<CPlayer*> list = CPlayer::GetList().GetList();
	if (list.empty()) return hr;

	// 追加されたプレイヤー取得
	CPlayer* pPlayer = list.back();
	if (pPlayer == nullptr) return hr;

	// 位置の初期化
	pPlayer->GetBase()->InitPosition(pos);

	return hr;
}

//==========================================================================
// 右のプレイヤー生成
//==========================================================================
HRESULT CPlayerManager::CreateRightPlayer(int i, const LoadInfo& info)
{
	CPlayer::EBaseType baseType = (info.nControllIdx >= 0) ? CPlayer::EBaseType::TYPE_USER : CPlayer::EBaseType::TYPE_AI;	// ベースタイプ
	CPlayer* pPlayer = CPlayer::Create
	(
		MyLib::Vector3(), 						// 位置
		CGameManager::ETeamSide::SIDE_RIGHT,	// チームサイド
		CPlayer::EFieldArea::FIELD_IN,			// ポジション
		baseType,								// ベースタイプ
		info.eBody,								// 体系
		info.eHanded							// 利き手
	);
	if (pPlayer == nullptr)
	{
		return E_FAIL;
	}
	pPlayer->SetRotation(MyLib::Vector3(0.0f, HALF_PI, 0.0f));
	pPlayer->SetRotDest(HALF_PI);

	// インデックス反映
	pPlayer->SetMyPlayerIdx(info.nControllIdx);
	pPlayer->SetMyPlayerIdxFace(info.nControllIdx);

	// ドレスアップ反映
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);
	pPlayer->BindDressUpFace(info.nHair, info.nAccessory, info.nFace);

	return S_OK;
}

//==========================================================================
// 右のプレイヤー生成(位置あり)
//==========================================================================
HRESULT CPlayerManager::CreateRightPlayer(int i, const LoadInfo& info, const MyLib::Vector3& pos)
{
	// 右のプレイヤー生成
	HRESULT hr = CreateRightPlayer(i, info);

	// リスト取得
	std::list<CPlayer*> list = CPlayer::GetList().GetList();
	if (list.empty()) return hr;

	// 追加されたプレイヤー取得
	CPlayer* pPlayer = list.back();
	if (pPlayer == nullptr) return hr;

	// 位置の初期化
	pPlayer->GetBase()->InitPosition(pos);

	return hr;
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

		// 死亡状態の場合次へ
		if (pItrPlayer->IsDeathState()) { continue; }

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

		// 死亡状態の場合次へ
		if (pItrPlayer->IsDeathState()) { continue; }
		
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
		assert(nOldUserIdx == -1);	// TODO：ここでえらーでたよ！？俺！？

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
void CPlayerManager::Save
(
	const std::vector<LoadInfo>& inLeftInfo,
	const std::vector<LoadInfo>& inRightInfo,
	const std::vector<LoadInfo>& outLeftInfo,
	const std::vector<LoadInfo>& outRightInfo
)
{
	// ファイルを開く
	std::ofstream File(PlayerManager::TEXT_PLAYERINFO);
	if (!File.is_open()) return;

	// 引数情報渡す
	m_vecInLoadInfo[CGameManager::ETeamSide::SIDE_LEFT]		= inLeftInfo;
	m_vecInLoadInfo[CGameManager::ETeamSide::SIDE_RIGHT]	= inRightInfo;
	m_vecOutLoadInfo[CGameManager::ETeamSide::SIDE_LEFT]	= outLeftInfo;
	m_vecOutLoadInfo[CGameManager::ETeamSide::SIDE_RIGHT]	= outRightInfo;

	// テキストファイル名目次
	File << TOP_LINE << std::endl;
	File << "# チーム情報" << std::endl;
	File << TOP_LINE << std::endl;

#if 0
	//--------------------------
	// 内野
	//--------------------------
	// 内野チーム
	File << TEXT_LINE << std::endl;
	File << "# 内野" << std::endl;
	File << TEXT_LINE << std::endl;
	File << "SETIN" << std::endl;

	// 左チーム
	File << "\t# チーム" << CGameManager::ETeamSide::SIDE_LEFT << std::endl;

	// プレイヤー情報セーブ
	File << "\tSETTEAM\n" << std::endl;
	SavePlayerInfo(&File, m_vecInLoadInfo[CGameManager::ETeamSide::SIDE_LEFT]);
	File << "\tEND_SETTEAM\n" << std::endl;

	// 右チーム
	File << "\t# チーム" << CGameManager::ETeamSide::SIDE_RIGHT << std::endl;

	// プレイヤー情報セーブ
	File << "\tSETTEAM\n" << std::endl;
	SavePlayerInfo(&File, m_vecInLoadInfo[CGameManager::ETeamSide::SIDE_RIGHT]);
	File << "\tEND_SETTEAM" << std::endl;

	File << "END_SETIN\n" << std::endl;

	//--------------------------
	// 外野
	//--------------------------
	// 外野チーム
	File << TEXT_LINE << std::endl;
	File << "# 外野" << std::endl;
	File << TEXT_LINE << std::endl;
	File << "SETOUT" << std::endl;

	// 左チーム
	File << "\t# チーム" << CGameManager::ETeamSide::SIDE_LEFT << std::endl;

	// プレイヤー情報セーブ
	File << "\tSETTEAM\n" << std::endl;
	SavePlayerInfo(&File, m_vecOutLoadInfo[CGameManager::ETeamSide::SIDE_LEFT]);
	File << "\tEND_SETTEAM\n" << std::endl;

	// 右チーム
	File << "\t# チーム" << CGameManager::ETeamSide::SIDE_RIGHT << std::endl;

	// プレイヤー情報セーブ
	File << "\tSETTEAM\n" << std::endl;
	SavePlayerInfo(&File, m_vecOutLoadInfo[CGameManager::ETeamSide::SIDE_RIGHT]);
	File << "\tEND_SETTEAM" << std::endl;

	File << "END_SETOUT\n" << std::endl;
#else
	//--------------------------
	// 左チーム
	//--------------------------
	// 書き出し開始
	File << TEXT_LINE << std::endl;
	File << "# 左チーム" << std::endl;
	File << TEXT_LINE << std::endl;
	File << "SETTEAM" << std::endl;

	// 内野書き出し
	File << "\t# 内野チーム" << std::endl;
	File << "\tSETIN\n" << std::endl;
	SavePlayerInfo(&File, m_vecInLoadInfo[CGameManager::ETeamSide::SIDE_LEFT]);
	File << "\tEND_SETIN\n" << std::endl;

	// 外野書き出し
	File << "\t# 外野チーム" << std::endl;
	File << "\tSETOUT\n" << std::endl;
	SavePlayerInfo(&File, m_vecOutLoadInfo[CGameManager::ETeamSide::SIDE_LEFT]);
	File << "\tEND_SETOUT" << std::endl;

	// 書き出し終了
	File << "END_SETTEAM\n" << std::endl;

	//--------------------------
	// 右チーム
	//--------------------------
	// 書き出し開始
	File << TEXT_LINE << std::endl;
	File << "# 右チーム" << std::endl;
	File << TEXT_LINE << std::endl;
	File << "SETTEAM" << std::endl;

	// 内野書き出し
	File << "\t# 内野チーム" << std::endl;
	File << "\tSETIN\n" << std::endl;
	SavePlayerInfo(&File, m_vecInLoadInfo[CGameManager::ETeamSide::SIDE_RIGHT]);
	File << "\tEND_SETIN\n" << std::endl;

	// 外野書き出し
	File << "\t# 外野チーム" << std::endl;
	File << "\tSETOUT\n" << std::endl;
	SavePlayerInfo(&File, m_vecOutLoadInfo[CGameManager::ETeamSide::SIDE_RIGHT]);
	File << "\tEND_SETOUT" << std::endl;

	// 書き出し終了
	File << "END_SETTEAM\n" << std::endl;
#endif

	// ファイルを閉じる
	File.close();
}

//==========================================================================
// 警戒処理
//==========================================================================
void CPlayerManager::CautionInAll(const CGameManager::ETeamSide team)
{
	CListManager<CPlayer> sampleList = GetInList(team);
	std::list<CPlayer*>::iterator itr = sampleList.GetEnd();
	CPlayer* pObj = nullptr;

	while (sampleList.ListLoop(itr))
	{
		pObj = (*itr);

		// 地上に張り付ける
		MyLib::Vector3 pos = pObj->GetPosition();
		pos.y = 0.0f;
		pObj->SetPosition(pos);

		// 通常モーション
		pObj->SetMotion(CPlayer::EMotion::MOTION_DEF);

		// ジャンプ強制終了
		pObj->SetEnableJump(false);

		// 操作不可
		//pObj->SetEnableAction(false);
		//pObj->SetEnableMove(false);
	}
}

//==========================================================================
// 警戒処理
//==========================================================================
void CPlayerManager::CautionOutAll()
{
	for (int i = 0; i < OUT_MAX; i++)
	{ // 外野人数分繰り返す

		// 地上に張り付ける
		MyLib::Vector3 pos = m_apOut[i]->GetPosition();
		pos.y = 0.0f;
		m_apOut[i]->SetPosition(pos);

		// 通常モーション
		m_apOut[i]->SetMotion(CPlayer::EMotion::MOTION_DEF);

		// 操作不可
		//m_apOut[i]->SetEnableAction(false);
		//m_apOut[i]->SetEnableMove(false);
	}
}

//==========================================================================
// プレイヤー情報セーブ
//==========================================================================
void CPlayerManager::SavePlayerInfo(std::ofstream* File, const std::vector<LoadInfo>& Info)
{
	for (const auto& info : Info)
	{
		(*File) << "\t\tSETPLAYER" << std::endl;

		(*File) << "\t\t\tID_CONTROLL = " << info.nControllIdx << std::endl;	// 操作するインデックス番号
		(*File) << "\t\t\tHAIR = " << info.nHair << std::endl;					// 髪着せ替え
		(*File) << "\t\t\tACCESSORY = " << info.nAccessory << std::endl;		// アクセ着せ替え
		(*File) << "\t\t\tFACE = " << info.nFace << std::endl;					// 顔着せ替え
		(*File) << "\t\t\tBODY = " << info.eBody << std::endl;					// 体型
		(*File) << "\t\t\tHANDED = " << info.eHanded << std::endl;				// 利き手

		(*File) << "\t\tEND_SETPLAYER" << std::endl;
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

			while (line.find("END_SETTEAM") == std::string::npos)
			{// END_SETTEAMが来るまで繰り返し

				// 確認する
				std::getline(File, line);

				if (line.find("SETIN") != std::string::npos)
				{// チーム情報

					// 読み込み情報クリア
					m_vecInLoadInfo[nTeam].clear();
					int nPlayerNum = 0;	// プレイヤー数
					while (line.find("END_SETIN") == std::string::npos)
					{// END_SETINが来るまで繰り返し

						// 確認する
						std::getline(File, line);

						if (line.find("SETPLAYER") != std::string::npos)
						{// プレイヤー情報読み込み

							// プレイヤー情報読み込み
							LoadPlayerInfo(&File, nTeam, nPlayerNum, &m_vecInLoadInfo[nTeam]);

							// プレイヤー数加算
							nPlayerNum++;
						}
					}
				}
				else if (line.find("SETOUT") != std::string::npos)
				{// チーム情報

					// 読み込み情報クリア
					m_vecOutLoadInfo[nTeam].clear();
					int nPlayerNum = 0;	// プレイヤー数
					while (line.find("END_SETOUT") == std::string::npos)
					{// END_SETOUTが来るまで繰り返し

						// 確認する
						std::getline(File, line);

						if (line.find("SETPLAYER") != std::string::npos)
						{// プレイヤー情報読み込み

							// プレイヤー情報読み込み
							LoadPlayerInfo(&File, nTeam, nPlayerNum, &m_vecOutLoadInfo[nTeam]);

							// プレイヤー数加算
							nPlayerNum++;
						}
					}
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
void CPlayerManager::LoadPlayerInfo(std::ifstream* File, int nTeam, int nIdxPlayer, std::vector<LoadInfo>* pInfo)
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
	pInfo->push_back(info);
}
