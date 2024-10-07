//=============================================================================
//
// �L�[�R���t�B�O���� [keyconfig.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "keyconfig.h"
#include "keyconfig_gamepad.h"
#include "keyconfig_keyboard.h"
#include "controlkeydisp.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string CONFIGFILE[CKeyConfigManager::Control::CONTROL_MAX] =	// �t�@�C����
	{
		"data\\TEXT\\keyconfig\\gamepad_in.txt",
		"data\\TEXT\\keyconfig\\gamepad_out.txt",
		"data\\TEXT\\keyconfig\\keyboard_in.txt",
		"data\\TEXT\\keyconfig\\keyboard_out.txt",
	};
}

// �l�[��SPACE
namespace KEY_PAD
{
	const std::string FOLDER_PATH = "data\\TEXTURE\\ui_button\\";
	std::map<CInputGamepad::BUTTON, std::string> TEXTURE_PATH =
	{
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_UP,			"button_up.png"),
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_DOWN,		"button_down.png"),
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_LEFT,		"button_left.png"),
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_RIGHT,		"button_right.png"),
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_START,		"button_start.png"),
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_BACK,		"button_back.png"),
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_LSTICKPUSH,	"button_ls.png"),
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_RSTICKPUSH,	"button_rs.png"),
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_LB,			"button_lb.png"),
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_RB,			"button_rb.png"),
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_LT,			"button_lt.png"),
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_RT,			"button_rt.png"),
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_A,			"button_a.png"),
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_B,			"button_b.png"),
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_X,			"button_x.png"),
		std::pair<CInputGamepad::BUTTON,std::string>(CInputGamepad::BUTTON::BUTTON_Y,			"button_y.png"),
	};
}


//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CKeyConfigManager* CKeyConfigManager::m_pInstance = nullptr;

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CKeyConfigManager::CKeyConfigManager()
{
	// �l�̃N���A
	for (int i = 0; i < Control::CONTROL_MAX; i++) {
		m_apKeyConfig[i] = nullptr;
	}
}

//==========================================================================
// �C���X�^���X����
//==========================================================================
CKeyConfigManager* CKeyConfigManager::Create()
{
	if (m_pInstance != nullptr) { return m_pInstance; }	// �C���X�^���X������
	m_pInstance = DEBUG_NEW CKeyConfigManager;
	m_pInstance->ConfigCreate();	// �R���t�B�O����

	return m_pInstance;
}

//==========================================================================
// �I������
//==========================================================================
void CKeyConfigManager::Uninit()
{
	for (int i = 0; i < CONTROL_MAX; i++)
	{
		if (m_apKeyConfig[i] == nullptr) { continue; }	// �g�p����Ă��Ȃ�

		// �p��
		m_apKeyConfig[i]->Uninit();
		delete m_apKeyConfig[i];
		m_apKeyConfig[i] = nullptr;
	}

	if (m_pInstance == nullptr) { return; }	// �C���X�^���X�����݂��Ȃ�

	// �I������
	delete m_pInstance;
	m_pInstance = nullptr;
}

//==========================================================================
// �����ɓo�^
//==========================================================================
bool CKeyConfigManager::Bind(CKeyConfig* pConfig, const int type)
{
	// �G���[�`�F�b�N
	if (type >= Control::CONTROL_MAX || type < Control::CONTROL_INPAD) { return false; }	// �͈͊O
	if (m_apKeyConfig[type] != nullptr) { return false; }	// ���łɓo�^�ς�

	m_apKeyConfig[type] = pConfig;	// �o�^

	return true;
}

//==========================================================================
// �R���t�B�O����
//==========================================================================
void CKeyConfigManager::ConfigCreate()
{
	if (m_pInstance == nullptr) { return; }	// �C���X�^���X����

	for (int i = 0; i < Control::CONTROL_MAX; i++) {

		CKeyConfig* p = nullptr;
		switch (i) 
		{
		case Control::CONTROL_INPAD:
			p = DEBUG_NEW CkeyConfigPad;
			break;

		case Control::CONTROL_OUTPAD:
			p = DEBUG_NEW CkeyConfigPad;
			break;

		case Control::CONTROL_INKEY:
			p = DEBUG_NEW CkeyConfigKeyboard;
			break;

		case Control::CONTROL_OUTKEY:
			p = DEBUG_NEW CkeyConfigKeyboard;
			break;

		default:

			break;
		}

		if (p == nullptr) { continue; }	// �������s
		p->Load(CONFIGFILE[i]);
		int h = DIK_SPACE;
		if (!m_pInstance->Bind(p, i))	// �o�^���s
		{
			p->Uninit();
			delete p;
		}
	}
}

//==========================================================================
// �R���t�B�O�p�e�N�X�`���ǂݍ���
//==========================================================================
void CKeyConfigManager::ConfigTextureLoad()
{
	// �L�[�R���t�B�O�{�^���e�N�X�`���ǂݍ���
	for (auto itr = KEY_PAD::TEXTURE_PATH.begin(); itr != KEY_PAD::TEXTURE_PATH.end(); itr++)
	{
		int nIdx = CTexture::GetInstance()->Regist(KEY_PAD::FOLDER_PATH + (*itr).second);
		CControlKeyDisp::Load((*itr).first, nIdx);
	}
}