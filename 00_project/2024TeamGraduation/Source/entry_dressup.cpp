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

	namespace ui
	{
		namespace left
		{
			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, VEC3_SCREEN_CENT.y, 0.0f);	// 左中心位置
			const float OFFSET_X = 210.0f;	// X座標オフセット
		}

		namespace right
		{
			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, VEC3_SCREEN_CENT.y, 0.0f);	// 右中心位置
			const float OFFSET_X = 210.0f;	// X座標オフセット
		}
	}
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

	int nCurLeft = 0;	// 現在の左プレイヤー数
	int nCurRight = 0;	// 現在の右プレイヤー数
	int nMaxLeft = pSetupTeam->GetPlayerNum(CGameManager::ETeamSide::SIDE_LEFT);	// 左プレイヤー総数
	int nMaxRight = pSetupTeam->GetPlayerNum(CGameManager::ETeamSide::SIDE_RIGHT);	// 右プレイヤー総数
	for (int i = 0; i < CGameManager::MAX_PLAYER; i++)
	{ // プレイヤー人数分繰り返す

		// チーム指定がない場合次へ
		CGameManager::ETeamSide team = pSetupTeam->GetTeamSide(i);	// チームサイド
		if (team == CGameManager::ETeamSide::SIDE_NONE) { continue; }

		CDressupUI* pDressup = nullptr;	// ドレスアップUI情報
		MyLib::Vector3 posUI;	// UI位置
		switch (team)
		{ // チームごとの処理
		case CGameManager::ETeamSide::SIDE_LEFT:
		{
			// UIの位置を計算
			posUI = ui::left::POS;
			posUI.x = ui::left::POS.x - (ui::left::OFFSET_X * (float)(nMaxLeft - 1)) * 0.5f + (ui::left::OFFSET_X * (float)nCurLeft);

			// チームメンバー数を加算
			nCurLeft++;
			break;
		}
		case CGameManager::ETeamSide::SIDE_RIGHT:
		{
			// UIの位置を計算
			posUI = ui::right::POS;
			posUI.x = ui::right::POS.x - (ui::right::OFFSET_X * (float)(nMaxRight - 1)) * 0.5f + (ui::right::OFFSET_X * (float)nCurRight);

			// チームメンバー数を加算
			nCurRight++;
			break;
		}
		default:
			assert(false);
			break;
		}

		// ドレスアップUIの生成
		pDressup = CDressupUI::Create(i, posUI);
		if (pDressup == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}

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
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CEntry_Dressup::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親の更新
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ゲーム設定遷移
	TransSetting();
}

//==========================================================================
// ゲーム設定遷移
//==========================================================================
void CEntry_Dressup::TransSetting()
{
	// 準備完了できていない場合抜ける
	if (!IsAllReady()) { return; }

	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報
	if (pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_X))
	{
		// セーブ処理
		Save();

		for (auto& rInfo : m_vecDressInfo)
		{ // 要素数分繰り返す

			// ドレスアップUIの終了
			SAFE_UNINIT(rInfo);
		}

		// ゲーム設定シーンへ遷移
		CEntry::GetInstance()->ChangeEntryScene(CEntry::ESceneType::SCENETYPE_GAMESETTING);
	}
}

//==========================================================================
// 準備全完了フラグの取得処理
//==========================================================================
bool CEntry_Dressup::IsAllReady()
{
	for (auto& rInfo : m_vecDressInfo)
	{ // 要素数分繰り返す

		// 準備が終わっている場合次へ
		if (rInfo->IsReady()) { continue; }

		// 準備未完了を返す
		return false;
	}

	// 準備全完了を返す
	return true;
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
