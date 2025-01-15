//=============================================================================
// 
// 着せ替え(ユニフォーム)処理 [dressup_uniform.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "dressup_uniform.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"
#include "player.h"
#include "model.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::wstring FOLDERNAME = L"data\\TEXTURE\\player\\uniform";	// 読み込むフォルダ名
}

//==========================================================================
// コンストラクタ
//==========================================================================
CDressup_Uniform::CDressup_Uniform()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CDressup_Uniform::~CDressup_Uniform()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CDressup_Uniform::Init()
{
	// 全て読み込み
	LoadAllTexture(FOLDERNAME);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CDressup_Uniform::Uninit()
{
	// 親の終了
	CDressup::Uninit();
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CDressup_Uniform::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CDressup_Uniform::ReRegist()
{
	// 顔切り替え
	CModel* pModel = nullptr;
	int nNumModel = m_pObjChara->GetNumModel();

	// テクスチャ取得
	int idx = CTexture::GetInstance()->Regist(m_vecModelName[m_nNowIdx]);

	for (int i = 0; i < nNumModel; i++)
	{
		// モデル取得
		pModel = m_pObjChara->GetModel(i);
		if (pModel == nullptr) continue;

		// 変更しないやつ
		if (i == CPlayer::ID_FACE ||
			i == CPlayer::ID_HAIR ||
			i == CPlayer::ID_ACCESSORY) continue;

		// テクスチャ設定
		pModel->SetIdxTexture(0, idx);
	}

}

//==========================================================================
// デバッグ処理
//==========================================================================
void CDressup_Uniform::Debug()
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
