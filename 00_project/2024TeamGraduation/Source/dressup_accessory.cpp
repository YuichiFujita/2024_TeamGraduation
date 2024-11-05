//=============================================================================
// 
// �����ւ�(�A�N�Z)���� [dressup_accessory.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "dressup_accessory.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::wstring FOLDERNAME = L"data\\MODEL\\player\\accessory";	// �ǂݍ��ރt�H���_��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CDressup_Accessory::CDressup_Accessory()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CDressup_Accessory::~CDressup_Accessory()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CDressup_Accessory::Init()
{
	// �S�ēǂݍ���
	LoadAll(FOLDERNAME);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CDressup_Accessory::Uninit()
{
	// �e�̏I��
	CDressup::Uninit();
	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CDressup_Accessory::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�̍X�V
	CDressup::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �f�o�b�O����
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
				// ���f���؂�ւ�
				m_pObjChara->ChangeObject(m_nSwitchIdx, m_vecModelName[m_nNowIdx - 1]);
			}
		}

		ImGui::TreePop();
	}

}
