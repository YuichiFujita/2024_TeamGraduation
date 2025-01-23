//=============================================================================
// 
// 着せ替え(顔)処理 [dressup_face.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "dressup_face.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"
#include "model.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::wstring FOLDERNAME = L"data\\TEXTURE\\player\\face";	// 読み込むフォルダ名
}

//==========================================================================
// コンストラクタ
//==========================================================================
CDressup_Face::CDressup_Face()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CDressup_Face::~CDressup_Face()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CDressup_Face::Init()
{
	// 全て読み込み
	LoadAllTexture(FOLDERNAME);

	// 親の初期化
	CDressup::Init();
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CDressup_Face::Uninit()
{
	// 親の終了
	CDressup::Uninit();
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CDressup_Face::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親の更新
	CDressup::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	int texSize = static_cast<int>(m_vecModelName.size());
	bool bChange = false;	// 変更フラグ

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, m_nControllIdx))
	{// ループ
		m_nNowIdx = (m_nNowIdx + 1) % texSize;
		bChange = true;
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, m_nControllIdx))
	{// 逆ループ
		m_nNowIdx = (m_nNowIdx + (texSize - 1)) % texSize;
		bChange = true;
	}

	// モデル切り替え
	if (bChange)
	{
		ReRegist();
	}
}

//==========================================================================
// 再割り当て
//==========================================================================
void CDressup_Face::ReRegist()
{
	// 顔切り替え
	CModel* pModel = m_pObjChara->GetModel(m_nSwitchIdx);
	int idx = CTexture::GetInstance()->Regist(m_vecModelName[m_nNowIdx]);
	
	// テクスチャインデックス設定
	std::vector<int> vecIdx = pModel->GetIdxTexture();
	std::fill(vecIdx.begin(), vecIdx.end(), idx);
	pModel->SetIdxTexture(vecIdx);
}

//==========================================================================
// デバッグ処理
//==========================================================================
void CDressup_Face::Debug()
{
	if (ImGui::TreeNode("dress-up_Face"))
	{
		if (ImGui::SliderInt("Change Switch Idx", &m_nNowIdx, 0, static_cast<int>(m_vecModelName.size()) - 1))
		{
			// 顔切り替え
			CModel* pModel = m_pObjChara->GetModel(m_nSwitchIdx);
			int idx = CTexture::GetInstance()->Regist(m_vecModelName[m_nNowIdx]);
			pModel->SetIdxTexture(0, idx);
		}
		ImGui::TreePop();
	}

}
