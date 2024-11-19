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
int CPlayerManager::RegistPlayer(CPlayer* pPlayer)
{
	// TODO：元のインデックス持っておいて、追加する要素番号と一致しているかを見たほうが安全そう

	switch (pPlayer->GetAreaType())
	{ // ポジションごとの処理
	case CPlayer::EFieldArea::FIELD_IN:

		// リストマネージャーに登録
		m_listIn.Regist(pPlayer);
		return m_listIn.GetNumAll() - 1;

	case CPlayer::EFieldArea::FIELD_OUT:

		// 配列に登録
		return RegistOutPlayer(pPlayer);

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
		m_listIn.Delete(pPlayer);
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
// 外野プレイヤー登録処理
//==========================================================================
int CPlayerManager::RegistOutPlayer(CPlayer* pPlayer)
{
	CGameManager::ETeamSide team = pPlayer->GetTeam();	// チーム
	int nHalfMax = (int)(EOutPos::OUT_MAX * 0.5f);		// チームごとの外野総数
	int nStart = nHalfMax * (int)team;					// 配列先頭
	int nEnd = nHalfMax + (nHalfMax * (int)team);		// 配列最後尾

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
	CGameManager::ETeamSide team = pPlayer->GetTeam();	// チーム
	int nHalfMax = (int)(EOutPos::OUT_MAX * 0.5f);		// チームごとの外野総数
	int nStart = nHalfMax * (int)team;					// 配列先頭
	int nEnd = nHalfMax + (nHalfMax * (int)team);		// 配列最後尾

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
// 左奥の外野情報の取得処理
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetInfoLeftFar()
{
	SOutInfo info;	// 外野情報
	info.posLeft	= MyLib::Vector3(50.0f, 0.0f, 650.0f);		// 移動可能左位置
	info.posRight	= MyLib::Vector3(900.0f, 0.0f, 650.0f);		// 移動可能右位置
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
	info.posLeft	= MyLib::Vector3(1040.0f, 0.0f, 650.0f);	// 移動可能左位置
	info.posRight	= MyLib::Vector3(1040.0f, 0.0f, -650.0f);	// 移動可能右位置
	info.pKeyLeft	= DEBUG_NEW CBindKeyFront;	// 左移動キー
	info.pKeyRight	= DEBUG_NEW CBindKeyBack;	// 右移動キー
	return info;
}

//==========================================================================
// 左手前の外野情報の取得処理
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetInfoLeftNear()
{
	SOutInfo info;	// 外野情報
	info.posLeft	= MyLib::Vector3(900.0f, 0.0f, -650.0f);	// 移動可能左位置
	info.posRight	= MyLib::Vector3(50.0f, 0.0f, -650.0f);		// 移動可能右位置
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
	info.posLeft	= MyLib::Vector3(-900.0f, 0.0f, 650.0f);	// 移動可能左位置
	info.posRight	= MyLib::Vector3(-50.0f, 0.0f, 650.0f);		// 移動可能右位置
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
	info.posLeft	= MyLib::Vector3(-1040.0f, 0.0f, -650.0f);	// 移動可能左位置
	info.posRight	= MyLib::Vector3(-1040.0f, 0.0f, 650.0f);	// 移動可能右位置
	info.pKeyLeft	= DEBUG_NEW CBindKeyBack;	// 左移動キー
	info.pKeyRight	= DEBUG_NEW CBindKeyFront;	// 右移動キー
	return info;
}

//==========================================================================
// 右手前の外野情報の取得処理
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetInfoRightNear()
{
	SOutInfo info;	// 外野情報
	info.posLeft	= MyLib::Vector3(-50.0f, 0.0f, -650.0f);	// 移動可能左位置
	info.posRight	= MyLib::Vector3(-900.0f, 0.0f, -650.0f);	// 移動可能右位置
	info.pKeyLeft	= DEBUG_NEW CBindKeyRight;	// 左移動キー
	info.pKeyRight	= DEBUG_NEW CBindKeyLeft;	// 右移動キー
	return info;
}
