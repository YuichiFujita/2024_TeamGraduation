//=============================================================================
// 
//  リザルト処理 [result.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "player.h"
#include "sound.h"
#include "game.h"
#include "resultmanager.h"

#include "blackframe.h"
#include "camera.h"

//=============================================================================
// 定数定義
//=============================================================================
namespace
{
	const D3DXCOLOR WIN_COLOR = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	const D3DXCOLOR LOSE_COLOR = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	const float START_LENGTH = 300.0f; // 初期距離
	const float END_LENGTH = 2000.0f; // 目標距離

	//モデルの配置位置
	const MyLib::Vector3 CENTER = MyLib::Vector3(0.0f, 0.0f, -2000.0f);
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CResult* CResult::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CResult::CResult() : m_pResultManager(nullptr)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CResult::~CResult()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CResult::Init()
{
	//プレイヤー数をリセット
	CManager::GetInstance()->SetNumPlayer(0);

	// 初期化処理
	if (FAILED(CScene::Init()))
	{// 失敗した場合
		return E_FAIL;
	}

	// リザルトマネージャ生成
	SAFE_UNINIT(m_pResultManager);
	m_pResultManager = CResultManager::Create();

	// BGM再生
	CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_RESULT);

	// 成功
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CResult::Uninit()
{
	SAFE_UNINIT(m_pResultManager);

	// 終了処理
	CScene::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CResult::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// シーンの更新処理
	CScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	if (m_pResultManager != nullptr)
	{
		m_pResultManager->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// インプット情報取得
	CInputKeyboard * pKey = CInputKeyboard::GetInstance();
	CInputGamepad *pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pKey->GetTrigger(DIK_RETURN))
	{
		// 遷移
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_TITLE);
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CResult::Draw()
{

}

