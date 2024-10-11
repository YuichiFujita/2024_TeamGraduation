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
	m_SceneType = SCENETYPE::SCENETYPE_NONE;	// シーンの種類
	m_fSceneTime = 0.0f;						// シーンカウンター
	m_pLogo = nullptr;		// ロゴのポインタ
	m_pPressEnter = nullptr;	// プレスエンター
	m_pConfigSetting = nullptr;
	m_pSpawn_Leaf = nullptr;		// 降る葉生成
	m_pPeopleManager = nullptr;	// 人マネージャ
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
	m_SceneType = SCENETYPE::SCENETYPE_NONE;

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
	// シーンカウンター減算
	m_fSceneTime -= fDeltaTime * fSlowRate;

	// 不透明度更新
	float alpha = m_fSceneTime / TIME_FADELOGO;

	if (m_fSceneTime <= 0.0f)
	{
		m_fSceneTime = 0.0f;
		m_SceneType = SCENETYPE_NONE;
		return;
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CTitle::Draw()
{

}
