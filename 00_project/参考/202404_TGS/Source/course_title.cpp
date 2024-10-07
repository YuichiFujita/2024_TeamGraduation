//=============================================================================
// 
//  起伏処理 [course_title.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
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

}

//==========================================================================
// コンストラクタ
//==========================================================================
CCourse_Title::CCourse_Title(int nPriority, const LAYER layer) : CCourse(nPriority, layer)
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CCourse_Title::~CCourse_Title()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCourse_Title::Init()
{
	HRESULT hr;

	// 波の高さ
	m_fWaveHeight = 20.0f;

	// 基点
	m_vecSegmentPosition.clear();
	m_vecSegmentPosition.push_back(MyLib::Vector3(-15000.0f, 0.0f, 0.0f));
	m_vecSegmentPosition.push_back(MyLib::Vector3(0.0f, 0.0f, 0.0f));
	m_vecSegmentPosition.push_back(MyLib::Vector3(15000.0f, 0.0f, 0.0f));
	m_vecSegmentPosition.push_back(MyLib::Vector3(50000.0f, 0.0f, 0.0f));


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
void CCourse_Title::Uninit()
{
	// 終了処理
	CCourse::Uninit();
}
