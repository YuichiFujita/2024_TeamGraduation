//=============================================================================
// 
//  街フィールド(固定平面) [townfield_fixedplane.cpp]
//  Author : 相馬 靜雅
// 
//=============================================================================
#include "townfield_fixedplane.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"
#include "particle.h"
#include "3D_effect.h"
#include "camera.h"
#include "spline.h"
#include "course.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\FIELD\\soil.jpg";
	const int WIDTH_BLOCK = 2;
	const float FIELD_WORLDLINE_Z = 3000.0f;		// 絶対座標のライン
	const float INTERVAL_TEXU = 600.0f;	// U座標の間隔
}

//==========================================================================
// コンストラクタ
//==========================================================================
CTownField_FixedPlane::CTownField_FixedPlane(int nPriority, const LAYER layer) : CMapMesh(nPriority, layer)
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CTownField_FixedPlane::~CTownField_FixedPlane()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTownField_FixedPlane::Init()
{
	HRESULT hr;

	m_vecVtxPosition =
	{
		MyLib::Vector3(-30000.0f, 300.0f, 33000.0f),
		MyLib::Vector3(100000.0f, 300.0f, 33000.0f),
		MyLib::Vector3(-30000.0f, 300.0f, 3000.0f),
		MyLib::Vector3(100000.0f, 300.0f, 3000.0f)
	};
	SetVecPosition(m_vecVtxPosition);

	// 各種変数初期化
	SetWidthBlock(1);		// 幅分割
	SetHeightBlock(1);	// 縦分割
	SetWidthLen(0.0f);		// 縦長さ
	SetHeightLen(0.0f);		// 横長さ

	// オブジェクト3Dメッシュの初期化処理
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// 初期化処理
	CMapMesh::Init();

	// テクスチャの割り当て
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTownField_FixedPlane::Uninit()
{
	// 終了処理
	CObject3DMesh::Uninit();
}

//==========================================================================
// 頂点座標
//==========================================================================
void CTownField_FixedPlane::BindVtxPosition()
{
	// 計算用変数
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;

	MyLib::Vector3* pVtxPos = GetVtxPos();
	MyLib::Vector3 rot;

	D3DXVECTOR2* pTex = GetVtxTex();

	float wourldLenU = m_vecVtxPosition[0].x - m_vecVtxPosition[1].x;
	float wourldLenV = m_vecVtxPosition[0].z - m_vecVtxPosition[2].z;
	float posU = wourldLenU / INTERVAL_TEXU, posV = wourldLenV / INTERVAL_TEXU;

	pTex[0] = D3DXVECTOR2(0.0f, posV);
	pTex[1] = D3DXVECTOR2(posU, posV);

	pTex[2] = D3DXVECTOR2(0.0f, 0.0f);
	pTex[3] = D3DXVECTOR2(posU, 0.0f);


	for (int cnt = 0; cnt < static_cast<int>(m_vecVtxPosition.size()); cnt++)
	{
		pVtxPos[cnt] = m_vecVtxPosition[cnt];
	}

	// 頂点情報設定
	SetVtx();

	VERTEX_3D* pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);

	// 頂点バッファをアンロックする
	GetVtxBuff()->Unlock();
}

//==========================================================================
// 各頂点UV座標設定
//==========================================================================
void CTownField_FixedPlane::SetVtxTexUV()
{
	D3DXVECTOR2* pTex = GetVtxTex();

	float wourldLenU = m_vecVtxPosition[0].x - m_vecVtxPosition[1].x;
	float wourldLenV = m_vecVtxPosition[0].z - m_vecVtxPosition[2].z;
	float posU = wourldLenU / INTERVAL_TEXU, posV = wourldLenV / INTERVAL_TEXU;

	pTex[0] = D3DXVECTOR2(0.0f, posV);
	pTex[1] = D3DXVECTOR2(posU, posV);

	pTex[2] = D3DXVECTOR2(0.0f, 0.0f);
	pTex[3] = D3DXVECTOR2(posU, 0.0f);
}


//==========================================================================
// リザルトのコンストラクタ
//==========================================================================
CTownField_FixedPlane_Result::CTownField_FixedPlane_Result() : CTownField_FixedPlane()
{

}

//==========================================================================
// リザルトの初期化
//==========================================================================
HRESULT CTownField_FixedPlane_Result::Init()
{
	// 初期化
	CTownField_FixedPlane::Init();

	m_vecVtxPosition =
	{
		MyLib::Vector3(-30000.0f, 300.0f, 33000.0f),
		MyLib::Vector3(100000.0f, 300.0f, 33000.0f),
		MyLib::Vector3(-30000.0f, 300.0f, -33000.0f),
		MyLib::Vector3(100000.0f, 300.0f, -33000.0f)
	};
	SetVecPosition(m_vecVtxPosition);

	// マップメッシュの初期化処理
	HRESULT hr = CMapMesh::Init();

	// テクスチャの割り当て
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	return hr;
}


//==========================================================================
// リザルトのコンストラクタ
//==========================================================================
CTownField_FixedPlane_Ranking::CTownField_FixedPlane_Ranking() : CTownField_FixedPlane()
{

}

//==========================================================================
// リザルトの初期化
//==========================================================================
HRESULT CTownField_FixedPlane_Ranking::Init()
{
	// 初期化
	CTownField_FixedPlane::Init();

	m_vecVtxPosition =
	{
		MyLib::Vector3(-30000.0f, 300.0f, 33000.0f),
		MyLib::Vector3(100000.0f, 300.0f, 33000.0f),
		MyLib::Vector3(-30000.0f, 300.0f, 2200.0f),
		MyLib::Vector3(100000.0f, 300.0f, 2200.0f)
	};
	SetVecPosition(m_vecVtxPosition);

	// マップメッシュの初期化処理
	HRESULT hr = CMapMesh::Init();

	// テクスチャの割り当て
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	return hr;
}
