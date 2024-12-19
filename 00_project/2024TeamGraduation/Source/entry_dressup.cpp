//=============================================================================
// 
//  ドレスアップ設定処理 [entry_dressup.cpp]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//=============================================================================
#include "entry_dressup.h"
#include "entry_setupTeam.h"
#include "playerManager.h"
#include "dressupUI.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTFILE	= "data\\TEXT\\entry\\setupTeam.txt";
	const std::string TOP_LINE	= "#==============================================================================";	// テキストのライン
	const std::string TEXT_LINE	= "#------------------------------------------------------------------------------";	// テキストのライン
}

//==========================================================================
// コンストラクタ
//==========================================================================
CEntry_Dressup::CEntry_Dressup() : CEntryScene()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CEntry_Dressup::~CEntry_Dressup()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEntry_Dressup::Init()
{
	// 前回の着せ替え読込
	Load();

	// チームセットアップ情報の取得
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) { return E_FAIL; }

	int nNumPlayer = pSetupTeam->GetAllPlayerNum();	// プレイヤー人数
	for (int i = 0; i < nNumPlayer; i++)
	{ // プレイヤー人数分繰り返す

		// TODO
		MyLib::Vector3 pos = MyLib::Vector3(160.0f, 450.0f, 0.0f);
		MyLib::Vector3 offset = MyLib::Vector3(320.0f * (float)i, 0.0f, 0.0f);

		// ドレスアップUIの生成
		CDressupUI* pDressup = CDressupUI::Create(i, pos + offset);
		if (pDressup == nullptr) { return E_FAIL; }

		// ドレスアップUIの追加
		m_vecDressInfo.push_back(pDressup);
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEntry_Dressup::Uninit()
{
	for (auto& info : m_vecDressInfo)
	{
		// ドレスアップUIの終了
		SAFE_UNINIT(info);
	}

	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CEntry_Dressup::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親の更新
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// チーム等設定
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) return;

	// 今回のサイズ
	for (auto& info : m_vecDressInfo)
	{
		// ドレスアップUIの更新
		info->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// 一旦シーン切り替え TODO : 全員チェックしてたらとかにする
	if (pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_X))
	{
		// セーブ処理
		Save();

		CEntry::GetInstance()->ChangeEntryScene(CEntry::ESceneType::SCENETYPE_GAMESETTING);
	}
}

//==========================================================================
// セーブ処理
//==========================================================================
void CEntry_Dressup::Save()
{
	// プレイヤーマネージャ取得
	CPlayerManager* pPlayerMgr = CPlayerManager::GetInstance();

	// 読み込み情報
	std::vector<CPlayerManager::LoadInfo> vecSaveInfo[CGameManager::ETeamSide::SIDE_MAX];

	// 今回のサイズ
	int size = static_cast<int>(m_vecDressInfo.size());
	for (int i = 0; i < size; i++)
	{
		// 自身の操作するインデックス番号取得
		int controllIdx = m_vecDressInfo[i]->GetMyPlayerIdx();
		CGameManager::ETeamSide side = m_vecDressInfo[i]->GetTeam();

		// 読み込み情報
		vecSaveInfo[side].emplace_back();
		CPlayerManager::LoadInfo* pLoadInfo = &vecSaveInfo[side].back();
		pLoadInfo->nControllIdx = controllIdx;								// 自身の操作するインデックス番号取得
		pLoadInfo->nHair = m_vecDressInfo[i]->GetHairNowIdx();				// 髪のインデックス番号
		pLoadInfo->nAccessory = m_vecDressInfo[i]->GetAccessoryNowIdx();	// アクセのインデックス番号
		pLoadInfo->nFace = m_vecDressInfo[i]->GetFaceNowIdx();				// 顔のインデックス番号
		pLoadInfo->eBody = m_vecDressInfo[i]->GetBodyType();				// 体型
		pLoadInfo->eHanded = m_vecDressInfo[i]->GetHandedness();			// 利き手
	}

	// セーブ処理
	pPlayerMgr->Save(vecSaveInfo[CGameManager::ETeamSide::SIDE_LEFT], vecSaveInfo[CGameManager::ETeamSide::SIDE_RIGHT]);
}

//==========================================================================
// ロード
//==========================================================================
void CEntry_Dressup::Load()
{
	// ファイルを開く
	std::ifstream File(TEXTFILE);
	if (!File.is_open()) {
		return;
	}

	// コメント用
	std::string hoge;

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

		if (line.find("PLAYERNUM") != std::string::npos)
		{// PLAYERNUMでチーム毎のプレイヤー数読み込み

			// ストリーム作成
			std::istringstream lineStream(line);
			
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
// デバッグ処理
//==========================================================================
void CEntry_Dressup::Debug()
{
	if (ImGui::TreeNode("SetUpTeam"))
	{
		//=============================
		// セーブ
		//=============================s
		if (ImGui::Button("Save"))
		{
			Save();
		}

		// 今回のサイズ
		int size = static_cast<int>(m_vecDressInfo.size());
		for (int i = 0; i < size; i++)
		{
			std::string treename = "Info : " + std::to_string(i);	// ツリー名
			if (ImGui::TreeNode(treename.c_str()))
			{
				const auto& info = m_vecDressInfo[i];

				ImGui::Text("editType[%s]", magic_enum::enum_name(info->GetTypeEdit()));
				ImGui::Text("changeType[%s]", magic_enum::enum_name(info->GetTypeChange()));
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}
