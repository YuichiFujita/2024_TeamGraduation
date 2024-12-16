//=============================================================================
// 
//  タイトル処理 [title.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "sound.h"
#include "particle.h"
#include "MyEffekseer.h"
#include "titleLogo.h"
#include "titlestudent.h"
#include "titlescene.h"
#include "camera.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float TIME_FADELOGO = 0.6f;	// ロゴのフェードアウト時間
	const char* TEXTURE = "data\\TEXTURE\\title\\title.png";
	const int TITLEPLAYER_MIN = 7;	// プレイヤー最少人数
	const int TITLEPLAYER_MAX = 12;	// プレイヤー最大人数
}

namespace STARTCAMERA
{
	const MyLib::Vector3 POSV = (321.91f, 233.25f, -654.0f);
	const MyLib::Vector3 POSR = (321.91f, 160.74f, 296.27f);
	const MyLib::Vector3 ROT = (0.0f, 0.0f, -0.2f);
	const float LENGTH = 365.0f;
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CTitle* CTitle::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// 関数ポインタ
//==========================================================================
CTitle::SCENE_FUNC CTitle::m_SceneFunc[] =
{
	&CTitle::SceneNone,			// なにもなし
	&CTitle::SceneFadeInLogo,	// ロゴフェードイン
	&CTitle::SceneFadeOutLoGo,	// ロゴフェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CTitle::CTitle()
{
	// 値のクリア
	m_SceneType = ESceneType::SCENETYPE_NONE;	// シーンの種類
	m_fSceneTime = 0.0f;						// シーンカウンター
	m_pLogo = nullptr;		// ロゴのポインタ
	m_pTitleScene = nullptr;		// タイトルシーン
}

//==========================================================================
// デストラクタ
//==========================================================================
CTitle::~CTitle()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTitle* CTitle::Create()
{
	if (m_pThisPtr == nullptr)
	{// まだ生成していなかったら

		// インスタンス生成
		m_pThisPtr = DEBUG_NEW CTitle;
	}

	return m_pThisPtr;
}

//==========================================================================
// インスタンス取得
//==========================================================================
CTitle* CTitle::GetInstance()
{
	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTitle::Init()
{

	// BGM再生
	CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_TITLE);

	// 初期化処理
	if (FAILED(CScene::Init()))
	{// 失敗した場合
		return E_FAIL;
	}

	// シーンの種類
	ChangeScene(ESceneType::SCENETYPE_SUSURU);

	// ロゴの生成
	CTitleLogo::Create();

	// プレイヤー生成
	int num = UtilFunc::Transformation::Random(TITLEPLAYER_MIN, TITLEPLAYER_MAX);
	for (int i = 0; i < num; i++)
	{
		// タイトルの生徒生成
		CTitleStudent::Create();
	}

	// 成功
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTitle::Uninit()
{
	m_pThisPtr = nullptr;

	// 終了処理
	CScene::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTitle::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// フェード中は抜ける
		return;
	}

	// シーンの更新
	if (m_pTitleScene != nullptr)
	{
		m_pTitleScene->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pKey->GetTrigger(DIK_RETURN))
	{
		// 遷移
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_ENTRY);
	}


#if _DEBUG

	// 生成
	if (ImGui::TreeNode("Create"))
	{
		if (ImGui::Button("TitleStudent"))
		{
			// タイトルの生徒生成
			CTitleStudent::Create();
		}

		// ツリー終端
		ImGui::TreePop();
	}
#endif
}

//==========================================================================
// シーンの更新処理
//==========================================================================
void CTitle::UpdateScene(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// シーンカウンター加算
	m_fSceneTime += fDeltaTime * fSlowRate;

	// 状態別更新処理
	(this->*(m_SceneFunc[m_SceneType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// なにもなし
//==========================================================================
void CTitle::SceneNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
}

//==========================================================================
// ロゴフェードイン
//==========================================================================
void CTitle::SceneFadeInLogo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ロゴフェードアウト
//==========================================================================
void CTitle::SceneFadeOutLoGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
}

//==========================================================================
// シーン切り替え
//==========================================================================
void CTitle::ChangeScene(ESceneType type)
{
	// 終了処理
	SAFE_UNINIT(m_pTitleScene);
	

	// 生成
	m_pTitleScene = CTitleScene::Create(type);

	// シーン設定
	m_SceneType = type;
}

//==========================================================================
// 描画処理
//==========================================================================
void CTitle::Draw()
{

}
