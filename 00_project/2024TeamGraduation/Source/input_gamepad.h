//=============================================================================
//
// ����(�Q�[���p�b�h)���� [input_gamepad.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _INPUT_GAMEPAD_H_	//���̃}�N����`������Ă��Ȃ�������
#define _INPUT_GAMEPAD_H_	//��d�C���N���[�h�h�~�̃}�N�����`����

#include "input.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �Q�[���p�b�h�N���X
class CInputGamepad : public CInput
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum BUTTON
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
	};

	// �X�e�B�b�N��
	enum STICK_AXIS
	{
		STICK_X = 0,
		STICK_Y,
		STICK_MAX
	};

	// �o�C�u���[�V����
	enum VIBRATION_STATE
	{
		VIBRATION_STATE_NONE = 0,	// �������ĂȂ����
		VIBRATION_STATE_DMG,		// �_���[�W��
		VIBRATION_STATE_MAX
	};


	//=============================
	// �\���̒�`
	//=============================
	struct STapInfo	// �^�b�v���
	{
		bool bInput;	// ���̓t���O
		bool bComplete;	// �����̏u�ԃt���O
		float fRatio;	// ����

		// �R���X�g���N�^
		STapInfo() : bInput(false), bComplete(false), fRatio(0.0f) {}
	};

	CInputGamepad();
	~CInputGamepad();

	//--------------------------
	// �p�C�v���C��
	//--------------------------
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

	//--------------------------
	// �{�^���n
	//--------------------------
	bool GetPress(BUTTON nKey, int nCntPlayer);
	bool GetTrigger(BUTTON nKey, int nCntPlayer);
	bool GetRepeat(BUTTON nKey, int nCntPlayer);
	bool GetRelease(BUTTON nKey, int nCntPlayer);
	STapInfo GetTap(BUTTON nKey, int nCntPlayer, float tapTime);

	//--------------------------
	// �g���K�[�n
	//--------------------------
	bool GetPressLT(int nCntPlayer);	// LT�̃v���X����
	bool GetPressRT(int nCntPlayer);	// RT�̃v���X����
	bool GetTriggerLT(int nCntPlayer);	// LT�̃g���K�[����
	bool GetTriggerRT(int nCntPlayer);	// RT�̃g���K�[����

	//--------------------------
	// �X�e�B�b�N�n
	//--------------------------
	bool GetLStickTrigger(int nCntPlayer, STICK_AXIS XY);	// �X�e�B�b�N�̃g���K�[����
	bool GetRStickTrigger(int nCntPlayer, STICK_AXIS XY);	// �X�e�B�b�N�̃g���K�[����
	MyLib::Vector3 GetStickMoveL(int nCntPlayer);
	MyLib::Vector3 GetStickMoveR(int nCntPlayer);
	MyLib::Vector3 GetStickPositionRatioL(int nCntPlayer);	// ���X�e�B�b�N�̊����擾
	MyLib::Vector3 GetStickPositionRatioR(int nCntPlayer);	// �E�X�e�B�b�N�̊����擾
	float GetStickRotL(int nCntPlayer);					// �X�e�B�b�N�̌����擾
	float GetStickRotR(int nCntPlayer);					// �X�e�B�b�N�̌����擾
	bool IsTipStickL(int nCntPlayer, STICK_AXIS XY) { return m_bLStickTip[nCntPlayer][XY]; }		// L�X�e�B�b�N���|��Ă邩�̔���
	bool IsTipStickR(int nCntPlayer, STICK_AXIS XY) { return m_bRStickTip[nCntPlayer][XY]; }		// R�X�e�B�b�N���|��Ă邩�̔���
	float GetVibMulti() { return m_fVibrationMulti; }
	void SetVibMulti(float fMulti) { m_fVibrationMulti = fMulti; }

	//--------------------------
	// �o�C�u���[�V�����n
	//--------------------------
	void SetEnableVibration(bool bUse) { m_bVibrationUse = bUse; }	// �o�C�u�̎g�p�󋵐؂�ւ�
	bool IsEnableVibration() { return m_bVibrationUse; }			// �o�C�u�̎g�p�󋵎擾
	void SetVibration(VIBRATION_STATE VibState, int nCntPlayer);
	int GetnCntPad();

	static CInputGamepad* GetInstance() { return m_pThisPtr; }		// �C���X�^���X�擾
	static CInputGamepad* Create(HINSTANCE hInstance, HWND hWnd);	// ��������

private:

	//=============================
	// �\���̒�`
	//=============================
	// �g���K�[
	struct sTrigger
	{
		bool bPress;	// �v���X����
		bool bTrigger;	// �g���K�[����

		sTrigger() :bPress(false), bTrigger(false) {}
	};

	//=============================
	// �����o�ϐ�
	//=============================
	void UpdateStickTrigger(int nCntPlayer);		// �X�e�B�b�N�̃g���K�[����
	void UpdateTriggerState(int nCntPlayer, XINPUT_STATE inputstate);	// �g���K�[�̔��菈��
	void UpdateVibration(int nCntPlayer);	// �U���̍X�V����
	void UpdateTapTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate, int nCntPlayer);	// �^�b�v����^�C�}�[�̍X�V

	//=============================
	// �����o�ϐ�
	//=============================
	//--------------------------
	// �@�\
	//--------------------------
	XINPUT_STATE m_aGamepadState[mylib_const::MAX_PLAYER];				// �v���X���
	XINPUT_STATE m_aGamepadStateTrigger[mylib_const::MAX_PLAYER];		// �g���K�[���
	XINPUT_STATE m_aGamepadStateRepeat[mylib_const::MAX_PLAYER];		// ���s�[�g���
	XINPUT_STATE m_aGamepadStateRelease[mylib_const::MAX_PLAYER];		// �����[�X���
	XINPUT_VIBRATION m_aGamepadStateVib[mylib_const::MAX_PLAYER];		// �o�C�u���[�V����
	XINPUT_VIBRATION m_aUpdateVib[mylib_const::MAX_PLAYER];				// �o�C�u���[�V�����X�V�p
	VIBRATION_STATE m_VibrationState[mylib_const::MAX_PLAYER];			// �U���̎��
	int m_nCntVibration[mylib_const::MAX_PLAYER];						// �U���̎���
	int m_nMaxCntVibration[mylib_const::MAX_PLAYER];					// �U���̎���
	int m_nCntPadrepeat;												// ���s�[�g�p�J�E���g
	float m_fTapTimer[BUTTON::BUTTON_MAX][mylib_const::MAX_PLAYER];		// �^�b�v����p�̃^�C�}�[
	float m_fOldTapTimer[BUTTON::BUTTON_MAX][mylib_const::MAX_PLAYER];	// �O��̃^�b�v����p�̃^�C�}�[

	//--------------------------
	// �g���K�[
	//--------------------------
	sTrigger m_StateLT[mylib_const::MAX_PLAYER];	// LT�̔���
	sTrigger m_StateRT[mylib_const::MAX_PLAYER];	// RT�̔���

	//--------------------------
	// �X�e�B�b�N
	//--------------------------
	bool m_bLStickSelect[mylib_const::MAX_PLAYER][STICK_MAX];		// ���X�e�B�b�N�̑I�𔻒�
	bool m_bLStickTrigger[mylib_const::MAX_PLAYER][STICK_MAX];		// ���X�e�B�b�N�̃g���K�[����
	bool m_bRStickTrigger[mylib_const::MAX_PLAYER][STICK_MAX];		// �E�X�e�B�b�N�̑I�𔻒�
	bool m_bRStickSelect[mylib_const::MAX_PLAYER][STICK_MAX];		// �E�X�e�B�b�N�̃g���K�[����
	bool m_bLStickTip[mylib_const::MAX_PLAYER][STICK_MAX];			// ���X�e�B�b�N�̌X������
	bool m_bRStickTip[mylib_const::MAX_PLAYER][STICK_MAX];			// �E�X�e�B�b�N�̌X������
	bool m_bVibrationUse;					// �o�C�u���g�p���邩�ǂ���
	float m_fVibrationMulti;				// �o�C�u���[�V�����{��
	float m_fDeadZone;						// �f�b�h�]�[��

	static CInputGamepad* m_pThisPtr;	// ���g�̃|�C���^
};

#endif