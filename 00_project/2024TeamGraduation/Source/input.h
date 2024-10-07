//=============================================================================
//
// ���͏��� [input.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _INPUT_H_	//���̃}�N����`������Ă��Ȃ�������
#define _INPUT_H_	//��d�C���N���[�h�h�~�̃}�N�����`����

#include "constans.h"
#pragma comment(lib, "xinput.lib")	//�R���g���[���[�ɕK�v

class CInputMouse;
class CInputKeyboard;
class CInputGamepad;
class CKeyConfigManager;

//==========================================================================
// �N���X��`
//==========================================================================
// ���̓N���X
class CInput
{
public:

	// �񋓌^��`
	enum TYPE
	{
		TYPE_KEYBOARD = 0,
		TYPE_PAD,
		TYPE_MOUSE,
		TYPE_MAX
	};

	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit();
	virtual void Update();
	void Release();

	static CInput* Create(HINSTANCE hInstance, HWND hWnd);	// ��������

protected:
	static LPDIRECTINPUT8 m_pInput;		// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8 m_pDevice;		// ���̓f�o�C�X�ւ̃|�C���^

private:

	CInputMouse* m_pMouse;
	CInputKeyboard* m_pKeyboard;
	CInputGamepad* m_pGamepad;
	CKeyConfigManager* m_pKeyConfig;
	static CInput* m_pThisPtr;	// ���g�̃|�C���^
};



#endif