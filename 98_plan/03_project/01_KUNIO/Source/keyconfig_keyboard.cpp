//=============================================================================
//
// �L�[�{�[�h�L�[�R���t�B�O���� [keyconfig_keyboard.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "keyconfig_keyboard.h"
#include "input_keyboard.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CkeyConfigKeyboard::CkeyConfigKeyboard()
{
	m_Info.clear();
}

//==========================================================================
// �v���X�擾
//==========================================================================
bool CkeyConfigKeyboard::GetPress(const int type, const int nId)
{
	if (m_Info.find(type) == m_Info.end()) { return false; }	// ���݂��Ȃ�

	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	return pInputKeyboard->GetPress(m_Info[type]);
}

//==========================================================================
// �g���K�[�擾
//==========================================================================
bool CkeyConfigKeyboard::GetTrigger(const int type, const int nId)
{
	// �m�F
	if (m_Info.find(type) == m_Info.end()) { return false; }	// ���݂��Ȃ�

	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	return pInputKeyboard->GetTrigger(m_Info[type]);
}

//==========================================================================
// �����[�X�擾
//==========================================================================
bool CkeyConfigKeyboard::GetRelease(const int type, const int nId)
{
	// �m�F
	if (m_Info.find(type) == m_Info.end()) { return false; }	// ���݂��Ȃ�

	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	return pInputKeyboard->GetRelease(m_Info[type]);
}

//==========================================================================
// ���s�[�g����
//==========================================================================
bool CkeyConfigKeyboard::GetRepeat(const int type, const int nId)
{
	// �m�F
	if (m_Info.find(type) == m_Info.end()) { return false; }	// ���݂��Ȃ�

	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	return pInputKeyboard->GetRepeat(m_Info[type], 50);
}

//==========================================================================
// ��ގ擾
//==========================================================================
int CkeyConfigKeyboard::GetKey(const int type, const int nId)
{
	// �m�F
	if (m_Info.find(type) == m_Info.end()) { return 0; }	// ���݂��Ȃ�

	return m_Info[type];
}

//==========================================================================
// �I������
//==========================================================================
void CkeyConfigKeyboard::Uninit() {
	m_Info.clear();
}

//==========================================================================
// �L�[���o�^
//==========================================================================
void CkeyConfigKeyboard::Set(const int action, const int key)
{
	m_Info[action] = key;
}

//==========================================================================
// �ۑ����ǂݍ���
//==========================================================================
void CkeyConfigKeyboard::Load(const std::string& file)
{
	// �t�@�C�����J��
	std::ifstream File(file);
	if (!File.is_open()) {
		return;
	}

	// �R�����g�p
	std::string hoge;

	// �f�[�^�ǂݍ���
	std::string line;
	while (std::getline(File, line))
	{
		// �R�����g�̓X�L�b�v
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		// �X�g���[���쐬
		std::istringstream lineStream(line);

		if (line.find("KEYSET") != std::string::npos)
		{// MODELSET�Ŕz�u���ǂݍ���
			int process = -1;
			int button = -1;

			// �ǂݍ��ݏ��

			while (line.find("END_KEYSET") == std::string::npos)
			{
				std::getline(File, line);
				if (line.find("PROCESS") != std::string::npos)
				{// TYPE�Ŕz�u���̎��

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// ���n��
					lineStream >>
						hoge >>
						hoge >>	// ��
						process;	// �z�u���̎��
					continue;
				}

				if (line.find("BUTTON") != std::string::npos)
				{// POS�ňʒu

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// ���n��
					lineStream >>
						hoge >>
						hoge >>						// ��
						button;	// �ʒu
					continue;
				}
			}

			// ����
			if (process >= 0 && button >= 0)
			{
				Set(process, button);
			}
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// �t�@�C�������
	File.close();
}

//==========================================================================
// �ݒ�
//==========================================================================
void CkeyConfigKeyboard::Setting(const int type, const int nId)
{
	CInputGamepad* pKeyboard = CInputGamepad::GetInstance();

	while (1)
	{
		for (int i = 0; i < CInputGamepad::BUTTON_MAX; i++) {
			if (pKeyboard->GetTrigger(static_cast<CInputGamepad::BUTTON>(i), 0))
			{
				Set(type, static_cast<CInputGamepad::BUTTON>(i));
				return;
			}
			else if (pKeyboard->GetPressLT(0))
			{
				Set(type, CInputGamepad::BUTTON_LT);
				return;
			}
			else if (pKeyboard->GetPressRT(0))
			{
				Set(type, CInputGamepad::BUTTON_RT);
				return;
			}
		}
	}
}