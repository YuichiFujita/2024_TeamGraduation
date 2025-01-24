//=============================================================================
// 
// �J�������� [camera.cpp]
// Author : ���n�Ή�
// Adder  : ���c�E��
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
// �Ǐ]�f�o�b�O�\����ON/OFF
//==========================================================================
#if 0
#define FOLLOW_DEBUG
#endif

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const MyLib::Vector3 INIT_VECU = MyLib::Vector3(0.0f, 1.0f, 0.0f);	// ������x�N�g�������l
	const CCamera::SSide INIT_SIDE = CCamera::SSide(0.0f, 0.0f);		// ���E�ő�ʒu�����l
	const D3DXCOLOR COL_LIGHT = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);		// ���C�g�g�U��
	const float MIN_NEAR	= 10.0f;		// ��O�̕`�搧��
	const float MAX_FAR		= 1500000.0f;	// ���̕`�搧��
	const float MIN_DIS		= 50.0f;		// �ŏ�����
	const float REV_SWING	= 0.001f;		// �J�����h�ꎞ�̃J���������␳�W��

	namespace dressup
	{
		const MyLib::Vector3 POSV	= MyLib::Vector3(0.0f, 70.0f, -284.0f);	// ���_
		const MyLib::Vector3 POSR	= MyLib::Vector3(0.0f, 70.0f, 0.0f);	// �����_
		const MyLib::Vector3 VECU	= MyLib::Vector3(0.0f, 1.0f, 0.0f);		// ������x�N�g��
		const float VIEW_ANGLE		= D3DXToRadian(45.0f);					// ����p
	}

	namespace none
	{
		const MyLib::Vector3 INIT_POSR	= VEC3_ZERO;		// �����_�̏����l
		const MyLib::Vector3 INIT_ROT	= VEC3_ZERO;		// �����̏����l
		const float INIT_DISTANCE	= 800.0f;				// �����̏����l
		const float INIT_VIEWANGLE	= D3DXToRadian(45.0f);	// ����p�̏����l
	}

	namespace game
	{
		const float END_TIME = 1.4f;	// �Q�[���J������ԑJ�ڂ̎���
		const float MIN_TIME = 0.5f;	// �Q�[���J������ԑJ�ڂ̍ŏ�����
	}

	namespace follow
	{
		const MyLib::Vector3 INIT_ROT	= MyLib::Vector3(0.0f, 0.0f, -0.32f);	// �����̏����l
		const float INIT_VIEWANGLE		= D3DXToRadian(30.0f);	// ����p�̏����l
		const float DEST_POSR_BOXSIZE	= 50.0f;	// �ڕW�����_�𓮂����Ȃ��͈�

		const float REV_SIDE		= 0.35f;		// �ő卶�E���W�̕␳�W��
		const float REV_POSR		= 0.05f;		// �����_�̕␳�W��
		const float REV_VIEWANGLE	= 0.1f;			// ����p�̕␳�W��
		const float MAX_DIS	= 2500.0f - MIN_DIS;	// �Ǐ]�J�����̍ő勗��

		const float POSRY_START	= 550.0f;	// �J����Y�����_�̊J�n���W
		const float POSRY_END	= 320.0f;	// �J����Y�����_�̏I�����W
		const float POSRZ_START	= -1600.0f;	// �J����Z�����_�̊J�n���W
		const float POSRZ_END	= -100.0f;	// �J����Z�����_�̏I�����W
	}

	namespace out
	{
		const MyLib::Vector3 ROT = follow::INIT_ROT;	// �J�����̌���
		const float POSRY = follow::POSRY_END;			// �J����Y�����_�̍��W
		const float POSRZ = follow::POSRZ_END - 160.0f;	// �J����Z�����_�̍��W
		const float DIS = follow::MAX_DIS + 160.0f;		// �J�����̋���
	}

	namespace Reset
	{
		const MyLib::Vector3 POSITION[CScene::MODE::MODE_MAX] = // �ʒu
		{
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// NONE
			MyLib::Vector3(0.0f, 50.0f, 0.0f),	// �^�C�g��
			MyLib::Vector3(0.0f, 173.33f, -770.77f),	// �G���g���[
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// �`���[�g���A��
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// �Q�[��
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// ���U���g
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// �����L���O
		};

		const MyLib::Vector3 ROTATION[CScene::MODE::MODE_MAX] = // ����
		{
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// NONE
			MyLib::Vector3(0.0f, 0.0f, -0.15f),	// �^�C�g��
			MyLib::Vector3(0.0f, 0.0f, -0.04f),	// �G���g���[
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// �`���[�g���A��
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// �Q�[��
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// ���U���g
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// �����L���O
		};

		const float DISTANCE[CScene::MODE::MODE_MAX] = // ����
		{
			0.0f,	// NONE
			800.0f,	// �^�C�g��
			800.0f,	// �G���g���[
			800.0f,	// �`���[�g���A��
			800.0f,	// �Q�[��
			800.0f,	// ���U���g
			800.0f,	// �����L���O
		};
	}
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
// ��Ԋ֐�
CCamera::STATE_FUNC CCamera::m_StateFuncList[] =
{
	&CCamera::UpdateNoneState,		// �ʏ�J�����X�V
	&CCamera::UpdateFollowState,	// �Ǐ]�J�����X�V
	&CCamera::UpdateOutFieldState,	// �O��J�����X�V
};

// ���Z�b�g�֐�
CCamera::RESET_FUNC CCamera::m_ResetFuncList[] =
{
	&CCamera::ResetNoneState,		// �ʏ�J�������Z�b�g
	&CCamera::ResetFollowState,		// �Ǐ]�J�������Z�b�g
	&CCamera::ResetOutFieldState,	// �O��J�������Z�b�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCamera::CCamera()
{
	// ��{���̃N���A
	m_pDebugControll	= nullptr;			// �f�o�b�O���
	m_pCameraMotion		= nullptr;			// �J�������[�V�������
	m_pLight			= nullptr;			// �f�B���N�V���i�����C�g���
	m_posV				= VEC3_ZERO;		// ���_
	m_posVDest			= VEC3_ZERO;		// �ڕW���_
	m_posR				= VEC3_ZERO;		// �����_
	m_posRDest			= VEC3_ZERO;		// �ڕW�����_
	m_posROrigin		= VEC3_ZERO;		// ���_�����_
	m_vecU				= INIT_VECU;		// ������x�N�g��
	m_rot				= VEC3_ZERO;		// ����
	m_rotDest			= VEC3_ZERO;		// �ڕW����
	m_rotOrigin			= VEC3_ZERO;		// ���_����
	m_fDistance			= 0.0f;				// ����
	m_fDestDistance		= 0.0f;				// �ڕW����
	m_fOriginDistance	= 0.0f;				// ���_����
	m_fViewAngle		= 0.0f;				// ����p
	m_fDestViewAngle	= 0.0f;				// �ڕW����p
	m_side				= INIT_SIDE;		// ���E�ő�ʒu
	m_sideDest			= INIT_SIDE;		// �ڕW���E�ő�ʒu
	m_state				= STATE_NONE;		// ���
	m_bMotion			= false;			// ���[�V����������
	m_bOldWithBall		= false;			// �O��̊O��{�[�������t���O
	m_fTransTime		= 0.0f;				// �Q�[���J������ԑJ�ڂ̎���
	m_fEndTime			= 0.0f;				// �Q�[���J������ԑJ�ڂ̏I������
	m_transStartPoint	= SCameraPoint();	// �Q�[���J������ԑJ�ڂ̊J�n�|�C���g
	b = false;

	// �}�g���b�N�X���̃N���A
	D3DXMatrixIdentity(&m_mtxProjection);	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMatrixIdentity(&m_mtxView);			// �r���[�}�g���b�N�X

	// �r���[�|�[�g���̃N���A
	m_viewport.X		= 0;	// ��ʂ̍���X���W
	m_viewport.Y		= 0;	// ��ʂ̍���Y���W
	m_viewport.Width	= 0;	// ��ʂ̕�
	m_viewport.Height	= 0;	// ��ʂ̍���
	m_viewport.MinZ		= 0.0f;
	m_viewport.MaxZ		= 0.0f;

	// �J�����h����̃N���A
	m_swing.shiftPos	 = VEC3_ZERO;	// �ʒu�����
	m_swing.fShiftAngle	 = 0.0f;		// �ʒu�����炷�p�x
	m_swing.fShiftLength = 0.0f;		// �ʒu�����炷����
	m_swing.fSubAngle	 = 0.0f;		// ���炷�p�x�̌��Z��
	m_swing.fSubLength	 = 0.0f;		// ���炷�����̌��Z��

	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_StateFuncList) == STATE::STATE_MAX, "ERROR : State Count Mismatch");
	static_assert(NUM_ARRAY(m_ResetFuncList) == STATE::STATE_MAX, "ERROR : State Count Mismatch");
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

	// �J�����̃��Z�b�g
	Reset();

	// �f�B���N�V���i�����C�g�̐���
	m_pLight = CLightDir::Create();
	if (m_pLight == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �I�u�W�F�N�g��ނ𖢐ݒ�ɂ���
	m_pLight->SetType(CObject::TYPE::TYPE_NONE);

	// �g�U���̐F��ݒ�
	m_pLight->SetDiffuse(COL_LIGHT);

	// �J�������[�V�����̐���
	m_pCameraMotion = CCameraMotion::Create();
	if (m_pCameraMotion == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

#if _DEBUG
	// �f�o�b�O��p�̐���
	m_pDebugControll = DEBUG_NEW CCamera_Debug(this);
	if (m_pDebugControll == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}
#endif

	return S_OK;
}

//==========================================================================
// �r���[�|�[�g�̐ݒ�
//==========================================================================
void CCamera::SetViewPort(const MyLib::Vector3& pos, const D3DXVECTOR2& size)
{
	// �������̐ݒ�
	m_viewport.X = (DWORD)pos.x;		// ��ʂ̍���X���W
	m_viewport.Y = (DWORD)pos.y;		// ��ʂ̍���Y���W
	m_viewport.Width  = (DWORD)size.x;	// ��ʂ̕�
	m_viewport.Height = (DWORD)size.y;	// ��ʂ̍���

	// ���̏�����
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

	// �J�������[�V�����̏I��
	SAFE_UNINIT(m_pCameraMotion);

	// �f�o�b�O��p�̏I��
	SAFE_DELETE(m_pDebugControll);
}

//==========================================================================
// �J�����̍X�V����
//==========================================================================
void CCamera::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pDebugControll != nullptr)
	{
		// �f�o�b�O��p�̍X�V
		m_pDebugControll->Update();
	}

	// �|�[�Y���̏ꍇ������
	CManager* pManager = GET_MANAGER;	// �}�l�[�W���[
	if (pManager->GetPause()->IsPause()) { return; }

	if (m_StateFuncList[m_state] != nullptr && !m_bMotion)
	{ // ��ԍX�V�֐������銎���[�V�������ł͂Ȃ��ꍇ

		// ��ԕʏ���
		(this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// �J�����h��̍X�V
	UpdateSwing();

	if (m_pLight != nullptr)
	{
		// ���C�g�̍X�V
		m_pLight->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	if (m_pCameraMotion != nullptr)
	{
		// �J�������[�V�����̍X�V
		m_pCameraMotion->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

#ifdef _DEBUG
	// �J���������_�̎擾
	MyLib::Vector3 posScreen = UtilFunc::Transformation::CalcScreenToXZ
	(
		CInputMouse::GetInstance()->GetPosition(),	// �}�E�X���W
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),	// �X�N���[���T�C�Y
		m_mtxView,		// �r���[�}�g���b�N�X
		m_mtxProjection	// �v���W�F�N�V�����}�g���b�N�X
	);

	// �J�������̃e�L�X�g�`��
	GET_MANAGER->GetDebugProc()->Print
	(
		"\n---------------- �J������� ----------------\n"
		"�y�����z[X�F%f Y�F%f Z�F%f]\n"
		"�y�����z[%f]\n"
		"�y���_�z[X�F%f Y�F%f Z�F%f]\n"
		"�y�����_�z[X�F%f Y�F%f Z�F%f]\n"
		"�y�����_�z[X�F%f Y�F%f Z�F%f]\n",
		m_rot.x, m_rot.y, m_rot.z,
		m_fDistance,
		m_posV.x, m_posV.y, m_posV.z,
		m_posR.x, m_posR.y, m_posR.z,
		posScreen.x, posScreen.y, posScreen.z
	);
#endif
}

//==========================================================================
// �J�����̐ݒ菈�� (�ʏ�)
//==========================================================================
void CCamera::SetCamera()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X���

	// �r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&m_viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	float fAspect = (float)m_viewport.Width / (float)m_viewport.Height;	// �A�X�y�N�g��
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,	// �v���W�F�N�V�����}�g���b�N�X
		m_fViewAngle,		// ����p
		fAspect,			// �A�X�y�N�g��
		MIN_NEAR,			// ��O�̕`�搧��
		MAX_FAR				// ���̕`�搧��
	);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH
	(
		&m_mtxView,	// �r���[�}�g���b�N�X
		&m_posV,	// ���_
		&m_posR,	// �����_
		&m_vecU		// ������x�N�g��
	);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// �X�|�b�g���C�g�x�N�g���̍X�V
	UpdateSpotLightVec(m_posR, m_posV);
}

//==========================================================================
// �J�����̐ݒ菈�� (�����ւ�)
//==========================================================================
void CCamera::SetCameraDressup()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X���

	// �r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&m_viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	float fAspect = (float)m_viewport.Width / (float)m_viewport.Height;	// �A�X�y�N�g��
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,		// �v���W�F�N�V�����}�g���b�N�X
		dressup::VIEW_ANGLE,	// ����p
		fAspect,	// �A�X�y�N�g��
		MIN_NEAR,	// ��O�̕`�搧��
		MAX_FAR		// ���̕`�搧��
	);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH
	(
		&m_mtxView,		// �r���[�}�g���b�N�X
		&dressup::POSV,	// ���_
		&dressup::POSR,	// �����_
		&dressup::VECU	// ������x�N�g��
	);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// �X�|�b�g���C�g�x�N�g���̍X�V
	UpdateSpotLightVec(dressup::POSR, dressup::POSV);
}

//==========================================================================
// �J�������Z�b�g
//==========================================================================
void CCamera::Reset()
{
	if (m_StateFuncList[m_state] != nullptr)
	{ // ��ԃ��Z�b�g�֐�������ꍇ

		// ��ԕʏ���
		(this->*(m_ResetFuncList[m_state]))();
	}

	// �J�����h��̃��Z�b�g
	ResetSwing();

	if (m_pCameraMotion != nullptr)
	{
		// �J�������[�V�����̈ʒu���Z�b�g
		m_pCameraMotion->SetPosition(0.0f);
	}

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	float fAspect = (float)m_viewport.Width / (float)m_viewport.Height;	// �A�X�y�N�g��
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,	// �v���W�F�N�V�����}�g���b�N�X
		m_fViewAngle,		// ����p
		fAspect,			// �A�X�y�N�g��
		MIN_NEAR,			// ��O�̕`�搧��
		MAX_FAR				// ���̕`�搧��
	);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
}

//==========================================================================
// ���[�h�ʃ��Z�b�g
//==========================================================================
void CCamera::ResetByMode(CScene::MODE mode)
{
	// �J�������[�V�������Z�b�g
	m_pCameraMotion->SetFinish(true);

	// ���_���
	m_posR = Reset::POSITION[mode];			// �����_
	m_rot = Reset::ROTATION[mode];			// ����
	m_fDistance = Reset::DISTANCE[mode];	// ����
	m_fDestDistance = m_fDistance;			// �ڕW�̋���
	m_fOriginDistance = m_fDistance;		// ���̋���

	// ���
	m_state = STATE::STATE_NONE;

	// ���̑�
	m_fViewAngle = m_fDestViewAngle = none::INIT_VIEWANGLE;

	// �J�������[�v
	SetWarp(m_posR);
}

//==========================================================================
// �J�����h�ꃊ�Z�b�g
//==========================================================================
void CCamera::ResetSwing()
{
	// �J�����h�����������
	m_swing.shiftPos	 = VEC3_ZERO;	// �ʒu�����
	m_swing.fShiftAngle	 = 0.0f;		// �ʒu�����炷�p�x
	m_swing.fShiftLength = 0.0f;		// �ʒu�����炷����
	m_swing.fSubAngle	 = 0.0f;		// ���炷�p�x�̌��Z��
	m_swing.fSubLength	 = 0.0f;		// ���炷�����̌��Z��
}

//==========================================================================
// �J�����h��̐ݒ菈��
//==========================================================================
void CCamera::SetSwing(const SSwing& swing)
{
	// �����̃J�����h�����ݒ�
	m_swing = swing;
}

//==========================================================================
// �J�������[�v����
//==========================================================================
void CCamera::SetWarp(const MyLib::Vector3& pos)
{
	// �����_�̐ݒ�
	m_posR = pos;			// �����_
	m_posRDest = m_posR;	// �ڕW�̒����_

	// ���ʍ��W�ϊ��ɂ�鎋�_�̑��Έʒu�擾
	m_posV = CalcSpherePosition(m_posR, m_rot, -m_fDistance);	// ���_
	m_posVDest = m_posV;	// �ڕW�̎��_
}

//==========================================================================
// ��Ԃ̐ݒ菈��
//==========================================================================
void CCamera::SetState(const STATE state, const bool bReset)
{
	// ������Ԃ�ݒ�
	m_state = state;

	// ���Z�b�g�t���O��ON�Ȃ�J�������Z�b�g
	if (bReset) { Reset(); }
}

//==========================================================================
// ���C�g�g�U���̐ݒ菈��
//==========================================================================
void CCamera::SetLightDiffuse(const D3DXCOLOR& rCol)
{
	return m_pLight->SetDiffuse(rCol);
}

//==========================================================================
// ���C�g�g�U���̎擾����
//==========================================================================
D3DXCOLOR CCamera::GetLightDiffuse() const
{
	return m_pLight->GetLight().Diffuse;
}

//==========================================================================
// ���C�g�ڕW�g�U���̎擾����
//==========================================================================
D3DXCOLOR CCamera::GetDestLightDiffuse() const
{
	return COL_LIGHT;
}

//==========================================================================
// �ʏ��Ԃ̍X�V����
//==========================================================================
void CCamera::UpdateNoneState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����̐��K��
#if _DEBUG
	if (!m_pCameraMotion->IsEdit())
	{
		UtilFunc::Transformation::RotNormalize(m_rot);
	}
#else
	UtilFunc::Transformation::RotNormalize(m_rot);
#endif

	// �ڕW�����̕␳
	if (m_fDestDistance < MIN_DIS)	 { m_fDestDistance = MIN_DIS; }
	if (m_fOriginDistance < MIN_DIS) { m_fOriginDistance = MIN_DIS; }

	// ���ʍ��W�ϊ��ɂ��ڕW���_�̑��Έʒu�擾
	m_posV = m_posVDest = CalcSpherePosition(m_posR, m_rot, -m_fDistance);
}

//==========================================================================
// �Ǐ]��Ԃ̍X�V����
//==========================================================================
void CCamera::UpdateFollowState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �Q�[�����[�h�ȊO�ł͎g�p�ł��Ȃ�
	if (GET_MANAGER->GetMode() != CScene::MODE_GAME) { return; }

	// ���݂̊O��{�[�������t���O���擾
	bool bCurWithBall = IsPlayerOutWithBall();
	if (bCurWithBall && !m_bOldWithBall)
	{ // �O�삪�{�[�����������u�Ԃ̏ꍇ

		// ���݂̃J�����|�C���g���J�n�|�C���g�Ƃ��ĕۑ�
		m_transStartPoint.posR = m_posR;
		m_transStartPoint.posV = m_posV;
		m_transStartPoint.rot = m_rot;
		m_transStartPoint.fDistance = m_fDistance;

		// ��ԑJ�ڂ̌o�ߎ��Ԃ�������
		m_fTransTime = 0.0f;

		// �J�ڂ̏I�����Ԃ�ۑ�
		m_fEndTime = game::END_TIME;

		b = false;
	}

	if (bCurWithBall)
	{ // �O�삪�{�[���������Ă����ꍇ

		// ��ԑJ�ڂ̌o�ߎ��Ԃ����Z
		m_fTransTime += fDeltaTime * fSlowRate;

		// �J������ԑJ�ڂ̍X�V
		if (UpdateTrans(STATE::STATE_OUTFIELD))
		{ // �J�ڂ��������ꍇ

			// �O���Ԃɂ���
			SetState(STATE::STATE_OUTFIELD);

			// ��ԑJ�ڂ̌o�ߎ��Ԃ�������
			m_fTransTime = 0.0f;

			// �J�ڂ̏I�����Ԃ�������
			m_fEndTime = 0.0f;
		}
	}
	else
	{ // �O�삪�{�[���������Ă��Ȃ��ꍇ

		if (m_fEndTime > 0.0f)
		{ // ��Ԃ��J�ڂ������Ă����ꍇ

			// �I�����Ԃ�ݒ�
			if (!b)
			{
				// ���݂̃J�����|�C���g���J�n�|�C���g�Ƃ��ĕۑ�
				m_transStartPoint.posR = m_posR;
				m_transStartPoint.posV = m_posV;
				m_transStartPoint.rot = m_rot;
				m_transStartPoint.fDistance = m_fDistance;

				// 
				m_fEndTime = m_fTransTime;
				UtilFunc::Transformation::ValueNormalize(m_fEndTime, game::END_TIME, game::MIN_TIME);
				m_fTransTime = 0.0f;
				b = true;
			}

			// ��ԑJ�ڂ̌o�ߎ��Ԃ����Z
			m_fTransTime += fDeltaTime * fSlowRate;

			// TODO
#if 0
			// 
			SCameraPoint transDestStart = CameraPoint(STATE::STATE_FOLLOW);
			SCameraPoint transDiffStart;

			// 
			transDiffStart.posR			= transDestStart.posR - m_transStartPoint.posR;
			transDiffStart.posV			= transDestStart.posV - m_transStartPoint.posV;
			transDiffStart.rot			= transDestStart.rot  - m_transStartPoint.rot;
			transDiffStart.fDistance	= transDestStart.fDistance - m_transStartPoint.fDistance;

			// 
			m_transStartPoint.posR		+= transDiffStart.posR * 0.2f;
			m_transStartPoint.posV		+= transDiffStart.posV * 0.2f;
			m_transStartPoint.rot		+= transDiffStart.rot  * 0.2f;
			m_transStartPoint.fDistance	+= transDiffStart.fDistance * 0.2f;
#endif

			// TODO
#if 0
			m_transStartPoint = CameraPoint(STATE::STATE_FOLLOW);
#endif

			// �J������ԑJ�ڂ̍X�V
			if (UpdateTrans(STATE::STATE_FOLLOW))
			{ // �J�ڂ��������ꍇ

				// ��ԑJ�ڂ̌o�ߎ��Ԃ�������
				m_fTransTime = 0.0f;

				// �J�ڂ̏I�����Ԃ�������
				m_fEndTime = 0.0f;

				Reset();
				b = false;
			}
		}
		else
		{ // ��Ԃ��J�ڍς݂̏ꍇ

			// �Ǐ]�J�����̍X�V
			UpdateFollow(fDeltaTime, fDeltaRate, fSlowRate);

			// ��ԑJ�ڂ̌o�ߎ��Ԃ�������
			m_fTransTime = 0.0f;

			// �J�ڂ̏I�����Ԃ�������
			m_fEndTime = 0.0f;

			b = false;
		}
	}

	// ���݂̊O��{�[�������t���O��ۑ�
	m_bOldWithBall = bCurWithBall;
}

//==========================================================================
// �O���Ԃ̍X�V����
//==========================================================================
void CCamera::UpdateOutFieldState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �Q�[�����[�h�ȊO�ł͎g�p�ł��Ȃ�
	if (GET_MANAGER->GetMode() != CScene::MODE_GAME) { return; }

	// ���݂̊O��{�[�������t���O���擾
	bool bCurWithBall = IsPlayerOutWithBall();
	if (!bCurWithBall && m_bOldWithBall)
	{ // �O�삪�{�[���𓊂����u�Ԃ̏ꍇ

		// ���݂̃J�����|�C���g���J�n�|�C���g�Ƃ��ĕۑ�
		m_transStartPoint.posR = m_posR;
		m_transStartPoint.posV = m_posV;
		m_transStartPoint.rot = m_rot;
		m_transStartPoint.fDistance = m_fDistance;

		// ��ԑJ�ڂ̌o�ߎ��Ԃ�������
		m_fTransTime = 0.0f;

		// �J�ڂ̏I�����Ԃ�ۑ�
		m_fEndTime = game::END_TIME;

		b = false;
	}

	if (!bCurWithBall)
	{ // �O�삪�{�[���������Ă��Ȃ��ꍇ

		// ��ԑJ�ڂ̌o�ߎ��Ԃ����Z
		m_fTransTime += fDeltaTime * fSlowRate;

		// �J������ԑJ�ڂ̍X�V
		if (UpdateTrans(STATE::STATE_FOLLOW))
		{ // �J�ڂ��������ꍇ

			// �Ǐ]��Ԃɂ���
			SetState(STATE::STATE_FOLLOW);

			// ��ԑJ�ڂ̌o�ߎ��Ԃ�������
			m_fTransTime = 0.0f;

			// �J�ڂ̏I�����Ԃ�������
			m_fEndTime = 0.0f;
		}
	}
	else
	{ // �O�삪�{�[���������Ă����ꍇ

		if (m_fEndTime > 0.0f)
		{ // ��Ԃ��J�ڂ������Ă����ꍇ

			// �I�����Ԃ�ݒ�
			if (!b)
			{
				// ���݂̃J�����|�C���g���J�n�|�C���g�Ƃ��ĕۑ�
				m_transStartPoint.posR = m_posR;
				m_transStartPoint.posV = m_posV;
				m_transStartPoint.rot = m_rot;
				m_transStartPoint.fDistance = m_fDistance;

				// 
				m_fEndTime = m_fTransTime;
				UtilFunc::Transformation::ValueNormalize(m_fEndTime, game::END_TIME, game::MIN_TIME);
				m_fTransTime = 0.0f;
				b = true;
			}

			// ��ԑJ�ڂ̌o�ߎ��Ԃ����Z
			m_fTransTime += fDeltaTime * fSlowRate;

			// �J������ԑJ�ڂ̍X�V
			if (UpdateTrans(STATE::STATE_OUTFIELD))
			{ // �J�ڂ��������ꍇ

				// ��ԑJ�ڂ̌o�ߎ��Ԃ�������
				m_fTransTime = 0.0f;

				// �J�ڂ̏I�����Ԃ�������
				m_fEndTime = 0.0f;

				Reset();
				b = false;
			}
		}
		else
		{ // ��Ԃ��J�ڍς݂̏ꍇ

			// �O��J�����̍X�V
			UpdateOutField(fDeltaTime, fDeltaRate, fSlowRate);

			// ��ԑJ�ڂ̌o�ߎ��Ԃ�������
			m_fTransTime = 0.0f;

			// �J�ڂ̏I�����Ԃ�������
			m_fEndTime = 0.0f;

			b = false;
		}
	}

	// ���݂̊O��{�[�������t���O��ۑ�
	m_bOldWithBall = bCurWithBall;
}

//==========================================================================
// �ʏ��Ԃ̃��Z�b�g
//==========================================================================
void CCamera::ResetNoneState()
{
	// �����_�̐ݒ�
	m_posR = m_posRDest = m_posROrigin = none::INIT_POSR;

	// �����̐ݒ�
	m_fDistance = m_fDestDistance = m_fOriginDistance = none::INIT_DISTANCE;

	// ����p�̐ݒ�
	m_fViewAngle = m_fDestViewAngle = none::INIT_VIEWANGLE;

	// ���_�����̐ݒ�
	m_rotOrigin = none::INIT_ROT;

	// ���_�����̐��K��
	UtilFunc::Transformation::RotNormalize(m_rotOrigin);

	// �����̐ݒ�
	m_rot = m_rotDest = m_rotOrigin;

	// ���ʍ��W�ϊ��ɂ��ڕW���_�̑��Έʒu�擾
	m_posV = m_posVDest = CalcSpherePosition(m_posROrigin, m_rotOrigin, -m_fOriginDistance);
}

//==========================================================================
// �Ǐ]��Ԃ̃��Z�b�g
//==========================================================================
void CCamera::ResetFollowState()
{
	// �Q�[�����[�h�ȊO�ł͎g�p�ł��Ȃ�
	if (GET_MANAGER->GetMode() != CScene::MODE_GAME) { return; }

	// �v���C���[���E�ő�ʒu���v�Z
	m_side = m_sideDest = GetPlayerMaxSide();

	// ���E�Ԃ̋����������v�Z
	const float fDisRate = CalcDistanceRate(m_sideDest);

	// �����_���v�Z
	m_posR = m_posRDest = m_posROrigin = CalcFollowPositionR(m_sideDest, fDisRate);

	// �������v�Z
	m_fDistance = m_fDestDistance = m_fOriginDistance = CalcFollowDistance(fDisRate);

	// ������ݒ�
	m_rot = m_rotDest = m_rotOrigin = follow::INIT_ROT;

	// ����p��ݒ�
	if (m_bMotion)	{ m_fViewAngle = m_fDestViewAngle = none::INIT_VIEWANGLE; }
	else			{ m_fViewAngle = m_fDestViewAngle = follow::INIT_VIEWANGLE; }

	// ���ʍ��W�ϊ��ɂ��ڕW���_�̑��Έʒu�擾
	m_posV = m_posVDest = CalcSpherePosition(m_posROrigin, m_rotOrigin, -m_fOriginDistance);
}

//==========================================================================
// �O���Ԃ̃��Z�b�g
//==========================================================================
void CCamera::ResetOutFieldState()
{
	// �Q�[�����[�h�ȊO�ł͎g�p�ł��Ȃ�
	if (GET_MANAGER->GetMode() != CScene::MODE_GAME) { return; }

	// �����_��ݒ�
	m_posR = m_posRDest = MyLib::Vector3(CGameManager::CENTER_LINE, out::POSRY, out::POSRZ);

	// ������ݒ�
	m_fDistance = m_fDestDistance = out::DIS;

	// ������ݒ�
	m_rot = m_rotDest = out::ROT;

	// �ڕW����p��ݒ�
	if (m_bMotion)	{ m_fDestViewAngle = none::INIT_VIEWANGLE; }
	else			{ m_fDestViewAngle = follow::INIT_VIEWANGLE; }

	// ���ݎ���p�������␳
	UtilFunc::Correction::InertiaCorrection(m_fViewAngle, m_fDestViewAngle, follow::REV_VIEWANGLE);

	// ���ʍ��W�ϊ��ɂ�鑊�Έʒu�̎擾
	m_posV = m_posVDest = CalcSpherePosition(m_posR, m_rot, -m_fDistance);	// �ڕW���_�ɐݒ�
}

//==========================================================================
// �Ǐ]�J�����̍X�V
//==========================================================================
void CCamera::UpdateFollow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �v���C���[���E�ő�ʒu���v�Z
	m_sideDest = GetPlayerMaxSide();

	// �����␳
	UtilFunc::Correction::InertiaCorrection(m_side.l, m_sideDest.l, follow::REV_SIDE);
	UtilFunc::Correction::InertiaCorrection(m_side.r, m_sideDest.r, follow::REV_SIDE);

	// ���E�Ԃ̋����������v�Z
	const float fDisRate = CalcDistanceRate(m_side);

	// �ڕW�����_���v�Z
	MyLib::Vector3 posCurDest = CalcFollowPositionR(m_side, fDisRate);

	// �ڕW�����_��␳
	if (m_posRDest.x + follow::DEST_POSR_BOXSIZE < posCurDest.x)
	{ // �ڕW�����_�𓮂����Ȃ��͈͂��E���̏ꍇ

		// �����_���E�[�ɕ␳
		m_posRDest.x = posCurDest.x - follow::DEST_POSR_BOXSIZE;
	}
	else if (m_posRDest.x - follow::DEST_POSR_BOXSIZE > posCurDest.x)
	{ // �ڕW�����_�𓮂����Ȃ��͈͂�荶���̏ꍇ

		// �����_�����[�ɕ␳
		m_posRDest.x = posCurDest.x + follow::DEST_POSR_BOXSIZE;
	}

	// X�����_�͈͕̔␳
	RevFollowPositionR(&m_posRDest.x, fDisRate);

#ifdef FOLLOW_DEBUG
	CEffect3D::Create(MyLib::Vector3(posCurDest.x, 50.0f, 0.0f), VEC3_ZERO, MyLib::color::Red(), 20.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);
	CEffect3D::Create(MyLib::Vector3(m_posRDest.x, 50.0f, 0.0f), VEC3_ZERO, MyLib::color::Yellow(), 15.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);

	CEffect3D::Create(MyLib::Vector3(m_posRDest.x + follow::DEST_POSR_BOXSIZE, 50.0f, -follow::DEST_POSR_BOXSIZE), VEC3_ZERO, MyLib::color::Green(), 5.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);
	CEffect3D::Create(MyLib::Vector3(m_posRDest.x - follow::DEST_POSR_BOXSIZE, 50.0f, -follow::DEST_POSR_BOXSIZE), VEC3_ZERO, MyLib::color::Green(), 5.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);
	CEffect3D::Create(MyLib::Vector3(m_posRDest.x + follow::DEST_POSR_BOXSIZE, 50.0f, +follow::DEST_POSR_BOXSIZE), VEC3_ZERO, MyLib::color::Green(), 5.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);
	CEffect3D::Create(MyLib::Vector3(m_posRDest.x - follow::DEST_POSR_BOXSIZE, 50.0f, +follow::DEST_POSR_BOXSIZE), VEC3_ZERO, MyLib::color::Green(), 5.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);
#endif

	// ���ݒ����_�������␳
	UtilFunc::Correction::InertiaCorrection(m_posR, m_posRDest, follow::REV_POSR);

	// ���ݒ����_��Y/Z���W�͖ڕW���W�ɂ�������
	m_posR.y = m_posRDest.y = posCurDest.y;
	m_posR.z = m_posRDest.z = posCurDest.z;

	// �������v�Z
	m_fDistance = m_fDestDistance = CalcFollowDistance(fDisRate);

	// ������ݒ�
	m_rot = m_rotDest = follow::INIT_ROT;

	// �ڕW����p��ݒ�
	if (m_bMotion)	{ m_fDestViewAngle = none::INIT_VIEWANGLE; }
	else			{ m_fDestViewAngle = follow::INIT_VIEWANGLE; }

	// ���ݎ���p�������␳
	UtilFunc::Correction::InertiaCorrection(m_fViewAngle, m_fDestViewAngle, follow::REV_VIEWANGLE);

	// ���ʍ��W�ϊ��ɂ�鑊�Έʒu�̎擾
	m_posV = m_posVDest = CalcSpherePosition(m_posR, m_rot, -m_fDistance);	// �ڕW���_�ɐݒ�
}

//==========================================================================
// �O��J�����̍X�V
//==========================================================================
void CCamera::UpdateOutField(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����_��ݒ�
	m_posR = m_posRDest = MyLib::Vector3(CGameManager::CENTER_LINE, out::POSRY, out::POSRZ);

	// ������ݒ�
	m_fDistance = m_fDestDistance = out::DIS;

	// ������ݒ�
	m_rot = m_rotDest = out::ROT;

	// �ڕW����p��ݒ�
	if (m_bMotion)	{ m_fDestViewAngle = none::INIT_VIEWANGLE; }
	else			{ m_fDestViewAngle = follow::INIT_VIEWANGLE; }

	// ���ݎ���p�������␳
	UtilFunc::Correction::InertiaCorrection(m_fViewAngle, m_fDestViewAngle, follow::REV_VIEWANGLE);

	// ���ʍ��W�ϊ��ɂ�鑊�Έʒu�̎擾
	m_posV = m_posVDest = CalcSpherePosition(m_posR, m_rot, -m_fDistance);	// �ڕW���_�ɐݒ�
}

//==========================================================================
// �J������ԑJ�ڂ̍X�V
//==========================================================================
bool CCamera::UpdateTrans(const STATE state)
{
	// ��ԑJ�ڂ̌o�ߎ��Ԃ�␳
	bool bRev = UtilFunc::Transformation::ValueNormalize(m_fTransTime, m_fEndTime, 0.0f);

	// �Q�[���J������ԑJ�ڂ̏I���|�C���g���擾
	SCameraPoint transEndPoint = CameraPoint(state);

	// �����_��ݒ�
	m_posR = m_posRDest = UtilFunc::Correction::EasingQuintOut(m_transStartPoint.posR, transEndPoint.posR, 0.0f, m_fEndTime, m_fTransTime);

	// ������ݒ�
	m_fDistance = m_fDestDistance = UtilFunc::Correction::EasingQuintOut(m_transStartPoint.fDistance, transEndPoint.fDistance, 0.0f, m_fEndTime, m_fTransTime);

	// ������ݒ�
	m_rot = m_rotDest = UtilFunc::Correction::EasingQuintOut(m_transStartPoint.rot, transEndPoint.rot, 0.0f, m_fEndTime, m_fTransTime);

	// ���ʍ��W�ϊ��ɂ�鑊�Έʒu�̎擾
	m_posV = m_posVDest = CalcSpherePosition(m_posR, m_rot, -m_fDistance);	// �ڕW���_�ɐݒ�

	// �o�ߎ��Ԃ��␳���ꂽ����Ԃ�
	return bRev;
}

//==========================================================================
// �O�삪�{�[���������Ă��邩
//==========================================================================
bool CCamera::IsPlayerOutWithBall()
{
	CGameManager* pGameManager = CGameManager::GetInstance();	// �Q�[���}�l�[�W���[
	assert(pGameManager != nullptr);		// �Q�[���}�l�[�W���[���Ȃ��ꍇ�G���[
	CBall* pBall = pGameManager->GetBall();	// �{�[�����
	assert(pBall != nullptr);				// �{�[�����Ȃ��ꍇ�G���[
	CPlayer* pTarget = pBall->GetTarget();	// �{�[���W�I�v���C���[���

	if (pTarget != nullptr
	&&  pTarget->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT)
	{ // �{�[���̃^�[�Q�b�g���O��̏ꍇ

		return true;
	}

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[�������X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �v���C���[�C�e���[�^�[
	while (list.ListLoop(itr))
	{ // �v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���

		if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT
		&&  pPlayer->GetBall() != nullptr)
		{ // �{�[�����������O�삪�����ꍇ

			return true;
		}
	}

	return false;
}

//==========================================================================
// �v���C���[�ő卶�E���W�̎擾
//==========================================================================
CCamera::SSide CCamera::GetPlayerMaxSide()
{
	SSide pos;	// ���E���W
	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[�������X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �v���C���[�C�e���[�^�[
	while (list.ListLoop(itr))
	{ // �v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���
		if (pPlayer == nullptr) continue;

		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// �v���C���[�ʒu

		// �O��̏ꍇ����
		if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT) { continue; }

		// ����^����Ԃ̏ꍇ����
		if (pPlayer->GetState() == CPlayer::EState::STATE_DEAD_CARRY) { continue; }

		// ���̍X�V
		if (posPlayer.x < pos.l) { pos.l = posPlayer.x; }

		// �E�̍X�V
		if (posPlayer.x > pos.r) { pos.r = posPlayer.x; }
	}

	// ���肵�����E���W��Ԃ�
	return pos;
}

//==========================================================================
// ���E�Ԃ̋��������̌v�Z
//==========================================================================
float CCamera::CalcDistanceRate(const SSide& rSide)
{
	const MyLib::Vector3 sizeCourt = CGameManager::GetInstance()->GetCourtSize();	// �R�[�g�T�C�Y
	const float fCurDis = rSide.r - rSide.l;	// ���E�Ԃ̋���
	const float fMaxDis = sizeCourt.x * 2.0f;	// ���E�Ԃ̍ő勗��

	// ���E�Ԃ̋���������Ԃ�
	return fCurDis / fMaxDis;
}

//==========================================================================
// �Ǐ]�J�����̋����v�Z
//==========================================================================
float CCamera::CalcFollowDistance(const float fDisRate)
{
	// �J����������������������v�Z���Ԃ�
	return (follow::MAX_DIS * fDisRate + MIN_DIS);
}

//==========================================================================
// �Ǐ]�J�����̒����_�v�Z
//==========================================================================
MyLib::Vector3 CCamera::CalcFollowPositionR(const SSide& rSide, const float fDisRate)
{
	const MyLib::Vector3 sizeCourt = CGameManager::GetInstance()->GetCourtSize();	// �R�[�g�T�C�Y

	// ���E���W�̕��ς���X�����_���v�Z
	float fTargetX = (rSide.l + rSide.r) * 0.5f;

	// X�����_�͈͕̔␳
	RevFollowPositionR(&fTargetX, fDisRate);

	// Y/Z�����_��������������v�Z
	float fTargetY = UtilFunc::Correction::EasingLinear(follow::POSRY_START, follow::POSRY_END, fDisRate);	// �J����Y�����_
	float fTargetZ = UtilFunc::Correction::EasingLinear(follow::POSRZ_START, follow::POSRZ_END, fDisRate);	// �J����Z�����_

	// �v�Z���������_��Ԃ�
	return MyLib::Vector3(fTargetX, fTargetY, fTargetZ);
}

//==========================================================================
// X�����_�͈͕̔␳
//==========================================================================
void CCamera::RevFollowPositionR(float* pTargetX, const float fDisRate)
{
	const MyLib::Vector3 sizeCourt = CGameManager::GetInstance()->GetCourtSize();	// �R�[�g�T�C�Y

	// X�����_�̕␳
	float fCourtHalfSize = sizeCourt.x * 0.5f;		// �R�[�g�̔����̑傫��
	float fPlusOffset = fCourtHalfSize * fDisRate;	// �����_�͈̓I�t�Z�b�g
	UtilFunc::Transformation::ValueNormalize(*pTargetX, CGameManager::CENTER_LINE + fCourtHalfSize - fPlusOffset, CGameManager::CENTER_LINE - fCourtHalfSize + fPlusOffset);

#ifdef FOLLOW_DEBUG
	CEffect3D::Create(MyLib::Vector3(CGameManager::CENTER_LINE - fCourtHalfSize + fPlusOffset, 50.0f, 0.0f), VEC3_ZERO, MyLib::color::Cyan(), 10.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);
	CEffect3D::Create(MyLib::Vector3(CGameManager::CENTER_LINE + fCourtHalfSize - fPlusOffset, 50.0f, 0.0f), VEC3_ZERO, MyLib::color::Cyan(), 10.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);
#endif
}

//==========================================================================
// ���ʍ��W�ϊ��ɂ�鑊�Έʒu�̎擾����
//==========================================================================
MyLib::Vector3 CCamera::CalcSpherePosition(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot, const float fDis)
{
	// ���ʍ��W�ϊ��ő��Έʒu���v�Z
	MyLib::Vector3 out;
	out.x = rPos.x + cosf(rRot.z) * sinf(rRot.y) * fDis;
	out.z = rPos.z + cosf(rRot.z) * cosf(rRot.y) * fDis;
	out.y = rPos.y + sinf(rRot.z) * fDis;

	// �ϊ��������΍��W��Ԃ�
	return out;
}

//==========================================================================
// �J�����h��̍X�V
//==========================================================================
void CCamera::UpdateSwing()
{
	// �����_�̂��炵�ʂ��ݒ肳��Ă��Ȃ��ꍇ������
	if (m_swing.fShiftLength <= 0.0f) { return; }

	float fRotY;			// �ʒu�������
	D3DXQUATERNION quat;	// �N�H�[�^�j�I��
	D3DXMATRIX mtxRot;		// ��]�}�g���b�N�X
	MyLib::Vector3 offset;	// �ʒu����I�t�Z�b�g
	MyLib::Vector3 vecAxis = m_posR - m_posV;	// ��]���x�N�g��

	// �N�H�[�^�j�I�����쐬
	D3DXVec3Normalize(&vecAxis, &vecAxis);	// ��]���𐳋K��
	D3DXQuaternionRotationAxis(&quat, &vecAxis, m_swing.fShiftAngle);

	// ��]�}�g���b�N�X���쐬
	D3DXMatrixRotationQuaternion(&mtxRot, &quat);

	// �ʒu�����炷���������߂�
	fRotY = atan2f(-vecAxis.z, vecAxis.x);

	// �ʒu����I�t�Z�b�g��ݒ�
	float fCalcTemp = m_swing.fShiftLength * (fabsf(m_fDistance) * REV_SWING);
	offset = MyLib::Vector3(sinf(fRotY) * fCalcTemp, 0.0f, cosf(fRotY) * fCalcTemp);

	// �I�t�Z�b�g�𔽉f������]�}�g���b�N�X�����W�ϊ�
	D3DXVec3TransformCoord(&m_swing.shiftPos, &offset, &mtxRot);

	// ���_�Ɉʒu�̂�������Z
	m_posV += m_swing.shiftPos;

	// �ʒu����ʂ����Z
	m_swing.fShiftAngle  -= m_swing.fSubAngle;
	m_swing.fShiftLength -= m_swing.fSubLength;

	// �p�x��␳
	UtilFunc::Transformation::RotNormalize(m_swing.fShiftAngle);

	// ������␳
	if (m_swing.fShiftLength < 0.0f) { m_swing.fShiftLength = 0.0f; }
}

//==========================================================================
// �X�|�b�g���C�g�̃x�N�g���X�V
//==========================================================================
void CCamera::UpdateSpotLightVec(const MyLib::Vector3& rPosR, const MyLib::Vector3& rPosV)
{
	// ���_���璍���_�ւ̃x�N�g�����v�Z
	MyLib::Vector3 vec = rPosR - rPosV;

	// �X�|�b�g���C�g�̕����ݒ�
	m_pLight->SetDirection(vec);
}

//==========================================================================
// �X�N���[�����̔���
//==========================================================================
bool CCamera::IsOnScreen(const MyLib::Vector3& pos)
{
	D3DVIEWPORT9 vp = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };	// �r���[�|�[�g
	D3DXMATRIX mtxWorld; // ���[���h�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �G�̃X�N���[�����W���Z�o
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
		return true;
	}

	return false;
}

//==========================================================================
// �X�N���[�����W�擾
//==========================================================================
MyLib::Vector3 CCamera::GetScreenPos(const MyLib::Vector3& pos)
{
	D3DVIEWPORT9 vp = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };	// �r���[�|�[�g
	D3DXMATRIX mtxWorld; // ���[���h�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�N���[�����W���Z�o
	MyLib::Vector3 screenPos;	// �X�N���[�����W
	D3DXVec3Project
	(
		&screenPos,
		&pos,
		&vp,
		&m_mtxProjection,
		&m_mtxView,
		&mtxWorld
	);

	return screenPos;
}

//==========================================================================
// ���݂̒Ǐ]�|�C���g�擾����
//==========================================================================
CCamera::SCameraPoint CCamera::CameraPoint(const STATE state)
{
	if (state == STATE::STATE_NONE)
	{ // ��Ԃ̎w�肪�Ȃ������ꍇ

		if (IsPlayerOutWithBall())
		{ // �O�삪�{�[���������Ă����ꍇ

			// ���݂̒Ǐ]�|�C���g��Ԃ�
			return FollowPoint();
		}
		else
		{ // �O�삪�{�[���������Ă��Ȃ��ꍇ

			// ���݂̊O��|�C���g��Ԃ�
			return OutFieldPoint();
		}
	}

	switch (state)
	{ // ��Ԃ��Ƃ̏���
	case STATE::STATE_FOLLOW:

		// ���݂̒Ǐ]�|�C���g��Ԃ�
		return FollowPoint();

	case STATE::STATE_OUTFIELD:

		// ���݂̊O��|�C���g��Ԃ�
		return OutFieldPoint();

	default:
		assert(false);
		break;
	}

	assert(false);
	return {};
}

//==========================================================================
// ���݂̒Ǐ]�|�C���g�擾����
//==========================================================================
CCamera::SCameraPoint CCamera::FollowPoint()
{
	SCameraPoint data;	// �J�������

	// �v���C���[���E�ő�ʒu���v�Z
	const SSide posSide = GetPlayerMaxSide();

	// ���E�Ԃ̋����������v�Z
	const float fDisRate = CalcDistanceRate(posSide);

	// �����_���v�Z
	data.posR = CalcFollowPositionR(posSide, fDisRate);

	// �������v�Z
	data.fDistance = CalcFollowDistance(fDisRate);

	// ������ݒ�
	data.rot = follow::INIT_ROT;

	// ���ʍ��W�ϊ��ɂ��ڕW���_�̑��Έʒu�擾
	data.posV = CalcSpherePosition(data.posR, data.rot, -data.fDistance);

	// �Ǐ]�J��������Ԃ�
	return data;
}

//==========================================================================
// ���݂̊O��|�C���g�擾����
//==========================================================================
CCamera::SCameraPoint CCamera::OutFieldPoint()
{
	SCameraPoint data;	// �J�������

	// �����_���v�Z
	data.posR = MyLib::Vector3(CGameManager::CENTER_LINE, out::POSRY, out::POSRZ);

	// �������v�Z
	data.fDistance = m_fDestDistance = out::DIS;

	// ������ݒ�
	data.rot = out::ROT;

	// ���ʍ��W�ϊ��ɂ��ڕW���_�̑��Έʒu�擾
	data.posV = CalcSpherePosition(data.posR, data.rot, -data.fDistance);

	// �O��J��������Ԃ�
	return data;
}
