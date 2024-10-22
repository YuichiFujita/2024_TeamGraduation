#if 0
//=============================================================================
// 
//  数字処理 [number.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "number.h"

// 派生先
#include "number_2D.h"
#include "number_3D.h"
#include "number_Billboard.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CNumber::CNumber(int nPriority, const LAYER layer) : CObject(nPriority, layer)
{
	// 値のクリア
	m_objType = OBJECTTYPE_2D;	// オブジェクトの種類
}

//==========================================================================
// デストラクタ
//==========================================================================
CNumber::~CNumber()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CNumber* CNumber::Create(EObjectType objtype, int nPriority)
{
	// 生成用のオブジェクト
	CNumber* pNumber = nullptr;

	// メモリの確保
	switch (objtype)
	{
	case CNumber::OBJECTTYPE_2D:
		pNumber = DEBUG_NEW CNumber2D(nPriority);
		break;

	case CNumber::OBJECTTYPE_3D:
		pNumber = DEBUG_NEW CNumber3D(nPriority);
		break;

	case CNumber::OBJECTTYPE_BILLBOARD:
		pNumber = DEBUG_NEW CNumberBillboard(nPriority);
		break;
	}

	if (pNumber != nullptr)
	{
		// オブジェクトの種類
		pNumber->m_objType = objtype;

		// 初期化処理
		pNumber->Init();
	}

	return pNumber;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CNumber::Init()
{
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CNumber::Uninit()
{
	Release();
}

//==========================================================================
// 削除
//==========================================================================
void CNumber::Kill()
{
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CNumber::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 描画処理
//==========================================================================
void CNumber::Draw()
{
	
}

//==========================================================================
// 種類設定
//==========================================================================
void CNumber::SetType(const CObject::TYPE type)
{
	CObject::SetType(type);
}

//==========================================================================
// 自分の数字設定
//==========================================================================
void CNumber::SetNum(int num)
{
	// TODO:ほんとにこんなことして大丈夫？

	float fWidthRate	= 1.0f / 10;		// 横の分割数の割合
	float fHeightRate	= 1.0f / 1;			// 縦の分割数の割合
	int nWidthCurrent	= num % 10;			// 現在の横のパターン
	int nHeightCurrent	= (num / 10) % 1;	// 現在の縦のパターン

	// テクスチャ座標の設定
	D3DXVECTOR2 tex[4];
	tex[0] = D3DXVECTOR2(fWidthRate *  nWidthCurrent,		fHeightRate *  nHeightCurrent);
	tex[1] = D3DXVECTOR2(fWidthRate * (nWidthCurrent + 1),	fHeightRate *  nHeightCurrent);
	tex[2] = D3DXVECTOR2(fWidthRate *  nWidthCurrent,		fHeightRate * (nHeightCurrent + 1));
	tex[3] = D3DXVECTOR2(fWidthRate * (nWidthCurrent + 1),	fHeightRate * (nHeightCurrent + 1));

	m_nNum = num;
	SetTex(&tex[0]);
}
#endif
