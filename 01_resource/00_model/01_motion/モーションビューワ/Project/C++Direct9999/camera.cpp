//=============================================================================
// 
// �J�������� [camera.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "calculation.h"
#include "player.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define MOVE			(2.5f)				// �ړ���
#define MAX_LENGTH		(20000.0f)			// �ő勗��
#define MIN_LENGTH		(10.0f)				// �ŏ�����
#define ROT_MOVE_MOUSE	(0.01f)				// ��]�ړ���
#define ROT_MOVE_STICK	(0.0015f)			// ��]�ړ���
#define ROT_MOVE		(0.025f)			// ��]�ړ���
#define MIN_ROT			(-D3DX_PI * 0.49f)	// �J�����Œ�p
#define MAX_ROT			(D3DX_PI * 0.49f)	// �J�����Œ�p
#define BACKFOLLOW_TIME	(20)				// �w�ʕ␳�܂ł̎���
#define START_CAMERALEN	(300.0f)			// ���̋���

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCamera::CCamera()
{
	m_viewport.X = 0;							// �`�悷���ʂ̍���X���W
	m_viewport.Y = 0;							// �`�悷���ʂ̍���Y���W
	m_viewport.Width = 0;						// �`�悷���ʂ̕�
	m_viewport.Height = 0;						// �`�悷���ʂ̍���
	m_viewport.Width = 0;						// �`�悷���ʂ̕�
	m_viewport.Height = 0;						// �`�悷���ʂ̍���
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 0.0f;
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����_(�������ꏊ)
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���_(�J�����̈ʒu)
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̎��_
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̒����_
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ������x�N�g��
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_rotVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�̎��_�̌���
	m_fDistance = 0.0f;							// ����
	m_nShakeLength = 0.0f;						// �h��̒���
	m_nShakeLengthY = 0.0f;						// Y�̗h��̒���
	m_fMoveShake = 0.0f;						// �h��̈ړ���
	m_fMoveShakeY = 0.0f;						// Y�̗h��̈ړ���
	m_bFollow = false;							// �Ǐ]���邩�ǂ���
	m_state = CAMERASTATE_NONE;					// ���
	m_nCntState = 0;							// ��ԃJ�E���^�[
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCamera::~CCamera()
{

}

//==================================================================================
// �J�����̏���������
//==================================================================================
HRESULT CCamera::Init(void)
{
	m_viewport.X = 0;										// �`�悷���ʂ̍���X���W
	m_viewport.Y = 0;										// �`�悷���ʂ̍���Y���W
	m_viewport.Width = SCREEN_WIDTH;						// �`�悷���ʂ̕�
	m_viewport.Height = SCREEN_HEIGHT;						// �`�悷���ʂ̍���
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	m_posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);				// �����_(�������ꏊ)
	m_posV = D3DXVECTOR3(0.0f, 300.0f, m_posR.z + -400.0f);	// ���_(�J�����̈ʒu)
	m_posVDest = m_posV;									// �ڕW�̎��_
	m_posRDest = m_posR;									// �ڕW�̒����_
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);					// ������x�N�g��
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, -0.2f);					// ����
	m_rotVDest = m_rot;										// �ڕW�̎��_�̌���
	m_fDistance = START_CAMERALEN;							// ����
	m_bFollow = false;										// �Ǐ]���邩�ǂ���
	m_state = CAMERASTATE_NONE;								// ���

	// ���_�̑������
	SetCameraV();

	return S_OK;
}

//==================================================================================
// �J�����̏I������
//==================================================================================
void CCamera::Uninit(void)
{

}

//==================================================================================
// �J�����̍X�V����
//==================================================================================
void CCamera::Update(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	//if (m_state == CAMERASTATE_NONE)
	{
		// ���_�E�����_�ړ�
		MoveCameraFollow();
		MoveCameraInput();
		MoveCameraVR();
		/*MoveCameraR();
		MoveCameraV();*/
		MoveCameraDistance();

		if (m_state == CAMERASTATE_SHAKE)
		{
			UpdateState();
		}
	}

//#ifdef _DEBUG
//#endif
}

//==================================================================================
// ���͋@��̃J�����ړ�
//==================================================================================
void CCamera::MoveCameraInput(void)
{
	// �}�E�X�ł̈ړ�����
	MoveCameraMouse();

	// �X�e�B�b�N�ł̈ړ�����
	MoveCameraStick();
}

//==================================================================================
// �R���g���[���[�ł̈ړ�����
//==================================================================================
void CCamera::MoveCameraStick(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h���擾
	CInputGamepad* pInputGamepad = CManager::GetInputGamepad();
	m_rot.y += pInputGamepad->GetStickMoveR(0).x * ROT_MOVE_STICK;
	m_rot.z += pInputGamepad->GetStickMoveR(0).y * ROT_MOVE_STICK;

	// �p�x�̐��K��
	RotNormalize(m_rot.y);
	RotNormalize(m_rot.z);

	// ���_�̑������
	SetCameraV();
}

//==================================================================================
// �}�E�X�ł̈ړ�����
//==================================================================================
void CCamera::MoveCameraMouse(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �L�[�{�[�h���擾
	CInputMouse *pInputMouse = CManager::GetInputMouse();

	if (!pInputKeyboard->GetPress(DIK_LALT) &&
		!pInputKeyboard->GetPress(DIK_RALT))
	{
		return;
	}

	if (pInputMouse->GetPress(CInputMouse::BUTTON_LEFT) == true &&
		pInputMouse->GetPress(CInputMouse::BUTTON_RIGHT) == true)
	{// ���E��������

//#if _DEBUG
		m_move.x += (pInputMouse->GetMouseMove().x * sinf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE) -
			(pInputMouse->GetMouseMove().y * cosf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE);

		m_move.z += (pInputMouse->GetMouseMove().x * cosf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE) +
			(pInputMouse->GetMouseMove().y * sinf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE);
//#endif

		// �ړ��ʕ␳
		MoveCameraVR();

		// �p�x�̐��K��
		RotNormalize(m_rot.y);
		RotNormalize(m_rot.z);

		// �����_�ݒ�
		SetCameraR();
	}
	else if (pInputMouse->GetPress(CInputMouse::BUTTON_LEFT) == true)
	{// ���N���b�N���Ă�Ƃ�,���_��]

		m_rot.y += pInputMouse->GetMouseMove().x * ROT_MOVE_MOUSE;

//#if _DEBUG

		m_rot.z += pInputMouse->GetMouseMove().y * ROT_MOVE_MOUSE;	
//#endif

		// �p�x�̐��K��
		RotNormalize(m_rot.y);
		RotNormalize(m_rot.z);

		// �l�̐��K��
		ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

		// ���_�̑������
		SetCameraV();

	}
	else if (pInputMouse->GetPress(CInputMouse::BUTTON_RIGHT) == true)
	{// �E�N���b�N���Ă�Ƃ�,�����_��]

		m_rot.y += pInputMouse->GetMouseMove().x * ROT_MOVE_MOUSE;

//#if _DEBUG
		m_rot.z += pInputMouse->GetMouseMove().y * ROT_MOVE_MOUSE;
//#endif

		// �p�x�̐��K��
		RotNormalize(m_rot.y);
		RotNormalize(m_rot.z);

		// �l�̐��K��
		ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

		// �����_�̈ʒu�X�V
		SetCameraR();
	}

	// �}�E�X�z�C�[���ŋ�������
	m_fDistance += pInputMouse->GetMouseMove().z * (MOVE * 0.3f);
	MoveCameraDistance();

	// ���_�̑������
	SetCameraV();
}

//==================================================================================
// �J�����̎��_�ړ�
//==================================================================================
void CCamera::MoveCameraV(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

//#ifdef _DEBUG
	// ���_�ړ�
	if (pInputKeyboard->GetPress(DIK_Y) == true)
	{// Y�L�[�������ꂽ,���_��ړ�

		m_rot.z -= ROT_MOVE;
	}
	else if (pInputKeyboard->GetPress(DIK_N) == true)
	{// N�L�[�������ꂽ,���_���ړ�

		m_rot.z += ROT_MOVE;
	}
//#endif

	if (pInputKeyboard->GetPress(DIK_Z) == true)
	{// Z�L�[�������ꂽ

		m_rot.y += ROT_MOVE;
	}
	if (pInputKeyboard->GetPress(DIK_C) == true)
	{// C�L�[�������ꂽ

		m_rot.y -= ROT_MOVE;
	}


	// �p�x�̐��K��
	RotNormalize(m_rot.y);
	RotNormalize(m_rot.z);

	// �l�̐��K��
	ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

	// ���_�̑������
	SetCameraV();

}

//==================================================================================
// �J�����̒����_�ړ�
//==================================================================================
void CCamera::MoveCameraR(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ����
	if (pInputKeyboard->GetPress(DIK_Q) == true)
	{// Q�L�[�������ꂽ,������


		m_rot.y -= ROT_MOVE;

	}
	else if (pInputKeyboard->GetPress(DIK_E) == true)
	{// E�L�[�������ꂽ,������

		m_rot.y += ROT_MOVE;
	}

//#ifdef _DEBUG
	// �㏸���~
	if (pInputKeyboard->GetPress(DIK_T) == true)
	{// T�L�[�������ꂽ

		m_rot.z += ROT_MOVE;

	}
	else if (pInputKeyboard->GetPress(DIK_B) == true)
	{// B�L�[�������ꂽ

		m_rot.z -= ROT_MOVE;

	}
//#endif


	// �p�x�̐��K��
	RotNormalize(m_rot.y);
	RotNormalize(m_rot.z);

	// �l�̐��K��
	ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

	// �����_�̈ʒu�X�V
	SetCameraR();
}

//==================================================================================
// �J�����̎��_�E�����_�ړ�
//==================================================================================
void CCamera::MoveCameraVR(void)
{

	// �ړ��ʕ������Z
	m_posV.x += m_move.x;
	m_posV.z += m_move.z;

	// �ړ��ʂ����Z�b�g
	m_move.x = 0.0f;
	m_move.z = 0.0f;

	// �����_�̈ʒu�X�V
	SetCameraR();
}

//==================================================================================
//  ���_�E�����_�ԋ���
//==================================================================================
void CCamera::MoveCameraDistance(void)
{

	// ���E�l�␳
	if (m_fDistance >= MAX_LENGTH)
	{
		m_fDistance = MAX_LENGTH;
	}
	else if (m_fDistance <= MIN_LENGTH)
	{
		m_fDistance = MIN_LENGTH;
	}

}

//==================================================================================
// �J�����̎��_�������
//==================================================================================
void CCamera::SetCameraV(void)
{

	if (m_bFollow == false)
	{// �Ǐ]���Ȃ��Ƃ�

		// ���_�̑������
		m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
	}
	else if(m_bFollow == true)
	{// �Ǐ]ON

		// �v���C���[�̏��擾
		CPlayer *pPlayer = CManager::GetPlayer();

		if (pPlayer == NULL)
		{
			return;
		}

		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();
		D3DXVECTOR3 PlayerRot = pPlayer->GetRotation();

		float fYcamera = 100.0f + (PlayerPos.y + 100.0f) - 100.0f;

		// ���_�̑������
		m_posVDest.x = (PlayerPos.x + sinf(D3DX_PI + PlayerRot.y) * 90.0f) + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posVDest.z = (PlayerPos.z + cosf(D3DX_PI + PlayerRot.y) * 90.0f) + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posVDest.y = fYcamera + sinf(m_rot.z) * -m_fDistance;

		// �␳����
		m_posV += (m_posVDest - m_posV) * 0.12f;
	}

}

//==================================================================================
// �J�����̒����_�������
//==================================================================================
void CCamera::SetCameraR(void)
{

	if (m_bFollow == false)
	{// �Ǐ]���Ȃ��Ƃ�

		// �����_�̑������
		m_posR.x = m_posV.x + cosf(m_rot.z) * sinf(m_rot.y) * m_fDistance;
		m_posR.z = m_posV.z + cosf(m_rot.z) * cosf(m_rot.y) * m_fDistance;
		m_posR.y = m_posV.y + sinf(m_rot.z) * m_fDistance;
	}
	else
	{// �Ǐ]ON

		// �����_�̑������
		m_posRDest.x = m_posV.x + cosf(m_rot.z) * sinf(m_rot.y) * m_fDistance;
		m_posRDest.z = m_posV.z + cosf(m_rot.z) * cosf(m_rot.y) * m_fDistance;
		m_posRDest.y = m_posV.y + sinf(m_rot.z) * m_fDistance;

		// �␳����
		m_posR += (m_posRDest - m_posR) * 0.08f;
	}

}

//==================================================================================
// �J�����̔w�ʎ����Ǐ]
//==================================================================================
void CCamera::MoveCameraFollow(void)
{
	//float fRotDiff = 0.0f;	// ����

	//// �v���C���[�̏��擾
	//Player *pPlayer = GetPlayer();

	//if (m_bBackFollow == true)
	//{// �Ǐ]����Ƃ�

	//	m_rotVDest.y = (D3DX_PI + pPlayer->rot.y);

	//	if (m_bSPush == false)
	//	{// S�n��������ĂȂ�

	//		m_nCntBackFollow = BACKFOLLOW_TIME;
	//	}
	//	else
	//	{// S�n��������Ă�Ƃ�

	//		// m_nCntBackFollow++;	// �J�E���g���Z
	//	}
	//	

	//	if (m_nCntBackFollow >= BACKFOLLOW_TIME)
	//	{// �w�ʕ␳�̃J�E���g���K��l��������

	//		m_nCntBackFollow = 0;

	//		// �␳����
	//		fRotDiff = (m_rotVDest.y - m_rot.y);

	//		// �p�x�̐��K��
	//		RotNormalize(m_rot.y);
	//		RotNormalize(m_rot.z);

	//		if (fRotDiff > D3DX_PI)
	//		{
	//			fRotDiff += (-D3DX_PI * 2.0f);
	//		}
	//		else if (fRotDiff < -D3DX_PI)
	//		{
	//			fRotDiff += (D3DX_PI * 2.0f);
	//		}

	//		// �����ŕ␳����
	//		m_rot.y += fRotDiff * 0.015f;

	//		// �p�x�̐��K��
	//		RotNormalize(m_rot.y);
	//		RotNormalize(m_rot.z);
	//	}
	//}
}

//==================================================================================
// �J�����̐ݒ菈��
//==================================================================================
void CCamera::SetCamera(void)
{

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&m_viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(45.0f),	// ����p
								(float)m_viewport.Width / (float)m_viewport.Height,	// �A�X�y�N�g��
								10.0f,		// ���s���̐���
								20000.0f);	// ���s���̐���

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,	// ���_
		&m_posR,	// �����_
		&m_vecU);	// ������x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

}


//==================================================================================
// �J�����̐U���ݒ菈��
//==================================================================================
void CCamera::SetShake(int nTime, float fLength, float fLengthY)
{
	// �U����Ԃɐݒ�
	m_state = CAMERASTATE_SHAKE;
	m_nCntState = nTime;		// ��ԑJ�ڃJ�E���^�[
	m_nShakeLength = fLength;	// �h��̑傫��
	m_nShakeLengthY = fLengthY;	// Y�̗h��̑傫��
}

//==================================================================================
// �J�����̐U������
//==================================================================================
void CCamera::Shake(void)
{
	// �����擾
	int nLength = (int)m_nShakeLength;
	int nLengthY = (int)m_nShakeLengthY;

	m_fMoveShake = (float)Random(-nLength, nLength) * sinf((float)Random(-314, 314) * 0.01f);						// �h��̈ړ���
	m_fMoveShakeY = (float)Random(-nLengthY, nLengthY);						// Y�̗h��̈ړ���

	// ���_�̑������
	m_posV.x += (float)Random(-nLength, nLength) * sinf((float)Random(-314, 314) * 0.01f);
	m_posV.y += (float)Random(-nLengthY, nLengthY);
	m_posV.z += (float)Random(-nLength, nLength) * cosf((float)Random(-314, 314) * 0.01f);

	// �����_�̑������
	m_posR.x += (float)Random(-nLength, nLength) * sinf((float)Random(-314, 314) * 0.01f);
	m_posR.y += (float)Random(-nLengthY, nLengthY);
	m_posR.z += (float)Random(-nLength, nLength) * cosf((float)Random(-314, 314) * 0.01f);

	// �����␳
	InertiaCorrection(m_nShakeLength, 0.0f, 0.1f);
	InertiaCorrection(m_nShakeLengthY, 0.0f, 0.1f);
}

//==================================================================================
// �J�����̏�ԍX�V����
//==================================================================================
void CCamera::UpdateState(void)
{
	switch (m_state)
	{
	case CAMERASTATE_NONE:
		break;

	case CAMERASTATE_SHAKE:

		if (m_nCntState % 3 == 0)
		{
			// ��ʗh��
			Shake();
		}
		else
		{
			// ���_�̑������
			m_posV.x += m_fMoveShake;
			m_posV.y += m_fMoveShakeY;
			m_posV.z += m_fMoveShake;

			// �����_�̑������
			m_posR.x += m_fMoveShake;
			m_posR.y += m_fMoveShakeY;
			m_posR.z += m_fMoveShake;

			// �����␳
			InertiaCorrection(m_nShakeLength, 0.0f, 0.01f);
			InertiaCorrection(m_nShakeLengthY, 0.0f, 0.01f);
		}

		// ��ԑJ�ڃJ�E���^�[���Z
		m_nCntState--;

		if (m_nCntState <= 0)
		{
			m_nCntState = 0;
			m_state = CAMERASTATE_NONE;
		}
		break;
	}
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CCamera::SetRotation(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//==========================================================================
// �����擾
//==========================================================================
D3DXVECTOR3 CCamera::GetRotation(void) const
{
	return m_rot;
}

void CCamera::EnableChase(void)
{
	m_bFollow = m_bFollow ? false : true;
}
