//=============================================================================
// 
// リザルトマネージャ処理 [resultmanager.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "resultmanager.h"
#include "result.h"

#include "playerResult.h"
#include "playerManager.h"
#include "audience.h"
#include "camera.h"

#include "gamemanager.h"
#include "object3D.h"
#include "3D_effect.h"
#include "resultCrown.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXFILE_PRELUDE = "data\\TEXTURE\\result\\prelude.png";	// 試合に勝利したのは！
	const std::string TEXFILE_CONTEST = "data\\TEXTURE\\result\\contest.png";	// よりモテたのは！
	const MyLib::Vector2 SIZE_POLY = MyLib::Vector2(500.0f, 100.0f);		// 3Dポリゴンサイズ
	const float POSY_POLY = 100.0f;											// 3Dポリゴン位置(y)

	const MyLib::Vector3 ROT_CAMERA = MyLib::Vector3(0.0f, 0.0f, -0.36f);	// スタート時カメラ向き
	const float DISTANCE_CAMERA = 1940.0f;									// スタート時カメラ距離
	const float POSR_HEIGHT = 100.0f;										// カメラの注視点の高さ

	const MyLib::Vector3 POS_COURT[CGameManager::ETeamSide::SIDE_MAX] =	// コート中央位置
	{
		MyLib::Vector3(-900.0f, 0.0f, 0.0f),	// 左
		MyLib::Vector3(+900.0f, 0.0f, 0.0f),	// 右
	};
}

// 状態時間
namespace StateTime
{
	const float NONE(3.0f);		// なし
	const float READY(3.0f);	// 準備
	const float PRELUDE(5.0f);	// 前座
}

namespace Draw
{// 引き分け
	const MyLib::Vector3 POS = MyLib::Vector3(0.0f, 100.0f, 0.0f);	// ポリゴン位置

	const float DISTANCE_CAMERA = 1700.0f;		// カメラ距離
}

namespace Ready
{
	const MyLib::Vector3 POSR_CAMERA = MyLib::Vector3(0.0f, POSR_HEIGHT, 0.0f);
	const MyLib::Vector3 ROT_CAMERA = MyLib::Vector3(0.0f, 0.0f, -0.36f);

	const float DISTANCE_CAMERA = 1500.0f;

	const float POSY_WORD = 200.0f;	// セリフの高さ
}

namespace Prelude
{
	const std::map<CGameManager::ETeamSide, MyLib::Vector3> POSR_CAMERA =
	{
		{CGameManager::ETeamSide::SIDE_NONE,	MyLib::Vector3(0.0f, POSR_HEIGHT, 0.0f)},				// コート不明
		{CGameManager::ETeamSide::SIDE_LEFT,	MyLib::Vector3(-350.0f, POSR_HEIGHT * 2.0f, -200.0f)},		// 左コート
		{CGameManager::ETeamSide::SIDE_RIGHT,	MyLib::Vector3(+350.0f, POSR_HEIGHT * 2.0f, -200.0f)},		// 右コート
	};

	const std::map<CGameManager::ETeamSide, MyLib::Vector3> ROT_CAMERA =
	{
		{CGameManager::ETeamSide::SIDE_NONE,	MyLib::Vector3(0.0f, 0.0f, -0.36f)},		// コート不明
		{CGameManager::ETeamSide::SIDE_LEFT,	MyLib::Vector3(0.0f, -0.95f, -0.36f)},		// 左コート
		{CGameManager::ETeamSide::SIDE_RIGHT,	MyLib::Vector3(0.0f, +0.95f, -0.36f)},		// 右コート
	};

	const float DISTANCE_CAMERA = 1500.0f;

	const float POSY_CROWN = 200.0f;	// 王冠の高さ
}

namespace Contest
{
	const std::map<CGameManager::ETeamSide, MyLib::Vector3> POSR_CAMERA =
	{
		{CGameManager::ETeamSide::SIDE_NONE,	MyLib::Vector3(0.0f, POSR_HEIGHT, 0.0f)},						// コート不明
		{CGameManager::ETeamSide::SIDE_LEFT,	MyLib::Vector3(-37.34f, 220.0f, 260.0f) + POS_COURT[0]},	// 左コート
		{CGameManager::ETeamSide::SIDE_RIGHT,	MyLib::Vector3(+37.34f, 220.0f, 260.0f) + POS_COURT[1]},	// 右コート
	};

	const std::map<CGameManager::ETeamSide, MyLib::Vector3> ROT_CAMERA =
	{
		{CGameManager::ETeamSide::SIDE_NONE,	MyLib::Vector3(0.0f, 0.0f, -0.36f)},		// コート不明
		{CGameManager::ETeamSide::SIDE_LEFT,	MyLib::Vector3(0.0f, -0.46f, 0.23f)},		// 左コート
		{CGameManager::ETeamSide::SIDE_RIGHT,	MyLib::Vector3(0.0f, +0.46f, 0.23f)},		// 右コート
	};

	const float DISTANCE_CAMERA = 700.0f;

	const float POSY_CROWN = 400.0f;	// 王冠の高さ
}

// カメラ遷移時間
namespace CameraTime
{
	const float END_TIME[] =
	{
		0.0f,		// なにもない
		0.3f,		// 前座勝敗準備
		1.0f,		// 前座勝敗
		0.3f,		// モテ勝敗準備
		1.0f,		// モテ勝敗
	};
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CResultManager::STATE_FUNC CResultManager::m_StateFunc[] =				// 状態関数
{
	& CResultManager::StateNone,				// なし
	& CResultManager::StatePreludeReady,		// 前座勝敗準備
	& CResultManager::StatePrelude,				// 前座勝敗
	& CResultManager::StateCharmContestReady,	// モテ勝敗準備
	& CResultManager::StateCharmContest,		// モテ勝敗
};

CResultManager::STATE_START_FUNC CResultManager::m_StateStartFunc[] =	// 状態開始
{
	nullptr,										// なし
	& CResultManager::StateStartPreludeReady,		// 前座勝敗準備
	& CResultManager::StateStartPrelude,			// 前座勝敗
	& CResultManager::StateStartCharmContestReady,	// モテ勝敗準備
	& CResultManager::StateStartCharmContest,		// モテ勝敗
};

CResultManager::STATE_END_FUNC CResultManager::m_StateEndFunc[] =		// 状態終了
{
	nullptr,									// なし
	nullptr,									// 前座勝敗準備
	& CResultManager::StateEndPrelude,			// 前座勝敗
	nullptr,									// モテ勝敗準備
	& CResultManager::StateEndCharmContest,		// モテ勝敗
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CResultManager* CResultManager::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CResultManager::CResultManager()
{
	m_fTension = 0.0f;												// 盛り上がり値
	m_teamPreludeWin = CGameManager::ETeamSide::SIDE_NONE;			// 勝利チーム
	m_teamContestWin = CGameManager::ETeamSide::SIDE_NONE;			// 勝利チーム
	memset(&m_fCharmValue[0], 0, sizeof(m_fCharmValue));			// モテ値
	m_state = EState::STATE_NONE;									// 状態
	m_fStateTime = 0.0f;											// 状態時間
	m_bStateTrans = false;											// 状態遷移可能フラグ(t:不可/f:可)
	m_pText = nullptr;												// 勝利チーム用3Dポリゴン
	m_pCrown = nullptr;												// 王冠モデル
	m_pEfkConfetti = nullptr;										// 紙吹雪エフェクシア
}

//==========================================================================
// デストラクタ
//==========================================================================
CResultManager::~CResultManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CResultManager* CResultManager::Create()
{
	if (m_pThisPtr != nullptr)
	{// 既にある場合
		return m_pThisPtr;
	}

	// インスタンス生成
	m_pThisPtr = DEBUG_NEW CResultManager;

	if (m_pThisPtr != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		HRESULT hr = m_pThisPtr->Init();

		if (FAILED(hr))
		{// 失敗していたら
			return nullptr;
		}
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CResultManager::Init()
{
	//チームステータス
	Load();

	// プレイヤーマネージャーの生成
	CPlayerManager::Create(CPlayerManager::EType::TYPE_RESULT);

	// 観客生成
	CreateAudience();

	// 遷移可能に
	m_bStateTrans = true;

	// カメラ設定
	CCamera* pCamera = GET_MANAGER->GetCamera();
	pCamera->SetDistance(DISTANCE_CAMERA);
	pCamera->SetRotation(ROT_CAMERA);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CResultManager::Uninit()
{
	// プレイヤーマネージャー
	CPlayerManager* pManager = CPlayerManager::GetInstance();
	SAFE_UNINIT(pManager);

	// 王冠
	m_pCrown = nullptr;
	
	// 勝利準備ポリゴン
	m_pText = nullptr;

	// エフェクト破棄
	if (m_pEfkConfetti != nullptr)
	{
		m_pEfkConfetti->SetTrigger(0);
		m_pEfkConfetti = nullptr;
	}

	// 自身の開放
	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CResultManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

#if _DEBUG	// デバッグ処理

	if (ImGui::TreeNode("ResultManager"))
	{
		Debug();
		ImGui::TreePop();
	}
#endif
}

//==========================================================================
// 状態更新
//==========================================================================
void CResultManager::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態タイマー加算
	m_fStateTime += fDeltaTime * fSlowRate;

	// 状態更新
	if (m_StateFunc[m_state] != nullptr)
	{
		(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// なし
//==========================================================================
void CResultManager::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 指定時間を過ぎたら
	if (m_fStateTime >= StateTime::NONE && m_bStateTrans)
	{
		SetState(EState::STATE_PRELUDE_READY);
	}
}

//==========================================================================
// 前座勝敗準備状態
//==========================================================================
void CResultManager::StatePreludeReady(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	float START = 0.0f;
	float END = CameraTime::END_TIME[m_state];
	
	// 試合に勝利したチーム！

	// カメラ補正
	CCamera* pCamera = GET_MANAGER->GetCamera();
	Ready::DISTANCE_CAMERA;
	if (pCamera->GetState() == CCamera::STATE::STATE_FOLLOW)
	{
		float fDistance = UtilFunc::Correction::EasingLinear(pCamera->GetDistanceOrigin(), Ready::DISTANCE_CAMERA, START, END, m_fStateTime);
		MyLib::Vector3 rot = UtilFunc::Correction::EasingLinear(pCamera->GetOriginRotation(), Ready::ROT_CAMERA, START, END, m_fStateTime);
		MyLib::Vector3 posR = UtilFunc::Correction::EasingLinear(pCamera->GetPositionROrigin(), Ready::POSR_CAMERA, START, END, m_fStateTime);

		pCamera->SetDistance(fDistance);
		pCamera->SetRotation(rot);
		pCamera->SetPositionR(posR);
	}

	// 指定時間を過ぎたら
	if (m_fStateTime >= StateTime::READY && m_bStateTrans)
	{
		// リザルトプレイヤー全員確認
		CListManager<CPlayerResult> list = CPlayerResult::GetList();
		std::list<CPlayerResult*>::iterator itr = list.GetEnd();	// 左チームの最後尾イテレーター
		while (list.ListLoop(itr))
		{
			// 勝敗チェック(前座)
			CPlayerResult* pPlayer = (*itr);	// プレイヤー情報
			pPlayer->CheckVictoryPrelude();
		}

		// 前座勝敗状態
		SetState(EState::STATE_PRELUDE);
	}
}

//==========================================================================
// 前座勝敗状態
//==========================================================================
void CResultManager::StatePrelude(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	float START = 0.0f;
	float END = CameraTime::END_TIME[m_state];

	// カメラ補正
	CCamera* pCamera = GET_MANAGER->GetCamera();

	if (pCamera->GetState() == CCamera::STATE::STATE_FOLLOW)
	{
		float fDistance = UtilFunc::Correction::EasingLinear(pCamera->GetDistanceOrigin(), Prelude::DISTANCE_CAMERA, START, END, m_fStateTime);
		MyLib::Vector3 rot = UtilFunc::Correction::EasingLinear(pCamera->GetOriginRotation(), Prelude::ROT_CAMERA.at(m_teamPreludeWin), START, END, m_fStateTime);
		MyLib::Vector3 posR = UtilFunc::Correction::EasingLinear(pCamera->GetPositionROrigin(), Prelude::POSR_CAMERA.at(m_teamPreludeWin), START, END, m_fStateTime);

		pCamera->SetDistance(fDistance);
		pCamera->SetRotation(rot);
		pCamera->SetPositionR(posR);
	}

	// 指定時間を過ぎたら
	if (m_fStateTime >= StateTime::PRELUDE && m_bStateTrans)
	{
		// リザルトプレイヤー全員確認
		CListManager<CPlayerResult> list = CPlayerResult::GetList();
		std::list<CPlayerResult*>::iterator itr = list.GetEnd();	// 左チームの最後尾イテレーター
		while (list.ListLoop(itr))
		{
			// 状態NONEへ
			CPlayerResult* pPlayer = (*itr);	// プレイヤー情報
			pPlayer->SetState(CPlayerResult::EState::STATE_NONE);
		}

		SetState(EState::STATE_CONTEST_READY);
	}
}

//==========================================================================
// モテ勝敗準備状態
//==========================================================================
void CResultManager::StateCharmContestReady(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	float START = 0.0f;
	float END = CameraTime::END_TIME[m_state];

	// モテ勝利したチーム！


	// カメラ補正
	CCamera* pCamera = GET_MANAGER->GetCamera();

	if (pCamera->GetState() == CCamera::STATE::STATE_FOLLOW)
	{
		float fDistance = UtilFunc::Correction::EasingLinear(pCamera->GetDistanceOrigin(), Ready::DISTANCE_CAMERA, START, END, m_fStateTime);
		MyLib::Vector3 rot = UtilFunc::Correction::EasingLinear(pCamera->GetOriginRotation(), Ready::ROT_CAMERA, START, END, m_fStateTime);
		MyLib::Vector3 posR = UtilFunc::Correction::EasingLinear(pCamera->GetPositionROrigin(), Ready::POSR_CAMERA, START, END, m_fStateTime);

		pCamera->SetDistance(fDistance);
		pCamera->SetRotation(rot);
		pCamera->SetPositionR(posR);
	}

	// 指定時間を過ぎたら
	if (m_fStateTime >= StateTime::READY && m_bStateTrans)
	{
		// リザルトプレイヤー全員確認
		CListManager<CPlayerResult> list = CPlayerResult::GetList();
		std::list<CPlayerResult*>::iterator itr = list.GetEnd();	// 左チームの最後尾イテレーター
		while (list.ListLoop(itr))
		{
			// 勝敗チェック(モテ)
			CPlayerResult* pPlayer = (*itr);	// プレイヤー情報
			pPlayer->CheckVictoryContest();
		}

		SetState(EState::STATE_CONTEST);
	}
}

//==========================================================================
// モテ勝敗状態
//==========================================================================
void CResultManager::StateCharmContest(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	float START = 0.0f;
	float END = CameraTime::END_TIME[m_state];

	// カメラ補正
	CCamera* pCamera = GET_MANAGER->GetCamera();

	if (pCamera->GetState() == CCamera::STATE::STATE_FOLLOW)
	{
		float fDistance = UtilFunc::Correction::EasingLinear(pCamera->GetDistanceOrigin(), Contest::DISTANCE_CAMERA, START, END, m_fStateTime);
		MyLib::Vector3 rot = UtilFunc::Correction::EasingLinear(pCamera->GetOriginRotation(), Contest::ROT_CAMERA.at(m_teamContestWin), START, END, m_fStateTime);
		MyLib::Vector3 posR = UtilFunc::Correction::EasingLinear(pCamera->GetPositionROrigin(), Contest::POSR_CAMERA.at(m_teamContestWin), START, END, m_fStateTime);

		pCamera->SetDistance(fDistance);
		pCamera->SetRotation(rot);
		pCamera->SetPositionR(posR);
	}

	// 紙吹雪

}

//==========================================================================
// [開始]前座勝敗準備
//==========================================================================
void CResultManager::StateStartPreludeReady()
{
	CTexture* pTexture = CTexture::GetInstance();

	// 観客盛り下げ
	CAudience::SetEnableJumpAll(false, m_teamPreludeWin);

	// ポリゴン生成
	SAFE_KILL(m_pCrown);
	SAFE_KILL(m_pText);
	m_pText = CObject3D::Create();
	MyAssert::CustomAssert(m_pText != nullptr, "なんでポリゴン生成できてないんだよ");
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT3D);
	m_pText->SetSize(SIZE_POLY);

	MyLib::Vector3 pos = VEC3_ZERO;
	pos.y += Ready::POSY_WORD;
	m_pText->SetPosition(pos);

	m_pText->BindTexture(pTexture->Regist(TEXFILE_PRELUDE));

	// カメラ設定
	CCamera* pCamera = GET_MANAGER->GetCamera();

	pCamera->SetDistanceOrigin(pCamera->GetDistance());
	pCamera->SetOriginRotation(pCamera->GetRotation());
	pCamera->SetPositionROrigin(pCamera->GetPositionR());

	pCamera->SetDistanceDest(Ready::DISTANCE_CAMERA);
	pCamera->SetDestRotation(Ready::ROT_CAMERA);
	pCamera->SetPositionRDest(Ready::POSR_CAMERA);

	pCamera->SetState(CCamera::STATE::STATE_FOLLOW);
}

//==========================================================================
// [開始]前座勝敗
//==========================================================================
void CResultManager::StateStartPrelude()
{
	CTexture* pTexture = CTexture::GetInstance();

	// 観客盛り上げ
	CAudience::SetEnableJumpAll(true, m_teamPreludeWin);

	// モデル生成
	SAFE_KILL(m_pText);
	CreateCrown(m_teamPreludeWin);

	// カメラ設定
	CCamera* pCamera = GET_MANAGER->GetCamera();

	pCamera->SetDistanceOrigin(pCamera->GetDistance());
	pCamera->SetOriginRotation(pCamera->GetRotation());
	pCamera->SetPositionROrigin(pCamera->GetPositionR());

	pCamera->SetDistanceDest(Prelude::DISTANCE_CAMERA);
	pCamera->SetDestRotation(Prelude::ROT_CAMERA.at(m_teamPreludeWin));
	pCamera->SetPositionRDest(Prelude::POSR_CAMERA.at(m_teamPreludeWin));

	pCamera->SetState(CCamera::STATE::STATE_FOLLOW);
}

//==========================================================================
// [開始]モテ勝敗準備
//==========================================================================
void CResultManager::StateStartCharmContestReady()
{
	CTexture* pTexture = CTexture::GetInstance();

	// 観客盛り下げ
	CAudience::SetEnableJumpAll(false, m_teamPreludeWin);

	// ポリゴン生成
	SAFE_KILL(m_pCrown);
	SAFE_KILL(m_pText);
	m_pText = CObject3D::Create();
	MyAssert::CustomAssert(m_pText != nullptr, "なんでポリゴン生成できてないんだよ");
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT3D);
	m_pText->SetSize(SIZE_POLY);

	MyLib::Vector3 pos = VEC3_ZERO;
	pos.y += Ready::POSY_WORD;
	m_pText->SetPosition(pos);

	m_pText->BindTexture(pTexture->Regist(TEXFILE_CONTEST));

	// カメラ設定
	CCamera* pCamera = GET_MANAGER->GetCamera();

	pCamera->SetDistanceOrigin(pCamera->GetDistance());
	pCamera->SetOriginRotation(pCamera->GetRotation());
	pCamera->SetPositionROrigin(pCamera->GetPositionR());

	pCamera->SetDistanceDest(Ready::DISTANCE_CAMERA);
	pCamera->SetDestRotation(Ready::ROT_CAMERA);
	pCamera->SetPositionRDest(Ready::POSR_CAMERA);

	pCamera->SetState(CCamera::STATE::STATE_FOLLOW);
}

//==========================================================================
// [開始]モテ勝敗
//==========================================================================
void CResultManager::StateStartCharmContest()
{
	CTexture* pTexture = CTexture::GetInstance();

	//TAKADA: モテ値出す？

	// 観客NTR
	CAudience::SetNTRAll(m_teamContestWin);

	// 観客盛り上げ
	CAudience::SetEnableJumpAll(true, m_teamContestWin);

	// モデル生成
	SAFE_KILL(m_pText);
	CreateCrown(m_teamContestWin);

	// エフェクシア生成
	CreateEffect();

	// カメラ設定
	CCamera* pCamera = GET_MANAGER->GetCamera();

	pCamera->SetDistanceOrigin(pCamera->GetDistance());
	pCamera->SetOriginRotation(pCamera->GetRotation());
	pCamera->SetPositionROrigin(pCamera->GetPositionR());

	pCamera->SetDistanceDest(Contest::DISTANCE_CAMERA);
	pCamera->SetDestRotation(Contest::ROT_CAMERA.at(m_teamContestWin));
	pCamera->SetPositionRDest(Contest::POSR_CAMERA.at(m_teamContestWin));

	pCamera->SetState(CCamera::STATE::STATE_FOLLOW);
}

//==========================================================================
// [終了]前座勝敗
//==========================================================================
void CResultManager::StateEndPrelude()
{
	// 全観客沈黙
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		CAudience::SetEnableJumpAll(false, static_cast<CGameManager::ETeamSide>(i));
	}
}

//==========================================================================
// [終了]モテ勝敗
//==========================================================================
void CResultManager::StateEndCharmContest()
{
	// 全観客沈黙
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		CAudience::SetEnableJumpAll(false, static_cast<CGameManager::ETeamSide>(i));
	}
}

//==========================================================================
// 観客更新
//==========================================================================
void CResultManager::CreateAudience()
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		float fMoteRate = m_fCharmValue[i] / 100.0f;					// モテ割合	//TODO: マジックナンバー
		int nNumAudience = (int)(CAudience::MAX_WATCH * fMoteRate);		// 現在の観客数

		// 観客数を設定
		CAudience::SetNumWatch(nNumAudience, static_cast<CGameManager::ETeamSide>(i));
	}
}

//==========================================================================
// 王冠モデル生成
//==========================================================================
void CResultManager::CreateCrown(CGameManager::ETeamSide team)
{
	CResultCrown::EResult result = CResultCrown::EResult::RESULT_WIN;
	MyLib::Vector3 pos = VEC3_ZERO;
	pos.y += Prelude::POSY_CROWN;

	if (team == CGameManager::ETeamSide::SIDE_NONE)
	{// 引き分け
		pos += Draw::POS;
		result = CResultCrown::EResult::RESULT_DRAW;
	}
	else
	{// 勝利
		pos += POS_COURT[team];
		result = CResultCrown::EResult::RESULT_WIN;
	}

	SAFE_KILL(m_pCrown);
	m_pCrown = CResultCrown::Create(result);
	MyAssert::CustomAssert(m_pCrown != nullptr, "なんでモデル生成できてないんだよ");
	m_pCrown->SetType(CObject::TYPE::TYPE_OBJECTX);

	m_pCrown->SetPosition(pos);
	m_pCrown->SetOriginPosition(pos);
	m_pCrown->SetState(CResultCrown::EState::STATE_LOOP);
}

//==========================================================================
// 勝敗ポリゴン生成
//==========================================================================
void CResultManager::CreatePolygon(EState state)
{
	CTexture* pTexture = CTexture::GetInstance();

	SAFE_KILL(m_pText);
	m_pText = CObject3D::Create();
	MyAssert::CustomAssert(m_pText != nullptr, "なんでポリゴン生成できてないんだよ");
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT3D);
	m_pText->SetSize(SIZE_POLY);

	MyLib::Vector3 pos = VEC3_ZERO;
	pos.y += Ready::POSY_WORD;
	m_pText->SetPosition(pos);

	std::string filepass = TEXFILE_PRELUDE;

	switch (state)
	{
	case CResultManager::STATE_PRELUDE_READY:
		filepass = TEXFILE_PRELUDE;
		break;

	case CResultManager::STATE_CONTEST_READY:
		filepass = TEXFILE_CONTEST;
		break;

	default:
		break;
	}
	
	m_pText->BindTexture(pTexture->Regist(filepass));
}

//==========================================================================
// エフェクト生成
//==========================================================================
void CResultManager::CreateEffect()
{
	MyLib::Vector3 pos;
	if (m_teamContestWin == CGameManager::ETeamSide::SIDE_NONE)
	{// 引き分け
		pos += Draw::POS;
	}
	else
	{// 勝利
		pos += POS_COURT[m_teamContestWin];
	}
	pos.y = 0.0f;

	// 生成
	m_pEfkConfetti = CEffekseerObj::Create
	(
		CMyEffekseer::EEfkLabel::EFKLABEL_CONFETTI,
		pos,
		MyLib::Vector3(),
		MyLib::Vector3(),
		10.0f,
		false
	);
}

//==========================================================================
// 試合情報読み込み
//==========================================================================
void CResultManager::Load()
{
	// ファイルを開く
	std::ifstream File(ResultManager::TEXTFILE);
	if (!File.is_open()) {
		return;
	}

	// コメント用
	std::string hoge;

	int winteam = 0;
	int i = 0;

	// データ読み込み
	std::string line;
	while (std::getline(File, line))
	{
		// コメントはスキップ
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("PRELUDE_WIN") != std::string::npos)
		{// 前座勝利

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				winteam;	// 勝利チーム

			m_teamPreludeWin = static_cast<CGameManager::ETeamSide>(winteam);
		}

		if (line.find("CONTEST_WIN") != std::string::npos)
		{// モテ勝利

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				winteam;	// 勝利チーム

			m_teamContestWin = static_cast<CGameManager::ETeamSide>(winteam);
		}

		if (line.find("TENSION") != std::string::npos)
		{// 盛り上がり値

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				m_fTension;	// 盛り上がり値
		}

		if (line.find("SETTEAM") != std::string::npos)
		{// チーム情報
			LoadTeam(&File, line, i);
	
			i++;
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// ファイルを閉じる
	File.close();
}

//==========================================================================
// チームステータス読み込み
//==========================================================================
void CResultManager::LoadTeam(std::ifstream* File, std::string line, int nTeam)
{
	// コメント用
	std::string hoge;
	int j = 0;	// プレイヤー人数

	while (std::getline(*File, line))
	{
		// コメントはスキップ
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("END_SETTEAM") != std::string::npos)
		{// 終了
			break;
		}

		if (line.find("CHARMVALUE") != std::string::npos)
		{// モテ値

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				m_fCharmValue[nTeam];	// モテ値
		}

	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CResultManager::SetState(EState state)
{
	// 状態更新
	if (m_StateEndFunc[state] != nullptr)
	{
		(this->*(m_StateEndFunc[state]))();
	}

	m_state = state;
	m_fStateTime = 0.0f;

	// 状態更新
	if (m_StateStartFunc[m_state] != nullptr)
	{
		(this->*(m_StateStartFunc[m_state]))();
	}
}

//==========================================================================
// コートの中心位置取得
//==========================================================================
MyLib::Vector3 CResultManager::GetPosMid(CGameManager::ETeamSide team)
{
	return POS_COURT[team];
}

//==========================================================================
// デバッグ
//==========================================================================
void CResultManager::Debug()
{
#if _DEBUG

	if (ImGui::TreeNode("Audience"))
	{
		for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
		{
			std::string lavel = "MoteValue";
			lavel += i;

			// チームのモテ値増減
			ImGui::DragFloat(lavel.c_str(), &m_fCharmValue[i], 0.1f, 0.0f, 100.0f, "%.2f");	// モテ値の変動操作
			float fMoteRate = m_fCharmValue[i] / 100.0f;					// モテ割合	//TODO: マジックナンバー
			int nNumAudience = (int)(CAudience::MAX_WATCH * fMoteRate);		// 現在の観客数

			// 観客数を設定
			CAudience::SetNumWatch(nNumAudience, static_cast<CGameManager::ETeamSide>(i));

			// 観客数を設定
			int num = CAudience::GetNumWatchAll(static_cast<CGameManager::ETeamSide>(i));

			ImGui::Text("NumWatch%d: %d", i, num);
		}

		// 位置設定
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("State"))
	{
		CCamera* pCamera = GET_MANAGER->GetCamera();
		float fDistance = pCamera->GetDistance();

		// 現在の状態
		ImGui::Text("state: %s", magic_enum::enum_name(m_state));
	
		// 遷移可能フラグ
		ImGui::Checkbox("Trans", &m_bStateTrans);

		// 勝敗反転
		if (ImGui::Button("Frag:prelude"))
		{
			m_teamPreludeWin = CGameManager::GetInstance()->RivalTeam(m_teamPreludeWin);
		}

		if (ImGui::Button("Frag:contest"))
		{
			m_teamContestWin = CGameManager::GetInstance()->RivalTeam(m_teamContestWin);
		}
		
		// 状態設定
		for (int i = 0; i < CResultManager::EState::STATE_MAX; i++)
		{
			CResultManager::EState state = static_cast<EState>(i);
			std::string str = magic_enum::enum_name(state).data();
			if (ImGui::Button(str.c_str()))
			{
				SetState(state);
			}
		}

		// 位置設定
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("crown"))
	{
		if (m_pCrown!= nullptr)
		{
			MyLib::Vector3 pos = m_pCrown->GetPosition();
			ImGui::DragFloat3("pos", (float*)&pos, 1.0f, 0.0f, 0.0f, "%.2f");
			m_pCrown->SetPosition(pos);
		}

		// 位置設定
		ImGui::TreePop();
	}

#endif
}
