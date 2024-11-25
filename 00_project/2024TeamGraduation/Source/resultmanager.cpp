//=============================================================================
// 
// リザルトマネージャ処理 [resultmanager.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "resultmanager.h"
#include "result.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// 静的メンバ変数
//==========================================================================
CResultManager* CResultManager::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CResultManager::CResultManager()
{
	m_fTension = 0.0f;										// 盛り上がり値
	m_teamWin = CGameManager::ETeamSide::SIDE_NONE;			// 勝利チーム
	memset(&m_fCharmValue[0], 0, sizeof(m_fCharmValue));	// モテ値
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
	// 最初勝利チームドーン
	// その後モテ値勝利ドーン
	// 盛り上がり値ドーン




#if _DEBUG	// デバッグ処理

	if (ImGui::TreeNode("GameManager"))
	{
		Debug();
		ImGui::TreePop();
	}
#endif
}

//==========================================================================
// チームステータス読み込み
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

		if (line.find("WIN") != std::string::npos)
		{// PLAYERNUMでチーム毎のプレイヤー数読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				winteam;	// 勝利チーム

			m_teamWin = static_cast<CGameManager::ETeamSide>(winteam);
		}

		if (line.find("TENSION") != std::string::npos)
		{// PLAYERNUMでチーム毎のプレイヤー数読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				m_fTension;	// 盛り上がり値
		}

		if (line.find("CHARMVALUE") != std::string::npos)
		{// PLAYERNUMでチーム毎のプレイヤー数読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				m_fCharmValue[i];	// モテ値

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
// デバッグ
//==========================================================================
void CResultManager::Debug()
{
#if _DEBUG

	// タイマー

	if (ImGui::TreeNode("Timer"))
	{
		ImGui::Text("bStop");

		ImGui::TreePop();
	}

#endif
}
