//=============================================================================
// 
// リザルトマネージャ処理 [resultmanager.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "resultmanager.h"
#include "result.h"

#include "playerResult.h"
#include "audience.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const MyLib::Vector2 SIZE_POLY = MyLib::Vector2(100.0f, 100.0f);
}

// 状態時間
namespace StateTime
{
	const float PRELUDE(5.0f);	// 前座
}

namespace Draw
{// 引き分け
	const MyLib::Vector3 POS_PRELUDE = MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.4f, 0.0f);
	const MyLib::Vector3 POS_CONTEST = MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.6f, 0.0f);
	const std::string TEXFILE = "data\\TEXTURE\\result\\draw.jpg";
}

namespace Prelude
{
	const std::string TEXFILE_LOSE = "data\\TEXTURE\\result\\prelude_lose.jpg";		// 負け
	const std::string TEXFILE_WIN = "data\\TEXTURE\\result\\prelude_win.jpg";		// 勝ち

	const MyLib::Vector3 POS_POLY[CGameManager::ETeamSide::SIDE_MAX] =
	{
		MyLib::Vector3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.4f, 0.0f),
		MyLib::Vector3(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.4f, 0.0f),
	};
}

namespace Contest
{
	const std::string TEXFILE_LOSE = "data\\TEXTURE\\result\\contest_lose.jpg";		// 負け
	const std::string TEXFILE_WIN = "data\\TEXTURE\\result\\contest_win.jpg";		// 勝ち

	const MyLib::Vector3 POS_POLY[CGameManager::ETeamSide::SIDE_MAX] =
	{
		MyLib::Vector3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.6f, 0.0f),
		MyLib::Vector3(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.6f, 0.0f),
	};
}

// プレイヤー
namespace Player
{
	const float INIT_IN_X	(100.0f);	// 初期位置(内野)
	const float INIT_IN_Z	(100.0f);	// 初期位置(内野)
	const float INIT_OUT_X	(100.0f);	// 初期位置(外野)
	const float INIT_OUT_Z	(300.0f);	// 初期位置(外野)
	const float SHIFT	(50.0f);	// ずらし値

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
// 関数ポインタ
//==========================================================================
CResultManager::STATE_FUNC CResultManager::m_StateFunc[] =	// 状態関数
{
	& CResultManager::StateNone,				// なし
	& CResultManager::StatePrelude,				// 前座勝敗
	& CResultManager::StateCharmContest,		// モテ勝敗
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
	//チームステータス＆プレイヤー生成
	Load();

	// 観客生成
	CreateAudience();

	// 前座勝敗ポリゴン生成
	CreatePrelude();

	// モテ勝敗ポリゴン生成
	CreateCharmContest();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CResultManager::Uninit()
{
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

	// 最初勝利チームドーン
	// その後モテ値勝利ドーン
	// 盛り上がり値ドーン


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
	// タイマーリセット
	m_fStateTime = 0.0f;
}

//==========================================================================
// 勝利
//==========================================================================
void CResultManager::StatePrelude(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 指定時間を過ぎたら
	if (m_fStateTime >= StateTime::PRELUDE)
	{
		SetState(EState::STATE_CONTEST);
	}
}

//==========================================================================
// 敗北モーション
//==========================================================================
void CResultManager::StateCharmContest(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 前座勝敗生成
//==========================================================================
void CResultManager::CreatePrelude()
{
	CTexture* pTexture = CTexture::GetInstance();

	if (m_teamPreludeWin == CGameManager::ETeamSide::SIDE_NONE)
	{// 引き分けだったら
		CObject2D* pObj = CObject2D::Create();

		pObj->SetPosition(Draw::POS_PRELUDE);
		pObj->SetSize(SIZE_POLY);
		pObj->BindTexture(pTexture->Regist(Draw::TEXFILE));
		return;
	}

	// ポリゴン生成
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		CObject2D* pObj = CObject2D::Create();
	
		pObj->SetPosition(Prelude::POS_POLY[i]);
		pObj->SetSize(SIZE_POLY);

		if (m_teamPreludeWin == static_cast<CGameManager::ETeamSide>(i))
		{// 勝利チームの場合
			pObj->BindTexture(pTexture->Regist(Prelude::TEXFILE_WIN));
		}
		else
		{
			pObj->BindTexture(pTexture->Regist(Prelude::TEXFILE_LOSE));
		}
	}
}

//==========================================================================
// モテ勝敗生成
//==========================================================================
void CResultManager::CreateCharmContest()
{
	CTexture* pTexture = CTexture::GetInstance();

	if (m_teamContestWin == CGameManager::ETeamSide::SIDE_NONE)
	{// 引き分けだったら
		CObject2D* pObj = CObject2D::Create();

		pObj->SetPosition(Draw::POS_CONTEST);
		pObj->SetSize(SIZE_POLY);
		pObj->BindTexture(pTexture->Regist(Draw::TEXFILE));
		return;
	}

	//TAKADA: モテ値出す？
	// ポリゴン生成
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		CObject2D* pObj = CObject2D::Create();

		pObj->SetPosition(Contest::POS_POLY[i]);
		pObj->SetSize(SIZE_POLY);

		if (m_teamContestWin == static_cast<CGameManager::ETeamSide>(i))
		{// 勝利チームの場合
			pObj->BindTexture(pTexture->Regist(Contest::TEXFILE_WIN));
		}
		else
		{
			pObj->BindTexture(pTexture->Regist(Contest::TEXFILE_LOSE));
		}
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

		if (line.find("SETPLAYER") != std::string::npos)
		{// プレイヤー情報

			LoadPlayer(File, line, nTeam, j);

			j++;	// 人数加算
		}
	}
}

//==========================================================================
// チームステータス読み込み
//==========================================================================
void CResultManager::LoadPlayer(std::ifstream* File, std::string line, int nTeam, int nIdxPlayer)
{
	// コメント用
	std::string hoge;

	// チーム
	CGameManager::ETeamSide team = static_cast<CGameManager::ETeamSide>(nTeam);

	CPlayer::EHandedness eHanded = CPlayer::EHandedness::HAND_R;	// 利き手
	CPlayer::EBody eBody = CPlayer::EBody::BODY_NORMAL;				// 体型
	int nHair = -1;													// 髪
	int nAccessory = -1;											// アクセサリー
	int nFace = -1;													// 顔

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

			eHanded = static_cast<CPlayer::EHandedness>(hand);
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

			eBody = static_cast<CPlayer::EBody>(body);
		}

		if (line.find("HAIR") != std::string::npos)
		{// 髪

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				nHair;		// 髪
		}

		if (line.find("ACCESSORY") != std::string::npos)
		{// アクセサリー

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				nHair;		// アクセサリー
		}

		if (line.find("FACE") != std::string::npos)
		{// 顔

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				nFace;		// 顔
		}
	}

	// 位置
	MyLib::Vector3 pos = Player::POS_OUT[team];
	pos += (Player::POS_SHIFT[team] * nIdxPlayer);

	// プレイヤー生成
	CPlayer* player = CPlayer::Create(pos, team, CPlayer::EHuman::HUMAN_RESULT, eBody, eHanded);
}

//==========================================================================
// 状態設定
//==========================================================================
void CResultManager::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
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
			// 観客数を設定
			int num = CAudience::GetNumWatchAll(static_cast<CGameManager::ETeamSide>(i));
	
			ImGui::Text("NumWatch%d: %d", i, num);
		}

		// 位置設定
		ImGui::TreePop();
	}


#endif
}
