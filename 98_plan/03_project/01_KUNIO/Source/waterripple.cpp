//=============================================================================
// 
//  水紋処理 [waterripple.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "waterripple.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"
#include "particle.h"
#include "3D_effect.h"
#include "camera.h"
#include "spline.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float DEFAULT_ALPHA = 0.5f;	// デフォ不透明度
}

//==========================================================================
// コンストラクタ
//==========================================================================
CWaterRipple::CWaterRipple(const int block, const float blockSize, int nPriority, const LAYER layer)
	: m_Block(block), m_BlockSize(blockSize), CObject3DMesh(nPriority, layer)
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CWaterRipple::~CWaterRipple()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CWaterRipple* CWaterRipple::Create(
	const int block, const float blockSize, 
	const MyLib::Vector3& pos, float height, float velocity, float thickness, int life)
{
	// メモリの確保
	CWaterRipple* pObjMeshField = DEBUG_NEW CWaterRipple(block, blockSize);

	if (pObjMeshField != nullptr)
	{
		pObjMeshField->m_Info = Info(height, velocity, 0.0f, thickness, life, life);

		// 初期化処理
		pObjMeshField->SetPosition(pos);
		pObjMeshField->Init();
	}

	return pObjMeshField;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CWaterRipple::Init()
{
	HRESULT hr;

	// テクスチャの割り当て
	int texIdx = CTexture::GetInstance()->Regist("");
	BindTexture(texIdx);

	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// 各種変数初期化
	SetWidthBlock(m_Block);		// 幅分割
	SetHeightBlock(m_Block);	// 縦分割
	SetWidthLen(m_BlockSize);	// 縦長さ
	SetHeightLen(m_BlockSize);	// 横長さ

	// オブジェクト3Dメッシュの初期化処理
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);


	D3DXCOLOR* pVtxCol = GetVtxCol();

	// 全ての要素を書き換え
	std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(0.6f, 0.6f, 1.0f, DEFAULT_ALPHA));


	// 頂点座標計算
	SetVtxPosition();

	SetVtx();
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CWaterRipple::Uninit()
{
	// 終了処理
	CObject3DMesh::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CWaterRipple::Update()
{
	// 中心からの長さ加算
	m_Info.length += m_Info.velocity;

	// 頂点座標計算
	SetVtxPosition();

	SetVtx();

	// 寿命減算
	m_Info.life--;

	if (m_Info.life <= 0)
	{
		Uninit();
	}
}

//==========================================================================
// 頂点座標
//==========================================================================
void CWaterRipple::SetVtxPosition()
{

	MyLib::Vector3* pVtxPos = GetVtxPos();
	MyLib::Vector3* pVtxNor = GetVtxNor();
	D3DXCOLOR* pVtxCol = GetVtxCol();
	MyLib::Vector3 vec1, vec2, nor;
	MyLib::Vector3 VtxRight, VtxLeft, VtxNow;
	int nHeight = GetHeightBlock();
	int nWidth = GetWidthBlock();
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();
	int vtxNum = GetNumVertex();

	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 vtxPos;

	float lifeRatio = (float)m_Info.life / (float)m_Info.maxLife;

	for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
	{// 縦の分割分繰り返す

		for (int nCntWidth = 0; nCntWidth < nWidth + 1; nCntWidth++)
		{// 横の分割分繰り返す

			int idx = nCntWidth + (nWidth + 1) * nCntHeight;
			vtxPos = pVtxPos[idx] + pos;

			// 原点と頂点の距離
			float fNowLength =
				sqrtf((pos.x - vtxPos.x) * (pos.x - vtxPos.x)
					+ (pos.z - vtxPos.z) * (pos.z - vtxPos.z));


			// 原点と最大範囲までの距離
			float fMaxLength = m_Info.length + m_Info.thickness;

			// 原点と最小範囲までの距離
			float fMinLength = m_Info.length - m_Info.thickness;

			// 範囲の長さ
			float fRangeLength = fMaxLength - fMinLength;


			// 透明度
			pVtxCol[idx].a = DEFAULT_ALPHA * (static_cast<float>(m_Info.life) / static_cast<float>(m_Info.maxLife));

			// 現在距離との割合
			float ratio = m_Info.length / fNowLength;

			if (fNowLength > fMaxLength || fNowLength < fMinLength)
			{// 範囲外は移動量ゼロ
				
				ratio = 0.0f;
				pVtxCol[idx].a = 0.0f;
			}

			if (fNowLength <= fMaxLength &&
				fNowLength >= fMinLength)
			{

				if (m_Info.length >= fNowLength)
				{
					int n = 0;
				}


				float calMinLen = fMinLength;
				if (calMinLen <= 0.0f)
				{// 範囲外
					calMinLen = 0.0f;
					pVtxCol[idx].a = 0.0f;
				}

				float calMaxLen = fMaxLength - calMinLen;
				float calVtxLen = fNowLength - calMinLen;
				if (calMaxLen <= 0.0f)
				{// 範囲外
					calMaxLen = 0.0f;
					pVtxCol[idx].a = 0.0f;
				}

				if (calVtxLen <= 0.0f)
				{// 範囲外
					calVtxLen = 0.0f;
					pVtxCol[idx].a = 0.0f;
				}


				// 最大距離が1.0fになる割合
				float maxratio = calVtxLen / calMaxLen;

				if (maxratio > 0.5f)
				{
					maxratio = 1.0f - maxratio;
				}
				else
				{
					int n = 0;
				}

				ratio = maxratio;
			}

			// 波の高さ
			float waveHeight = m_Info.height * ratio;

			waveHeight *= lifeRatio;

			// 波の高さ設定
			pVtxPos[idx].y = waveHeight;
		}
	}

}

//==========================================================================
// 描画処理
//==========================================================================
void CWaterRipple::Draw()
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (CManager::GetInstance()->IsWireframe())
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	// ワイヤーフレームモード
	}

	// 描画処理
	CObject3DMesh::Draw();

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// 埋めるモード
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CWaterRipple::SetVtx()
{

	MyLib::Vector3 *pVtxPos = GetVtxPos();
	MyLib::Vector3 *pVtxNor = GetVtxNor();
	MyLib::Vector3 vec1, vec2, nor;
	MyLib::Vector3 VtxRight, VtxLeft, VtxNow;
	int nHeight = GetHeightBlock();
	int nWidth = GetWidthBlock();
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();
	int vtxNum = GetNumVertex();

	for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
	{// 縦の分割分繰り返す

		for (int nCntWidth = 0; nCntWidth < nWidth + 1; nCntWidth++)
		{// 横の分割分繰り返す

			// 今回の頂点
			int nNowPoint = (nCntWidth + 1) + (nCntHeight * (nWidth));
			int nVerTexW = (nWidth) + 1;

			int nLeft = nCntWidth + (nCntHeight * (nWidth));
			int nRight = nCntWidth + (nCntHeight * (nWidth)) + nVerTexW;

			if (nNowPoint >= vtxNum)
			{
				continue;
			}

			if (nRight >= vtxNum)
			{// 頂点数超えたら

				// 頂点座標の設定
				VtxRight = MyLib::Vector3(
					(fWidthLen * nCntWidth) - ((fWidthLen * nWidth) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * nHeight) * 0.5f)));
			}
			else
			{
				VtxRight = pVtxPos[nRight];
			}

			if (nLeft >= vtxNum)
			{// 頂点数超えたら

				// 頂点座標の設定
				VtxLeft = MyLib::Vector3(
					(fWidthLen * nCntWidth) - ((fWidthLen * nWidth) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * nHeight) * 0.5f)));
			}
			else
			{
				VtxLeft = pVtxPos[nLeft];
			}

			if (nNowPoint >= vtxNum)
			{// 頂点数超えたら

				// 頂点座標の設定
				VtxNow = MyLib::Vector3(
					(fWidthLen * nCntWidth) - ((fWidthLen * nWidth) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * nHeight) * 0.5f)));
			}
			else
			{
				VtxNow = pVtxPos[nNowPoint];
			}

			// ベクトルを計算
			vec1 = VtxRight - VtxNow;
			vec2 = VtxLeft - VtxNow;

			// 外積を求める
			D3DXVec3Cross(&nor, &vec1, &vec2);

			// 外積の正規化をして法線にする
			D3DXVec3Normalize(&nor, &nor);

			// 法線
			pVtxNor[nNowPoint] = nor;
		}
	}

	// 頂点情報更新
	CObject3DMesh::SetVtx();
}
