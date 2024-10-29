//=============================================================================
// 
// �J�������� [camera.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "calculation.h"
#include "player.h"
#include "title.h"
#include "instantfade.h"
#include "3D_effect.h"
#include "calculation.h"
#include "pause.h"
#include "objectX.h"

#include "camera_debug.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const MyLib::Vector3 TITLE_POSR_DEST = MyLib::Vector3(45271.0f, -34.0f, 591.0f);
	const MyLib::Vector3 RANKING_POSR_DEST = MyLib::Vector3(625.34f, 503.34f, 2667.39f);	// �����L���O�̒����_
	const MyLib::Vector3 DEFAULT_TITLEROT = MyLib::Vector3(0.0f, 0.67f, -0.08f);	// �^�C�g���̃f�t�H���g����
	const MyLib::Vector3 DEFAULT_GAMEROT = MyLib::Vector3(0.0f, 0.0f, -0.32f);		// �Q�[���̃f�t�H���g����
	const MyLib::Vector3 DEFAULT_RESULTROT = MyLib::Vector3(0.0f, 0.0f, -0.15f);	// ���U���g�̃f�t�H���g����
	const MyLib::Vector3 DEFAULT_RANKINGROT = MyLib::Vector3(0.0f, 0.0f, -0.05f);	// �����L���O�̃f�t�H���g����
	const float DEFAULT_TITLELEN = 1265.0f;		// �^�C�g���̃f�t�H���g����
	const float DEFAULT_RANKINGLEN = 1540.0f;	// �����L���O�̃f�t�H���g����
	const float MIN_DISNTANCE = 500.0f;		// �ŏ�����

	const float MULTIPLY_CHASE_POSR = 1.5f;		// �����_�Ǐ]�̔{��
	const float MULTIPLY_CHASE_POSV = 1.5f;		// �����_�Ǐ]�̔{��

	const float MIN_STICKROT = -D3DX_PI * 0.25f;		// �J�����Œ�p
	const float ROT_MOVE_STICK_Y = D3DX_PI * 0.003f;	// ��]�ړ���
	const float ROT_MOVE_STICK_Z = D3DX_PI * 0.003f;	// ��]�ړ���
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCamera::CCamera()
{
	m_viewport.X = 0;		// �`�悷���ʂ̍���X���W
	m_viewport.Y = 0;		// �`�悷���ʂ̍���Y���W
	m_viewport.Width = 0;	// �`�悷���ʂ̕�
	m_viewport.Height = 0;	// �`�悷���ʂ̍���
	m_viewport.Width = 0;	// �`�悷���ʂ̕�
	m_viewport.Height = 0;	// �`�悷���ʂ̍���
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 0.0f;
	m_posR = MyLib::Vector3();		// �����_(�������ꏊ)
	m_posV = MyLib::Vector3();		// ���_(�J�����̈ʒu)
	m_posVDest = MyLib::Vector3();	// �ڕW�̎��_
	m_posRDest = MyLib::Vector3();	// �ڕW�̒����_
	m_posROrigin = MyLib::Vector3();	// ���̒����_
	m_vecU = MyLib::Vector3(0.0f, 1.0f, 0.0f);		// ������x�N�g��
	m_move = MyLib::Vector3();		// �ړ���
	m_rot = MyLib::Vector3();		// ����
	m_rotDest = 0.0f;				// �ڕW�̌���
	m_posTarget = MyLib::Vector3();	// �Ǐ]�ڕW�̈ʒu
	m_fDistance = 0.0f;				// ����
	m_fDestDistance = 0.0f;			// �ڕW�̋���
	m_fOriginDistance = 0.0f;		// ���̋���
	m_nShakeLength = 0.0f;			// �h��̒���
	m_nShakeLengthY = 0.0f;			// Y�̗h��̒���
	m_fMoveShake = 0.0f;			// �h��̈ړ���
	m_fMoveShakeY = 0.0f;			// Y�̗h��̈ړ���
	m_bFollow = false;				// �Ǐ]���邩�ǂ���
	m_bMotion = false;				// ���[�V���������ǂ���
	m_state = CAMERASTATE_NONE;		// ���
	m_pLight = nullptr;				// �f�B���N�V���i�����C�g
	m_fTimerState = 0.0f;			// ��ԃJ�E���^�[
	m_fTimerShake = 0.0f;			// �U���J�E���^�[
	m_pCameraMotion = nullptr;		// �J�������[�V�����̃|�C���^
	m_pControlState = nullptr;		// ����̏�ԃ|�C���^
	m_pDebugControll = nullptr;		// �f�o�b�O����

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCamera::~CCamera()
{

}

//==========================================================================
// �J�����̏���������
//==========================================================================
HRESULT CCamera::Init()
{
	// �r���[�|�[�g�̐ݒ�
	SetViewPort(MyLib::Vector3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));

	// ���Z�b�g
	m_bFollow = true;	// �Ǐ]���邩�ǂ���
	Reset(CScene::MODE_GAME);

	// �f�B���N�V���i�����C�g�̐���
	m_pLight = CLightDir::Create();
	if (m_pLight == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �I�u�W�F�N�g��ނ𖢐ݒ�ɂ���
	m_pLight->SetType(CObject::TYPE::TYPE_NONE);

	// �g�U���̐F��ݒ�
	m_pLight->SetDiffuse(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));

	// ����̏�Ԑݒ�
	SetControlState(DEBUG_NEW CCameraControlState_Normal(this));

#if _DEBUG
	// �f�o�b�O��p����
	if (m_pDebugControll == nullptr)
	{
		m_pDebugControll = DEBUG_NEW CCamera_Debug(this);
	}
#endif

	// �J�������[�V�����쐬
	m_pCameraMotion = CCameraMotion::Create();
	return S_OK;
}

//==========================================================================
// �r���[�|�[�g�̐ݒ�
//==========================================================================
void CCamera::SetViewPort(const MyLib::Vector3& pos, const D3DXVECTOR2& size)
{
	m_viewport.X = (DWORD)pos.x;			// �`�悷���ʂ̍���X���W
	m_viewport.Y = (DWORD)pos.y;			// �`�悷���ʂ̍���Y���W
	m_viewport.Width = (DWORD)size.x;		// �`�悷���ʂ̕�
	m_viewport.Height = (DWORD)size.y;		// �`�悷���ʂ̍���
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
}

//==========================================================================
// �J�����̏I������
//==========================================================================
void CCamera::Uninit()
{
	// ���C�g�̏I��
	SAFE_UNINIT(m_pLight);

	if (m_pControlState != nullptr)
	{
		delete m_pControlState;
		m_pControlState = nullptr;
	}

	if (m_pCameraMotion != nullptr)
	{
		m_pCameraMotion->Uninit();
		m_pCameraMotion = nullptr;
	}

	// �f�o�b�O��p����
	if (m_pDebugControll != nullptr)
	{
		delete m_pDebugControll;
		m_pDebugControll = nullptr;
	}
}

//==========================================================================
// �J�����̍X�V����
//==========================================================================
void CCamera::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	// �f�o�b�O��p����
	if (m_pDebugControll != nullptr)
	{
		m_pDebugControll->Update();
	}

	// ���_�E�����_�ړ�
	MoveCameraInput();
	UpdateSpotLightVec();

	// �����␳
	UtilFunc::Transformation::RotNormalize(m_rot);
	UtilFunc::Transformation::RotNormalize(m_rotDest);
	UtilFunc::Transformation::RotNormalize(m_rotOrigin);

	// �����_�̔��f
	ReflectCameraR();

	// ���_���f
	ReflectCameraV();

	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);


	CInputMouse* pMouse = CInputMouse::GetInstance();

	MyLib::Vector3 pos = UtilFunc::Transformation::CalcScreenToXZ(pMouse->GetPosition(), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), m_mtxView, m_mtxProjection);

	// �e�L�X�g�̕`��
	CManager::GetInstance()->GetDebugProc()->Print(
		"---------------- �J������� ----------------\n"
		"�y�����z[X�F%f Y�F%f Z�F%f]\n"
		"�y�����z[%f]\n"
		"�y���_�z[X�F%f Y�F%f Z�F%f]\n"
		"�y�����_�z[X�F%f Y�F%f Z�F%f]\n",
		m_rot.x, m_rot.y, m_rot.z,
		m_fDistance,
		m_posV.x, m_posV.y, m_posV.z,
		m_posR.x, m_posR.y, m_posR.z);

	// �e�L�X�g�̕`��
	CManager::GetInstance()->GetDebugProc()->Print(
		"---------------- �J������� ----------------\n"
		"�y�����_�z[X�F%f Y�F%f Z�F%f]\n",
		pos.x, pos.y, pos.z);

	// ���C�g�̍X�V
	m_pLight->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �J�������[�V�����쐬
	if (m_pCameraMotion != nullptr)
	{
		m_pCameraMotion->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// ���[�h�ʍX�V����
//==========================================================================
void CCamera::UpdateByMode()
{
	switch (CManager::GetInstance()->GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
		break;

	default:
		break;
	}
}

//==========================================================================
// ���͋@��̃J�����ړ�
//==========================================================================
void CCamera::MoveCameraInput()
{
	// �X�e�B�b�N�ł̈ړ�����
	MoveCameraStick();
}

//==========================================================================
// �R���g���[���[�ł̈ړ�����
//==========================================================================
void CCamera::MoveCameraStick(int nIdx)
{
#if 1
	// ���쏈��
	m_pControlState->Controll(this);
#endif

}

//==========================================================================
// �J�������[�v����
//==========================================================================
void CCamera::WarpCamera(const MyLib::Vector3& pos)
{
	// �����_�̑��
	m_posR = pos;			// �����_
	m_posRDest = m_posR;	// �ڕW�̒����_

	// ���_�̑��
	ReflectCameraV();
	m_posVDest = m_posV;	// �ڕW�̎��_
}

//==========================================================================
// �J�����̎��_�������
//==========================================================================
void CCamera::ReflectCameraV()
{
	if (!m_bFollow)
	{// �Ǐ]���Ȃ��Ƃ�

		// ���_�̑������
		m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
	}
	else
	{// �Ǐ]ON

		// ���_�̑������
		m_posVDest.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posVDest.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posVDest.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;

		// �␳����
		m_posV += (m_posVDest - m_posV) * MULTIPLY_CHASE_POSV;
	}
}

//==========================================================================
// �J�����̒����_�������
//==========================================================================
void CCamera::ReflectCameraR()
{
	if (!m_bFollow ||
		(m_bMotion && m_bFollow))
	{// �Ǐ]���Ȃ��Ƃ�

		//// �����_�̑������
		//m_posR.x = m_posV.x + cosf(m_rot.z) * sinf(m_rot.y) * m_fDistance;
		//m_posR.z = m_posV.z + cosf(m_rot.z) * cosf(m_rot.y) * m_fDistance;
		//m_posR.y = m_posV.y + sinf(m_rot.z) * m_fDistance;
	}
	else
	{// �Ǐ]ON

		// �^�[�Q�b�g�݂�
		m_posRDest = m_posTarget;

		// �␳����
		m_posR += (m_posRDest - m_posR) * MULTIPLY_CHASE_POSR;
	}
}

//==========================================================================
// �J�����̐ݒ菈��
//==========================================================================
void CCamera::SetCamera()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&m_viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(45.0f),	// ����p
								(float)m_viewport.Width / (float)m_viewport.Height,	// �A�X�y�N�g��
								10.0f,		// ��O�̐���
								150000.0f);	// ���s���̐���

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

//==========================================================================
// �X�|�b�g���C�g�̃x�N�g���X�V
//==========================================================================
void CCamera::UpdateSpotLightVec()
{
	// �����x�N�g��
	MyLib::Vector3 vec = MyLib::Vector3();

	// ���_���璍���_�ւ̌���
	vec = m_posR - m_posV;

	// �X�|�b�g���C�g�̕����ݒ�
	m_pLight->SetDirection(vec);
}

//==========================================================================
// �J�����̐U���ݒ菈��
//==========================================================================
void CCamera::SetShake(float fTime, float fLength, float fLengthY)
{
	// �U����Ԃɐݒ�
	m_state = CAMERASTATE_SHAKE;

	if (m_fTimerState > 0.0f)
	{
		if (m_nShakeLength <= fLength)
		{
			m_nShakeLength = fLength;	// �h��̑傫��
		}
		if (m_nShakeLengthY <= fLengthY)
		{
			m_nShakeLengthY = fLengthY;	// �h��̑傫��
		}
		if (m_fTimerState <= fTime)
		{
			m_fTimerState = fTime;	// ��ԑJ�ڃJ�E���^�[
		}

		// �U���J�E���^�[�̏�����
		m_fTimerShake = 0.0f;
	}
	else
	{
		m_nShakeLength = fLength;	// �h��̑傫��
		m_nShakeLengthY = fLengthY;	// Y�̗h��̑傫��
		m_fTimerState = fTime;		// ��ԑJ�ڃJ�E���^�[

		// �U���J�E���^�[�̏�����
		m_fTimerShake = 0.0f;
	}
}

//==========================================================================
// �J�����̏�ԍX�V����
//==========================================================================
void CCamera::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	switch (m_state)
	{
	case CAMERASTATE_NONE:
		break;

	case CAMERASTATE_SHAKE:

		// �U���J�E���^�[���Z
		m_fTimerShake += fDeltaTime * fSlowRate;
		if (m_fTimerShake >= 0.05f)
		{
			// �U���J�E���^�[������
			m_fTimerShake = 0.0f;

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
			UtilFunc::Correction::InertiaCorrection(m_nShakeLength, 0.0f, 0.01f);
			UtilFunc::Correction::InertiaCorrection(m_nShakeLengthY, 0.0f, 0.01f);
		}

		// ��ԑJ�ڃJ�E���^�[���Z
		m_fTimerState -= fDeltaTime * fSlowRate;

		if (m_fTimerState <= 0.0f)
		{
			m_fTimerState = 0.0f;
			m_fTimerShake = 0.0f;
			m_state = CAMERASTATE_NONE;
			m_nShakeLength = 0.0f;	// �h��̑傫��
			m_nShakeLengthY = 0.0f;	// Y�̗h��̑傫��
		}
		break;
	}
}

//==========================================================================
// �J�����̐U������
//==========================================================================
void CCamera::Shake()
{
	// �����擾
	int nLength = (int)m_nShakeLength;
	int nLengthY = (int)m_nShakeLengthY;

	m_fMoveShake = (float)UtilFunc::Transformation::Random(-nLength, nLength) * sinf((float)UtilFunc::Transformation::Random(-314, 314) * 0.01f);	// �h��̈ړ���
	m_fMoveShakeY = (float)UtilFunc::Transformation::Random(-nLengthY, nLengthY);						// Y�̗h��̈ړ���

	// ���_�̑������
	m_posV.x += (float)UtilFunc::Transformation::Random(-nLength, nLength) * sinf((float)UtilFunc::Transformation::Random(-314, 314) * 0.01f);
	m_posV.y += (float)UtilFunc::Transformation::Random(-nLengthY, nLengthY);
	m_posV.z += (float)UtilFunc::Transformation::Random(-nLength, nLength) * cosf((float)UtilFunc::Transformation::Random(-314, 314) * 0.01f);

	// �����_�̑������
	m_posR.x += (float)UtilFunc::Transformation::Random(-nLength, nLength) * sinf((float)UtilFunc::Transformation::Random(-314, 314) * 0.01f);
	m_posR.y += (float)UtilFunc::Transformation::Random(-nLengthY, nLengthY);
	m_posR.z += (float)UtilFunc::Transformation::Random(-nLength, nLength) * cosf((float)UtilFunc::Transformation::Random(-314, 314) * 0.01f);

	// �����␳
	UtilFunc::Correction::InertiaCorrection(m_nShakeLength, 0.0f, 0.1f);
	UtilFunc::Correction::InertiaCorrection(m_nShakeLengthY, 0.0f, 0.1f);
}


//==========================================================================
// ���Z�b�g
//==========================================================================
void CCamera::Reset(CScene::MODE mode)
{
	// ����̏�Ԑݒ�
	SetControlState(DEBUG_NEW CCameraControlState_Normal(this));

	m_bFollow = false;	// �Ǐ]���邩�ǂ���

	// �J�������[�V�����̈ʒu���Z�b�g
	if (m_pCameraMotion != nullptr)
	{
		m_pCameraMotion->SetPosition(0.0f);
	}

	// ���Z�b�g
	ResetGame();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(45.0f),
								(float)m_viewport.Width / (float)m_viewport.Height,
								10.0f,			// ���s���̐���
								1500000.0f);	// ���s���̐���

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
}

//==========================================================================
// �Q�[���̃��Z�b�g
//==========================================================================
void CCamera::ResetGame()
{
	m_posR = MyLib::Vector3(0.0f, 200.0f, -560.0f);				// �����_(�������ꏊ)
	m_posV = MyLib::Vector3(0.0f, 300.0f, m_posR.z + -400.0f);	// ���_(�J�����̈ʒu)
	m_posVDest = m_posV;							// �ڕW�̎��_
	m_posRDest = m_posR;							// �ڕW�̒����_
	m_posROrigin = m_posR;							// ���̒����_
	m_vecU = MyLib::Vector3(0.0f, 1.0f, 0.0f);		// ������x�N�g��
	m_move = MyLib::Vector3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_rot = DEFAULT_GAMEROT;						// ����
	m_rotOrigin = m_rot;							// ���̌���
	m_rotDest = DEFAULT_GAMEROT;					// �ڕW�̌���
	m_posTarget = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// �ڕW�̈ʒu
	m_fDistance = MIN_DISNTANCE;					// ����
	m_fDestDistance = MIN_DISNTANCE;				// �ڕW�̋���
	m_fOriginDistance = MIN_DISNTANCE;				// ���̋���
	m_state = CAMERASTATE_NONE;						// ���
	m_fTimerState = 0.0f;							// ��ԃJ�E���^�[
	m_fTimerShake = 0.0f;							// �U���J�E���^�[
}

//==========================================================================
//  �X�N���[�����̔���
//==========================================================================
bool CCamera::IsOnScreen(const MyLib::Vector3& pos)
{
	// �Ԃ�l�p�̕ϐ�
	bool bIn = false;

	// ����
	//�r���[�|�[�g�̐ݒ�
	D3DVIEWPORT9 vp = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };

	//�v�Z�p�ϐ��錾
	D3DXMATRIX mtxWorld; //���[���h�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	//�G�̃X�N���[�����W���Z�o
	MyLib::Vector3 screenPos;
	D3DXVec3Project
	(
		&screenPos,
		&pos,
		&vp,
		&m_mtxProjection,
		&m_mtxView,
		&mtxWorld
	);

	// ����
	if (screenPos.x >= 0.0f && screenPos.x <= SCREEN_WIDTH &&
		screenPos.y >= 0.0f && screenPos.y <= SCREEN_HEIGHT &&
		screenPos.z < 1.0f)
	{
		bIn = true;
	}

	// �Ԃ�
	return bIn;
}

//==========================================================================
//  �X�N���[�����W�擾
//==========================================================================
MyLib::Vector3 CCamera::GetScreenPos(const MyLib::Vector3& pos)
{
	//�r���[�|�[�g�̐ݒ�
	D3DVIEWPORT9 vp = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };

	//�v�Z�p�ϐ��錾
	D3DXMATRIX mtxWorld; //���[���h�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	//�X�N���[�����W���Z�o
	MyLib::Vector3 screenPos;
	D3DXVec3Project
	(
		&screenPos,
		&pos,
		&vp,
		&m_mtxProjection,
		&m_mtxView,
		&mtxWorld
	);

	// �Ԃ�
	return screenPos;
}

//==========================================================================
// ����̏�Ԑݒ�
//==========================================================================
void CCamera::SetControlState(CCameraControlState* state)
{
	delete m_pControlState;
	m_pControlState = state;
}

//==========================================================================
// ���̑��쏈��
//==========================================================================
void CCameraControlState::Controll(CCamera* pCamera)
{
	// �Q�[���p�b�h���擾
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	MyLib::Vector3 rot = pCamera->GetRotation();

	// ����]
	m_moveRot.y += pInputGamepad->GetStickPositionRatioR(0).x * ROT_MOVE_STICK_Y;

	// �c��]
	if (pInputGamepad->GetStickPositionRatioR(0).y < 0.0f)
	{
		m_moveRot.z += pInputGamepad->GetStickPositionRatioR(0).y * ROT_MOVE_STICK_Z;
	}
	else if (pInputGamepad->GetStickPositionRatioR(0).y > 0.0f)
	{
		m_moveRot.z += pInputGamepad->GetStickPositionRatioR(0).y * ROT_MOVE_STICK_Z;
	}

	// �ړ�����
	rot += m_moveRot;
	rot.z = UtilFunc::Transformation::Clamp(rot.z, (D3DX_PI * -0.5f) + 0.02f, (D3DX_PI * 0.5f) - 0.02f);

	// 0�␳
	m_moveRot += (MyLib::Vector3(0.0f) - m_moveRot) * 0.25f;

	// ��]�ړ��ʐݒ�
	pCamera->SetRotation(rot);
}
