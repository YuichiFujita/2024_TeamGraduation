//=============================================================================
// 
//  �I�v�V�������j���[���� [optionmenu.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "optionmenu.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "sound.h"
#include "object2D.h"

// �h����
#include "optionmenu_sound.h"
#include "optionmenu_keyconfig.h"
#include "optionmenu_other.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	
}

namespace StateTime	// ��ԕʎ���
{
	const float FADE = 0.3f;	// �t�F�[�h
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
COptionMenu::STATE_FUNC COptionMenu::m_StateFunc[] =
{
	&COptionMenu::StateNone,	// �Ȃ�
	&COptionMenu::StateEdit,	// �ҏW
	&COptionMenu::StateFadeIn,	// �t�F�[�h�C��
	&COptionMenu::StateFadeOut,	// �t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
COptionMenu::COptionMenu(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_state = STATE::STATE_NONE;	// ���
	m_fStateTimer = 0.0f;			// ��ԃ^�C�}�[

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
COptionMenu::~COptionMenu()
{

}

//==========================================================================
// ��������
//==========================================================================
COptionMenu* COptionMenu::Create(CTitle_OptionSelect::Select selectType)
{
	// �������̊m��
	COptionMenu* pObj = nullptr;

	switch (selectType)
	{
	case CTitle_OptionSelect::SELECT_KEYKONFIG:
		pObj = DEBUG_NEW COptionMenu_Keyconfig;
		break;

	case CTitle_OptionSelect::SELECT_SOUND:
		pObj = DEBUG_NEW COptionMenu_Sound;
		break;

	case CTitle_OptionSelect::SELECT_OTHER:
		pObj = DEBUG_NEW COptionMenu_Other;
		break;

	default:
		return pObj;
		break;
	}

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT COptionMenu::Init()
{
	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// ���
	SetState(STATE::STATE_FADEIN);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void COptionMenu::Uninit()
{
	

	// �I������
	CObject::Release();
}

//==========================================================================
// �폜
//==========================================================================
void COptionMenu::Kill()
{
	
	// �I������
	CObject::Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void COptionMenu::Update()
{
	// ��ԍX�V
	UpdateState();

	if (IsDeath()) return;
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void COptionMenu::UpdateState()
{
	// ��ԃ^�C�}�[
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// �Ȃɂ��Ȃ�
//==========================================================================
void COptionMenu::StateNone()
{
	
}

//==========================================================================
// �ҏW
//==========================================================================
void COptionMenu::StateEdit()
{

}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void COptionMenu::StateFadeIn()
{
	// �s�����x�X�V
	float alpha = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, StateTime::FADE, m_fStateTimer);

	// ���Ԍo��
	if (m_fStateTimer >= StateTime::FADE)
	{
		SetState(STATE::STATE_NONE);
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void COptionMenu::StateFadeOut()
{
	// �s�����x�X�V
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADE, m_fStateTimer);

	// ���Ԍo��
	if (m_fStateTimer >= StateTime::FADE)
	{
		Kill();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void COptionMenu::Draw()
{
	
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void COptionMenu::SetState(const STATE& state)
{
	m_state = state;
	m_fStateTimer = 0.0f;
}
