//=============================================================================
// 
//  シーン処理 [scene.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "scene.h"
#include "renderer.h"
#include "calculation.h"
#include "Xload.h"
#include "map.h"
#include "fade.h"
#include "player.h"
#include "camera.h"
#include "MyEffekseer.h"
#include "fog.h"
#include "edit_map.h"
#include "blackframe.h"

// 遷移先
#include "game.h"
#include "title.h"
#include "result.h"
#include "tutorial.h"
#include "ranking.h"

// TODO
#include "manager.h"
#include "debugproc.h"
#include "char2D.h"
#include "string2D.h"
#include "text2D.h"
#include "timer.h"
#include "timerUI.h"
#include "timeUI.h"
#include "3D_effect.h"
#include "2D_effect.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	
	const char* MAP_TEXT[] =
	{
		"data\\TEXT\\map\\info.txt",	// なし
		"data\\TEXT\\map\\info.txt",	// タイトル
		"data\\TEXT\\map\\info.txt",	// チュートリアル
		"data\\TEXT\\map\\info.txt",	// ゲーム
		"data\\TEXT\\map\\info.txt",	// ゲーム
		"data\\TEXT\\map\\map_result.txt",	// リザルト
		"data\\TEXT\\map\\info.txt",	// ランキング
	};
	
	const char* ELEVATION_TEXT = "data\\TEXT\\elevation\\field_ingame.txt";
}

#if 1
#define LOADMAP = 0;
#endif

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CScene::CScene()
{
	// 変数のクリア
	m_mode = MODE_TITLE;
	m_pEditMap = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CScene::~CScene()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CScene *CScene::Create(CScene::MODE mode)
{
	// 生成用のオブジェクト
	CScene *pScene = nullptr;

	if (pScene == nullptr)
	{// nullptrだったら

		// 生成処理
		switch (mode)
		{
		case CScene::MODE_TITLE:
			pScene = CTitle::Create();
			break;

		case CScene::MODE_GAME:
		case CScene::MODE::MODE_GAMETUTORIAL:
			pScene = CGame::Create(mode);
			break;

		case MODE_TUTORIAL:
			pScene = CTutorial::Create();
			break;

		case CScene::MODE_RESULT:
			pScene = DEBUG_NEW CResult;
			break;

		case CScene::MODE_RANKING:
			pScene = DEBUG_NEW CRanking;
			break;
		}

		if (pScene != nullptr)
		{// メモリの確保が出来ていたら

			// モードの設定
			pScene->m_mode = mode;
		}

		return pScene;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CScene::Init()
{
	// TODO
#if 0
	CChar2D::Create("data\\FONT\\零ゴシック.otf", false, L'あ', SCREEN_CENT + D3DXVECTOR3(100.0f, 0.0f, 0.0f) * 1.0f);
	CChar2D::Create("data\\FONT\\零ゴシック.otf", false, L'a',  SCREEN_CENT + D3DXVECTOR3(100.0f, 0.0f, 0.0f) * 2.0f);
	CChar2D::Create("data\\FONT\\零ゴシック.otf", false, L'0',  SCREEN_CENT + D3DXVECTOR3(100.0f, 0.0f, 0.0f) * 0.0f);
#endif

	// TODO
#if 0
	CString2D::Create("data\\FONT\\零ゴシック.otf", false, L"abcdefghijklmnopqrstuvwxyz", SCREEN_CENT, 60.0f, CString2D::XALIGN_RIGHT);
#endif

	// TODO
#if 0
	CText2D* p = CText2D::Create("data\\FONT\\零ゴシック.otf", false, SCREEN_CENT, 60.0f, 140.0f, CString2D::XALIGN_LEFT, CText2D::YALIGN_TOP);
	p->AddString(L"abcdefghijklmnopqrstuvwxyz");
	p->AddString(L"あいうえおかきくけこさしすせそ");
	p->AddString(L"さん/ボボボーボ・ボーボボ");
#endif

	// TODO
#if 0
	CTimer::Create(0.0f, 120.0f);
	CTimer::Create(120.0f, 120.0f);
#endif

	// TODO
#if 0
	CTimerUI* p = CTimerUI::Create(145.0f, 145.0f, SCREEN_CENT, D3DXVECTOR2(60.0f, 60.0f), D3DXVECTOR2(60.0f, 60.0f), D3DXVECTOR2(60.0f, 0.0f), D3DXVECTOR2(60.0f, 0.0f), CTimeUI::XALIGN_LEFT);
	p->Start();	// TODO：タイマー破棄できないし数字の表示が変
#endif

	// TODO
#if 0
	CTimeUI::Create(145.0f, SCREEN_CENT, D3DXVECTOR2(60.0f, 60.0f), D3DXVECTOR2(60.0f, 60.0f), D3DXVECTOR2(60.0f, 0.0f), D3DXVECTOR2(60.0f, 0.0f));
#endif

	// エフェクト全て停止
	CMyEffekseer::GetInstance()->StopAll();

	// フォグリセット
	MyFog::ToggleFogFrag(false);


	//**********************************
	// 黒フレーム
	//**********************************
	CBlackFrame::Create();

	//**********************************
	// マップの生成
	//**********************************
#ifdef LOADMAP
	if (FAILED(MyMap::Create(MAP_TEXT[m_mode])))
	{// 失敗した場合
		return E_FAIL;
	}
#endif

	//**********************************
	// マップの生成
	//**********************************
#ifdef LOADMAP
	m_pEditMap = CEdit_Map_Release::Create(MAP_TEXT[m_mode], CManager::BuildMode::MODE_RELEASE);
#endif

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CScene::Uninit()
{
	//**********************************
	// 破棄フェーズ
	//**********************************
	// エディットマップ
	if (m_pEditMap != nullptr)
	{
		m_pEditMap->Uninit();
		m_pEditMap = nullptr;
	}

	if (CBlackFrame::GetInstance() != nullptr)
	{
		CBlackFrame::GetInstance()->Uninit();
	}
}

//==========================================================================
// 削除処理
//==========================================================================
void CScene::Kill()
{
	// マップ
	MyMap::Release();
}


//==========================================================================
// 更新処理
//==========================================================================
void CScene::Update(const float fDeltaTime)
{
	if (GET_INPUTKEY->GetTrigger(DIK_0))
	{
		// エフェクト2D
#if 0
		CEffect2D* p = CEffect2D::Create(MyLib::Vector3(0.0f, 200.0f, 0.0f), MyLib::Vector3(250.0f, 0.0f, 0.0f), MyLib::color::White(), 120.0f, 1.0f, 0, CEffect2D::TYPE::TYPE_BLACK);
#endif

		// エフェクシア
#if 0
		CEffekseerObj* p = CEffekseerObj::Create(CMyEffekseer::EFKLABEL::EFKLABEL_HIT, VEC3_ZERO, VEC3_ZERO, VEC3_ZERO, 50.0f);
#endif
	}

	if (GET_INPUTKEY->GetTrigger(DIK_9))
	{
		// TODO：エフェクト3D
#if 0
		CEffect3D* p = CEffect3D::Create(VEC3_ZERO, MyLib::Vector3(0.0f, 600.0f, 0.0f), MyLib::color::White(), 120.0f, 10.0f, 0, CEffect3D::TYPE::TYPE_BLACK, 0.1f);
		p->SetGravityValue(10.0f);
#endif

		// TODO：
#if 0

#endif
	}

	int* pFps = GetDebugFps();
	if (GET_INPUTKEY->GetPress(DIK_UP))
	{
		*pFps += 1;
	}
	if (GET_INPUTKEY->GetPress(DIK_DOWN))
	{
		*pFps -= 1;
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CScene::Draw()
{
	
}

//==========================================================================
// 現在のモード取得
//==========================================================================
CScene::MODE CScene::GetMode()
{
	return m_mode;
}
