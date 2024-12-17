//=============================================================================
// 
// タイトルシーン処理 [titlescene.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "titlescene.h"
#include "manager.h"

// シーン
#include "title_controllwait.h"
#include "title_susuru.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CTitleScene::CTitleScene()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CTitleScene::~CTitleScene()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTitleScene* CTitleScene::Create(CTitle::ESceneType sceneType)
{
	// メモリの確保
	CTitleScene* pScene = nullptr;

	switch (sceneType)
	{
	case CTitle::ESceneType::SCENETYPE_CONTROLLWAIT:	// 操作待機
		pScene = DEBUG_NEW CTitle_ControllWait;
		break;

	case CTitle::ESceneType::SCENETYPE_SUSURU:	// SUSURUシーン
		pScene = DEBUG_NEW CTitle_SUSURU;
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
// 初期化
//==========================================================================
HRESULT CTitleScene::Init()
{
	// サウンド停止
	CSound::GetInstance()->StopSound();
	return S_OK;
}

//==========================================================================
// 更新処理
//==========================================================================
void CTitleScene::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}
