//=============================================================================
// 
//  リトライUI処理 [retryui.cpp]
//  Author : 堀川萩大
// 
//=============================================================================
#include "retryui.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "player.h"
#include "keyconfig.h"
#include "keyconfig_gamepad.h"
#include "controlkeydisp.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_UI[3] = { "data\\TEXTURE\\dead\\restartbutton.png",
		"data\\TEXTURE\\dead\\retrybutton.png" ,
		"data\\TEXTURE\\dead\\posebutton.png" };	// テクスチャのファイル

	const MyLib::Vector3 DEST_POS[3] = { MyLib::Vector3(70.0f, 685.0f, 0.0f),
		MyLib::Vector3(640.0f, 685.0f, 0.0f),
		MyLib::Vector3(1210.0f, 685.0f, 0.0f) };	// 目標の位置

	const MyLib::Vector3 START_POS[3] = { MyLib::Vector3(-115.0f, 685.0f, 0.0f),
		MyLib::Vector3(640.0f, 900.0f, 0.0f),
		MyLib::Vector3(1415.0f, 685.0f, 0.0f) };	// 開始の位置

	const int USE_CONFIG[CRetry_Ui::BUTTON_MAX] =	// 使用するキーコンフィグ
	{
		INGAME::ACT_RETRY,
		INGAME::ACT_CHECKPOINT,
		INGAME::ACT_PAUSE,
	};

	const float UI_HEIGHT[3] = { 75.0f,100.0f,75.0f };// Uiの幅
	const float TIME_RETRY = 0.8f;		// リトライでボタンを押し続ける時間
	const float UI_MOVE_COEF = 0.04f;
}

//==========================================================================
// コンストラクタ
//==========================================================================
CRetry_Ui::CRetry_Ui(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
	{
		m_Button[cnt] = nullptr;
		m_apKeyDisp[cnt] = nullptr;
		m_DispPos[cnt] = 0.0f;
	}
	m_fRetryPushTime = 0.0f;	// リトライの押下時間
}

//==========================================================================
// デストラクタ
//==========================================================================
CRetry_Ui::~CRetry_Ui()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CRetry_Ui* CRetry_Ui::Create()
{
	// メモリの確保
	CRetry_Ui* pObj = DEBUG_NEW CRetry_Ui;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CRetry_Ui::Init()
{
	// インプット情報取得
	CKeyConfigManager* pKeyConfig = CKeyConfigManager::GetInstance();
	CKeyConfig* pPad = pKeyConfig->GetConfig(CKeyConfigManager::Control::CONTROL_INPAD);
	//// オブジェクト2Dの初期化
	//CObject2D::Init();

	int texID;
	D3DXVECTOR2 size;
	for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
	{
		m_Button[cnt] = CObject2D::Create();

		// オブジェクト2Dの初期化
		m_Button[cnt]->Init();

		// テクスチャ設定
		texID = CTexture::GetInstance()->Regist(TEXTURE_UI[cnt]);
		m_Button[cnt]->BindTexture(texID);

		// サイズ設定
		size = CTexture::GetInstance()->GetImageSize(texID);

#if 1	// 横幅を元にサイズ設定
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, UI_HEIGHT[cnt]);

#else	// 縦幅を元にサイズ設定
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, 320.0f);
#endif

		m_Button[cnt]->SetSize(size);
		m_Button[cnt]->SetSizeOrigin(size);

		// 位置の設定
		m_Button[cnt]->SetPosition(START_POS[cnt]);

		// 種類の設定
		m_Button[cnt]->SetType(CObject::TYPE::TYPE_OBJECT2D);

		// キーコンフィグを生成する
		if(m_apKeyDisp[cnt] == nullptr)
		{
			// 初期地点を設定
			MyLib::Vector3 pos = START_POS[cnt];
			pos.x += size.x * 0.7f;
			float range = size.y * 0.45f;

			// 目標地点も調整
			m_DispPos[cnt] = DEST_POS[cnt];
			m_DispPos[cnt].x += size.x * 0.5f;

			// 生成
			m_apKeyDisp[cnt] = CControlKeyDisp::Create(pos, 0.0f, range, range, pPad->GetKey(USE_CONFIG[cnt]));
		}
	}

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CRetry_Ui::Uninit()
{
	// 終了処理
	for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
	{
		if (m_Button[cnt] != nullptr)
		{
			m_Button[cnt]->Uninit();
			m_Button[cnt] = nullptr;
		}

		if (m_apKeyDisp[cnt] != nullptr)
		{
			m_apKeyDisp[cnt]->Uninit();
			m_apKeyDisp[cnt] = nullptr;
		}
	}

	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CRetry_Ui::Update()
{
	// インプット情報取得
	CKeyConfigManager* pKeyConfig = CKeyConfigManager::GetInstance();
	CKeyConfig* pPad = pKeyConfig->GetConfig(CKeyConfigManager::Control::CONTROL_INPAD);
	CKeyConfig* pKey = pKeyConfig->GetConfig(CKeyConfigManager::Control::CONTROL_INKEY);

	// フェード情報取得
	CInstantFade* pFade = CManager::GetInstance()->GetInstantFade();
	CInstantFade::STATE fadestate = pFade->GetState();

	// プレイヤー取得
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;
	playerList.ListLoop(&pPlayer);

	Moveui();
	if (pPad->GetTrigger(INGAME::ACT_CHECKPOINT) || pKey->GetTrigger(INGAME::ACT_CHECKPOINT))
	{// Yを入力している
		pPlayer->SetState(CPlayer::STATE::STATE_RETURN);
	}
	else if (pPad->GetPress(INGAME::ACT_RETRY) || pKey->GetTrigger(INGAME::ACT_RETRY))
	{// Xを入力している
		
		pPlayer->SetState(CPlayer::STATE::STATE_RESTART);
	}
	else
	{// 押されてないのでカウントリセット
		m_fRetryPushTime = 0.0f;
	}


	// 更新処理
	for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
	{
		m_Button[cnt]->Update();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CRetry_Ui::Draw()
{
	// プレイヤー取得
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;

	// リストループ
	while (playerList.ListLoop(&pPlayer))
	{
		if (pPlayer->GetState() == CPlayer::STATE_DEAD
			|| pPlayer->GetState() == CPlayer::STATE_RESPAWN
			|| pPlayer->GetState() == CPlayer::STATE_RETURN
			|| pPlayer->GetState() == CPlayer::STATE_RESTART)
		{
			// 描画処理
			for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
			{
				m_Button[cnt]->Draw();
			}
		}

	}
}

//==========================================================================
// 移動処理
//==========================================================================
void CRetry_Ui::Moveui()
{
	MyLib::Vector3 deffpos, changepos;
	for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
	{
		// 目標位置までの差を計算
		deffpos.x = (DEST_POS[cnt].x - m_Button[cnt]->GetPosition().x) * UI_MOVE_COEF;
		deffpos.y = (DEST_POS[cnt].y - m_Button[cnt]->GetPosition().y) * UI_MOVE_COEF;

		// 徐々に目標位置に移動
		MyLib::Vector3 changepos =
			MyLib::Vector3(m_Button[cnt]->GetPosition().x +
				deffpos.x, m_Button[cnt]->GetPosition().y +
				deffpos.y, m_Button[cnt]->GetPosition().z);

		// 位置を設定
		m_Button[cnt]->SetPosition(changepos);

		// キーコンフィグ表示も移動
		if (m_apKeyDisp[cnt] != nullptr)
		{
			// 目標位置までの差を計算
			deffpos.x = (m_DispPos[cnt].x - m_apKeyDisp[cnt]->GetPosition().x) * UI_MOVE_COEF;
			deffpos.y = (m_DispPos[cnt].y - m_apKeyDisp[cnt]->GetPosition().y) * UI_MOVE_COEF;

			// 目標位置を設定
			changepos =
				MyLib::Vector3(m_apKeyDisp[cnt]->GetPosition().x +
					deffpos.x, m_apKeyDisp[cnt]->GetPosition().y +
					deffpos.y, m_apKeyDisp[cnt]->GetPosition().z);

			// 位置を設定
			m_apKeyDisp[cnt]->SetPosition(changepos);
		}
	}
}