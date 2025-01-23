//============================================================
//
//	�X�y�V�������o�}�l�[�W���[���� [specialManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "specialManager.h"
#include "specialEffect.h"
#include "manager.h"
#include "camera.h"
#include "fade.h"

#include "game.h"
#include "gameManager.h"
#include "player.h"
#include "playerStatus.h"
#include "lightManager.h"
#include "lightPoint.h"
#include "cutin.h"
#include "audience.h"
#include "gymWallManager.h"
#include "spotlight.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY = 4;			// �D�揇��
	const float	LIGHT_RANGE = 600.0f;	// �����͈�
	const float LIGHTUP_TIME = 1.2f;	// ���邭�Ȃ�܂ł̎���
	const MyLib::Vector3 LIGHT_POS = MyLib::Vector3(0.0f, 160.0f, 0.0f);	// ���C�g�I�t�Z�b�g

	const float	JUMP_HEIGHT = 1000.0f;	// �W�����v�̍ō����B�_
	const float	JUMP_TIME = 1.24f;		// �W�����v����

	namespace hype
	{
		namespace trans
		{
			const float END_TIME = 4.2f;
		}
	}

	namespace follow
	{
		namespace trans
		{
			const float END_TIME = 1.8f;
		}
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CSpecialManager::AFuncUpdateState CSpecialManager::m_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,								// �������Ȃ��X�V
	&CSpecialManager::UpdateCutIn,			// �J�b�g�C���X�V
	&CSpecialManager::UpdateHypeTrans,		// ����オ��J�ڍX�V
	&CSpecialManager::UpdateHype,			// ����オ��X�V
	&CSpecialManager::UpdateStag,			// �X�y�V�������o�X�V
	&CSpecialManager::UpdateFollowTrans,	// �Ǐ]�J�ڍX�V
	&CSpecialManager::UpdateEnd,			// �I���X�V
};
CSpecialManager::AFuncUpdateSpecial CSpecialManager::m_aFuncUpdateSpecial[] =	// �X�y�V�����X�V�֐�
{
	nullptr,							// �X�y�V��������
	&CSpecialManager::UpdateKamehameha,	// ���߂͂ߔg�̍X�V
};
CSpecialManager* CSpecialManager::m_pInstance = nullptr;	// ���g�̃C���X�^���X

//************************************************************
//	�e�N���X [CSpecialManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSpecialManager::CSpecialManager(CPlayer* pAttack, CPlayer* pTarget) : CObject(PRIORITY),
	m_pCenterLight	(nullptr),		// ���C�g���
	m_pAttackLight	(nullptr),		// �U���v���C���[���Ƃ炷���C�g
	m_pTargetLight	(nullptr),		// �W�I�v���C���[���Ƃ炷���C�g
	m_pAttackPlayer	(pAttack),		// �U���v���C���[
	m_pTargetPlayer	(pTarget),		// �W�I�v���C���[
	m_pCutIn		(nullptr),		// �J�b�g�C�����
	m_state			(STATE_NONE),	// ���
	m_fCurTime		(0.0f)			// ���݂̑ҋ@����
{
	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_aFuncUpdateState)   == CSpecialManager::STATE_MAX, "ERROR : State Count Mismatch");
	static_assert(NUM_ARRAY(m_aFuncUpdateSpecial) == CBall::SPECIAL_MAX,         "ERROR : Special Count Mismatch");
}

//============================================================
//	�f�X�g���N�^
//============================================================
CSpecialManager::~CSpecialManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSpecialManager::Init(void)
{
	// �����o�ϐ���������
	m_state = STATE_CUTIN;	// ���
	m_fJumpTime = 0.0f;		// ���݂̃W�����v����
	m_bJump = false;		// �W�����v�t���O

	// ��ނ��}�l�[�W���[�ɂ���
	SetType(CObject::TYPE::TYPE_MANAGER);

	// ���E��~���ɓ�����悤�ɂ���
	SetEnablePosibleMove_WorldPause(true);

	// �J�b�g�C���̐���
	m_pCutIn = CCutIn::Create();
	if (m_pCutIn == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ���`�[���̊ϋq��ʏ��Ԃɂ���
	CAudience::SetEnableJumpAll(false, CGameManager::ETeamSide::SIDE_LEFT);
	CAudience::SetEnableJumpAll(false, CGameManager::ETeamSide::SIDE_RIGHT);

	// ���E�̎����~�߂�
	GET_MANAGER->SetEnableWorldPaused(true);

	// �Q�[�����X�y�V�������o�V�[���ɕύX
	CGameManager::GetInstance()->SetSceneType(CGameManager::ESceneType::SCENE_SPECIAL);

	// �W�����v���[�V�����ɂ���
	m_pAttackPlayer->SetMotion(CPlayer::EMotion::MOTION_JUMP);

	// �ʏ�J�����̐ݒ�
	CCamera* pCamera = GET_MANAGER->GetCamera();	// �J�������
	pCamera->SetState(CCamera::STATE_NONE, false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSpecialManager::Uninit(void)
{
	// �U���v���C���[���Ƃ炷���C�g�̏I��
	SAFE_UNINIT(m_pAttackLight);

	// �W�I�v���C���[���Ƃ炷���C�g�̏I��
	SAFE_UNINIT(m_pTargetLight);

	// �I�u�W�F�N�g��j��
	Release();

	// ���g�̕ۑ����Ă����C���X�^���X��������
	m_pInstance = nullptr;
}

//============================================================
//	�폜����
//============================================================
void CSpecialManager::Kill()
{
	// ���g�̏I��
	Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSpecialManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	assert(m_state > -1 && m_state < STATE_MAX);
	if (m_aFuncUpdateState[m_state] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ

		// �e��Ԃ��Ƃ̍X�V
		(this->*(m_aFuncUpdateState[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	if (m_pAttackPlayer->GetBall() != nullptr)
	{ // �{�[���𓊂��Ă��Ȃ��ꍇ

		CBall::ESpecial typeSpecial = CGameManager::GetInstance()->GetBall()->GetTypeSpecial();	// �X�y�V�������
		assert(typeSpecial > CBall::SPECIAL_NONE && typeSpecial < CBall::SPECIAL_MAX);
		if (m_aFuncUpdateState[typeSpecial] != nullptr)
		{ // �X�V�֐����w�肳��Ă���ꍇ

			// �e�X�y�V�������Ƃ̍X�V
			(this->*(m_aFuncUpdateSpecial[typeSpecial]))(fDeltaTime, fDeltaRate, fSlowRate);
		}
	}

	if (m_bJump)
	{ // �W�����v���̏ꍇ

		// �o�ߎ��Ԃ����Z
		m_fJumpTime += fDeltaTime * fSlowRate;
		if (m_fJumpTime >= JUMP_TIME * 0.5f)
		{ // ��ԂĂ��؂�܂Ō������ꍇ

			// �o�ߎ��Ԃ�����
			m_fJumpTime += m_fJumpTime * 0.0075f;
		}

		// �o�ߎ��Ԃ̊������v�Z
		float fTimeRate = m_fJumpTime / JUMP_TIME;
		fTimeRate = UtilFunc::Transformation::Clamp(fTimeRate, 0.0f, 1.0f);	// ������␳

		// �W�����v������
		MyLib::Vector3 posPlayer = UtilFunc::Calculation::GetParabola3D(m_posJumpStart, m_posJumpEnd, JUMP_HEIGHT, fTimeRate);
		if (m_fJumpTime >= JUMP_TIME)
		{ // ���Ԃ��o�߂��������ꍇ

			// �W�����v�I��
			m_bJump = false;

			// �q�[���[���n���[�V�����ɂ���
			m_pAttackPlayer->SetMotion(CPlayer::EMotion::MOTION_LAND_SP);
		}

		// �ʒu��ݒ�
		m_pAttackPlayer->SetPosition(posPlayer);
	}

	// ���C�g�ʒu�̐ݒ�
	SetLightPosition();
}

//============================================================
//	�`�揈��
//============================================================
void CSpecialManager::Draw()
{

}

//============================================================
//	��������
//============================================================
CSpecialManager *CSpecialManager::Create(CPlayer* pAttack, CPlayer* pTarget)
{
	if (m_pInstance != nullptr)
	{ // ���N���X�̑��C���X�^���X������ꍇ

		// �C���X�^���X��j��
		SAFE_UNINIT(m_pInstance);
	}

	// �X�y�V�������o�}�l�[�W���[�̐���
	CSpecialManager *pSpecialManager = DEBUG_NEW CSpecialManager(pAttack, pTarget);
	if (pSpecialManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �X�y�V�������o�}�l�[�W���[�̏�����
		if (FAILED(pSpecialManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �X�y�V�������o�}�l�[�W���[�̔j��
			SAFE_DELETE(pSpecialManager);
			return nullptr;
		}

		// �C���X�^���X��ۑ�
		assert(m_pInstance == nullptr);
		m_pInstance = pSpecialManager;

		// �m�ۂ����A�h���X��Ԃ�
		return pSpecialManager;
	}
}

//============================================================
//	�J�b�g�C���̍X�V����
//============================================================
void CSpecialManager::UpdateCutIn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pCutIn->IsEnd())
	{ // �J�b�g�C�����o���I�������ꍇ

		CCamera* pCamera = GET_MANAGER->GetCamera();				// �J�������
		CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// �J�������[�V�������
		bool bInverse = (m_pAttackPlayer->GetTeam() == CGameManager::ETeamSide::SIDE_LEFT) ? false : true;	// �J�������[�V�����̔��]�t���O

		// ���E�̎��͂���������
		GET_MANAGER->SetEnableWorldPaused(false);

		// �J�b�g�C���̏I��
		SAFE_UNINIT(m_pCutIn);

		// �U�����v���C���[�`�[���̊ϋq�𐷂�グ��
		CAudience::SetSpecialAll(m_pAttackPlayer->GetTeam());

		// �ǂ̕\����ON�ɂ���
		CGameManager* pGameManager = CGameManager::GetInstance();		// �Q�[���}�l�[�W���[
		CGymWallManager* pGymWall = pGameManager->GetGymWallManager();	// �̈�ٕǃ}�l�[�W���[
		pGymWall->SetIsWall(true);

		// �J�����̌��_��ݒ�
		pCamera->SetPositionROrigin(pCamera->GetPositionR());	// �����_
		pCamera->SetOriginRotation(pCamera->GetRotation());		// ����
		pCamera->SetDistanceOrigin(pCamera->GetDistance());		// ����

		{
			// ���S���C�g�̐���
			m_pCenterLight = CLightPoint::Create();
			if (m_pCenterLight == nullptr)
			{ // �����Ɏ��s�����ꍇ

			}

			// �g�U����ݒ�
			m_pCenterLight->SetDiffuse(MyLib::color::White());

			// �����͈͂�ݒ�
			m_pCenterLight->SetRange(LIGHT_RANGE);

			// �ʒu��ݒ�
			m_pCenterLight->SetPosition(LIGHT_POS);

			// �U���v���C���[���Ƃ炷���C�g�̐���
			m_pAttackLight = CSpotLight::Create();
			if (m_pAttackLight == nullptr)
			{ // �����Ɏ��s�����ꍇ

			}

			// �W�I�v���C���[���Ƃ炷���C�g�̐���
			m_pTargetLight = CSpotLight::Create();
			if (m_pTargetLight == nullptr)
			{ // �����Ɏ��s�����ꍇ

			}

			// �̈�ق��Â�����
			GET_MANAGER->GetLight()->SetEnableBright(false);

			// �v���C���[����グ�ʒu�̐ݒ�
			SetPlayerHypePosition();
		}

		// ����オ��J�ڏ�Ԃɂ���
		m_state = STATE_HYPE_TRANS;
	}
}

//============================================================
//	����オ��J�ڂ̍X�V����
//============================================================
void CSpecialManager::UpdateHypeTrans(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CCamera* pCamera = GET_MANAGER->GetCamera();				// �J�������
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// �J�������[�V�������
	CCameraMotion::MotionKey key = pCameraMotion->GetKeyData(CCameraMotion::MOTION::MOTION_SPECIAL_HYPE, 0);	// �擪�L�[���
	bool bInverse = (m_pAttackPlayer->GetTeam() == CGameManager::ETeamSide::SIDE_LEFT) ? false : true;			// �J�������[�V�����̔��]�t���O
	if (bInverse)
	{ // ���]����ꍇ

		// ������Y���ɔ��]������
		key.rotDest.y *= -1.0f;

		// �����𐳋K��
		UtilFunc::Transformation::RotNormalize(key.rotDest.y);
	}

	// �J�������̎擾
	MyLib::Vector3 posR	= pCamera->GetPositionR();	// �����_
	MyLib::Vector3 rot	= pCamera->GetRotation();	// ����
	float fDis			= pCamera->GetDistance();	// ����

	// ���݂̑ҋ@���Ԃ����Z
	m_fCurTime += fDeltaTime * fSlowRate;

	// �ǉ��Ŏ��Ԃ����Z
	m_fCurTime += UtilFunc::Correction::EasingQuintOut(0.08f, 0.01f, 0.0f, hype::trans::END_TIME, m_fCurTime);

	// �J�������̐��`�␳
	posR = UtilFunc::Correction::EasingQuintOut(pCamera->GetPositionROrigin(), key.posRDest + GetDestAttackPosition(), 0.0f, hype::trans::END_TIME, m_fCurTime);
	rot  = UtilFunc::Correction::EasingQuintOut(pCamera->GetOriginRotation(),  key.rotDest,  0.0f, hype::trans::END_TIME, m_fCurTime);
	fDis = UtilFunc::Correction::EasingQuintOut(pCamera->GetDistanceOrigin(),  key.distance, 0.0f, hype::trans::END_TIME, m_fCurTime);

	// �J�������̔��f
	pCamera->SetPositionR(posR);	// �����_
	pCamera->SetRotation(rot);		// ����
	pCamera->SetDistance(fDis);		// ����

	if (m_fCurTime >= hype::trans::END_TIME)
	{ // ���Ԃ��o�߂��������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �J�������̕␳
		pCamera->SetPositionR(key.posRDest + GetDestAttackPosition());	// �����_
		pCamera->SetRotation(key.rotDest);	// ����
		pCamera->SetDistance(key.distance);	// ����

		// �J�����ʒu���U���v���C���[�̈ʒu�ɂ���
		pCameraMotion->SetPosition(GetDestAttackPosition());

		// �X�y�V��������グ���[�V������ݒ�
		pCameraMotion->SetMotion(CCameraMotion::MOTION_SPECIAL_HYPE, bInverse, true, true, true);

		// ����オ���Ԃɂ���
		m_state = STATE_HYPE;
	}
}

//============================================================
//	����オ��̍X�V����
//============================================================
void CSpecialManager::UpdateHype(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CCamera* pCamera = GET_MANAGER->GetCamera();				// �J�������
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// �J�������[�V�������
	if (pCameraMotion->IsFinish())
	{ // �J�������[�V�������I�������ꍇ

		// �U�����v���C���[�ɃX�y�V�����U����ݒ�
		m_pAttackPlayer->SetSpecialAttack();

		// �v���C���[�X�y�V�������o��Ԃɂ���
		m_state = STATE_STAG;
	}
}

//============================================================
//	�X�y�V�������o�̍X�V����
//============================================================
void CSpecialManager::UpdateStag(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CCamera* pCamera = GET_MANAGER->GetCamera();				// �J�������
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// �J�������[�V�������
	if (pCameraMotion->IsFinish())
	{ // �J�������[�V�������I������ꍇ

		// �ǂ̕\����OFF�ɂ���
		CGameManager* pGameManager = CGameManager::GetInstance();		// �Q�[���}�l�[�W���[
		CGymWallManager* pGymWall = pGameManager->GetGymWallManager();	// �̈�ٕǃ}�l�[�W���[
		pGymWall->SetIsWall(false);

		// �Ǐ]�J�ڏ�Ԃɂ���
		m_state = STATE_FOLLOW_TRANS;
	}
}

//============================================================
//	�Ǐ]�J�ڂ̍X�V����
//============================================================
void CSpecialManager::UpdateFollowTrans(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �J�������̎擾
	CCamera* pCamera	= GET_MANAGER->GetCamera();	// �J�������
	MyLib::Vector3 posR	= pCamera->GetPositionR();	// �����_
	MyLib::Vector3 rot	= pCamera->GetRotation();	// ����
	float fDis			= pCamera->GetDistance();	// ����
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// �J�������[�V�������
	CCamera::SCameraPoint key = pCamera->CameraPoint();			// �Ǐ]�ڕW���
	CCameraMotion::MotionKey keyEnd = pCameraMotion->GetKeyData((CCameraMotion::MOTION)pCameraMotion->GetNowMotionIdx(), pCameraMotion->GetNowKeyMax() - 1);	// �I�[�L�[���

	bool bInverse = (m_pAttackPlayer->GetTeam() == CGameManager::ETeamSide::SIDE_LEFT) ? false : true;	// �J�������[�V�����̔��]�t���O
	if (bInverse)
	{ // ���]����ꍇ

		// ������Y���ɔ��]������
		keyEnd.rotDest.y *= -1.0f;
		key.rot.y *= -1.0f;

		// �����𐳋K��
		UtilFunc::Transformation::RotNormalize(keyEnd.rotDest.y);
		UtilFunc::Transformation::RotNormalize(key.rot.y);
	}

	// ���݂̑ҋ@���Ԃ����Z
	m_fCurTime += fDeltaTime * fSlowRate;

	// �J�������̐��`�␳
	posR = UtilFunc::Correction::EaseInOutExpo(keyEnd.posRDest + pCameraMotion->GetPosition(), key.posR, 0.0f, follow::trans::END_TIME, m_fCurTime);
	rot  = UtilFunc::Correction::EaseInOutExpo(keyEnd.rotDest,  key.rot,       0.0f, follow::trans::END_TIME, m_fCurTime);
	fDis = UtilFunc::Correction::EaseInOutExpo(keyEnd.distance, key.fDistance, 0.0f, follow::trans::END_TIME, m_fCurTime);

	// �J�������̔��f
	pCamera->SetPositionR(posR);	// �����_
	pCamera->SetRotation(rot);		// ����
	pCamera->SetDistance(fDis);		// ����

	if (m_fCurTime >= follow::trans::END_TIME)
	{ // ���Ԃ��o�߂��������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �J�������̕␳
		pCamera->SetPositionR(key.posR);		// �����_
		pCamera->SetRotation(key.rot);			// ����
		pCamera->SetDistance(key.fDistance);	// ����

		// �I����Ԃɂ���
		m_state = STATE_END;
	}
}

//============================================================
//	�I���̍X�V����
//============================================================
void CSpecialManager::UpdateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �̈�ق𖾂邭����
	GET_MANAGER->GetLight()->SetEnableBright(true, LIGHTUP_TIME);

	// �U�����v���C���[�`�[���̊ϋq��ʏ��Ԃɂ���
	CAudience::SetEnableJumpAll(false, m_pAttackPlayer->GetTeam());

	// �Q�[�������C���V�[���ɕύX
	CGameManager* pGameManager = CGameManager::GetInstance();	// �Q�[���}�l�[�W���[
	pGameManager->SetSceneType(CGameManager::ESceneType::SCENE_MAIN);

	// �Ǐ]�J�����̐ݒ�
	CCamera* pCamera = GET_MANAGER->GetCamera();	// �J�������
	pCamera->SetState(CCamera::STATE_FOLLOW);

	// �X�y�V�������o�̏I�����ݒ�
	CSpecialEffect* pSPEffect = m_pAttackPlayer->GetSpecialEffect();
	if (pSPEffect != nullptr)
	{
		pSPEffect->FinishSetting();
	}

	// ���g�̏I��
	Uninit();
}

//============================================================
//	���߂͂ߔg�̍X�V����
//============================================================
void CSpecialManager::UpdateKamehameha(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// TODO�F�����Ɋe�X�y�V�����̃J��������

#if 0
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= 2.0f)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �I����Ԃɂ���
		m_state = STATE_END;
	}
#endif
}

//============================================================
//	�U���v���C���[�ڕW�ʒu�̎擾����
//============================================================
MyLib::Vector3 CSpecialManager::GetDestAttackPosition() const
{
	bool bInverse = (m_pAttackPlayer->GetTeam() == CGameManager::ETeamSide::SIDE_LEFT) ? false : true;	// �J�������[�V�����̔��]�t���O
	float fOffset = (bInverse) ? 1.0f : -1.0f;	// �I�t�Z�b�g����
	return MyLib::Vector3(800.0f * fOffset, 0.0f, 0.0f);
}

//============================================================
//	�v���C���[����グ�ʒu�̐ݒ菈��
//============================================================
void CSpecialManager::SetPlayerHypePosition()
{
	bool bInverse = (m_pAttackPlayer->GetTeam() == CGameManager::ETeamSide::SIDE_LEFT) ? false : true;	// �J�������[�V�����̔��]�t���O
	float fOffset = (bInverse) ? 1.0f : -1.0f;	// �I�t�Z�b�g����

	// �W�����v�J�n�ʒu��ݒ�
	m_posJumpStart = m_pAttackPlayer->GetPosition();

	// �W�����v�I���ʒu��ݒ�
	m_posJumpEnd = GetDestAttackPosition();

	// �U���v���C���[�̌�����ݒ�
	m_pAttackPlayer->SetRotation(MyLib::Vector3(0.0f, HALF_PI * fOffset, 0.0f));
	m_pAttackPlayer->SetRotDest(HALF_PI * fOffset);

	// �W�����v���ɂ���
	m_bJump = true;
}

//============================================================
//	���C�g�ʒu�̐ݒ菈��
//============================================================
void CSpecialManager::SetLightPosition()
{
	// �U���v���C���[�̈ʒu�Ɉړ�
	if (m_pAttackLight != nullptr)
	{
		m_pAttackLight->SetLightPosition(m_pAttackPlayer->GetPosition());
	}

	// �W�I�v���C���[�̈ʒu�Ɉړ�
	if (m_pTargetLight != nullptr)
	{
		m_pTargetLight->SetLightPosition(m_pTargetPlayer->GetPosition());
	}
}
