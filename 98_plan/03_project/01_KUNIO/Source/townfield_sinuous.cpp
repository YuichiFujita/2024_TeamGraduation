//=============================================================================
// 
//  街フィールド(うねり)処理 [townfield_sinuous.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "townfield_sinuous.h"
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
CTownField_Sinuous::CTownField_Sinuous(int nPriority, const LAYER layer) : CMapMesh(nPriority, layer)
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CTownField_Sinuous::~CTownField_Sinuous()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTownField_Sinuous::Init()
{
	HRESULT hr;

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
void CTownField_Sinuous::Uninit()
{
	// 終了処理
	CObject3DMesh::Uninit();
}

//==========================================================================
// 頂点座標
//==========================================================================
void CTownField_Sinuous::BindVtxPosition()
{
	// 計算用変数
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;

	MyLib::Vector3* pVtxPos = GetVtxPos();
	MyLib::Vector3 rot;

	for (int y = 0; y < static_cast<int>(m_vecVtxPosition.size()); y++)
	{
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		rot.y = D3DX_PI * 0.5f;
		UtilFunc::Transformation::RotNormalize(rot.y);


		// 回転反映
		mtxRotate.RotationYawPitchRoll(rot.y, rot.x, rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// 位置反映
		mtxTrans.Translation(m_vecVtxPosition[y]);
		mtxParent.Multiply(mtxParent, mtxTrans);



		// オフセット反映
		offset = MyLib::Vector3(0.0f, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		float wourldLen = FIELD_WORLDLINE_Z - m_vecVtxPosition[y].z;
		offset = MyLib::Vector3(-wourldLen, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);


		// 頂点座標代入
		pVtxPos[idx] = mtxLeft.GetWorldPosition();
		pVtxPos[nextidx] = mtxRight.GetWorldPosition();

	}

	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 各頂点UV座標設定
//==========================================================================
void CTownField_Sinuous::SetVtxTexUV()
{
	D3DXVECTOR2* pTex = GetVtxTex();
	MyLib::Vector3* pVtxPos = GetVtxPos();
	int nHeight = GetHeightBlock();

	if (pTex != nullptr)
	{
		float posU = 0.0f, posV = 0.0f;
		int idx = 0;

		int texID = CTexture::GetInstance()->Regist(TEXTURE);
		float intervalV = UtilFunc::Transformation::AdjustSizeByWidth(CTexture::GetInstance()->GetImageSize(texID), INTERVAL_TEXU).y;

		for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
		{// 縦の分割分繰り返す

			// リセット
			posV = 0.0f;


			int front = (nCntHeight * WIDTH_BLOCK);
			int back = (nCntHeight * WIDTH_BLOCK) + 1;

			// 縦の割合分進める
			posV += sqrtf((pVtxPos[front].z - pVtxPos[front + 1].z) * (pVtxPos[front].z - pVtxPos[front + 1].z)) / intervalV;

			pTex[back] = D3DXVECTOR2(posU, 0.0f);
			pTex[front] = D3DXVECTOR2(posU, posV);

			// 横の割合分進める
			posU += pVtxPos[back + 1].DistanceXZ(pVtxPos[back - 1]) / INTERVAL_TEXU;

		}
	}
}
