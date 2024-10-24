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
	const float DEADZONE = 0.8f;	// �f�b�h�]�[��
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
	m_bVibrationUse = false;				// �o�C�u���g�p���邩�ǂ���
	m_nCntPadrepeat = 0;									// ���s�[�g�p�J�E���g
	m_fVibrationMulti = 0.0f;
	m_fDeadZone = 0.0f;						// �f�b�h�]�[��
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

	// �X�e�B�b�N�̏���������
	memset(m_bLStickSelect, false, sizeof(m_bLStickSelect));	// ���g���K�[�̑I�𔻒�
	memset(m_bRStickSelect, false, sizeof(m_bRStickSelect));	// �E�g���K�[�̑I�𔻒�

	memset(m_bLStickTip, false, sizeof(m_bLStickTip));	// ���X�e�B�b�N�̌X������
	memset(m_bRStickTip, false, sizeof(m_bRStickTip));	// �E�X�e�B�b�N�̌X������

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

	// �f�b�h�]�[��
	m_fDeadZone = DEADZONE;

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

	// ���̓f�o�C�X����f�[�^���擾
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
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

		// �X�e�B�b�N�̃g���K�[����X�V
		UpdateStickTrigger(nCntPlayer);
	}
}

//==========================================================================
// �X�e�B�b�N�̃g���K�[
//==========================================================================
void CInputGamepad::UpdateStickTrigger(int nCntPlayer)
{
	
	//=============================
	// �t���O���Z�b�g
	//=============================
	//--------------------------
	// �g���K�[
	//--------------------------
	memset(m_bLStickTrigger[nCntPlayer], false, sizeof(m_bLStickTrigger[nCntPlayer]));	// ���X�e�B�b�N�̃g���K�[����
	memset(m_bRStickTrigger[nCntPlayer], false, sizeof(m_bRStickTrigger[nCntPlayer]));	// �E�X�e�B�b�N�̃g���K�[����

	//--------------------------
	// �X��
	//--------------------------
	// L�X�e�B�b�N
	m_bLStickTip[nCntPlayer][STICK_AXIS::STICK_X] = fabsf(GetStickPositionRatioL(nCntPlayer).x) > 0.0f;
	m_bLStickTip[nCntPlayer][STICK_AXIS::STICK_Y] = fabsf(GetStickPositionRatioL(nCntPlayer).y) > 0.0f;

	// R�X�e�B�b�N
	m_bRStickTip[nCntPlayer][STICK_AXIS::STICK_X] = fabsf(GetStickPositionRatioR(nCntPlayer).x) > 0.0f;
	m_bRStickTip[nCntPlayer][STICK_AXIS::STICK_Y] = fabsf(GetStickPositionRatioR(nCntPlayer).y) > 0.0f;

	for (int i = 0; i < static_cast<int>(STICK_AXIS::STICK_MAX); i++)
	{
		// ���X�e�B�b�N
		if (!m_bLStickSelect[nCntPlayer][i] && m_bLStickTip[nCntPlayer][i])
		{// �O��I�𔻒�OFF && �|����Ă�
			m_bLStickSelect[nCntPlayer][i] = true;	// �I�𔻒�
			m_bLStickTrigger[nCntPlayer][i] = true;	// �g���K�[����
		}

		// �E�X�e�B�b�N
		if (!m_bRStickSelect[nCntPlayer][i] && m_bRStickTip[nCntPlayer][i])
		{// �O��I�𔻒�OFF && �|����Ă�
			m_bRStickSelect[nCntPlayer][i] = true;	// �I�𔻒�
			m_bRStickTrigger[nCntPlayer][i] = true;	// �g���K�[����
		}


		//--------------------------
		// �߂�����
		//--------------------------
		// ���X�e�B�b�N
		if (m_bLStickSelect[nCntPlayer][i] && !m_bLStickTip[nCntPlayer][i])
		{// �X�e�B�b�N���߂��Ă���Ƃ�
			m_bLStickSelect[nCntPlayer][i] = false;
		}

		// �E�X�e�B�b�N
		if (m_bRStickSelect[nCntPlayer][i] && !m_bRStickTip[nCntPlayer][i])
		{// �X�e�B�b�N���߂��Ă���Ƃ�
			m_bRStickSelect[nCntPlayer][i] = false;
		}
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

	case VIBRATION_STATE_DMG:
	{
		// ���炵�Ă���
		m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = m_aUpdateVib[nCntPlayer].wLeftMotorSpeed;
		m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = m_aUpdateVib[nCntPlayer].wRightMotorSpeed;
		m_aUpdateVib[nCntPlayer].wLeftMotorSpeed *= m_fVibrationMulti;
		m_aUpdateVib[nCntPlayer].wRightMotorSpeed *= m_fVibrationMulti;
	}

	default:
		break;
	}

	// �␳
	UtilFunc::Transformation::Clamp(m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed, static_cast<WORD>(0.0f), static_cast<WORD>(USHRT_MAX));
	UtilFunc::Transformation::Clamp(m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed, static_cast<WORD>(0.0f), static_cast<WORD>(USHRT_MAX));

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
	return MyLib::Vector3(
		static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbLX),
		static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbLY),
		0.0f);
}

//==========================================================================
//  R�X�e�B�b�N�̈ړ��ʎ擾
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickMoveR(int nCntPlayer)
{
	// R�X�e�B�b�N�̈ړ���
	return MyLib::Vector3(
		static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbRX),
		static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbRY),
		0.0f);
}


//==========================================================================
//  L�X�e�B�b�N�̊����擾
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickPositionRatioL(int nCntPlayer)
{
	// X��
	float ratioX = static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbLX) / static_cast<float>(SHRT_MAX);
	if (fabsf(ratioX) <= m_fDeadZone)
	{
		ratioX = 0.0f;
	}

	// Y��
	float ratioY = static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbLY) / static_cast<float>(SHRT_MAX);
	if (fabsf(ratioY) <= m_fDeadZone)
	{
		ratioY = 0.0f;
	}

	return MyLib::Vector3(ratioX, ratioY, 0.0f);
}

//==========================================================================
//  R�X�e�B�b�N�̊����擾
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickPositionRatioR(int nCntPlayer)
{
	// X��
	float ratioX = static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbRX) / static_cast<float>(SHRT_MAX);
	if (fabsf(ratioX) <= m_fDeadZone)
	{
		ratioX = 0.0f;
	}

	// Y��
	float ratioY = static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbRY) / static_cast<float>(SHRT_MAX);
	if (fabsf(ratioY) <= m_fDeadZone)
	{
		ratioY = 0.0f;
	}

	return MyLib::Vector3(ratioX, ratioY, 0.0f);
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
bool CInputGamepad::GetLStickTrigger(int nCntPlayer, STICK_AXIS XY)
{
	return m_bLStickTrigger[nCntPlayer][XY];
}

//==========================================================================
// �X�e�B�b�N�̃g���K�[����
//==========================================================================
bool CInputGamepad::GetRStickTrigger(int nCntPlayer, STICK_AXIS XY)
{
	return m_bRStickTrigger[nCntPlayer][XY];
}

//==========================================================================
// �p�b�h�̃��s�[�gFPS
//==========================================================================
int CInputGamepad::GetnCntPad()
{
	return m_nCntPadrepeat;
}