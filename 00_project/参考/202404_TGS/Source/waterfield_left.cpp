//=============================================================================
// 
//  水フィールド処理 [waterfield_left.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "waterfield_left.h"
#include "manager.h"
#include "input.h"
#include "calculation.h"
#include "debugproc.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\FIELD\\water-bg-pattern-04.jpg";
	const float SCROLL_V = -0.0015f;
}

//==========================================================================
// コンストラクタ
//==========================================================================
CWaterField_Left::CWaterField_Left() : CWaterField()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CWaterField_Left::~CWaterField_Left()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CWaterField_Left::Init()
{
	// 水フィールドの初期化
	CWaterField::Init();

	// テクスチャの割り当て
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// 各種変数初期化
	SetPosition(MyLib::Vector3(0.0f, -50.0f, 0.0f));			// 位置


	// 全ての要素を書き換え
	D3DXCOLOR* pVtxCol = GetVtxCol();
	std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));

	return E_FAIL;
}

//==========================================================================
// 終了処理
//==========================================================================
void CWaterField_Left::Uninit()
{
	// 終了処理
	CWaterField::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CWaterField_Left::Update()
{
	m_fTexV += SCROLL_V;		// Vスクロール用
	m_fTexU -= SCROLL_V;		// Uスクロール用

	CWaterField::Update();

#if _DEBUG
	// カラーエディット
	static ImVec4 myColor = ImVec4(1.0f, 1.0f, 1.0f, 0.21f); // RGBA

	if (ImGui::ColorEdit4("Left Color", &myColor.x))
	{
		D3DXCOLOR* pVtxCol = GetVtxCol();

		// 全ての要素を書き換え
		std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(myColor.x, myColor.y, myColor.z, myColor.w));
	}
#endif
}
