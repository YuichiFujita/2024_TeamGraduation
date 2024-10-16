//=============================================================================
// 
//  サンプル_オブジェクト3D処理 [sample_obj3D.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "sample_obj3D.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\key\\A.png";	// テクスチャのファイル
}

//==========================================================================
// コンストラクタ
//==========================================================================
CSample_Obj3D::CSample_Obj3D(int nPriority) : CObject3D(nPriority)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CSample_Obj3D::~CSample_Obj3D()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CSample_Obj3D* CSample_Obj3D::Create()
{
	// メモリの確保
	CSample_Obj3D* pObj = DEBUG_NEW CSample_Obj3D;

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
HRESULT CSample_Obj3D::Init()
{

	// オブジェクト3Dの初期化
	CObject3D::Init();

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

	MyLib::Vector3 setsize = MyLib::Vector3(size.x, size.y, 0.0f);
	SetSize(setsize);
	SetSizeOrigin(setsize);


	// 位置、向き設定は必要があれば追加

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CSample_Obj3D::Uninit()
{
	// 終了処理
	CObject3D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CSample_Obj3D::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 更新処理
	CObject3D::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CSample_Obj3D::Draw()
{
	// 描画処理
	CObject3D::Draw();
}
