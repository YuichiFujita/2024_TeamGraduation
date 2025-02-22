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
#include "gymDoor.h"

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

// ドッジボールコート情報
namespace Gym
{
	const MyLib::Vector3 POS_LEFT = MyLib::Vector3(-972.85f, 0.0f, 1717.35f);	// ドア左位置
	const MyLib::Vector3 POS_RIGHT = MyLib::Vector3(972.85f, 0.0f, 1717.35f);	// ドア右位置
	const MyLib::Vector3 POS[] = { POS_LEFT, POS_RIGHT };	// ドア位置
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
	m_pEntryScene = nullptr;		// エントリーシーン
	m_pSetupTeam = nullptr;	// チーム等の設定取得
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
	// 初期化処理
	if (FAILED(CScene::Init()))
	{// 失敗した場合
		return E_FAIL;
	}

	// シーン設定
	SetSceneType(ESceneType::SCENETYPE_SETUPTEAM);

	// エントリーシーン生成
	m_pEntryScene = CEntryScene::Create(ESceneType::SCENETYPE_SETUPTEAM);

	for (int i = 0; i < CGameManager::EDoor::DOOR_MAX; i++)
	{ // ドアの配置数分繰り返す

		// 体育館ドア生成
		CGymDoor* pGymDoor = CGymDoor::Create(Gym::POS[i]);
		if (pGymDoor == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}
	}

	// BGM再生
	CSound::GetInstance()->PlaySound(CSound::ELabel::LABEL_BGM_ENTRY);

	// 成功
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEntry::Uninit()
{
	// エントリーシーン終了
	if (m_pEntryScene != nullptr)
	{
		m_pEntryScene->Uninit();
		m_pEntryScene = nullptr;
	}

	if (m_pSetupTeam != nullptr)
	{
		m_pSetupTeam->Uninit();
		m_pSetupTeam = nullptr;
	}

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

	// シーン毎の更新
	m_pEntryScene->Update(fDeltaTime, fDeltaRate, fSlowRate);

#if 0
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
#endif
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
// エントリーシーン切り替え
//==========================================================================
void CEntry::ChangeEntryScene(ESceneType type)
{
	// 終了処理
	if (m_pEntryScene != nullptr &&
		m_SceneType != ESceneType::SCENETYPE_SETUPTEAM)
	{
		m_pEntryScene->Uninit();
		m_pEntryScene = nullptr;
	}
	else if (m_SceneType == ESceneType::SCENETYPE_SETUPTEAM)
	{// チーム設定保存

		// チーム等の設定取得
		m_pSetupTeam = m_pEntryScene->GetSetupTeam();
	}

	// 生成
	m_pEntryScene = CEntryScene::Create(type);

	// シーン設定
	m_SceneType = type;
}

//==========================================================================
// シーン設定
//==========================================================================
void CEntry::SetSceneType(ESceneType type)
{
	m_SceneType = type;
	m_fSceneTime = 0.0f;
}
