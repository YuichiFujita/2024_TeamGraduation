//=============================================================================
// 
//  リザルト処理 [result.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "result.h"
#include "resultManager.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "player.h"
#include "sound.h"
#include "game.h"

#include "result_people.h"
#include "mapmesh.h"
#include "peoplemanager.h"
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
CResultScore *CResult::m_pResultScore = nullptr;	// リザルトスクリーンのオブジェクト

//==========================================================================
// コンストラクタ
//==========================================================================
CResult::CResult() : m_clear(false)
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

	// BGM再生
	CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_RESULT);

	//=============================
	// 固定平面街フィールド
	//=============================
	CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_FIXEDPLANE_RESULT);

	//=============================
	// 人マネージャ
	//=============================
	m_pPeopleManager = CPeopleManager::Create(CPeopleManager::Type::TYPE_RESULT);


	// 黒フレームイン
	CBlackFrame::GetInstance()->SetState(CBlackFrame::STATE::STATE_INCOMPLETION);

	// カメラモーション再生
	CManager::GetInstance()->GetCamera()->GetCameraMotion()->SetMotion(CCameraMotion::MOTION::MOTION_RESULT, CCameraMotion::EASING::Linear);

	// リザルトの人生成
	CResultPeople::Create(MyLib::Vector3(76325.0f, 300.0f, 3060.0f));

	// 成功
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CResult::Uninit()
{
	m_pResultScore = nullptr;

	// 人マネージャ
	if (m_pPeopleManager != nullptr)
	{
		m_pPeopleManager->Uninit();
		m_pPeopleManager = nullptr;
	}

	// リザルトマネージャのリセット
	CResultManager* pResultManager = CResultManager::GetInstance();
	pResultManager->Reset();

	// 終了処理
	CScene::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CResult::Update()
{
	// シーンの更新処理
	CScene::Update();

	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();

	// リザルト画面
	CResultManager* pResultManager = CResultManager::GetInstance();
	pResultManager->Update();

	if (pResultManager->GetState() != CResultManager::State::STATE_PRESSENTER) return;

	// 画面遷移
	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{
		// モード設定
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RANKING);
	}

}

//==========================================================================
// 描画処理
//==========================================================================
void CResult::Draw()
{

}

//==========================================================================
// リザルトスクリーンの情報取得
//==========================================================================
CResultScore *CResult::GetResultScore()
{
	return m_pResultScore;
}

