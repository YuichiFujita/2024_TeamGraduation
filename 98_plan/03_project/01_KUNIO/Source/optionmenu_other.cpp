//=============================================================================
// 
//  ���̑����j���[���� [optionmenu_other.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "optionmenu_other.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "object2D.h"
#include "selectdraw.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const MyLib::Vector3 BASEPOINT = MyLib::Vector3(850.0f, 200.0f, 0.0f);
	const float DISTANCE_Y = 120.0f;		// �I�������m�̊Ԋu
	const float MULTIPLY_SELECT = 1.25f;	// �I�����̔{��
	const std::string TEXTURE = "data\\TEXTURE\\number\\number_oradano03.png";	// �e�N�X�`���̃t�@�C��
	const std::string TEXTURE_TEXT[] =											// �e�L�X�g�����̃t�@�C��
	{
		"data\\TEXTURE\\option\\windowmode.png",
		"data\\TEXTURE\\option\\vib.png",

	};
	const std::string TEXTURE_ACTIVE[COptionMenu_Other::Select::SELECT_MAX][COptionMenu_Other::Active::MAX] =	// �A�N�e�B�u�����̃t�@�C��
	{
		{// �E�B���h�E���[�h
			"data\\TEXTURE\\option\\window_window.png",
			"data\\TEXTURE\\option\\window_full.png",
		},
		{// �o�C�u���[�V����
			"data\\TEXTURE\\option\\vib_ON.png",
			"data\\TEXTURE\\option\\vib_OFF.png",
		},
	};

	const float HEIGHT = 50.0f;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
COptionMenu_Other::COptionMenu_Other(int nPriority) : COptionMenu(nPriority)
{
	m_selectType = Select::SELECT_WINDOW;	// �I�𒆂̎��
	m_bNowChange = false;						// �ύX�̃t���O
	m_pDrawing = nullptr;					// �I�����M

	memset(m_pText, 0, sizeof(m_pText));	// �e�L�X�g
	memset(m_switchInfo, 0, sizeof(m_switchInfo));	// �؂�ւ����
	m_pDrawing = nullptr;					// �I�����M
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
COptionMenu_Other::~COptionMenu_Other()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT COptionMenu_Other::Init()
{
	// ����������
	COptionMenu::Init();

	// �؂�ւ�����
	CreateSwitch();

	// �e�L�X�g����
	CreateText();

	// �ݒ�ʒu
	MyLib::Vector3 setpos = BASEPOINT;

	// �I�����M
	m_pDrawing = CSelectDraw::Create(BASEPOINT);
	m_pDrawing->SetSizeOrigin(D3DXVECTOR2(200.0f, HEIGHT));
	m_pDrawing->SetSize(m_pDrawing->GetSizeOrigin());

	// �I�����M�̏����ʒu�ݒ�
	m_pDrawing->SetPosition(setpos);


	// �o�C�u�󋵂ɉ����ď󋵐ݒ�
	CInputGamepad* pPad = CInputGamepad::GetInstance();
	m_switchInfo[m_selectType].active = pPad->IsEnableVibration() ? Active::ON : Active::OFF;

	// �A�N�e�B�u��ԕʃe�N�X�`��
	ActiveByTexture(Select::SELECT_WINDOW, m_switchInfo[m_selectType].active);
	ActiveByTexture(Select::SELECT_VIB, m_switchInfo[m_selectType].active);


	// �I�������Z�b�g
	ResetSelect();

	return S_OK;
}

//==========================================================================
// �؂�ւ�����
//==========================================================================
void COptionMenu_Other::CreateSwitch()
{
	// �ݒ�ʒu
	MyLib::Vector3 setpos = BASEPOINT;

	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		// �A�N�e�B�u�֕ύX
		m_switchInfo[i].active = Active::ON;

		// �؂�ւ�����
		m_switchInfo[i].pSwitch = CObject2D::Create();

		// �A�h���X�n��
		CObject2D** pObj2D = &m_switchInfo[i].pSwitch;
		
		// �ʒu�ݒ�
		(*pObj2D)->SetPosition(setpos);
		(*pObj2D)->SetOriginPosition(setpos);

		// �e�N�X�`���ݒ�
		int texID = CTexture::GetInstance()->Regist(TEXTURE_TEXT[i]);
		(*pObj2D)->BindTexture(texID);

		// �c�������ɃT�C�Y�ݒ�
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, HEIGHT);
		(*pObj2D)->SetSize(size);
		(*pObj2D)->SetSizeOrigin(size);

		// �ʒu����
		setpos.y += DISTANCE_Y;
	}

}

//==========================================================================
// �e�L�X�g����
//==========================================================================
void COptionMenu_Other::CreateText()
{
	// �ݒ�ʒu
	MyLib::Vector3 setpos = BASEPOINT;
	setpos.x -= 450.0f;

	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		// �e�L�X�g����
		m_pText[i] = CObject2D::Create(GetPriority());

		CObject2D* pObj2D = m_pText[i];
		pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

		// �ʒu�ݒ�
		pObj2D->SetPosition(setpos);
		setpos.y += DISTANCE_Y;


		// �e�N�X�`���ݒ�
		int texID = CTexture::GetInstance()->Regist(TEXTURE_TEXT[i]);
		pObj2D->BindTexture(texID);

		// �c�������ɃT�C�Y�ݒ�
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, HEIGHT);

		// �T�C�Y�ݒ�
		pObj2D->SetSize(size);
		pObj2D->SetSizeOrigin(size);
	}

}

//==========================================================================
// �I������
//==========================================================================
void COptionMenu_Other::Uninit()
{
	// �I������
	COptionMenu::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void COptionMenu_Other::Kill()
{
	// �e�L�X�g
	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		if (m_pText[i] == nullptr) continue;
		m_pText[i]->Uninit();
		m_pText[i] = nullptr;
	}

	// �؂�ւ�
	for (const auto& obj : m_switchInfo)
	{
		if (obj.pSwitch == nullptr) continue;
		obj.pSwitch->Uninit();
	}
	
	// �I�����M
	if (m_pDrawing != nullptr)
	{
		m_pDrawing->Uninit();
		m_pDrawing = nullptr;
	}

	// �폜����
	COptionMenu::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void COptionMenu_Other::Update()
{
	// �X�V����
	COptionMenu::Update();
}

//==========================================================================
// �ҏW
//==========================================================================
void COptionMenu_Other::StateEdit()
{

	// ���͏��擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (!m_bNowChange)
	{// �ύX���ȊO�I�����؂�ւ�

		// �؂�ւ�����
		bool bSwitch = false;

		// ��
		if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y > 0) ||
			pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
			pKey->GetTrigger(DIK_W))
		{
			// �p�^�[��No.���X�V
			m_selectType = static_cast<Select>((m_selectType + (Select::SELECT_MAX - 1)) % Select::SELECT_MAX);
			m_pDrawing->SetState(CSelectDraw::State::STATE_DRAWING);
			bSwitch = true;

			// �T�E���h�Đ�
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
		}

		// ��
		if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y < 0) ||
			pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
			pKey->GetTrigger(DIK_S))
		{
			// �p�^�[��No.���X�V
			m_selectType = static_cast<Select>(((int)m_selectType + 1) % Select::SELECT_MAX);
			m_pDrawing->SetState(CSelectDraw::State::STATE_DRAWING);
			bSwitch = true;

			// �T�E���h�Đ�
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
		}

		if (bSwitch)
		{// �M�̈ʒu�X�V
			m_pDrawing->SetPosition(m_switchInfo[m_selectType].pSwitch->GetPosition());
		}
	}


	// �L�[�R���t�B�O�擾
	CKeyConfig* pKeyConfigPad = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKeyConfigKey = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INKEY);

	if (pKeyConfigPad->GetTrigger(INGAME::ACT_OK) ||
		pKeyConfigKey->GetTrigger(INGAME::ACT_OK))
	{
		// �ύX�̃t���O�؂�ւ�
		m_bNowChange = m_bNowChange ? false : true;

		// SE�Đ�
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_DICTION);
	}
	else if (pKeyConfigPad->GetTrigger(INGAME::ACT_BACK) ||
		pKeyConfigKey->GetTrigger(INGAME::ACT_BACK))
	{
		m_bNowChange = false;

		// SE�Đ�
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_DICTION);
	}

	// �ύX���̂ݐ؂�ւ�
	if (m_bNowChange)
	{
		if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pPad->GetStickMoveL(0).x > 0) ||
			pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
			pKey->GetTrigger(DIK_D))
		{// �؂�ւ�
			m_switchInfo[m_selectType].active = m_switchInfo[m_selectType].active ? Active::ON : Active::OFF;
			
			// SE�Đ�
			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_SELECT);

			// �E�B���h�E���[�h�ؑ�
			if (m_selectType == Select::SELECT_WINDOW)
			{
				bool bUse = (m_switchInfo[m_selectType].active == Active::ON) ? true : false;
				CManager::GetInstance()->ChangeDisplayMode(bUse);
			}
		}
		else if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pPad->GetStickMoveL(0).x < 0) ||
			pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
			pKey->GetTrigger(DIK_A))
		{// �؂�ւ�
			m_switchInfo[m_selectType].active = m_switchInfo[m_selectType].active ? Active::ON : Active::OFF;
			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_SELECT);

			// �E�B���h�E���[�h�ؑ�
			if (m_selectType == Select::SELECT_WINDOW)
			{
				bool bUse = (m_switchInfo[m_selectType].active == Active::ON) ? true : false;
				CManager::GetInstance()->ChangeDisplayMode(bUse);
			}
		}

		// �A�N�e�B�u��ԕʃe�N�X�`��
		ActiveByTexture(m_selectType, m_switchInfo[m_selectType].active);
	}

	// �o�C�u�̎�
	if (m_selectType == Select::SELECT_VIB)
	{
		bool bUse = (m_switchInfo[m_selectType].active == Active::ON) ? true : false;
		pPad->SetEnableVibration(bUse);
	}

	// �I�������Z�b�g
	ResetSelect();
}

//==========================================================================
// �I�������Z�b�g
//==========================================================================
void COptionMenu_Other::ResetSelect()
{
	// �I���󋵂ɉ������T�C�Y�ύX
	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		D3DXVECTOR2 setsize = m_switchInfo[i].pSwitch->GetSizeOrigin();
		D3DXCOLOR setcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		if (m_bNowChange &&
			i == m_selectType)
		{// �I�𒆂͊g��
			setsize *= 1.15f;
		}

		if (i != m_selectType)
		{// �I���O�͈Â�
			setcol = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
		}

		m_switchInfo[i].pSwitch->SetSize(setsize);
		m_switchInfo[i].pSwitch->SetColor(setcol);
		m_pText[i]->SetColor(setcol);	// �e�L�X�g
	}
}

//==========================================================================
// �A�N�e�B�u��ԕʃe�N�X�`��
//==========================================================================
void COptionMenu_Other::ActiveByTexture(Select _select, Active _active)
{
	// �A�h���X�n��
	CObject2D** pObj2D = &m_switchInfo[_select].pSwitch;

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE_ACTIVE[_select][_active]);
	(*pObj2D)->BindTexture(texID);

	// �c�������ɃT�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, HEIGHT);
	(*pObj2D)->SetSize(size);
	(*pObj2D)->SetSizeOrigin(size);
}
