//=============================================================================
//
// ����(�Q�[���p�b�h)���� [input_gamepad.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "input_gamepad.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int SHOT_FPS = 15;	// �e�̊Ԋu
	const float DEADZONE = 0.99f;	// �f�b�h�]�[��

	namespace Vib
	{
		CInputGamepad::SVib SETINFO[CInputGamepad::EVibType::VIBTYPE_MAX] =		// �ݒ肷����
		{
			CInputGamepad::SVib(0.0f, 0.00f, 0.00f, 0.00f, CInputGamepad::EEasing::Linear),		// VIBTYPE_NONE
			CInputGamepad::SVib(0.0f, 0.12f, 0.80f, 0.50f, CInputGamepad::EEasing::Linear),		// VIBTYPE_CATCH_NORMAL
			CInputGamepad::SVib(0.0f, 0.27f, 0.67f, 0.34f, CInputGamepad::EEasing::EaseInOut),	// VIBTYPE_CATCH_FAST
			CInputGamepad::SVib(0.0f, 0.15f, 0.55f, 0.76f, CInputGamepad::EEasing::EaseOut),	// VIBTYPE_THROW_NORMAL
			CInputGamepad::SVib(0.0f, 0.20f, 0.55f, 0.77f, CInputGamepad::EEasing::EaseOut),	// VIBTYPE_THROW_FAST
			CInputGamepad::SVib(0.0f, 0.18f, 1.00f, 0.85f, CInputGamepad::EEasing::EaseInOut),	// VIBTYPE_HIT
			CInputGamepad::SVib(0.0f, 0.60f, 1.00f, 0.85f, CInputGamepad::EEasing::EaseInOut),	// VIBTYPE_HIT_SP
			CInputGamepad::SVib(0.0f, 2.19f, 0.80f, 0.00f, CInputGamepad::EEasing::EaseInExpo),	// VIBTYPE_HIT_SP
		};
	}
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
// float���`�␳���X�g
CInputGamepad::FLOAT_EASING_FUNC CInputGamepad::m_FloatEasingFunc[] =
{
	&UtilFunc::Correction::EasingLinear,
	&UtilFunc::Correction::EasingQuintIn,
	&UtilFunc::Correction::EasingQuintOut,
	&UtilFunc::Correction::EasingEaseInOutQuart,
	&UtilFunc::Correction::EaseInExpo,
	&UtilFunc::Correction::EaseOutExpo,
	&UtilFunc::Correction::EaseInOutExpo,
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
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
		m_aVibType[nCntPlayer] = VIBTYPE_NONE;	// �U���̎��
		m_aVibInfo[nCntPlayer] = SVib();				// �U�����
	}
	memset(&m_fTapTimer, 0, sizeof(m_fTapTimer));		// �^�b�v����p�̃^�C�}�[
	memset(&m_fOldTapTimer, 0, sizeof(m_fOldTapTimer));	// �O��̃^�b�v����p�̃^�C�}�[

	m_bVibrationUse = false;				// �o�C�u���g�p���邩�ǂ���
	m_nCntPadrepeat = 0;					// ���s�[�g�p�J�E���g
	m_fVibrationMulti = 1.0f;				// �o�C�u���[�V�����{��
	m_fDeadZone = 0.0f;						// �f�b�h�]�[��

	// �f�o�b�O
	m_EditVibType = EVibType::VIBTYPE_NONE;	// �ҏW����o�C�u�̎��
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
		m_aVibType[nCntPlayer] = VIBTYPE_NONE;			// �o�C�u�̎��
		m_aVibInfo[nCntPlayer] = SVib();				// �U�����
		memset(&m_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));

		memset(&m_aGamepadState[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateTrigger[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRepeat[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRelease[nCntPlayer], 0, sizeof(XINPUT_STATE));
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
void CInputGamepad::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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

		// �U���X�V
		UpdateVibration(fDeltaTime, fDeltaRate, fSlowRate, nCntPlayer);

		// �X�e�B�b�N�̃g���K�[����X�V
		UpdateStickTrigger(nCntPlayer);

		// �^�b�v����^�C�}�[�̍X�V
		UpdateTapTimer(fDeltaTime, fDeltaRate, fSlowRate, nCntPlayer);
	}

#if _DEBUG

	// �f�o�b�O
	Debug();
#endif // _DEBUG

}

//==========================================================================
// �^�b�v����^�C�}�[�̍X�V
//==========================================================================
void CInputGamepad::UpdateTapTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate, int nCntPlayer)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return;

	// �^�b�v����p�̃^�C�}�[���Z
	float time = fDeltaTime * fDeltaRate * fSlowRate;
	for (int i = 0; i < static_cast<int>(BUTTON::BUTTON_MAX); i++)
	{
		// �O��̃^�b�v����p�̃^�C�}�[
		m_fOldTapTimer[i][nCntPlayer] = m_fTapTimer[i][nCntPlayer];

		if (GetPress(static_cast<BUTTON>(i), nCntPlayer))
		{
			// �^�b�v����p�̃^�C�}�[���Z
			m_fTapTimer[i][nCntPlayer] += time;
		}
		else
		{
			m_fTapTimer[i][nCntPlayer] = 0.0f;
		}
	}
}

//==========================================================================
// �X�e�B�b�N�̃g���K�[
//==========================================================================
void CInputGamepad::UpdateStickTrigger(int nCntPlayer)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return;

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
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return;

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
void CInputGamepad::UpdateVibration(const float fDeltaTime, const float fDeltaRate, const float fSlowRate, int nCntPlayer)
{
	// �o�C�u���
	SVib* pVibInfo = &m_aVibInfo[nCntPlayer];

	// �U���^�C�}�[�����Z
	pVibInfo->timer += fDeltaTime;	// �U�����

	// �U���̃p�����[�^�ݒ�
	SetVibrationParam(nCntPlayer);

	// ����
	if (pVibInfo->timer >= pVibInfo->maxTimer)
	{
		// �U���^�C�}�[���Z�b�g
		pVibInfo->timer = 0.0;

		// �������ĂȂ���Ԃɐݒ�
		SetVibration(EVibType::VIBTYPE_NONE, nCntPlayer);

		// ������������
		memset(&m_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
		XInputSetState(nCntPlayer, &m_aGamepadStateVib[nCntPlayer]);
	}
}

//==========================================================================
// �U���̃p�����[�^�ݒ�
//==========================================================================
void CInputGamepad::SetVibrationParam(int nCntPlayer)
{
	// �o�C�u���
	SVib* pVibInfo = &m_aVibInfo[nCntPlayer];

	// �C�[�W���O���
	float speed = m_FloatEasingFunc[pVibInfo->easeType](pVibInfo->startSpeed, pVibInfo->endSpeed, 0.0f, pVibInfo->maxTimer, pVibInfo->timer);
	speed *= static_cast<float>(USHRT_MAX);

	// �␳
	speed = UtilFunc::Transformation::Clamp(speed, static_cast<float>(0.0f), static_cast<float>(USHRT_MAX));
	m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = speed;
	m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = speed;

	// �R���g���[���[�Ƀo�C�u�̏���XINPUT�ɑ���
	m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed *= m_fVibrationMulti;
	m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed *= m_fVibrationMulti;
	XInputSetState(nCntPlayer, &m_aGamepadStateVib[nCntPlayer]);
}

//==========================================================================
// �o�C�u�̐ݒ菈��
//==========================================================================
void CInputGamepad::SetVibration(EVibType vibType, int nCntPlayer)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return;

	// �o�C�u�g�p���Ȃ��Ƃ��͂�߂�
	if (!m_bVibrationUse)
	{
		// �U���̃p�����[�^�ݒ�
		m_aVibInfo[nCntPlayer] = Vib::SETINFO[EVibType::VIBTYPE_NONE];
		SetVibrationParam(nCntPlayer);
		return;
	}

	// �o�C�u�̎��
	m_aVibType[nCntPlayer] = vibType;

	// ������
	m_aVibInfo[nCntPlayer].timer = 0.0f;

	// �U���̃p�����[�^�ݒ�
	m_aVibInfo[nCntPlayer] = Vib::SETINFO[vibType];
	SetVibrationParam(nCntPlayer);
}

//==========================================================================
// �v���X���������� (�{�^��)
//==========================================================================
void CInputGamepad::InitPress(int nPadIdx)
{
	// ���������N���A
	memset(&m_aGamepadState[nPadIdx].Gamepad.wButtons, 0, sizeof(m_aGamepadState[nPadIdx].Gamepad.wButtons));
}

//==========================================================================
// �g���K�[���������� (�{�^��)
//==========================================================================
void CInputGamepad::InitTrigger(int nPadIdx)
{
	// ���������N���A
	memset(&m_aGamepadStateTrigger[nPadIdx].Gamepad.wButtons, 0, sizeof(m_aGamepadStateTrigger[nPadIdx].Gamepad.wButtons));
}

//==========================================================================
// ���s�[�g���������� (�{�^��)
//==========================================================================
void CInputGamepad::InitRepeat(int nPadIdx)
{
	// ���������N���A
	memset(&m_aGamepadStateRepeat[nPadIdx].Gamepad.wButtons, 0, sizeof(m_aGamepadStateRepeat[nPadIdx].Gamepad.wButtons));
}

//==========================================================================
// �����[�X���������� (�{�^��)
//==========================================================================
void CInputGamepad::InitRelease(int nPadIdx)
{
	// ���������N���A
	memset(&m_aGamepadStateRelease[nPadIdx].Gamepad.wButtons, 0, sizeof(m_aGamepadStateRelease[nPadIdx].Gamepad.wButtons));
}

//==========================================================================
// �Q�[���p�b�h�̃v���X����
//==========================================================================
bool CInputGamepad::GetPress(BUTTON nKey, int nCntPlayer)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return (m_aGamepadState[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// �Q�[���p�b�h�̃g���K�[����
//==========================================================================
bool CInputGamepad::GetTrigger(BUTTON nKey, int nCntPlayer)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return (m_aGamepadStateTrigger[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// �Q�[���p�b�h�̃����[�X����
//==========================================================================
bool CInputGamepad::GetRelease(BUTTON nKey, int nCntPlayer)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return (m_aGamepadStateRelease[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// �Q�[���p�b�h�̃��s�[�g����
//==========================================================================
bool CInputGamepad::GetRepeat(BUTTON nKey, int nCntPlayer)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return (m_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// �^�b�v�擾
//==========================================================================
CInputGamepad::STapInfo CInputGamepad::GetTap(BUTTON nKey, int nCntPlayer, float tapTime)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return {};

	// �^�b�v���
	STapInfo returnInfo;

	// �^�b�v�̎��ԂȂ��̂͑�
	if (tapTime <= 0.0f)
	{
		returnInfo.bInput = true;
		returnInfo.bComplete = true;
		returnInfo.fRatio = 1.0f;
		return returnInfo;
	}

	// �{�^���͈͊O
	if (nKey >= BUTTON::BUTTON_MAX) return returnInfo;

	// �����ꂽ && �w��̎��ԓ�
	returnInfo.bInput = GetRelease(nKey, nCntPlayer) && m_fTapTimer[nKey][nCntPlayer] <= tapTime;

	// �^�b�v�̊���
	returnInfo.fRatio = UtilFunc::Transformation::Clamp(m_fTapTimer[nKey][nCntPlayer] / tapTime, 0.0f, 1.0f);

	if (returnInfo.fRatio >= 1.0f)
	{// ���Ԍo��

		// �ߋ��͌o�߂��Ă��Ȃ������Ƃ�
		float oldRatio = UtilFunc::Transformation::Clamp(m_fOldTapTimer[nKey][nCntPlayer] / tapTime, 0.0f, 1.0f);
		returnInfo.bComplete = oldRatio < 1.0f;
	}

	return returnInfo;
}

//==========================================================================
// �Q�[���p�b�h�̃v���X����
//==========================================================================
bool CInputGamepad::GetAllPress(BUTTON nKey)
{
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // �v���C���[�̑������J��Ԃ�

		// �Q�[���p�b�h�̃v���X
		if (GetPress(nKey, i))
		{
			// ���͏�񂠂��Ԃ�
			return true;
		}
	}

	// ���͏�񖳂���Ԃ�
	return false;
}

//==========================================================================
// �Q�[���p�b�h�̃g���K�[����
//==========================================================================
CInputGamepad::SAllTrigger CInputGamepad::GetAllTrigger(BUTTON nKey)
{
	SAllTrigger info;
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // �v���C���[�̑������J��Ԃ�

		// �Q�[���p�b�h�̃g���K�[
		if (GetTrigger(nKey, i))
		{
			// ���͏�񂠂��Ԃ�
			return SAllTrigger(true, i);
		}
	}

	// ���͏�񖳂���Ԃ�
	return info;
}

//==========================================================================
// �Q�[���p�b�h�̃��s�[�g����
//==========================================================================
bool CInputGamepad::GetAllRepeat(BUTTON nKey)
{
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // �v���C���[�̑������J��Ԃ�

		// �Q�[���p�b�h�̃��s�[�g
		if (GetRepeat(nKey, i))
		{
			// ���͏�񂠂��Ԃ�
			return true;
		}
	}

	// ���͏�񖳂���Ԃ�
	return false;
}

//==========================================================================
// �Q�[���p�b�h�̃����[�X����
//==========================================================================
bool CInputGamepad::GetAllRelease(BUTTON nKey)
{
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // �v���C���[�̑������J��Ԃ�

		// �Q�[���p�b�h�̃����[�X
		if (GetRelease(nKey, i))
		{
			// ���͏�񂠂��Ԃ�
			return true;
		}
	}

	// ���͏�񖳂���Ԃ�
	return false;
}

//==========================================================================
// LT�̃v���X����
//==========================================================================
bool CInputGamepad::GetPressLT(int nCntPlayer)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return m_StateLT[nCntPlayer].bPress;
}

//==========================================================================
// RT�̃v���X����
//==========================================================================
bool CInputGamepad::GetPressRT(int nCntPlayer)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return m_StateRT[nCntPlayer].bPress;
}

//==========================================================================
// LT�̃g���K�[����
//==========================================================================
bool CInputGamepad::GetTriggerLT(int nCntPlayer)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return m_StateLT[nCntPlayer].bTrigger;
}

//==========================================================================
// RT�̃g���K�[����
//==========================================================================
bool CInputGamepad::GetTriggerRT(int nCntPlayer)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return m_StateRT[nCntPlayer].bTrigger;
}

//==========================================================================
//  L�X�e�B�b�N�̈ړ��ʎ擾
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickMoveL(int nCntPlayer)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return VEC3_ZERO;

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
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return VEC3_ZERO;

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
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return VEC3_ZERO;

	// �f�b�h�]�[�����f��̊���
	const float shortMax = static_cast<float>(SHRT_MAX);
	float deadZoneRate = (shortMax * m_fDeadZone);

	// X��
	float thumbLX = static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbLX);
	float ratioX = 0.0f;

	if (fabs(thumbLX) > deadZoneRate)
	{// �f�b�h�]�[���O

		if (thumbLX > 0) 
		{
			// ���̒l�̏ꍇ
			ratioX = (thumbLX - deadZoneRate) / (shortMax - deadZoneRate);
		}
		else 
		{
			// ���̒l�̏ꍇ
			ratioX = (thumbLX + deadZoneRate) / (shortMax - deadZoneRate);
		}
	}

	// Y��
	float thumbLY = static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbLY);
	float ratioY = 0.0f;

	if (fabs(thumbLY) > deadZoneRate)
	{// �f�b�h�]�[���O

		if (thumbLY > 0)
		{
			// ���̒l�̏ꍇ
			ratioY = (thumbLY - deadZoneRate) / (shortMax - deadZoneRate);
		}
		else
		{
			// ���̒l�̏ꍇ
			ratioY = (thumbLY + deadZoneRate) / (shortMax - deadZoneRate);
		}
	}

	return MyLib::Vector3(ratioX, ratioY, 0.0f);
}

//==========================================================================
//  R�X�e�B�b�N�̊����擾
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickPositionRatioR(int nCntPlayer)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return VEC3_ZERO;

	// �f�b�h�]�[�����f��̊���
	const float shortMax = static_cast<float>(SHRT_MAX);
	float deadZoneRate = (shortMax * m_fDeadZone);

	// X��
	float thumbRX = static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbRX);
	float ratioX = 0.0f;

	if (fabs(thumbRX) > deadZoneRate)
	{// �f�b�h�]�[���O

		if (thumbRX > 0)
		{
			// ���̒l�̏ꍇ
			ratioX = (thumbRX - deadZoneRate) / (shortMax - deadZoneRate);
		}
		else
		{
			// ���̒l�̏ꍇ
			ratioX = (thumbRX + deadZoneRate) / (shortMax - deadZoneRate);
		}
	}

	// Y��
	float thumbRY = static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbRY);
	float ratioY = 0.0f;

	if (fabs(thumbRY) > deadZoneRate)
	{// �f�b�h�]�[���O

		if (thumbRY > 0)
		{
			// ���̒l�̏ꍇ
			ratioY = (thumbRY - deadZoneRate) / (shortMax - deadZoneRate);
		}
		else
		{
			// ���̒l�̏ꍇ
			ratioY = (thumbRY + deadZoneRate) / (shortMax - deadZoneRate);
		}
	}

	return MyLib::Vector3(ratioX, ratioY, 0.0f);
}

//==========================================================================
// �Q�[���p�b�h�̃X�e�B�b�N�̌����擾
//==========================================================================
float CInputGamepad::GetStickRotL(int nCntPlayer)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return 0.0f;

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
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return 0.0f;

	// �p�x�����߂�
	float fRot = atan2f(
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRX - 0.0f),
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRY - 0.0f));

	return fRot;
}

//==========================================================================
// L�X�e�B�b�N�̃g���K�[����
//==========================================================================
bool CInputGamepad::GetLStickTrigger(int nCntPlayer, STICK_CROSS_AXIS closs)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	switch (closs)
	{ // �\�������Ƃ̏���
	case STICK_CROSS_LEFT:
	case STICK_CROSS_RIGHT:
	{ // X���̏ꍇ

		// X�����͂��Ȃ��ꍇ������
		if (!m_bLStickTrigger[nCntPlayer][STICK_X]) return false;

		// �X�e�B�b�N�X���x�N�g���擾
		MyLib::Vector3 vec = GetStickPositionRatioL(nCntPlayer);
		if		(closs == STICK_CROSS_LEFT)	 return (vec.x <= 0.0f);
		else if	(closs == STICK_CROSS_RIGHT) return (vec.x >= 0.0f);
		break;
	}
	case STICK_CROSS_UP:
	case STICK_CROSS_DOWN:
	{ // Y���̏ꍇ

		// Y�����͂��Ȃ��ꍇ������
		if (!m_bLStickTrigger[nCntPlayer][STICK_Y]) return false;

		// �X�e�B�b�N�X���x�N�g���擾
		MyLib::Vector3 vec = GetStickPositionRatioL(nCntPlayer);
		if		(closs == STICK_CROSS_UP) 	return (vec.y >= 0.0f);
		else if	(closs == STICK_CROSS_DOWN)	return (vec.y <= 0.0f);
		break;
	}
	default:
		assert(false);
		break;
	}

	assert(false);
	return false;
}

//==========================================================================
// R�X�e�B�b�N�̃g���K�[����
//==========================================================================
bool CInputGamepad::GetRStickTrigger(int nCntPlayer, STICK_CROSS_AXIS closs)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	switch (closs)
	{ // �\�������Ƃ̏���
	case STICK_CROSS_LEFT:
	case STICK_CROSS_RIGHT:
	{ // X���̏ꍇ

		// X�����͂��Ȃ��ꍇ������
		if (!m_bRStickTrigger[nCntPlayer][STICK_X]) return false;

		// �X�e�B�b�N�X���x�N�g���擾
		MyLib::Vector3 vec = GetStickPositionRatioR(nCntPlayer);
		if		(closs == STICK_CROSS_LEFT)	 return (vec.x <= 0.0f);
		else if (closs == STICK_CROSS_RIGHT) return (vec.x >= 0.0f);
		break;
	}
	case STICK_CROSS_UP:
	case STICK_CROSS_DOWN:
	{ // Y���̏ꍇ

		// Y�����͂��Ȃ��ꍇ������
		if (!m_bRStickTrigger[nCntPlayer][STICK_Y]) return false;

		// �X�e�B�b�N�X���x�N�g���擾
		MyLib::Vector3 vec = GetStickPositionRatioR(nCntPlayer);
		if		(closs == STICK_CROSS_UP) 	return (vec.y >= 0.0f);
		else if (closs == STICK_CROSS_DOWN)	return (vec.y <= 0.0f);
		break;
	}
	default:
		assert(false);
		break;
	}

	assert(false);
	return false;
}

//==========================================================================
// L�X�e�B�b�N�̃g���K�[����
//==========================================================================
bool CInputGamepad::GetAllLStickTrigger(STICK_CROSS_AXIS closs)
{
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // �v���C���[�̑������J��Ԃ�

		if (GetLStickTrigger(i, closs))
		{
			// ���͏�񂠂��Ԃ�
			return true;
		}
	}

	// ���͏�񖳂���Ԃ�
	return false;
}

//==========================================================================
// R�X�e�B�b�N�̃g���K�[����
//==========================================================================
bool CInputGamepad::GetAllRStickTrigger(STICK_CROSS_AXIS closs)
{
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // �v���C���[�̑������J��Ԃ�

		if (GetRStickTrigger(i, closs))
		{
			// ���͏�񂠂��Ԃ�
			return true;
		}
	}

	// ���͏�񖳂���Ԃ�
	return false;
}

//==========================================================================
// �X�e�B�b�N�̃g���K�[����
//==========================================================================
bool CInputGamepad::GetLStickTrigger(int nCntPlayer, STICK_AXIS XY)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return {};

	return m_bLStickTrigger[nCntPlayer][XY];
}

//==========================================================================
// �X�e�B�b�N�̃g���K�[����
//==========================================================================
bool CInputGamepad::GetRStickTrigger(int nCntPlayer, STICK_AXIS XY)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return {};

	return m_bRStickTrigger[nCntPlayer][XY];
}

//==========================================================================
// �p�b�h�̃��s�[�gFPS
//==========================================================================
int CInputGamepad::GetnCntPad()
{
	return m_nCntPadrepeat;
}

//==========================================================================
// �f�o�b�O
//==========================================================================
void CInputGamepad::Debug()
{
	if (ImGui::TreeNode("Gamepad"))
	{
		// �N��
		if (ImGui::Button("Vib!"))
		{
			SetVibration(m_EditVibType, 0);
		}

		// �ҏW����o�C�u�̎��
		int type = (int)m_EditVibType;
		ImGui::Text(magic_enum::enum_name(m_EditVibType).data());
		ImGui::SliderInt("EditVibType", &type, 0, EVibType::VIBTYPE_MAX - 1);
		m_EditVibType = (EVibType)type;

		// ����
		ImGui::DragFloat("maxTimer", (float*)&Vib::SETINFO[m_EditVibType].maxTimer, 0.01f, 0.0f, 100.0f, "%.2f");
		ImGui::DragFloat("startSpeed", (float*)&Vib::SETINFO[m_EditVibType].startSpeed, 0.01f, 0.0f, 1.0f, "%.2f");
		ImGui::DragFloat("endSpeed", (float*)&Vib::SETINFO[m_EditVibType].endSpeed, 0.01f, 0.0f, 1.0f, "%.2f");

		// ���[�^�[�X�s�[�h
		ImGui::Text("m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed : %.2f", (float)m_aGamepadStateVib[0].wLeftMotorSpeed);

		// �C�[�W���O
		int esseType = Vib::SETINFO[m_EditVibType].easeType;
		ImGui::SliderInt("EaseType", &esseType, 0, static_cast<int>(EEasing::MAX) - 1, "%d");
		Vib::SETINFO[m_EditVibType].easeType = static_cast<EEasing>(esseType);
		ImGui::SameLine();
		ImGui::Text("[%s]", magic_enum::enum_name(Vib::SETINFO[m_EditVibType].easeType));

		ImGui::TreePop();
	}
}
