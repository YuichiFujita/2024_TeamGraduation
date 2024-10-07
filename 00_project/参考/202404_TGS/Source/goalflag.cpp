//=============================================================================
// 
//  ゴールフラグ処理 [goalflag.cpp]
//  Author : 日野澤匠泉のプログラム(お借りしてません)
// 
//=============================================================================
#include "goalflag.h"
#include "manager.h"
#include "calculation.h"
#include "player.h"
#include "game.h"
#include "stagecleartext.h"
#include "spline.h"
#include "course.h"
#include "input.h"
#include "camera.h"
#include "camera_motion.h"
#include "sound.h"
#include "blackframe.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\checkpoint\\goal_flag.x";
	const float TIME_SWING = 2.0f;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CGoalflagX::GOALFLAG_FUNC CGoalflagX::m_GoalflagFuncList[] =
{
	& CGoalflagX::SampleWho,	// フー
	& CGoalflagX::SampleWao,	// ワオ
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CGoalflagX> CGoalflagX::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CGoalflagX::CGoalflagX(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_fStateTime = 0.0f;	// 状態カウンター
	m_state = SAMPLE_WAO;	// 状態
	m_DestRot = MyLib::Vector3();		// 目標の向き
	m_bClear = false;
}

//==========================================================================
// デストラクタ
//==========================================================================
CGoalflagX::~CGoalflagX()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CGoalflagX* CGoalflagX::Create(float fLength)
{
	// メモリの確保
	CGoalflagX* pObj = DEBUG_NEW CGoalflagX;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();

		// 位置情報せってうい
		pObj->SetPosition(MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), fLength));
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CGoalflagX::Init()
{
	// リストに追加
	m_List.Regist(this);

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 初期化処理
	HRESULT hr = CObjectX::Init(MODEL);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// ゆらゆらの向き設定
	m_DestRot.x = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
	m_DestRot.y = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
	m_DestRot.z = UtilFunc::Transformation::Random(40, 80) * 0.001f;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CGoalflagX::Uninit()
{

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CGoalflagX::Kill()
{

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CGoalflagX::Update()
{
	// 状態カウンター加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 状態別処理
	(this->*(m_GoalflagFuncList[m_state]))();

	// 揺れ
	Swing();

	// 位置情報取得
	MyLib::Vector3 pos = CObjectX::GetPosition();

	// リストループ
	CListManager<CPlayer> PlayerList = CPlayer::GetListObj();
	CPlayer* pPlayer = PlayerList.GetData(0);
	
	// プレイヤーの位置情報取得
	MyLib::Vector3 Playerpos = pPlayer->GetPosition();

	if (m_bClear) { return; }

	CInputKeyboard* pKey = CInputKeyboard::GetInstance();

	if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL &&
		Playerpos.x >= pos.x || pKey->GetTrigger(DIK_3))
	{// ゴールしたっぺ

		// サウンド再生
		CSound::GetInstance()->StopSound(CSound::LABEL_BGM_GAME);
		CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_GOAL);

		// 必要なゲームの状態設定してね
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_GOAL);

		// カメラ設定
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->SetStateCameraV(DEBUG_NEW CStateCameraV_Goal);
		pCamera->GetMotion()->SetPosition(Playerpos);
		pCamera->GetMotion()->SetMotion(CCameraMotion::MOTION::MOTION_GOAL, CCameraMotion::EASING::Linear);

		// クリアテキスト生成
		CStageClearText::Create(MyLib::Vector3(640.0f, 400.0f, 0.0f));

		// ゴール時の設定
		pPlayer->GoalSetting();

		// 黒フレーム削除
		CBlackFrame::GetInstance()->SetState(CBlackFrame::STATE::STATE_IN);

		// クリア判定ＯＮ
		m_bClear = true;
	}
}

//==========================================================================
// 揺れ
//==========================================================================
void CGoalflagX::Swing()
{
	MyLib::Vector3 rot = GetRotation();

	// サインカーブ補間
	float ratio = UtilFunc::Correction::EasingEaseInOutSine(0.0f, 1.0f, 0.0f, TIME_SWING, m_fStateTime);
	rot = m_DestRot_Old + (m_DestRot - m_DestRot_Old) * ratio;
	SetRotation(rot);

	if (m_fStateTime >= TIME_SWING)
	{
		m_fStateTime = 0.0f;
		
		m_DestRot_Old = m_DestRot;
		
		m_DestRot.x = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
		m_DestRot.y = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
		m_DestRot.z *= -1;
	}
}

//==========================================================================
// フー
//==========================================================================
void CGoalflagX::SampleWho()
{

}

//==========================================================================
// ワオ
//==========================================================================
void CGoalflagX::SampleWao()
{

}

//==========================================================================
// 描画処理
//==========================================================================
void CGoalflagX::Draw()
{
	// 描画
	CObjectX::Draw();
}

