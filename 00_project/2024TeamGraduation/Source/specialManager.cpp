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

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY = 4;			// �D�揇��
	const float	LIGHT_RANGE = 600.0f;	// �����͈�
	const MyLib::Vector3 LIGHT_OFFSET = MyLib::Vector3(0.0f, 160.0f, 0.0f);	// ���C�g�I�t�Z�b�g

	namespace fade
	{
		const float	MOVE_TIME	 = 0.5f;	// �ϓ�����
		const float DEST_ALPHA	 = 0.5f;	// �ڕW�����x
		const float INIT_ALPHA	 = 0.0f;	// ���������x
		const float DIFF_ALPHA	 = DEST_ALPHA - INIT_ALPHA;	// ���������x
		const D3DXCOLOR INIT_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, INIT_ALPHA);		// �����F
		const D3DXVECTOR3 SIZE	 = D3DXVECTOR3(SCREEN_SIZE.x, 280.0f, 0.0f);	// �傫��
		const D3DXVECTOR3 POS	 = D3DXVECTOR3(SCREEN_CENT.x, 548.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 ROT	 = VEC3_ZERO;	// ����
	}

	namespace title
	{
		const char *TEXTURE		= "data\\TEXTURE\\get_magatama.png";	// �e�N�X�`���p�X
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(632.0f, 184.0f, 0.0f);	// �傫��
		const float	MOVE_TIME	= 0.68f;	// �ړ�����
		const D3DXCOLOR DEST_COL	= MyLib::color::White();		// �ڕW�F
		const D3DXCOLOR INIT_COL	= MyLib::color::White(0.0f);	// �����F
		const D3DXCOLOR DIFF_COL	= DEST_COL - INIT_COL;			// �����F
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(SCREEN_CENT.x, 505.0f, 0.0f);		// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// �����ʒu
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;							// �����ʒu
	}

	namespace line
	{
		const float	MOVE_TIME	= 0.5f;	// �ړ�����
		const D3DXVECTOR3 POS	= D3DXVECTOR3(SCREEN_CENT.x, 590.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 DEST_SIZE	= D3DXVECTOR3(980.0f, 10.0f, 0.0f);		// �ڕW�傫��
		const D3DXVECTOR3 INIT_SIZE	= D3DXVECTOR3(0.0f, DEST_SIZE.y, 0.0f);	// �����傫��
		const D3DXVECTOR3 DIFF_SIZE	= DEST_SIZE - INIT_SIZE;				// �����傫��
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CSpecialManager::AFuncUpdateState CSpecialManager::m_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,								// �������Ȃ��X�V
	&CSpecialManager::UpdateCutIn,			// �J�b�g�C���X�V
	&CSpecialManager::UpdatePlayerHype,		// �v���C���[����グ�X�V
	&CSpecialManager::UpdateAudienceHype,	// �ϋq����グ�X�V
	&CSpecialManager::UpdatePlayerSpecial,	// �v���C���[�X�y�V�������o�X�V
	&CSpecialManager::UpdateNormal,			// �ʏ�X�V
	&CSpecialManager::UpdateEnd,			// �I���X�V
};
CSpecialManager::AFuncUpdateSpecial CSpecialManager::m_aFuncUpdateSpecial[] =	// �X�y�V�����X�V�֐�
{
	nullptr,							// �X�y�V��������
	&CSpecialManager::UpdateKamehameha,	// ���߂͂ߔg�̍X�V
};
CSpecialManager* CSpecialManager::m_pThisClass = nullptr;	// ���g�̃C���X�^���X

//************************************************************
//	�e�N���X [CSpecialManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSpecialManager::CSpecialManager(const CPlayer* pAttack, const CPlayer* pTarget) : CObject(PRIORITY),
	m_pAttackPlayer	(pAttack),		// �U���v���C���[
	m_pTargetPlayer	(pTarget),		// �W�I�v���C���[
	m_pAttackLight	(nullptr),		// �U���v���C���[���Ƃ炷���C�g
	m_pTargetLight	(nullptr),		// �W�I�v���C���[���Ƃ炷���C�g
	m_pCutIn		(nullptr),		// �J�b�g�C�����
	m_state			(STATE_NONE),	// ���
	m_fCurTime		(0.0f)			// ���݂̑ҋ@����
{
	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CSpecialManager::STATE_MAX, "ERROR : State Count Mismatch");
	static_assert(NUM_ARRAY(m_aFuncUpdateSpecial) == CBall::SPECIAL_MAX, "ERROR : Special Count Mismatch");
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
	m_state		= STATE_CUTIN;	// ���
	m_fCurTime	= 0.0f;			// ���݂̑ҋ@����

	// ��ނ��}�l�[�W���[�ɂ���
	SetType(CObject::TYPE::TYPE_MANAGER);

	// ���E��~���ɓ�����悤�ɂ���
	SetEnablePosibleMove_WorldPause(true);

	// �U���v���C���[���Ƃ炷���C�g�̐���
	m_pAttackLight = CLightPoint::Create();
	if (m_pAttackLight == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �g�U����ݒ�
	m_pAttackLight->SetDiffuse(MyLib::color::White());

	// �����͈͂�ݒ�
	m_pAttackLight->SetRange(LIGHT_RANGE);

	// �W�I�v���C���[���Ƃ炷���C�g�̐���
	m_pTargetLight = CLightPoint::Create();
	if (m_pTargetLight == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �g�U����ݒ�
	m_pTargetLight->SetDiffuse(MyLib::color::White());

	// �����͈͂�ݒ�
	m_pTargetLight->SetRange(LIGHT_RANGE);

	// �J�b�g�C���̐���
	m_pCutIn = CCutIn::Create();
	if (m_pCutIn == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

#if 1
	// �̈�ق��Â�����
	GET_MANAGER->GetLight()->SetEnableBright(false);
#endif

#if 1
	// �U�����v���C���[�`�[���̊ϋq�𐷂�グ��
	CAudience::SetSpecialAll(m_pAttackPlayer->GetStatus()->GetTeam());
#endif

#if 1
	// ���E�̎����~�߂�
	GET_MANAGER->SerEnableWorldPaused(true);
#endif

#if 1
	// �Q�[�����X�y�V�������o�V�[���ɕύX
	CGame::GetInstance()->GetGameManager()->SetType(CGameManager::ESceneType::SCENE_SPECIAL_STAG);
#endif

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
	m_pThisClass = nullptr;
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
CSpecialManager *CSpecialManager::Create(const CPlayer* pAttack, const CPlayer* pTarget)
{
	if (m_pThisClass != nullptr)
	{ // ���N���X�̑��C���X�^���X������ꍇ

		// �C���X�^���X��j��
		SAFE_UNINIT(m_pThisClass);
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
		assert(m_pThisClass == nullptr);
		m_pThisClass = pSpecialManager;

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

		// ���E�̎��͂���������
		GET_MANAGER->SerEnableWorldPaused(false);

		// �J�b�g�C���̏I��
		SAFE_UNINIT(m_pCutIn);

		// �v���C���[����グ��Ԃɂ���
		m_state = STATE_PLAYER_HYPE;
	}

	// ���C�g�ʒu�̐ݒ�
	SetLightPosition();
}

//============================================================
//	�v���C���[����グ�̍X�V����
//============================================================
void CSpecialManager::UpdatePlayerHype(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// TODO�F�����Ƀv���C���[�J����

#if 1
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= 1.0f)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �ϋq����グ��Ԃɂ���
		m_state = STATE_AUDIENCE_HYPE;
	}
#endif

	// ���C�g�ʒu�̐ݒ�
	SetLightPosition();
}

//============================================================
//	�ϋq����グ�̍X�V����
//============================================================
void CSpecialManager::UpdateAudienceHype(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// TODO�F�����Ɋϋq�J����

#if 1
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= 1.0f)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �v���C���[���X�y�V�������[�V�����ɂ���
		m_pAttackPlayer->SetMotion(CPlayer::MOTION_SPECIAL);

		// �v���C���[�X�y�V�������o��Ԃɂ���
		m_state = STATE_PLAYER_SPECIAL;
	}
#endif

	// ���C�g�ʒu�̐ݒ�
	SetLightPosition();
}

//============================================================
//	�v���C���[�X�y�V�������o�̍X�V����
//============================================================
void CSpecialManager::UpdatePlayerSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall::ESpecial typeSpecial = CGame::GetInstance()->GetGameManager()->GetBall()->GetTypeSpecial();	// �X�y�V�������
	assert(typeSpecial > CBall::SPECIAL_NONE && typeSpecial < CBall::SPECIAL_MAX);
	if (m_aFuncUpdateState[typeSpecial] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ

		// �e�X�y�V�������Ƃ̍X�V
		(this->*(m_aFuncUpdateSpecial[typeSpecial]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// TODO�F�������u�Ԃ̉����͂������...
	if (m_pAttackPlayer->GetBall() == nullptr)
	{ // �{�[���𓊂��Ă���ꍇ

		// �I����Ԃɂ���
		m_state = STATE_END;
	}

	// ���C�g�ʒu�̐ݒ�
	SetLightPosition();
}

//============================================================
//	�ʏ�̍X�V����
//============================================================
void CSpecialManager::UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#if 1
	if (m_pAttackPlayer->GetBall() == nullptr)
	{ // �{�[���𓊂��Ă���ꍇ

		// �I����Ԃɂ���
		m_state = STATE_END;
	}
#endif

	// ���C�g�ʒu�̐ݒ�
	SetLightPosition();
}

//============================================================
//	�I���̍X�V����
//============================================================
void CSpecialManager::UpdateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �̈�ق𖾂邭����
	GET_MANAGER->GetLight()->SetEnableBright(true);

	// �U�����v���C���[�`�[���̊ϋq��ʏ��Ԃɂ���
	CAudience::SetEnableJumpAll(false, m_pAttackPlayer->GetStatus()->GetTeam());

	// �Q�[�������C���V�[���ɕύX
	CGame::GetInstance()->GetGameManager()->SetType(CGameManager::ESceneType::SCENE_MAIN);

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
//	���C�g�ʒu�̐ݒ菈��
//============================================================
void CSpecialManager::SetLightPosition()
{
	// �U���v���C���[�̈ʒu�Ɉړ�
	m_pAttackLight->SetPosition(m_pAttackPlayer->GetPosition() + LIGHT_OFFSET);

	// �W�I�v���C���[�̈ʒu�Ɉړ�
	m_pTargetLight->SetPosition(m_pTargetPlayer->GetPosition() + LIGHT_OFFSET);
}
