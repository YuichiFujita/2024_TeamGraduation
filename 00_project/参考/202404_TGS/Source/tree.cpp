//=============================================================================
// 
//  tree処理 [tree.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "tree.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_TREE_000 = "data\\TEXTURE\\tree\\trees_001.png";	// テクスチャのファイル
	const std::string TEXTURE_TREE_001 = "data\\TEXTURE\\tree\\trees_002.png";	// テクスチャのファイル
}

//==========================================================================
// コンストラクタ
//==========================================================================
CTree::CTree(int nPriority, const LAYER layer) : CObject3D(nPriority, layer)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CTree::~CTree()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTree* CTree::Create(MyLib::Vector3 pos)
{
	// メモリの確保
	CTree* pObj = DEBUG_NEW CTree;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();
		pObj->SetPosition(pos);
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTree::Init()
{
	int choice = rand() % 2;
	int texID;

	// オブジェクト3Dの初期化
	CObject3D::Init();

	if (choice == 0)
	{
		// テクスチャ設定
		texID = CTexture::GetInstance()->Regist(TEXTURE_TREE_000);
		BindTexture(texID);

	}
	else
	{
		// テクスチャ設定
		texID = CTexture::GetInstance()->Regist(TEXTURE_TREE_001);
		BindTexture(texID);

	}

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 1000.0f);
#endif

	MyLib::Vector3 setsize = MyLib::Vector3(size.x, size.y, 0.0f);
	SetSize(setsize);
	//SetPosition(MyLib::Vector3(-2223.0f,900.0f,10128.0f));
	SetSizeOrigin(setsize);


	// 位置、向き設定は必要があれば追加

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTree::Uninit()
{
	// 終了処理
	CObject3D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTree::Update()
{
	// 更新処理
	CObject3D::Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CTree::Draw()
{
	// 描画処理
	CObject3D::Draw();
}
