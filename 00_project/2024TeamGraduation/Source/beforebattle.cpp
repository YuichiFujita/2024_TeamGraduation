//=============================================================================
// 
//  戦闘準備処理 [beforebattle.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "beforebattle.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "input.h"
#include "battlestart.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\battlestart\\yoi.png";	// テクスチャのファイル
	const std::string PRESS_TEXTURE = "data\\TEXTURE\\battlestart\\beforebattle_button.png";	// テクスチャのファイル
}

//==========================================================================
// コンストラクタ
//==========================================================================
CBeforeBattle::CBeforeBattle(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_pAnyButton= nullptr;	// 「キー押して」のテキスト
}

//==========================================================================
// デストラクタ
//==========================================================================
CBeforeBattle::~CBeforeBattle()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CBeforeBattle* CBeforeBattle::Create(const MyLib::Vector3& pos)
{
	// メモリの確保
	CBeforeBattle* pMarker = DEBUG_NEW CBeforeBattle;

	if (pMarker != nullptr)
	{// メモリの確保が出来ていたら

		// 位置設定
		pMarker->SetPosition(pos);
		pMarker->SetOriginPosition(pos);

		// 初期化処理
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CBeforeBattle::Init()
{

	// オブジェクト2Dの初期化
	CObject2D::Init();

	// テクスチャ設定
	int pointTex = CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	BindTexture(pointTex);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(pointTex);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
	SetSize(size);
	SetSizeOrigin(size);

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);



	// テキスト生成
	m_pAnyButton = CObject2D::Create(GetPriority());
	m_pAnyButton->SetPosition(GetPosition() + MyLib::Vector3(0.0f, 70.0f, 0.0f));

	// テクスチャ設定
	int textTex = CTexture::GetInstance()->Regist(PRESS_TEXTURE);
	m_pAnyButton->BindTexture(textTex);

	// サイズ設定
	size = CTexture::GetInstance()->GetImageSize(textTex);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, 30.0f);
	m_pAnyButton->SetSize(size);
	m_pAnyButton->SetSizeOrigin(size);
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CBeforeBattle::Uninit()
{
	if (m_pAnyButton != nullptr)
	{
		m_pAnyButton->Uninit();
		m_pAnyButton = nullptr;
	}

	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CBeforeBattle::Update()
{
	// 入力情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_Y, 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_START, 0) ||
		pInputKeyboard->GetTrigger(DIK_RETURN) ||
		pInputKeyboard->GetTrigger(DIK_SPACE) ||
		pInputKeyboard->GetTrigger(DIK_BACKSPACE) 
		)
	{
		// 戦闘開始に遷移
		CBattleStart::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f));
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_BATTLESTART);

		// 終了処理
		Uninit();
		return;
	}

	// 更新処理
	CObject2D::Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CBeforeBattle::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// オブジェクト2Dの描画
	CObject2D::Draw();

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}
