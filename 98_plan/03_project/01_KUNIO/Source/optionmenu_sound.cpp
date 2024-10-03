//=============================================================================
// 
//  �T�E���h���j���[���� [optionmenu_sound.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "optionmenu_sound.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "object2D.h"
#include "multinumber.h"
#include "sound.h"
#include "selectdraw.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const MyLib::Vector3 BASEPOINT = MyLib::Vector3(850.0f, 200.0f, 0.0f);
	const D3DXVECTOR2 SIZE_NUMBER = D3DXVECTOR2(40.0f, 40.0f);
	const float DISTANCE_Y = 120.0f;
	const float MULTIPLY_SELECT = 1.25f;
	const char* TEXTURE = "data\\TEXTURE\\number\\number_oradano03.png";	// �e�N�X�`���̃t�@�C��
	const char* TEXTURE_TEXT[] =
	{
		"data\\TEXTURE\\option\\mastervolume.png",
		"data\\TEXTURE\\option\\SE.png",
		"data\\TEXTURE\\option\\BGM.png",

	};// �e�N�X�`���̃t�@�C��
	const float HEIGHT = 50.0f;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
COptionMenu_Sound::COptionMenu_Sound(int nPriority) : COptionMenu(nPriority)
{
	m_selectType = Select::SELECT_MASTER;	// �I�𒆂̎��
	m_bNowChange = false;						// �ύX�̃t���O
	m_pDrawing = nullptr;		// �I�����M


	m_pNumber_Master = nullptr;			// �}�X�^�[�{�����[���̐���
	m_pNumber_SE = nullptr;				// SE�̐���
	m_pNumber_BGM = nullptr;			// BGM�̐���
	memset(m_pText, 0, sizeof(m_pText));// �e�L�X�g
	m_pDrawing = nullptr;				// �I�����M
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
COptionMenu_Sound::~COptionMenu_Sound()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT COptionMenu_Sound::Init()
{
	// ����������
	COptionMenu::Init();

	// �e�L�X�g����
	CreateText();

	// �ݒ�ʒu
	MyLib::Vector3 setpos = BASEPOINT;

	// �}�X�^�[�{�����[���̐���
	m_pNumber_Master = CMultiNumber::Create(
		setpos,
		SIZE_NUMBER,
		3,
		CNumber::EObjectType::OBJECTTYPE_2D,
		TEXTURE, true, 4);
	m_pNumber_Master->SetAlignmentType(CMultiNumber::ALIGNMENT_RIGHT);
	m_pNumber_Master->SetType(CObject::TYPE::TYPE_NUMBER);
	m_pNumber_Master->SetPosition(m_pNumber_Master->GetPosition());

	// SE�̐���
	setpos.y += DISTANCE_Y;
	m_pNumber_SE = CMultiNumber::Create(
		setpos,
		SIZE_NUMBER,
		3,
		CNumber::EObjectType::OBJECTTYPE_2D,
		TEXTURE, true, 4);
	m_pNumber_SE->SetAlignmentType(CMultiNumber::ALIGNMENT_RIGHT);
	m_pNumber_SE->SetType(CObject::TYPE::TYPE_NUMBER);
	m_pNumber_SE->SetPosition(m_pNumber_SE->GetPosition());

	// BGM�̐���
	setpos.y += DISTANCE_Y;
	m_pNumber_BGM = CMultiNumber::Create(
		setpos,
		SIZE_NUMBER,
		3,
		CNumber::EObjectType::OBJECTTYPE_2D,
		TEXTURE, true, 4);
	m_pNumber_BGM->SetAlignmentType(CMultiNumber::ALIGNMENT_RIGHT);
	m_pNumber_BGM->SetType(CObject::TYPE::TYPE_NUMBER);
	m_pNumber_BGM->SetPosition(m_pNumber_BGM->GetPosition());



	// �I�����M
	m_pDrawing = CSelectDraw::Create(BASEPOINT);
	m_pDrawing->SetSizeOrigin(D3DXVECTOR2(200.0f, HEIGHT));
	m_pDrawing->SetSize(m_pDrawing->GetSizeOrigin());


	// �T�E���h�擾
	CSound* pSound = CSound::GetInstance();

	// �}�X�^�[�{�����[��
	float masterVolume = pSound->GetVolume();

	// �e�{�����[��
	float volumeSE = pSound->GetVolume(CSound::TYPE::TYPE_SE), volumeBGM = pSound->GetVolume(CSound::TYPE::TYPE_BGM);

	// �l�̐ݒ�
	m_pNumber_Master->SetValue(pSound->GetVolumeNum() / 2);
	m_pNumber_SE->SetValue(pSound->GetVolumeNum(CSound::TYPE::TYPE_SE) / 2);
	m_pNumber_BGM->SetValue(pSound->GetVolumeNum(CSound::TYPE::TYPE_BGM) / 2);



	// �I�����M�̏����ʒu�ݒ�
	m_pDrawing->SetPosition(m_pNumber_Master->GetPosition());


	// �I�������Z�b�g
	ResetSelect();

	return S_OK;
}

//==========================================================================
// �e�L�X�g����
//==========================================================================
void COptionMenu_Sound::CreateText()
{
	// �ݒ�ʒu
	MyLib::Vector3 setpos = BASEPOINT;
	setpos.x -= 400.0f;

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
void COptionMenu_Sound::Uninit()
{
	// �I������
	COptionMenu::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void COptionMenu_Sound::Kill()
{
	// �}�X�^�[�{�����[���̐���
	if (m_pNumber_Master != nullptr)
	{
		m_pNumber_Master->Kill();
		m_pNumber_Master = nullptr;
	}

	// �}�X�^�[�{�����[���̐���
	if (m_pNumber_SE != nullptr)
	{
		m_pNumber_SE->Kill();
		m_pNumber_SE = nullptr;
	}

	// �}�X�^�[�{�����[���̐���
	if (m_pNumber_BGM != nullptr)
	{
		m_pNumber_BGM->Kill();
		m_pNumber_BGM = nullptr;
	}

	// �e�L�X�g
	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		if (m_pText[i] == nullptr) continue;
		m_pText[i]->Uninit();
		m_pText[i] = nullptr;
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
void COptionMenu_Sound::Update()
{
	// �X�V����
	COptionMenu::Update();
}

//==========================================================================
// �ҏW
//==========================================================================
void COptionMenu_Sound::StateEdit()
{
	// �T�E���h�擾
	CSound* pSound = CSound::GetInstance();

	// �}�X�^�[�{�����[��
	float masterVolume = pSound->GetVolume();

	// �e�{�����[��
	float volumeSE = pSound->GetVolume(CSound::TYPE::TYPE_SE), volumeBGM = pSound->GetVolume(CSound::TYPE::TYPE_BGM);


	// ���͏��擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (!m_bNowChange)
	{
		// �T�C�Y���Z�b�g
		{
			m_pNumber_Master->SetSize(m_pNumber_Master->GetSizeOrigin());
			m_pNumber_Master->SetKerning(m_pNumber_Master->GetSizeOrigin().y);
			m_pNumber_Master->SetPosition(m_pNumber_Master->GetPosition());

			m_pNumber_SE->SetSize(m_pNumber_SE->GetSizeOrigin());
			m_pNumber_SE->SetKerning(m_pNumber_SE->GetSizeOrigin().y);
			m_pNumber_SE->SetPosition(m_pNumber_SE->GetPosition());

			m_pNumber_BGM->SetSize(m_pNumber_BGM->GetSizeOrigin());
			m_pNumber_BGM->SetKerning(m_pNumber_BGM->GetSizeOrigin().y);
			m_pNumber_BGM->SetPosition(m_pNumber_BGM->GetPosition());
		}

		// ��
		if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y > 0) ||
			pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
			pKey->GetTrigger(DIK_W))
		{
			// �p�^�[��No.���X�V
			m_selectType = static_cast<Select>((m_selectType + (Select::SELECT_MAX - 1)) % Select::SELECT_MAX);
			m_pDrawing->SetState(CSelectDraw::State::STATE_DRAWING);

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

			// �T�E���h�Đ�
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
		}

		// �I�����ʉ��ʐݒ�
		switch (m_selectType)
		{
		case COptionMenu_Sound::SELECT_MASTER:
			m_pDrawing->SetPosition(m_pNumber_Master->GetPosition());
			break;

		case COptionMenu_Sound::SELECT_SE:
			m_pDrawing->SetPosition(m_pNumber_SE->GetPosition());
			break;

		case COptionMenu_Sound::SELECT_BGM:
			m_pDrawing->SetPosition(m_pNumber_BGM->GetPosition());
			break;

		default:
			break;
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


	// �ύX���̂݉��ʕύX
	if (m_bNowChange)
	{
		float fVolume = 1.0f;

		m_pNumber_Master->SetSize(m_pNumber_Master->GetSizeOrigin());
		m_pNumber_SE->SetSize(m_pNumber_SE->GetSizeOrigin());
		m_pNumber_BGM->SetSize(m_pNumber_BGM->GetSizeOrigin());

		// �I�����ʉ��ʎ擾
		switch (m_selectType)
		{
		case COptionMenu_Sound::SELECT_MASTER:
			fVolume = pSound->GetVolume();
			m_pNumber_Master->SetSize(m_pNumber_Master->GetSizeOrigin() * MULTIPLY_SELECT);
			m_pNumber_Master->SetKerning(m_pNumber_Master->GetSize().y);
			m_pNumber_Master->SetPosition(m_pNumber_Master->GetPosition());
			break;

		case COptionMenu_Sound::SELECT_SE:
			fVolume = pSound->GetVolume(CSound::TYPE::TYPE_SE);
			m_pNumber_SE->SetSize(m_pNumber_SE->GetSizeOrigin() * MULTIPLY_SELECT);
			m_pNumber_SE->SetKerning(m_pNumber_SE->GetSize().y);
			m_pNumber_SE->SetPosition(m_pNumber_SE->GetPosition());
			break;

		case COptionMenu_Sound::SELECT_BGM:
			fVolume = pSound->GetVolume(CSound::TYPE::TYPE_BGM);
			m_pNumber_BGM->SetSize(m_pNumber_BGM->GetSizeOrigin() * MULTIPLY_SELECT);
			m_pNumber_BGM->SetKerning(m_pNumber_BGM->GetSize().y);
			m_pNumber_BGM->SetPosition(m_pNumber_BGM->GetPosition());
			break;

		default:
			break;
		}

		if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pPad->GetStickMoveL(0).x > 0) ||
			pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
			pKey->GetTrigger(DIK_D))
		{// ����

			fVolume += 0.1f;
			pSound->PlaySound(CSound::LABEL_SE_SELECT);
		}
		else if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pPad->GetStickMoveL(0).x < 0) ||
			pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
			pKey->GetTrigger(DIK_A))
		{// ����

			fVolume -= 0.1f;
			pSound->PlaySound(CSound::LABEL_SE_SELECT);
		}

		// �I�����ʉ��ʐݒ�
		switch (m_selectType)
		{
		case COptionMenu_Sound::SELECT_MASTER:
			pSound->VolumeChange(fVolume);
			break;

		case COptionMenu_Sound::SELECT_SE:
			pSound->VolumeChange(CSound::TYPE::TYPE_SE, fVolume);
			break;

		case COptionMenu_Sound::SELECT_BGM:
			pSound->VolumeChange(CSound::TYPE::TYPE_BGM, fVolume);
			break;

		default:
			break;
		}
	}

	// �l�̐ݒ�
	m_pNumber_Master->SetValue(pSound->GetVolumeNum() / 2);
	m_pNumber_SE->SetValue(pSound->GetVolumeNum(CSound::TYPE::TYPE_SE) / 2);
	m_pNumber_BGM->SetValue(pSound->GetVolumeNum(CSound::TYPE::TYPE_BGM) / 2);

	// �I�������Z�b�g
	ResetSelect();
}


//==========================================================================
// �I�������Z�b�g
//==========================================================================
void COptionMenu_Sound::ResetSelect()
{
	// �I���󋵂ɉ������T�C�Y�ύX
	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		D3DXVECTOR2 setsize = m_pNumber_Master->GetSizeOrigin();
		D3DXCOLOR setcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		if (m_bNowChange &&
			i == m_selectType)
		{// �I�𒆂͊g��
			setsize *= MULTIPLY_SELECT;
		}

		if (i != m_selectType)
		{// �I���O�͈Â�
			setcol = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
		}


		// �I�����ʉ��ʎ擾
		switch (i)
		{
		case COptionMenu_Sound::SELECT_MASTER:
			m_pNumber_Master->SetSize(setsize);
			m_pNumber_Master->SetColor(setcol);
			break;

		case COptionMenu_Sound::SELECT_SE:
			m_pNumber_SE->SetSize(setsize);
			m_pNumber_SE->SetColor(setcol);
			break;

		case COptionMenu_Sound::SELECT_BGM:
			m_pNumber_BGM->SetSize(setsize);
			m_pNumber_BGM->SetColor(setcol);
			break;

		default:
			break;
		}

		// �e�L�X�g�̐F�ݒ�
		m_pText[i]->SetColor(setcol);
	}
}
