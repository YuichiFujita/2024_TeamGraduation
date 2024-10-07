//=============================================================================
//
// �Q�[���p�b�h�L�[�R���t�B�O���� [keyconfig_gamepad.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "keyconfig_gamepad.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CkeyConfigPad::CkeyConfigPad()
{
	m_Info.clear();
}

//==========================================================================
// �v���X�擾
//==========================================================================
bool CkeyConfigPad::GetPress(const int type, const int nId)
{
	if (m_Info.find(type) == m_Info.end()) { return false; }	// ���݂��Ȃ�

	CInputGamepad* pInputPad = CInputGamepad::GetInstance();

	if (m_Info[type] == CInputGamepad::BUTTON_LT) {
		return pInputPad->GetPressLT(0);
	}
	else if (m_Info[type] == CInputGamepad::BUTTON_RT) {
		return pInputPad->GetPressRT(0);
	}

	return pInputPad->GetPress(m_Info[type], nId);
}

//==========================================================================
// �g���K�[�擾
//==========================================================================
bool CkeyConfigPad::GetTrigger(const int type, const int nId)
{
	// �m�F
	if (m_Info.find(type) == m_Info.end()) { return false; }	// ���݂��Ȃ�

	CInputGamepad* pInputPad = CInputGamepad::GetInstance();

	if (m_Info[type] == CInputGamepad::BUTTON_LT) {
		return pInputPad->GetTriggerLT(0);
	}
	else if (m_Info[type] == CInputGamepad::BUTTON_RT) {
		return pInputPad->GetTriggerRT(0);
	}

	return pInputPad->GetTrigger(m_Info[type], nId);
}

//==========================================================================
// �����[�X�擾
//==========================================================================
bool CkeyConfigPad::GetRelease(const int type, const int nId)
{
	// �m�F
	if (m_Info.find(type) == m_Info.end()) { return false; }	// ���݂��Ȃ�

	CInputGamepad* pInputPad = CInputGamepad::GetInstance();
	return pInputPad->GetRelease(m_Info[type], nId);
}

//==========================================================================
// ���s�[�g����
//==========================================================================
bool CkeyConfigPad::GetRepeat(const int type, const int nId)
{
	// �m�F
	if (m_Info.find(type) == m_Info.end()) { return false; }	// ���݂��Ȃ�

	CInputGamepad* pInputPad = CInputGamepad::GetInstance();
	return pInputPad->GetRepeat(m_Info[type], nId);
}

//==========================================================================
// �L�[���擾
//==========================================================================
int CkeyConfigPad::GetKey(const int type, const int nId)
{
	return m_Info[type];
}

//==========================================================================
// �I������
//==========================================================================
void CkeyConfigPad::Uninit() {
	m_Info.clear();
}

//==========================================================================
// �L�[���o�^
//==========================================================================
void CkeyConfigPad::Join(const int action, const CInputGamepad::BUTTON key)
{
	m_Info[action] = key;
}

//==========================================================================
// �ۑ����ǂݍ���
//==========================================================================
void CkeyConfigPad::Load(const std::string& file)
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
				Join(process, static_cast<CInputGamepad::BUTTON>(button));
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
void CkeyConfigPad::Setting(const int type, const int nId)
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	while (1)
	{
		for (int i = 0; i < CInputGamepad::BUTTON_MAX; i++) {
			if (pPad->GetTrigger(static_cast<CInputGamepad::BUTTON>(i), 0))
			{
				Join(type, static_cast<CInputGamepad::BUTTON>(i));
				return;
			}
			else if (pPad->GetPressLT(0))
			{
				Join(type, CInputGamepad::BUTTON_LT);
				return;
			}
			else if (pPad->GetPressRT(0))
			{
				Join(type, CInputGamepad::BUTTON_RT);
				return;
			}
		}
	}
}