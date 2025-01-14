//=============================================================================
// 
//  オブジェクト3D処理 [object3D.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "object3D.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int POLYGON_TOP = 4;	// ポリゴンの頂点数
}

//==========================================================================
// コンストラクタ
//==========================================================================
CObject3D::CObject3D(int nPriority, const LAYER layer) : CObject(nPriority, layer)
{
	m_mtxWorld.Identity();				// ワールドマトリックス
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	m_pVtxBuff = nullptr;							// 頂点バッファ
	m_AnchorType = EAnchorPoint::CENTER;			// アンカーポイントの種類
	m_fAnchorRate = 0.0f;							// アンカーの割合
	m_fLength = 0.0f;								// 対角線の長さ
	m_fAngle = 0.0f;								// 対角線の向き
	m_nTexIdx = 0;									// テクスチャのインデックス番号
	m_vecUV.clear();	// テクスチャ座標
}

//==========================================================================
// デストラクタ
//==========================================================================
CObject3D::~CObject3D()
{

}

//==========================================================================
// テクスチャの割り当て
//==========================================================================
void CObject3D::BindTexture(int nIdx)
{
	// 割り当てる
	m_nTexIdx = nIdx;
}

//==========================================================================
// 生成処理
//==========================================================================
CObject3D* CObject3D::Create(int nPriority)
{
	// メモリの確保
	CObject3D* pObject3D = DEBUG_NEW CObject3D(nPriority);

	if (pObject3D != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		pObject3D->Init();
	}

	return pObject3D;
}

//==========================================================================
// 生成処理
//==========================================================================
CObject3D* CObject3D::Create(const MyLib::Vector3& pos, const MyLib::Vector3& rot)
{

	// メモリの確保
	CObject3D* pObject3D = CObject3D::Create();

	if (pObject3D != nullptr)
	{// メモリの確保が出来ていたら

		// 位置・向き
		pObject3D->SetPosition(pos);
		pObject3D->SetOriginPosition(pos);
		pObject3D->SetRotation(rot);
	}

	return pObject3D;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObject3D::Init()
{
	HRESULT hr;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (m_pVtxBuff != nullptr)
	{
		return E_FAIL;
	}

	// 頂点バッファの生成
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}

	// 引数情報設定
	m_vecUV.clear();
	m_vecUV.resize(POLYGON_TOP);

	// アンカーの割合
	m_fAnchorRate = 1.0f;

	// テクスチャ座標
	m_vecUV[0] = D3DXVECTOR2(0.0f, 0.0f);
	m_vecUV[1] = D3DXVECTOR2(1.0f, 0.0f);
	m_vecUV[2] = D3DXVECTOR2(0.0f, 1.0f);
	m_vecUV[3] = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点情報設定
	SetVtx();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObject3D::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 動的削除処理
//==========================================================================
void CObject3D::Kill()
{
	// 終了処理
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObject3D::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CObject3D::Draw()
{
	// マネージャのインスタンス取得
	CManager* pMgr = CManager::GetInstance();
	if (GetType() == CObject::TYPE::TYPE_UI && !pMgr->IsDisp_UI())
	{
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pMgr->GetRenderer()->GetDevice();

	// ワールドマトリックスの計算
	CalWorldMtx();

	// ワールドマトリックスの設定
	D3DXMATRIX mtx = m_mtxWorld.ConvertD3DXMATRIX();
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nTexIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================================
// ワールドマトリックスの計算処理
//==========================================================================
void CObject3D::CalWorldMtx()
{
	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 rotOrigin = GetOriginRotation();

	// 計算用マトリックス宣言
	MyLib::Matrix mtxRot, mtxTrans, mtxRotOrigin;
	mtxRotOrigin.Identity();

	// ワールドマトリックスの初期化
	m_mtxWorld.Identity();

	// 元の向きを反映する
	mtxRotOrigin.RotationYawPitchRoll(rotOrigin.y, rotOrigin.x, rotOrigin.z);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRotOrigin);

	// 向きを反映する
	mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRot);

	// 位置を反映する
	mtxTrans.Translation(pos);
	m_mtxWorld.Multiply(m_mtxWorld, mtxTrans);
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CObject3D::SetVtx()
{

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR2 size = GetSize();
	D3DXCOLOR col = GetColor();
	MyLib::Vector3 rot = GetRotation();

	// アンカーポイントの設定
	float anchorX = 0.0f;
	float anchorY = 0.0f;
	MyLib::Vector3 pos = GetPosition();

	// センター以外は計算
	if (m_AnchorType != EAnchorPoint::CENTER)
	{
		float distanceX = size.x * m_fAnchorRate, distanceY = size.y * m_fAnchorRate;
		switch (m_AnchorType)
		{
		case EAnchorPoint::LEFT:
			anchorX = pos.x + distanceX;
			anchorY = pos.y;
			break;

		case EAnchorPoint::TOP_LEFT:
			anchorX = pos.x + distanceX;
			anchorY = pos.y - distanceY;
			break;

		case EAnchorPoint::TOP_CENTER:
			anchorX = pos.x;
			anchorY = pos.y - distanceY;
			break;

		case EAnchorPoint::TOP_RIGHT:
			anchorX = pos.x - distanceX;
			anchorY = pos.y - distanceY;
			break;

		case EAnchorPoint::RIGHT:
			anchorX = pos.x - distanceX;
			anchorY = pos.y;
			break;

		case EAnchorPoint::UNDER_RIGHT:
			anchorX = pos.x - distanceX;
			anchorY = pos.y + distanceY;
			break;

		case EAnchorPoint::UNDER_CENTER:
			anchorX = pos.x;
			anchorY = pos.y + distanceY;
			break;

		case EAnchorPoint::UNDER_LEFT:
			anchorX = pos.x + distanceX;
			anchorY = pos.y + distanceY;
			break;

		default:
			anchorX = pos.x;
			anchorY = pos.y;
			break;
		}
	}

	// 頂点座標の設定
	if (m_AnchorType == EAnchorPoint::CENTER)
	{
		// 位置を更新
		pVtx[0].pos = MyLib::Vector3(-size.x, +size.y, 0.0f);
		pVtx[1].pos = MyLib::Vector3(+size.x, +size.y, 0.0f);
		pVtx[2].pos = MyLib::Vector3(-size.x, -size.y, 0.0f);
		pVtx[3].pos = MyLib::Vector3(+size.x, -size.y, 0.0f);
	}
	else
	{
		pVtx[0].pos = RotateVtx(MyLib::Vector3(anchorX - size.x, anchorY + size.y, 0.0f), pos) - pos;
		pVtx[1].pos = RotateVtx(MyLib::Vector3(anchorX + size.x, anchorY + size.y, 0.0f), pos) - pos;
		pVtx[2].pos = RotateVtx(MyLib::Vector3(anchorX - size.x, anchorY - size.y, 0.0f), pos) - pos;
		pVtx[3].pos = RotateVtx(MyLib::Vector3(anchorX + size.x, anchorY - size.y, 0.0f), pos) - pos;
		ImGui::Text("pos : [X : %.2f, Y : %.2f, Z : %.2f]", pos.x, pos.y, pos.z);

		pVtx[0].pos.z = pVtx[1].pos.z = pVtx[2].pos.z = pVtx[3].pos.z = 0.0f;
	}

	// 境界線のベクトル
	MyLib::Vector3 vecLine0 = pVtx[1].pos - pVtx[0].pos;
	MyLib::Vector3 vecLine1 = pVtx[2].pos - pVtx[0].pos;

	// 外積求める
	MyLib::Vector3 Nor[4];

	// 外積
	D3DXVec3Cross(&Nor[0], &vecLine0, &vecLine1);

	// 正規化
	D3DXVec3Normalize(&Nor[0], &Nor[0]);

	Nor[0] = MyLib::Vector3(
		((vecLine0.y * vecLine1.z) - (vecLine0.z * vecLine1.y)),
		((vecLine0.z * vecLine1.x) - (vecLine0.x * vecLine1.z)),
		((vecLine0.x * vecLine1.y) - (vecLine0.y * vecLine1.x)));

	// 正規化
	D3DXVec3Normalize(&Nor[0], &Nor[0]);


	// 境界線のベクトル
	vecLine0 = pVtx[2].pos - pVtx[3].pos;
	vecLine1 = pVtx[1].pos - pVtx[3].pos;

	// 外積求める
	Nor[3] = MyLib::Vector3(
		((vecLine0.y * vecLine1.z) - (vecLine0.z * vecLine1.y)),
		((vecLine0.z * vecLine1.x) - (vecLine0.x * vecLine1.z)),
		((vecLine0.x * vecLine1.y) - (vecLine0.y * vecLine1.x)));

	// 外積
	D3DXVec3Cross(&Nor[3], &vecLine0, &vecLine1);

	// 正規化
	D3DXVec3Normalize(&Nor[3], &Nor[3]);

	Nor[1] = (Nor[0] + Nor[3]) / 2.0f;
	Nor[2] = (Nor[0] + Nor[3]) / 2.0f;

	// 法線ベクトルの設定
	pVtx[0].nor = Nor[0];
	pVtx[1].nor = Nor[1];
	pVtx[2].nor = Nor[2];
	pVtx[3].nor = Nor[3];

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// テクスチャ座標の設定
	pVtx[0].tex = m_vecUV[0];
	pVtx[1].tex = m_vecUV[1];
	pVtx[2].tex = m_vecUV[2];
	pVtx[3].tex = m_vecUV[3];

	// 頂点バッファをアンロックロック
	m_pVtxBuff->Unlock();
}

//==========================================================================
// 頂点回転処理
//==========================================================================
MyLib::Vector3 CObject3D::RotateVtx(const MyLib::Vector3& vtx, const MyLib::Vector3& center)
{
	// 計算用マトリックス
	MyLib::Matrix mtxRot;
	MyLib::Matrix mtxWepon;
	mtxWepon.Identity();

	// 情報取得
	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 pos = GetPosition();

	rot = 0.0f;

	// 向きを反映する
	mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
	mtxWepon.Multiply(mtxWepon, mtxRot);

	// オフセットを反映する
	MyLib::Vector3 offset(vtx.x - pos.x, vtx.y - pos.y, 0.0f);

	// オフセットに回転行列を適用
	offset = mtxRot.Coord(offset);

	return (offset + center);
}

//==========================================================================
// サイズ設定
//==========================================================================
void CObject3D::SetSize(const MyLib::Vector2& size)
{
	m_size = size;		// サイズ
	m_fLength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);	// 対角線の長さ
	m_fAngle = atan2f(m_size.x, m_size.y);							// 対角線の向き
}
