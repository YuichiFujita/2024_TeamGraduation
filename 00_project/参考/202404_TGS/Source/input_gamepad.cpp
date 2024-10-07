//=============================================================================
//
// ����(�Q�[���p�b�h)���� [input_gamepad.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "input_gamepad.h"
#include "debugproc.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int SHOT_FPS = 15;	// �e�̊Ԋu
	const int DMG_TIME = 30;	// �o�C�u�̎���
	const int WIN_TIME = 15;	// �o�C�u�̎���
	const float DEFAULT_TIME = (1.0f / 2.0f);
}
CInputGamepad* CInputGamepad::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CInputGamepad::CInputGamepad()
{
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		memset(&m_aGamepadState[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateTrigger[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRepeat[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRelease[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
		m_VibrationState[nCntPlayer] = VIBRATION_STATE_NONE;	// �U���̎��
		m_nCntVibration[nCntPlayer] = 0;						// �U���̎���
		m_nMaxCntVibration[nCntPlayer] = 0;						// �U���̎���
	}
	m_bLeftStickSelect[STICK_X] = false;	// ���g���K�[�̑I�𔻒�
	m_bLeftStickSelect[STICK_Y] = false;	// ���g���K�[�̑I�𔻒�
	m_bVibrationUse = false;				// �o�C�u���g�p���邩�ǂ���
	m_bLStickTip = false;					// ���X�e�B�b�N�̌X������

	m_nCntPadrepeat = 0;									// ���s�[�g�p�J�E���g
	m_fVibrationMulti = 0.0f;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CInputGamepad::~CInputGamepad()
{

}

//==========================================================================
// ��������
//==========================================================================
CInputGamepad* CInputGamepad::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pThisPtr != nullptr) {
		return m_pThisPtr;
	}

	// �C���X�^���X����
	m_pThisPtr = DEBUG_NEW CInputGamepad;
	if (m_pThisPtr != nullptr) {

		// ����������
		m_pThisPtr->Init(hInstance, hWnd);
	}

	return m_pThisPtr;
}

//==========================================================================
//�Q�[���p�b�h�̏���������
//==========================================================================
HRESULT CInputGamepad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ����������
	CInput::Init(hInstance, hWnd);

	//�@XInput��L����
	XInputEnable(true);

	// ���X�e�B�b�N�̏���������
	m_bLeftStickSelect[STICK_X] = false;	// ���g���K�[�̑I�𔻒�
	m_bLeftStickSelect[STICK_Y] = false;	// ���g���K�[�̑I�𔻒�
	m_bLStickTip = false;					// ���X�e�B�b�N�̌X������

	memset(m_StateLT, 0, sizeof(m_StateLT));	// LT�̔���
	memset(m_StateRT, 0, sizeof(m_StateRT));	// RT�̔���

	// �o�C�u�̏���������
	m_bVibrationUse = true;

	// �������N���A
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		m_nCntVibration[nCntPlayer] = 0;
		m_VibrationState[nCntPlayer] = VIBRATION_STATE_NONE;
		m_nMaxCntVibration[nCntPlayer] = 0;

		memset(&m_aGamepadState[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateTrigger[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRepeat[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRelease[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
		memset(&m_aUpdateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
		memset(&m_aGamepadState[nCntPlayer], 0, sizeof(XINPUT_STATE));

	}

	return S_OK;
}

//==========================================================================
//�Q�[���p�b�h�̏I������
//==========================================================================
void CInputGamepad::Uninit()
{
	// ���̓f�o�C�X����f�[�^���擾
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		// �R���g���[���[�Ƀo�C�u�̏���XINPUT�ɑ���
		m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = 0.0f;
		m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = 0.0f;
		XInputSetState(nCntPlayer, &m_aGamepadStateVib[nCntPlayer]);
	}

	// XInput�𖳌���
	XInputEnable(false);

	// �I������
	CInput::Uninit();

	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// �Q�[���p�b�h�̍X�V����
//==========================================================================
void CInputGamepad::Update()
{
	XINPUT_STATE  aGamepadState[mylib_const::MAX_PLAYER];
	int nCntPlayer;

	// ���̓f�o�C�X����f�[�^���擾
	for (nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		if (XInputGetState(nCntPlayer, &aGamepadState[nCntPlayer]) == ERROR_SUCCESS)
		{
			// �g���K�[�̔��菈��
			UpdateTriggerState(nCntPlayer, aGamepadState[nCntPlayer]);

			// �g���K�[����ۑ�
			m_aGamepadStateTrigger[nCntPlayer].Gamepad.wButtons =
				(~m_aGamepadState[nCntPlayer].Gamepad.wButtons) & aGamepadState[nCntPlayer].Gamepad.wButtons;

			// �����[�X����ۑ�
			m_aGamepadStateRelease[nCntPlayer].Gamepad.wButtons =
				(m_aGamepadState[nCntPlayer].Gamepad.wButtons ^ aGamepadState[nCntPlayer].Gamepad.wButtons) & m_aGamepadState[nCntPlayer].Gamepad.wButtons;


			// �v���X����ۑ�
			m_aGamepadState[nCntPlayer] = aGamepadState[nCntPlayer];

			if ((m_nCntPadrepeat % SHOT_FPS) == 0)
			{
				// ���s�[�g����ۑ�
				m_nCntPadrepeat = (m_nCntPadrepeat + 1) % SHOT_FPS;
				m_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons = aGamepadState[nCntPlayer].Gamepad.wButtons;
			}
			else
			{
				// ���s�[�g�Ƀ����[�X�̏��ۑ�
				m_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons =
					(m_aGamepadState[nCntPlayer].Gamepad.wButtons ^ aGamepadState[nCntPlayer].Gamepad.wButtons) & m_aGamepadState[nCntPlayer].Gamepad.wButtons;
			}
		}

		// �^�C�}�[�����Z
		m_nCntVibration[nCntPlayer]--;

		if (m_nCntVibration[nCntPlayer] < 0)
		{// �J�E���g���[���ȉ�

			// �������ĂȂ���Ԃɐݒ�
			m_VibrationState[nCntPlayer] = VIBRATION_STATE_NONE;

			// ������������
			memset(&m_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
		}

		// �U���X�V
		UpdateVibration(nCntPlayer);
	}

	// �X�e�B�b�N�̃g���K�[����X�V
	UpdateStickTrigger();
}

//==========================================================================
// �X�e�B�b�N�̃g���K�[
//==========================================================================
void CInputGamepad::UpdateStickTrigger()
{
	bool bTipX = false, bTipY = false;
	bTipX = !UtilFunc::Calculation::IsNearlyTarget(GetStickMoveL(0).x, 0.0f, 0.01f);
	bTipY = !UtilFunc::Calculation::IsNearlyTarget(GetStickMoveL(0).y, 0.0f, 0.01f);

	// ���g���K�[�̃g���K�[����OFF
	m_bLeftStickTrigger[STICK_X] = false;
	m_bLeftStickTrigger[STICK_Y] = false;

	if (!m_bLeftStickSelect[STICK_X] && bTipX)
	{
		m_bLeftStickSelect[STICK_X] = true;
		m_bLeftStickTrigger[STICK_X] = true;	// ���g���K�[�̃g���K�[����
	}
	if (!m_bLeftStickSelect[STICK_Y] && bTipY)
	{
		m_bLeftStickSelect[STICK_Y] = true;
		m_bLeftStickTrigger[STICK_Y] = true;	// ���g���K�[�̃g���K�[����
	}

	if (m_bLeftStickSelect[STICK_X] &&
		UtilFunc::Calculation::IsNearlyTarget(GetStickMoveL(0).x, 0.0f, 0.01f))
	{// �X�e�B�b�N���߂��Ă���Ƃ�
		m_bLeftStickSelect[STICK_X] = false;
	}

	if (m_bLeftStickSelect[STICK_Y] &&
		UtilFunc::Calculation::IsNearlyTarget(GetStickMoveL(0).y, 0.0f, 0.01f))
	{// �X�e�B�b�N���߂��Ă���Ƃ�
		m_bLeftStickSelect[STICK_Y] = false;
	}



	bTipX = false, bTipY = false;
	bTipX = !UtilFunc::Calculation::IsNearlyTarget(GetStickMoveR(0).x, 0.0f, 0.01f);
	bTipY = !UtilFunc::Calculation::IsNearlyTarget(GetStickMoveR(0).y, 0.0f, 0.01f);

	// ���g���K�[�̃g���K�[����OFF
	m_bRightStickTrigger[STICK_X] = false;
	m_bRightStickTrigger[STICK_Y] = false;

	if (!m_bRightStickSelect[STICK_X] && bTipX)
	{
		m_bRightStickSelect[STICK_X] = true;
		m_bRightStickTrigger[STICK_X] = true;	// ���g���K�[�̃g���K�[����
	}
	if (!m_bRightStickSelect[STICK_Y] && bTipY)
	{
		m_bRightStickSelect[STICK_Y] = true;
		m_bRightStickTrigger[STICK_Y] = true;	// ���g���K�[�̃g���K�[����
	}

	if (m_bRightStickSelect[STICK_X] &&
		UtilFunc::Calculation::IsNearlyTarget(GetStickMoveR(0).x, 0.0f, 0.01f))
	{// �X�e�B�b�N���߂��Ă���Ƃ�
		m_bRightStickSelect[STICK_X] = false;
	}

	if (m_bRightStickSelect[STICK_Y] &&
		UtilFunc::Calculation::IsNearlyTarget(GetStickMoveR(0).y, 0.0f, 0.01f))
	{// �X�e�B�b�N���߂��Ă���Ƃ�
		m_bRightStickSelect[STICK_Y] = false;
	}


	if (UtilFunc::Calculation::IsNearlyTarget(GetStickMoveL(0).x, 0.0f, 0.01f) &&
		UtilFunc::Calculation::IsNearlyTarget(GetStickMoveL(0).y, 0.0f, 0.01f))
	{
		m_bLStickTip = false;
	}
	else
	{
		m_bLStickTip = true;
	}

}

//==========================================================================
// �g���K�[�̔��菈��
//==========================================================================
void CInputGamepad::UpdateTriggerState(int nCntPlayer, XINPUT_STATE inputstate)
{
	// ���胊�Z�b�g
	m_StateLT[nCntPlayer] = sTrigger();	// LT�̔���
	m_StateRT[nCntPlayer] = sTrigger();	// RT�̔���

	// �g���K�[����ۑ�
	// ���g���K�[
	if (inputstate.Gamepad.bLeftTrigger > 0)
	{
		// �v���X����
		m_StateLT[nCntPlayer].bPress = true;

		// �g���K�[����
		if (m_aGamepadState[nCntPlayer].Gamepad.bLeftTrigger == 0)
		{// �O�񑀍�Ȃ�
			m_StateLT[nCntPlayer].bTrigger = true;
		}
	}

	// �E�g���K�[
	if (inputstate.Gamepad.bRightTrigger > 0)
	{
		// �v���X����
		m_StateRT[nCntPlayer].bPress = true;

		// �g���K�[����
		if (m_aGamepadState[nCntPlayer].Gamepad.bRightTrigger == 0)
		{// �O�񑀍�Ȃ�
			m_StateRT[nCntPlayer].bTrigger = true;
		}
	}
}

//==========================================================================
// �o�C�u�̍X�V����
//==========================================================================
void CInputGamepad::UpdateVibration(int nCntPlayer)
{
	switch (m_VibrationState[nCntPlayer])
	{
	case VIBRATION_STATE_NONE:
	{
		
	}
		break;

	case VIBRATION_STATE_AIR:
	{

	}
		break;

	case VIBRATION_STATE_DMG:
	{
		m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = m_aUpdateVib[nCntPlayer].wLeftMotorSpeed;
		m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = m_aUpdateVib[nCntPlayer].wRightMotorSpeed;
		m_aUpdateVib[nCntPlayer].wLeftMotorSpeed *= m_fVibrationMulti;
		m_aUpdateVib[nCntPlayer].wRightMotorSpeed *= m_fVibrationMulti;
	}
		break;

	default:
	{

	}
		break;
	}

	if (m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed <= 0)
	{// �X�s�[�h��0�ȉ�

		m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = 0;
	}
	else if (m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed <= 0)
	{// �X�s�[�h��0�ȉ�

		m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = 0;
	}

	// �R���g���[���[�Ƀo�C�u�̏���XINPUT�ɑ���
	m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed *= m_fVibrationMulti;
	m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed *= m_fVibrationMulti;
	XInputSetState(nCntPlayer, &m_aGamepadStateVib[nCntPlayer]);
}

//==========================================================================
// �o�C�u�̐ݒ菈��
//==========================================================================
void CInputGamepad::SetVibration(VIBRATION_STATE VibState, int nCntPlayer)
{
	if (!m_bVibrationUse) return;

	// ��Ԃ���
	m_VibrationState[nCntPlayer] = VibState;

	switch (VibState)
	{
	case VIBRATION_STATE_DMG:

		m_nCntVibration[nCntPlayer] = 15;
		m_nMaxCntVibration[nCntPlayer] = m_nCntVibration[nCntPlayer];
		m_aUpdateVib[nCntPlayer].wLeftMotorSpeed = (USHRT_MAX * 0.8f);
		m_aUpdateVib[nCntPlayer].wRightMotorSpeed = (USHRT_MAX * 0.8f);
		break;

	case VIBRATION_STATE_AIR:

		m_nCntVibration[nCntPlayer] = 10;
		m_nMaxCntVibration[nCntPlayer] = m_nCntVibration[nCntPlayer];
		m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = (USHRT_MAX * 0.3f);
		m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = (USHRT_MAX * 0.3f);
		break;
	}

	// �R���g���[���[�Ƀo�C�u�̏���XINPUT�ɑ���
	XInputSetState(nCntPlayer, &m_aGamepadStateVib[nCntPlayer]);
}

//==========================================================================
// �Q�[���p�b�h�̃v���X����
//==========================================================================
bool CInputGamepad::GetPress(BUTTON nKey, int nCntPlayer)
{
	return (m_aGamepadState[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// �Q�[���p�b�h�̃g���K�[����
//==========================================================================
bool CInputGamepad::GetTrigger(BUTTON nKey, int nCntPlayer)
{
	return (m_aGamepadStateTrigger[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// �Q�[���p�b�h�̃����[�X����
//==========================================================================
bool CInputGamepad::GetRelease(int nKey, int nCntPlayer)
{
	return (m_aGamepadStateRelease[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// �Q�[���p�b�h�̃��s�[�g����
//==========================================================================
bool CInputGamepad::GetRepeat(BUTTON nKey, int nCntPlayer)
{
	return (m_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// LT�̃v���X����
//==========================================================================
bool CInputGamepad::GetPressLT(int nCntPlayer)
{
	return m_StateLT[nCntPlayer].bPress;
}

//==========================================================================
// RT�̃v���X����
//==========================================================================
bool CInputGamepad::GetPressRT(int nCntPlayer)
{
	return m_StateRT[nCntPlayer].bPress;
}

//==========================================================================
// LT�̃g���K�[����
//==========================================================================
bool CInputGamepad::GetTriggerLT(int nCntPlayer)
{
	return m_StateLT[nCntPlayer].bTrigger;
}

//==========================================================================
// RT�̃g���K�[����
//==========================================================================
bool CInputGamepad::GetTriggerRT(int nCntPlayer)
{
	return m_StateRT[nCntPlayer].bTrigger;
}

//==========================================================================
//  L�X�e�B�b�N�̈ړ��ʎ擾
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickMoveL(int nCntPlayer)
{

	// L�X�e�B�b�N�̈ړ���
	MyLib::Vector3 StickMoveL =
		MyLib::Vector3(
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLX * 0.001f,
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLY * 0.001f,
			0.0f);

	return StickMoveL;
}

//==========================================================================
//  R�X�e�B�b�N�̈ړ��ʎ擾
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickMoveR(int nCntPlayer)
{
	// R�X�e�B�b�N�̈ړ���
	MyLib::Vector3 StickMoveR =
		MyLib::Vector3(
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRX * 0.001f,
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRY * 0.001f,
			0.0f);

	return StickMoveR;
}


//==========================================================================
//  L�X�e�B�b�N�̊����擾
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickPositionRatioL(int nCntPlayer)
{
	// L�X�e�B�b�N�̊���
	MyLib::Vector3 StickRatio =
		MyLib::Vector3(
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLX / (float)SHRT_MAX,
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLY / (float)SHRT_MAX,
			0.0f);

	return StickRatio;
}

//==========================================================================
//  R�X�e�B�b�N�̊����擾
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickPositionRatioR(int nCntPlayer)
{
	// L�X�e�B�b�N�̊���
	MyLib::Vector3 StickRatio =
		MyLib::Vector3(
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRX / (float)SHRT_MAX,
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRY / (float)SHRT_MAX,
			0.0f);

	return StickRatio;
}

//==========================================================================
// �Q�[���p�b�h�̃X�e�B�b�N�̌����擾
//==========================================================================
float CInputGamepad::GetStickRotL(int nCntPlayer)
{
	// �p�x�����߂�
	float fRot = atan2f(
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLX - 0.0f),
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLY - 0.0f));

	return fRot;
}

//==========================================================================
// �Q�[���p�b�h�̃X�e�B�b�N�̌����擾
//==========================================================================
float CInputGamepad::GetStickRotR(int nCntPlayer)
{
	// �p�x�����߂�
	float fRot = atan2f(
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRX - 0.0f),
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRY - 0.0f));

	return fRot;
}

//==========================================================================
// �X�e�B�b�N�̃g���K�[����
//==========================================================================
bool CInputGamepad::GetLStickTrigger(STICK XY)
{
	return m_bLeftStickTrigger[XY];
}

//==========================================================================
// �X�e�B�b�N�̃g���K�[����
//==========================================================================
bool CInputGamepad::GetRStickTrigger(STICK XY)
{
	return m_bRightStickTrigger[XY];
}

//==========================================================================
// �p�b�h�̃��s�[�gFPS
//==========================================================================
int CInputGamepad::GetnCntPad()
{
	return m_nCntPadrepeat;
}