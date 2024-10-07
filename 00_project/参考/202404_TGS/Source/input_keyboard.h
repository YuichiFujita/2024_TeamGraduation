//=============================================================================
//
// ����(�L�[�{�[�h)���� [input_keyboard.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _INPUT_KEYBOARD_H_	//���̃}�N����`������Ă��Ȃ�������
#define _INPUT_KEYBOARD_H_	//��d�C���N���[�h�h�~�̃}�N�����`����

#include "input.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �L�[�{�[�h�N���X
class CInputKeyboard : public CInput
{
public:

	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey, int nPattern);

	static CInputKeyboard* GetInstance() { return m_pThisPtr; }		// �C���X�^���X�擾
	static CInputKeyboard* Create(HINSTANCE hInstance, HWND hWnd);	// ��������

private:
	int m_nPatternRepeat[mylib_const::NUM_KEY_MAX];		// ���s�\�g�̊Ԋu
	int m_nCntKeyRepeat[mylib_const::NUM_KEY_MAX];		// ���s�[�g�p�J�E���g
	BYTE m_aKeyState[mylib_const::NUM_KEY_MAX];			// �v���X���
	BYTE m_aKeyStateTrigger[mylib_const::NUM_KEY_MAX];	// �g���K�[���
	BYTE m_aKeyStateRelease[mylib_const::NUM_KEY_MAX];	// �����[�X���
	BYTE m_aKeyStateRepeat[mylib_const::NUM_KEY_MAX];	// ���s�[�g���

	static CInputKeyboard* m_pThisPtr;	// ���g�̃|�C���^
};

#endif