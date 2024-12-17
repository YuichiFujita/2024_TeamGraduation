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

#include "titlescene.h"
#include "camera.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float TIME_FADELOGO = 0.6f;	// ロゴのフェードアウト時間
	const char* TEXTURE = "data\\TEXTURE\\title\\title.png";
	
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
CTitle::STATE_FUNC CTitle::m_StateFunc[] =
{
	&CTitle::StateNone,			// なにもなし
	&CTitle::StateChangeScene,	// シーン切り替え
};

//==========================================================================
// コンストラクタ
//==========================================================================
CTitle::CTitle()
{
	// 値のクリア
	m_state = EState::STATE_NONE;				// 状態
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

	// シーン生成
	m_pTitleScene = CTitleScene::Create(ESceneType::SCENETYPE_CONTROLLWAIT);

	// シーン設定
	m_SceneType = ESceneType::SCENETYPE_CONTROLLWAIT;

	// 成功
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTitle::Uninit()
{
	m_pThisPtr = nullptr;

	// シーン終了
	SAFE_UNINIT(m_pTitleScene);

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

	// 状態更新処理
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 状態更新処理
//==========================================================================
void CTitle::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// シーンカウンター加算
	m_fSceneTime += fDeltaTime * fSlowRate;

	// 状態別更新処理
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// なにもなし
//==========================================================================
void CTitle::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// シーンの更新
	if (m_pTitleScene != nullptr)
	{
		m_pTitleScene->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// シーン切り替え
//==========================================================================
void CTitle::StateChangeScene(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// フェード完了したらシーン切り替え
	if (GET_MANAGER->GetInstantFade()->IsCompletion())
	{
		// 削除処理
		SAFE_KILL(m_pTitleScene);

		// シーン生成
		m_pTitleScene = CTitleScene::Create(m_SceneType);

		// カメラリセット
		GET_MANAGER->GetCamera()->ResetByMode(CScene::MODE::MODE_TITLE);

		// NONE状態
		m_state = EState::STATE_NONE;
	}
}

//==========================================================================
// シーン切り替え
//==========================================================================
void CTitle::ChangeScene(ESceneType type)
{
	// シーン設定
	m_SceneType = type;

	// フェード設定
	GET_MANAGER->GetInstantFade()->SetFade();

	// シーン切り替え状態
	m_state = EState::STATE_CHANGESCENE;
}

//==========================================================================
// 描画処理
//==========================================================================
void CTitle::Draw()
{

}
