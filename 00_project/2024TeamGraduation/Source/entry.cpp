//=============================================================================
// 
//  エントリー処理 [entry.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "entry.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "sound.h"
#include "MyEffekseer.h"
#include "camera.h"

// シーン
#include "entryscene.h"
#include "entry_setupTeam.h"


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
CEntry* CEntry::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// 関数ポインタ
//==========================================================================
CEntry::SCENE_FUNC CEntry::m_SceneFunc[] =
{
	&CEntry::SceneNone,			// なにもなし
	&CEntry::SceneFadeInLogo,	// ロゴフェードイン
	&CEntry::SceneFadeOutLoGo,	// ロゴフェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CEntry::CEntry()
{
	// 値のクリア
	m_SceneType = ESceneType::SCENETYPE_NONE;	// シーンの種類
	m_fSceneTime = 0.0f;						// シーンカウンター
}

//==========================================================================
// デストラクタ
//==========================================================================
CEntry::~CEntry()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CEntry* CEntry::Create()
{
	if (m_pThisPtr == nullptr)
	{// まだ生成していなかったら

		// インスタンス生成
		m_pThisPtr = DEBUG_NEW CEntry;
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEntry::Init()
{

	// BGM再生
	CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_TITLE);

	// 初期化処理
	if (FAILED(CScene::Init()))
	{// 失敗した場合
		return E_FAIL;
	}

	// シーン設定
	SetSceneType(ESceneType::SCENETYPE_SETUPTEAM);

	// エントリーシーン生成
	m_pEntryScene = DEBUG_NEW CEntry_SetUpTeam;

	// 成功
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEntry::Uninit()
{
	m_pThisPtr = nullptr;

	// 終了処理
	CScene::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CEntry::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// フェード中は抜ける
		return;
	}

	m_pEntryScene->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 状態別更新処理
	(this->*(m_SceneFunc[m_SceneType]))(fDeltaTime, fDeltaRate, fSlowRate);


	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pKey->GetTrigger(DIK_RETURN))
	{
		// 遷移
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_GAME);
	}
}

//==========================================================================
// なにもなし
//==========================================================================
void CEntry::SceneNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
}

//==========================================================================
// ロゴフェードイン
//==========================================================================
void CEntry::SceneFadeInLogo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ロゴフェードアウト
//==========================================================================
void CEntry::SceneFadeOutLoGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CEntry::Draw()
{
}

//==========================================================================
// シーン設定
//==========================================================================
void CEntry::SetSceneType(ESceneType type)
{
	m_SceneType = type;
	m_fSceneTime = 0.0f;
}
