//=============================================================================
// 
// エントリーシーン処理 [entryscene.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "entryscene.h"
#include "manager.h"

// シーン
#include "entry_setupTeam.h"
#include "entry_dressup.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CEntryScene::CEntryScene()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CEntryScene::~CEntryScene()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CEntryScene* CEntryScene::Create(CEntry::ESceneType sceneType)
{
	// メモリの確保
	CEntryScene* pScene = nullptr;

	switch (sceneType)
	{
	case CEntry::SCENETYPE_SETUPTEAM:
	{
		CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();	// チーム設定シーン

		// チーム設定シーンの終了
		SAFE_UNINIT(pSetupTeam);

		// チーム設定シーンの再生成
		pScene = DEBUG_NEW CEntry_SetUpTeam;
		break;
	}
	case CEntry::SCENETYPE_DRESSUP:
		pScene = DEBUG_NEW CEntry_Dressup;
		break;

	default:
		break;
	}

	if (pScene != nullptr)
	{
		// クラスの初期化
		if (FAILED(pScene->Init()))
		{ // 初期化に失敗した場合
			SAFE_UNINIT(pScene);
			return nullptr;
		}
	}

	return pScene;
}

//==========================================================================
// 更新処理
//==========================================================================
void CEntryScene::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	Debug();
}
