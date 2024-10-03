//=============================================================================
// 
// 当たり判定ボックス処理 [collisionLine_Box.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "collisionLine_Box.h"
#include "manager.h"
#include "calculation.h"
#include "3D_Effect.h"

namespace 
{
	const int IDXLIST[12][2] =	// インデックス格納
	{
		{0, 1}, {1, 5}, {5, 4}, {4, 0}, // 底面
		{3, 2}, {2, 6}, {6, 7}, {7, 3}, // 上面
		{0, 2}, {1, 3}, {4, 6}, {5, 7}  // 側面
	};
}

//==========================================================================
// コンストラクタ
//==========================================================================
CCollisionLine_Box::CCollisionLine_Box(int nPriority, const LAYER layer) : CObject(nPriority, layer)
{
	// 値のクリア
	memset(m_pLine, 0, sizeof(m_pLine));
}

//==========================================================================
// デストラクタ
//==========================================================================
CCollisionLine_Box::~CCollisionLine_Box()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCollisionLine_Box* CCollisionLine_Box::Create(const MyLib::AABB& aabb, const D3DXCOLOR& col)
{
	
	// メモリの確保
	CCollisionLine_Box* pBox = DEBUG_NEW CCollisionLine_Box;

	if (pBox != nullptr)
	{// メモリの確保が出来ていたら

		pBox->m_AABB = aabb;
		pBox->m_LineColor = col;

		// 初期化処理
		pBox->Init();
	}

	return pBox;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCollisionLine_Box::Init()
{

	// 種類の設定
	SetType(TYPE_OBJECT2D);

	// ボックスの頂点
	MyLib::Vector3 vtx[8];
	vtx[0] = m_AABB.vtxMin; // 最小頂点
	vtx[1] = MyLib::Vector3(m_AABB.vtxMin.x, m_AABB.vtxMin.y, m_AABB.vtxMax.z);
	vtx[2] = MyLib::Vector3(m_AABB.vtxMin.x, m_AABB.vtxMax.y, m_AABB.vtxMin.z);
	vtx[3] = MyLib::Vector3(m_AABB.vtxMin.x, m_AABB.vtxMax.y, m_AABB.vtxMax.z);

	vtx[4] = MyLib::Vector3(m_AABB.vtxMax.x, m_AABB.vtxMin.y, m_AABB.vtxMin.z);
	vtx[5] = MyLib::Vector3(m_AABB.vtxMax.x, m_AABB.vtxMin.y, m_AABB.vtxMax.z);
	vtx[6] = MyLib::Vector3(m_AABB.vtxMax.x, m_AABB.vtxMax.y, m_AABB.vtxMin.z);
	vtx[7] = m_AABB.vtxMax; // 最大頂点

#if 0
	// 底面の辺
	m_pLine[0] = CObjectLine::Create(vtx[0], vtx[1], m_LineColor);
	m_pLine[1] = CObjectLine::Create(vtx[1], vtx[5], m_LineColor);
	m_pLine[2] = CObjectLine::Create(vtx[5], vtx[4], m_LineColor);
	m_pLine[3] = CObjectLine::Create(vtx[4], vtx[0], m_LineColor);

	// 上面の辺
	m_pLine[4] = CObjectLine::Create(vtx[3], vtx[2], m_LineColor);
	m_pLine[5] = CObjectLine::Create(vtx[2], vtx[6], m_LineColor);
	m_pLine[6] = CObjectLine::Create(vtx[6], vtx[7], m_LineColor);
	m_pLine[7] = CObjectLine::Create(vtx[7], vtx[3], m_LineColor);

	// 側面の辺
	m_pLine[8] = CObjectLine::Create(vtx[0], vtx[2], m_LineColor);
	m_pLine[9] = CObjectLine::Create(vtx[1], vtx[3], m_LineColor);
	m_pLine[10] = CObjectLine::Create(vtx[4], vtx[6], m_LineColor);
	m_pLine[11] = CObjectLine::Create(vtx[5], vtx[7], m_LineColor);
#else

	// ライン生成
	for (int i = 0; i < 12; ++i) 
	{
		m_pLine[i] = CObjectLine::Create(vtx[IDXLIST[i][0]], vtx[IDXLIST[i][1]], m_LineColor);
	}
#endif

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCollisionLine_Box::Uninit()
{
	for (int i = 0; i < 12; i++)
	{
		if (m_pLine[i] == nullptr){
			continue;
		}
		m_pLine[i] = nullptr;
	}

	// 終了処理
	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CCollisionLine_Box::Kill()
{
	for (int i = 0; i < 12; i++)
	{
		if (m_pLine[i] == nullptr) {
			continue;
		}
		m_pLine[i]->Uninit();
		m_pLine[i] = nullptr;
	}

	// 終了処理
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CCollisionLine_Box::Update()
{
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	bool bDisp = IsDisp();

	for (int i = 0; i < 12; ++i)
	{
		if (m_pLine[i] == nullptr) {
			continue;
		}
		m_pLine[i]->SetPosition(pos);
		m_pLine[i]->SetRotation(rot);
		m_pLine[i]->SetEnableDisp(bDisp);
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CCollisionLine_Box::Draw()
{
	// ボックスコライダーの描画
	if (!CManager::IsDisp_BoxColliders()) return;

	for (int i = 0; i < 12; ++i)
	{
		if (m_pLine[i] == nullptr) {
			continue;
		}
		m_pLine[i]->Draw();
	}
}

//==========================================================================
// 位置設定
//==========================================================================
void CCollisionLine_Box::SetPosition(const MyLib::Vector3& pos)
{
	CObject::SetPosition(pos);
	for (int i = 0; i < 12; ++i)
	{
		if (m_pLine[i] == nullptr) {
			continue;
		}
		m_pLine[i]->SetPosition(pos);
	}
}

//==========================================================================
// 色設定
//==========================================================================
void CCollisionLine_Box::SetColor(const D3DXCOLOR& col)
{
	for (int i = 0; i < 12; ++i)
	{
		if (m_pLine[i] == nullptr) {
			continue;
		}
		m_pLine[i]->SetColor(col);
	}
}

//==========================================================================
// AAbB情報設定
//==========================================================================
void CCollisionLine_Box::SetAABB(MyLib::AABB aabb, float scale)
{
	// 情報設定
	m_AABB = aabb;
	m_AABB.vtxMax *= scale;
	m_AABB.vtxMin *= scale;

	// ボックスの頂点
	MyLib::Vector3 vtx[8];
	vtx[0] = m_AABB.vtxMin; // 最小頂点
	vtx[1] = MyLib::Vector3(m_AABB.vtxMin.x, m_AABB.vtxMin.y, m_AABB.vtxMax.z);
	vtx[2] = MyLib::Vector3(m_AABB.vtxMin.x, m_AABB.vtxMax.y, m_AABB.vtxMin.z);
	vtx[3] = MyLib::Vector3(m_AABB.vtxMin.x, m_AABB.vtxMax.y, m_AABB.vtxMax.z);

	vtx[4] = MyLib::Vector3(m_AABB.vtxMax.x, m_AABB.vtxMin.y, m_AABB.vtxMin.z);
	vtx[5] = MyLib::Vector3(m_AABB.vtxMax.x, m_AABB.vtxMin.y, m_AABB.vtxMax.z);
	vtx[6] = MyLib::Vector3(m_AABB.vtxMax.x, m_AABB.vtxMax.y, m_AABB.vtxMin.z);
	vtx[7] = m_AABB.vtxMax; // 最大頂点

	// ライン生成
	for (int i = 0; i < 12; ++i)
	{
		m_pLine[i]->SetLeftPosition(vtx[IDXLIST[i][0]]);
		m_pLine[i]->SetRightPosition(vtx[IDXLIST[i][1]]);
	}
}
