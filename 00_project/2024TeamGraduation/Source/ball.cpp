//=============================================================================
// 
//  �{�[������ [ball.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "ball.h"
#include "player.h"
#include "playerStatus.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char*	MODEL = "data\\MODEL\\dadgeball\\dodgeball.x";	// �{�[�����f��
	const float	RADIUS = 7.0f;			// ���a
	const float	REV_MOVE = 0.025f;		// �ړ��ʂ̕␳�W��
	const float	MAX_DIS = 100000.0f;	// �z�[�~���O����ő勗��
	const int	VIEW_ANGLE = 135;		// ����p

	const char* DEBUG_STATE_PRINT[] =	// �f�o�b�O�\���p���
	{
		"SPAWN   �������       (�t���[�{�[��)",
		"CATCH   �L���b�`���   (�v���C���[����)",
		"HOMING  �z�[�~���O��� (�U������ON)",
		"MOVE    �ړ����       (�U������ON)",
		"REBOUND ���o�E���h��� (�Ԃ��������̗���)",
		"LAND    ���n���       (�n�ʗ���)",
	};
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CBall::STATE_FUNC CBall::m_SampleFuncList[] =
{
	&CBall::UpdateSpawn,	// ������Ԃ̍X�V
	&CBall::UpdateCatch,	// �L���b�`��Ԃ̍X�V
	&CBall::UpdateHoming,	// �z�[�~���O��Ԃ̍X�V
	&CBall::UpdateMove,		// �ړ���Ԃ̍X�V
	&CBall::UpdateReBound,	// ���o�E���h��Ԃ̍X�V
	&CBall::UpdateLand,		// ���n��Ԃ̍X�V
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CBall> CBall::m_list = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBall::CBall(int nPriority) : CObjectX(nPriority),
	m_typeTeam	 (CGameManager::SIDE_NONE),	// �`�[���T�C�h
	m_pPlayer	 (nullptr),		// �v���C���[���
	m_pTarget	 (nullptr),		// �z�[�~���O�^�[�Q�b�g���
	m_fMoveSpeed (0.0f),		// �ړ����x
	m_fGravity	 (0.0f),		// �d��
	m_typeAtk	 (ATK_NONE),	// �U�����
	m_state		 (STATE_SPAWN),	// ���
	m_fStateTime (0.0f)			// ��ԃJ�E���^�[
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
	// ��ԕʏ���
	(this->*(m_SampleFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);

	// �e�N���X�̍X�V
	CObjectX::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �{�[���̏�ԕ\��
	GET_MANAGER->GetDebugProc()->Print("�{�[����ԁF%s\n", DEBUG_STATE_PRINT[m_state]);
	GET_MANAGER->GetDebugProc()->Print("�^�[�Q�b�g�F%s\n", (m_pTarget == nullptr) ? "nullptr" : "player");
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
// �L���b�`����
//==========================================================================
void CBall::Catch(CPlayer* pPlayer)
{
	// �ړ��ʂ�������
	SetMove(VEC3_ZERO);

	// �L���b�`��Ԃɂ���
	m_state = STATE_CATCH;

	// �v���C���[�̃`�[����ۑ�
	m_typeTeam = pPlayer->GetStatus()->GetTeam();

	// �L���b�`�����v���C���[��ۑ�
	m_pPlayer = pPlayer;

	// �v���C���[�Ƀ{�[����ۑ�
	pPlayer->SetBall(this);
}

//==========================================================================
// �ʏ퓊������
//==========================================================================
void CBall::ThrowNormal(CPlayer* pPlayer)
{
	// ��������
	Throw(pPlayer);

	// �ʏ�U����ݒ�
	m_typeAtk = ATK_NORMAL;

	// �ړ��ʂ�ݒ�
	m_fMoveSpeed = 11.5f;

	// TODO�F��
	float fRot = pPlayer->GetRotation().y + D3DX_PI;
	MyLib::Vector3 vec = MyLib::Vector3(sinf(fRot), 0.0f, cosf(fRot));
	vec = vec.Normal();
	SetMove(vec);
}

//==========================================================================
// �W�����v��������
//==========================================================================
void CBall::ThrowJump(CPlayer* pPlayer)
{
	// ��������
	Throw(pPlayer);

	// �W�����v�U����ݒ�
	m_typeAtk = ATK_JUMP;

	// �ړ��ʂ�ݒ�
	m_fMoveSpeed = 13.0f;

	// TODO�F��
	float fRot = pPlayer->GetRotation().y + D3DX_PI;
	MyLib::Vector3 vec = MyLib::Vector3(sinf(fRot), 0.0f, cosf(fRot));
	vec = vec.Normal();
	SetMove(vec);
}

//==========================================================================
// �X�y�V������������
//==========================================================================
void CBall::ThrowSpecial(CPlayer* pPlayer)
{
	// ��������
	Throw(pPlayer);

	// �X�y�V�����U����ݒ�
	m_typeAtk = ATK_SPECIAL;

	// �ړ��ʂ�ݒ�
	m_fMoveSpeed = 60.0f;

	// TODO�F��
	float fRot = pPlayer->GetRotation().y + D3DX_PI;
	MyLib::Vector3 vec = MyLib::Vector3(sinf(fRot), 0.0f, cosf(fRot));
	vec = vec.Normal();
	SetMove(vec);
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
	return (m_state == STATE_HOMING || m_state == STATE_MOVE);	// TODO�F�U����Ԃ���������ǉ�
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
	// TODO�F�����̈ʒu�ɐe�q�t��
	SetPosition(m_pPlayer->GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f));	// �v���C���[���
}

//==========================================================================
// �z�[�~���O��Ԃ̍X�V����
//==========================================================================
void CBall::UpdateHoming(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 vecMove = GetMove();	// �ړ���
	MyLib::Vector3 posTarget = m_pTarget->GetCenterPosition();	// �^�[�Q�b�g�ʒu

	// �ڕW�x�N�g�����擾
	MyLib::Vector3 vecDest = posTarget - pos;
	vecDest = vecDest.Normal();

	// �����x�N�g�����v�Z
	MyLib::Vector3 vecDiff = vecDest - vecMove;
	vecDiff = vecDiff.Normal();

	// �ړ��x�N�g�����X�V
	vecMove.x += vecDiff.x * 0.3f;
	vecMove.z += vecDiff.z * 0.3f;
	vecMove = vecMove.Normal();

	// �ʒu�Ɉړ��ʂ𔽉f
	UpdateMovePosition(&pos, &vecMove, fDeltaRate, fSlowRate);

	MyLib::Vector3 vecTarget = posTarget - pos;
	if (vecTarget.Length() <= 80.0f)
	{ // �ڕW�x�N�g�� (�^�[�Q�b�g�ʒu�ƃ{�[���ʒu�̃x�N�g��) �̒������Z���ꍇ

		// �ړ���Ԃɂ���
		m_state = STATE_MOVE;
	}

	// �n�ʂ̒��n
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // ���n�����ꍇ

		// ���n�J��
		Landing();
	}

	// �v���C���[�Ƃ̓����蔻��
	if (CollisionPlayer(&pos))
	{ // ���������ꍇ

		// ���o�E���h�J��
		ReBound(&vecMove);
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

	// �ړ��ʂ̌���
	UpdateDecay(fDeltaRate, fSlowRate);

	if (m_fMoveSpeed <= 5.5f)
	{ // �ړ��ʂ����������ꍇ

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
	if (CollisionPlayer(&pos))
	{ // ���������ꍇ

		// ���o�E���h�J��
		ReBound(&vecMove);
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
	m_fGravity -= (mylib_const::GRAVITY * 0.6f) * fDeltaRate * fSlowRate;
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
	*pPos += (*pMove * m_fMoveSpeed) * fDeltaRate * fSlowRate;
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

		// �c�ړ��ʂ�������	// TODO�F��Ńo�E���h����悤�ύX
		pMove->y = 0.0f;

		// �d�͂�������
		m_fGravity = 0.0f;
		return true;
	}

	return false;
}

//==========================================================================
// �v���C���[�Ƃ̓����蔻��
//==========================================================================
bool CBall::CollisionPlayer(MyLib::Vector3* pPos)
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
			pPlayer->GetHeight()
		);
		if (bHit)
		{ // �������Ă����ꍇ

			// �v���C���[�q�b�g����
			if (pPlayer->Hit(this)) { return true; }	// �q�b�g�������Ń_���[�W���󂯂��ꍇ
		}
	}

	return false;
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
// ��������
//==========================================================================
void CBall::Throw(CPlayer* pPlayer)
{
	// �����Ă����v���C���[�ƈႤ�ꍇ�G���[
	assert(m_pPlayer == pPlayer);

	// �z�[�~���O�Ώۂ̐ݒ�
	m_pTarget = CollisionThrow();
	if (m_pTarget != nullptr)	{ m_state = STATE_HOMING; }	// �^�[�Q�b�g������ꍇ�z�[�~���O��Ԃ�
	else						{ m_state = STATE_MOVE; }	// �^�[�Q�b�g�����Ȃ��ꍇ�ړ���Ԃ�

	// �L���b�`���Ă����v���C���[��j��
	m_pPlayer = nullptr;

	// �v���C���[����ۑ����̃{�[����j��
	pPlayer->SetBall(nullptr);
}

//==========================================================================
// ���o�E���h����
//==========================================================================
void CBall::ReBound(MyLib::Vector3* pMove)
{
	// �ړ��x�N�g���𔽓]
	*pMove = pMove->Invert();

	// ��ړ��ʂ�ǉ�
	pMove->y = 2.5f;

	// �ړ����x��ቺ
	m_fMoveSpeed = 2.5f;

	// ���o�E���h��Ԃɂ���
	m_state = STATE_REBOUND;
}

//==========================================================================
// ���n����
//==========================================================================
void CBall::Landing(void)
{
	// ���n��Ԃɂ���
	m_state = STATE_LAND;

	// �z�[�~���O�Ώۂ̏�����
	m_pTarget = nullptr;

	// �`�[���̏�����
	m_typeTeam = CGameManager::SIDE_NONE;

	// �U���̏�����
	m_typeAtk = ATK_NONE;
}
