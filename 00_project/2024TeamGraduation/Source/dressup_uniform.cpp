//=============================================================================
// 
// �����ւ�(���j�t�H�[��)���� [dressup_uniform.cpp]
// Author : ���n�Ή�
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
// �萔��`
//==========================================================================
namespace
{
	const std::wstring FOLDERNAME = L"data\\TEXTURE\\player\\uniform";	// �ǂݍ��ރt�H���_��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CDressup_Uniform::CDressup_Uniform()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CDressup_Uniform::~CDressup_Uniform()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CDressup_Uniform::Init()
{
	// �S�ēǂݍ���
	LoadAllTexture(FOLDERNAME);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CDressup_Uniform::Uninit()
{
	// �e�̏I��
	CDressup::Uninit();
	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CDressup_Uniform::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�̍X�V
	CDressup::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	int texSize = static_cast<int>(m_vecModelName.size());
	bool bChange = false;	// �ύX�t���O

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, m_nControllIdx))
	{// ���[�v
		m_nNowIdx = (m_nNowIdx + 1) % texSize;
		bChange = true;
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, m_nControllIdx))
	{// �t���[�v
		m_nNowIdx = (m_nNowIdx + (texSize - 1)) % texSize;
		bChange = true;
	}

	// ���f���؂�ւ�
	if (bChange)
	{
		ReRegist();
	}
}

//==========================================================================
// �Ċ��蓖��
//==========================================================================
void CDressup_Uniform::ReRegist()
{
	// ��؂�ւ�
	CModel* pModel = nullptr;
	int nNumModel = m_pObjChara->GetNumModel();

	// �e�N�X�`���擾
	int idx = CTexture::GetInstance()->Regist(m_vecModelName[m_nNowIdx]);

	for (int i = 0; i < nNumModel; i++)
	{
		// ���f���擾
		pModel = m_pObjChara->GetModel(i);
		if (pModel == nullptr) continue;

		// �ύX���Ȃ����
		if (i == CPlayer::ID_FACE ||
			i == CPlayer::ID_HAIR ||
			i == CPlayer::ID_ACCESSORY) continue;

		// �e�N�X�`���ݒ�
		pModel->SetIdxTexture(0, idx);
	}

}

//==========================================================================
// �f�o�b�O����
//==========================================================================
void CDressup_Uniform::Debug()
{
	if (ImGui::TreeNode("dress-up_Face"))
	{
		if (ImGui::SliderInt("Change Switch Idx", &m_nNowIdx, 0, static_cast<int>(m_vecModelName.size()) - 1))
		{
			// ��؂�ւ�
			CModel* pModel = m_pObjChara->GetModel(m_nSwitchIdx);
			int idx = CTexture::GetInstance()->Regist(m_vecModelName[m_nNowIdx]);
			pModel->SetIdxTexture(0, idx);
		}
		ImGui::TreePop();
	}

}
