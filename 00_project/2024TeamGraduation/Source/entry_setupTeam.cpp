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
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		m_vecAddIdx[i].clear();		// 追加されたインデックス
		m_nMaxChangeIdx[i] = -1;	// 最大数変更するインデック;
		m_nPlayerNum[i] = 1;		// プレイヤーの数
	}

	for (int i = 0; i < CGameManager::MAX_PLAYER; i++)
	{
		m_TeamSide[i] = CGameManager::ETeamSide::SIDE_NONE;	// チームサイド
	}

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		m_nEntryIdx[i] = -1;								// エントリーのインデックス
	}
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
	// 読み込み
	Load();

	// 追加されたインデックスリセット TODO : 毎回リセットしちゃうから要検討
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		m_vecAddIdx[i].clear();
	}
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEntry_SetUpTeam::Uninit()
{
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CEntry_SetUpTeam::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親の更新
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// チーム選択
	SelectTeam();

	// プレイヤー最大数変更
	ChangeMaxPlayer();

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// 一旦シーン切り替え TODO : 全員エントリーしてたらとかにする
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, 0))
	{
		// 空いたところにAI追加
		for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
		{
			// チームサイド追加
			for (int j = 0; j < CGameManager::MAX_PLAYER; j++)
			{
				if (m_TeamSide[j] == CGameManager::ETeamSide::SIDE_NONE)
				{// NONEの人を変える

					if ((int)m_vecAddIdx[i].size() <= j &&
						m_nPlayerNum[i] > (int)m_vecAddIdx[i].size())
					{// サイド毎のプレイヤー数より大きい数 && チーム内人数を越えていない
						m_TeamSide[j] = (CGameManager::ETeamSide)i;
						m_vecAddIdx[i].push_back(-1);
					}

				}

			}

			//while (m_nPlayerNum[i] >= (int)m_vecAddIdx[i].size())
			//{// 空きがなくなるまで
			//	m_vecAddIdx[i].push_back(-1);
			//}
		}

		CEntry::GetInstance()->ChangeEntryScene(CEntry::ESceneType::SCENETYPE_DRESSUP);
	}
}

//==========================================================================
// チーム選択
//==========================================================================
void CEntry_SetUpTeam::SelectTeam()
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		//--------------------------
		// 参加
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, i))
		{// エントリー、コントローラー生成

			// インデックス格納
			for (int j = 0; j < mylib_const::MAX_PLAYER; j++)
			{
				// もうエントリーされてるのは使わない
				if (m_nEntryIdx[j] >= 0 && m_nEntryIdx[j] != i) continue;
				m_nEntryIdx[j] = i;
				break;
			}
		}

		// 今回のインデックス
		int nowIdx = m_nEntryIdx[i];
		if (nowIdx < 0) continue;	// 未エントリーは抜ける

		//--------------------------
		// チーム替え
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nowIdx) &&
			m_TeamSide[nowIdx] != CGameManager::ETeamSide::SIDE_LEFT)
		{// 左移動(左端じゃない時)

			// 左に移動
			m_TeamSide[nowIdx] = (m_TeamSide[nowIdx] == CGameManager::ETeamSide::SIDE_NONE) ? CGameManager::ETeamSide::SIDE_LEFT : CGameManager::ETeamSide::SIDE_NONE;
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nowIdx) &&
			m_TeamSide[nowIdx] != CGameManager::ETeamSide::SIDE_RIGHT)
		{// 右移動(右端じゃない時)

			// 右に移動
			m_TeamSide[nowIdx] = (m_TeamSide[nowIdx] == CGameManager::ETeamSide::SIDE_NONE) ? CGameManager::ETeamSide::SIDE_RIGHT : CGameManager::ETeamSide::SIDE_NONE;
		}

		//--------------------------
		// チーム内人数変え
		//--------------------------
		// 変更するインデックス決める
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, nowIdx) &&
			m_TeamSide[nowIdx] != CGameManager::ETeamSide::SIDE_NONE &&
			m_nMaxChangeIdx[m_TeamSide[nowIdx]] < 0)
		{// 上移動(NONEじゃない時)

			// 自分のサイドを変更する用インデックス保持
			int side = m_TeamSide[nowIdx];
			m_nMaxChangeIdx[side] = nowIdx;
		}

		//--------------------------
		// 決定!
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, nowIdx) &&
			m_TeamSide[nowIdx] != CGameManager::ETeamSide::SIDE_NONE)
		{// チーム決定 && 左右どちらかに合わせてるとき

			int side = m_TeamSide[nowIdx];
			
			// 既にある場合は追加しない
			const auto& itr = std::find(m_vecAddIdx[side].begin(), m_vecAddIdx[side].end(), nowIdx);
			if (itr == m_vecAddIdx[side].end())
			{// 存在しない
				m_vecAddIdx[side].push_back(nowIdx);
			}
		}
	}
}

//==========================================================================
// プレイヤー最大数変更
//==========================================================================
void CEntry_SetUpTeam::ChangeMaxPlayer()
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	for (int side = 0; side < CGameManager::ETeamSide::SIDE_MAX; side++)
	{
		// 今回変更するコントローラー番号
		int changeIdx = m_nMaxChangeIdx[side];
		if (changeIdx < 0) continue;

		//--------------------------
		// 最大数変更
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, changeIdx))
		{// 減らす
			m_nPlayerNum[side]--;	// プレイヤーの数
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, changeIdx))
		{// 増やす
			m_nPlayerNum[side]++;	// プレイヤーの数
		}
		m_nPlayerNum[side] = UtilFunc::Transformation::Clamp(m_nPlayerNum[side], 1, CGameManager::MAX_SIDEPLAYER);

		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, changeIdx))
		{// 下移動(最大数変更解除)

			// 自分のサイドを変更する用インデックス解除
			m_nMaxChangeIdx[side] = -1;
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
		m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT] << 
		" " <<
		m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT] << std::endl;
	
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
// ロード
//==========================================================================
void CEntry_SetUpTeam::Load()
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

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT] >>
				m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT];	// プレイヤー数
		}

		if (line.find("LEFT_ID") != std::string::npos)
		{// LEFT_IDで左側チームのインデックス

			// ストリーム作成
			std::istringstream lineStream(line);

			// 破棄
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].clear();

			// リサイズ
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].resize(CGameManager::MAX_SIDEPLAYER);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][0] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][1] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][2];	// プレイヤー数
		}

		if (line.find("RIGHT_ID") != std::string::npos)
		{// RIGHT_IDで右側チームのインデックス

			// ストリーム作成
			std::istringstream lineStream(line);

			// 破棄
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].clear();

			// リサイズ
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].resize(CGameManager::MAX_SIDEPLAYER);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][0] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][1] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][2];	// プレイヤー数
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
void CEntry_SetUpTeam::Debug()
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
				m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT]--;		// プレイヤーの数
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
		ImGui::SliderInt("SIDE_LEFT", &m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT], 1, CGameManager::MAX_SIDEPLAYER);
		ImGui::SliderInt("SIDE_RIGHT", &m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT], 1, CGameManager::MAX_SIDEPLAYER);

		// エントリーのインデックス
		for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
		{
			ImGui::Text("EntryIdx[%d] : %d", i, m_nEntryIdx[i]);
		}

		ImGui::TreePop();
	}
}

//==========================================================================
// エントリーのインデックス
//==========================================================================
int CEntry_SetUpTeam::GetEntryIdx(int i)
{
	if (i >= mylib_const::MAX_PLAYER) return -1;
	return m_nEntryIdx[i];
}
