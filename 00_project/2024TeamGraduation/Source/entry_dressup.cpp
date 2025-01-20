//=============================================================================
// 
//  ドレスアップ設定処理 [entry_dressup.cpp]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//=============================================================================
#include "entry_dressup.h"
#include "manager.h"
#include "fade.h"
#include "entry_setupTeam.h"
#include "playerManager.h"
#include "inputKeyButton.h"
#include "string2D.h"
#include "dressupUI.h"
#include "selectUI.h"
#include "object2D.h"
#include "entryRuleManager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTFILE	= "data\\TEXT\\entry\\setupTeam.txt";
	const std::string TOP_LINE	= "#==============================================================================";	// テキストのライン
	const std::string TEXT_LINE	= "#------------------------------------------------------------------------------";	// テキストのライン
	const int PRIORITY = 6;	// 優先順位

	namespace ui
	{
		namespace left
		{
			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, VEC3_SCREEN_CENT.y - 40.0f, 0.0f);	// 左中心位置
			const float OFFSET_X = 210.0f;	// X座標オフセット
		}

		namespace right
		{
			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, VEC3_SCREEN_CENT.y - 40.0f, 0.0f);	// 右中心位置
			const float OFFSET_X = 210.0f;	// X座標オフセット
		}

		namespace name
		{
			const MyLib::Vector3 POS[] = { MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, 75.0f, 0.0f), MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, 75.0f, 0.0f) };	// 原点位置
			const char*	FONT		= "data\\FONT\\玉ねぎ楷書激無料版v7改.ttf";	// フォントパス
			const bool	ITALIC		= false;					// イタリック
			const float	HEIGHT		= 42.0f;					// 文字縦幅
			const EAlignX ALIGN_X	= XALIGN_CENTER;			// 横配置
			const D3DXCOLOR COL		= MyLib::color::Black();	// 色
		}

		namespace back
		{
			const MyLib::Vector3 POS	= MyLib::Vector3(VEC3_SCREEN_CENT.x - 420.0f, 640.0f, 0.0f);	// 位置
			const MyLib::Vector2 SIZE	= MyLib::Vector2(180.0f, 45.0f);	// 大きさ
		}

		namespace enter
		{
			const MyLib::Vector3 POS	= MyLib::Vector3(VEC3_SCREEN_CENT.x + 420.0f, 640.0f, 0.0f);	// 位置
			const MyLib::Vector2 SIZE	= MyLib::Vector2(180.0f, 45.0f);	// 大きさ
		}
	}
}

//==========================================================================
// コンストラクタ
//==========================================================================
CEntry_Dressup::CEntry_Dressup() : CEntryScene(),
	m_pRuleManager	(nullptr),		// ルールマネージャー
	m_pBack			(nullptr),		// 戻る情報
	m_pEnter		(nullptr),		// 決定情報
	m_state			(STATE_DRESSUP)	// 状態
{
	// メンバ変数をクリア
	memset(&m_apTeamName[0], 0, sizeof(m_apTeamName));	// チーム名情報
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
	int nMaxLeft = pSetupTeam->GetPlayerNum(CGameManager::ETeamSide::SIDE_LEFT);	// 左プレイヤー総数
	for (int nPlayerIdx = 0; nPlayerIdx < CGameManager::MAX_PLAYER; nPlayerIdx++)
	{ // プレイヤー人数分繰り返す

		// チーム指定がない場合次へ
		CGameManager::ETeamSide team = pSetupTeam->GetTeamSide(nPlayerIdx);	// チームサイド
		if (team != CGameManager::ETeamSide::SIDE_LEFT) { continue; }

		// UIの位置を計算
		MyLib::Vector3 posUI = ui::left::POS;	// UI位置
		posUI.x = ui::left::POS.x - (ui::left::OFFSET_X * (float)(nMaxLeft - 1)) * 0.5f + (ui::left::OFFSET_X * (float)nCurLeft);

		// チームメンバー数を加算
		nCurLeft++;

		// 着せ替えUIの生成
		CDressupUI* pDressup = CDressupUI::Create(this, nPlayerIdx, posUI);
		if (pDressup == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// 着せ替えUIの追加
		m_vecDressInfo.push_back(pDressup);

		const int nPadIdx = pSetupTeam->PlayerIdxToPadIdx(nPlayerIdx);	// 操作権インデックス
		if (nPadIdx > -1)
		{ // プレイヤーの場合

			// 選択UIの生成
			CSelectUI* pSelect = CSelectUI::Create(team, nCurLeft - 1, nPadIdx, posUI);
			if (pSelect == nullptr)
			{ // 生成に失敗した場合

				return E_FAIL;
			}

			// 選択UIの追加
			m_vecSelect.push_back(pSelect);
		}
	}

	int nCurRight = 0;	// 現在の右プレイヤー数
	int nMaxRight = pSetupTeam->GetPlayerNum(CGameManager::ETeamSide::SIDE_RIGHT);	// 右プレイヤー総数
	for (int nPlayerIdx = 0; nPlayerIdx < CGameManager::MAX_PLAYER; nPlayerIdx++)
	{ // プレイヤー人数分繰り返す

		// チーム指定がない場合次へ
		CGameManager::ETeamSide team = pSetupTeam->GetTeamSide(nPlayerIdx);	// チームサイド
		if (team != CGameManager::ETeamSide::SIDE_RIGHT) { continue; }

		// UIの位置を計算
		MyLib::Vector3 posUI = ui::right::POS;	// UI位置
		posUI.x = ui::right::POS.x - (ui::right::OFFSET_X * (float)(nMaxRight - 1)) * 0.5f + (ui::right::OFFSET_X * (float)nCurRight);

		// チームメンバー数を加算
		nCurRight++;

		// 着せ替えUIの生成
		CDressupUI* pDressup = CDressupUI::Create(this, nPlayerIdx, posUI);
		if (pDressup == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// 着せ替えUIの追加
		m_vecDressInfo.push_back(pDressup);

		const int nPadIdx = pSetupTeam->PlayerIdxToPadIdx(nPlayerIdx);	// 操作権インデックス
		if (nPadIdx > -1)
		{ // プレイヤーの場合

			// 選択UIの生成
			CSelectUI* pSelect = CSelectUI::Create(team, nCurLeft + nCurRight - 1, nPadIdx, posUI);
			if (pSelect == nullptr)
			{ // 生成に失敗した場合

				return E_FAIL;
			}

			// 選択UIの追加
			m_vecSelect.push_back(pSelect);
		}
	}

	for (int i = 0; i < CGameManager::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		// チーム名の生成
		m_apTeamName[i] = CString2D::Create
		( // 引数
			ui::name::FONT,		// フォントパス
			ui::name::ITALIC,	// イタリック
			L"ここにチーム名",	// 指定文字列
			ui::name::POS[i],	// 原点位置
			ui::name::HEIGHT,	// 文字縦幅
			ui::name::ALIGN_X,	// 横配置
			VEC3_ZERO,			// 原点向き
			ui::name::COL		// 色
		);
		if (m_apTeamName[i] == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}
	}

	// 戻るの生成
	m_pBack = CObject2D::Create(PRIORITY);
	if (m_pBack == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// 位置を設定
	m_pBack->SetPosition(ui::back::POS);

	// 大きさを設定
	m_pBack->SetSize(ui::back::SIZE);

	// 決定の生成
	m_pEnter = CObject2D::Create(PRIORITY);
	if (m_pEnter == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// 位置を設定
	m_pEnter->SetPosition(ui::enter::POS);

	// 大きさを設定
	m_pEnter->SetSize(ui::enter::SIZE);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEntry_Dressup::Uninit()
{
	// エントリールールの破棄
	SAFE_REF_RELEASE(m_pRuleManager);

	for (auto& rInfo : m_vecDressInfo)
	{ // 要素数分繰り返す

		// 着せ替えUIの終了
		SAFE_UNINIT(rInfo);
	}

	for (auto& rSelect : m_vecSelect)
	{ // 要素数分繰り返す

		// 選択UIの終了
		SAFE_UNINIT(rSelect);
	}

	for (int i = 0; i < CGameManager::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		// チーム名の終了
		SAFE_UNINIT(m_apTeamName[i]);
	}

	// 戻るの終了
	SAFE_UNINIT(m_pBack);

	// 決定の終了
	SAFE_UNINIT(m_pEnter);

	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CEntry_Dressup::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	switch (m_state)
	{ // 状態ごとの処理
	case EState::STATE_DRESSUP:
		break;

	case EState::STATE_SETTING:

		// ルールマネージャーの更新
		assert(m_pRuleManager != nullptr);
		m_pRuleManager->Update(fDeltaTime, fDeltaRate, fSlowRate);
		break;

	case EState::STATE_END:

		// セーブ処理
		Save();

		// ゲーム画面に遷移する
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_GAME);
		break;

	default:
		assert(false);
		break;
	}

	// 親の更新
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 状態設定
//==========================================================================
void CEntry_Dressup::SetState(const EState state)
{
	// 状態を設定
	m_state = state;
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_DRESSUP:

		// エントリールールの破棄
		SAFE_REF_RELEASE(m_pRuleManager);
		break;

	case STATE_SETTING:

		if (m_pRuleManager == nullptr)
		{
			// エントリールールの生成
			m_pRuleManager = CEntryRuleManager::Create(this);
		}
		break;

	case STATE_END:
		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// 着せ替えUI準備完了フラグの設定
//==========================================================================
void CEntry_Dressup::SetDressUIReady(const int nPlayerIdx, const bool bReady)
{
	// プレイヤーインデックスが範囲外の場合エラー
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumPlayer()) { assert(false); return; }

	// 準備完了フラグを返す
	m_vecDressInfo[nPlayerIdx]->SetReady(bReady);
}

//==========================================================================
// 着せ替えUIの準備完了済みかの確認
//==========================================================================
bool CEntry_Dressup::IsDressUIReady(const int nPlayerIdx)
{
	// プレイヤーインデックスが範囲外の場合エラー
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumPlayer()) { assert(false); return false; }

	// 準備完了フラグを返す
	return m_vecDressInfo[nPlayerIdx]->IsReady();
}

//==========================================================================
// 着せ替えUI操作権の設定
//==========================================================================
void CEntry_Dressup::SetDressUIControl(const int nPadIdx, const int nPlayerIdx)
{
	// プレイヤーインデックスが範囲外の場合エラー
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumPlayer()) { assert(false); return; }

	// 引数プレイヤーの着せ替えUI位置を返す
	m_vecDressInfo[nPlayerIdx]->SetPadIdx(nPadIdx);
}

//==========================================================================
// 選択可能かの確認
//==========================================================================
bool CEntry_Dressup::IsSelectOK(const int nPadIdx, const int nPlayerIdx) const
{
	// 自分以外のユーザーの着せ替えUIの場合選択不可
	const int nSelectPadIdx = m_vecDressInfo[nPlayerIdx]->GetMyPlayerIdx();	// 選択予定先の操作権インデックス
	if (nSelectPadIdx > -1 && nSelectPadIdx != nPadIdx) { return false; }

	for (const auto& rSelect : m_vecSelect)
	{ // 要素数分繰り返す

		// 既に自分以外のユーザーが選択中の場合選択不可
		if (rSelect->GetSelectIdx().x == nPlayerIdx
		&&  rSelect->GetPadIdx()	  != nPadIdx) { return false; }
	}

	return true;
}

//==========================================================================
// 選択UI選択操作フラグの設定
//==========================================================================
void CEntry_Dressup::SetSelectUISelect(const int nPadIdx, const bool bSelect)
{
	for (auto& rSelect : m_vecSelect)
	{ // 要素数分繰り返す

		// 選択UIの操作権インデックスが引数の操作権インデックスと一致しない場合次へ
		if (rSelect->GetPadIdx() != nPadIdx) { continue; }

		// 選択操作可能フラグの設定
		return rSelect->SetSelect(bSelect);
	}
}

//==========================================================================
// 着せ替えUI位置取得
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetDressUIPosition(const int nPlayerIdx) const
{
	// プレイヤーインデックスが範囲外の場合エラー
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumPlayer()) { assert(false); return VEC3_ZERO; }

	// 引数プレイヤーの着せ替えUI位置を返す
	return m_vecDressInfo[nPlayerIdx]->GetPosition();
}

//==========================================================================
// 名前UI位置取得
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetNameUIPosition(const CGameManager::ETeamSide team)
{
	// 文字列の位置を返す
	return m_apTeamName[team]->GetPosition();
}

//==========================================================================
// 名前UI大きさ取得
//==========================================================================
MyLib::Vector2 CEntry_Dressup::GetNameUISize(const CGameManager::ETeamSide team)
{
	// 文字列の大きさを返す
	return MyLib::Vector2(m_apTeamName[team]->GetStrWidth(), ui::name::HEIGHT);
}

//==========================================================================
// 戻るUI位置取得
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetBackUIPosition()
{
	// ポリゴンの位置を返す
	return m_pBack->GetPosition();
}

//==========================================================================
// 戻るUI大きさ取得
//==========================================================================
MyLib::Vector2 CEntry_Dressup::GetBackUISize()
{
	// ポリゴンの大きさを返す
	return m_pBack->GetSize();
}

//==========================================================================
// 決定UI位置取得
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetEnterUIPosition()
{
	// ポリゴンの位置を返す
	return m_pEnter->GetPosition();
}

//==========================================================================
// 決定UI大きさ取得
//==========================================================================
MyLib::Vector2 CEntry_Dressup::GetEnterUISize()
{
	// ポリゴンの大きさを返す
	return m_pEnter->GetSize();
}

//==========================================================================
// ゲーム設定遷移
//==========================================================================
bool CEntry_Dressup::TransSetting()
{
	// 準備完了できていない場合抜ける
	if (!IsAllReady()) { return false; }

	// ゲーム設定状態に遷移
	SetState(EState::STATE_SETTING);

	return true;
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
		int nPadIdx = m_vecDressInfo[i]->GetPadIdx();
		CGameManager::ETeamSide side = m_vecDressInfo[i]->GetTeam();

		// 読み込み情報
		vecSaveInfo[side].emplace_back();
		CPlayerManager::LoadInfo* pLoadInfo = &vecSaveInfo[side].back();
		pLoadInfo->nControllIdx = nPadIdx;									// 自身の操作するインデックス番号取得
		pLoadInfo->nHair = m_vecDressInfo[i]->GetHairNowIdx();				// 髪のインデックス番号
		pLoadInfo->nAccessory = m_vecDressInfo[i]->GetAccessoryNowIdx();	// アクセのインデックス番号
		pLoadInfo->nFace = m_vecDressInfo[i]->GetFaceNowIdx();				// 顔のインデックス番号
		pLoadInfo->eBody = m_vecDressInfo[i]->GetBodyType();				// 体型
		pLoadInfo->eHanded = m_vecDressInfo[i]->GetHandedness();			// 利き手
	}

	// セーブ処理
	pPlayerMgr->Save(vecSaveInfo[CGameManager::ETeamSide::SIDE_LEFT], vecSaveInfo[CGameManager::ETeamSide::SIDE_RIGHT]);

	// ゲーム設定の保存
	assert(m_pRuleManager != nullptr);
	CEntryRuleManager::SRule rule = m_pRuleManager->GetRule();	// ルール
	CEntryRuleManager::SaveSetting(&rule);
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

	//--------------------------
	// シーン遷移
	//--------------------------
	if (IsAllReady() &&
		CInputKeyboard::GetInstance()->GetTrigger(DIK_X))
	{
		// ゲーム設定状態に遷移
		SetState(EState::STATE_SETTING);
	}

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
