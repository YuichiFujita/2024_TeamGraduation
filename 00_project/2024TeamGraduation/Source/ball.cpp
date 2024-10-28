//==========================================================================
// 
//  �{�[������ [ball.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "ball.h"
#include "player.h"
#include "playerStatus.h"
#include "manager.h"
#include "game.h"
#include "gamemanager.h"
#include "calculation.h"
#include "model.h"

#include "debugproc.h"
#include "3D_Effect.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char*	MODEL = "data\\MODEL\\dadgeball\\dodgeball.x";	// �{�[�����f��
	const float	RADIUS = 12.0f;			// ���a
	const float	REV_MOVE = 0.025f;		// �ړ��ʂ̕␳�W��
	const float	REV_INIMOVE = 0.29f;	// �����̕␳�W��
	const float	MAX_DIS = 100000.0f;	// �z�[�~���O����ő勗��
	const int	VIEW_ANGLE = 104;		// ����p
	const float DEST_POSY = 45.0f;		// �ʏ�{�[���̖ڕWY���W
	const float REV_POSY = 0.1f;		// �ʏ�{�[���̖ڕWY���W�̕␳�W��
	const float GRAVITY = mylib_const::GRAVITY * 0.6f;	// �{�[���ɂ�����d��
	const float MAX_BOUND_MOVE = 1.0f;	// �o�E���h���̏�ړ��ʍő�l

	const char* DEBUG_STATE_PRINT[] =	// �f�o�b�O�\���p���
	{
		"SPAWN    �������               (�t���[�{�[��)",
		"CATCH    �L���b�`���           (�v���C���[����)",
		"HOM_NOR  �ʏ�z�[�~���O���     (�U������ON)",
		"HOM_JUMP �W�����v�z�[�~���O��� (�U������ON)",
		"MOVE     �ړ����               (�U������ON)",
		"REBOUND  ���o�E���h���         (�Ԃ��������̗���)",
		"LAND     ���n���               (�n�ʗ���)",
	};

	namespace normal
	{
		const float THROW_MOVE = 19.5f;	// �ʏ퓊���ړ����x
		const float REV_HOMING = 0.3f;	// �z�[�~���O�̊����␳�W��
		const float TIME_HOMING = 1.2f;	// �z�[�~���O���؂��܂ł̎���
	}

	namespace jump
	{
		const float THROW_MOVE = 24.0f;		// �W�����v�����ړ����x
		const float REV_HOMING = 0.24f;		// �z�[�~���O�̊����␳�W��
		const float MIN_MOVE_DOWN = -0.3f;	// �W�����v�U���̍Œቺ�ړ���
		const float OFFSET_TARGET_BACK = 150.0f;	// �^�[�Q�b�g�̌��I�t�Z�b�g
	}

	namespace Special
	{
		const float THROW_MOVE = 60.0f;	// �X�y�V���������ړ����x
	}

	namespace move
	{
		const float TIME_GRAVITY = 0.8f;		// �d�͂�������n�߂�܂ł̎���
		const float MULTIPLY_INIMOVE = 3.5f;	// �����̔{��
	}

	namespace rebound
	{
		const float MOVE_UP = 2.5f;		// ��ړ���
		const float MOVE_SPEED = 2.5f;	// �ړ����x
	}
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CBall::STATE_FUNC CBall::m_StateFuncList[] =
{
	&CBall::UpdateSpawn,		// ������Ԃ̍X�V
	&CBall::UpdateCatch,		// �L���b�`��Ԃ̍X�V
	&CBall::UpdateHomingNormal,	// �ʏ�z�[�~���O��Ԃ̍X�V
	&CBall::UpdateHomingJump,	// �W�����v�z�[�~���O��Ԃ̍X�V
	&CBall::UpdateMove,			// �ړ���Ԃ̍X�V
	&CBall::UpdateReBound,		// ���o�E���h��Ԃ̍X�V
	&CBall::UpdateLand,			// ���n��Ԃ̍X�V
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
	m_pPlayer		(nullptr),		// �v���C���[���
	m_pTarget		(nullptr),		// �z�[�~���O�^�[�Q�b�g���
	m_pCover		(nullptr),		// �J�o�[�Ώۃv���C���[���
	m_fMoveSpeed	(0.0f),			// �ړ����x
	m_fGravity		(0.0f),			// �d��
	m_oldOverLine	(VEC3_ZERO),	// �z�[�~���O�I�����C��
	m_typeAtk		(ATK_NONE),		// �U�����
	m_state			(STATE_SPAWN),	// ���
	m_fStateTime	(0.0f)			// ��ԃJ�E���^�[
{

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

	// �����ړ��ʍX�V
	if (IsAttack())
	{// �U�����̂ݕ␳

		m_fInitialSpeed += (0.0f - m_fInitialSpeed) * (REV_INIMOVE * fDeltaRate * fSlowRate);
	}

	// ��ԕʏ���
	(this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);

	// �e�N���X�̍X�V
	CObjectX::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �{�[���̏�ԕ\��
	GET_MANAGER->GetDebugProc()->Print("�{�[����ԁF%s\n", DEBUG_STATE_PRINT[m_state]);
	GET_MANAGER->GetDebugProc()->Print("�^�[�Q�b�g�F%s\n", (m_pTarget == nullptr) ? "nullptr" : "player");
	GET_MANAGER->GetDebugProc()->Print("�J�o�[�ΏہF%s\n", (m_pCover == nullptr) ? "nullptr" : "player");
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
// �ʏ퓊������
//==========================================================================
void CBall::ThrowNormal(CPlayer* pPlayer)
{
	// �L���b�`���Ă��Ȃ��{�[���𓊂��悤�Ƃ����ꍇ�G���[
	assert(m_state == STATE_CATCH);

	// �z�[�~���O�Ώۂ̐ݒ�
	m_pTarget = CollisionThrow();
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
	}
	else
	{ // �^�[�Q�b�g�����Ȃ��ꍇ

		// �ړ���Ԃɂ���
		SetState(STATE_MOVE);
	}

	// ��������
	Throw(pPlayer);

	// �ʏ�U����ݒ�
	m_typeAtk = ATK_NORMAL;

	// �ړ��ʂ�ݒ�
	m_fMoveSpeed = normal::THROW_MOVE;
	CalSetInitialSpeed(m_fMoveSpeed);
}

//==========================================================================
// �W�����v��������
//==========================================================================
void CBall::ThrowJump(CPlayer* pPlayer)
{
	// �L���b�`���Ă��Ȃ��{�[���𓊂��悤�Ƃ����ꍇ�G���[
	assert(m_state == STATE_CATCH);

	// �z�[�~���O�Ώۂ̐ݒ�
	m_pTarget = CollisionThrow();
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
	}
	else
	{ // �^�[�Q�b�g�����Ȃ��ꍇ

		// �ړ���Ԃɂ���
		SetState(STATE_MOVE);
	}

	// ��������
	Throw(pPlayer);

	// �W�����v�U����ݒ�
	m_typeAtk = ATK_JUMP;

	// �ړ��ʂ�ݒ�
	m_fMoveSpeed = jump::THROW_MOVE;
	CalSetInitialSpeed(m_fMoveSpeed);
}

//==========================================================================
// �X�y�V������������
//==========================================================================
void CBall::ThrowSpecial(CPlayer* pPlayer)
{
	// �L���b�`���Ă��Ȃ��{�[���𓊂��悤�Ƃ����ꍇ�G���[
	assert(m_state == STATE_CATCH);

	// �z�[�~���O�Ώۂ̐ݒ�
	m_pTarget = CollisionThrow();
	if (m_pTarget != nullptr)
	{ // �^�[�Q�b�g������ꍇ

		MyLib::Vector3 posPlayer = pPlayer->GetPosition();		// �{�[���ߋ��ʒu
		MyLib::Vector3 posTarget = m_pTarget->GetPosition();	// �v���C���[�ʒu
		float fAngleY = posPlayer.AngleXZ(posTarget);			// �{�[������

		// �z�[�~���O��Ԃɂ���
		SetState(STATE_HOM_NOR);	// TODO�F�X�y�V�����Ɍ�X�ύX

		// �ڕW����/�������{�[�������ɂ���
		m_pPlayer->SetRotDest(fAngleY);
		m_pPlayer->SetRotation(MyLib::Vector3(0.0f, fAngleY, 0.0f));
	}
	else
	{ // �^�[�Q�b�g�����Ȃ��ꍇ

		// �ړ���Ԃɂ���
		SetState(STATE_MOVE);
	}

	// ��������
	Throw(pPlayer);

	// �X�y�V�����U����ݒ�
	m_typeAtk = ATK_SPECIAL;

	// �ړ��ʂ�ݒ�
	m_fMoveSpeed = Special::THROW_MOVE;
	CalSetInitialSpeed(m_fMoveSpeed);
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
	return (m_state == STATE_HOM_NOR || m_state == STATE_HOM_JUMP || m_state == STATE_MOVE);	// TODO�F�U����Ԃ���������ǉ�
}

//==========================================================================
// ���[���h�}�g���b�N�X�̌v�Z����
//==========================================================================
void CBall::CalWorldMtx()
{
	if (m_state == STATE_CATCH)
	{ // �L���b�`���̏ꍇ

		const int nPartsIdx = m_pPlayer->GetParameter().nBallPartsIdx;		// �{�[���p�[�c�C���f�b�N�X
		const MyLib::Vector3 offset = m_pPlayer->GetParameter().ballOffset;	// �{�[���I�t�Z�b�g
		MyLib::Matrix* pMtxParts = m_pPlayer->GetModel(nPartsIdx)->GetPtrWorldMtx();	// �p�[�c�}�g���b�N�X
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
		mtxWorld.Multiply(mtxWorld, mtxTrans);

		// ��������v���C���[�p�[�c�̃}�g���b�N�X�Ɗ|�����킹��
		mtxWorld.Multiply(*pMtxParts, mtxWorld);

		// �}�g���b�N�X�𔽉f
		SetWorldMtx(*pMtxParts);

		// �L���b�`���̃}�g���b�N�X����ʒu�𔽉f
		SetPosition(pMtxParts->GetWorldPosition());
	}
	else
	{ // �L���b�`���Ă��Ȃ��ꍇ

		// �e�N���X�̃��[���h�}�g���b�N�X�v�Z
		CObjectX::CalWorldMtx();
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

	// Y���W�������␳
	UtilFunc::Correction::InertiaCorrection(pos.y, DEST_POSY, REV_POSY);

	// �o�ߎ��Ԃ����Z
	m_fStateTime += fDeltaTime;
	if (m_fStateTime >= normal::TIME_HOMING)
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

	// �o�ߎ��Ԃ����Z
	m_fStateTime += fDeltaTime;
	if (m_fStateTime >= move::TIME_GRAVITY)
	{
		// �d�͂̉���
		UpdateGravity(fDeltaRate, fSlowRate);

		// �ʒu�ɏd�͔��f
		UpdateGravityPosition(&pos, &vecMove, fDeltaRate, fSlowRate);
	}
	else
	{
		// Y���W�������␳
		UtilFunc::Correction::InertiaCorrection(pos.y, DEST_POSY, REV_POSY);
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
	CGame::GetInstance()->GetGameManager()->SetPosLimit(*pPos);
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
		pMove->y = m_fMoveSpeed;

		// ����ɕ␳
		UtilFunc::Transformation::ValueNormalize(pMove->y, MAX_BOUND_MOVE, 0.0f);

		// �d�͂�������
		m_fGravity = 0.0f;
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
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���

		// �����`�[���̏ꍇ����
		if (pPlayer->GetStatus()->GetTeam() == m_typeTeam) { continue; }

		bool bHit = UtilFunc::Collision::CollisionCircleCylinder
		( // ����
			*pPos,
			pPlayer->GetPosition(),
			RADIUS,
			pPlayer->GetRadius(),
			pPlayer->GetParameter().fHeight
		);
		if (bHit)
		{ // �������Ă����ꍇ

			// �v���C���[�q�b�g����
			if (pPlayer->Hit(this))
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
// �z�[�~���O�ΏۂƂ̓����蔻��
//==========================================================================
CPlayer* CBall::CollisionThrow(void)
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
		if (m_typeTeam == pPlayer->GetStatus()->GetTeam()) { continue; }

		// ���E���ɂ��Ȃ��ꍇ����
		bool bHit = UtilFunc::Collision::CollisionViewRange3D(posThrow, posPlayer, rotThrow.y, VIEW_ANGLE);
		if (!bHit) { continue; }

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
	// �ړ��ʂ�������
	SetMove(VEC3_ZERO);

	// �L���b�`��Ԃɂ���
	SetState(STATE_CATCH);

	// �v���C���[�̃`�[����ۑ�
	m_typeTeam = pPlayer->GetStatus()->GetTeam();

	// �L���b�`�����v���C���[��ۑ�
	m_pPlayer = pPlayer;

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

	// �{�[���̈ړ��x�N�g�����쐬
	float fRotY = pPlayer->GetRotation().y + D3DX_PI;	// �{�[���̓��������
	MyLib::Vector3 vecMove = MyLib::Vector3(sinf(fRotY), 0.0f, cosf(fRotY));	// �ړ��x�N�g��

	// �ړ��x�N�g���𐳋K�����Đݒ�
	SetMove(vecMove.Normal());
}

//==========================================================================
// ���n����
//==========================================================================
void CBall::Landing(void)
{
	// ���n��Ԃɂ���
	SetState(STATE_LAND);

	// �z�[�~���O�Ώۂ̏�����
	m_pTarget = nullptr;

	// �`�[���̏�����
	m_typeTeam = CGameManager::SIDE_NONE;

	// �U���̏�����
	m_typeAtk = ATK_NONE;
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

	// �ړ����x��ቺ
	m_fMoveSpeed = rebound::MOVE_SPEED;

	// ���o�E���h��Ԃɂ���
	SetState(STATE_REBOUND);

	// �J�o�[�Ώۃv���C���[��ۑ�
	m_pCover = pHitPlayer;
}

//==========================================================================
// �����̌v�Z�ݒ菈��
//==========================================================================
void CBall::CalSetInitialSpeed(float move)
{
	m_fInitialSpeed = move * move::MULTIPLY_INIMOVE;
}
