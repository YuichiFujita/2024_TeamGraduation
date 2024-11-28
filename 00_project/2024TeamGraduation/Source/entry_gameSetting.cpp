//=============================================================================
// 
// ゲーム詳細設定処理 [entry_gameSetting.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "entry_gameSetting.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"
#include "fade.h"

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
CEntry_GameSetting::CEntry_GameSetting() : CEntryScene()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CEntry_GameSetting::~CEntry_GameSetting()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEntry_GameSetting::Init()
{
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEntry_GameSetting::Uninit()
{
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CEntry_GameSetting::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親の更新
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// 一旦シーン切り替え TODO : 全員エントリーしてたらとかにする
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pKey->GetTrigger(DIK_RETURN))
	{
		// 遷移
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_GAME);
	}
}

//==========================================================================
// デバッグ処理
//==========================================================================
void CEntry_GameSetting::Debug()
{
	if (ImGui::TreeNode("GameSetting"))
	{
		//=============================
		// セーブ
		//=============================s
		if (ImGui::Button("Save"))
		{
			
		}

		// TODO : 時間とか変更する

		ImGui::TreePop();
	}
}
