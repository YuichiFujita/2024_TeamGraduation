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
// �萔��`
//==========================================================================
namespace
{
	const MyLib::Vector3 DEFAULT_GAMEROT = MyLib::Vector3(0.0f, 0.0f, -0.32f);	// �f�t�H���g����
	const float MIN_NEAR	= 10.0f;		// ��O�̕`�搧��
	const float MAX_FAR		= 1500000.0f;	// ���̕`�搧��
	const float REV_DIS		= 0.001f;		// �J�����h��v�Z���̃J���������̕␳�W��
	const float MIN_DISNTANCE	= 500.0f;	// �ŏ�����
	const float DISNTANCE		= 1160.0f;	// ����
	const float MULTIPLY_CHASE_POSR	= 1.5f;	// �����_�Ǐ]�̔{��
	const float MULTIPLY_CHASE_POSV	= 1.5f;	// �����_�Ǐ]�̔{��
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
// ��Ԋ֐�
CCamera::STATE_FUNC CCamera::m_StateFuncList[] =
{
	&CCamera::UpdateNoneState,		// �ʏ�J�����X�V
	&CCamera::UpdateFollowState,	// �Ǐ]�J�����X�V
};

// ���Z�b�g�֐�
CCamera::RESET_FUNC CCamera::m_ResetFuncList[] =
{
	&CCamera::ResetNoneState,		// �ʏ�J�������Z�b�g
	&CCamera::ResetFollowState,		// �Ǐ]�J�������Z�b�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCamera::CCamera()
{
	m_pDebugControll	= nullptr;		// �f�o�b�O���
	m_pCameraMotion		= nullptr;		// �J�������[�V�������
	m_pLight			= nullptr;		// �f�B���N�V���i�����C�g���
	m_posV				= VEC3_ZERO;	// ���_
	m_posVDest			= VEC3_ZERO;	// �ڕW���_
	m_posR				= VEC3_ZERO;	// �����_
	m_posRDest			= VEC3_ZERO;	// �ڕW�����_
	m_posROrigin		= VEC3_ZERO;	// ���_�����_
	m_vecU				= MyLib::Vector3(0.0f, 1.0f, 0.0f);	// ������x�N�g��
	m_move				= VEC3_ZERO;	// �ړ���
	m_rot				= VEC3_ZERO;	// ����
	m_rotDest			= VEC3_ZERO;	// �ڕW����
	m_rotOrigin			= VEC3_ZERO;	// ���_����
	m_posTarget			= VEC3_ZERO;	// �Ǐ]�ʒu
	m_posTargetDest		= VEC3_ZERO;	// �ڕW�Ǐ]�ʒu
	m_fDistance			= 0.0f;			// ����
	m_fDestDistance		= 0.0f;			// �ڕW����
	m_fOriginDistance	= 0.0f;			// ���_����
	m_fViewAngle		= 0.0f;			// ����p
	m_fDestViewAngle	= 0.0f;			// �ڕW����p
	m_bFollow			= false;		// �Ǐ]����
	m_bMotion			= false;		// ���[�V����������
	m_state				= STATE_NONE;	// ���

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
	m_swing.shiftPos		= VEC3_ZERO;	// �ʒu�����
	m_swing.fShiftAngle		= 0.0f;			// �ʒu�����炷�p�x
	m_swing.fShiftLength	= 0.0f;			// �ʒu�����炷����
	m_swing.fSubAngle		= 0.0f;			// ���炷�p�x�̌��Z��
	m_swing.fSubLength		= 0.0f;			// ���炷�����̌��Z��
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

	// �J�������Z�b�g
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
	m_pLight->SetDiffuse(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));

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
	m_viewport.X = (DWORD)pos.x;			// ��ʂ̍���X���W
	m_viewport.Y = (DWORD)pos.y;			// ��ʂ̍���Y���W
	m_viewport.Width = (DWORD)size.x;		// ��ʂ̕�
	m_viewport.Height = (DWORD)size.y;		// ��ʂ̍���

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

	if (m_StateFuncList[m_state] != nullptr)
	{ // ��ԍX�V�֐�������ꍇ

		// ��ԕʏ���
		(this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// ����p�̍X�V
	UpdateViewAngle();

	// �J�����h��̍X�V
	UpdateSwing();

	// �X�|�b�g���C�g�x�N�g���̍X�V
	UpdateSpotLightVec();

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
// �J�����̐ݒ菈��
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
}

//==========================================================================
// �J�������Z�b�g
//==========================================================================
void CCamera::Reset()
{
	// TODO�F���̏������Q��STATE���ł�낤�˂��Ęb
#if 1
	m_posR = MyLib::Vector3(0.0f, 200.0f, -560.0f);				// �����_(�������ꏊ)
	m_posV = MyLib::Vector3(0.0f, 300.0f, m_posR.z + -400.0f);	// ���_(�J�����̈ʒu)
	m_posVDest			= m_posV;			// �ڕW�̎��_
	m_posRDest			= m_posR;			// �ڕW�̒����_
	m_posROrigin		= m_posR;			// ���̒����_
	m_rot				= DEFAULT_GAMEROT;	// ����
	m_rotOrigin			= m_rot;			// ���̌���
	m_rotDest			= m_rot;			// �ڕW�̌���
	m_fDistance			= DISNTANCE;		// ����
	m_fDestDistance		= m_fDistance;		// �ڕW�̋���
	m_fOriginDistance	= m_fDistance;		// ���̋���
	m_fViewAngle		= D3DXToRadian(30);	// ����p
	m_fDestViewAngle	= m_fViewAngle;		// �ڕW����p
#endif

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

	// ���_�̑��ΐݒ�
	ReflectCameraV();		// ���_
	m_posVDest = m_posV;	// �ڕW�̎��_
}

//==========================================================================
// �J�����̎��_�������
//==========================================================================
void CCamera::ReflectCameraV()
{
	if (!m_bFollow)
	{ // �Ǐ]���Ȃ��Ƃ�

		// ���_�̑������
		m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
	}
	else
	{ // �Ǐ]ON

		// ���_�̑������
		m_posVDest.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posVDest.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posVDest.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;

		// �␳����
		m_posV += (m_posVDest - m_posV) * MULTIPLY_CHASE_POSV;
	}
}

//==========================================================================
// �ʏ��Ԃ̍X�V����
//==========================================================================
void CCamera::UpdateNoneState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����̐��K��
	UtilFunc::Transformation::RotNormalize(m_rot);
	UtilFunc::Transformation::RotNormalize(m_rotDest);
	UtilFunc::Transformation::RotNormalize(m_rotOrigin);

	// �����_�̔��f
	ReflectCameraR();

	// ���_�̔��f
	ReflectCameraV();
}

//==========================================================================
// �Ǐ]��Ԃ̍X�V����
//==========================================================================
void CCamera::UpdateFollowState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����̐��K��
	UtilFunc::Transformation::RotNormalize(m_rot);
	UtilFunc::Transformation::RotNormalize(m_rotDest);
	UtilFunc::Transformation::RotNormalize(m_rotOrigin);

	// �����_�̔��f
	ReflectCameraR();

	// ���_�̔��f
	ReflectCameraV();
}

//==========================================================================
// �ʏ��Ԃ̃��Z�b�g
//==========================================================================
void CCamera::ResetNoneState()
{

}

//==========================================================================
// �Ǐ]��Ԃ̃��Z�b�g
//==========================================================================
void CCamera::ResetFollowState()
{

}

//==========================================================================
// �J�����̒����_�������
//==========================================================================
void CCamera::ReflectCameraR()
{
	if (!m_bFollow ||
		(m_bMotion && m_bFollow))
	{ // �Ǐ]���Ȃ��Ƃ�

		//// �����_�̑������
		//m_posR.x = m_posV.x + cosf(m_rot.z) * sinf(m_rot.y) * m_fDistance;
		//m_posR.z = m_posV.z + cosf(m_rot.z) * cosf(m_rot.y) * m_fDistance;
		//m_posR.y = m_posV.y + sinf(m_rot.z) * m_fDistance;
	}
	else
	{ // �Ǐ]ON

		// �^�[�Q�b�g�݂�
		m_posRDest = m_posTarget;

		// �␳����
		m_posR += (m_posRDest - m_posR) * MULTIPLY_CHASE_POSR;
	}
}

//==========================================================================
// ����p�̍X�V
//==========================================================================
void CCamera::UpdateViewAngle()
{
	// �ڕW�̎���p�֊����␳
	UtilFunc::Correction::InertiaCorrection(m_fViewAngle, m_fDestViewAngle, 0.2f);
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
	float fCalcTemp = m_swing.fShiftLength * (fabsf(m_fDistance) * REV_DIS);
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
void CCamera::UpdateSpotLightVec()
{
	// ���_���璍���_�ւ̃x�N�g�����v�Z
	MyLib::Vector3 vec = m_posR - m_posV;

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
