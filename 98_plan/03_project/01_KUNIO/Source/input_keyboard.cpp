//=============================================================================
//
// ����(�L�[�{�[�h)���� [input_keyboard.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "input_keyboard.h"
#include "debugproc.h"
#include "calculation.h"

CInputKeyboard* CInputKeyboard::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CInputKeyboard::CInputKeyboard()
{
	for (int nCntKey = 0; nCntKey < mylib_const::NUM_KEY_MAX; nCntKey++)
	{
		m_nCntKeyRepeat[nCntKey] = 0;		// ���s�[�g�p�J�E���g
		m_nPatternRepeat[nCntKey] = 1;		// ���s�\�g�̊Ԋu
	}
	memset(m_aKeyState, 0, sizeof(m_aKeyState));				// �v���X���
	memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));	// �g���K�[���
	memset(m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));	// �����[�X���
	memset(m_aKeyStateRepeat, 0, sizeof(m_aKeyStateRepeat));	// ���s�[�g���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//==========================================================================
// ��������
//==========================================================================
CInputKeyboard* CInputKeyboard::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pThisPtr != nullptr) {
		return m_pThisPtr;
	}

	// �C���X�^���X����
	m_pThisPtr = DEBUG_NEW CInputKeyboard;
	if (m_pThisPtr != nullptr) {

		// ����������
		m_pThisPtr->Init(hInstance, hWnd);
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ����������
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CInputKeyboard::Uninit()
{
	// �I������
	CInput::Uninit();

	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CInputKeyboard::Update()
{
	BYTE aKeyState[mylib_const::NUM_KEY_MAX];	// �L�[�{�[�h�̓��͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < mylib_const::NUM_KEY_MAX; nCntKey++)
		{
			// �g���K�[����ۑ�
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			// �����[�X����ۑ�
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];

			// �L�[�{�[�h�̃v���X����ۑ�
			m_aKeyState[nCntKey] = aKeyState[nCntKey];

			if (GetPress(nCntKey))
			{
				// ���s�[�g�J�E���g���Z
				m_nCntKeyRepeat[nCntKey] = (m_nCntKeyRepeat[nCntKey] + 1) % m_nPatternRepeat[nCntKey];

				if ((m_nCntKeyRepeat[nCntKey] % m_nPatternRepeat[nCntKey]) == 0)
				{
					// �����[�X����ۑ�
					m_nCntKeyRepeat[nCntKey] = (m_nCntKeyRepeat[nCntKey] + 1) % m_nPatternRepeat[nCntKey];

					// �v���X����ۑ�
					m_aKeyStateRepeat[nCntKey] = m_aKeyState[nCntKey];
				}
				else
				{
					// ���s�[�g�Ƀ����[�X�̏��ۑ�
					m_aKeyStateRepeat[nCntKey] = m_aKeyStateRelease[nCntKey];
				}
			}
			else
			{
				// ���s�[�g�Ƀ����[�X�̏��ۑ�
				m_aKeyStateRepeat[nCntKey] = m_aKeyStateRelease[nCntKey];
			}
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
		m_pDevice->Acquire();
	}
}

//==========================================================================
// �L�[�{�[�h�̃v���X�����擾
//==========================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//==========================================================================
// �L�[�{�[�h�̃g���K�[�����擾
//==========================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==========================================================================
// �L�[�{�[�h�̃����[�X�����擾
//==========================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//==========================================================================
// �L�[�{�[�h�̃��s�[�g�����擾
//==========================================================================
bool CInputKeyboard::GetRepeat(int nKey, int nPattern)
{
	m_nPatternRepeat[nKey] = nPattern;		// ���s�\�g�̊Ԋu
	return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}