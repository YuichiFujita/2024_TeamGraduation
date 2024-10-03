//=============================================================================
// 
//  数字(ビルボード)処理 [number_Billboard.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "number_Billboard.h"
#include "manager.h"
#include "renderer.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CNumberBillboard::CNumberBillboard(int nPriority) : CNumber(nPriority)
{
	// 値のクリア
	m_pObjBillboard = nullptr;		// オブジェクトビルボードのオブジェクト
}

//==========================================================================
// デストラクタ
//==========================================================================
CNumberBillboard::~CNumberBillboard()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CNumberBillboard::Init()
{
	// 生成処理
	m_pObjBillboard = CObjectBillboard::Create(GetPriority());

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CNumberBillboard::Uninit()
{
	// 終了処理
	if (m_pObjBillboard != nullptr)
	{
		m_pObjBillboard = nullptr;
	}

	CNumber::Uninit();
}

//==========================================================================
// 削除処理
//==========================================================================
void CNumberBillboard::Kill()
{
	if (m_pObjBillboard != nullptr)
	{
		m_pObjBillboard->Uninit();
		m_pObjBillboard = nullptr;
	}

	CNumber::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CNumberBillboard::Update()
{
	// 更新処理
	if (m_pObjBillboard != nullptr)
	{
		m_pObjBillboard->SetEnableDisp(IsDisp());
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CNumberBillboard::Draw()
{
	//// デバイスの取得
	//LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//// ライティングを無効にする
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//// アルファテストを有効にする
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//// 描画処理
	//if (m_pObjBillboard != nullptr)
	//{// nullptrじゃなかったら
	//	m_pObjBillboard->Draw();
	//}

	//// アルファテストを無効にする
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//// ライティングを有効にする
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CNumberBillboard::SetVtx()
{
	m_pObjBillboard->SetVtx();
}

//==========================================================================
// テクスチャの割り当て
//==========================================================================
void CNumberBillboard::BindTexture(int nIdx)
{
	// 割り当てる
	m_pObjBillboard->BindTexture(nIdx);
}

//==========================================================================
// 種類設定
//==========================================================================
void CNumberBillboard::SetType(const CObject::TYPE type)
{
	m_pObjBillboard->SetType(type);
	CNumber::SetType(type);
}


//==========================================================================
// 位置設定
//==========================================================================
void CNumberBillboard::SetPosition(const MyLib::Vector3& pos)
{
	m_pObjBillboard->SetPosition(pos);
}

//==========================================================================
// 位置取得
//==========================================================================
MyLib::Vector3 CNumberBillboard::GetPosition() const
{
	return m_pObjBillboard->GetPosition();
}

//==========================================================================
// 移動量設定
//==========================================================================
void CNumberBillboard::SetMove(const MyLib::Vector3& move)
{
	m_pObjBillboard->SetMove(move);
}

//==========================================================================
// 移動量取得
//==========================================================================
MyLib::Vector3 CNumberBillboard::GetMove() const
{
	return m_pObjBillboard->GetMove();
}

//==========================================================================
// 向き設定
//==========================================================================
void CNumberBillboard::SetRotation(const MyLib::Vector3& rot)
{
	m_pObjBillboard->SetRotation(rot);
}

//==========================================================================
// 向き取得
//==========================================================================
MyLib::Vector3 CNumberBillboard::GetRotation() const
{
	return m_pObjBillboard->GetRotation();
}

//==========================================================================
// 色設定
//==========================================================================
void CNumberBillboard::SetColor(const D3DXCOLOR col)
{
	m_pObjBillboard->SetColor(col);
}

//==========================================================================
// 色取得
//==========================================================================
D3DXCOLOR CNumberBillboard::GetColor() const
{
	return m_pObjBillboard->GetColor();
}

//==========================================================================
// サイズ設定
//==========================================================================
void CNumberBillboard::SetSize(const D3DXVECTOR2 size)
{
	m_pObjBillboard->SetSize(size);		// サイズ
}

//==========================================================================
// サイズ取得
//==========================================================================
D3DXVECTOR2 CNumberBillboard::GetSize() const
{
	return m_pObjBillboard->GetSize();
}

//==========================================================================
// 元のサイズの設定
//==========================================================================
void CNumberBillboard::SetSizeOrigin(const D3DXVECTOR2 size)
{
	m_pObjBillboard->SetSizeOrigin(size);
}

//==========================================================================
// 元のサイズの取得
//==========================================================================
D3DXVECTOR2 CNumberBillboard::GetSizeOrigin() const
{
	return m_pObjBillboard->GetSizeOrigin();
}

//==========================================================================
// テクスチャ座標設定
//==========================================================================
void CNumberBillboard::SetTex(D3DXVECTOR2 *tex)
{
	m_pObjBillboard->SetTex(tex);
}

//==========================================================================
// テクスチャ座標取得
//==========================================================================
D3DXVECTOR2 *CNumberBillboard::GetTex()
{
	return m_pObjBillboard->GetTex();
}
