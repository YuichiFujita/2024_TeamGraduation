//=============================================================================
// 
//  数字(2D)処理 [number_2D.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "number_2D.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CNumber2D::CNumber2D(int nPriority, const LAYER layer) : CNumber(nPriority, layer)
{
	// 値のクリア
	m_aObject2D = nullptr;			// オブジェクト2Dのオブジェクト
}

//==========================================================================
// デストラクタ
//==========================================================================
CNumber2D::~CNumber2D()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CNumber2D::Init()
{
	// 生成処理
	m_aObject2D = CObject2D::Create(GetPriority());

	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CNumber2D::Uninit()
{
	// 終了処理
	if (m_aObject2D != nullptr)
	{
		m_aObject2D = nullptr;
	}

	CNumber::Uninit();
}

//==========================================================================
// 解放処理
//==========================================================================
void CNumber2D::Kill()
{
	if (m_aObject2D != nullptr)
	{
		m_aObject2D->Uninit();
		m_aObject2D = nullptr;
	}

	CNumber::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CNumber2D::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 更新処理
	if (m_aObject2D != nullptr)
	{
		m_aObject2D->SetEnableDisp(IsDisp());
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CNumber2D::Draw()
{
	//// 描画処理
	//if (m_aObject2D != nullptr)
	//{
	//	m_aObject2D->Draw();
	//}
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CNumber2D::SetVtx()
{
	m_aObject2D->SetVtx();
}

//==========================================================================
// テクスチャの割り当て
//==========================================================================
void CNumber2D::BindTexture(int nIdx)
{
	m_aObject2D->BindTexture(nIdx);
}

//==========================================================================
// 種類設定
//==========================================================================
void CNumber2D::SetType(const CObject::TYPE type)
{
	m_aObject2D->SetType(type);
	CNumber::SetType(type);
}

//==========================================================================
// 位置設定
//==========================================================================
void CNumber2D::SetPosition(const MyLib::Vector3& pos)
{
	m_aObject2D->SetPosition(pos);
}

//==========================================================================
// 位置取得
//==========================================================================
MyLib::Vector3 CNumber2D::GetPosition() const
{
	return m_aObject2D->GetPosition();
}

//==========================================================================
// 移動量設定
//==========================================================================
void CNumber2D::SetMove(const MyLib::Vector3& move)
{
	m_aObject2D->SetMove(move);
}

//==========================================================================
// 移動量取得
//==========================================================================
MyLib::Vector3 CNumber2D::GetMove() const
{
	return m_aObject2D->GetMove();
}

//==========================================================================
// 向き設定
//==========================================================================
void CNumber2D::SetRotation(const MyLib::Vector3& rot)
{
	m_aObject2D->SetRotation(rot);
}

//==========================================================================
// 向き取得
//==========================================================================
MyLib::Vector3 CNumber2D::GetRotation() const
{
	return m_aObject2D->GetRotation();
}

//==========================================================================
// 色設定
//==========================================================================
void CNumber2D::SetColor(const D3DXCOLOR col)
{
	m_aObject2D->SetColor(col);
}

//==========================================================================
// 色取得
//==========================================================================
D3DXCOLOR CNumber2D::GetColor() const
{
	return m_aObject2D->GetColor();
}

//==========================================================================
// サイズ設定
//==========================================================================
void CNumber2D::SetSize(const D3DXVECTOR2 size)
{
	m_aObject2D->SetSize(size);		// サイズ
}

//==========================================================================
// サイズ取得
//==========================================================================
D3DXVECTOR2 CNumber2D::GetSize() const
{
	return m_aObject2D->GetSize();
}

//==========================================================================
// 元のサイズの設定
//==========================================================================
void CNumber2D::SetSizeOrigin(const D3DXVECTOR2 size)
{
	m_aObject2D->SetSizeOrigin(size);
}

//==========================================================================
// 元のサイズの取得
//==========================================================================
D3DXVECTOR2 CNumber2D::GetSizeOrigin() const
{
	return m_aObject2D->GetSizeOrigin();
}

//==========================================================================
// テクスチャ座標設定
//==========================================================================
void CNumber2D::SetTex(D3DXVECTOR2 *tex)
{
	m_aObject2D->SetTex(tex);
}

//==========================================================================
// テクスチャ座標取得
//==========================================================================
D3DXVECTOR2 *CNumber2D::GetTex()
{
	return m_aObject2D->GetTex();
}

