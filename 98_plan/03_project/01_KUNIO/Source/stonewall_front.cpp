//=============================================================================
// 
//  石垣処理 [stonewall_front.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "stonewall_front.h"
#include "manager.h"
#include "calculation.h"
#include "spline.h"
#include "course.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\map_object\\ishigaki000.png";
	const int WIDTH_BLOCK = 2;
	const float WIDTH = -50.0f;		// 斜めの幅
	const float HEIGHT = 100.0f;	// 高さ
	const float INTERVAL_TEXU = 500.0f;	// U座標の間隔
}

//==========================================================================
// コンストラクタ
//==========================================================================
CStoneWall_Front::CStoneWall_Front(int nPriority, const LAYER layer) : CStoneWall(nPriority, layer)
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CStoneWall_Front::~CStoneWall_Front()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CStoneWall_Front* CStoneWall_Front::Create()
{
	// メモリの確保
	CStoneWall_Front* pObjMeshField = DEBUG_NEW CStoneWall_Front;

	if (pObjMeshField != nullptr)
	{
		// 初期化処理
		pObjMeshField->Init();
	}

	return pObjMeshField;
}


//==========================================================================
// 点表示
//==========================================================================
void CStoneWall_Front::DispPoint()
{
#if _DEBUG

	// 描画消す
	SetEnableDisp(false);

	MyLib::Vector3* pVtxPos = GetVtxPos();

	for (int y = 0; y < static_cast<int>(m_vecVtxPosition.size()); y++)
	{
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		CEffect3D::Create(
			pVtxPos[nextidx],
			0.0f,
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}
#endif
}

//==========================================================================
// 頂点座標
//==========================================================================
void CStoneWall_Front::BindVtxPosition()
{
	// 計算用変数
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;

	MyLib::Vector3* pVtxPos = GetVtxPos();
	MyLib::Vector3 rot;

	m_vecTopPosition.clear();

	for (int y = 0; y < static_cast<int>(m_vecVtxPosition.size()); y++)
	{
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		// 向き反映
		int next = (y + 1) % static_cast<int>(m_vecVtxPosition.size());

		bool bEnd = false;
		if (next == 0)
		{
			next = y - 1;
			bEnd = true;
		}

		rot.y = m_vecVtxPosition[next].AngleXZ(m_vecVtxPosition[y]);
		UtilFunc::Transformation::RotNormalize(rot.y);

		if (bEnd)
		{
			rot.y *= -1;
		}

		// 回転反映
		mtxRotate.RotationYawPitchRoll(rot.y, rot.x, rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// 位置反映
		mtxTrans.Translation(m_vecVtxPosition[y]);
		mtxParent.Multiply(mtxParent, mtxTrans);



		// オフセット反映
		offset = MyLib::Vector3(-WIDTH, HEIGHT, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(0.0f, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);


		// 頂点座標代入
		pVtxPos[idx] = mtxLeft.GetWorldPosition();
		pVtxPos[nextidx] = mtxRight.GetWorldPosition();

		// 頂上の位置
		m_vecTopPosition.push_back(pVtxPos[idx]);
	}

	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 各頂点UV座標設定
//==========================================================================
void CStoneWall_Front::SetVtxTexUV()
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

			for (int nCntWidth = 0; nCntWidth < WIDTH_BLOCK; nCntWidth++)
			{// 横の分割分繰り返す

				idx = nCntWidth + (nCntHeight * WIDTH_BLOCK);
				pTex[idx] = D3DXVECTOR2(posU, posV);

				// 縦の割合分進める
				posV += sqrtf((pVtxPos[idx].y - pVtxPos[idx + 1].y) * (pVtxPos[idx].y - pVtxPos[idx + 1].y)) / intervalV;

			}

			// 横の割合分進める
			posU += pVtxPos[idx + 1].DistanceXZ(pVtxPos[idx - 1]) / INTERVAL_TEXU;

		}
	}
}
