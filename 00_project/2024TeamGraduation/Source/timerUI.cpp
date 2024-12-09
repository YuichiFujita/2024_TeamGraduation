//============================================================
//
//	�^�C�}�[UI���� [timerUI.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "timerUI.h"
#include "timer.h"

//************************************************************
//	�e�N���X [CTimerUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTimerUI::CTimerUI() : m_pTimer(nullptr)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CTimerUI::~CTimerUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTimerUI::Init(void)
{
	// �����o�ϐ���������
	m_pTimer = nullptr;	// �^�C�}�[�Ǘ����

	// �^�C��UI�̏�����
	if (FAILED(CTimeUI::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �^�C�}�[�̐���
	m_pTimer = CTimer::Create(0.0f, 0.0f);
	if (m_pTimer == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTimerUI::Uninit(void)
{
	// �^�C�}�[�̏I��
	SAFE_UNINIT(m_pTimer);

	// �e�N���X�̏I��
	CTimeUI::Uninit();
}

//============================================================
// ���I�폜����
//============================================================
void CTimerUI::Kill()
{
	// �^�C�}�[�̓��I�폜
	SAFE_KILL(m_pTimer);

	// �I������
	Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CTimerUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �\�����Ԃ̐ݒ�
	SetTime(m_pTimer->GetTime());

	// �^�C��UI�̍X�V
	CTimeUI::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//============================================================
//	�`�揈��
//============================================================
void CTimerUI::Draw(void)
{
	// �^�C��UI�̕`��
	CTimeUI::Draw();
}

//============================================================
//	�\�����Ԃ̐ݒ菈��
//============================================================
void CTimerUI::SetTime(const float fTime)
{
	// �^�C�}�[���Ԃ̐ݒ�
	m_pTimer->SetTime(fTime);

	// �\�����Ԃ̐ݒ�
	CTimeUI::SetTime(fTime);
}

//============================================================
//	��������
//============================================================
CTimerUI *CTimerUI::Create
(
	const float fTime,				// �J�n����
	const float fLimit,				// ��������
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR2& rSizeValue,	// �����̑傫��
	const D3DXVECTOR2& rSizePart,	// ��؂�̑傫��
	const D3DXVECTOR2& rSpaceValue,	// �����̋�
	const D3DXVECTOR2& rSpacePart,	// ��؂�̋�
	const EAlignX alignX,			// ���z�u
	const EAlignY alignY,			// �c�z�u
	const D3DXVECTOR3& rRot,		// ����
	const D3DXCOLOR& rCol			// �F
)
{
	// �^�C�}�[UI�̐���
	CTimerUI *pTimerUI = DEBUG_NEW CTimerUI;
	if (pTimerUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �^�C�}�[UI�̏�����
		if (FAILED(pTimerUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �^�C�}�[UI�̔j��
			SAFE_DELETE(pTimerUI);
			return nullptr;
		}

		// �J�n���Ԃ�ݒ�
		pTimerUI->SetTime(fTime);

		// �������Ԃ�ݒ�
		pTimerUI->SetLimit(fLimit);

		// ���_�ʒu��ݒ�
		pTimerUI->SetPosition(rPos);

		// ���_������ݒ�
		pTimerUI->SetRotation(rRot);

		// �����̑傫����ݒ�
		pTimerUI->SetSizeValue(rSizeValue);

		// ��؂�̑傫����ݒ�
		pTimerUI->SetSizePart(rSizePart);

		// �����̋󔒂�ݒ�
		pTimerUI->SetSpaceValue(rSpaceValue);

		// ��؂�̋󔒂�ݒ�
		pTimerUI->SetSpacePart(rSpacePart);

		// �F��ݒ�
		pTimerUI->SetColor(rCol);

		// ���z�u��ݒ�
		pTimerUI->SetAlignX(alignX);

		// �c�z�u��ݒ�
		pTimerUI->SetAlignY(alignY);

		// �m�ۂ����A�h���X��Ԃ�
		return pTimerUI;
	}
}
