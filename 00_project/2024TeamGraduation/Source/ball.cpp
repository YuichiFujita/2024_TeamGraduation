//==========================================================================
// 
//  �{�[������ [ball.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "ball.h"
#include "player.h"
#include "playerBase.h"
#include "playerStatus.h"
#include "manager.h"
#include "game.h"
#include "gamemanager.h"
#include "calculation.h"
#include "model.h"
#include "ballHolderMarker.h"
#include "ballLandingMarker.h"
#include "shadow.h"
#include "specialManager.h"
#include "playerManager.h"

#include "debugproc.h"
#include "3D_Effect.h"

// TODO�FAI/User�؂�ւ�
#if 1
#define CHANGE
#endif

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char*	MODEL = "data\\MODEL\\dadgeball\\dodgeball.x";	// �{�[�����f��
	const float GRAVITY = mylib_const::GRAVITY * 0.6f;	// �{�[���ɂ�����d��
	const float	RADIUS = 17.0f;					// ���a
	const float PLUS_RADIUS = RADIUS * 1.8f;	// ����p���a
	const float	RADIUS_SHADOW = 20.0f;			// �e�̔��a
	const float	MIN_ALPHA_SHADOW = 0.32f;		// �e�̓����x
	const float	MAX_ALPHA_SHADOW = 0.48f;		// �e�̓����x
	const float	REV_MOVE = 0.025f;		// �ړ��ʂ̕␳�W��
	const float	REV_INIMOVE = 0.1f;		// �����̕␳�W��
	const float	MAX_DIS = 100000.0f;	// �z�[�~���O����ő勗��
	const int	VIEW_ANGLE = 104;		// ����p
	const float DEST_POSY = 45.0f;		// �ʏ�{�[���̖ڕWY���W
	const float REV_POSY = 0.1f;		// �ʏ�{�[���̖ڕWY���W�̕␳�W��
	const float MAX_BOUND_MOVE = 0.8f;	// �o�E���h���̏�ړ��ʍő�l

	const char* DEBUG_STATE_PRINT[] =	// �f�o�b�O�\���p���
	{
		"[SPAWN]    ������� (�t���[�{�[��)",
		"[CATCH]    �L���b�`��� (�v���C���[����)",
		"[HOM_NOR]  �ʏ�z�[�~���O��� (�U������ON)",
		"[HOM_JUMP] �W�����v�z�[�~���O��� (�U������ON)",
		"[MOVE]     �ړ���� (�U������ON)",
		"[S_STAG]   �X�y�V�������o��� (�J�n�O���o)",
		"[S_THROW]  �X�y�V����������� (�U������ON)",
		"[HOM_PASS] �z�[�~���O�p�X��� (���쁨�O��)",
		"[PASS]     �p�X��� (�O�쁨����)",
		"[REBOUND]  ���o�E���h��� (�Ԃ��������̗���)",
		"[FREE]     �t���[��� (�G�݂̂Ƃ��)",
		"[LAND]     ���n��� (�n�ʗ���)",
	};
	const char* DEBUG_TEAM_PRINT[] =	// �f�o�b�O�\���p�`�[��
	{
		"[NONE]    (�R�[�g�w��Ȃ�)",
		"[LEFT]    (���R�[�g)",
		"[RIGHT]   (�E�R�[�g)",
	};
	const char* DEBUG_ATK_PRINT[] =		// �f�o�b�O�\���p�U��
	{
		"[NONE]    (�U�����薳��)",
		"[NORMAL]  (�ʏ�U��)",
		"[JUMP]    (�W�����v�U��)",
		"[SPECIAL] (�X�y�V�����X�g�U��)",
	};
	const char* DEBUG_SPECIAL_PRINT[] =	// �f�o�b�O�\���p�X�y�V����
	{
		"[NONE]    (�w��Ȃ�)",
		"[���߂͂ߔg]",
	};
	const char* DEBUG_BOOL_PRINT[] =	// �f�o�b�O�\���p�t���O
	{
		"[FALSE]",
		"[TRUE]",
	};

	namespace normal
	{
		const float THROW_MOVE = 22.5f;		// �ʏ퓊���ړ����x
		const float REV_HOMING = 0.035f;	// �z�[�~���O�̊����␳�W��
		const float TIME_HOMING = 1.2f;		// �z�[�~���O���؂��܂ł̎���
	}

	namespace jump
	{
		const float THROW_MOVE = 27.0f;		// �W�����v�����ړ����x
		const float REV_HOMING = 0.24f;		// �z�[�~���O�̊����␳�W��
		const float MIN_MOVE_DOWN = -0.3f;	// �W�����v�U���̍Œቺ�ړ���
		const float OFFSET_TARGET_BACK = 150.0f;	// �^�[�Q�b�g�̌��I�t�Z�b�g
	}

	namespace special
	{
		const int	TIMING_ATK_IDX = 0;	// �X�y�V�����{�[���̓�����^�C�~���O
		const float	THROW_MOVE = 20.0f;	// �X�y�V���������ړ����x
	}

	namespace kamehameha
	{
		const float REV_HOMING = 0.46f;	// �z�[�~���O�̊����␳�W��
	}

	namespace move
	{
		const float TIME_GRAVITY = 0.8f;		// �d�͂�������n�߂�܂ł̎���
		const float MULTIPLY_INIMOVE = 2.8f;	// �����̔{��
	}

	namespace rebound
	{
#if _DEBUG
		const float MOVE_UP = 10.5f;	// ��ړ���
#else
		const float MOVE_UP = 5.5f;		// ��ړ���
#endif
		const float MOVE_SPEED = 2.5f;	// �ړ����x
	}

	namespace toss
	{
		const float THROW_MOVE = 3.5f;	// �g�X�ړ����x
		const float MOVE_UP = 2.5f;		// �g�X��ړ���
	}

	namespace pass
	{
		const float MOVE_RATE = 0.015f;			// �ړ����x�̋�������
		const float HOMING_TIMERATE = 0.65f;	// Y���W�̃z�[�~���O���s�����Ԋ���
		const float TARGET_PULSY = 50.0f;		// �^�[�Q�b�g�Ώۂ�Y���W���Z��
		const float TIME_NORAML = 1.2f;			// �ʏ�p�X�̌o�ߎ���
		const float TIME_HOMING = 1.0f;			// �z�[�~���O�p�X�̌o�ߎ���
		const float MAX_HEIGHT = 250.0f;		// �z�[�~���O�̍ō����B�_
	}

	namespace spawn
	{
		const float THROW_MOVE = 3.5f;	// �X�|�[���ړ����x
		const float MOVE_UP = 6.5f;		// �X�|�[����ړ���
	}
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
// ��Ԋ֐�
CBall::STATE_FUNC CBall::m_StateFuncList[] =
{
	&CBall::UpdateSpawn,		// ������Ԃ̍X�V
	&CBall::UpdateCatch,		// �L���b�`��Ԃ̍X�V
	&CBall::UpdateHomingNormal,	// �ʏ�z�[�~���O��Ԃ̍X�V
	&CBall::UpdateHomingJump,	// �W�����v�z�[�~���O��Ԃ̍X�V
	&CBall::UpdateMove,			// �ړ���Ԃ̍X�V
	&CBall::UpdateSpecialStag,	// �X�y�V�������o��Ԃ̍X�V
	&CBall::UpdateSpecialThrow,	// �X�y�V����������Ԃ̍X�V
	&CBall::UpdateHomingPass,	// �p�X�z�[�~���O��Ԃ̍X�V
	&CBall::UpdatePass,			// �p�X��Ԃ̍X�V
	&CBall::UpdateReBound,		// ���o�E���h��Ԃ̍X�V
	&CBall::UpdateFree,			// �t���[��Ԃ̍X�V
	&CBall::UpdateLand,			// ���n��Ԃ̍X�V
};

// �X�y�V�����֐�
CBall::SPECIAL_FUNC CBall::m_SpecialFuncList[] =
{
	nullptr,					// �X�y�V��������
	&CBall::UpdateKamehameha,	// ���߂͂ߔg�̍X�V
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CBall> CBall::m_list = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBall::CBall(int nPriority) : CObjectX(nPriority),
	m_typeTeam		(CGameManager::SIDE_NONE),	// �`�[���T�C�h
	m_pHoldMarker	(nullptr),		// �{�[�������}�[�J�[���
	m_pLandMarker	(nullptr),		// �{�[�����n�_�}�[�J�[���
	m_pShadow		(nullptr),		// �e���
	m_pPlayer		(nullptr),		// �v���C���[���
	m_pTarget		(nullptr),		// �z�[�~���O�^�[�Q�b�g���
	m_pCover		(nullptr),		// �J�o�[�Ώۃv���C���[���
	m_fMoveSpeed	(0.0f),			// �ړ����x
	m_fGravity		(0.0f),			// �d��
	m_fHomingTime	(0.0f),			// �z�[�~���O����
	m_bLanding		(false),		// ���n�t���O
	m_posPassStart	(VEC3_ZERO),	// �p�X�J�n�ʒu
	m_posPassEnd	(VEC3_ZERO),	// �p�X�I���ʒu
	m_typeSpecial	(SPECIAL_NONE),	// �X�y�V�������
	m_typeAtk		(ATK_NONE),		// �U�����
	m_state			(STATE_SPAWN),	// ���
	m_fStateTime	(0.0f),			// ��ԃJ�E���^�[
	m_pThrowLine	(nullptr),		// �����̃��C��
	m_pAura			(nullptr),		// �I�[��
	m_nDamage		(0),			// �_���[�W
	m_fKnockback	(0.0f)			// �m�b�N�o�b�N
{
	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_StateFuncList)   == CBall::STATE_MAX,   "ERROR : State Count Mismatch");
	static_assert(NUM_ARRAY(m_SpecialFuncList) == CBall::SPECIAL_MAX, "ERROR : Special Count Mismatch");

	static_assert(NUM_ARRAY(DEBUG_STATE_PRINT)   == CBall::STATE_MAX,   "ERROR : State Count Mismatch");
	static_assert(NUM_ARRAY(DEBUG_ATK_PRINT)     == CBall::ATK_MAX,     "ERROR : Attack Count Mismatch");
	static_assert(NUM_ARRAY(DEBUG_SPECIAL_PRINT) == CBall::SPECIAL_MAX, "ERROR : Special Count Mismatch");
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBall::~CBall()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CBall *CBall::Create(const MyLib::Vector3& rPos)
{
	// �������̊m��
	CBall* pObj = DEBUG_NEW CBall;
	if (pObj != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pObj->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pObj);
			return nullptr;
		}

		// �ʒu��ݒ�
		pObj->SetPosition(rPos);
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CBall::Init()
{
	// ���X�g�ɒǉ�
	m_list.Regist(this);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// �e�N���X�̏�����
	HRESULT hr = CObjectX::Init(MODEL);
	if (FAILED(hr)) { return E_FAIL; }

	// �{�[�������}�[�J�[�̐���
	m_pHoldMarker = CBallHolderMarker::Create(nullptr);
	if (m_pHoldMarker == nullptr) { return E_FAIL; }

	// �{�[�����n�_�}�[�J�[�̐���
	m_pLandMarker = CBallLandingMarker::Create(this);
	if (m_pLandMarker == nullptr) { return E_FAIL; }

	// �e�̐���
	m_pShadow = CShadow::Create(this, RADIUS_SHADOW, MIN_ALPHA_SHADOW, MAX_ALPHA_SHADOW);
	if (m_pShadow == nullptr) { return E_FAIL; }

	// �I�[������
	m_pAura = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_BALL,
			GetPosition(),
			MyLib::Vector3(),
			MyLib::Vector3(),
			15.0f, false);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CBall::Uninit()
{
	// ���X�g����폜
	m_list.Delete(this);

	// �e�N���X�̏I��
	CObjectX::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CBall::Kill()
{
	// �e�̍폜
	SAFE_KILL(m_pShadow);

	// ���X�g����폜
	m_list.Delete(this);

	// �e�N���X�̏I��
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CBall::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �O��̈ʒu���X�V
	SetOldPosition(GetPosition());

	// ���n���Ă��Ȃ���Ԃɂ���
	m_bLanding = false;

	// �����ړ��ʂ̍X�V
	if (IsAttack())
	{ // �U�����̏ꍇ

		// ����������������
		m_fInitialSpeed += (0.0f - m_fInitialSpeed) * (REV_INIMOVE * fDeltaRate * fSlowRate);
	}

	if (m_StateFuncList[m_state] != nullptr)
	{ // ��ԍX�V�֐�������ꍇ

		// ��ԕʏ���
		(this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// ���݂̃`�[���T�C�h���X�V
	UpdateTypeTeam();

	// ���݂̍U����ނ��X�V
	UpdateTypeAtk();

	// ���݂̃X�y�V������ނ��X�V
	UpdateTypeSpecial();

	// �����G�t�F�N�g
	UpdateThrowLine();

	// �e�N���X�̍X�V
	CObjectX::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �J�������̃e�L�X�g�`��
	GET_MANAGER->GetDebugProc()->Print
	(
		"\n---------------- �{�[����� ----------------\n"
		"�y�{�[����ԁz%s\n"
		"�y�`�[�������z%s\n"
		"�y �U����� �z%s\n"
		"�y�X�y�V�����z%s\n"
		"�y���n�t���O�z%s\n"
		"�y ���L�Ώ� �z[%s]\n"
		"�y�^�[�Q�b�g�z[%s]\n"
		"�y�J�o�[�Ώہz[%s]\n",
		DEBUG_STATE_PRINT[m_state],
		DEBUG_TEAM_PRINT[m_typeTeam + 1],
		DEBUG_ATK_PRINT[m_typeAtk],
		DEBUG_SPECIAL_PRINT[m_typeSpecial],
		DEBUG_BOOL_PRINT[(int)m_bLanding],
		(m_pPlayer == nullptr) ? "nullptr" : "player",
		(m_pTarget == nullptr) ? "nullptr" : "player",
		(m_pCover  == nullptr) ? "nullptr" : "player"
	);


#if _DEBUG
	// �f�o�b�O�o��
	if (ImGui::TreeNode("Ball Info"))
	{
		MyLib::Vector3 pos = GetPosition();
		MyLib::Vector3 move = GetMove();
		ImGui::Text("pos : [X : %.2f, Y : %.2f, Z : %.2f]", pos.x, pos.y, pos.z);
		ImGui::Text("move : [X : %.2f, Y : %.2f, Z : %.2f]", move.x, move.y, move.z);
		ImGui::Text("m_fMoveSpeed : [%.2f]", m_fMoveSpeed);
		ImGui::Text("m_fGravity : [%.2f]", m_fGravity);
		ImGui::Text("m_nDamage : [%d]", m_nDamage);

		if (ImGui::Button("Reset"))
		{// ���Z�b�g
			pos = MyLib::Vector3();
			m_state = EState::STATE_LAND;

			SetPosition(pos);
		}

		ImGui::TreePop();
	}

#endif
}

//==========================================================================
// �`�揈��
//==========================================================================
void CBall::Draw()
{
	// �e�N���X�̕`��
	CObjectX::Draw();
}

//==========================================================================
// �U���L���b�`����
//==========================================================================
void CBall::CatchAttack(CPlayer* pPlayer)
{
	// �N�������łɎ����Ă���{�[���̏ꍇ������
	if (m_pPlayer != nullptr) { return; }

	MyLib::Vector3 oldPosBall = GetOldPosition();				// �{�[���ߋ��ʒu
	MyLib::Vector3 posPlayer = pPlayer->GetPosition();			// �v���C���[�ʒu
	float fAngleY = oldPosBall.AngleXZ(posPlayer) - D3DX_PI;	// �{�[������

	// �L���b�`����
	Catch(pPlayer);

	// �ڕW�������{�[�������ɂ���
	m_pPlayer->SetRotDest(fAngleY);
}

//==========================================================================
// ���n�L���b�`����
//==========================================================================
void CBall::CatchLand(CPlayer* pPlayer)
{
	// �N�������łɎ����Ă���{�[���̏ꍇ������
	if (m_pPlayer != nullptr) { return; }

	// �L���b�`����
	Catch(pPlayer);
}

//==========================================================================
// �X�|�[������
//==========================================================================
void CBall::Spawn(CPlayer* pPlayer)
{
	// ��������
	Throw(pPlayer);

	// ������Ԃɂ���
	SetState(STATE_SPAWN);

	// ���ʒu��������
	MyLib::Vector3 pos = GetPosition();
	pos.x = pos.z = 0.0f;
	SetPosition(pos);

	// ��ړ��ʂ�^����
	MyLib::Vector3 move = GetMove();
	move.x = move.z = 0.0f;
	move.y = spawn::MOVE_UP;
	SetMove(move);

	// �ړ��ʂ�ݒ�
	m_fMoveSpeed = spawn::THROW_MOVE;

	// �T�E���h�Đ�
	PLAY_SOUND(CSound::ELabel::LABEL_SE_THROW_NORMAL);
}

//==========================================================================
// �ʏ퓊������
//==========================================================================
void CBall::ThrowNormal(CPlayer* pPlayer)
{
	// �L���b�`���Ă��Ȃ��{�[���𓊂��悤�Ƃ����ꍇ�G���[
	assert(m_state == STATE_CATCH);

	// �z�[�~���O�Ώۂ̐ݒ�
	m_pTarget = CollisionThrowTarget();
	if (m_pTarget != nullptr)
	{ // �^�[�Q�b�g������ꍇ

		MyLib::Vector3 posPlayer = pPlayer->GetPosition();		// �{�[���ߋ��ʒu
		MyLib::Vector3 posTarget = m_pTarget->GetPosition();	// �v���C���[�ʒu
		float fAngleY = posPlayer.AngleXZ(posTarget);			// �{�[������

		// �z�[�~���O��Ԃɂ���
		SetState(STATE_HOM_NOR);

		// �ڕW����/�������{�[�������ɂ���
		m_pPlayer->SetRotDest(fAngleY);
		m_pPlayer->SetRotation(MyLib::Vector3(0.0f, fAngleY, 0.0f));

		// ��������
		Throw(pPlayer);

		// �ړ��x�N�g���𐳋K�����čĐݒ�
		SetMove(CalcVecMove(m_pTarget, pPlayer).Normal());
	}
	else
	{ // �^�[�Q�b�g�����Ȃ��ꍇ

		// ��������
		Throw(pPlayer);

		// �ړ���Ԃɂ���
		SetState(STATE_MOVE);
	}

#ifdef CHANGE
	if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT)
	{ // �O�삪�������ꍇ

		// �߂���AI�ɑ��쌠���ڂ��A���g��AI�ɂ���
		CPlayerManager::GetInstance()->ChangeUserToAI(pPlayer);
	}
#endif

	// �ʏ�U����ݒ�
	m_typeAtk = ATK_NORMAL;

	// �_���[�W�ݒ�
	m_nDamage = pPlayer->GetBallParameter().nDamageNormal;

	// �m�b�N�o�b�N�ݒ�
	m_fKnockback = pPlayer->GetBallParameter().fKnockbackNormal;
	
	// �ړ��ʐݒ�
	m_fMoveSpeed = pPlayer->GetBallParameter().fThrowMoveNormal;

	// ������ݒ�
	CalcSetInitialSpeed(m_fMoveSpeed);

	// �T�E���h�Đ�
	PLAY_SOUND(CSound::ELabel::LABEL_SE_THROW_NORMAL);
}

//==========================================================================
// �W�����v��������
//==========================================================================
void CBall::ThrowJump(CPlayer* pPlayer)
{
	// �L���b�`���Ă��Ȃ��{�[���𓊂��悤�Ƃ����ꍇ�G���[
	assert(m_state == STATE_CATCH);

	// �z�[�~���O�Ώۂ̐ݒ�
	m_pTarget = CollisionThrowTarget();
	if (m_pTarget != nullptr)
	{ // �^�[�Q�b�g������ꍇ

		MyLib::Vector3 posPlayer = pPlayer->GetPosition();		// �{�[���ߋ��ʒu
		MyLib::Vector3 posTarget = m_pTarget->GetPosition();	// �v���C���[�ʒu
		float fAngleY = posPlayer.AngleXZ(posTarget);			// �{�[������

		// �z�[�~���O��Ԃɂ���
		SetState(STATE_HOM_JUMP);

		// �ڕW����/�������{�[�������ɂ���
		m_pPlayer->SetRotDest(fAngleY);
		m_pPlayer->SetRotation(MyLib::Vector3(0.0f, fAngleY, 0.0f));

		// ��������
		Throw(pPlayer);
	}
	else
	{ // �^�[�Q�b�g�����Ȃ��ꍇ

		// ��������
		Throw(pPlayer);

		// �ړ��x�N�g���̎擾
		MyLib::Vector3 vecMove = GetMove();

		// �������Ɉړ��x�N�g����^����
		vecMove.y = jump::MIN_MOVE_DOWN;

		// �ړ��x�N�g���𐳋K�����Đݒ�
		SetMove(vecMove.Normal());

		// �ړ���Ԃɂ���
		SetState(STATE_MOVE);
	}

#ifdef CHANGE
	if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT)
	{ // �O�삪�������ꍇ

		// �߂���AI�ɑ��쌠���ڂ��A���g��AI�ɂ���
		CPlayerManager::GetInstance()->ChangeUserToAI(pPlayer);
	}
#endif

	// �W�����v�U����ݒ�
	m_typeAtk = ATK_JUMP;

	// �_���[�W�ݒ�
	m_nDamage = pPlayer->GetBallParameter().nDamageJump;

	// �m�b�N�o�b�N�ݒ�
	m_fKnockback = pPlayer->GetBallParameter().fKnockbackJump;

	// �ړ��ʐݒ�
	m_fMoveSpeed = pPlayer->GetBallParameter().fThrowMoveJump;

	// ������ݒ�
	CalcSetInitialSpeed(m_fMoveSpeed);

	// �T�E���h�Đ�
	PLAY_SOUND(CSound::ELabel::LABEL_SE_THROW_JUMP);
}

//==========================================================================
// �X�y�V������������
//==========================================================================
void CBall::Special(CPlayer* pPlayer)
{
	// �z�[�~���O�Ώۂ̐ݒ�
	m_pTarget = CollisionThrowTarget(true);
	assert(m_pTarget != nullptr);	// ������Ȃ���G�����Ȃ���`

	// �X�y�V�������o��Ԃɂ���
	SetState(STATE_SPECIAL_STAG);

	// TODO�F�X�y�V�����̎�ސݒ�
	m_typeSpecial = SPECIAL_KAMEHAMEHA;

	// �_���[�W�ݒ�
	m_nDamage = pPlayer->GetBallParameter().nDamageSpecial;

	// �m�b�N�o�b�N�ݒ�
	m_fKnockback = pPlayer->GetBallParameter().fKnockbackSpecial;
	
	// �X�y�V�������o�}�l�[�W���[�̐���
	CSpecialManager::Create(m_pPlayer, m_pTarget);
}

//==========================================================================
// �p�X����
//==========================================================================
void CBall::Pass(CPlayer* pPlayer)
{
	// �L���b�`���Ă��Ȃ��{�[���𓊂��悤�Ƃ����ꍇ�G���[
	assert(m_state == STATE_CATCH);

	// �p�X�Ώۂ̐ݒ�
	m_pTarget = CollisionPassTarget();
	if (m_pTarget != nullptr)
	{ // �^�[�Q�b�g������ꍇ

		MyLib::Vector3 posPlayer = pPlayer->GetPosition();		// �{�[���ߋ��ʒu
		MyLib::Vector3 posTarget = m_pTarget->GetPosition();	// �v���C���[�ʒu
		float fAngleY = posPlayer.AngleXZ(posTarget);			// �{�[������

		// �ڕW����/�������{�[�������ɂ���
		m_pPlayer->SetRotDest(fAngleY);
		m_pPlayer->SetRotation(MyLib::Vector3(0.0f, fAngleY, 0.0f));
	}

	// �^�[�Q�b�g�����Ȃ��ꍇ�G���[
	assert(m_pTarget != nullptr);

	// ��������
	Throw(pPlayer);

	// ���݂̃{�[���ʒu���p�X�J�n�ʒu�ɂ���
	m_posPassStart = GetPosition();

	if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_IN)
	{ // ����̏ꍇ

		// �x�[�X�̓���ւ�
		CPlayerManager::GetInstance()->SwapBase(pPlayer, m_pTarget);

		// �z�[�~���O�p�X��Ԃɂ���
		SetState(STATE_HOM_PASS);

		// �p�X�I��Y���W��ݒ�
		m_posPassEnd.y = CGameManager::FIELD_LIMIT + pass::TARGET_PULSY;
	}
	else
	{ // �O��̏ꍇ

		if (m_pTarget->GetBaseType() == CPlayer::EBaseType::TYPE_AI)
		{ // �p�X�^�[�Q�b�g��AI�̏ꍇ

			// �x�[�X�̓���ւ�
			CPlayerManager::GetInstance()->SwapBase(pPlayer, m_pTarget);
		}
		else
		{ // �p�X�^�[�Q�b�g�����[�U�[�̏ꍇ

			// �߂���AI�ɑ��쌠���ڂ��A���g��AI�ɂ���
			CPlayerManager::GetInstance()->ChangeUserToAI(pPlayer);
		}

		// �p�X��Ԃɂ���
		SetState(STATE_PASS);

		// �p�X�I���ʒu��ݒ�
		m_posPassEnd = m_pTarget->GetPosition();	// ���݂̃^�[�Q�b�g�ʒu
		m_posPassEnd.y = CGameManager::FIELD_LIMIT;	// Y���W�͒n�ʌŒ�

		// �ړ��ʂ�ݒ�
		m_fMoveSpeed = m_posPassStart.Distance(m_posPassEnd) * pass::MOVE_RATE;
	}

	// �T�E���h�Đ�
	PLAY_SOUND(CSound::ELabel::LABEL_SE_THROW_NORMAL);
}

//==========================================================================
// �g�X����
//==========================================================================
void CBall::Toss(CPlayer* pPlayer)
{
	// ��������
	Throw(pPlayer);

	// �t���[��Ԃɂ���
	SetState(STATE_FREE);

	// ��ړ��ʂ�^����
	MyLib::Vector3 move = GetMove();
	move.y = toss::MOVE_UP;
	SetMove(move);

	// �ړ��ʂ�ݒ�
	m_fMoveSpeed = toss::THROW_MOVE;

	// �T�E���h�Đ�
	PLAY_SOUND(CSound::ELabel::LABEL_SE_THROW_NORMAL);
}

//==========================================================================
// ���a�̎擾����
//==========================================================================
float CBall::GetRadius() const
{
	// ���a��Ԃ�
	return RADIUS;
}

//==========================================================================
// �U���t���O�̎擾����
//==========================================================================
bool CBall::IsAttack() const
{
	// �U���t���O��Ԃ�
	return (m_state == STATE_HOM_NOR || m_state == STATE_HOM_JUMP || m_state == STATE_MOVE || m_state == STATE_SPECIAL_THROW);	// TODO�F�U����Ԃ���������ǉ�
}

//==========================================================================
// �p�X�t���O�̎擾����
//==========================================================================
bool CBall::IsPass() const
{
	// �p�X�t���O��Ԃ�
	return (m_state == STATE_HOM_PASS || m_state == STATE_PASS);	// TODO�F�p�X��Ԃ���������ǉ�
}

//==========================================================================
// �X�y�V�����t���O�̎擾����
//==========================================================================
bool CBall::IsSpecial() const
{
	// �X�y�V�����t���O��Ԃ�
	return (m_state == STATE_SPECIAL_STAG || m_state == STATE_SPECIAL_THROW);	// TODO�F�X�y�V������Ԃ���������ǉ�
}

//==========================================================================
// ���[���h�}�g���b�N�X�̌v�Z����
//==========================================================================
void CBall::CalWorldMtx()
{
	if (m_state == STATE_CATCH || m_state == STATE_SPECIAL_STAG)
	{ // �L���b�`���̏ꍇ

		const int nPartsIdx = m_pPlayer->GetParameter().nBallPartsIdx;		// �{�[���p�[�c�C���f�b�N�X
		const MyLib::Vector3 offset = m_pPlayer->GetParameter().ballOffset;	// �{�[���I�t�Z�b�g
		MyLib::Matrix mtxParts = m_pPlayer->GetModel(nPartsIdx)->GetWorldMtx();	// �p�[�c�}�g���b�N�X
		MyLib::Vector3 rot = GetRotation();			// ���g�̌���
		MyLib::Vector3 scale = GetScale();			// ���g�̊g�嗦
		MyLib::Matrix mtxWorld = GetWorldMtx();		// ���g�̃��[���h�}�g���b�N�X
		MyLib::Matrix mtxRot, mtxTrans, mtxScale;	// �v�Z�p�}�g���b�N�X�錾

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld.Identity();

		// �X�P�[���𔽉f����
		mtxScale.Scaling(scale);
		mtxWorld.Multiply(mtxWorld, mtxScale);

		// �����𔽉f����
		mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
		mtxWorld.Multiply(mtxWorld, mtxRot);

		// �ʒu�𔽉f����
		mtxTrans.Translation(offset);
		mtxParts.Multiply(mtxTrans, mtxParts);

		// �}�g���b�N�X�𔽉f
		SetWorldMtx(mtxParts);

		// �L���b�`���̃}�g���b�N�X����ʒu�𔽉f
		SetPosition(mtxParts.GetWorldPosition());

		// �e�̕`����~
		m_pShadow->SetEnableDisp(false);
	}
	else
	{ // �L���b�`���Ă��Ȃ��ꍇ

		// �e�N���X�̃��[���h�}�g���b�N�X�v�Z
		CObjectX::CalWorldMtx();

		// �e�̕`����ĊJ
		m_pShadow->SetEnableDisp(true);
	}
}

//==========================================================================
// ������Ԃ̍X�V����
//==========================================================================
void CBall::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 vecMove = GetMove();	// �ړ���

	// �ړ�
	UpdateMove(&pos, &vecMove, fDeltaRate, fSlowRate);

	// �n�ʂ̒��n
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // ���n�����ꍇ

		// ���n�J��
		Landing();
	}

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(vecMove);	// �ړ���

	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer(&pos);
}

//==========================================================================
// �L���b�`��Ԃ̍X�V����
//==========================================================================
void CBall::UpdateCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �ʏ�z�[�~���O��Ԃ̍X�V����
//==========================================================================
void CBall::UpdateHomingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 vecMove = GetMove();	// �ړ���
	MyLib::Vector3 posTarget = m_pTarget->GetCenterPosition();	// �^�[�Q�b�g���S�ʒu

	// �ڕW�x�N�g�����擾
	MyLib::Vector3 vecDest = posTarget - pos;
	vecDest = vecDest.Normal();

	// �����x�N�g�����v�Z
	MyLib::Vector3 vecDiff = vecDest - vecMove;
	vecDiff = vecDiff.Normal();

	// �ړ��x�N�g�����X�V
	vecMove.x += vecDiff.x * normal::REV_HOMING;
	vecMove.z += vecDiff.z * normal::REV_HOMING;
	vecMove = vecMove.Normal();

	// �ʒu�Ɉړ��ʂ𔽉f
	UpdateMovePosition(&pos, &vecMove, fDeltaRate, fSlowRate);

	// �o�ߎ��Ԃ����Z
	m_fStateTime += fDeltaTime;
	if (m_fStateTime >= m_fHomingTime)	// TODO�F������ɍs���������痎���Ă��鏈�����K�v����
	{
		// �ړ���Ԃɂ���
		SetState(STATE_MOVE);
	}

	// �n�ʂ̒��n
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // ���n�����ꍇ

		// ���n�J��
		Landing();
	}

	// �v���C���[�Ƃ̓����蔻��
	CPlayer* pHit = CollisionPlayer(&pos);
	if (pHit != nullptr)
	{ // ���������ꍇ

		// ���o�E���h�J��
		ReBound(pHit, &vecMove);
	}

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(vecMove);	// �ړ���
}

//==========================================================================
// �W�����v�z�[�~���O��Ԃ̍X�V����
//==========================================================================
void CBall::UpdateHomingJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 vecMove = GetMove();	// �ړ���
	MyLib::Vector3 posTarget = m_pTarget->GetPosition();	// �^�[�Q�b�g�ʒu
	MyLib::Vector3 rotTarget = m_pTarget->GetRotation();	// �^�[�Q�b�g����

	// �^�[�Q�b�g�ʒu��Y���W�͒n�ʌŒ�ɂ���
	posTarget.y = CGameManager::FIELD_LIMIT;

	// �^�[�Q�b�g�̏������ɂ���
	const float fTargetAngle = posTarget.AngleXZ(pos);
	posTarget.x += sinf(fTargetAngle) * jump::OFFSET_TARGET_BACK;
	posTarget.z += cosf(fTargetAngle) * jump::OFFSET_TARGET_BACK;

	// �ڕW�x�N�g�����擾
	MyLib::Vector3 vecDest = posTarget - pos;
	vecDest = vecDest.Normal();

	// �����x�N�g�����v�Z
	MyLib::Vector3 vecDiff = vecDest - vecMove;
	vecDiff = vecDiff.Normal();

	// �ړ��x�N�g�����X�V
	vecMove += vecDiff * jump::REV_HOMING;
	if (vecMove.y >= jump::MIN_MOVE_DOWN)
	{
		// �ő�l�ɕ␳
		vecMove.y = jump::MIN_MOVE_DOWN;
	}

	// �x�N�g���𐳋K��
	vecMove = vecMove.Normal();

	// �ʒu�Ɉړ��ʂ𔽉f
	UpdateMovePosition(&pos, &vecMove, fDeltaRate, fSlowRate);

	// �n�ʂ̒��n
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // ���n�����ꍇ

		// ���n�J��
		Landing();
	}

	// �v���C���[�Ƃ̓����蔻��
	CPlayer* pHit = CollisionPlayer(&pos);
	if (pHit != nullptr)
	{ // ���������ꍇ

		// ���o�E���h�J��
		ReBound(pHit, &vecMove);
	}

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(vecMove);	// �ړ���
}

//==========================================================================
// �ړ���Ԃ̍X�V����
//==========================================================================
void CBall::UpdateMove(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 vecMove = GetMove();	// �ړ���

	// �ʒu�Ɉړ��ʂ𔽉f
	UpdateMovePosition(&pos, &vecMove, fDeltaRate, fSlowRate);

	// �d�͂̉���
	UpdateGravity(fDeltaRate, fSlowRate);

	// �ʒu�ɏd�͔��f
	UpdateGravityPosition(&pos, &vecMove, fDeltaRate, fSlowRate);

	// �n�ʂ̒��n
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // ���n�����ꍇ

		// ���n�J��
		Landing();
	}

	// �v���C���[�Ƃ̓����蔻��
	CPlayer* pHit = CollisionPlayer(&pos);
	if (pHit != nullptr)
	{ // ���������ꍇ

		// ���o�E���h�J��
		ReBound(pHit, &vecMove);
	}

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(vecMove);	// �ړ���
}

//==========================================================================
// �X�y�V�������o��Ԃ̍X�V����
//==========================================================================
void CBall::UpdateSpecialStag(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CMotion* pMotion = m_pPlayer->GetMotion();	// �v���C���[���[�V�������

	// ���[�V�������X�y�V��������Ȃ��ꍇ������
	if (!pMotion->IsSpecial()) { return; }

	if (pMotion->IsImpactFrame(special::TIMING_ATK_IDX))
	{ // �U���^�C�~���O���K�ꂽ�ꍇ

		// �X�y�V��������
		ThrowSpecial();
	}
}

//==========================================================================
// �X�y�V����������Ԃ̍X�V����
//==========================================================================
void CBall::UpdateSpecialThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_SpecialFuncList[m_typeSpecial] != nullptr)
	{ // �X�y�V�����X�V�֐�������ꍇ

		// �X�y�V�����ʏ���
		(this->*(m_SpecialFuncList[m_typeSpecial]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �p�X�z�[�~���O��Ԃ̍X�V����
//==========================================================================
void CBall::UpdateHomingPass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 posBall = GetPosition();					// �{�[���ʒu
	MyLib::Vector3 posTarget = m_pTarget->GetPosition();	// �^�[�Q�b�g�ʒu
	MyLib::Vector3 vecMove = GetMove();						// �ړ���

	// �o�ߎ��Ԃ����Z
	m_fStateTime += fDeltaTime * fSlowRate;
	if (m_fStateTime <= pass::TIME_HOMING)
	{
		// �o�ߎ��Ԃ̊������v�Z
		float fTimeRate = m_fStateTime / pass::TIME_HOMING;
		fTimeRate = UtilFunc::Transformation::Clamp(fTimeRate, 0.0f, 1.0f);	// ������␳

		// XZ���ʂ̈ʒu���^�[�Q�b�g�ʒu�Ɠ���ɂ���
		m_posPassEnd.x = posTarget.x;
		m_posPassEnd.z = posTarget.z;

		// �p�X�I��Y���W���X�V
		if (m_posPassEnd.y < posTarget.y + pass::TARGET_PULSY && fTimeRate < pass::HOMING_TIMERATE)
		{ // ���݂̃p�X�I��Y���W���^�[�Q�b�g�ʒu���Ⴂ���A�o�ߎ��Ԃ̊������O���̏ꍇ

			// ���݂̃^�[�Q�b�g�ʒu�ɍĐݒ�
			m_posPassEnd.y = posTarget.y + pass::TARGET_PULSY;
		}

		// ��������Ɉʒu��␳
		posBall = UtilFunc::Calculation::GetParabola3D(m_posPassStart, m_posPassEnd, pass::MAX_HEIGHT, fTimeRate);
	}
	else
	{
		// �d�͂̉���
		UpdateGravity(fDeltaRate, fSlowRate);

		// �ʒu�ɏd�͔��f
		UpdateGravityPosition(&posBall, &vecMove, fDeltaRate, fSlowRate);
	}

	// �n�ʂ̒��n
	if (UpdateLanding(&posBall, &vecMove, fDeltaRate, fSlowRate))
	{ // ���n�����ꍇ

		// ���n�J��
		Landing();
	}

	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer(&posBall);

	// ���𔽉f
	SetPosition(posBall);	// �ʒu
	SetMove(vecMove);		// �ړ���
}

//==========================================================================
// �p�X��Ԃ̍X�V����
//==========================================================================
void CBall::UpdatePass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �{�[���ʒu
	MyLib::Vector3 vecMove = GetMove();	// �ړ���

	// �o�ߎ��Ԃ����Z
	m_fStateTime += fDeltaTime * fSlowRate;
	if (m_fStateTime <= pass::TIME_NORAML)
	{
		// �o�ߎ��Ԃ̊������v�Z
		float fTimeRate = m_fStateTime / pass::TIME_NORAML;
		fTimeRate = UtilFunc::Transformation::Clamp(fTimeRate, 0.0f, 1.0f);	// ������␳

		// ��������Ɉʒu��␳
		pos = UtilFunc::Calculation::GetParabola3D(m_posPassStart, m_posPassEnd, pass::MAX_HEIGHT, fTimeRate);
	}
	else
	{
		// �d�͂̉���
		UpdateGravity(fDeltaRate, fSlowRate);

		// �ʒu�ɏd�͔��f
		UpdateGravityPosition(&pos, &vecMove, fDeltaRate, fSlowRate);
	}

	// �n�ʂ̒��n
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // ���n�����ꍇ

		// ���n�J��
		Landing();
	}

	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer(&pos);

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(vecMove);	// �ړ���
}

//==========================================================================
// ���o�E���h��Ԃ̍X�V����
//==========================================================================
void CBall::UpdateReBound(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 vecMove = GetMove();	// �ړ���

	// �ړ�
	UpdateMove(&pos, &vecMove, fDeltaRate, fSlowRate);

	// �n�ʂ̒��n
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // ���n�����ꍇ

		// ���n�J��
		Landing();
	}

	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer(&pos);

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(vecMove);	// �ړ���
}

//==========================================================================
// �t���[��Ԃ̍X�V����
//==========================================================================
void CBall::UpdateFree(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 vecMove = GetMove();	// �ړ���

	// �ړ�
	UpdateMove(&pos, &vecMove, fDeltaRate, fSlowRate);

	// �n�ʂ̒��n
	UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate);

	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer(&pos);

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(vecMove);	// �ړ���
}

//==========================================================================
// ���n��Ԃ̍X�V����
//==========================================================================
void CBall::UpdateLand(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 vecMove = GetMove();	// �ړ���

	// �ړ�
	UpdateMove(&pos, &vecMove, fDeltaRate, fSlowRate);

	// �n�ʂ̒��n
	UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate);

	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer(&pos);

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(vecMove);	// �ړ���
}

//==========================================================================
// ���߂͂ߔg�̍X�V����
//==========================================================================
void CBall::UpdateKamehameha(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 vecMove = GetMove();	// �ړ���
	MyLib::Vector3 posTarget = m_pTarget->GetCenterPosition();	// �^�[�Q�b�g���S�ʒu
	MyLib::Vector3 rotTarget = m_pTarget->GetRotation();		// �^�[�Q�b�g����

	// �ڕW�x�N�g�����擾
	MyLib::Vector3 vecDest = posTarget - pos;
	vecDest = vecDest.Normal();

	// �����x�N�g�����v�Z
	MyLib::Vector3 vecDiff = vecDest - vecMove;
	vecDiff = vecDiff.Normal();

	// �ړ��x�N�g�����X�V
	vecMove += vecDiff * kamehameha::REV_HOMING;

	// �x�N�g���𐳋K��
	vecMove = vecMove.Normal();

	// �ʒu�Ɉړ��ʂ𔽉f
	UpdateMovePosition(&pos, &vecMove, fDeltaRate, fSlowRate);

#if 0
	// �n�ʂ̒��n
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // ���n�����ꍇ

		// ���n�J��
		Landing();
	}
#endif

	// �v���C���[�Ƃ̓����蔻��
	CPlayer* pHit = CollisionPlayer(&pos);
	if (pHit != nullptr)
	{ // ���������ꍇ

		// ���o�E���h�J��
		ReBound(pHit, &vecMove);
	}

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(vecMove);	// �ړ���
}

//==========================================================================
// �d�͂̉�������
//==========================================================================
void CBall::UpdateGravity(const float fDeltaRate, const float fSlowRate)
{
	// �d�͂�^����
	m_fGravity -= GRAVITY * fDeltaRate * fSlowRate;
}

//==========================================================================
// �ړ��ʂ̌�������
//==========================================================================
void CBall::UpdateDecay(const float fDeltaRate, const float fSlowRate)
{
	// �ړ��ʂ�����������
	m_fMoveSpeed += (0.0f - m_fMoveSpeed) * (REV_MOVE * fDeltaRate * fSlowRate);
}

//==========================================================================
// �ʒu�ɏd�͂𔽉f���鏈��
//==========================================================================
void CBall::UpdateGravityPosition(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate)
{
	// �ʒu�ɏd�͂𔽉f
	pPos->y += m_fGravity * fDeltaRate * fSlowRate;
}

//==========================================================================
// �ʒu�Ɉړ��ʂ𔽉f���鏈��
//==========================================================================
void CBall::UpdateMovePosition(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate)
{
	// �ʒu�Ɉړ��ʂ𔽉f
	*pPos += (*pMove * (m_fMoveSpeed + m_fInitialSpeed)) * fDeltaRate * fSlowRate;

	// ��O�̕␳
	CGameManager::GetInstance()->SetPosLimit(*pPos, 60.0f);	// TODO�F�����{�[���͓]�����Ăǂ����ɂ�����
}

//==========================================================================
// �ړ�����
//==========================================================================
void CBall::UpdateMove(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate)
{
	// �d�͂̉���
	UpdateGravity(fDeltaRate, fSlowRate);

	// �ʒu�ɏd�͔��f
	UpdateGravityPosition(pPos, pMove, fDeltaRate, fSlowRate);

	// �ʒu�Ɉړ��ʔ��f
	UpdateMovePosition(pPos, pMove, fDeltaRate, fSlowRate);

	// �ړ��ʂ̌���
	UpdateDecay(fDeltaRate, fSlowRate);
}

//==========================================================================
// �n�ʂ̒��n����
//==========================================================================
bool CBall::UpdateLanding(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate)
{
	if (pPos->y - RADIUS < CGameManager::FIELD_LIMIT)
	{ // �{�[�����n�ʂɖ�����Ă���ꍇ

		// �{�[���̈ʒu��␳
		pPos->y = CGameManager::FIELD_LIMIT + RADIUS;

		// �c�ړ��ʂ�^����
		pMove->y = UtilFunc::Transformation::Clamp(m_fMoveSpeed, 0.0f, MAX_BOUND_MOVE);

		// ������������
		m_fInitialSpeed = 0.0f;

		// �d�͂�������
		m_fGravity = 0.0f;

		// ���n���Ă����Ԃɂ���
		m_bLanding = true;

		// �T�E���h�Đ�
		// TODO : �l�̒����ƒ萔������
		if (!UtilFunc::Calculation::IsNearlyTarget(m_fMoveSpeed, 0.0f, 0.75f))
		{
			// ����
			float ratio = UtilFunc::Transformation::Clamp(fabsf(m_fMoveSpeed) / 5.0f, 0.0f, 1.0f);

			if (ratio > 0.5f)
			{
				PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_HIGH);
			}
			else if (ratio <= 0.5f && ratio > 0.2f)
			{
				PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_MEDIUM);
			}
			else
			{
				PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_LOW);
			}
		}
		return true;
	}

	return false;
}

//==========================================================================
// �v���C���[�Ƃ̓����蔻��
//==========================================================================
CPlayer* CBall::CollisionPlayer(MyLib::Vector3* pPos)
{
	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	const float fCollRadius = GetCollRadius();			// ����{�[�����a
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���

		// �~�Ɖ~���̓����蔻��
		bool bHit = UtilFunc::Collision::CollisionCircleCylinder
		( // ����
			*pPos,
			pPlayer->GetPosition(),
			fCollRadius,
			pPlayer->GetRadius(),
			pPlayer->GetParameter().fHeight
		);
		if (bHit)
		{ // �������Ă����ꍇ

			// �v���C���[�q�b�g����
			if (pPlayer->Hit(this).bHit)
			{ // �q�b�g�������Ń_���[�W���󂯂��ꍇ

				// �z�[�~���O�Ώۂ̏�����
				m_pTarget = nullptr;

				// �q�b�g�����v���C���[��Ԃ�
				return pPlayer;
			}
		}
	}

	return nullptr;
}

//==========================================================================
// ������ΏۂƂ̓����蔻��
//==========================================================================
CPlayer* CBall::CollisionThrowTarget(const bool bAbsLock)
{
	// �����Ă����v���C���[���������ς݂̏ꍇ�G���[
	assert(m_pPlayer != nullptr);

	// �����Ă����v���C���[�̃`�[�����������ς݂̏ꍇ�G���[
	assert(m_typeTeam != CGameManager::SIDE_NONE);

	MyLib::Vector3 posBall = GetPosition();						// �{�[���ʒu
	MyLib::Vector3 posThrow = m_pPlayer->GetCenterPosition();	// �������v���C���[�̈ʒu
	MyLib::Vector3 rotThrow = m_pPlayer->GetRotation();			// �������v���C���[�̌���

	CPlayer* pTarget = nullptr;	// �ڕW�^�[�Q�b�g
	float fMinDis = MAX_DIS;	// �{�[������߂��v���C���[

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���
		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// �v���C���[�ʒu

		// �����`�[���̏ꍇ����
		if (m_typeTeam == pPlayer->GetTeam()) { continue; }

		// �|�W�V�������O��̏ꍇ����
		if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT) { continue; }

		// ���E���ɂ��Ȃ��ꍇ����
		bool bHit = UtilFunc::Collision::CollisionViewRange3D(posThrow, posPlayer, rotThrow.y, VIEW_ANGLE);
		if (!bHit && !bAbsLock) { continue; }

		// �v���C���[���쒆�S�x�N�g������̋�������
		MyLib::Vector3 posFront = posThrow + MyLib::Vector3(sinf(rotThrow.y), 0.0f, cosf(rotThrow.y));
		float fCurDis = fabsf(UtilFunc::Collision::LineOuterProduct(posThrow, posFront, posPlayer));
		if (fCurDis < fMinDis)
		{ // ���{�[���ɋ߂��v���C���[�������ꍇ

			// ������ۑ�
			fMinDis = fCurDis;

			// �^�[�Q�b�g���X�V
			pTarget = pPlayer;
		}
	}

	// �����^�[�Q�b�g��Ԃ�
	return pTarget;
}

//==========================================================================
// �p�X����ΏۂƂ̓����蔻��
//==========================================================================
CPlayer* CBall::CollisionPassTarget()
{
	// �����Ă����v���C���[���������ς݂̏ꍇ�G���[
	assert(m_pPlayer != nullptr);

	// �����Ă����v���C���[�̃`�[�����������ς݂̏ꍇ�G���[
	assert(m_typeTeam != CGameManager::SIDE_NONE);

	MyLib::Vector3 posBall = GetPosition();						// �{�[���ʒu
	MyLib::Vector3 posThrow = m_pPlayer->GetCenterPosition();	// �������v���C���[�̈ʒu
	MyLib::Vector3 rotThrow = m_pPlayer->GetRotation();			// �������v���C���[�̌���
	CPlayer::EFieldArea typeArea = m_pPlayer->GetAreaType();	// �������v���C���[�̃|�W�V����

	CPlayer* pTarget = nullptr;	// �ڕW�^�[�Q�b�g
	float fMinDis = MAX_DIS;	// �{�[������߂��v���C���[

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���
		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// �v���C���[�ʒu

		// �Ⴄ�`�[���̏ꍇ����
		if (m_typeTeam != pPlayer->GetTeam()) { continue; }

		// �����Ɠ����|�W�V�����̏ꍇ����
		if (typeArea == pPlayer->GetAreaType()) { continue; }

		// ���E���ɂ��Ȃ��ꍇ����	// TODO
#if 0
		bool bHit = UtilFunc::Collision::CollisionViewRange3D(posThrow, posPlayer, rotThrow.y, VIEW_ANGLE);
		if (!bHit) { continue; }
#endif

		// �v���C���[���쒆�S�x�N�g������̋�������
		MyLib::Vector3 posFront = posThrow + MyLib::Vector3(sinf(rotThrow.y), 0.0f, cosf(rotThrow.y));
		float fCurDis = fabsf(UtilFunc::Collision::LineOuterProduct(posThrow, posFront, posPlayer));
		if (fCurDis < fMinDis)
		{ // ���{�[���ɋ߂��v���C���[�������ꍇ

			// ������ۑ�
			fMinDis = fCurDis;

			// �^�[�Q�b�g���X�V
			pTarget = pPlayer;
		}
	}

	// �����^�[�Q�b�g��Ԃ�
	return pTarget;
}

//==========================================================================
// ���蔼�a�擾
//==========================================================================
float CBall::GetCollRadius()
{
	if (m_state == EState::STATE_HOM_PASS
	||  m_state == EState::STATE_PASS
	||  m_state == EState::STATE_REBOUND
	||  m_state == EState::STATE_FREE
	||  m_state == EState::STATE_LAND)
	{ // ���肪�ł��������悳���ȏ�Ԃ̏ꍇ

		return PLUS_RADIUS;
	}

	return RADIUS;
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CBall::SetState(const EState state)
{
	// �����̏�Ԃɂ���
	m_state = state;

	// �J�E���^�[��������
	m_fStateTime = 0.0f;

	// �J�o�[�Ώۃv���C���[�̏�����
	m_pCover = nullptr;
}

//==========================================================================
// �L���b�`����
//==========================================================================
void CBall::Catch(CPlayer* pPlayer)
{
#ifdef CHANGE
	if (IsPass() && m_pTarget != pPlayer)
	{ // �p�X��Ԋ��A�^�[�Q�b�g�ȊO���L���b�`�����ꍇ

		// ���^�[�Q�b�g��AI�ɖ߂�
		CPlayerManager::GetInstance()->ChangeUserToAI(m_pTarget);
	}
#endif

	// �ړ��ʂ�������
	SetMove(VEC3_ZERO);

	// �L���b�`��Ԃɂ���
	SetState(STATE_CATCH);

	// �v���C���[�̃`�[����ۑ�
	m_typeTeam = pPlayer->GetTeam();

	// �z�[�~���O�Ώۂ̏�����
	m_pTarget = nullptr;

	// �L���b�`�����v���C���[��ۑ�
	m_pPlayer = pPlayer;

	// �L���b�`�����v���C���[�������}�[�J�[�Ɋ���
	m_pHoldMarker->BindPlayer(pPlayer);

#ifdef CHANGE
	// �L���b�`����AI�ɑ��쌠���ڂ�
	CPlayerManager::GetInstance()->ChangeAIToUser(pPlayer);
#endif

	// �v���C���[�Ƀ{�[����ۑ�
	pPlayer->SetBall(this);
}

//==========================================================================
// ��������
//==========================================================================
void CBall::Throw(CPlayer* pPlayer)
{
	// �����Ă����v���C���[�ƈႤ�ꍇ�G���[
	assert(m_pPlayer == pPlayer);

	// �L���b�`���Ă����v���C���[��j��
	m_pPlayer = nullptr;

	// �v���C���[����ۑ����̃{�[����j��
	pPlayer->SetBall(nullptr);

	// �����}�[�J�[����v���C���[��j��
	m_pHoldMarker->BindPlayer(nullptr);

	// �{�[���̈ړ��x�N�g�����쐬
	float fRotY = pPlayer->GetRotation().y + D3DX_PI;	// �{�[���̓��������
	MyLib::Vector3 vecMove = MyLib::Vector3(sinf(fRotY), 0.0f, cosf(fRotY));	// �ړ��x�N�g��

	// �ړ��x�N�g���𐳋K�����Đݒ�
	SetMove(vecMove.Normal());

	// ������������
	m_fInitialSpeed = 0.0f;

	// �d�͂�������
	m_fGravity = 0.0f;
}

//==========================================================================
// �X�y�V������������
//==========================================================================
void CBall::ThrowSpecial()
{
	// �X�y�V�������o��ʉ߂��Ă��Ȃ��{�[���𓊂��悤�Ƃ����ꍇ�G���[
	assert(m_state == STATE_SPECIAL_STAG);

	// �X�y�V����������Ԃɂ���
	SetState(STATE_SPECIAL_THROW);

	MyLib::Vector3 posPlayer = m_pPlayer->GetPosition();	// �{�[���ߋ��ʒu
	MyLib::Vector3 posTarget = m_pTarget->GetPosition();	// �v���C���[�ʒu
	float fAngleY = posPlayer.AngleXZ(posTarget);			// �{�[������

	// �ڕW����/�������{�[�������ɂ���
	m_pPlayer->SetRotDest(fAngleY);
	m_pPlayer->SetRotation(MyLib::Vector3(0.0f, fAngleY, 0.0f));

	// ��������
	Throw(m_pPlayer);

	// �X�y�V�����U����ݒ�
	m_typeAtk = ATK_SPECIAL;

	// �ړ��ʂ�ݒ�
	m_fMoveSpeed = special::THROW_MOVE;

	// ������ݒ�
	CalcSetInitialSpeed(m_fMoveSpeed);
}

//==========================================================================
// ���n����
//==========================================================================
void CBall::Landing()
{
	// ���n��Ԃɂ���
	SetState(STATE_LAND);

	// �z�[�~���O�Ώۂ̏�����
	m_pTarget = nullptr;
}

//==========================================================================
// �`�[���T�C�h�̍X�V����
//==========================================================================
void CBall::UpdateTypeTeam()
{
	// �U�����肪����ꍇ�͌��݂̃`�[����ێ�
	if (IsAttack()) { return; }

	// �p�X�{�[���̏ꍇ�����݂̃`�[����ێ�
	if (IsPass()) { return; }

	// �t���[�{�[���̏ꍇ�����݂̃`�[����ێ�
	if (m_state == STATE_FREE) { return; }

	// �v���C���[���������Ă��Ȃ��ꍇ�`�[���w��Ȃ�
	if (m_pPlayer == nullptr) { m_typeTeam = CGameManager::SIDE_NONE; return; }

	// �v���C���[�̃`�[����ۑ�
	m_typeTeam = m_pPlayer->GetTeam();

	// �p�X�J�n/�I���ʒu�����Z�b�g
	m_posPassStart	= VEC3_ZERO;
	m_posPassEnd	= VEC3_ZERO;
}

//==========================================================================
// �U����ނ̍X�V����
//==========================================================================
void CBall::UpdateTypeAtk()
{
	// �U�����肪����ꍇ�͌��݂̍U����ނ�ێ�
	if (IsAttack()) { return; }

	// �U����ނ�j��
	m_typeAtk = ATK_NONE;

	// �������Z�b�g
	m_nDamage = 0;			// �_���[�W��
	m_fKnockback = 0.0f;	// �m�b�N�o�b�N
	m_fHomingTime = 0.0f;	// �z�[�~���O����
}

//==========================================================================
// �X�y�V������ނ̍X�V����
//==========================================================================
void CBall::UpdateTypeSpecial()
{
	// �X�y�V�������̏ꍇ�͌��݂̃X�y�V������ێ�
	if (IsSpecial()) { return; }

	// �X�y�V������j��
	m_typeSpecial = SPECIAL_NONE;
}

//==========================================================================
// ���o�E���h����
//==========================================================================
void CBall::ReBound(CPlayer* pHitPlayer, MyLib::Vector3* pMove)
{
	// �ړ��x�N�g���𔽓]
	*pMove = pMove->Invert();

	// ��ړ��ʂ�ǉ�
	pMove->y = rebound::MOVE_UP;

	// ������������
	m_fInitialSpeed = 0.0f;

	// �ړ����x��ቺ
	m_fMoveSpeed = rebound::MOVE_SPEED;

	// ���o�E���h��Ԃɂ���
	SetState(STATE_REBOUND);

	// �J�o�[�Ώۃv���C���[��ۑ�
	m_pCover = pHitPlayer;
}

//==========================================================================
// �����̌v�Z����
//==========================================================================
void CBall::CalcSetInitialSpeed(const float fMove)
{
	// ������^����
	m_fInitialSpeed = fMove * move::MULTIPLY_INIMOVE;
}

//==========================================================================
// �ړ��x�N�g���̌v�Z
//==========================================================================
MyLib::Vector3 CBall::CalcVecMove(CPlayer* pTarget, CPlayer* pPlayer)
{
	const float fThrowMove = pPlayer->GetBallParameter().fThrowMoveNormal;	// �ʏ퓊�����x

	// �{�[�����G�ɓ��B����܂ł̎��Ԍv�Z
	SHitTimingInfo info = CalcHitSpeedTime
	( // ����
		pTarget->GetPosition(),	// �^�[�Q�b�g�ʒu
		GetPosition(),			// �{�[���ʒu
		pPlayer->GetRadius(),	// �^�[�Q�b�g���a
		fThrowMove * move::MULTIPLY_INIMOVE,	// ����
		fThrowMove								// �ړ���
	);

	// �z�[�~���O���Ԃ̕ۑ�
	m_fHomingTime = info.fHitTime;

	// �{�[����������Ȃ������ꍇ���݂̈ʒu�ւ̃x�N�g����Ԃ�
	if (!info.bHit) { return pTarget->GetPosition() + pTarget->GetLookOffset() - GetPosition(); }

	// �����ʒu�ւ̃x�N�g����Ԃ�
	return pTarget->CalcFuturePosition(info.nHitFrame) - GetPosition();
}

//==========================================================================
// �{�[�����G�ɓ��B����܂ł̎��Ԍv�Z
//==========================================================================
CBall::SHitTimingInfo CBall::CalcHitSpeedTime
(
	const MyLib::Vector3& rPosTarget,	// �^�[�Q�b�g�ʒu
	const MyLib::Vector3& rPosBall,		// �{�[���ʒu
	const float fRadiusTarget,			// �^�[�Q�b�g���a
	const float fInitSpeed,				// ����
	const float fMoveSpeed				// �ړ���
)
{
	// �{�[���ړ��x�N�g���̍쐬
	MyLib::Vector3 vecMove = rPosTarget - rPosBall;
	vecMove = vecMove.Normal();	// �x�N�g���̐��K��

	const float fFrameTime = 1.0f / (float)GetFPS();	// �z��1F����
	float fNextInitSpeed = fInitSpeed;		// ���̏���
	MyLib::Vector3 nextPosBall = rPosBall;	// ���̃{�[���ʒu

	int nHitFrame = 0;		// �q�b�g�܂łɂ�����t���[����
	float fHitTime = 0.0f;	// �q�b�g�܂łɂ����鎞��
	bool bHit = false;		// �q�b�g�t���O
	while (fHitTime < normal::TIME_HOMING)
	{ // �z�[�~���O�̐؂��ő厞�Ԃ܂ŌJ��Ԃ�

		// �ʒu���ړ�������
		nextPosBall += (vecMove * (fMoveSpeed + fNextInitSpeed));

		// �{�[�������������ꍇ������
		if (UtilFunc::Collision::CircleRange3D(rPosTarget, nextPosBall, fRadiusTarget, RADIUS)) { bHit = true; break; }

		// ����������������
		fNextInitSpeed += (0.0f - fNextInitSpeed) * REV_INIMOVE;

		// 1F���̎��Ԃ�i�߂�
		fHitTime += fFrameTime;

		// �t���[������i�߂�
		nHitFrame++;
	}

	// ���������^�C�~���O�̏����쐬���Ԃ�
	SHitTimingInfo info;
	info.nHitFrame = nHitFrame;
	info.fHitTime = fHitTime;
	info.bHit = bHit;
	return info;
}

//==========================================================================
// ���������̍X�V
//==========================================================================
void CBall::UpdateThrowLine()
{
	// �I�[���X�V
	if (m_pAura != nullptr)
	{
		// ���[���h�}�g���b�N�X�v�Z
		CalWorldMtx();

		// �ʒu�ݒ�
		m_pAura->SetPosition(GetWorldMtx().GetWorldPosition());
	}

	if (IsAttack())
	{// �U����

		// �����擾
		MyLib::Vector3 pos = GetPosition();	// �ʒu
		MyLib::Vector3 vecMove = GetMove();	// �ړ���

		// Y���̌���
		MyLib::Vector3 rot;
		rot.y = vecMove.AngleXZ(0.0f) - D3DX_PI * 0.5f;

		// Z���̌���
		rot.z = vecMove.y;

		// �����̃��C��
		if (m_pThrowLine == nullptr)
		{
			switch (GetTypeAtk())
			{
			case ATK_NORMAL:	// �ʏ�U��

				m_pThrowLine = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_THROWLINE_NORMAL,
					GetPosition(),
					rot,
					MyLib::Vector3(),
					8.0f);
				break;
				
			case ATK_JUMP:	// �W�����v�U��

				m_pThrowLine = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_THROWLINE_FAST,
					GetPosition(),
					rot,
					MyLib::Vector3(),
					8.0f);
				break;

			default:

				m_pThrowLine = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_THROWLINE_FAST,
					GetPosition(),
					rot,
					MyLib::Vector3(),
					8.0f);

				break;
			}
		}
		m_pThrowLine->SetPosition(pos);
		m_pThrowLine->SetRotation(rot);
	}
	else if(m_pThrowLine != nullptr)
	{// �����I���

		// ��~
		if (m_pThrowLine != nullptr)
		{
			m_pThrowLine->SetTrigger(0);
			m_pThrowLine->SetTrigger(1);
			m_pThrowLine = nullptr;
		}
	}
}
