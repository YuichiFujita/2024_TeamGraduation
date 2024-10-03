//=============================================================================
// 
//  �^�C�g���̃I�v�V�����I�������� [title_optionselect.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "title_optionselect.h"
#include "title_select.h"
#include "title_pressenter.h"
#include "title.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "sound.h"
#include "object2D.h"
#include "scroll.h"
#include "optionmenu.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE[] =	// �������̃e�N�X�`��
	{
		"data\\TEXTURE\\option\\selection_01.png",
		"data\\TEXTURE\\option\\selection_02.png",
		"data\\TEXTURE\\option\\selection_03.png",
	};
	const float SIZE_X = 100.0f;	// ��T�C�YX
	const float DISTANCE_Y = 150.0f;	// Y�̊Ԋu
}

namespace StateTime	// ��ԕʎ���
{
	const float FADE = 0.2f;	// �t�F�[�h
}


//==========================================================================
// �֐��|�C���^
//==========================================================================
CTitle_OptionSelect::STATE_FUNC CTitle_OptionSelect::m_StateFunc[] =
{
	&CTitle_OptionSelect::StateNone,		// �Ȃ�
	&CTitle_OptionSelect::StateScrollWait,	// �������҂�
	&CTitle_OptionSelect::StateScrollWait_Close,	// �������҂�
	&CTitle_OptionSelect::StateSelect,		// �I��
	&CTitle_OptionSelect::StateEdit,		// �ҏW
	&CTitle_OptionSelect::StateFadeIn,		// �t�F�[�h�C��
	&CTitle_OptionSelect::StateFadeOut,		// �t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitle_OptionSelect::CTitle_OptionSelect(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_state = STATE::STATE_NONE;			// ���
	m_fStateTimer = 0.0f;					// ��ԃ^�C�}�[
	m_select = Select::SELECT_KEYKONFIG;	// �I����
	m_Oldselect = Select::SELECT_KEYKONFIG;	// �O��̑I����

	memset(m_pSelect, 0, sizeof(m_pSelect));	// �I�����̃I�u�W�F�N�g
	m_pScroll = nullptr;					// ������
	m_pOptionMenu = nullptr;				// �I�v�V�������j���[

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTitle_OptionSelect::~CTitle_OptionSelect()
{

}

//==========================================================================
// ��������
//==========================================================================
CTitle_OptionSelect* CTitle_OptionSelect::Create()
{
	// �������̊m��
	CTitle_OptionSelect* pObj = DEBUG_NEW CTitle_OptionSelect();

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
HRESULT CTitle_OptionSelect::Init()
{
	
	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �ʒu�ݒ�
	SetPosition(MyLib::Vector3(200.0f, 200.0f, 0.0f));

	
	// �I��������
	CreateSelect();

	// ���
	SetState(STATE::STATE_NONE);

	return S_OK;
}

//==========================================================================
// �I��������
//==========================================================================
void CTitle_OptionSelect::CreateSelect()
{
	MyLib::Vector3 pos = GetPosition();

	// �e�N�X�`���擾
	CTexture* pTexture = CTexture::GetInstance();

	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		// ����
		m_pSelect[i] = CObject2D::Create();
		if (m_pSelect[i] == nullptr) continue;

		CObject2D* pObj2D = m_pSelect[i];

		// ��ނ̐ݒ�
		pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

		// �e�N�X�`���ݒ�
		int texID = pTexture->Regist(TEXTURE[i]);
		pObj2D->BindTexture(texID);

		// �T�C�Y�ݒ�
		D3DXVECTOR2 size = pTexture->GetImageSize(texID);

		// �c�������ɃT�C�Y�ݒ�
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, SIZE_X);
		pObj2D->SetSize(size);
		pObj2D->SetSizeOrigin(size);

		// �s�����x�ݒ�
		pObj2D->SetAlpha(0.0f);


		// �ʒu�ݒ�
		pObj2D->SetPosition(pos + MyLib::Vector3(0.0f, DISTANCE_Y * i, 0.0f));
		pObj2D->SetOriginPosition(pObj2D->GetPosition());
	}
}

//==========================================================================
// �I������
//==========================================================================
void CTitle_OptionSelect::Uninit()
{
	

	// �I������
	CObject::Release();
}

//==========================================================================
// �폜
//==========================================================================
void CTitle_OptionSelect::Kill()
{
	for (const auto& select : m_pSelect)
	{
		select->Uninit();
	}

	// �I������
	CObject::Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTitle_OptionSelect::Update()
{

	// ��ԍX�V
	UpdateState();

	if (IsDeath()) return;
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CTitle_OptionSelect::UpdateState()
{
	// ��ԃ^�C�}�[
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// �Ȃɂ��Ȃ�
//==========================================================================
void CTitle_OptionSelect::StateNone()
{
	
}

//==========================================================================
// �������҂�
//==========================================================================
void CTitle_OptionSelect::StateScrollWait()
{
	if (m_pScroll == nullptr)
	{
		m_pScroll = CScroll::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f), 0.1f, 400.0f, 1200.0f, false, true, 2);
	}

	// �X�N���[���I��
	if (m_pScroll->GetState() == CScroll::STATE::STATE_WAIT)
	{
		// �I�v�V�������j���[�؂�ւ�
		ChangeOptionMenu();

		SetState(STATE::STATE_FADEIN);
	}
}

//==========================================================================
// �������҂�(��)
//==========================================================================
void CTitle_OptionSelect::StateScrollWait_Close()
{
	if (m_pScroll == nullptr ||
		(m_pScroll != nullptr && m_pScroll->GetState() != CScroll::STATE::STATE_FADEOUT)) return;


	// ������
	if (m_pScroll != nullptr)
	{
		m_pScroll = nullptr;
	}

	// �O��̑I�����֖߂�
	m_select = m_Oldselect;

	// �^�C�g���擾
	CTitle* pTitle = CTitle::GetInstance();

	// �V�[���ݒ�
	pTitle->SetSceneType(CTitle::SCENETYPE::SCENETYPE_NONE);

	// �I�����̏�Ԑݒ�
	CTitle_Select* pSelect = pTitle->GetTitlePressEnter()->GetSelect();
	pSelect->SetState(CTitle_Select::STATE::STATE_FADEIN);

}

//==========================================================================
// �I��
//==========================================================================
void CTitle_OptionSelect::StateSelect()
{
	// ���͏��擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// ��
	if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y > 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
		pKey->GetTrigger(DIK_W))
	{
		// �p�^�[��No.���X�V
		m_Oldselect = m_select;
		m_select = static_cast<Select>((m_select + (Select::SELECT_MAX - 1)) % Select::SELECT_MAX);

		// �I�v�V�������j���[�؂�ւ�
		ChangeOptionMenu();
	}

	// ��
	if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y < 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
		pKey->GetTrigger(DIK_S))
	{
		// �p�^�[��No.���X�V
		m_Oldselect = m_select;
		m_select = static_cast<Select>(((int)m_select + 1) % Select::SELECT_MAX);

		// �I�v�V�������j���[�؂�ւ�
		ChangeOptionMenu();
	}


	// ���j���[��
	if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pPad->GetStickMoveL(0).x > 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		pKey->GetTrigger(DIK_D))
	{
		m_Oldselect = m_select;
		m_select = Select::SELECT_MAX;

		// �ҏW���
		SetState(STATE::STATE_EDIT);

		// �G�f�B�b�g
		if (m_pOptionMenu != nullptr)
		{
			m_pOptionMenu->SetState(COptionMenu::STATE::STATE_EDIT);
		}

		// SE�Đ�
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_DICTION);
	}

	// �I�����ɖ߂�ݒ�
	SetBackSelect();

	// �I�������Z�b�g
	ResetSelect();

}

//==========================================================================
// �ҏW
//==========================================================================
void CTitle_OptionSelect::StateEdit()
{
	// ���͏��擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (m_pOptionMenu != nullptr &&
		m_pOptionMenu->IsNowChange())
	{// �ύX��
		return;
	}


	// �I�����ɖ߂�ݒ�
	SetBackSelect();

	// �I������
	if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pPad->GetStickMoveL(0).x < 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		pKey->GetTrigger(DIK_A))
	{
		// �O��̑I�����֖߂�
		m_select = m_Oldselect;

		// �I�����
		SetState(STATE::STATE_SELECT);

		// �G�f�B�b�g�I��
		if (m_pOptionMenu != nullptr)
		{
			m_pOptionMenu->SetState(COptionMenu::STATE::STATE_NONE);
		}

		// SE�Đ�
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_DICTION);
	}
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CTitle_OptionSelect::StateFadeIn()
{
	// �s�����x�X�V
	float alpha = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, StateTime::FADE, m_fStateTimer);

	for (const auto& select : m_pSelect)
	{
		select->SetAlpha(alpha);
	}

	// ���Ԍo��
	if (m_fStateTimer >= StateTime::FADE)
	{
		SetState(STATE::STATE_SELECT);
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CTitle_OptionSelect::StateFadeOut()
{
	// �s�����x�X�V
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADE, m_fStateTimer);

	for (const auto& select : m_pSelect)
	{
		select->SetAlpha(alpha);
	}

	// ���Ԍo��
	if (m_fStateTimer >= StateTime::FADE)
	{
		SetState(STATE::STATE_SCROLLWAIT_CLOSE);
	}
}

//==========================================================================
// �I�������Z�b�g
//==========================================================================
void CTitle_OptionSelect::ResetSelect()
{
	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		CObject2D* pObj2D = m_pSelect[i];

		D3DXCOLOR setcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		MyLib::Vector3 setpos = pObj2D->GetOriginPosition();
		if (i != m_select)
		{// �I�����ȊO�͍�
			setcol = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
		}
		else
		{// �I�����͉��˂��o��
			setpos.x += 20.0f;
		}

		setcol.a = pObj2D->GetAlpha();
		pObj2D->SetColor(setcol);
		pObj2D->SetPosition(setpos);
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTitle_OptionSelect::Draw()
{
	
}

//==========================================================================
// �I�����ɖ߂�ݒ�
//==========================================================================
void CTitle_OptionSelect::SetBackSelect()
{
	// �L�[�R���t�B�O�擾
	CKeyConfig* pKeyConfigPad = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKeyConfigKey = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INKEY);

	if (m_pScroll == nullptr ||
		(m_pScroll != nullptr && m_pScroll->GetState() != CScroll::STATE::STATE_WAIT)) return;

	// �I������ʂ֖߂�
	if (pKeyConfigPad->GetTrigger(INGAME::ACT_BACK) ||
		pKeyConfigKey->GetTrigger(INGAME::ACT_BACK))
	{
		// ����������
		if (m_pScroll != nullptr)
		{
			m_pScroll->SetState(CScroll::STATE::STATE_CLOSE);
		}


		// �^�C�g���擾
		CTitle* pTitle = CTitle::GetInstance();
		pTitle->SetSceneType(CTitle::SCENETYPE::SCENETYPE_NONE);

		// �L�[�R���t�B�O�擾
		if (m_pOptionMenu != nullptr)
		{
			m_pOptionMenu->Kill();
			m_pOptionMenu = nullptr;
		}
		pTitle->SetSetting(nullptr);


		// �t�F�[�h�A�E�g
		SetState(STATE::STATE_FADEOUT);
	}
}

//==========================================================================
// �I�v�V�������j���[�؂�ւ�
//==========================================================================
void CTitle_OptionSelect::ChangeOptionMenu()
{
	if (m_pOptionMenu != nullptr)
	{
		m_pOptionMenu->Kill();
		m_pOptionMenu = nullptr;
	}

	// �I�𒆂̃I�v�V�����𐶐�
	m_pOptionMenu = COptionMenu::Create(m_select);
	// �T�E���h�Đ�
	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_SELECT);
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CTitle_OptionSelect::SetState(const STATE& state)
{
	m_state = state;
	m_fStateTimer = 0.0f;
}
