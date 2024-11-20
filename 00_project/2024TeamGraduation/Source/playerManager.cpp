//==========================================================================
// 
//  プレイヤーマネージャー処理 [playerManager.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerManager.h"
#include "player.h"
#include "playerUserOut.h"
#include "bindKeyFront.h"
#include "bindKeyBack.h"
#include "bindKeyLeft.h"
#include "bindKeyRight.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

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

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerManager::CPlayerManager()
{
	// メンバ変数をクリア
	memset(&m_apOut[0], 0, sizeof(m_apOut));	// 外野プレイヤー
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
CPlayerManager* CPlayerManager::Create()
{
	// 既に生成済みの場合抜ける
	if (m_pInstance != nullptr) { return m_pInstance; }

	// インスタンスの生成
	m_pInstance = DEBUG_NEW CPlayerManager;
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
	for (int i = 0; i < 4; i++)
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
// 外野情報の取得処理
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetOutInfo(const EOutPos out)
{
	// 外野情報の取得
	return (this->*(m_InfoFuncList[out]))();
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
// キャッチしたプレイヤーのユーザーチェンジ
//==========================================================================
void CPlayerManager::CatchUserChange(CPlayer* pPlayer)
{
#if 1
	if (pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_AI) { return; }

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	int nCatchPlayerIdx = -1;	// キャッチしたプレイヤーの操作インデックス

	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pItrPlayer = (*itr);	// プレイヤー情報
		
		// TODO：これだとプレイヤー二人になった途端終了するよ
		//		(近いやつを見つけてそういつのインデックスに置き換えよう)

		// キャッチしたプレイヤーと別チームのプレイヤーの場合次へ
		if (pItrPlayer->GetTeam() != pPlayer->GetTeam()) { continue; }

		if (pItrPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER)
		{ // 同じチームにユーザーがいた場合

			// 同チームのプレイヤーをAIベースに変更
			pItrPlayer->GetBase()->SetNewBase(CPlayer::EBaseType::TYPE_AI);
			nCatchPlayerIdx = pItrPlayer->GetMyPlayerIdx();
		}
	}

	// キャッチしたプレイヤーをユーザーベースに変更
	pPlayer->GetBase()->SetNewBase(CPlayer::EBaseType::TYPE_USER);
	pPlayer->SetMyPlayerIdx(nCatchPlayerIdx);
#endif
}

//==========================================================================
// 近いプレイヤーのユーザーチェンジ
//==========================================================================
void CPlayerManager::NearUserChange(CPlayer* pPlayer)
{
#if 1
	if (pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_AI) { return; }

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	int nCatchPlayerIdx = -1;	// キャッチしたプレイヤーの操作インデックス

	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pItrPlayer = (*itr);	// プレイヤー情報
		
		// TODO：これだとプレイヤー二人になった途端終了するよ
		//		(近いやつを見つけてそういつのインデックスに置き換えよう)

		// キャッチしたプレイヤーと別チームのプレイヤーの場合次へ
		if (pItrPlayer->GetTeam() != pPlayer->GetTeam()) { continue; }

		if (pItrPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER)
		{ // 同じチームにユーザーがいた場合

			// 同チームのプレイヤーをAIベースに変更
			pItrPlayer->GetBase()->SetNewBase(CPlayer::EBaseType::TYPE_AI);
			nCatchPlayerIdx = pItrPlayer->GetMyPlayerIdx();
		}
	}

	// キャッチしたプレイヤーをユーザーベースに変更
	pPlayer->GetBase()->SetNewBase(CPlayer::EBaseType::TYPE_USER);
	pPlayer->SetMyPlayerIdx(nCatchPlayerIdx);
#endif
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
	if ((int)team != (nPosIdx / nHalfMax)) { assert(false); return -1; }

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
