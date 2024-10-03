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
// �}�N����`
//==========================================================================
#define ALPHAMOVE	(0.025f)

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CInstantFade::CInstantFade()
{
	// �l�̃N���A
	m_nDuration = 0;		// �t�F�[�h�܂ł̎���
	m_aObject2D = nullptr;		// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
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
CInstantFade *CInstantFade::Create()
{
	// �����p�̃I�u�W�F�N�g
	CInstantFade *pFade = nullptr;

	if (pFade == nullptr)
	{// nullptr��������

		// �������̊m��
		pFade = DEBUG_NEW CInstantFade;

		if (pFade != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			if (FAILED(pFade->Init()))
			{// ���s���Ă�����
				return nullptr;
			}
		}
		else
		{
			delete pFade;
			pFade = nullptr;
		}

		return pFade;
	}

	return nullptr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CInstantFade::Init()
{
	// ��������
	m_aObject2D = CObject2D::Create(8);
	m_aObject2D->SetType(CObject::TYPE::TYPE_NONE);

	if (m_aObject2D == nullptr)
	{// ���s���Ă�����
		return E_FAIL;
	}

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
	if (m_aObject2D != nullptr)
	{// nullptr����Ȃ�������

		// �I������
		m_aObject2D->Uninit();
		delete m_aObject2D;
		m_aObject2D = nullptr;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CInstantFade::Update()
{
	// �F�擾
	D3DXCOLOR col = m_aObject2D->GetColor();

	switch (m_state)
	{
	case STATE_NONE:
		break;

	case STATE_FADEOUT:

		// �s�����x����
		col.a += 1.0f / m_nDuration;

		if (col.a >= 1.0f)
		{// �ڕW�܂ōs������
			col.a = 1.0f;
			m_state = STATE_FADECOMPLETION;
		}
		break;

	case STATE_FADEIN:

		// �s�����x����
		col.a -= 1.0f / m_nDuration;

		if (col.a <= 0.0f)
		{// �����ɂȂ�����
			col.a = 0.0f;
			m_state = STATE_NONE;
		}
		break;

	case STATE_FADECOMPLETION:
		m_state = STATE_FADEIN;
		break;
	}

	// �F�ݒ�
	m_aObject2D->SetColor(col);

	// ���_���W�X�V
	m_aObject2D->SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CInstantFade::Draw()
{
	// �`�揈��
	m_aObject2D->Draw();
}

//==========================================================================
// �t�F�[�h�ݒ�
//==========================================================================
void CInstantFade::SetFade(D3DXCOLOR FadeColor, int nDuration)
{
	if (m_state == STATE_NONE)
	{// �������Ă��Ȃ��Ƃ�

		m_nDuration = nDuration;	// �t�F�[�h�܂ł̎���
		m_state = STATE_FADEOUT;	// �t�F�[�h�A�E�g��Ԃɐݒ�

		// �F�ݒ�
		m_aObject2D->SetColor(D3DXCOLOR(FadeColor.r, FadeColor.g, FadeColor.b, 0.0f));
	}
}

//==========================================================================
// ��Ԏ擾
//==========================================================================
CInstantFade::STATE CInstantFade::GetState()
{
	return m_state;
}

//==========================================================================
// �I�u�W�F�N�g2D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject2D *CInstantFade::GetMyObject()
{
	return m_aObject2D;
}