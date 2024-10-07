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
#include "elevation.h"
#include "title.h"
#include "instantfade.h"
#include "light.h"
#include "3D_effect.h"
#include "calculation.h"
#include "limitarea.h"
#include "pause.h"

#include "objectX.h"
#include "spline.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define MOVE			(2.5f)				// �ړ���
#define MAX_LENGTH		(50000.0f)			// �ő勗��
#define MIN_LENGTH		(10.0f)				// �ŏ�����
#define START_CAMERALEN	(1700.0f)			// ���̋���
#define ROT_MOVE_MOUSE	(0.01f)				// ��]�ړ���
#define ROT_MOVE_STICK_Y	(0.00040f)			// ��]�ړ���
#define ROT_MOVE_STICK_Z	(0.00020f)			// ��]�ړ���
#define ROT_MOVE		(0.025f)			// ��]�ړ���
#define MIN_STICKROT			(-D3DX_PI * 0.25f)	// �J�����Œ�p
#define MIN_ROT			(-D3DX_PI * 0.49f)	// �J�����Œ�p
#define MAX_ROT			(D3DX_PI * 0.49f)	// �J�����Œ�p
#define BACKFOLLOW_TIME	(20)				// �w�ʕ␳�܂ł̎���
#define TITLESTATE_CHANGE	(60 * 14)
#define TITLESTATE_CHASE	(60 * 20)
#define RESULT_LEN	(500.0f)
//#define RESULT_LEN	(1000.0f)
#define DECIDECAMERAROT_NONE		(MyLib::Vector3(0.0f, 0.0f, 0.0f))
#define DECIDECAMERAPOS_NONE		(MyLib::Vector3(0.0f, 230.0f, -50.0f))
#define DECIDE_LEN	(500.0f)


namespace
{
	const MyLib::Vector3 TITLE_POSR_DEST = MyLib::Vector3(45271.0f, -34.0f, 591.0f);
	const MyLib::Vector3 RANKING_POSR_DEST = MyLib::Vector3(625.34f, 503.34f, 2667.39f);	// �����L���O�̒����_
	//const MyLib::Vector3 DEFAULT_GAMEROT = MyLib::Vector3(0.0f, 0.0f, -0.10f);	// �Q�[���̃f�t�H���g����
	const MyLib::Vector3 DEFAULT_TITLEROT = MyLib::Vector3(0.0f, 0.67f, -0.08f);	// �^�C�g���̃f�t�H���g����
	const MyLib::Vector3 DEFAULT_RESULTROT = MyLib::Vector3(0.0f, 0.0f, -0.15f);	// ���U���g�̃f�t�H���g����
	const MyLib::Vector3 DEFAULT_RANKINGROT = MyLib::Vector3(0.0f, 0.0f, -0.05f);	// �����L���O�̃f�t�H���g����
	const MyLib::Vector3 DEFAULT_GAMEROT = MyLib::Vector3(0.0f, 0.38f, -0.05f);	// �Q�[���̃f�t�H���g����
	const float DEFAULT_TITLELEN = 1265.0f;		// �^�C�g���̃f�t�H���g����
	const float DEFAULT_RANKINGLEN = 1540.0f;	// �����L���O�̃f�t�H���g����
	const float MULTIPLY_POSV_CORRECTION = 2.1f;	// (�Q�[����)���_�̕␳�W���{��
	const float MULTIPLY_POSR_CORRECTION = 2.1f;	// (�Q�[����)�����_�̕␳�W���{��
	const float DISATNCE_POSR_PLAYER = 0.0f;		// (�Q�[����)�v���C���[�Ƃ̒����_����
	const float MIN_ROCKONDISTANCE = 1.0f;
	const float ROTDISTANCE_ROCKON = D3DX_PI * 0.095f;	// ���b�N�I�������̃Y��
	const MyLib::Vector3 ROTDISTANCE_COUNTER = MyLib::Vector3(0.0f, D3DX_PI * 0.5f, -D3DX_PI * 0.05f);	// �������̌����Y��
	const float LENGTH_COUNTER = 400.0f;					// �J�E���^�[���̃J��������
	const MyLib::Vector3 ROTATION_PRAYER = MyLib::Vector3(0.0f, -0.89f, 0.06f);	// �F�莞�̌���
	const float UPDISTANCE_MULTIPLY = (0.25f);
	const float NOTUPDISTANCE_MULTIPLY = (0.05f);
	const float MIN_DISNTANCE = (1500.0f);
	const float DISTANCE_TIMER = (1.0f / 120.0f);
	const float MAX_AUTODISTANCEHEIGHT = 600.0f;	// ������������̍ő�l
	const MyLib::Vector3 GOAL_ROT = MyLib::Vector3(0.0f, D3DX_PI * 0.3f, 0.06f);	// �F�莞�̌���
	const float GOAL_MULTI = (0.03f);
	const float GOAL_LEN = (500.0f);
	const float MOVE_WASD = 10.0f;
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CCamera::ROCKON_STATE_FUNC CCamera::m_RockOnStateFunc[] =	// �J�E���^�[���
{
	&CCamera::RockOnStateNormal,	// �ʏ�
	&CCamera::RockOnStateCounter,	// �J�E���^�[
};

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
	m_posR = mylib_const::DEFAULT_VECTOR3;		// �����_(�������ꏊ)
	m_posV = mylib_const::DEFAULT_VECTOR3;		// ���_(�J�����̈ʒu)
	m_posVDest = mylib_const::DEFAULT_VECTOR3;	// �ڕW�̎��_
	m_posRDest = mylib_const::DEFAULT_VECTOR3;	// �ڕW�̒����_
	m_vecU = MyLib::Vector3(0.0f, 1.0f, 0.0f);		// ������x�N�g��
	m_move = mylib_const::DEFAULT_VECTOR3;		// �ړ���
	m_rot = mylib_const::DEFAULT_VECTOR3;		// ����
	m_rotDest = 0.0f;							// �ڕW�̌���
	m_Moverot = 0.0f;							// �����̈ړ���
	m_rotVDest = mylib_const::DEFAULT_VECTOR3;	// �ڕW�̎��_�̌���
	m_TargetPos = mylib_const::DEFAULT_VECTOR3;	// �Ǐ]�ڕW�̈ʒu
	m_TargetRot = mylib_const::DEFAULT_VECTOR3;	// �Ǐ]�ڕW�̈ʒu
	m_fDistance = 0.0f;							// ����
	m_fDestDistance = 0.0f;						// �ڕW�̋���
	m_fOriginDistance = 0.0f;					// ���̋���
	m_nShakeLength = 0.0f;						// �h��̒���
	m_nShakeLengthY = 0.0f;						// Y�̗h��̒���
	m_fMoveShake = 0.0f;						// �h��̈ړ���
	m_fMoveShakeY = 0.0f;						// Y�̗h��̈ړ���
	m_fHeightMaxDest = 0.0f;					// �J�����̍ő卂���̖ڕW
	m_fHeightMax = 0.0f;						// �J�����̍ő卂��
	m_fDiffHeight = 0.0f;						// �����̍���
	m_fDiffHeightSave = 0.0f;					// �����̍����ۑ��p
	m_fDiffHeightDest = 0.0f;					// �ڕW�̍����̍���
	m_bFollow = false;							// �Ǐ]���邩�ǂ���
	m_bMotion = false;							// ���[�V���������ǂ���
	m_bRotationZ = false;						// Z��]�o���邩�ǂ���
	m_bRotationY = false;						// Y��]�o���邩�ǂ���
	m_bRockON = false;							// ���b�N�I�����邩
	m_state = CAMERASTATE_NONE;					// ���
	m_nCntState = 0;							// ��ԃJ�E���^�[
	m_nCntDistance = 0;							// �����J�E���^�[
	m_nOriginCntDistance = 0;					// ���̋����J�E���^�[
	m_fDistanceCorrection = 0.0f;				// �����̊����␳�W��
	m_fDistanceDecrementValue = 0.0f;			// �����̌����W��
	m_nChasePlayerIndex = 0;					// �Ǐ]����v���C���[�̃C���f�b�N�X�ԍ�
	m_RockOnDir = ROCKON_DIR_RIGHT;				// ���b�N�I�����̌���
	m_stateRockOn = ROCKON_NORMAL;				// ���b�N�I�����̏��
	m_pCameraMotion = nullptr;					// �J�������[�V�����̃|�C���^

	m_StateCameraR = POSR_STATE_NORMAL;		// �����_�̏��
	m_pStateCameraR = nullptr;	// �����_�̏�ԃ|�C���^
	m_pStateCameraV = nullptr;	// ���_�̏�ԃ|�C���^
	m_pControlState = nullptr;	// ����̏�ԃ|�C���^

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

	// ���_�̑������
	SetCameraV();

	// �����_�̏�Ԑݒ�
	SetStateCamraR(DEBUG_NEW CStateCameraR());

	// ���_�̏�Ԑݒ�
	SetStateCameraV(DEBUG_NEW CStateCameraV());

	// ����̏�Ԑݒ�
	SetControlState(DEBUG_NEW CCameraControlState_Normal(this));

	// �J�������[�V�����쐬
	m_pCameraMotion = CCameraMotion::Create();
	return S_OK;
}

//==========================================================================
// �r���[�|�[�g�̐ݒ�
//==========================================================================
void CCamera::SetViewPort(MyLib::Vector3 pos, D3DXVECTOR2 size)
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
	if (m_pStateCameraR != nullptr)
	{
		delete m_pStateCameraR;
		m_pStateCameraR = nullptr;
	}

	// ���_�̏�ԃ|�C���^
	if (m_pStateCameraV != nullptr)
	{
		delete m_pStateCameraV;
		m_pStateCameraV = nullptr;
	}

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
}

//==========================================================================
// �J�����̍X�V����
//==========================================================================
void CCamera::Update()
{
	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	// �����̍������Z�b�g
	m_fDiffHeightSave = 0.0f;

	// ���_�E�����_�ړ�
	MoveCameraFollow();
	MoveCameraInput();
	MoveCameraVR();
	MoveCameraR();
	MoveCameraV();
	MoveCameraDistance();
	MoveCameraDistance();
	UpdateSpotLightVec();

	// ��ԍX�V
	UpdateState();
	

	//#ifdef _DEBUG

	if (pInputKeyboard->GetTrigger(DIK_F7))
	{// F7�������ꂽ,�Ǐ]�؂�ւ�

		m_bFollow = m_bFollow ? false : true;
	}

	if (pInputKeyboard->GetTrigger(DIK_F5))
	{
		m_move = MyLib::Vector3(0.0f, 0.0f, 0.0f);					// �ړ���
		m_rot = MyLib::Vector3(m_rot.x, 0.0f, m_rot.z);					// ����
		m_rotVDest = m_rot;										// �ڕW�̎��_�̌���
		m_fOriginDistance = 0.0f;					// ���̋���
		m_fDestDistance = START_CAMERALEN;
		m_fDistance = m_fDestDistance;
	}
	//#endif

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





	if (!pInputKeyboard->GetPress(DIK_LALT) && 
		pMouse->GetTrigger(CInputMouse::BUTTON::BUTTON_LEFT))
	{
	/*	CMyEffekseer::GetInstance()->SetEffect(
			CMyEffekseer::EFKLABEL::EFKLABEL_SPRAYWATER,
			pos, MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 40.0f, true);*/
	}

	/*CEffect3D::Create(
		pos,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
		80.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);*/



	// �J�������[�V�����쐬
	if (m_pCameraMotion != nullptr)
	{
		m_pCameraMotion->Update();
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
	// �}�E�X�ł̈ړ�����
	MoveCameraMouse();


	// �X�e�B�b�N�ł̈ړ�����
	MoveCameraStick();
}

//==========================================================================
// �R���g���[���[�ł̈ړ�����
//==========================================================================
void CCamera::MoveCameraStick(int nIdx)
{
#if 0
	if (CManager::GetInstance()->GetMode() == CScene::MODE::MODE_GAME ||
		CManager::GetInstance()->GetMode() == CScene::MODE::MODE_GAMETUTORIAL)
	{
		// ���쏈��
		m_pControlState->MoveCamera(this);
	}
#endif

	// �p�x�̐��K��
	UtilFunc::Transformation::RotNormalize(m_rot);

	// ���_�̑������
	SetCameraV();
}

//==========================================================================
// �J�������[�v����
//==========================================================================
void CCamera::WarpCamera(MyLib::Vector3 pos)
{
	// �����_�̑��
	m_posR = pos;
	m_posRDest = m_posR;		// �ڕW�̒����_

	// ���_�̑��
	m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
	m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
	m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
	m_posVDest = m_posV;								// �ڕW�̎��_

}

//==========================================================================
// �}�E�X�ł̈ړ�����
//==========================================================================
void CCamera::MoveCameraMouse()
{
#if 1

	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	// �L�[�{�[�h���擾
	CInputMouse* pInputMouse = CInputMouse::GetInstance();

	if (pInputMouse->GetPress(CInputMouse::BUTTON_LEFT) &&
		pInputMouse->GetPress(CInputMouse::BUTTON_RIGHT))
	{// ���E��������

		m_move.x += (pInputMouse->GetMouseMove().x * sinf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE) -
			(pInputMouse->GetMouseMove().y * cosf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE);

		m_move.z += (pInputMouse->GetMouseMove().x * cosf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE) +
			(pInputMouse->GetMouseMove().y * sinf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE);

		// �ړ��ʕ␳
		MoveCameraVR();

		// �p�x�̐��K��
		UtilFunc::Transformation::RotNormalize(m_rot.y);
		UtilFunc::Transformation::RotNormalize(m_rot.z);

		// �����_�ݒ�
		SetCameraR();
	}
	else if (
		!m_pCameraMotion->IsEdit() &&
		pInputMouse->GetPress(CInputMouse::BUTTON_LEFT) &&
		!pInputKeyboard->GetPress(DIK_LALT))
	{// ���N���b�N�ړ�

		if (pInputKeyboard->GetPress(DIK_W))
		{
			if (pInputKeyboard->GetPress(DIK_A))
			{
				m_moveWASD.x += sinf(D3DX_PI * -0.25f + m_rot.y) * MOVE_WASD;
				m_moveWASD.z += cosf(D3DX_PI * -0.25f + m_rot.y) * MOVE_WASD;
			}
			else if (pInputKeyboard->GetPress(DIK_D))
			{
				m_moveWASD.x += sinf(D3DX_PI * 0.25f + m_rot.y) * MOVE_WASD;
				m_moveWASD.z += cosf(D3DX_PI * 0.25f + m_rot.y) * MOVE_WASD;
			}
			else
			{
				m_moveWASD.x += sinf(D3DX_PI * 0.0f + m_rot.y) * MOVE_WASD;
				m_moveWASD.z += cosf(D3DX_PI * 0.0f + m_rot.y) * MOVE_WASD;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_S))
		{
			if (pInputKeyboard->GetPress(DIK_A))
			{
				m_moveWASD.x += sinf(D3DX_PI * -0.75f + m_rot.y) * MOVE_WASD;
				m_moveWASD.z += cosf(D3DX_PI * -0.75f + m_rot.y) * MOVE_WASD;
			}
			else if (pInputKeyboard->GetPress(DIK_D))
			{
				m_moveWASD.x += sinf(D3DX_PI * 0.75f + m_rot.y) * MOVE_WASD;
				m_moveWASD.z += cosf(D3DX_PI * 0.75f + m_rot.y) * MOVE_WASD;
			}
			else
			{
				m_moveWASD.x += sinf(D3DX_PI * 1.0f + m_rot.y) * MOVE_WASD;
				m_moveWASD.z += cosf(D3DX_PI * 1.0f + m_rot.y) * MOVE_WASD;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_A))
		{
			m_moveWASD.x += sinf(D3DX_PI * -0.5f + m_rot.y) * MOVE_WASD;
			m_moveWASD.z += cosf(D3DX_PI * -0.5f + m_rot.y) * MOVE_WASD;
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{
			m_moveWASD.x += sinf(D3DX_PI * 0.5f + m_rot.y) * MOVE_WASD;
			m_moveWASD.z += cosf(D3DX_PI * 0.5f + m_rot.y) * MOVE_WASD;
		}

		// �ړ��ʕ������Z
		m_posV.x += m_moveWASD.x;
		m_posV.z += m_moveWASD.z;

		// �ړ��ʂ����Z�b�g
		m_moveWASD.x += (0.0f - m_moveWASD.x) * 0.15f;
		m_moveWASD.z += (0.0f - m_moveWASD.z) * 0.15f;

		// �����_�̈ʒu�X�V
		SetCameraR();



		// �p�x�̐��K��
		UtilFunc::Transformation::RotNormalize(m_rot.y);
		UtilFunc::Transformation::RotNormalize(m_rot.z);

		// �����_�ݒ�
		SetCameraR();
	}
	else if (
		pInputKeyboard->GetPress(DIK_LALT) &&
		pInputMouse->GetPress(CInputMouse::BUTTON_LEFT))
	{// ���N���b�N���Ă�Ƃ�,���_��]

		m_rot.y += pInputMouse->GetMouseMove().x * ROT_MOVE_MOUSE;
		m_rot.z += pInputMouse->GetMouseMove().y * ROT_MOVE_MOUSE;

		// �p�x�̐��K��
		UtilFunc::Transformation::RotNormalize(m_rot);

		// �l�̐��K��
		UtilFunc::Transformation::ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

		// ���_�̑������
		SetCameraV();

	}
	else if (pInputMouse->GetPress(CInputMouse::BUTTON_RIGHT))
	{// �E�N���b�N���Ă�Ƃ�,�����_��]

		m_rot.y += pInputMouse->GetMouseMove().x * ROT_MOVE_MOUSE;
		m_rot.z += pInputMouse->GetMouseMove().y * ROT_MOVE_MOUSE;

		// �p�x�̐��K��
		UtilFunc::Transformation::RotNormalize(m_rot);

		// �l�̐��K��
		UtilFunc::Transformation::ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

		// �����_�̈ʒu�X�V
		SetCameraR();
	}

	// �}�E�X�z�C�[���ŋ�������
	m_fDistance += pInputMouse->GetMouseMove().z * (MOVE * 0.3f);
	m_fDestDistance += pInputMouse->GetMouseMove().z * (MOVE * 0.3f);
	m_fOriginDistance += pInputMouse->GetMouseMove().z * (MOVE * 0.3f);
#endif

	// ���_�̑������
	SetCameraV();
}

//==========================================================================
// �J�����̎��_�ړ�
//==========================================================================
void CCamera::MoveCameraV()
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

#ifdef _DEBUG
	// ���_�ړ�
	if (pInputKeyboard->GetPress(DIK_Y))
	{// Y�L�[�������ꂽ,���_��ړ�

		m_rot.z -= ROT_MOVE;
	}
	else if (pInputKeyboard->GetPress(DIK_N))
	{// N�L�[�������ꂽ,���_���ړ�

		m_rot.z += ROT_MOVE;
	}

	if (pInputKeyboard->GetPress(DIK_Z))
	{// Z�L�[�������ꂽ

		m_rot.y += ROT_MOVE;
	}
	if (pInputKeyboard->GetPress(DIK_C))
	{// C�L�[�������ꂽ

		m_rot.y -= ROT_MOVE;
	}
#endif


	// �p�x�̐��K��
	UtilFunc::Transformation::RotNormalize(m_rot.y);
	UtilFunc::Transformation::RotNormalize(m_rot.z);

	// �l�̐��K��
	UtilFunc::Transformation::ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

	// ���_�̑������
	SetCameraV();

}

//==========================================================================
// �J�����̒����_�ړ�
//==========================================================================
void CCamera::MoveCameraR()
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

#ifdef _DEBUG
	// ����
	if (pInputKeyboard->GetPress(DIK_Q))
	{// Q�L�[�������ꂽ,������


		m_rot.y -= ROT_MOVE;

	}
	else if (pInputKeyboard->GetPress(DIK_E))
	{// E�L�[�������ꂽ,������

		m_rot.y += ROT_MOVE;
	}

	// �㏸���~
	if (pInputKeyboard->GetPress(DIK_T))
	{// T�L�[�������ꂽ

		m_rot.z += ROT_MOVE;

	}
	else if (pInputKeyboard->GetPress(DIK_B))
	{// B�L�[�������ꂽ

		m_rot.z -= ROT_MOVE;

	}
#endif


	// �p�x�̐��K��
	UtilFunc::Transformation::RotNormalize(m_rot.y);
	UtilFunc::Transformation::RotNormalize(m_rot.z);

	// �l�̐��K��
	UtilFunc::Transformation::ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

	// �����_�̈ʒu�X�V
	SetCameraR();
}

//==========================================================================
// �J�����̎��_�E�����_�ړ�
//==========================================================================
void CCamera::MoveCameraVR()
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

//==========================================================================
//  ���_�E�����_�ԋ���
//==========================================================================
void CCamera::MoveCameraDistance()
{
	// �����J�E���^�[���Z
	m_nCntDistance--;

	if (m_nCntDistance <= 0)
	{
		m_nCntDistance = 0;

		if (m_fDistanceDecrementValue >= 0.0f)
		{
			if (m_fDestDistance >= m_fOriginDistance)
			{// �J�������߂Â��Ă���
				m_fDestDistance -= m_fDistanceDecrementValue;
			}
		}
		else
		{
			if (m_fDestDistance <= m_fOriginDistance)
			{// �J�������߂Â��Ă���
				m_fDestDistance -= m_fDistanceDecrementValue;
			}
		}
	}

	// ���E�l�␳
	if (m_fDistance >= MAX_LENGTH)
	{
		m_fDistance = MAX_LENGTH;
	}
	else if (m_fDistance <= MIN_LENGTH)
	{
		m_fDistance = MIN_LENGTH;
	}

	if (!m_pCameraMotion->IsEdit() || !m_bMotion)
	{
		// �ڕW�̋����֕␳
		UtilFunc::Correction::InertiaCorrection(m_fDistance, m_fDestDistance, m_fDistanceCorrection);
	}

}

//==========================================================================
// �ʏ��Ԃ̃��b�N�I������
//==========================================================================
void CCamera::RockOnStateNormal()
{
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(m_nChasePlayerIndex);
	MyLib::Vector3 playerpos = pPlayer->GetPosition();

	// �ڕW�n�_�����b�N�I���Ƃ̒��S�ɂ���
	m_TargetPosDest = UtilFunc::Calculation::GetCenterPosition3D(m_RockOnPos, playerpos);

	// �����␳
	float factor = 0.2f;
	UtilFunc::Correction::InertiaCorrection(m_TargetPos.x, m_TargetPosDest.x, factor);
	UtilFunc::Correction::InertiaCorrection(m_TargetPos.y, m_TargetPosDest.y, factor);
	UtilFunc::Correction::InertiaCorrection(m_TargetPos.z, m_TargetPosDest.z, factor);

	// 2�_�Ԃ̋���
	float fLen = UtilFunc::Calculation::GetFabsPosLength3D(m_RockOnPos, playerpos);
	float ratio = fLen / CGame::GetInstance()->GetRockOnDistance();

	// �ő働�b�N�I������
	float maxRockOnDistance = CGame::GetInstance()->GetRockOnDistance();

	if (ratio <= 1.0f)
	{
		// �ڕW�̒����ݒ�
		float setDistance = maxRockOnDistance * ratio;
		if (setDistance <= m_fRockOnDistance)
		{
			setDistance = m_fRockOnDistance;
		}

		SetLenDest(setDistance, 2, 2.0f, 0.1f);

		// �ڕW�̊p�x�����߂�
		m_rotDest.y =
			D3DX_PI +
			atan2f((m_TargetPos.x - m_RockOnPos.x), (m_TargetPos.z - m_RockOnPos.z)) +
			((ROTDISTANCE_ROCKON * 2.0f) * (m_RockOnDir - 1)) + ROTDISTANCE_ROCKON;

		if (m_RockOnPos.y > 150.0f)
		{
			// �ڕW�̊p�x�����߂�
			m_rotDest.z = atan2f((m_RockOnPos.y - 150.0f), setDistance) + DEFAULT_GAMEROT.z;
		}
		else
		{
			m_rotDest.z = DEFAULT_GAMEROT.z;
		}

		UtilFunc::Transformation::RotNormalize(m_rotDest);

		// �ڕW�̌���
		float fRotDiff = m_rotDest.y - m_rot.y;
		UtilFunc::Transformation::RotNormalize(fRotDiff);
		m_rot.y += fRotDiff * 0.08f;

		fRotDiff = m_rotDest.z - m_rot.z;
		UtilFunc::Transformation::RotNormalize(fRotDiff);
		m_rot.z += fRotDiff * 0.08f;
	}
	else
	{
		// �ʏ���
		SetStateCamraR(DEBUG_NEW CStateCameraR());

		m_bRockON = false;

		// �ڕW�̒����ݒ�
		SetLenDest(maxRockOnDistance * 0.5f, 60, 2.0f, 0.1f);
	}


	// �e�L�X�g�̕`��
	CManager::GetInstance()->GetDebugProc()->Print(
		"---------------- �J������� ----------------\n"
		"�y�ڕW�̌����z[X�F%f Y�F%f Z�F%f]\n",
		m_rotDest.x, m_rotDest.y, m_rotDest.z);
}

//==========================================================================
// �J�E���^�[��Ԃ̃��b�N�I������
//==========================================================================
void CCamera::RockOnStateCounter()
{
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(m_nChasePlayerIndex);
	MyLib::Vector3 playerpos = pPlayer->GetPosition();

	// �ڕW�n�_�����b�N�I���Ƃ̒��S�ɂ���
	m_TargetPosDest = playerpos;

	// �����␳
	UtilFunc::Correction::InertiaCorrection(m_TargetPos.x, m_TargetPosDest.x, 0.25f);
	UtilFunc::Correction::InertiaCorrection(m_TargetPos.y, m_TargetPosDest.y, 0.25f);
	UtilFunc::Correction::InertiaCorrection(m_TargetPos.z, m_TargetPosDest.z, 0.25f);

	// �ڕW�̌���
	float fRotDiff = m_rotDest.y - m_rot.y;
	UtilFunc::Transformation::RotNormalize(fRotDiff);

	m_rot.y += fRotDiff * 0.08f;
	UtilFunc::Transformation::RotNormalize(m_rot.y);
}

//==========================================================================
// �J�����̎��_�������
//==========================================================================
void CCamera::SetCameraV()
{

	switch (CManager::GetInstance()->GetMode())
	{
	case CScene::MODE_TITLE:
		SetCameraVTitle();
		break;

	case CScene::MODE_TUTORIAL:
		SetCameraVGame();
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_GAMETUTORIAL:
		SetCameraVGame();
		break;

	case CScene::MODE_RESULT:
		SetCameraVResult();
		break;

	case CScene::MODE_RANKING:
		SetCameraVRanking();
		SetCameraVTitle();
		break;

	default:
		SetCameraVResult();
		break;
	}

}

//==========================================================================
// �J�����̎��_���f����
//==========================================================================
void CCamera::SetPositionV()
{
	// ���_�̑������
	m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
	m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
	m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
}

//==========================================================================
// �J�����̎��_�������(�^�C�g��)
//==========================================================================
void CCamera::SetCameraVTitle()
{
	if (m_bFollow == false)
	{// �Ǐ]���Ȃ��Ƃ�

		// ���_�̑������
		m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
	}
	else if (m_bFollow)
	{// �Ǐ]ON

		// ���_�̑������
		m_posVDest.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posVDest.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posVDest.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;

		// �␳����
		m_posV += (m_posVDest - m_posV) * 0.12f;
	}
}

//==========================================================================
// �J�����̎��_�������(�Q�[��)
//==========================================================================
void CCamera::SetCameraVGame()
{
	if (CGame::GetInstance() == nullptr) return;

	CGameManager* pGameManager = CGame::GetInstance()->GetGameManager();
	if (pGameManager == nullptr)
	{
		return;
	}

	// Y��]�̃t���O
	m_bRotationY = true;

	if (m_bFollow == false)
	{// �Ǐ]���Ȃ��Ƃ�

		// ���_�̑������
		m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
	}
	else if (m_bMotion && m_bFollow)
	{// �J�������[�V������

		// ���_�̑������
		m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;

		if (pGameManager->GetType() != CGameManager::SceneType::SCENE_DEBUG)
		{
			m_pStateCameraV->SetMotion(this);
		}
	}
	else if (m_bFollow)
	{// �Ǐ]ON
		
		// �����X���̌v�Z����
		if (pGameManager->GetType() != CGameManager::SceneType::SCENE_DEBUG)
		{
			m_fAutoRot_Dest = (m_posR.y - 200.0f) / m_fDistance + m_rotOrigin.z;
			m_rot.z += (m_fAutoRot_Dest - m_rot.z) * 0.2f;
		}

		// ���_�̑������
		m_posVDest.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posVDest.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posVDest.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;

		// �ʒu����
		m_pStateCameraV->LimitPos(this);

		float fDistance = 0.0f;
		m_fHeightMaxDest = m_posVDest.y;

		// Z����o������
		m_bRotationZ = true;

		// �ڕW�̊p�x�����߂�
		float fRotDest = atan2f((m_posVDest.x - m_posR.x), (m_posVDest.z - m_posR.z));
		

		// �ڕW�̎��_�X�V
		if (m_fHeightMaxDest > m_posVDest.y)
		{
			// �ő�̍����␳
			m_fHeightMax += (m_fHeightMaxDest - m_fHeightMax) * 0.08f;

			m_posVDest.y = m_fHeightMax;

			// �����̍���
			m_fDiffHeightSave += m_fHeightMax - m_posV.y;
		}

		// �����ɂ�鋗���ݒ�
		if (pGameManager->GetType() != CGameManager::SceneType::SCENE_DEBUG)
		{
			if (!m_bMotion)
			{
				m_pStateCameraV->Distance(this);
			}
		}

		// �␳����
		m_posV += (m_posVDest - m_posV) * (0.12f * MULTIPLY_POSV_CORRECTION);
	}
}

//==========================================================================
// �J�����̎��_�������(���U���g)
//==========================================================================
void CCamera::SetCameraVResult()
{
	// ���_�̑������
	m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
	m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
	m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
}

//==========================================================================
// �J�����̎��_�������(�����L���O)
//==========================================================================
void CCamera::SetCameraVRanking()
{
	SetCameraVResult();
}

//==========================================================================
// �J�����̒����_�������
//==========================================================================
void CCamera::SetCameraR()
{

	switch (CManager::GetInstance()->GetMode())
	{
	case CScene::MODE_TITLE:
		SetCameraRTitle();
		break;

	case CScene::MODE_TUTORIAL:
		SetCameraRGame();
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_GAMETUTORIAL:
		SetCameraRGame();
		break;

	case CScene::MODE_RESULT:
		SetCameraRResult();
		break;

	case CScene::MODE_RANKING:
		SetCameraRRanking();
		SetCameraRTitle();
		break;

	default:
		SetCameraRResult();
		break;
	}

}

//==========================================================================
// �J�����̒����_�������(�^�C�g��)
//==========================================================================
void CCamera::SetCameraRTitle()
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
		m_posRDest = m_TargetPos;

		// �␳����
		m_posR += (m_posRDest - m_posR) * 0.08f;
	}
}

//==========================================================================
// �J�����̒����_�������(�Q�[��)
//==========================================================================
void CCamera::SetCameraRGame()
{
	if (CGame::GetInstance() == nullptr) return;

	CGameManager* pGameManager = CGame::GetInstance()->GetGameManager();
	if (pGameManager == nullptr)
	{
		return;
	}

	if (!m_bFollow)
	{// �Ǐ]���Ȃ��Ƃ�

		// �����_�̑������
		m_posR.x = m_posV.x + cosf(m_rot.z) * sinf(m_rot.y) * m_fDistance;
		m_posR.z = m_posV.z + cosf(m_rot.z) * cosf(m_rot.y) * m_fDistance;
		m_posR.y = m_posV.y + sinf(m_rot.z) * m_fDistance;
	}
	else if (m_bMotion && m_bFollow)
	{// �J�������[�V������

		// �����_�̑������
		m_posR.x = m_posV.x + cosf(m_rot.z) * sinf(m_rot.y) * m_fDistance;
		m_posR.z = m_posV.z + cosf(m_rot.z) * cosf(m_rot.y) * m_fDistance;
		m_posR.y = m_posV.y + sinf(m_rot.z) * m_fDistance;
	}
	else
	{// �Ǐ]ON

		float fYcamera = 100.0f;
		float fCorrectionHeight = 50.0f;

		if (m_TargetPos.y >= fCorrectionHeight)
		{
			fYcamera = (m_TargetPos.y - fCorrectionHeight) + fYcamera;
		}
		else if (m_TargetPos.y <= -fCorrectionHeight)
		{
			fYcamera = (m_TargetPos.y + fCorrectionHeight) + fYcamera;
		}

		fYcamera = m_TargetPos.y + 150.0f;

		if (fYcamera <= 0.0f)
		{
			m_fDiffHeightSave += -fYcamera;
		}

		// �ڕW�̍����̍�������
		//m_fDiffHeightDest = m_fDiffHeightSave;
		m_fDiffHeightDest = m_TargetPos.y;

		// �����̍�����␳����
		m_fDiffHeight += (m_fDiffHeightDest - m_fDiffHeight) * 0.01f;

		// �����_�̑������
		m_pStateCameraR->SetCameraR(this);

		if (pGameManager->GetType() == CGameManager::SceneType::SCENE_DEBUG)
		{
			m_posRDest = m_TargetPos;
		}
		else
		{
			m_posRDest.y = m_AutoMovingPosR.y;
		}

		// �␳����
		m_posR += (m_posRDest - m_posR) * (0.08f * MULTIPLY_POSR_CORRECTION);
	}
}

//==========================================================================
// �J�����̒����_�������(���U���g)
//==========================================================================
void CCamera::SetCameraRResult()
{
	// �����_�̑������
	m_posR.x = m_posV.x + cosf(m_rot.z) * sinf(m_rot.y) * m_fDistance;
	m_posR.z = m_posV.z + cosf(m_rot.z) * cosf(m_rot.y) * m_fDistance;
	m_posR.y = m_posV.y + sinf(m_rot.z) * m_fDistance;
}

//==========================================================================
// �J�����̒����_�������(�����L���O)
//==========================================================================
void CCamera::SetCameraRRanking()
{
	// �����_�̑������
	m_posR.x = m_posV.x + cosf(m_rot.z) * sinf(m_rot.y) * m_fDistance;
	m_posR.z = m_posV.z + cosf(m_rot.z) * cosf(m_rot.y) * m_fDistance;
	m_posR.y = m_posV.y + sinf(m_rot.z) * m_fDistance;
}

//==========================================================================
// �J�����̔w�ʎ����Ǐ]
//==========================================================================
void CCamera::MoveCameraFollow()
{

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
	MyLib::Vector3 vec = mylib_const::DEFAULT_VECTOR3;

	// ���_���璍���_�ւ̌���
	vec = m_posR - m_posV;

	// ���K��
	D3DXVec3Normalize(&vec, &vec);

	// �X�|�b�g���C�g�̕����ݒ�
	CManager::GetInstance()->GetLight()->UpdateSpotLightDirection(vec);
}

//==========================================================================
// �ڕW�̒����ݒ�
//==========================================================================
void CCamera::SetLenDest(float fLength, int nCntTime, float DecrementValue, float fCorrection)
{
	// �ڕW�̋����ݒ�
	m_fDestDistance = fLength;
	m_nCntDistance = nCntTime;
	m_nOriginCntDistance = m_nCntDistance;		// ���̋����J�E���^�[
	m_fDistanceCorrection = fCorrection;		// �����␳�W��
	m_fDistanceDecrementValue = DecrementValue;	// ������
}

//==========================================================================
// �J�����̐U���ݒ菈��
//==========================================================================
void CCamera::SetShake(int nTime, float fLength, float fLengthY)
{
	// �U����Ԃɐݒ�
	m_state = CAMERASTATE_SHAKE;

	if (m_nCntState > 0)
	{
		if (m_nShakeLength <= fLength)
		{
			m_nShakeLength = fLength;	// �h��̑傫��
		}
		if (m_nShakeLengthY <= fLengthY)
		{
			m_nShakeLengthY = fLengthY;	// �h��̑傫��
		}
		if (m_nCntState <= nTime)
		{
			m_nCntState = nTime;	// ��ԑJ�ڃJ�E���^�[
		}
	}
	else
	{
		m_nShakeLength = fLength;	// �h��̑傫��
		m_nShakeLengthY = fLengthY;	// Y�̗h��̑傫��
		m_nCntState = nTime;		// ��ԑJ�ڃJ�E���^�[
	}
}

//==========================================================================
// �J�����̏�ԍX�V����
//==========================================================================
void CCamera::UpdateState()
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
			UtilFunc::Correction::InertiaCorrection(m_nShakeLength, 0.0f, 0.01f);
			UtilFunc::Correction::InertiaCorrection(m_nShakeLengthY, 0.0f, 0.01f);
		}

		// ��ԑJ�ڃJ�E���^�[���Z
		m_nCntState--;

		if (m_nCntState <= 0)
		{
			m_nCntState = 0;
			m_state = CAMERASTATE_NONE;
			m_nShakeLength = 0.0f;	// �h��̑傫��
			m_nShakeLengthY = 0.0f;	// Y�̗h��̑傫��
			switch (CManager::GetInstance()->GetMode())
			{
			case CScene::MODE_RESULT:
				ResetResult();
				break;
			}
		}
		break;

	case CAMERASTATE_PRAYER:
		StatePrayer();
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
// �F��X�V����
//==========================================================================
void CCamera::StatePrayer()
{
	
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CCamera::Reset(CScene::MODE mode)
{
	// �ʏ���
	SetStateCamraR(DEBUG_NEW CStateCameraR());
	SetStateCameraV(DEBUG_NEW CStateCameraV());

	// ����̏�Ԑݒ�
	SetControlState(DEBUG_NEW CCameraControlState_Normal(this));

	m_bFollow = true;	// �Ǐ]���邩�ǂ���
	m_bRockON = false;	// ���b�N�I�����邩

	// �J�������[�V�����̈ʒu���Z�b�g
	if (m_pCameraMotion != nullptr)
	{
		m_pCameraMotion->SetPosition(0.0f);
	}

	switch (mode)
	{
	case CScene::MODE_NONE:
		break;

	case CScene::MODE_TITLE:
		ResetTitle();
		break;

	case CScene::MODE_TUTORIAL:
		ResetGame();
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_GAMETUTORIAL:
		ResetGame();
		break;

	case CScene::MODE_RESULT:
		ResetResult();
		break;

	case CScene::MODE_RANKING:
		ResetRanking();
		break;

	case CScene::MODE_MAX:
		break;

	default:
		break;
	}
	m_AutoMovingPosR = m_posR;


	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(45.0f),
								(float)m_viewport.Width / (float)m_viewport.Height,
								10.0f,		// ���s���̐���
								1500000.0f);	// ���s���̐���

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	// ���_�̑������
	SetCameraV();
}

//==========================================================================
// �Q�[���̃��Z�b�g
//==========================================================================
void CCamera::ResetGame()
{
	m_posR = MyLib::Vector3(0.0f, 200.0f, 0.0f);				// �����_(�������ꏊ)
	m_posV = MyLib::Vector3(0.0f, 300.0f, m_posR.z + -400.0f);	// ���_(�J�����̈ʒu)
	m_posVDest = m_posV;									// �ڕW�̎��_
	m_posRDest = m_posR;									// �ڕW�̒����_
	m_vecU = MyLib::Vector3(0.0f, 1.0f, 0.0f);				// ������x�N�g��
	m_move = MyLib::Vector3(0.0f, 0.0f, 0.0f);				// �ړ���
	m_rot = DEFAULT_GAMEROT;								// ����
	m_rotOrigin = m_rot;									// ���̌���
	m_rotDest = DEFAULT_GAMEROT;							// �ڕW�̌���
	m_rotVDest = m_rot;										// �ڕW�̎��_�̌���
	m_TargetPos = MyLib::Vector3(0.0f, 0.0f, 0.0f);			// �ڕW�̈ʒu
	m_fDistance = START_CAMERALEN;							// ����
	m_fDestDistance = START_CAMERALEN;						// �ڕW�̋���
	m_fOriginDistance = START_CAMERALEN;					// ���̋���
	m_fDiffHeight = 0.0f;									// �����̍���
	m_fDiffHeightSave = 0.0f;								// �����̍����ۑ��p
	m_fDiffHeightDest = 0.0f;								// �ڕW�̍����̍���
	m_state = CAMERASTATE_NONE;								// ���
	m_nCntState = 0;							// ��ԃJ�E���^�[
	m_nCntDistance = 0;							// �����J�E���^�[
	m_nOriginCntDistance = 0;					// ���̋����J�E���^�[
	m_fDistanceCorrection = 0;					// �����̊����␳�W��
	m_fDistanceDecrementValue = 0.0f;			// �����̌����W��
	m_fHeightMaxDest = 0.0f;					// �J�����̍ő卂���̖ڕW
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CCamera::ResetBoss()
{
	ResetGame();
}

//==========================================================================
//  �X�N���[�����̔���
//==========================================================================
bool CCamera::IsOnScreen(const MyLib::Vector3 pos)
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
MyLib::Vector3 CCamera::GetScreenPos(const MyLib::Vector3 pos)
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
// �^�C�g���̃��Z�b�g
//==========================================================================
void CCamera::ResetTitle()
{
	m_posV = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// ���_(�J�����̈ʒu)
	m_posR = TITLE_POSR_DEST;	// �����_(�������ꏊ)
	m_posVDest = m_posV;								// �ڕW�̎��_
	m_posRDest = m_posR;								// �ڕW�̒����_
	m_vecU = MyLib::Vector3(0.0f, 1.0f, 0.0f);				// ������x�N�g��
	m_move = MyLib::Vector3(0.0f, 0.0f, 0.0f);				// �ړ���
	m_rot = DEFAULT_TITLEROT;						// ����
	m_rotVDest = m_rot;									// �ڕW�̎��_�̌���
	m_TargetPos = m_posR;								// �ڕW�̈ʒu
	m_fDistance = DEFAULT_TITLELEN;						// ����
	m_fDestDistance = DEFAULT_TITLELEN;					// �ڕW�̋���
	m_fOriginDistance = DEFAULT_TITLELEN;				// ���̋���
	m_fDiffHeight = 0.0f;								// �����̍���
	m_fDiffHeightSave = 0.0f;							// �����̍����ۑ��p
	m_fDiffHeightDest = 0.0f;							// �ڕW�̍����̍���
	m_state = CAMERASTATE_NONE;							// ���
	m_nCntState = 0;									// ��ԃJ�E���^�[
	m_nCntDistance = 0;									// �����J�E���^�[
	m_nOriginCntDistance = 0;							// ���̋����J�E���^�[
	m_fDistanceCorrection = 0;							// �����̊����␳�W��
	m_fDistanceDecrementValue = 0.0f;					// �����̌����W��
	m_fHeightMaxDest = 0.0f;							// �J�����̍ő卂���̖ڕW
	m_bFollow = false;									// �Ǐ]�t���O
}

//==========================================================================
// ���U���g���Z�b�g
//==========================================================================
void CCamera::ResetResult()
{
	m_posR = MyLib::Vector3(76329.0f, 440.0f, 2989.0f);				// �����_(�������ꏊ)
	m_posV = MyLib::Vector3(76326.0f, 545.27, 2017.0f);	// ���_(�J�����̈ʒu)
	m_posVDest = m_posV;									// �ڕW�̎��_
	m_posRDest = m_posR;									// �ڕW�̒����_
	m_vecU = MyLib::Vector3(0.0f, 1.0f, 0.0f);				// ������x�N�g��
	m_move = MyLib::Vector3(0.0f, 0.0f, 0.0f);				// �ړ���
	m_rot = DEFAULT_RESULTROT;						// ����
	m_rotVDest = m_rot;									// �ڕW�̎��_�̌���
	m_TargetPos = m_posR;								// �ڕW�̈ʒu
	m_fDistance = DEFAULT_TITLELEN;						// ����
	m_fDestDistance = DEFAULT_TITLELEN;					// �ڕW�̋���
	m_fOriginDistance = DEFAULT_TITLELEN;				// ���̋���
	m_fDiffHeight = 0.0f;								// �����̍���
	m_fDiffHeightSave = 0.0f;							// �����̍����ۑ��p
	m_fDiffHeightDest = 0.0f;							// �ڕW�̍����̍���
	m_state = CAMERASTATE_NONE;							// ���
	m_nCntState = 0;									// ��ԃJ�E���^�[
	m_nCntDistance = 0;									// �����J�E���^�[
	m_nOriginCntDistance = 0;							// ���̋����J�E���^�[
	m_fDistanceCorrection = 0;							// �����̊����␳�W��
	m_fDistanceDecrementValue = 0.0f;					// �����̌����W��
	m_fHeightMaxDest = 0.0f;							// �J�����̍ő卂���̖ڕW
	m_bFollow = true;									// �Ǐ]�t���O
}

//==========================================================================
// �����L���O�̃��Z�b�g
//==========================================================================
void CCamera::ResetRanking()
{
	
	m_vecU = MyLib::Vector3(0.0f, 1.0f, 0.0f);				// ������x�N�g��
	m_move = MyLib::Vector3(0.0f, 0.0f, 0.0f);				// �ړ���
	m_TargetPos = RANKING_POSR_DEST;		// �ڕW�̈ʒu
	m_fDistance = DEFAULT_RANKINGLEN;						// ����
	m_fDestDistance = DEFAULT_RANKINGLEN;					// �ڕW�̋���
	m_fOriginDistance = DEFAULT_RANKINGLEN;					// ���̋���
	m_fDiffHeight = 0.0f;								// �����̍���
	m_fDiffHeightSave = 0.0f;							// �����̍����ۑ��p
	m_fDiffHeightDest = 0.0f;							// �ڕW�̍����̍���
	m_state = CAMERASTATE_NONE;							// ���
	m_nCntState = 0;									// ��ԃJ�E���^�[
	m_nCntDistance = 0;									// �����J�E���^�[
	m_nOriginCntDistance = 0;							// ���̋����J�E���^�[
	m_fDistanceCorrection = 0;							// �����̊����␳�W��
	m_fDistanceDecrementValue = 0.0f;					// �����̌����W��
	m_fHeightMaxDest = 0.0f;							// �J�����̍ő卂���̖ڕW

	m_rot = DEFAULT_RANKINGROT;							// ����
	m_rotVDest = m_rot;									// �ڕW�̎��_�̌���

	m_posR = RANKING_POSR_DEST;					// �����_(�������ꏊ)
	m_posV = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// ���_(�J�����̈ʒu)
	m_posVDest = m_posV;								// �ڕW�̎��_
	m_posRDest = m_posR;								// �ڕW�̒����_

	// �J�������[�v
	WarpCamera(m_posR);

	m_bFollow = false;									// �Ǐ]�t���O
}

//==========================================================================
// �J�����̈ʒu�擾
//==========================================================================
MyLib::Vector3 CCamera::GetPositionV() const
{
	return m_posV;
}

//==========================================================================
// �J�����̒����_�擾
//==========================================================================
MyLib::Vector3 CCamera::GetPositionR() const
{
	return m_posR;
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CCamera::SetRotation(const MyLib::Vector3 rot)
{
	m_rot = rot;
}

//==========================================================================
// �����擾
//==========================================================================
MyLib::Vector3 CCamera::GetRotation() const
{
	return m_rot;
}

//==========================================================================
// �����̌����ݒ�
//==========================================================================
void CCamera::SetOriginRotation(const MyLib::Vector3 rot)
{
	m_rotOrigin = DEFAULT_GAMEROT + rot;
}

//==========================================================================
// �ڕW�̌����ݒ�
//==========================================================================
void CCamera::SetDestRotation(const MyLib::Vector3& rot)
{
	m_rotDest = rot;
}

//==========================================================================
// �ڕW�̌����擾
//==========================================================================
MyLib::Vector3 CCamera::GetDestRotation()
{
	return m_rotDest;
}

//==========================================================================
// �ڕW�̈ʒu�ݒ�
//==========================================================================
void CCamera::SetTargetPosition(const MyLib::Vector3 pos)
{
	// �ڕW�̈ʒu
	m_TargetPos = pos;
}

//==========================================================================
// �ڕW�̈ʒu�擾
//==========================================================================
MyLib::Vector3 CCamera::GetTargetPosition()
{
	return m_TargetPos;
}

//==========================================================================
// ���b�N�I���̈ʒu�ݒ�
//==========================================================================
void CCamera::SetRockOnPosition(const MyLib::Vector3 pos)
{
	m_RockOnPos = pos;
}

//==========================================================================
// ���b�N�I���̈ʒu�擾
//==========================================================================
MyLib::Vector3 CCamera::GetRockOnPosition()
{
	return m_RockOnPos;
}

//==========================================================================
// ���b�N�I���̉��������ݒ�
//==========================================================================
void CCamera::SetRockOnDistance(const float distance)
{
	m_fRockOnDistance = distance;
}

//==========================================================================
// ���b�N�I���ݒ�
//==========================================================================
void CCamera::SetRockOn(const MyLib::Vector3 pos, bool bSet)
{
	if (bSet)
	{
		SetStateCamraR(DEBUG_NEW CStateCameraR_RockOn());
		SetControlState(DEBUG_NEW CCameraControlState_RockOn(this));
	}
	else
	{
		SetStateCamraR(DEBUG_NEW CStateCameraR());
		// ����̏�Ԑݒ�
		SetControlState(DEBUG_NEW CCameraControlState_Normal(this));
	}

	m_RockOnPos = pos;
	m_bRockON = bSet;
}

//==========================================================================
// �Ǐ]�ڕW�̌����ݒ�
//==========================================================================
void CCamera::SetTargetRotation(const MyLib::Vector3 rot)
{
	// �ڕW�̌���
	m_TargetRot = rot;
}

//==========================================================================
// �Ǐ]�ڕW�̌����擾
//==========================================================================
MyLib::Vector3 CCamera::GetTargetRotation()
{
	return m_TargetRot;
}

//==========================================================================
// ���ɂȂ�J�����̋����ݒ�
//==========================================================================
void CCamera::SetOriginDistance(float fDistance)
{
	m_fOriginDistance = fDistance;
}

//==========================================================================
// ���ɂȂ�J�����̋����擾
//==========================================================================
float CCamera::GetOriginDistance()
{
	return m_fOriginDistance;
}

//==========================================================================
// �Ǐ]����v���C���[�̃C���f�b�N�X�ԍ��ݒ�
//==========================================================================
void CCamera::SetPlayerChaseIndex(int nIdx)
{
	m_nChasePlayerIndex = nIdx;
}

//==========================================================================
// �Ǐ]����v���C���[�̃C���f�b�N�X�ԍ��擾
//==========================================================================
int CCamera::GetPlayerChaseIndex()
{
	return m_nChasePlayerIndex;
}

//==========================================================================
// �Ǐ]�̔���ݒ�
//==========================================================================
void CCamera::SetEnableFollow(bool bFollow)
{
	m_bFollow = bFollow;
}

//==========================================================================
// �Ǐ]��Ԏ擾
//==========================================================================
bool CCamera::IsFollow()
{
	return m_bFollow;
}

//==========================================================================
// ���b�N�I����Ԑݒ�
//==========================================================================
void CCamera::SetRockOnState(RockOnState state, float height)
{
	if (state == ROCKON_COUNTER)
	{
		// �ڕW�̊p�x�����߂�
		m_rotDest.y =
			D3DX_PI +
			atan2f((m_TargetPos.x - m_RockOnPos.x), (m_TargetPos.z - m_RockOnPos.z)) +
			((ROTDISTANCE_ROCKON * 2.0f) * (m_RockOnDir - 1)) + ROTDISTANCE_ROCKON;
		UtilFunc::Transformation::RotNormalize(m_rotDest);
		
		m_rotDest.y -= ROTDISTANCE_COUNTER.y;
		m_rotDest.z = D3DX_PI * 0.02f;

		height -= 200.0f;
		if (height <= 0.0f) {
			height = 0.0f;
		}

		float len = LENGTH_COUNTER + height;

		m_fOriginDistance = len;	// ���̋���

		// �ڕW�̒����ݒ�
		SetLenDest(len, 48, 4.0f, 0.25f);
	}
	else
	{
		m_rotDest.z = DEFAULT_GAMEROT.z;
		m_fOriginDistance = START_CAMERALEN;	// ���̋���
	}
	UtilFunc::Transformation::RotNormalize(m_rotDest);

	m_stateRockOn = state;
}

//==========================================================================
// �����_�̏�Ԑݒ�
//==========================================================================
void CCamera::SetStateCamraR(CStateCameraR* state)
{
	delete m_pStateCameraR;
	m_pStateCameraR = state;
}

//==========================================================================
// ���_�̏�Ԑݒ�
//==========================================================================
void CCamera::SetStateCameraV(CStateCameraV* state)
{
	delete m_pStateCameraV;
	m_pStateCameraV = state;
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
// �ʏ�̒����_�ݒ�
//==========================================================================
void CStateCameraR::SetCameraR(CCamera* pCamera)
{
	MyLib::Vector3 targetpos = pCamera->GetTargetPosition();
	MyLib::Vector3 rot = pCamera->GetRotation();

	MyLib::Vector3 posdest;
	posdest.x = (targetpos.x + sinf(rot.y) * DISATNCE_POSR_PLAYER);
	posdest.z = (targetpos.z + cosf(rot.y) * DISATNCE_POSR_PLAYER);
	pCamera->SetPositionRDest(posdest);
}

//==========================================================================
// ���C���̈ʒu����
//==========================================================================
void CStateCameraV::LimitPos(CCamera* pCamera)
{
	return;
	MyLib::Vector3 posVDest = pCamera->GetPositionVDest();

	if (posVDest.LengthXZ() > mylib_const::RADIUS_STAGE)
	{// �␳
		posVDest = posVDest.Normal() * mylib_const::RADIUS_STAGE;
	}
	if (posVDest.LengthXZ() > mylib_const::RADIUS_STAGE)
	{// �␳
		posVDest = posVDest.Normal() * mylib_const::RADIUS_STAGE;
	}

	pCamera->SetPositionVDest(posVDest);
}

//==========================================================================
// �����ɂ�鋗���ݒ�
//==========================================================================
void CStateCameraV::Distance(CCamera* pCamera)
{
	pCamera->SetDistance(GetDistance(pCamera, UPDISTANCE_MULTIPLY));
}

//==========================================================================
// �����擾
//==========================================================================
float CStateCameraV::GetDistance(CCamera* pCamera, const float fMultiply)
{
	// ����
	float ratio = (pCamera->GetPositionR().y - 200.0f) / MAX_AUTODISTANCEHEIGHT;
	ratio = UtilFunc::Transformation::Clamp(ratio, 0.0f, 1.0f);
	pCamera->SetAutoDistanceDest(1200.0f * ratio + MIN_DISNTANCE);

	float Value = pCamera->GetDistance() + (pCamera->GetAutoDistanceDest() - pCamera->GetDistance()) * fMultiply;
	return Value;
}

//==========================================================================
// �����X�e�[�W�̈ʒu����
//==========================================================================
void CStateCameraV_Enhance::LimitPos(CCamera* pCamera)
{
	CStateCameraV::LimitPos(pCamera);

	MyLib::Vector3 posVDest = pCamera->GetPositionVDest();

	CListManager<CLimitArea> areaList = CLimitArea::GetListObj();
	CLimitArea* pArea = nullptr;

	float radius = 50.0f;

	while (areaList.ListLoop(&pArea))
	{
		CLimitArea::sLimitEreaInfo info = pArea->GetLimitEreaInfo();

		// ��l�̕ǂ�K�p
		//if (posVDest.x + radius >= info.fMaxX) { posVDest.x = info.fMaxX - radius; }
		//if (posVDest.x - radius <= info.fMinX) { posVDest.x = info.fMinX + radius; }
		if (posVDest.z + radius >= info.fMaxZ) { posVDest.z = info.fMaxZ - radius; }
		//if (posVDest.z - radius <= info.fMinZ) { posVDest.z = info.fMinZ + radius; }
	}
	pCamera->SetPositionVDest(posVDest);

}

//==========================================================================
// �����ɂ�鋗���ݒ�
//==========================================================================
void CStateCameraV_Distance::Distance(CCamera* pCamera)
{
	if (pCamera->GetDistanceCnt() > 0) { return; }
	float defdistance = GetDistance(pCamera, UPDISTANCE_MULTIPLY);
	float distanceDecrementValue = pCamera->GetDistanceDecrementValue();

	float distance = UtilFunc::Correction::EasingLinear(pCamera->GetDistance(), defdistance, m_fMultiPly);

	pCamera->SetDistance(distance);

	// �����m�F
	if (m_fMultiPly >= 1.0f)
	{
		pCamera->SetStateCameraV(DEBUG_NEW CStateCameraV);
	}
	else m_fMultiPly += DISTANCE_TIMER;
}

//==========================================================================
// �����ɂ�鋗���ݒ�
//==========================================================================
void CStateCameraV_Goal::LimitPos(CCamera* pCamera)
{

}

//==========================================================================
// �����ݒ�
//==========================================================================
void CStateCameraV_Goal::Distance(CCamera* pCamera)
{
	SetMotion(pCamera);
}

//==========================================================================
// ���[�V�����ݒ�
//==========================================================================
void CStateCameraV_Goal::SetMotion(CCamera* pCamera)
{
	// ���
	CCameraMotion* pMotion = pCamera->GetMotion();
	if (pMotion->GetNowKeyIdx() + 1 == pMotion->GetNowKeyMax())
	{
		pMotion->SetEnablePause(true);
	}
}

//==========================================================================
// ���b�N�I�����̒����_�ݒ�
//==========================================================================
void CStateCameraR_RockOn::SetCameraR(CCamera* pCamera)
{
	MyLib::Vector3 targetpos = pCamera->GetTargetPosition();
	MyLib::Vector3 rot = pCamera->GetRotation();

	MyLib::Vector3 posdest;
	posdest.x = (targetpos.x);
	posdest.z = (targetpos.z);
	pCamera->SetPositionRDest(posdest);
}

//==========================================================================
// �F�蒆�̒����_�ݒ�
//==========================================================================
void CStateCameraR_Prayer::SetCameraR(CCamera* pCamera)
{
	MyLib::Vector3 targetpos = pCamera->GetTargetPosition();
	pCamera->SetPositionRDest(targetpos);
}



//==========================================================================
// ���̑��쏈��
//==========================================================================
void CCameraControlState::MoveCamera(CCamera* pCamera)
{
	// �Q�[���p�b�h���擾
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	MyLib::Vector3 moverot = pCamera->GetMoveRot();
	MyLib::Vector3 rot = pCamera->GetRotation();

	if (pCamera->IsRotationY())
	{
		moverot.y += pInputGamepad->GetStickMoveR(0).x * ROT_MOVE_STICK_Y;
	}

	if (rot.z > MIN_STICKROT &&
		(pCamera->IsRotationZ() || pInputGamepad->GetStickMoveR(0).y < 0.0f))
	{
		moverot.z += pInputGamepad->GetStickMoveR(0).y * ROT_MOVE_STICK_Z;
	}
	else if (rot.z <= MIN_STICKROT && pInputGamepad->GetStickMoveR(0).y > 0.0f)
	{
		moverot.z += pInputGamepad->GetStickMoveR(0).y * ROT_MOVE_STICK_Z;
	}

	// �ړ�����
	rot += moverot;

	UtilFunc::Correction::InertiaCorrection(moverot.y, 0.0f, 0.25f);
	UtilFunc::Correction::InertiaCorrection(moverot.z, 0.0f, 0.25f);

	// ��]�ړ��ʐݒ�
	pCamera->SetRotation(rot);
	pCamera->SetMoveRot(moverot);
}

//==========================================================================
// ���b�N�I���̑��쏈��
//==========================================================================
void CCameraControlState_RockOn::MoveCamera(CCamera* pCamera)
{
	// �Q�[���p�b�h���擾
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	MyLib::Vector3 moverot = pCamera->GetMoveRot();
	MyLib::Vector3 rot = pCamera->GetRotation();
	MyLib::Vector3 rotdest = pCamera->GetDestRotation();

	// �ڕW�̌���
	float fRotDiff = rotdest.z - rot.z;
	UtilFunc::Transformation::RotNormalize(fRotDiff);
	rot.z += fRotDiff * 0.08f;
	UtilFunc::Transformation::RotNormalize(rot.z);
	pCamera->SetRotation(rot);

	// ���b�N�I����ԕʏ���
	(pCamera->*(CCamera::m_RockOnStateFunc[pCamera->GetRockOnState()]))();
}

//==========================================================================
// �F�菀���̑��쏈��
//==========================================================================
void CCameraControlState_BeforePrayer::MoveCamera(CCamera* pCamera)
{
	// ���擾
	MyLib::Vector3 rot = pCamera->GetRotation();

	// �ڕW�̌���
	MyLib::Vector3 rotDiff = DEFAULT_GAMEROT - rot;
	UtilFunc::Transformation::RotNormalize(rotDiff);

	rot += rotDiff * 0.08f;
	UtilFunc::Transformation::RotNormalize(rot);

	// �����ݒ�
	pCamera->SetRotation(rot);
}

//==========================================================================
// �F��̑��쏈��
//==========================================================================
void CCameraControlState_Prayer::MoveCamera(CCamera* pCamera)
{
	// ���擾
	MyLib::Vector3 rot = pCamera->GetRotation();
	MyLib::Vector3 targetpos = pCamera->GetTargetPosition();

	// �ڕW�l
	//const MyLib::Vector3& rotdest = pCamera->GetDestRotation();
	const MyLib::Vector3& rotdest = ROTATION_PRAYER;

	// �ڕW�̌���
	MyLib::Vector3 rotDiff = rotdest - rot;
	UtilFunc::Transformation::RotNormalize(rotDiff);

	rot += rotDiff * 0.08f;
	UtilFunc::Transformation::RotNormalize(rot);

	// �����ݒ�
	pCamera->SetLenDest(170.0f, 10, 50.0f, 0.1f);

	// �����ݒ�
	pCamera->SetRotation(rot);
}