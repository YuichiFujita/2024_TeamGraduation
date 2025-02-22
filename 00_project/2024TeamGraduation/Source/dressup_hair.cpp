//=============================================================================
// 
// 着せ替え(髪)処理 [dressup_hair.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "dressup_hair.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::wstring FOLDERNAME = L"data\\MODEL\\player\\hair";	// 読み込むフォルダ名
}

//==========================================================================
// コンストラクタ
//==========================================================================
CDressup_Hair::CDressup_Hair()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CDressup_Hair::~CDressup_Hair()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CDressup_Hair::Init()
{
	// 全て読み込み
	LoadAllModel(FOLDERNAME);

	// 親の初期化
	CDressup::Init();
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CDressup_Hair::Uninit()
{
	// 親の終了
	CDressup::Uninit();
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CDressup_Hair::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親の更新
	CDressup::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	int modelSize = static_cast<int>(m_vecModelName.size());
	bool bChange = false;	// 変更フラグ

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, m_nControllIdx)
	||  pPad->GetLStickTrigger(m_nControllIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_RIGHT))
	{// ループ
		m_nNowIdx = (m_nNowIdx + 1) % modelSize;
		bChange = true;
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, m_nControllIdx)
		 ||  pPad->GetLStickTrigger(m_nControllIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_LEFT))
	{// 逆ループ
		m_nNowIdx = (m_nNowIdx + (modelSize - 1)) % modelSize;
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
void CDressup_Hair::ReRegist()
{
	m_pObjChara->ChangeObject(m_nSwitchIdx, m_vecModelName[m_nNowIdx]);
}

//==========================================================================
// デバッグ処理
//==========================================================================
void CDressup_Hair::Debug()
{
	if (ImGui::TreeNode("dress-up_Hair"))
	{
		if (ImGui::SliderInt("Change Switch Idx", &m_nNowIdx, 0, static_cast<int>(m_vecModelName.size()) - 1))
		{
			// モデル切り替え
			m_pObjChara->ChangeObject(m_nSwitchIdx, m_vecModelName[m_nNowIdx]);
		}

		ImGui::TreePop();
	}

}
