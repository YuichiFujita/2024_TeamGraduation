//=============================================================================
// 
// �����ւ�(��)���� [dressup_hair.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "dressup_hair.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::wstring FOLDERNAME = L"data\\MODEL\\player\\hair";	// �ǂݍ��ރt�H���_��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CDressup_Hair::CDressup_Hair()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CDressup_Hair::~CDressup_Hair()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CDressup_Hair::Init()
{
	// �S�ēǂݍ���
	LoadAll(FOLDERNAME);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CDressup_Hair::Uninit()
{
	// �e�̏I��
	CDressup::Uninit();
	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CDressup_Hair::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�̍X�V
	CDressup::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	int modelSize = static_cast<int>(m_vecModelName.size());
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, m_nControllIdx))
	{// ���[�v
		m_nNowIdx = (m_nNowIdx + 1) % (modelSize - 1);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, m_nControllIdx))
	{// �t���[�v
		m_nNowIdx = (m_nNowIdx + (modelSize - 1)) % modelSize;
	}
}

//==========================================================================
// �f�o�b�O����
//==========================================================================
void CDressup_Hair::Debug()
{
	if (ImGui::TreeNode("dress-up_Hair"))
	{
		if (ImGui::SliderInt("Change Switch Idx", &m_nNowIdx, 0, static_cast<int>(m_vecModelName.size()) - 1))
		{
			// ���f���؂�ւ�
			m_pObjChara->ChangeObject(m_nSwitchIdx, m_vecModelName[m_nNowIdx]);
		}

		ImGui::TreePop();
	}

}
