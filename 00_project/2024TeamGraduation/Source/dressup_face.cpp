//=============================================================================
// 
// �����ւ�(��)���� [dressup_face.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "dressup_face.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"
#include "model.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::wstring FOLDERNAME = L"data\\TEXTURE\\player\\face";	// �ǂݍ��ރt�H���_��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CDressup_Face::CDressup_Face()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CDressup_Face::~CDressup_Face()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CDressup_Face::Init()
{
	// �S�ēǂݍ���
	LoadAllTexture(FOLDERNAME);

	// �e�̏�����
	CDressup::Init();
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CDressup_Face::Uninit()
{
	// �e�̏I��
	CDressup::Uninit();
	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CDressup_Face::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CDressup_Face::ReRegist()
{
	// ��؂�ւ�
	CModel* pModel = m_pObjChara->GetModel(m_nSwitchIdx);
	int idx = CTexture::GetInstance()->Regist(m_vecModelName[m_nNowIdx]);
	
	// �e�N�X�`���C���f�b�N�X�ݒ�
	std::vector<int> vecIdx = pModel->GetIdxTexture();
	std::fill(vecIdx.begin(), vecIdx.end(), idx);
	pModel->SetIdxTexture(vecIdx);
}

//==========================================================================
// �f�o�b�O����
//==========================================================================
void CDressup_Face::Debug()
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
