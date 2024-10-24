//==========================================================================
// 
//  �ϋq���� [audience.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "audience.h"
#include "audienceAnim.h"
#include "gameManager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int MIN_JUMP = 6;		// �Œ�W�����v��
	const int MAX_JUMP = 12;	// �ő�W�����v��
	const float GRAVITY_RATE = 0.5f;	// �d�͂ɂ����銄��
	const float JUMP_RATE = 0.5f;		// �W�����v�͂ɂ����銄��
	const float TIME_SPAWN = 2.4f;		// ���ꎞ��
	const float TIME_DESPAWN = 3.2f;	// �ޏꎞ��
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CAudience::STATE_FUNC CAudience::m_StateFuncList[] =
{
	&CAudience::UpdateSpawn,	// �����Ԃ̍X�V
	&CAudience::UpdateNormal,	// �ʏ��Ԃ̍X�V
	&CAudience::UpdateJump,		// ����オ���Ԃ̍X�V
	&CAudience::UpdateDespawn,	// �ޏ��Ԃ̍X�V
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CAudience> CAudience::m_list = {};	// ���X�g
int CAudience::m_aNumWatchAll[2] = {};	// �ϐ풆�̐l��

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CAudience::CAudience(EObjType type, CGameManager::TeamSide team, int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_fJumpLevel		(UtilFunc::Transformation::Random(MIN_JUMP * 100, MAX_JUMP * 100) * 0.01f),	// �W�����v��
	m_team				(team),			// �����`�[��
	m_type				(type),			// �I�u�W�F�N�g���
	m_posSpawn			(VEC3_ZERO),	// ����ʒu
	m_posWatch			(VEC3_ZERO),	// �ϐ�ʒu
	m_posDespawnStart	(VEC3_ZERO),	// �ޏ�J�n�ʒu
	m_state				(STATE_SPAWN),	// ���
	m_fTimeState		(0.0f)			// ��ԊǗ�����
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CAudience::~CAudience()
{

}

//==========================================================================
// ��������
//==========================================================================
CAudience* CAudience::Create(EObjType type, CGameManager::TeamSide team)
{
	// �������̊m��
	CAudience* pAudience = nullptr;
	switch (type)
	{ // �I�u�W�F�N�g��ނ��Ƃ̏���
	case CAudience::OBJTYPE_ANIM:
		pAudience = DEBUG_NEW CAudienceAnim(type, team);
		break;

	default:
		assert(false);
		break;
	}

	if (pAudience != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pAudience->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pAudience);
			return nullptr;
		}
	}

	return pAudience;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CAudience::Init()
{
	// ���X�g�ɒǉ�
	m_list.Regist(this);

	// �ϐ풆�̐l�������Z
	int nIdxTeam = m_team - 1;
	m_aNumWatchAll[nIdxTeam]++;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CAudience::Uninit()
{
	// ���X�g����폜
	m_list.Delete(this);

	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �폜����
//==========================================================================
void CAudience::Kill()
{
	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CAudience::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԕʏ���
	(this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CAudience::Draw()
{

}

//==========================================================================
// ����オ��̐ݒ菈��
//==========================================================================
void CAudience::SetEnableJump(const bool bJump)
{
	// �ʏ��Ԃł�����オ���Ԃł��Ȃ��ꍇ
	if (m_state != STATE_NORMAL && m_state != STATE_JUMP) { return; }

	if (bJump)
	{ // ����オ��ꍇ

		// ����オ���Ԃɂ���
		m_state = STATE_JUMP;
	}
	else
	{ // ����オ��Ȃ��ꍇ

		// �ʏ��Ԃɂ���
		m_state = STATE_NORMAL;
	}
}

//==========================================================================
// �ޏ�̐ݒ菈��
//==========================================================================
void CAudience::SetDespawn()
{
	// �ޏ�J�n�ʒu��ۑ�
	m_posDespawnStart = GetPosition();	// ���݂̈ʒu

	// �ϐ풆�̐l�������Z
	int nIdxTeam = m_team - 1;
	m_aNumWatchAll[nIdxTeam]--;

	// �ޏ��Ԃɂ���
	m_state = STATE_DESPAWN;
}

//==========================================================================
// �S�ϐ풆�̐l���擾����
//==========================================================================
int CAudience::GetNumWatchAll(CGameManager::TeamSide team)
{
	// �`�[�����ݒ肳��Ă��Ȃ��ꍇ������
	if (team != CGameManager::TeamSide::SIDE_LEFT && team != CGameManager::TeamSide::SIDE_RIGHT) { return -1; }

	// �����T�C�h�̊ϐ�l����Ԃ�
	int nIdxTeam = team - 1;
	return m_aNumWatchAll[nIdxTeam];
}

//==========================================================================
// �S����オ��̐ݒ菈��
//==========================================================================
void CAudience::SetEnableJumpAll(const bool bJump, CGameManager::TeamSide team)
{
	// �`�[�����ݒ肳��Ă��Ȃ��ꍇ������
	if (team != CGameManager::TeamSide::SIDE_LEFT && team != CGameManager::TeamSide::SIDE_RIGHT) { return; }

	std::list<CAudience*>::iterator itr = m_list.GetEnd();
	while (m_list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CAudience* pAudience = (*itr);	// �ϋq���

		// �w��`�[���ł͂Ȃ��ꍇ����
		if (pAudience->m_team != team) { continue; }

		// ����オ��󋵂�ݒ�
		pAudience->SetEnableJump(bJump);
	}
}

//==========================================================================
// �S�ޏ�̐ݒ菈��
//==========================================================================
void CAudience::SetDespawnAll(CGameManager::TeamSide team)
{
	// �`�[�����ݒ肳��Ă��Ȃ��ꍇ������
	if (team != CGameManager::TeamSide::SIDE_LEFT && team != CGameManager::TeamSide::SIDE_RIGHT) { return; }

	std::list<CAudience*>::iterator itr = m_list.GetEnd();
	while (m_list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CAudience* pAudience = (*itr);	// �ϋq���

		// �w��`�[���ł͂Ȃ��ꍇ����
		if (pAudience->m_team != team) { continue; }

		// �ޏ��ݒ�
		pAudience->SetDespawn();
	}
}

//==========================================================================
// �����Ԃ̍X�V����
//==========================================================================
void CAudience::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 move = GetMove();	// �ړ���

	// �o�ߎ��Ԃ����Z
	m_fTimeState += fDeltaTime * fSlowRate;

	// ����ʒu����ϐ�ʒu�Ɉړ� (Y���W�͖�������)
	MyLib::Vector3 posDest = UtilFunc::Correction::EasingLinear(m_posSpawn, m_posWatch, 0.0f, TIME_SPAWN, m_fTimeState);
	pos.x = posDest.x;
	pos.z = posDest.z;

	// �d�͂̍X�V
	if (UpdateGravity(&pos, &move, fDeltaTime, fDeltaRate, fSlowRate, GRAVITY_RATE))
	{ // ���n�����ꍇ

		// TODO�F�����Ő���オ��^�C�~���O�����m�F
		if (true)
		{ // ����オ���Ă���ꍇ

			// �c�ړ��ʂ�^����
			move.y = m_fJumpLevel * JUMP_RATE;
		}
	}

	if (m_fTimeState >= TIME_SPAWN)
	{ // ���Ԃ��o�߂����ꍇ

		// �o�ߎ��Ԃ�������
		m_fTimeState = 0.0f;

		// �ϐ�ʒu�ɕ␳
		pos.x = m_posWatch.x;
		pos.z = m_posWatch.z;

		// TODO�F�����Ő���オ��^�C�~���O�����m�F
		if (true)	{ m_state = STATE_JUMP; }	// ����オ���Ă���Ȃ琷��オ���Ԃɂ���
		else		{ m_state = STATE_NORMAL; }	// ����ȊO�Ȃ�ʏ��Ԃɂ���
	}

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(move);		// �ړ���
}

//==========================================================================
// �ʏ��Ԃ̍X�V����
//==========================================================================
void CAudience::UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 move = GetMove();	// �ړ���

	// �d�͂̍X�V
	UpdateGravity(&pos, &move, fDeltaTime, fDeltaRate, fSlowRate);

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(move);		// �ړ���
}

//==========================================================================
// ����オ���Ԃ̍X�V����
//==========================================================================
void CAudience::UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 move = GetMove();	// �ړ���

	// �d�͂̍X�V
	if (UpdateGravity(&pos, &move, fDeltaTime, fDeltaRate, fSlowRate))
	{ // ���n�����ꍇ

		// �c�ړ��ʂ�^����
		move.y = m_fJumpLevel;
	}

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(move);		// �ړ���
}

//==========================================================================
// �ޏ��Ԃ̍X�V����
//==========================================================================
void CAudience::UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 move = GetMove();	// �ړ���

	// �ޏ�ʒu���쐬
	MyLib::Vector3 posDespawn = m_posSpawn;	// �ޏ�ʒu
	posDespawn.x = -m_posSpawn.x;			// X���W�𔽓]������

	// �o�ߎ��Ԃ����Z
	m_fTimeState += fDeltaTime * fSlowRate;

	// �ϐ�ʒu�������ʒu�̋t�����Ɉړ� (Y���W�͖�������)
	MyLib::Vector3 posDest = UtilFunc::Correction::EasingLinear(m_posDespawnStart, posDespawn, 0.0f, TIME_DESPAWN, m_fTimeState);
	pos.x = posDest.x;
	pos.z = posDest.z;

	// �d�͂̍X�V
	UpdateGravity(&pos, &move, fDeltaTime, fDeltaRate, fSlowRate);

	if (m_fTimeState >= TIME_DESPAWN)
	{ // ���Ԃ��o�߂����ꍇ

		// �o�ߎ��Ԃ�������
		m_fTimeState = 0.0f;

		// �ϐ�ʒu�ɕ␳
		pos.x = posDespawn.x;
		pos.z = posDespawn.z;

		// ���g���폜����
		Kill();
		return;
	}

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(move);		// �ړ���
}

//==========================================================================
// �d�͂̍X�V����
//==========================================================================
bool CAudience::UpdateGravity(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaTime, const float fDeltaRate, const float fSlowRate, const float fGravityRate)
{
	// �d�͂�^����
	pMove->y -= (mylib_const::GRAVITY * fGravityRate) * fDeltaRate * fSlowRate;

	// �ʒu�ɏd�͂𔽉f
	pPos->y += pMove->y * fDeltaRate * fSlowRate;

	if (pPos->y < CGameManager::FIELD_LIMIT)
	{ // �ϋq���n�ʂɖ�����Ă���ꍇ

		// �ϋq�̈ʒu��␳
		pPos->y = CGameManager::FIELD_LIMIT;
		return true;
	}

	return false;
}
