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

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	//const std::string TEXFILE_WIN = "data\\TEXTURE\\result\\win.png";		// 勝ち
	const std::string TEXFILE_WIN = "data\\TEXTURE\\result\\draw.jpg";		// 勝ち
	const MyLib::Vector2 SIZE_POLY = MyLib::Vector2(100.0f, 100.0f);
	const float POSY_POLY = 100.0f;

	const MyLib::Vector3 ROT_CAMERA = MyLib::Vector3(0.0f, 0.0f, -0.36f);
	const float DISTANCE_CAMERA = 1940.0f;

	const float POSR_HEIGHT = 100.0f;	// カメラの注視点の高さ
	const MyLib::Vector3 POS_COURT[CGameManager::ETeamSide::SIDE_MAX] =	// 位置
	{
		MyLib::Vector3(-475.0f, 0.0f, 0.0f),	// 左
		MyLib::Vector3(475.0f, 0.0f, 0.0f),		// 右
	};
}

// 状態時間
namespace StateTime
{
	const float NONE(5.0f);		// なし
	const float PRELUDE(5.0f);	// 前座
}

namespace Draw
{// 引き分け
	const MyLib::Vector3 POS = MyLib::Vector3(0.0f, 100.0f, 0.0f);
}

namespace Prelude
{
	const MyLib::Vector3 POSR_CAMERA[] =
	{
		MyLib::Vector3(150.0f, POSR_HEIGHT, 0.0f)	+ POS_COURT[0],
		MyLib::Vector3(-150.0f, POSR_HEIGHT, 0.0f)	+ POS_COURT[1],
	};

	const MyLib::Vector3 ROT_CAMERA[] =
	{
		MyLib::Vector3(0.0f, -0.17f, -0.36f),
		MyLib::Vector3(0.0f, 0.17f, -0.36f),
	};

	const float DISTANCE_CAMERA = 1940.0f;
}

namespace Contest
{
	const MyLib::Vector3 POSR_CAMERA[] =
	{
		MyLib::Vector3(-400.0f, 0.0f, 240.0f)	+ POS_COURT[0],
		MyLib::Vector3(400.0f, 0.0f, 240.0f)	+ POS_COURT[1],
	};

		const MyLib::Vector3 ROT_CAMERA[] =
	{
		MyLib::Vector3(0.0f, 0.32f, -0.36f),
		MyLib::Vector3(0.0f, -0.32f, -0.36f),
	};

	const float DISTANCE_CAMERA = 1500.0f;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CResultManager::STATE_FUNC CResultManager::m_StateFunc[] =				// 状態関数
{
	& CResultManager::StateNone,				// なし
	& CResultManager::StatePrelude,				// 前座勝敗
	& CResultManager::StateCharmContest,		// モテ勝敗
};

CResultManager::STATE_START_FUNC CResultManager::m_StateStartFunc[] =	// 状態開始
{
	nullptr,									// なし
	&CResultManager::StateStartPrelude,			// 前座勝敗
	&CResultManager::StateStartCharmContest,	// モテ勝敗
};

CResultManager::STATE_END_FUNC CResultManager::m_StateEndFunc[] =		// 状態終了
{
	nullptr,									// なし
	&CResultManager::StateEndPrelude,			// 前座勝敗
	&CResultManager::StateEndCharmContest,		// モテ勝敗
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
	m_pCrown = nullptr;												// 勝利チーム用3Dポリゴン
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
	CPlayerManager::Create(CScene::MODE::MODE_RESULT);

	// 観客生成
	CreateAudience();

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

	if (m_pCrown != nullptr)
	{
		CEffect3D::Create(
			m_pCrown->GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			100.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}


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
	if (m_fStateTime >= StateTime::NONE)
	{
		SetState(EState::STATE_PRELUDE);
	}
}

//==========================================================================
// 前座勝敗状態
//==========================================================================
void CResultManager::StatePrelude(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	const float START = 0.0f;
	const float END = 1.0f;

	// カメラ補正
	CCamera* pCamera = GET_MANAGER->GetCamera();

	if (pCamera->GetState() == CCamera::STATE::STATE_FOLLOW)
	{
		float fDistance = UtilFunc::Correction::EasingLinear(pCamera->GetDistanceOrigin(), Prelude::DISTANCE_CAMERA, START, END, m_fStateTime);
		MyLib::Vector3 rot = UtilFunc::Correction::EasingLinear(pCamera->GetOriginRotation(), Prelude::ROT_CAMERA[m_teamPreludeWin], START, END, m_fStateTime);
		MyLib::Vector3 posR = UtilFunc::Correction::EasingLinear(pCamera->GetPositionROrigin(), Prelude::POSR_CAMERA[m_teamPreludeWin], START, END, m_fStateTime);

		pCamera->SetDistance(fDistance);
		pCamera->SetRotation(rot);
		pCamera->SetPositionR(posR);

		// 無意味かも
		pCamera->SetDestRotation(Prelude::ROT_CAMERA[m_teamPreludeWin]);
	}

	// 指定時間を過ぎたら
	if (m_fStateTime >= StateTime::PRELUDE)
	{
		SetState(EState::STATE_CONTEST);
	}
}

//==========================================================================
// モテ勝敗状態
//==========================================================================
void CResultManager::StateCharmContest(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	const float START = 0.0f;
	const float END = 1.0f;

	// カメラ補正
	CCamera* pCamera = GET_MANAGER->GetCamera();

	if (pCamera->GetState() == CCamera::STATE::STATE_FOLLOW)
	{
		float fDistance = UtilFunc::Correction::EasingLinear(pCamera->GetDistanceOrigin(), Contest::DISTANCE_CAMERA, START, END, m_fStateTime);
		MyLib::Vector3 rot = UtilFunc::Correction::EasingLinear(pCamera->GetOriginRotation(), Contest::ROT_CAMERA[m_teamContestWin], START, END, m_fStateTime);
		MyLib::Vector3 posR = UtilFunc::Correction::EasingLinear(pCamera->GetPositionROrigin(), Contest::POSR_CAMERA[m_teamContestWin], START, END, m_fStateTime);

		pCamera->SetDistance(fDistance);
		pCamera->SetRotation(rot);
		pCamera->SetPositionR(posR);

		// 無意味かも
		pCamera->SetDestRotation(Contest::ROT_CAMERA[m_teamContestWin]);
	}
}

//==========================================================================
// [開始]前座勝敗
//==========================================================================
void CResultManager::StateStartPrelude()
{
	CTexture* pTexture = CTexture::GetInstance();

	// 観客盛り上げ
	CAudience::SetEnableJumpAll(true, m_teamPreludeWin);

	// ポリゴン生成
	SAFE_UNINIT(m_pCrown);
	m_pCrown = CObject3D::Create();
	MyAssert::CustomAssert(m_pCrown != nullptr, "なんでポリゴン生成できてないんだよ");
	m_pCrown->SetType(CObject::TYPE::TYPE_OBJECT3D);


	MyLib::Vector3 pos = VEC3_ZERO;
	m_pCrown->SetSize(SIZE_POLY);

	if (m_teamPreludeWin == CGameManager::ETeamSide::SIDE_NONE)
	{// 引き分け
		pos = Draw::POS;
		pos.y += POSY_POLY;
		m_pCrown->SetPosition(pos);
		m_pCrown->BindTexture(pTexture->Regist(TEXFILE_WIN));
	}
	else
	{// 勝利
		pos = POS_COURT[m_teamPreludeWin];
		pos.y += POSY_POLY;
		m_pCrown->SetPosition(pos);
		m_pCrown->BindTexture(pTexture->Regist(TEXFILE_WIN));
	}

	// カメラ設定
	CCamera* pCamera = GET_MANAGER->GetCamera();

	pCamera->SetDistanceOrigin(pCamera->GetDistance());
	pCamera->SetOriginRotation(pCamera->GetRotation());
	pCamera->SetPositionROrigin(pCamera->GetPositionR());

	pCamera->SetDistanceDest(Prelude::DISTANCE_CAMERA);
	pCamera->SetDestRotation(Prelude::ROT_CAMERA[m_teamPreludeWin]);
	pCamera->SetPositionRDest(Prelude::POSR_CAMERA[m_teamPreludeWin]);

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

	// ポリゴン生成
	SAFE_UNINIT(m_pCrown);
	m_pCrown = CObject3D::Create();
	MyAssert::CustomAssert(m_pCrown != nullptr, "なんでポリゴン生成できてないんだよ");

	MyLib::Vector3 pos = VEC3_ZERO;
	m_pCrown->SetSize(SIZE_POLY);

	if (m_teamContestWin == CGameManager::ETeamSide::SIDE_NONE)
	{// 引き分け
		pos = Draw::POS;
		pos.y += POSY_POLY;
		m_pCrown->SetPosition(pos);
		m_pCrown->BindTexture(pTexture->Regist(TEXFILE_WIN));
	}
	else
	{// 勝利
		pos = POS_COURT[m_teamContestWin];
		pos.y += POSY_POLY;
		m_pCrown->SetPosition(pos);
		m_pCrown->BindTexture(pTexture->Regist(TEXFILE_WIN));
	}

	// カメラ設定
	CCamera* pCamera = GET_MANAGER->GetCamera();

	pCamera->SetDistanceOrigin(pCamera->GetDistance());
	pCamera->SetOriginRotation(pCamera->GetRotation());
	pCamera->SetPositionROrigin(pCamera->GetPositionR());

	pCamera->SetDistanceDest(Contest::DISTANCE_CAMERA);
	pCamera->SetDestRotation(Contest::ROT_CAMERA[m_teamContestWin]);
	pCamera->SetPositionRDest(Contest::POSR_CAMERA[m_teamContestWin]);

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

		if (ImGui::Button("Frag:prelude"))
		{
			m_teamPreludeWin = CGameManager::GetInstance()->RivalTeam(m_teamPreludeWin);
		}

		if (ImGui::Button("Frag:contest"))
		{
			m_teamContestWin = CGameManager::GetInstance()->RivalTeam(m_teamContestWin);
		}

		if (ImGui::Button("prelude"))
		{
			SetState(EState::STATE_PRELUDE);
		}

		if (ImGui::Button("contest"))
		{
			SetState(EState::STATE_CONTEST);
		}

		// 位置設定
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("crown"))
	{
		if (m_pCrown != nullptr)
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
