//=============================================================================
//
// ���͏��� [input.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "input.h"
#include "debugproc.h"
#include "manager.h"
#include "calculation.h"

#include "input_mouse.h"
#include "input_keyboard.h"
#include "input_gamepad.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;	// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
CInput* CInput::m_pThisPtr = nullptr;		// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CInput::CInput()
{
	m_pMouse = nullptr;
	m_pKeyboard = nullptr;
	m_pGamepad = nullptr;
	m_pDevice = nullptr;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CInput::~CInput()
{

}

//==========================================================================
// ��������
//==========================================================================
CInput* CInput::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pThisPtr != nullptr) {
		return m_pThisPtr;
	}

	m_pThisPtr = DEBUG_NEW CInput;
	if (m_pThisPtr == nullptr) {
		return nullptr;
	}

	// �}�E�X
	m_pThisPtr->m_pMouse = CInputMouse::Create(hInstance, hWnd);
	if (m_pThisPtr->m_pMouse == nullptr) {
		return nullptr;
	}

	// �L�[�{�[�h
	m_pThisPtr->m_pKeyboard = CInputKeyboard::Create(hInstance, hWnd);
	if (m_pThisPtr->m_pKeyboard == nullptr) {
		return nullptr;
	}

	// �Q�[���p�b�h
	m_pThisPtr->m_pGamepad = CInputGamepad::Create(hInstance, hWnd);
	if (m_pThisPtr->m_pGamepad == nullptr) {
		return nullptr;
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	if (m_pInput == nullptr)
	{// �܂��������Ă��Ȃ�������

		// DirectInput�I�u�W�F�N�g�̐���
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, nullptr);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

void CInput::Release()
{
	if (m_pMouse != nullptr) {
		m_pMouse->Uninit();
		m_pMouse = nullptr;
	}

	if (m_pKeyboard != nullptr) {
		m_pKeyboard->Uninit();
		m_pKeyboard = nullptr;
	}

	if (m_pGamepad != nullptr) {
		m_pGamepad->Uninit();
		m_pGamepad = nullptr;
	}

	// �I�u�W�F�N�g�̔j��
	if (m_pInput != nullptr) {
		m_pInput->Release();
		m_pInput = nullptr;
	}

	if (m_pThisPtr != nullptr) {
		delete m_pThisPtr;
		m_pThisPtr = nullptr;
	}
}

//==========================================================================
// �I������
//==========================================================================
void CInput::Uninit()
{
	// ���̓f�o�C�X�̔j��
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();	// �A�N�Z�X�������
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// �I�u�W�F�N�g�̔j��
	if (m_pInput != nullptr){
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CInput::Update()
{
	m_pMouse->Update();
	m_pKeyboard->Update();
	m_pGamepad->Update();
}