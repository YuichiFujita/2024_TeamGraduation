//=============================================================================
// 
//  キーコンフィグに応じたボタン表示 [controlkeydisp.cpp]
//  Author : 石原颯馬
// 
//=============================================================================
#include "controlkeydisp.h"
#include "keyconfig.h"

std::map<CInputGamepad::BUTTON, int> CControlKeyDisp::m_buttonTextures;
//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	
}

//==========================================================================
// コンストラクタ
//==========================================================================
CControlKeyDisp::CControlKeyDisp(int nPriority) : CObject2D(nPriority)
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CControlKeyDisp::~CControlKeyDisp()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CControlKeyDisp* CControlKeyDisp::Create(MyLib::Vector3 pos, MyLib::Vector3 rot, float width, float height, int type)
{
	// メモリの確保
	CControlKeyDisp* pObject = DEBUG_NEW CControlKeyDisp;

	if (pObject != nullptr)
	{
		pObject->SetPosition(pos);
		pObject->SetOriginPosition(pos);
		pObject->SetRotation(rot);
		pObject->SetSize(D3DXVECTOR2(width, height));
		pObject->SetSizeOrigin(pObject->GetSize());

		// 初期化処理
		pObject->Init();

		// キーコンフィグに応じてテクスチャ設定
		pObject->CControlKeyDisp::SetType(type);
	}

	return pObject;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CControlKeyDisp::Init()
{
	CObject2D::Init();

	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CControlKeyDisp::Uninit()
{
	CObject2D::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CControlKeyDisp::Kill()
{
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CControlKeyDisp::Update()
{
	CObject2D::Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CControlKeyDisp::Draw()
{
	CObject2D::Draw();
}

//==========================================================================
// 種類・テクスチャ設定
//==========================================================================
void CControlKeyDisp::SetType(int type)
{
	auto itr = m_buttonTextures.find(static_cast<CInputGamepad::BUTTON>(type));
	if (itr != m_buttonTextures.end())
	{
		BindTexture((*itr).second);
		SetVtx();
	}
}