//=============================================================================
// 
//  ガラス割れる処理 [sample_obj2D.cpp]
//  Author : 日野澤匠泉様
// 
//=============================================================================
#include "glassclush.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "renderer.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\result\\grass_crush.png";	// テクスチャのファイル
}

CGlassclush *CGlassclush::m_ThisPtr = nullptr;

//==========================================================================
// コンストラクタ
//==========================================================================
CGlassclush::CGlassclush(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CGlassclush::~CGlassclush()
{

}

//==========================================================================
// 削除
//==========================================================================
void CGlassclush::Kill()
{
	if (m_ThisPtr != nullptr)
	{
		m_ThisPtr->Uninit();
		m_ThisPtr = nullptr;
	}
}

//==========================================================================
// 生成処理
//==========================================================================
CGlassclush* CGlassclush::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// メモリの確保
		m_ThisPtr = DEBUG_NEW CGlassclush;

		if (m_ThisPtr != nullptr)
		{
			// 初期化処理
			m_ThisPtr->Init();
		}
	}

	return m_ThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CGlassclush::Init()
{

	// オブジェクト2Dの初期化
	CObject2D::Init();

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SAMPLE);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
#endif
	SetSize(D3DXVECTOR2(640.0f,360.0f));
	SetSizeOrigin(size);
	SetPosition(MyLib::Vector3(640.0f,360.0f,0.0f));

	SetAlpha(0.0f);

	// 位置、向き設定は必要があれば追加

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// ブラーの削除
	CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(0.0f, 0.0f, 60.0f);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CGlassclush::Uninit()
{
	// 終了処理
	CObject2D::Uninit();

	m_ThisPtr = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CGlassclush::Update()
{
	float alpha = GetAlpha();

	alpha += 0.1f;

	SetAlpha(alpha);

	if (alpha >= 1.0f)
	{
		SetAlpha(1.0f);
	}

	// 更新処理
	CObject2D::Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CGlassclush::Draw()
{
	// 描画処理
	CObject2D::Draw();
}
