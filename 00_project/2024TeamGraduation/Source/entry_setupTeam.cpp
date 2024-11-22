//=============================================================================
// 
// チーム等設定処理 [entry_setupTeam.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "entry_setupTeam.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTFILE = "data\\TEXT\\entry\\setupTeam.txt";
	const std::string TOP_LINE = "#==============================================================================";	// テキストのライン
	const std::string TEXT_LINE = "#------------------------------------------------------------------------------";	// テキストのライン
}

//==========================================================================
// コンストラクタ
//==========================================================================
CEntry_SetUpTeam::CEntry_SetUpTeam() : CEntryScene()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CEntry_SetUpTeam::~CEntry_SetUpTeam()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEntry_SetUpTeam::Init()
{
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEntry_SetUpTeam::Uninit()
{
}

//==========================================================================
// 更新処理
//==========================================================================
void CEntry_SetUpTeam::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, i))
		{// エントリー、コントローラー生成


		}

		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, i))
		{// チーム決定

			m_TeamSide[i];		// チームサイド
		}
	}

}

//==========================================================================
// セーブ処理
//==========================================================================
void CEntry_SetUpTeam::Save()
{
	// ファイルを開く
	std::ofstream File(TEXTFILE);
	if (!File.is_open()) {
		return;
	}

	// テキストファイル名目次
	File << TOP_LINE			<< std::endl;
	File << "# チーム等設定"	<< std::endl;
	File << TOP_LINE			<< std::endl;


	// サイド毎の人数
	File << TEXT_LINE			<< std::endl;
	File << "# プレイヤー数"	<< std::endl;
	File << TEXT_LINE			<< std::endl;
	File << "PLAYERNUM = "	<< 
		m_vecPlayerNum[CGameManager::ETeamSide::SIDE_LEFT] << 
		" " <<
		m_vecPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT] << std::endl;
	
	File << "" << std::endl;


	// プレイヤーインデックス
	File << TEXT_LINE << std::endl;
	File << "# プレイヤーインデックス" << std::endl;
	File << TEXT_LINE << std::endl;
	
	// 左
	File << "LEFT_ID = ";
	for (int i = 0; i < CGameManager::MAX_SIDEPLAYER; i++)
	{
		if (static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size()) - 1 >= i)
		{// サイズ内
			File << m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][i];
		}
		else
		{
			File << -1;
		}
		File << " ";

	}
	File << std::endl;

	// 右
	File << "RIGHT_ID = ";
	for (int i = 0; i < CGameManager::MAX_SIDEPLAYER; i++)
	{
		if (static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size()) - 1 >= i)
		{// サイズ内
			File << m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][i];
		}
		else
		{
			File << -1;
		}
		File << " ";
	}
	File << std::endl;


	// ファイルを閉じる
	File.close();
}

//==========================================================================
// デバッグ処理
//==========================================================================
void CEntry_SetUpTeam::Debug()
{
	//=============================
	// セーブ
	//=============================s
	if (ImGui::Button("Save"))
	{
		Save();
	}

	//=============================
	// 総数変更
	//=============================
	//--------------------------
	// 左
	//--------------------------
	{
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Entry Left:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##left", ImGuiDir_Left) &&
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size() > 1)
		{// 削除
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].pop_back();
		}
		ImGui::SameLine(0.0f);

		if (ImGui::ArrowButton("##right", ImGuiDir_Right) &&
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size() < CGameManager::MAX_SIDEPLAYER)
		{// 追加
			int size = m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size();
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].push_back(size);
		}
		ImGui::SameLine();
		ImGui::PopID();

		// サイズ
		int segmentSize = static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size());
		ImGui::Text("%d", segmentSize);
	}

	//--------------------------
	// 右
	//--------------------------
	{
		ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Entry Right:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##left", ImGuiDir_Left) &&
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size() > 1)
		{// 削除
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].pop_back();
		}
		ImGui::SameLine(0.0f);

		if (ImGui::ArrowButton("##right", ImGuiDir_Right) &&
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size() < CGameManager::MAX_SIDEPLAYER)
		{// 追加
			int size = m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size();
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].push_back(size);
		}
		ImGui::SameLine();
		ImGui::PopID();

		// サイズ
		int segmentSize = static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size());
		ImGui::Text("%d", segmentSize);
	}


	//=============================
	// 総数変更
	//=============================
	ImGui::Text("Side Player");
	ImGui::SliderInt("SIDE_LEFT", &m_vecPlayerNum[CGameManager::ETeamSide::SIDE_LEFT], 1, CGameManager::MAX_SIDEPLAYER);
	ImGui::SliderInt("SIDE_RIGHT", &m_vecPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT], 1, CGameManager::MAX_SIDEPLAYER);

}
