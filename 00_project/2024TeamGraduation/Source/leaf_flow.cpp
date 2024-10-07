//=============================================================================
// 
//  葉っぱ処理 [leaf_flow.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "leaf_flow.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "game.h"
#include "course.h"
#include "spline.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\effect\\leaf.png";	// テクスチャのファイル
}

namespace StateTime	// 状態別時間
{
	const float WAIT = 0.2f;	// 待機
	const float FADEOUT = 0.3f;	// フェードアウト
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CLeafFlow::STATE_FUNC CLeafFlow::m_StateFunc[] =
{
	&CLeafFlow::StateFlow,		// 流れ
	&CLeafFlow::StateFadeOut,	// フェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CLeafFlow::CLeafFlow(int nPriority) : CLeaf(nPriority)
{
	// 値のクリア
	m_fStateTimer = 0.0f;			// 状態カウンター
	m_state = CLeafFlow::State::STATE_FLOW;	// 状態
	m_fMoveLen = 0.0f;	// 移動距離
}

//==========================================================================
// デストラクタ
//==========================================================================
CLeafFlow::~CLeafFlow()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CLeafFlow::Init()
{

	// 葉っぱの初期化
	CLeaf::Init();

	// 向き
	MyLib::Vector3 rot;
	rot.y = UtilFunc::Transformation::Random(-31, 31) * 0.1f;
	rot.x = D3DX_PI * 0.5f;
	SetRotation(rot);

	m_fMoveLen = 0.0f;	// 移動距離

	return S_OK;
}

//==========================================================================
// 更新処理
//==========================================================================
void CLeafFlow::Update()
{
	// 状態更新
	UpdateState();
	if (IsDeath()) return;

	// 更新処理
	CObject3D::Update();
}

//==========================================================================
// 状態更新
//==========================================================================
void CLeafFlow::UpdateState()
{
	// 状態タイマー
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// 状態更新
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// 流れ
//==========================================================================
void CLeafFlow::StateFlow()
{
	MyLib::Vector3 pos = GetPosition();

	// コース取得
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	int courseIdx = pCourse->GetMyVtxIndex(pos);

	// 位置計算
	m_fMoveLen += 5.0f;

	MyLib::Vector3 calpos;
	calpos = MySpline::GetSplinePosition_NonLoop(pCourse->GetVecPosition(), m_fMoveLen);
	pos.x = calpos.x;
	pos.z = calpos.z;

	pos.z += GetOriginPosition().z;

	// 平面判定
	MyLib::Vector3 colPos[3];
	bool bHit[3] = {};

	bHit[0] = pCourse->CollisionVtxQuad(courseIdx, MyLib::Vector3(pos.x, 2000.0f, pos.z), &colPos[0]);
	/*bHit[1] = pCourse->CollisionVtxQuad(courseIdx - 1, MyLib::Vector3(pos.x, 2000.0f, pos.z), &colPos[1]);
	bHit[2] = pCourse->CollisionVtxQuad(courseIdx + 1, MyLib::Vector3(pos.x, 2000.0f, pos.z), &colPos[2]);*/

	float minHeight = 2000.0f;
	bool bAllHit = false;
	for (int i = 0; i < 3; i++)
	{
		if (bHit[i] &&
			minHeight >= colPos[i].y)
		{
			minHeight = colPos[i].y;

			bAllHit = true;
		}
	}

	if (bAllHit)
	{
		pos.y = minHeight + 2.0f;
	}
	SetPosition(pos);

	if (pos.x >= GetOriginPosition().x + 20000.0f)
	{// 時間経過

		m_state = State::STATE_FADEOUT;
		m_fStateTimer = 0.0f;
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CLeafFlow::StateFadeOut()
{
	float alpha = 1.0f - (m_fStateTimer / StateTime::FADEOUT);
	SetAlpha(alpha);

	if (m_fStateTimer >= StateTime::FADEOUT)
	{// 時間経過

		Uninit();
	}
}

//==========================================================================
// 移動距離設定
//==========================================================================
void CLeafFlow::SetMoveLen(float len)
{

	// 移動距離設定
	m_fMoveLen = len;

	// コース取得
	CCourse* pCourse = CGame::GetInstance()->GetCourse();

	// 位置補正
	MyLib::Vector3 calpos;
	MyLib::Vector3 pos = GetPosition();
	calpos = MySpline::GetSplinePosition_NonLoop(pCourse->GetVecPosition(), m_fMoveLen);
	pos.x = calpos.x;
	pos.z = calpos.z;
	pos.z += GetOriginPosition().z;
	SetPosition(pos);
}
