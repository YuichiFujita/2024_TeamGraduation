//=============================================================================
// 
//  �J�ڂȂ��t�F�[�h���� [instantfade.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "instantfade.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CInstantFade::CInstantFade()
{
	// �l�̃N���A
	m_fDuration = 0.0f;		// �t�F�[�h�܂ł̎���
	m_fTimerMove = 0.0f;	// �t�F�[�h����
	m_aObject2D = nullptr;	// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	m_state = STATE_NONE;	// ���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CInstantFade::~CInstantFade()
{

}

//==========================================================================
// ��������
//==========================================================================
CInstantFade* CInstantFade::Create()
{
	// �������̊m��
	CInstantFade* pFade = DEBUG_NEW CInstantFade;

	if (pFade != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		if (FAILED(pFade->Init()))
		{// ���s���Ă�����
			return nullptr;
		}
	}

	return pFade;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CInstantFade::Init()
{
	// ��������
	m_aObject2D = CObject2D::Create(8);
	if (m_aObject2D == nullptr)
	{// ���s���Ă�����
		return E_FAIL;
	}

	m_aObject2D->SetType(CObject::TYPE::TYPE_NONE);

	m_state = STATE_NONE;			// ���
	m_aObject2D->SetSize(D3DXVECTOR2(640.0f, 360.0f));	// �T�C�Y
	m_aObject2D->SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));	// �ʒu
	m_aObject2D->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// �F�ݒ�

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CInstantFade::Uninit()
{
	// �I������
	if (m_aObject2D != nullptr)
	{
		m_aObject2D->Uninit();
		delete m_aObject2D;
		m_aObject2D = nullptr;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CInstantFade::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_aObject2D == nullptr) return;

	// �F�擾
	float alpha = m_aObject2D->GetAlpha();

	// ���ԉ��Z
	m_fTimerMove += fDeltaTime;

	switch (m_state)
	{
	case STATE_NONE:
		break;

	case STATE_FADEOUT:

		// �s�����x����
		alpha = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, m_fDuration, m_fTimerMove);

		if (m_fTimerMove >= m_fDuration)
		{// �ڕW�܂ōs������
			alpha = 1.0f;
			m_fTimerMove = 0.0f;
			m_state = STATE_FADECOMPLETION;
		}
		break;

	case STATE_FADEIN:

		// �s�����x����
		alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, m_fDuration, m_fTimerMove);

		if (m_fTimerMove >= m_fDuration)
		{// �����ɂȂ�����
			alpha = 0.0f;
			m_fTimerMove = 0.0f;
			m_state = STATE_NONE;
		}
		break;

	case STATE_FADECOMPLETION:
		m_state = STATE_FADEIN;
		alpha = 1.0f;
		break;
	}

	// �F�ݒ�
	m_aObject2D->SetAlpha(alpha);

	// ���_���W�X�V
	m_aObject2D->SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CInstantFade::Draw()
{
	// �`�揈��
	if (m_aObject2D != nullptr)
	{
		m_aObject2D->Draw();
	}
}

//==========================================================================
// �t�F�[�h�ݒ�
//==========================================================================
void CInstantFade::SetFade(const D3DXCOLOR& FadeColor, float fDuration)
{
	if (m_state == STATE_NONE)
	{// �������Ă��Ȃ��Ƃ�

		m_fDuration = fDuration;	// �t�F�[�h�܂ł̎���
		m_fTimerMove = 0.0f;		// �t�F�[�h����
		m_state = STATE_FADEOUT;	// �t�F�[�h�A�E�g��Ԃɐݒ�

		// �F�ݒ�
		m_aObject2D->SetColor(D3DXCOLOR(FadeColor.r, FadeColor.g, FadeColor.b, 0.0f));
	}
}
