//=============================================================================
//
// ���͏��� [input.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _INPUT_H_	//���̃}�N����`������Ă��Ȃ�������
#define _INPUT_H_	//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#pragma comment(lib, "xinput.lib")	//�R���g���[���[�ɕK�v

//==========================================================================
// �}�N����`
//==========================================================================
#define NUM_KEY_MAX	(256)	// �L�[�̍ő吔
#define MAX_PLAYER	(1)		// �ő�v���C���[

//==========================================================================
// �N���X��`
//==========================================================================
// ���̓N���X
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static int nNumAll;					// ����
	static LPDIRECTINPUT8 m_pInput;		// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8 m_pDevice;		// ���̓f�o�C�X�ւ̃|�C���^
};

// �L�[�{�[�h�N���X
class CInputKeyboard : public CInput
{
public:

	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey, int nPattern);

private:
	int m_nPatternRepeat[NUM_KEY_MAX];		// ���s�\�g�̊Ԋu
	int m_nCntKeyRepeat[NUM_KEY_MAX];		// ���s�[�g�p�J�E���g
	BYTE m_aKeyState[NUM_KEY_MAX];			// �v���X���
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	// �g���K�[���
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];	// �����[�X���
	BYTE m_aKeyStateRepeat[NUM_KEY_MAX];	// ���s�[�g���
};

// �W���C�p�b�h�N���X
class CInputGamepad
{
public:

	// �񋓌^��`
	typedef enum
	{
		BUTTON_UP = 0,
		BUTTON_DOWN,
		BUTTON_LEFT,
		BUTTON_RIGHT,
		BUTTON_START,
		BUTTON_BACK,
		BUTTON_LSTICKPUSH,
		BUTTON_RSTICKPUSH,
		BUTTON_LB,
		BUTTON_RB,
		BUTTON_LT,
		BUTTON_RT,
		BUTTON_A,
		BUTTON_B,
		BUTTON_X,
		BUTTON_Y,
		BUTTON_LX,
		BUTTON_LY,
		BUTTON_18,
		BUTTON_19,
		BUTTON_MAX
	}BUTTON;

	typedef enum
	{
		VIBRATION_STATE_NONE = 0,	//�������ĂȂ����
		VIBRATION_STATE_DMG,		//�_���[�W���
		VIBRATION_STATE_ITEM,		//�A�C�e��

	}VIBRATION_STATE;

	CInputGamepad();
	~CInputGamepad();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(BUTTON nKey, int nCntPlayer);
	bool GetPressLTrigger(BUTTON nKey, int nCntPlayer);
	bool GetPressRTrigger(BUTTON nKey, int nCntPlayer);
	bool GetTrigger(BUTTON nKey, int nCntPlayer);
	bool GetRepeat(BUTTON nKey, int nCntPlayer);
	bool GetRelease(int nKey, int nCntPlayer);
	D3DXVECTOR3 GetStickMoveL(int nCntPlayer);
	D3DXVECTOR3 GetStickMoveR(int nCntPlayer);
	BYTE GetLeftTriggerPress(int nPlayer);
	BYTE GetRightTriggerPress(int nPlayer);
	void SetVibration(VIBRATION_STATE VibState, int nCntPlayer);
	void SetEnableStickSelect(bool bStick);
	bool GetStickSelect(void);
	void SetEnableVibration(void);
	bool GetEnableVibration(void);
	int GetnCntPad(void);

private:
	XINPUT_STATE m_aGamepadState[MAX_PLAYER];				// �v���X���
	XINPUT_STATE m_aGamepadStateTrigger[MAX_PLAYER];		// �g���K�[���
	XINPUT_STATE m_aGamepadStateRepeat[MAX_PLAYER];			// ���s�[�g���
	XINPUT_STATE m_aGamepadStateRelease[MAX_PLAYER];		// �����[�X���
	XINPUT_VIBRATION m_aGamepadStateVib[MAX_PLAYER];		// �o�C�u���[�V����
	VIBRATION_STATE m_VibrationState[MAX_PLAYER];			// �U���̎��
	int m_nCntVibration[MAX_PLAYER];						// �U���̎���
	int m_nMaxCntVibration[MAX_PLAYER];						// �U���̎���
	int m_nCntPadrepeat;									// ���s�[�g�p�J�E���g
	int m_nLeftStickCount;									// ���g���K�[�̑I���J�E���g
	bool m_bLeftStickSelect;								// ���g���K�[�̑I�𔻒�
	bool m_bVibrationUse;									// �o�C�u���g�p���邩�ǂ���
};

// �}�E�X�N���X
class CInputMouse : public CInput
{
public:

	// �񋓌^��`
	typedef enum
	{
		BUTTON_LEFT = 0,
		BUTTON_RIGHT,
		BUTTON_WHEEL,
		BUTTON_SIDE_1,
		BUTTON_SIDE_2
	}BUTTON;

	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(BUTTON nKey);
	D3DXVECTOR3 GetMouseMove(void);

private:
	DIMOUSESTATE2 m_MouseState;			//�S���͏��̕ۊ�
	DIMOUSESTATE2 m_MouseStateTrigger;	//�g���K�[���
	DIMOUSESTATE2 m_MouseStateRerease;	//�����[�X���
	DIMOUSESTATE2 m_MouseStateRepeat;	//���s�[�g���
	BYTE aOldState[8];					//�O��̓��͏���ۑ�
};


typedef enum
{
	STICK_0 = 0,
	STICK_MAX
}STICK;

#endif