//=============================================================================
// 
// 着せ替え(アクセ)処理 [dressup_accessory.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "dressup_accessory.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::wstring FOLDERNAME = L"data\\MODEL\\player\\accessory";	// 読み込むフォルダ名
}

//==========================================================================
// コンストラクタ
//==========================================================================
CDressup_Accessory::CDressup_Accessory()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CDressup_Accessory::~CDressup_Accessory()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CDressup_Accessory::Init()
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
void CDressup_Accessory::Uninit()
{
	// 親の終了
	CDressup::Uninit();
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CDressup_Accessory::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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

	// 変更してなかったら終わる
	if (!bChange) return;

	// 再割り当て
	ReRegist();
}

//==========================================================================
// 再割り当て
//==========================================================================
void CDressup_Accessory::ReRegist()
{
	if (m_nNowIdx == 0)
	{
		m_pObjChara->DeleteObject(m_nSwitchIdx);
	}
	else
	{
		// モデル切り替え
		m_pObjChara->ChangeObject(m_nSwitchIdx, m_vecModelName[m_nNowIdx - 1]);
	}
}

//==========================================================================
// デバッグ処理
//==========================================================================
void CDressup_Accessory::Debug()
{
	if (ImGui::TreeNode("dress-up_Accessory"))
	{
		if (ImGui::SliderInt("Change Switch Idx", &m_nNowIdx, 0, static_cast<int>(m_vecModelName.size())))
		{
			if (m_nNowIdx == 0)
			{
				m_pObjChara->DeleteObject(m_nSwitchIdx);
			}
			else
			{
				// モデル切り替え
				m_pObjChara->ChangeObject(m_nSwitchIdx, m_vecModelName[m_nNowIdx - 1]);
			}
		}

		ImGui::TreePop();
	}

}
