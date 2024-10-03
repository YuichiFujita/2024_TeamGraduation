//=============================================================================
// 
// �^�C�g���G���^�[���� [title_pressenter.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "title_pressenter.h"
#include "title.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
#include "input.h"
#include "fade.h"
#include "title_select.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\title\\enter.png";
	const float TIME_TUTORIAL_FADEOUT = 0.3f;	// �t�F�[�h�A�E�g
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CTitle_PressEnter::STATE_FUNC CTitle_PressEnter::m_StateFunc[] =
{
	&CTitle_PressEnter::StateNone,		// �Ȃ�
	&CTitle_PressEnter::StateFadeIn,	// �t�F�[�h�C��
	&CTitle_PressEnter::StateFadeOut,	// �t�F�[�h�A�E�g
	&CTitle_PressEnter::StateNoActive,	// �������Ȃ�
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitle_PressEnter::CTitle_PressEnter(float fadetime, int nPriority) : m_fFadeOutTime(fadetime), CObject2D(nPriority)
{
	// �l�̃N���A
	m_state = STATE::STATE_NONE;		// ���
	m_fStateTime = 0.0f;			// ��ԃJ�E���^�[
	m_pSelect = nullptr;
}

//==========================================================================
// ��������
//==========================================================================
CTitle_PressEnter* CTitle_PressEnter::Create(float fadetime)
{
	// �������̊m��
	CTitle_PressEnter* pScreen = DEBUG_NEW CTitle_PressEnter(fadetime);

	if (pScreen != nullptr)
	{
		// ����������
		pScreen->Init();
	}

	return pScreen;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTitle_PressEnter::Init()
{
	// ����������
	HRESULT hr = CObject2D::Init();
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// ��ނ̐ݒ�
	SetType(TYPE_OBJECT2D);

	// �e�N�X�`���̊��蓖��
	int nIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nIdx);

	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nIdx);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 250.0f);

	// �T�C�Y�ݒ�
	SetSize(size);
	SetSizeOrigin(GetSize());

	// �ʒu�ݒ�
	SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f));

	// �F�ݒ�
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// ��ԃJ�E���^�[
	m_fStateTime = m_fFadeOutTime;
	m_state = STATE_FADEIN;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTitle_PressEnter::Uninit()
{
	m_pSelect = nullptr;

	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTitle_PressEnter::Update()
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// �t�F�[�h���͔�����
		return;
	}

	// ��ԕʍX�V����
	(this->*(m_StateFunc[m_state]))();
	if (IsDeath())
	{
		return;
	}

	// �X�V����
	CObject2D::Update();
}

//==========================================================================
// �Ȃɂ��Ȃ�
//==========================================================================
void CTitle_PressEnter::StateNone()
{
	// ���͏��擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	

	if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_Y, 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_START, 0) ||
		pInputKeyboard->GetTrigger(DIK_RETURN) ||
		pInputKeyboard->GetTrigger(DIK_SPACE) ||
		pInputKeyboard->GetTrigger(DIK_BACKSPACE)
		)
	{
		SetState(CTitle_PressEnter::STATE::STATE_FADEOUT);

		// �I��������
		if (m_pSelect == nullptr)
		{
			m_pSelect = CTitle_Select::Create(m_fFadeOutTime);
		}
		else
		{
			m_pSelect->SetState(CTitle_Select::STATE::STATE_FADEIN);
		}
	}
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CTitle_PressEnter::StateFadeIn()
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// �s�����x�X�V
	float alpha = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, TIME_TUTORIAL_FADEOUT, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= TIME_TUTORIAL_FADEOUT)
	{
		SetState(STATE::STATE_NONE);

		// �s�����x�X�V
		SetAlpha(1.0f);
		return;
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CTitle_PressEnter::StateFadeOut()
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, TIME_TUTORIAL_FADEOUT, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= TIME_TUTORIAL_FADEOUT)
	{
		SetState(STATE::STATE_NOACTIVE);
	}
}


//==========================================================================
// �������Ȃ�
//==========================================================================
void CTitle_PressEnter::StateNoActive()
{
	if (m_pSelect->GetState() == CTitle_Select::STATE_NOACTIVE)
	{
		SetState(STATE::STATE_NONE);
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CTitle_PressEnter::SetState(STATE state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}