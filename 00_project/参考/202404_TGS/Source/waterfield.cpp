//=============================================================================
// 
//  起伏処理 [waterfield.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "waterfield.h"
#include "manager.h"
#include "input.h"
#include "calculation.h"
#include "debugproc.h"

// 継承先
#include "waterfield_right.h"
#include "waterfield_left.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\FIELD\\water-bg-pattern-04.jpg";
	const int BLOCK = 8;
	const float BLOCK_SIZE = 40000.0f;
	const float INTERVAL_TEXU = 900.0f;	// U座標の間隔
}

//==========================================================================
// コンストラクタ
//==========================================================================
CWaterField::CWaterField(int nPriority, const LAYER layer) : CObject3DMesh(nPriority, layer)
{
	m_fTexU = 0.0f;		// Uスクロール用
	m_fTexV = 0.0f;		// Vスクロール用
}

//==========================================================================
// デストラクタ
//==========================================================================
CWaterField::~CWaterField()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CWaterField* CWaterField::Create(TYPE type)
{
	// メモリの確保
	CWaterField* pObjMeshField = nullptr;

	switch (type)
	{
	case CWaterField::TYPE_NORMAL:
		pObjMeshField = DEBUG_NEW CWaterField;
		break;

	case CWaterField::TYPE_RIGHT:
		pObjMeshField = DEBUG_NEW CWaterField_Right;
		break;

	case CWaterField::TYPE_LEFT:
		pObjMeshField = DEBUG_NEW CWaterField_Left;
		break;

	default:
		return nullptr;
		break;
	}

	if (pObjMeshField != nullptr)
	{
		// 初期化処理
		pObjMeshField->Init();
	}

	return pObjMeshField;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CWaterField::Init()
{
	// テクスチャの割り当て
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// 各種変数初期化
	SetPosition(MyLib::Vector3(0.0f, 0.1f, 0.0f));			// 位置
	SetWidthBlock(BLOCK);		// 幅分割
	SetHeightBlock(BLOCK);		// 縦分割
	SetWidthLen(BLOCK_SIZE);	// 縦長さ
	SetHeightLen(BLOCK_SIZE);	// 横長さ

	// オブジェクト3Dメッシュの初期化処理
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// 頂点情報設定
	SetVtx();


	// 全ての要素を書き換え
	D3DXCOLOR* pVtxCol = GetVtxCol();
	std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.8f));

	return E_FAIL;
}

//==========================================================================
// 終了処理
//==========================================================================
void CWaterField::Uninit()
{
	// 終了処理
	CObject3DMesh::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CWaterField::Update()
{
	m_fTexU -= 0.003f;		// Uスクロール用

	if (m_fTexU >= 1.0f)
	{
		m_fTexU = 0.0f;
	}
	else if (m_fTexU <= 0.0f)
	{
		m_fTexU = 1.0f;
	}

	if (m_fTexV >= 1.0f)
	{
		m_fTexV = 0.0f;
	}
	else if (m_fTexV <= 0.0f)
	{
		m_fTexV = 1.0f;
	}

	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CWaterField::Draw()
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 描画処理
	CObject3DMesh::Draw();
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CWaterField::SetVtx()
{
	int nHBlock = GetHeightBlock();
	int nWBlock = GetWidthBlock();

	// 頂点情報
	MyLib::Vector3* pVtxPos = GetVtxPos();
	D3DXVECTOR2* pVtxTex = GetVtxTex();

	int idx = 0;

	// テクスチャ情報
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	D3DXVECTOR2 size = UtilFunc::Transformation::AdjustSizeByWidth(CTexture::GetInstance()->GetImageSize(texID), INTERVAL_TEXU);
	float intervalU = size.x, intervalV = size.y;
	float posU = 0.0f, posV = 0.0f;

	// 頂点情報の設定
	for (int nCntHeight = 0; nCntHeight < nHBlock + 1; nCntHeight++)
	{//縦の頂点数分繰り返す

		// リセット
		posU = 0.0f;

		for (int nCntWidth = 0; nCntWidth < nWBlock + 1; nCntWidth++)
		{// 横の頂点数分繰り返す

			
			// UV座標
			pVtxTex[idx] = D3DXVECTOR2(posU + m_fTexU, posV + m_fTexV);

			// 横の割合分進める
			if (nCntWidth + 1 <= nWBlock)
			{
				int u = nCntWidth + (nCntHeight * (nWBlock + 1));
				posU += pVtxPos[u].DistanceXZ(pVtxPos[u + 1]) / INTERVAL_TEXU;
			}

			idx++;
		}

		// 縦の割合分進める
		if (nCntHeight != nHBlock)
		{
			int vtxIdx = (nCntHeight * (nWBlock + 1));
			int vtxIdxDown = ((nCntHeight + 1) * (nWBlock + 1));
			posV += pVtxPos[vtxIdx].DistanceXZ(pVtxPos[vtxIdxDown]) / intervalV;
		}
	}

	// 頂点情報更新
	CObject3DMesh::SetVtx();
}
