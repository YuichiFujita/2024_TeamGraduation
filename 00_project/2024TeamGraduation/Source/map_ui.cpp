//=============================================================================
//
// マップUI処理 [map_ui.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "map_ui.h"
#include "object2D.h"
#include "game.h"
#include "course.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"
#include "player.h"
#include "texture.h"
#include "goalflag.h"

//=============================================================================
// 定数定義
//=============================================================================
namespace
{
	const MyLib::Vector3 DEF_POS = MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.95f, 0.0f);
	const std::string TEXTURELIST[CMapUI::TYPE::TYPE_MAX] = {	// テクスチャリスト
		"data\\TEXTURE\\map_ui\\map.png",
		"data\\TEXTURE\\map_ui\\player.png",
		"data\\TEXTURE\\map_ui\\water.png",
	};

	const float HEIGHTLIST[CMapUI::TYPE::TYPE_MAX] = {	// 高さリスト
		100.0f,
		50.0f,
		100.0f,
	};

	const float STARTX = SCREEN_WIDTH * 0.09f;
	const float GOALX = SCREEN_WIDTH * 0.9f;
	const float ADD_TIME = (1.0f / 25.0f);
	const float WATER_MOVE = (1.0f / 1800.0f);
	const float UPDOWN_HEIGHT = (6.0f);
}

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
// ファイル名

// インスタンス
CMapUI* CMapUI::m_pInstance = nullptr;

//=============================================================================
// コンストラクタ
//=============================================================================
CMapUI::CMapUI() : CObject(7)
{
	// 値のクリア
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		m_apObj[i] = nullptr;
	}

	m_pMyCamera = nullptr;
	m_pMyPlayer = nullptr;
	m_fSin = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMapUI::~CMapUI()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMapUI::Init()
{
	SetType(CObject::TYPE::TYPE_UI);

	// ゲーム画面が存在しているか、コースがあるか確認
	if (CGame::GetInstance() == nullptr) { return E_FAIL; }
	if (CGame::GetInstance()->GetCourse() == nullptr) { return E_FAIL; }

	// オブジェクトの生成と設定
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		m_apObj[i] = CObject2D::Create(1);
		int texidx = CTexture::GetInstance()->Regist(TEXTURELIST[i]);
		m_apObj[i]->BindTexture(texidx);
		m_apObj[i]->SetType(CObject::TYPE_UI);
		m_apObj[i]->SetPosition(DEF_POS);
		m_apObj[i]->SetRotation(0.0f);
		D3DXVECTOR2 texture = CTexture::GetInstance()->GetImageSize(m_apObj[i]->GetIdxTexture());
		m_apObj[i]->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, HEIGHTLIST[i]));
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMapUI::Uninit()
{
	// オブジェクトの廃棄
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		if (m_apObj[i] != nullptr)
		{
			m_apObj[i] = nullptr;
		}
	}

	// カメラの廃棄
	if (m_pMyCamera != nullptr)
	{
		m_pMyCamera->Uninit();
		delete m_pMyCamera;
		m_pMyCamera = nullptr;
	}

	m_pMyPlayer = nullptr;
	m_pInstance = nullptr;

	CObject::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMapUI::Update()
{
	// マップの回転処理
	SetMapPosition();

	m_fSin += ADD_TIME;
}

//=============================================================================
// 描画処理
//=============================================================================
void CMapUI::Draw()
{
	
}

//=============================================================================
// 生成
//=============================================================================
CMapUI* CMapUI::Create()
{
	if (m_pInstance != nullptr) { return m_pInstance; }
	m_pInstance = DEBUG_NEW CMapUI;

	if (m_pInstance->Init() == E_FAIL)
	{
		// 開放
		Release();
	}

	return m_pInstance;
}

//=============================================================================
// 開放
//=============================================================================
void CMapUI::Release()
{
	if (m_pInstance == nullptr) { return; }

	// 終了処理
	m_pInstance = nullptr;
}

//=============================================================================
// 移動処理
//=============================================================================
void CMapUI::SetMapPosition()
{
	CGame* pgame = CGame::GetInstance();

	// ゲーム画面か確認
	if (pgame == nullptr) { return; }

	// プレイヤーがいるか確認
	if (m_pMyPlayer == nullptr) { return; }

	// プレイヤーの進行の割合を求めて移動
	float courselen = CGoalflagX::GetListObj().GetData(0)->GetPosition().x;
	float playerlen = m_pMyPlayer->GetPosition().x;
	m_fRatio = playerlen / courselen;

	float x = UtilFunc::Correction::EasingLinear(STARTX, GOALX, 0.0f, 1.0f, m_fRatio);

	MyLib::Vector3 pos = m_apObj[TYPE::TYPE_PLAYER]->GetPosition();
	pos.x = x;
	pos.y = (DEF_POS.y + UPDOWN_HEIGHT) + sinf(m_fSin) * UPDOWN_HEIGHT;
	m_apObj[TYPE::TYPE_PLAYER]->SetPosition(pos);
	D3DXVECTOR2* ptex = m_apObj[TYPE::TYPE_WATER]->GetTex();
	for (int i = 0; i < 4; i++)
	{
		ptex[i].x -= WATER_MOVE;
	}
}
