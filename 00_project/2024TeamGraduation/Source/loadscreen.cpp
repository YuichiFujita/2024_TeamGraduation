//=============================================================================
// 
//  フェード処理 [loadscreen.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "loadscreen.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int NUM_STRING = 10;	// NOWLOADINGの文字数
	const std::string TEXPATH[NUM_STRING] =
	{
		"data\\TEXTURE\\load\\n.png",
		"data\\TEXTURE\\load\\o.png",
		"data\\TEXTURE\\load\\w.png",
		"data\\TEXTURE\\load\\l.png",
		"data\\TEXTURE\\load\\o.png",
		"data\\TEXTURE\\load\\a.png",
		"data\\TEXTURE\\load\\d.png",
		"data\\TEXTURE\\load\\i.png",
		"data\\TEXTURE\\load\\mini_n.png",
		"data\\TEXTURE\\load\\g.png",
	};
	const std::string TEXTURE_CYLINDER = "data\\TEXTURE\\load\\cylinder.png";
	const MyLib::Vector3 STR_DEFPOS = MyLib::Vector3(150.0f, 300.0f, 0.0f);	// 基点の位置
	const MyLib::Vector3 DEFAULTPOS_CYLINDER = MyLib::Vector3(-300.0f, 600.0f, 0.0f);	// 基点の位置
	const float STR_HEIGHT = 100.0f;		// 文字列の高さ
	const float CYLINDER_HEIGHT = 100.0f;	// 筒の高さ
}


//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CLoadScreen::CLoadScreen()
{
	// 値のクリア
	for (int i = 0; i < NUM_STRING; i++)
	{
		m_apObj2D[i] = nullptr;
	}

	// 筒
	m_pCylinder = nullptr;
	m_fBobbingTime = 0.0f;		// ぷかぷかタイマー
}

//==========================================================================
// デストラクタ
//==========================================================================
CLoadScreen::~CLoadScreen()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CLoadScreen* CLoadScreen::Create()
{
	// メモリの確保
	CLoadScreen* pFade = DEBUG_NEW CLoadScreen;

	if (pFade != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		if (FAILED(pFade->Init()))
		{// 失敗していたら
			return nullptr;
		}
	}

	return pFade;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CLoadScreen::Init()
{
	
	MyLib::Vector3 pos = STR_DEFPOS;
	CTexture* pTex = CTexture::GetInstance();

	// 文字生成
	for (int i = 0; i < NUM_STRING; i++)
	{
		m_apObj2D[i] = CObject2D::Create();
		m_apObj2D[i]->SetType(CObject::TYPE::TYPE_NONE);
		m_apObj2D[i]->SetPosition(pos);
		m_apObj2D[i]->SetOriginPosition(pos);

		// テクスチャ割り当て
		int nIdx = pTex->Regist(TEXPATH[i]);
		m_apObj2D[i]->BindTexture(nIdx);

		// サイズ変更
		D3DXVECTOR2 size = UtilFunc::Transformation::AdjustSizeByHeight(pTex->GetImageSize(nIdx), STR_HEIGHT);
		m_apObj2D[i]->SetSize(size);

		// 生成位置ずらす
		pos.x += size.x * 2.0f;

		if (i == 2)
		{// NOWとLoadingの間
			pos.x += size.x * 2;
		}
	}

	// 筒生成
	CreateCylinder();

	return S_OK;
}

//==========================================================================
// 筒生成
//==========================================================================
void CLoadScreen::CreateCylinder()
{
	// 筒生成
	m_pCylinder = CObject2D::Create();
	m_pCylinder->SetType(CObject::TYPE::TYPE_NONE);
	m_pCylinder->SetPosition(DEFAULTPOS_CYLINDER);
	m_pCylinder->SetOriginPosition(DEFAULTPOS_CYLINDER);

	// テクスチャ割り当て
	int nIdx = CTexture::GetInstance()->Regist(TEXTURE_CYLINDER);
	m_pCylinder->BindTexture(nIdx);

	// サイズ変更
	D3DXVECTOR2 size = UtilFunc::Transformation::AdjustSizeByHeight(CTexture::GetInstance()->GetImageSize(nIdx), CYLINDER_HEIGHT);
	m_pCylinder->SetSize(size);

}

//==========================================================================
// 終了処理
//==========================================================================
void CLoadScreen::Uninit()
{

	for (int i = 0; i < NUM_STRING; i++)
	{
		if (m_apObj2D[i] == nullptr)
		{
			m_apObj2D[i]->Uninit();
			m_apObj2D[i] = nullptr;
		}
	}

	if (m_pCylinder != nullptr)
	{
		m_pCylinder->Uninit();
		m_pCylinder = nullptr;
	}
}

//==========================================================================
// 削除処理
//==========================================================================
void CLoadScreen::Kill()
{
	for (int i = 0; i < NUM_STRING; i++)
	{
		if (m_apObj2D[i] != nullptr)
		{
			m_apObj2D[i]->Uninit();
			delete m_apObj2D[i];
			m_apObj2D[i] = nullptr;
		}
	}

	if (m_pCylinder != nullptr)
	{
		m_pCylinder->Uninit();
		delete m_pCylinder;
		m_pCylinder = nullptr;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CLoadScreen::Update()
{
	// 文字生成
	for (int i = 0; i < NUM_STRING; i++)
	{
		if (m_apObj2D[i] == nullptr) continue;

		// アドレス渡し
		CObject2D* pObj2D = m_apObj2D[i];

		// 情報取得
		MyLib::Vector3 pos = pObj2D->GetPosition(), move = pObj2D->GetMove(), rot = pObj2D->GetRotation();
		MyLib::Vector3 posOrigin = pObj2D->GetOriginPosition();
		pos.y += move.y;
		move.y += 0.15f;

		// 回転
		rot.z = UtilFunc::Correction::EasingEaseIn(0.0f, D3DX_PI * -0.5f, posOrigin.y, posOrigin.y - 200.0f, pos.y);

		if (posOrigin.y <= pos.y)
		{// 着地
			move.y = 0.0f;
			pos.y = posOrigin.y;
		}

		// 情報設定
		pObj2D->SetPosition(pos);
		pObj2D->SetMove(move);
		pObj2D->SetRotation(rot);

		// 更新処理
		pObj2D->Update();
	}

	// 筒の動き
	MoveCylinder();
	if (m_pCylinder != nullptr)
	{
		m_pCylinder->Update();
	}

	// 文字との判定
	CollisionText();
}

//==========================================================================
// 文字との判定
//==========================================================================
void CLoadScreen::CollisionText()
{
	if (m_pCylinder == nullptr) return;

	// 位置取得
	MyLib::Vector3 pos = m_pCylinder->GetPosition();

	MyLib::Vector3 textpos;
	D3DXVECTOR2 textsize;
	for (const auto& text : m_apObj2D)
	{
		// 情報取得
		textpos = text->GetPosition();
		textsize = text->GetSize();

		if (pos.x >= textpos.x - textsize.x &&
			pos.x <= textpos.x + textsize.x &&
			textpos.IsNearlyTargetY(text->GetOriginPosition().y, 0.1f))
		{// 判定 && 着地

			// 吹上げ
			text->SetMove(MyLib::Vector3(0.0f, -5.0f, 0.0f));

		}
	}

}

//==========================================================================
// 筒の動き
//==========================================================================
void CLoadScreen::MoveCylinder()
{
	if (m_pCylinder == nullptr) return;

	// 位置取得
	MyLib::Vector3 pos = m_pCylinder->GetPosition(), posOrigin = m_pCylinder->GetOriginPosition();

	// 横移動
	pos.x += 5.0f;

	// ぷかぷか
	m_fBobbingTime += CManager::GetInstance()->GetDeltaTime();
	pos.y = posOrigin.y + sinf(D3DX_PI * (m_fBobbingTime / 0.8f)) * 10.0f;

	// 折り返し
	if (pos.x >= 1280.0f + m_pCylinder->GetSize().x)
	{
		pos.x = -100.0f;
	}

	// 位置設定
	m_pCylinder->SetPosition(pos);
}

//==========================================================================
// 描画処理
//==========================================================================
void CLoadScreen::Draw()
{
	// 文字描画
	for (int i = 0; i < NUM_STRING; i++)
	{
		if (m_apObj2D[i] != nullptr)
		{
			m_apObj2D[i]->Draw();
		}
	}

	if (m_pCylinder != nullptr)
	{
		m_pCylinder->Draw();
	}
}
