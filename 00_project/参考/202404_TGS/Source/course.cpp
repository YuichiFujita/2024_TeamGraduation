//=============================================================================
// 
//  起伏処理 [course.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "course.h"
#include "course_title.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"
#include "particle.h"
#include "3D_effect.h"
#include "camera.h"
#include "spline.h"
#include "convert.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\FIELD\\water-bg-pattern-04.jpg";
	const int WIDTH_BLOCK = 2;
	const float WIDTH = 2000.0f;
	const float INTERVAL_TEXU = 500.0f;		// U座標の間隔
	const float INTERVAL_SINCURVE = 1200.0f;	// サインカーブの間隔
	const float HEIGHT_SINCURVE = 8.4f;	// サインカーブの高さ
	const float SCROLL_VELOCITY = 10.0f;
	const ImVec4 FILLCOLOR = ImVec4(0.80, 0.98, 1.0, 0.4f); // RGBA

}
const float CCourse::m_fCreateDistance = 600.0f;	// 生成間隔

//==========================================================================
// コンストラクタ
//==========================================================================
CCourse::CCourse(int nPriority, const LAYER layer) : CObject3DMesh(nPriority, layer)
{
	m_pCollisionLineBox.clear();	// 当たり判定ボックス
	m_vecSegmentPosition.clear();	// 基点の位置
	m_vecVtxInfo.clear();			// 各頂点の情報
	m_courceLength = 0.0f;
	m_fTexU = 0.0f;	// Uスクロール用
	m_fTexV = 0.0f;	// Vスクロール用
	m_fSinCurve = 0.0f;	// サインカーブの移動量
	m_fWaveHeight = HEIGHT_SINCURVE;	// サインカーブの高さ
	m_bEnableWave = true;	// 波の有効判定
}

//==========================================================================
// デストラクタ
//==========================================================================
CCourse::~CCourse()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCourse* CCourse::Create(const std::string& file, CScene::MODE mode)
{
	// メモリの確保
	CCourse* pObjMeshField = nullptr;

	switch (mode)
	{
	case CScene::MODE_TITLE:
		pObjMeshField = DEBUG_NEW CCourse_Title;
		break;

	case CScene::MODE_GAME:
		pObjMeshField = DEBUG_NEW CCourse;
		break;
	
	default:
		return pObjMeshField;
		break;
	}

	if (pObjMeshField != nullptr)
	{
		pObjMeshField->m_FileName = file;

		// 初期化処理
		pObjMeshField->Init();
	}

	return pObjMeshField;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCourse::Init()
{
	HRESULT hr;

	// ロード処理
	Load(m_FileName);

	// テクスチャの割り当て
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// オブジェクト3Dメッシュの初期化処理
	Reset();

	// 頂点情報設定
	SetVtx();


	D3DXCOLOR* pVtxCol = GetVtxCol();

	// 全ての要素を書き換え
	std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));



	// 頂点座標計算
	SetVtxPosition();

	SetPosition(MyLib::Vector3(0.0f, m_fWaveHeight, 0.0f));

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCourse::Uninit()
{
	m_pCollisionLineBox.clear();

	// 終了処理
	CObject3DMesh::Uninit();
}

//==========================================================================
// 各頂点計算
//==========================================================================
void CCourse::CalVtxPosition()
{

	// 最初と最後、逆方向に少し出す
	MyLib::Vector3 begin, end;
	float angle = 0.0f;

	// 最初
	angle = m_vecSegmentPosition[1].AngleXZ(m_vecSegmentPosition[0]);
	begin = MyLib::Vector3(
		m_vecSegmentPosition[0].x + sinf(angle) * -10.0f,
		m_vecSegmentPosition[0].y,
		m_vecSegmentPosition[0].z + cosf(angle) * -10.0f);

	// 最後
	int endIdx = m_vecSegmentPosition.size() - 1;
	angle = m_vecSegmentPosition[endIdx].AngleXZ(m_vecSegmentPosition[endIdx - 1]);
	end = MyLib::Vector3(
		m_vecSegmentPosition[endIdx].x + sinf(angle) * 10.0f,
		m_vecSegmentPosition[endIdx].y,
		m_vecSegmentPosition[endIdx].z + cosf(angle) * 10.0f);

	m_vecSegmentPosition.insert(m_vecSegmentPosition.begin(), begin);
	m_vecSegmentPosition.push_back(end);


	// 頂点情報クリア
	m_vecVtxInfo.clear();



	// セグメント分割計算
	std::vector<MyLib::Vector3> vecpos = CalSegmentDivision(m_vecSegmentPosition);

	// posの要素渡し
	std::transform(vecpos.begin(), vecpos.end(), std::back_inserter(m_vecVtxInfo), MyConvert::Vector3ToVtxInfo);
}

//==========================================================================
// セグメント分割計算
//==========================================================================
std::vector<MyLib::Vector3> CCourse::CalSegmentDivision(const std::vector<MyLib::Vector3>& segment)
{
	// セグメントの長さを計算
	int segmentSize = segment.size();
	std::vector<float> vecLength(segmentSize);

	for (int i = 0; i < segmentSize; ++i)
	{
		// 次回のインデックス(ループ)
		int next = (i + 1) % segmentSize;

		if (next == 0)
		{
			vecLength[i] = 10.0f;
			break;
		}

		// 点同士の距離
		vecLength[i] = segment[i].Distance(segment[next]);
	}

	// 各頂点格納
	m_courceLength = 0.0f;
	std::vector<MyLib::Vector3> vecpos;

	for (int i = 0; i < segmentSize; i++)
	{
		float distance = 0.0f;

		while (1)
		{
			distance += m_fCreateDistance;

			if (distance >= vecLength[i])
			{
				m_courceLength += m_fCreateDistance - (distance - vecLength[i]);

				distance = vecLength[i];

				vecpos.push_back(MySpline::GetSplinePosition_NonLoop(segment, m_courceLength, 20.0f));
				break;
			}

			m_courceLength += m_fCreateDistance;
			vecpos.push_back(MySpline::GetSplinePosition_NonLoop(segment, m_courceLength));
		}
	}

	return vecpos;
}

//==========================================================================
// リセット
//==========================================================================
void CCourse::Reset()
{
	// 終了処理
	CObject3DMesh::Uninit();

	// 死亡フラグ強制リセット
	SetEnableDeath(false);


	// テクスチャの割り当て
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);


	// 各頂点計算
	CalVtxPosition();

	// 各種変数初期化
	SetPosition(MyLib::Vector3(0.0f, 0.5f, 0.0f));				// 位置
	SetWidthBlock(1);		// 幅分割
	SetHeightBlock(static_cast<int>(m_vecVtxInfo.size()) - 1);	// 縦分割
	SetWidthLen(0.0f);		// 縦長さ
	SetHeightLen(0.0f);		// 横長さ

	// オブジェクト3Dメッシュの初期化処理
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// 頂点情報設定
	SetVtx();

	for (const auto& box : m_pCollisionLineBox)
	{
		box->Kill();
	}
	m_pCollisionLineBox.clear();

	MyLib::AABB aabb(-25.0f, 25.0f);
	for (const auto& vtx : m_vecSegmentPosition)
	{
		m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}

	// 頂点座標計算
	SetVtxPosition();
	SetPosition(MyLib::Vector3(0.0f, m_fWaveHeight, 0.0f));
}

//==========================================================================
// リセット
//==========================================================================
void CCourse::ReCreateVtx()
{

	// 終了処理
	CObject3DMesh::Uninit();

	// 死亡フラグ強制リセット
	SetEnableDeath(false);


	// テクスチャの割り当て
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// 最初と最後を消す
	m_vecSegmentPosition.erase(m_vecSegmentPosition.begin());
	m_vecSegmentPosition.pop_back();

	// 各頂点計算
	CalVtxPosition();

	// 各種変数初期化
	SetPosition(MyLib::Vector3(0.0f, 10.0f, 0.0f));				// 位置
	SetWidthBlock(1);		// 幅分割
	SetHeightBlock(static_cast<int>(m_vecVtxInfo.size()) - 1);	// 縦分割
	SetWidthLen(0.0f);		// 縦長さ
	SetHeightLen(0.0f);		// 横長さ
	m_fSinCurve = 0.0f;	// サインカーブの移動量

	// オブジェクト3Dメッシュの初期化処理
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// 頂点情報設定
	SetVtx();


	D3DXCOLOR* pVtxCol = GetVtxCol();

	// 全ての要素を書き換え
	std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(FILLCOLOR.x, FILLCOLOR.y, FILLCOLOR.z, FILLCOLOR.w));


	for (const auto& box : m_pCollisionLineBox)
	{
		box->Kill();
	}
	m_pCollisionLineBox.clear();

	MyLib::AABB aabb(-25.0f, 25.0f);
	for (const auto& vtx : m_vecSegmentPosition)
	{
		m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}

	// 頂点座標計算
	SetVtxPosition();

	SetPosition(MyLib::Vector3(0.0f, m_fWaveHeight, 0.0f));
}

//==========================================================================
// 両方の頂点座標計算
//==========================================================================
void CCourse::CalBothVtxPosition()
{
#if 0
	// 計算用変数
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;


	int segmentSize = static_cast<int>(m_vecSegmentPosition.size());

	std::vector<MyLib::Vector3> vecLeft, vecRight;
	MyLib::Vector3 rot;

	// 左右のセグメント計算
	for (int y = 0; y < segmentSize; y++)
	{
		// インデックス計算
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		// 各種マトリックス初期化
		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		// 向き反映
		int next = (y + 1) % segmentSize;

		bool bEnd = false;
		if (next == 0)
		{
			next = y - 1;
			bEnd = true;
		}

		// 次のセグメントとの向き取得
		rot.y = m_vecSegmentPosition[next].AngleXZ(m_vecSegmentPosition[y]);
		UtilFunc::Transformation::RotNormalize(rot.y);

		if (bEnd)
		{// 終端のみ
			rot.y *= -1;
		}

		// 回転反映
		mtxRotate.RotationYawPitchRoll(rot.y, rot.x, rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// 位置反映
		mtxTrans.Translation(m_vecSegmentPosition[y]);
		mtxParent.Multiply(mtxParent, mtxTrans);




		// オフセット反映
		offset = MyLib::Vector3(WIDTH, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(-WIDTH, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);

		// 頂点座標代入
		vecLeft.push_back(mtxLeft.GetWorldPosition());
		vecRight.push_back(mtxRight.GetWorldPosition());

		CEffect3D::Create(
			vecLeft.back() + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

		CEffect3D::Create(
			vecRight.back() + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	}


	// 頂点座標
	MyLib::Vector3* pVtxPos = GetVtxPos();
	int idx = 0;

	// セグメント分割計算
	std::vector<MyLib::Vector3> vecpos;

	// 左
	vecpos.clear();
	vecpos = CalSegmentDivision(vecLeft);
	idx = 0;
	for (int i = 0; i < static_cast<int>(vecpos.size()) - 1; i++)
	{
		// 頂点座標代入
		pVtxPos[idx] = vecpos[i];

		idx += WIDTH_BLOCK;
	}

	// 右
	vecpos.clear();
	vecpos = CalSegmentDivision(vecRight);
	idx = 1;
	for (int i = 0; i < static_cast<int>(vecpos.size()); i++)
	{
		// 頂点座標代入
		pVtxPos[idx] = vecpos[i];

		idx += WIDTH_BLOCK;
	}
#else

	// 計算用変数
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;


	int segmentSize = static_cast<int>(m_vecSegmentPosition.size());

	std::vector<MyLib::Vector3> vecLeft, vecRight;
	MyLib::Vector3 rot;

	// 左右のセグメント計算
	for (int y = 0; y < segmentSize; y++)
	{
		// インデックス計算
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		// 各種マトリックス初期化
		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		// 向き反映
		int next = (y + 1) % segmentSize;

		bool bEnd = false;
		if (next == 0)
		{
			next = y - 1;
			bEnd = true;
		}

		// 次のセグメントとの向き取得
		rot.y = m_vecSegmentPosition[next].AngleXZ(m_vecSegmentPosition[y]);
		UtilFunc::Transformation::RotNormalize(rot.y);

		if (bEnd)
		{// 終端のみ
			rot.y *= -1;
		}

		// 回転反映
		mtxRotate.RotationYawPitchRoll(rot.y, rot.x, rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// 位置反映
		mtxTrans.Translation(m_vecSegmentPosition[y]);
		mtxParent.Multiply(mtxParent, mtxTrans);




		// オフセット反映
		offset = MyLib::Vector3(WIDTH, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(-WIDTH, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);

		// 頂点座標代入
		vecLeft.push_back(mtxLeft.GetWorldPosition());
		vecRight.push_back(mtxRight.GetWorldPosition());

		CEffect3D::Create(
			vecLeft.back() + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

		CEffect3D::Create(
			vecRight.back() + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	}


	// 頂点座標
	MyLib::Vector3* pVtxPos = GetVtxPos();
	int idx = 0;

	// セグメント分割計算
	std::vector<MyLib::Vector3> vecpos;

	// 左
	vecpos.clear();
	vecpos = CalSegmentDivision(vecLeft);
	idx = 0;
	for (int i = 0; i < GetNumVertex() / 2; i++)
	{
		if (vecpos.size() <= i)
		{
			break;
		}

		// 頂点座標代入
		pVtxPos[idx] = vecpos[i];

		idx += WIDTH_BLOCK;
	}

	// 右
	vecpos.clear();
	vecpos = CalSegmentDivision(vecRight);
	idx = 1;
	for (int i = 0; i < GetNumVertex() / 2; i++)
	{
		if (vecpos.size() <= i)
		{
			break;
		}

		// 頂点座標代入
		pVtxPos[idx] = vecpos[i];

		idx += WIDTH_BLOCK;
	}

#endif

}

//==========================================================================
// 更新処理
//==========================================================================
void CCourse::Update()
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

#if _DEBUG

	static float velocity = SCROLL_VELOCITY;
	if (ImGui::TreeNode("Course"))
	{
		ImGui::Checkbox("Enable Wave!!!", &m_bEnableWave);
		ImGui::DragFloat("velocity", &velocity, 0.1f, 0.0f, 0.0f, "%.2f");
		/*ImGui::DragFloat("intervalWave", &INTERVAL_SINCURVE, 10.0f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("waveHeight", &HEIGHT_SINCURVE, 0.1f, 0.0f, 0.0f, "%.2f");*/
		SetPosition(MyLib::Vector3(0.0f, m_fWaveHeight, 0.0f));

		ImGui::TreePop();
	}

	// サインカーブの移動量
	m_fSinCurve -= velocity;

	// カラーエディット
	static ImVec4 myColor = ImVec4(0.80, 0.98, 1.0, 0.46f); // RGBA

	if (ImGui::ColorEdit4("Color", &myColor.x))
	{
		D3DXCOLOR* pVtxCol = GetVtxCol();

		// 全ての要素を書き換え
		std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(myColor.x, myColor.y, myColor.z, myColor.w));
	}

#else

	// サインカーブの移動量
	m_fSinCurve -= SCROLL_VELOCITY;

#endif

	// 頂点座標
	SetVtx();
}

//==========================================================================
// 頂点座標
//==========================================================================
void CCourse::SetVtxPosition()
{
#if 0
	// 計算用変数
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;

	MyLib::Vector3* pVtxPos = GetVtxPos();

	MyLib::Vector3 beforeVec = MyLib::Vector3(0.0f, 0.0f, 1.0f);

	for (int y = 0; y < static_cast<int>(m_vecVtxInfo.size()); y++)
	{
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		// 向き反映
		int next = (y + 1) % static_cast<int>(m_vecVtxInfo.size());

		bool bEnd = false;
		if (next == 0)
		{
			next = y - 1;
			bEnd = true;
		}

		m_vecVtxInfo[y].rot.y = m_vecVtxInfo[next].pos.AngleXZ(m_vecVtxInfo[y].pos);
		UtilFunc::Transformation::RotNormalize(m_vecVtxInfo[y].rot.y);

		if (bEnd)
		{
			m_vecVtxInfo[y].rot.y *= -1;
		}

		// 回転反映
		mtxRotate.RotationYawPitchRoll(m_vecVtxInfo[y].rot.y, m_vecVtxInfo[y].rot.x, m_vecVtxInfo[y].rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// 位置反映
		mtxTrans.Translation(m_vecVtxInfo[y].pos);
		mtxParent.Multiply(mtxParent, mtxTrans);

		if (y == 1)
		{// 初回スキップ
			CEffect3D::Create(
				m_vecVtxInfo[y].pos + GetPosition(),
				0.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				20.0f, 4, CEffect3D::MOVEEFFECT::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
		}

		// オフセット反映
		offset = MyLib::Vector3(WIDTH, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(-WIDTH, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);

		//mtxLeft.Multiply(mtxLeft, mtxRotate);
		ImGui::Text("x:%f y:%f z:%f, rot.y:%f", m_vecVtxInfo[y].pos.x, m_vecVtxInfo[y].pos.y, m_vecVtxInfo[y].pos.z, m_vecVtxInfo[y].rot.y);

		// 頂点座標代入
		pVtxPos[idx] = mtxLeft.GetWorldPosition();
		pVtxPos[nextidx] = mtxRight.GetWorldPosition();


		if (y > 0)
		{// 初回スキップ

			int up = idx - WIDTH_BLOCK, down = nextidx - WIDTH_BLOCK;
			int before = y - 1;



			// ベクトルABを90度反時計回りに回転
			MyLib::Vector3 rotatedB = (m_vecVtxInfo[before].pos - m_vecVtxInfo[y].pos).rotateAroundY();

			// 新しいベクトルの終点を計算
			MyLib::Vector3 newB = m_vecVtxInfo[y].pos + rotatedB;


			MyLib::Vector3 AB = newB - m_vecVtxInfo[y].pos;
			MyLib::Vector3 AP = pVtxPos[idx] - m_vecVtxInfo[y].pos;

			// 今回の点が前回のよりも左にあったら同地点に置く
			// 上
			float crossProductY = AB.x * AP.z - AB.z * AP.x;
			if (crossProductY > 0)
			{
				pVtxPos[idx] = pVtxPos[up];

				CEffect3D::Create(
					pVtxPos[idx] + GetPosition(),
					0.0f,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					80.0f, 2, CEffect3D::MOVEEFFECT::MOVEEFFECT_GENSUI, CEffect3D::TYPE_NORMAL);
			}

			AP = pVtxPos[nextidx] - m_vecVtxInfo[y].pos;

			// 下
			crossProductY = AB.x * AP.z - AB.z * AP.x;
			if (crossProductY > 0)
			{
				pVtxPos[nextidx] = pVtxPos[down];
			}

		}



	}

#else
	// 両方の頂点座標計算
	CalBothVtxPosition();
#endif

	int i = 0;
	MyLib::Vector3 fieldpos = GetPosition();
	for (const auto& vtx : m_vecSegmentPosition)
	{
		if (m_pCollisionLineBox.empty()) break;

		MyLib::Vector3 setpos = vtx + fieldpos;
		m_pCollisionLineBox[i]->SetPosition(setpos);
		i++;
	}

}

//==========================================================================
// 高さ取得
//==========================================================================
float CCourse::GetHeight(const MyLib::Vector3& pos, bool* pLand)
{
	// ベクトルと法線
	MyLib::Vector3 vec1, vec2, nor;
	float fHeight = 0.0f;
	MyLib::Vector3* pVtxPos = GetVtxPos();

	// フィールドの位置
	MyLib::Vector3 posfield = GetPosition();
	int nHeight = GetHeightBlock();
	int nWidth = GetWidthBlock();

	// 長さ取得
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();

	// 最大の長さ
	float fMaxWidthLen = fWidthLen * GetWidthBlock();
	float fMaxHeightLen = -fWidthLen * GetHeightBlock();

	// 判定する頂点の計算
	int nCntWidth = 0;
	int nCntHeight = 0;
	int nWidthPoint = static_cast<int>(((pos.x - posfield.x) - fMaxWidthLen * 0.5f) / fWidthLen + GetWidthBlock());
	int nHeightPoint = static_cast<int>(GetHeightBlock() - ((pos.z - posfield.z) - fMaxHeightLen * 0.5f) / fHeightLen);

	for (int nCntH = 0; nCntH < 2; nCntH++)
	{
		for (int nCntW = 0; nCntW < 2; nCntW++)
		{
			// 横頂点
			nCntWidth = nWidthPoint + (nCntW - 1);
			nCntHeight = nHeightPoint + (nCntH - 1);

			if (nCntWidth < 0 || nWidth < nCntWidth)
			{// 範囲外で先頭
				continue;
			}

			if (nCntHeight < 0 || nHeight < nCntHeight)
			{// 範囲外で先頭
				continue;
			}

			// 今回の頂点
			int nNowPoint = (nCntWidth + 1) + (nCntHeight * (nWidth + 1));
			int nVerTexW = (nWidth + 1) + 1;

			int nLeft = nCntWidth + (nCntHeight * (nWidth + 1));
			int nRight = nCntWidth + (nCntHeight * (nWidth + 1)) + nVerTexW;

			if (nLeft >= GetNumVertex() || nRight >= GetNumVertex())
			{
				continue;
			}

			fHeight = UtilFunc::Calculation::GetVtxHeight(pos, pVtxPos[nNowPoint] + posfield, pVtxPos[nLeft] + posfield, pVtxPos[nRight] + posfield, pLand);
			if (*pLand)
			{
				break;
			}

			fHeight = UtilFunc::Calculation::GetVtxHeight(pos, pVtxPos[nNowPoint + GetWidthBlock()] + posfield, pVtxPos[nRight] + posfield, pVtxPos[nLeft] + posfield, pLand);
			if (*pLand)
			{
				break;
			}
		}
	}

	if (*pLand)
	{// 着地していたら

		return fHeight;
	}

	// 高さを取得
	return pos.y;
}

//==========================================================================
// 自分のいる頂点のインデックス取得
//==========================================================================
int CCourse::GetMyVtxIndex(const MyLib::Vector3& _pos)
{
	int idx = -1;
	for (const auto& vtxInfo : m_vecVtxInfo)
	{
		const MyLib::Vector3& pos = vtxInfo.pos;

		if (pos.x <= _pos.x)
		{
			idx++;
		}
		else
		{
			break;
		}
	}

	return idx;
}

//==========================================================================
// 頂点からなる平面との当たり判定
//==========================================================================
bool CCourse::CollisionVtxQuad(int idx, MyLib::Vector3 rayPos, MyLib::Vector3* colPos)
{
	// 例外処理
	if (static_cast<int>(m_vecVtxInfo.size()) <= idx) return false;
	if (idx < 0) return false;

	MyLib::Vector3* pVtxPos = GetVtxPos();
	MyLib::Vector3 rayVec(0.0f, -1.0f, 0.0f);

	// レイと4点の判定
	int calIdx = idx * 2;
	float raylen = 90000.0f;

	// 時計回り
	//MyLib::Vector3 pos0(pVtxPos[calIdx + 1]), pos1(pVtxPos[calIdx + 3]), pos2(pVtxPos[calIdx + 2]), pos3(pVtxPos[calIdx + 0]);
	MyLib::Vector3 pos0(pVtxPos[calIdx + 3]), pos1(pVtxPos[calIdx + 1]), pos2(pVtxPos[calIdx + 0]), pos3(pVtxPos[calIdx + 2]);

	pos0 += GetPosition();
	pos1 += GetPosition();
	pos2 += GetPosition();
	pos3 += GetPosition();

	bool bHit = UtilFunc::Collision::CollisionRayQuad(&rayPos, &rayVec,
		&pos0, &pos1, &pos2, &pos3,
		raylen, colPos);
	//*colPos += GetPosition();

#if 0

	MyLib::Vector3 aaaa(800.0f, 2000.0f, 0.0f), koutne;
	int courseIdx = GetMyVtxIndex(aaaa);
	courseIdx *= 2;

	MyLib::Vector3 Apos0(pVtxPos[courseIdx + 3]), Apos1(pVtxPos[courseIdx + 1]), Apos2(pVtxPos[courseIdx + 0]), Apos3(pVtxPos[courseIdx + 2]);
	Apos0 += GetPosition();
	Apos1 += GetPosition();
	Apos2 += GetPosition();
	Apos3 += GetPosition();

	UtilFunc::Collision::CollisionRayQuad(&aaaa, &rayVec,
		&Apos0, &Apos1, &Apos2, &Apos3,
		raylen, &koutne);
	//koutne += GetPosition();

	CEffect3D::Create(
		koutne,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
		40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);


	CEffect3D::Create(
		pos0,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	CEffect3D::Create(
		pos1,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	CEffect3D::Create(
		pos2,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	CEffect3D::Create(
		pos3,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	ImGui::Text("rot:x%.2f, %.2f, %.2f", pos1.x, pos1.y, pos1.z);
	ImGui::Text("rot:x%.2f, %.2f, %.2f", pos2.x, pos2.y, pos2.z);
	ImGui::Text("rot:x%.2f, %.2f, %.2f", pos3.x, pos3.y, pos3.z);
#endif

	return bHit;
}

//==========================================================================
// 描画処理
//==========================================================================
void CCourse::Draw()
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
void CCourse::SetVtx()
{

	MyLib::Vector3 *pVtxPos = GetVtxPos();
	MyLib::Vector3 *pVtxNor = GetVtxNor();
	D3DXVECTOR2* pTex = GetVtxTex();
	MyLib::Vector3 vec1, vec2, nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	MyLib::Vector3 VtxRight, VtxLeft, VtxNow;
	int nHeight = GetHeightBlock();
	int nWidth = GetWidthBlock();
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();
	int vtxNum = GetNumVertex();

	int heightBlock = static_cast<int>(m_vecVtxInfo.size());

	// テクスチャ情報
	float posU_UP = 0.0f, posU_Down = 0.0f, posV = 0.0f;
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	float intervalV = UtilFunc::Transformation::AdjustSizeByWidth(CTexture::GetInstance()->GetImageSize(texID), INTERVAL_TEXU).y;
	int idx = 0, nextIdx = 0;

	float sincurveDistance = 0.0f;
	for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
	{// 縦の分割分繰り返す

		// リセット
		posV = 0.0f;

		for (int nCntWidth = 0; nCntWidth < nWidth + 1; nCntWidth++)
		{// 横の分割分繰り返す

			// 今回の頂点インデックス
			idx = nCntWidth + (nCntHeight * (nWidth + 1));

			// 波
			if (m_bEnableWave)
			{
				if (nCntHeight != 0)
				{
					// 前回との距離
					sincurveDistance += m_vecVtxInfo[nCntHeight].pos.DistanceXZ(m_vecVtxInfo[nCntHeight - 1].pos);
					pVtxPos[idx].y = sinf(D3DX_PI * ((sincurveDistance + m_fSinCurve) / INTERVAL_SINCURVE)) * m_fWaveHeight;
				}
			}
			else
			{
				pVtxPos[idx].y = 0.0f;
			}

			// 隣のインデックス
			nextIdx = idx + 2;

			// UV座標設定
			if (nCntWidth == 0)
			{
				pTex[idx] = D3DXVECTOR2(posU_UP + m_fTexU, posV + m_fTexV);


				// 横の割合分進める
				if (nextIdx < vtxNum)
				{
					posU_UP += pVtxPos[idx].DistanceXZ(pVtxPos[idx + 2]) / INTERVAL_TEXU;
				}
				
			}
			else
			{
				pTex[idx] = D3DXVECTOR2(posU_Down + m_fTexU, posV + m_fTexV);

				// 横の割合分進める
				if (nextIdx < vtxNum)
				{
					posU_Down += pVtxPos[idx].DistanceXZ(pVtxPos[idx + 2]) / INTERVAL_TEXU;
				}
			}

			// 縦の割合分進める
			posV += pVtxPos[idx].DistanceXZ(pVtxPos[idx + 1]) / intervalV;
			
			pVtxNor[idx] = nor;
		}
	}

	// 頂点情報更新
	CObject3DMesh::SetVtx();
}

//==========================================================================
// ロード処理
//==========================================================================
HRESULT CCourse::Load(const std::string& file)
{
	// ファイルを開く
	std::ifstream File(file, std::ios::binary);
	if (!File.is_open()) {

		// 例外処理
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 0.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 0.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 500.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 1000.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 1800.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 1800.0f });

		MyLib::AABB aabb(-25.0f, 25.0f);

		for (int i = 0; i < static_cast<int>(m_vecSegmentPosition.size()); i++)
		{
			m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
		}

		// オブジェクト3Dメッシュの初期化処理
		Reset();

		Save();
		return E_FAIL;
	}

	// 構造体のサイズを取得
	std::streamsize structSize = sizeof(MyLib::Vector3);
	
	// ファイルの末尾までデータを読み込む
	File.seekg(0, std::ios::end);
	std::streampos fileSize = File.tellg();
	File.seekg(0, std::ios::beg);
	
	// データの個数を計算
	size_t numVectors = fileSize / structSize;
	
	// ベクトルの配列を用意
	m_vecSegmentPosition.resize(numVectors);
	
	// ファイルからデータを読み込む
	File.read(reinterpret_cast<char*>(m_vecSegmentPosition.data()), fileSize);

	// ファイルを閉じる
	File.close();

#if 0
	int i = 0;
	for (auto& vvv : m_vecSegmentPosition)
	{
		if (i != 0)
		{
			vvv.x *= 1.25f;
		}
		i++;
	}
	//Save();
#endif

	return S_OK;
}

//==========================================================================
// セーブ処理
//==========================================================================
void CCourse::Save()
{
	// ファイルを開く
	std::ofstream File(m_FileName, std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	std::vector<MyLib::Vector3> savedata = m_vecSegmentPosition;
	savedata.erase(savedata.begin());
	savedata.pop_back();


	// データをバイナリファイルに書き出す
	File.write(reinterpret_cast<char*>(savedata.data()), savedata.size() * sizeof(MyLib::Vector3));
		
	// ファイルを閉じる
	File.close();
}

//==========================================================================
// 当たり判定ボックス取得
//==========================================================================
CCollisionLine_Box* CCourse::GetCollisionLineBox(int idx)
{
	if (static_cast<int>(m_pCollisionLineBox.size()) <= idx) return nullptr;

	return m_pCollisionLineBox[idx];
}

//==========================================================================
// 各頂点情報の位置取得
//==========================================================================
CCourse::VtxInfo CCourse::GetVecVtxinfo(int idx)
{
	if (static_cast<int>(m_vecVtxInfo.size()) <= idx) return VtxInfo();

	return m_vecVtxInfo[idx];
}

//==========================================================================
// 各頂点情報の位置設定
//==========================================================================
void CCourse::SetVecVtxinfo(int idx, const VtxInfo& vecinfo)
{
	if (static_cast<int>(m_vecVtxInfo.size()) <= idx) return;

	m_vecVtxInfo[idx] = vecinfo;
}

//==========================================================================
// 基点の位置取得
//==========================================================================
MyLib::Vector3 CCourse::GetVecPosition(int idx)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return MyLib::Vector3();

	return m_vecSegmentPosition[idx];
}

//==========================================================================
// 基点の位置設定
//==========================================================================
void CCourse::SetVecPosition(int idx, const MyLib::Vector3& pos)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return;

	m_vecSegmentPosition[idx] = pos;
}
