//=============================================================================
// 
//  判定ゾーンエディタ―処理 [edit_judgezone.cpp]
//  Author : 石原颯馬
// 
//=============================================================================
#include "edit_judgezone.h"
#include "judge.h"
#include "judgezoneManager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	
}

//==========================================================================
// コンストラクタ
//==========================================================================
CEdit_JudgeZone::CEdit_JudgeZone()
{
	// 値のクリア
	m_bHoverWindow = false;	// マウスのウィンドウホバー判定
	for (int cntborder = 0; cntborder < CJudge::BORDER::MAX; cntborder++)
	{
		for (int cntjudge = 0; cntjudge < CJudge::JUDGE::JUDGE_MAX; cntjudge++)
		{
			m_abEnable[cntborder][cntjudge] = true;
		}
		m_aConditionType[cntborder] = CJudge::CONDITIONTYPE::TYPE_NONE;
	}
	m_JudgeZoneData.conditionTop.type = CJudge::CONDITIONTYPE::TYPE_NONE;
	m_JudgeZoneData.conditionUnder.type = CJudge::CONDITIONTYPE::TYPE_NONE;
	m_pSelectZone = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CEdit_JudgeZone::~CEdit_JudgeZone()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEdit_JudgeZone::Init()
{

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEdit_JudgeZone::Uninit()
{
	// 終了処理
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CEdit_JudgeZone::Update()
{
	// ウィンドウのマウスホバー判定
	ImGuiHoveredFlags frag = 128;
	m_bHoverWindow = ImGui::IsWindowHovered(frag);

	// ゾーン選択
	SelectZone();

	// ファイル操作
	FileControl();

	// 判定ゾーン設定
	SetJudgeZone();
}

//==========================================================================
// ゾーン選択
//==========================================================================
void CEdit_JudgeZone::SelectZone()
{
	std::map<std::string, CJudgeZone*> item;

	CListManager<CJudgeZone> list = CJudgeZone::GetListObj();
	auto itr = list.GetEnd();

	// 既存表示
	int cnt = 0;
	while (list.ListLoop(itr))
	{
		CJudgeZone::SJudgeZone zone = (*itr)->GetZone();
		std::string str = "<" + std::to_string(zone.start) + "~" + std::to_string(zone.end) + ">";
		item.insert(std::pair<std::string, CJudgeZone*>(str, (*itr)));
		cnt++;
	}

	// 新規
	item.insert(std::pair<std::string, CJudgeZone*>("<Create>", nullptr));

	// リスト表示
	static std::string strSelect = "<Create>";
	if (ImGui::BeginCombo("Edit Zone", strSelect.c_str()))
	{
		for (auto itrDisp = item.begin(); itrDisp != item.end(); itrDisp++)
		{
			bool isSelect = (strSelect == (*itrDisp).first);
			if (ImGui::Selectable((*itrDisp).first.c_str(), isSelect))
			{
				strSelect = (*itrDisp).first;
			}
			cnt++;
		}
		ImGui::EndCombo();
	}

	// オブジェクト選択
	m_pSelectZone = item[strSelect];

	//選択しているゾーンがあるなら取得
	if (m_pSelectZone != nullptr)
	{
		m_JudgeZoneData.zone = m_pSelectZone->GetZone();
		m_pSelectZone->SetInfo(CJudge::BORDER::TOP, m_JudgeZoneData.conditionTop);
		m_pSelectZone->SetInfo(CJudge::BORDER::UNDER, m_JudgeZoneData.conditionUnder);
	}
}

//==========================================================================
// ファイル操作
//==========================================================================
void CEdit_JudgeZone::FileControl()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	CJudgeZoneManager* manager = CJudgeZoneManager::GetInstance();
	float width = 150.0f;
	
	// リスト
	ImGui::SeparatorText("List");
	if (ImGui::Button("SaveList"))
	{// 保存
		OPENFILENAMEA filename = {};
		char sFilePass[1024] = {};
		filename = CreateOFN("\\data\\TEXT\\judgezonelist", sFilePass);

		// ファイル選択ダイアログを表示・セーブ
		if (GetOpenFileNameA(&filename) && strcmp(&sFilePass[0], "") != 0)
		{
			manager->Save(sFilePass);
		}
	}
	ImGui::SameLine();

	if (ImGui::Button("LoadList"))
	{// 読み込み
		OPENFILENAMEA filename = {};
		char sFilePass[1024] = {};
		filename = CreateOFN("\\data\\TEXT\\judgezonelist", sFilePass);

		// ファイル選択ダイアログを表示・ロード
		if (GetOpenFileNameA(&filename) && strcmp(&sFilePass[0], "") != 0)
		{
			manager->ReleaseAll();
			manager->Load(sFilePass);
		}
	}

	// 単体
	ImGui::SeparatorText("Single");
	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save & Apply"))
	{// 保存
		OPENFILENAMEA filename = {};
		char sFilePass[1024] = {};
		filename = CreateOFN("\\data\\TEXT\\judgezone", sFilePass);

		// ファイル選択ダイアログを表示・セーブ
		if (GetOpenFileNameA(&filename) && strcmp(&sFilePass[0], "") != 0)
		{
			manager->SaveZone(sFilePass, m_JudgeZoneData.zone, m_JudgeZoneData.conditionTop, m_JudgeZoneData.conditionUnder);

			// 新規なら生成
			if (m_pSelectZone == nullptr)
			{
				CJudgeZone* pJudgeZone = CJudgeZone::Create(m_JudgeZoneData.zone.start, m_JudgeZoneData.zone.end, m_JudgeZoneData.zone.borderHeight);
				pJudgeZone->SetInfo(CJudge::BORDER::TOP, m_JudgeZoneData.conditionTop);
				pJudgeZone->SetInfo(CJudge::BORDER::UNDER, m_JudgeZoneData.conditionTop);
				pJudgeZone->SetPath(sFilePass);
			}
		}
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Load"))
	{// 読み込み
		OPENFILENAMEA filename = {};
		char sFilePass[1024] = {};
		filename = CreateOFN("\\data\\TEXT\\judgezone", sFilePass);

		// ファイル選択ダイアログを表示・ロード
		if (GetOpenFileNameA(&filename) && strcmp(&sFilePass[0], "") != 0)
		{
			manager->LoadZone(sFilePass);
		}
	}
}

//==========================================================================
// 判定ゾーン設定
//==========================================================================
void CEdit_JudgeZone::SetJudgeZone()
{
	if (ImGui::TreeNode("JudgeZone"))
	{
		ImGui::DragFloat("Start", &m_JudgeZoneData.zone.start, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("End", &m_JudgeZoneData.zone.end, 0.01f, m_JudgeZoneData.zone.start, 1.0f);
		ImGui::DragFloat("Border", &m_JudgeZoneData.zone.borderHeight);
		
		if (ImGui::TreeNode("Condition_Top"))
		{// 上側条件
			SetCondition(m_JudgeZoneData.conditionTop,m_abEnable[CJudge::BORDER::TOP],m_aConditionType[CJudge::BORDER::TOP]);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Condition_Under"))
		{// 下側条件
			SetCondition(m_JudgeZoneData.conditionUnder,m_abEnable[CJudge::BORDER::UNDER], m_aConditionType[CJudge::BORDER::UNDER]);

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	//選択しているゾーンがあるなら設定
	if (m_pSelectZone != nullptr)
	{
		m_pSelectZone->SetZone(m_JudgeZoneData.zone);
		m_pSelectZone->SetInfo(CJudge::BORDER::TOP, m_JudgeZoneData.conditionTop);
		m_pSelectZone->SetInfo(CJudge::BORDER::UNDER, m_JudgeZoneData.conditionUnder);
	}
}

//==========================================================================
// 条件設定
//==========================================================================
void CEdit_JudgeZone::SetCondition(CJudge::SJudgeCondition& condition, bool* pEnable, CJudge::CONDITIONTYPE& conditionType)
{
	static const char* items[] = { "None", "HitNum" };
	int selectedItem = conditionType;

	// 種類
	for (int cnt = 0; cnt < CJudge::CONDITIONTYPE::TYPE_MAX; cnt++)
	{
		if (ImGui::RadioButton(items[cnt], &selectedItem, cnt))
		{
			conditionType = static_cast<CJudge::CONDITIONTYPE>(selectedItem);
		}
	}
	condition.type = conditionType;

	// 有効・無効設定後数値設定
	// =========AAA=========
	ImGui::Checkbox("Enable_AAA", &pEnable[0]);
	if (!pEnable[0]) ImGui::BeginDisabled(true);
	
		ImGui::InputInt("AAA_Num", &condition.judgeParam[CJudge::JUDGE::JUDGE_AAA]);
	
	if (!pEnable[0])
	{
		ImGui::EndDisabled();
		condition.judgeParam[CJudge::JUDGE::JUDGE_AAA] = -1;
	}
	// =========AAA=========

	// =========BBB=========
	ImGui::Checkbox("Enable_BBB", &pEnable[1]);
	if (!pEnable[1]) ImGui::BeginDisabled(true);
	
		ImGui::InputInt("BBB_Num", &condition.judgeParam[CJudge::JUDGE::JUDGE_BBB]);
	
	if (!pEnable[1])
	{
		ImGui::EndDisabled();
		condition.judgeParam[CJudge::JUDGE::JUDGE_BBB] = -1;
	}
	// =========BBB=========

	// =========CCC=========
	ImGui::Checkbox("Enable_CCC", &pEnable[2]);
	if (!pEnable[2]) ImGui::BeginDisabled(true);
	
		ImGui::InputInt("CCC_Num", &condition.judgeParam[CJudge::JUDGE::JUDGE_CCC]);
	
	if (!pEnable[2])
	{
		ImGui::EndDisabled();
		condition.judgeParam[CJudge::JUDGE::JUDGE_CCC] = -1;
	}
	// =========CCC=========

	// =========DDD=========
	ImGui::Checkbox("Enable_DDD", &pEnable[3]);
	if (!pEnable[3]) ImGui::BeginDisabled(true);
	
		ImGui::InputInt("DDD_Num", &condition.judgeParam[CJudge::JUDGE::JUDGE_DDD]);
	
	if (!pEnable[3])
	{
		ImGui::EndDisabled();
		condition.judgeParam[CJudge::JUDGE::JUDGE_DDD] = -1;
	}
	// =========DDD=========
}

//==========================================================================
// ファイルを開くウィンドウに必要な情報生成
//==========================================================================
OPENFILENAMEA CEdit_JudgeZone::CreateOFN(std::string relativeDir, char* filePass)
{
	OPENFILENAMEA filename = {};
	// ファイル選択ダイアログの設定
	filename.lStructSize = sizeof(OPENFILENAMEA);
	filename.hwndOwner = NULL;
	filename.lpstrFilter = "テキストファイル\0*.txt\0画像ファイル\0*.bmp;.jpg\0すべてのファイル\0.*\0\0";
	filename.lpstrFile = filePass;
	filename.nMaxFile = MAX_PATH;
	filename.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;


	// カレントディレクトリを取得する
	char szCurrentDir[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, szCurrentDir);

	// "data"フォルダの絶対パスを求める
	std::string strDataDir = szCurrentDir;
	strDataDir += relativeDir;

	// 存在する場合は、lpstrInitialDirに指定する
	if (GetFileAttributesA(strDataDir.c_str()) != INVALID_FILE_ATTRIBUTES)
	{
		filename.lpstrInitialDir = strDataDir.c_str();
	}

	return filename;
}
