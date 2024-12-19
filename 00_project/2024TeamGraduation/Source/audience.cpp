//==========================================================================
// 
//  �ϋq���� [audience.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "audience.h"
#include "charmManager.h"
#include "shadow.h"
#include "manager.h"

// �h����
#include "audienceAnim.h"
#include "audienceLowPoly.h"
#include "audienceHighPoly.h"
#include "audienceAnim_result.h"
#include "audienceLowPoly_result.h"
#include "audienceHighPoly_result.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int MIN_JUMP = 6;		// �Œ�W�����v��
	const int MAX_JUMP = 12;	// �ő�W�����v��
	const float	RADIUS_SHADOW	 = 44.0f;	// �e�̔��a
	const float	MIN_ALPHA_SHADOW = 0.02f;	// �e�̓����x
	const float	MAX_ALPHA_SHADOW = 0.08f;	// �e�̓����x
	const float GRAVITY_RATE	 = 0.5f;	// �d�͂ɂ����銄��
	const float JUMP_RATE		 = 0.5f;	// �W�����v�͂ɂ����銄��
	const float TIME_SPAWN		 = 2.4f;	// ���ꎞ��
	const float TIME_DESPAWN	 = 3.2f;	// �ޏꎞ��

#if _DEBUG	// TODO�F���[�|�����������猩����
	const float RATE_HIGH = 0.1f;	// �n�C�|���䗦
	const float RATE_LOW  = 0.2f;	// ���[�|���䗦
#else
	const float RATE_HIGH = 0.12f;	// �n�C�|���䗦
	const float RATE_LOW = 0.28f;	// ���[�|���䗦
#endif
	const float RATE_ANIM = 1.0f - (RATE_HIGH + RATE_LOW);	// �A�j���[�V�����䗦
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CAudience::STATE_FUNC CAudience::m_StateFuncList[] =
{
	&CAudience::UpdateSpawn,	// �����Ԃ̍X�V
	&CAudience::UpdateNormal,	// �ʏ��Ԃ̍X�V
	&CAudience::UpdateJump,		// ����オ���Ԃ̍X�V
	&CAudience::UpdateSpecial,	// �X�y�V������Ԃ̍X�V
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
CAudience::CAudience(EObjType type, CGameManager::ETeamSide team, int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_fJumpLevel		(UtilFunc::Transformation::Random(MIN_JUMP * 100, MAX_JUMP * 100) * 0.01f),	// �W�����v��
	m_fLandY			(CGameManager::FIELD_LIMIT),	// ���nY���W
	m_pShadow			(nullptr),		// �e���
	m_team				(team),			// �����`�[��
	m_type				(type),			// �I�u�W�F�N�g���
	m_nArea				(0),			// �ϐ�G���A
	m_posSpawn			(VEC3_ZERO),	// ����ʒu
	m_posWatch			(VEC3_ZERO),	// �ϐ�ʒu
	m_posDespawn		(VEC3_ZERO),	// �ޏ�ʒu
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
CAudience* CAudience::Create(EObjType type, CGameManager::ETeamSide team, CScene::MODE mode)
{
	// �������̊m��
	CAudience* pAudience = nullptr;
	switch (type)
	{ // �I�u�W�F�N�g��ނ��Ƃ̏���
	case CAudience::OBJTYPE_ANIM:
		if (mode == CScene::MODE::MODE_RESULT)
		{
			pAudience = DEBUG_NEW CAudienceAnimResult(type, team);
		}
		else
		{
			pAudience = DEBUG_NEW CAudienceAnim(type, team);
		}
		break;

	case CAudience::OBJTYPE_LOWPOLY:
		if (mode == CScene::MODE::MODE_RESULT)
		{
			pAudience = DEBUG_NEW CAudienceLowPolyResult(type, team);
		}
		else
		{
			pAudience = DEBUG_NEW CAudienceLowPoly(type, team);
		}
		break;

	case CAudience::EObjType::OBJTYPE_HIGHPOLY:
		if (mode == CScene::MODE::MODE_RESULT)
		{
			pAudience = DEBUG_NEW CAudienceHighPolyResult(type, team);
		}
		else
		{
			pAudience = DEBUG_NEW CAudienceHighPoly(type, team);
		}
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
// �e�̐�������
//==========================================================================
HRESULT CAudience::CreateShadow(CObject* pParent)
{
	// �e�̐���
	m_pShadow = CShadow::Create(pParent, RADIUS_SHADOW, MIN_ALPHA_SHADOW, MAX_ALPHA_SHADOW, m_fLandY);
	if (m_pShadow == nullptr) { return E_FAIL; }

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CAudience::Init()
{
	// ���X�g�ɒǉ�
	m_list.Regist(this);

	// �ϐ풆�̐l�������Z
	int nIdxTeam = m_team;
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
	// �e�̍폜
	SAFE_KILL(m_pShadow);

	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CAudience::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԕʏ���
	int nMotion = (this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);

	// ���[�V������ݒ�
	SetMotion(nMotion);
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
	// �ʏ��Ԃł�����オ���Ԃł��X�y�V������Ԃł��Ȃ��ꍇ������
	if (m_state != STATE_NORMAL && m_state != STATE_JUMP && m_state != STATE_SPECIAL) { return; }

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
// �X�y�V�����̐ݒ菈��
//==========================================================================
void CAudience::SetSpecial()
{
	// �ʏ��Ԃł�����オ���Ԃł��X�y�V������Ԃł��Ȃ��ꍇ������
	if (m_state != STATE_NORMAL && m_state != STATE_JUMP && m_state != STATE_SPECIAL) { return; }

	// �X�y�V������Ԃɂ���
	m_state = STATE_SPECIAL;
}

//==========================================================================
// �ޏ�̐ݒ菈��
//==========================================================================
bool CAudience::SetDespawn(EObjType type)
{
	// ���ɑޏꒆ�̏ꍇ������
	if (m_state == STATE_DESPAWN) { return false; }

	// �w���ނł͂Ȃ��ꍇ������
	if (type != OBJTYPE_NONE && type != m_type) { return false; }

	// �ޏ�J�n�ʒu��ۑ�
	m_posDespawnStart = GetPosition();	// ���݂̈ʒu

	// �ϐ풆�̐l�������Z
	int nIdxTeam = m_team;
	m_aNumWatchAll[nIdxTeam]--;

	// �ޏ��Ԃɂ���
	m_state = STATE_DESPAWN;
	return true;
}

//==========================================================================
// �ϐ풆�̐l���ݒ菈��
//==========================================================================
HRESULT CAudience::SetNumWatch(const int nNumWatch, CGameManager::ETeamSide team)
{
	// �`�[�����ݒ肳��Ă��Ȃ��ꍇ������
	if (team != CGameManager::ETeamSide::SIDE_LEFT && team != CGameManager::ETeamSide::SIDE_RIGHT) { return E_FAIL; }

	// �ϐ�l��������̏ꍇ������
	if (nNumWatch == m_aNumWatchAll[team]) { return E_FAIL; }

	if (nNumWatch < m_aNumWatchAll[team])
	{ // �ϐ�l���������ꍇ

		// �l�����ޏ�
		int nNumDespawn = m_aNumWatchAll[team] - nNumWatch;	// �ޏ�l��
		SetDespawnAll(team, nNumDespawn);
	}
	else if (nNumWatch > m_aNumWatchAll[team])
	{ // �ϐ�l�������Ȃ��ꍇ

		int nNumSpawn = nNumWatch - m_aNumWatchAll[team];	// �o��l��
		for (int i = 0; i < nNumSpawn; i++)
		{ // �o��l�����J��Ԃ�

			CAudience::EObjType type = OBJTYPE_NONE;	// �I�u�W�F�N�g���
			if		(m_aNumWatchAll[team] < (int)(MAX_WATCH * RATE_HIGH))				{ type = OBJTYPE_HIGHPOLY; }	// �n�C�|��
			else if	(m_aNumWatchAll[team] < (int)(MAX_WATCH * (RATE_HIGH + RATE_LOW)))	{ type = OBJTYPE_LOWPOLY; 	}	// ���[�|��
			else																		{ type = OBJTYPE_ANIM; }		// �A�j���[�V����

			// ���U���g = ���U���g�p����
			CScene::MODE mode = GET_MANAGER->GetMode();

			// �ϋq�𐶐�
			if (FAILED(CAudience::Create(type, team, mode)))
			{ // �����Ɏ��s�����ꍇ

				return E_FAIL;
			}
		}
	}

	return S_OK;
}

//==========================================================================
// �S�ϐ풆�̐l���擾����
//==========================================================================
int CAudience::GetNumWatchAll(CGameManager::ETeamSide team)
{
	// �`�[�����ݒ肳��Ă��Ȃ��ꍇ������
	if (team != CGameManager::ETeamSide::SIDE_LEFT && team != CGameManager::ETeamSide::SIDE_RIGHT) { return -1; }

	// �����T�C�h�̊ϐ�l����Ԃ�
	return m_aNumWatchAll[team];
}

//==========================================================================
// �S����オ��̐ݒ菈��
//==========================================================================
void CAudience::SetEnableJumpAll(const bool bJump, CGameManager::ETeamSide team)
{
	// �`�[�����ݒ肳��Ă��Ȃ��ꍇ������
	if (team != CGameManager::ETeamSide::SIDE_LEFT && team != CGameManager::ETeamSide::SIDE_RIGHT) { return; }

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
// �S�X�y�V�����̐ݒ菈��
//==========================================================================
void CAudience::SetSpecialAll(CGameManager::ETeamSide team)
{
	// �`�[�����ݒ肳��Ă��Ȃ��ꍇ������
	if (team != CGameManager::ETeamSide::SIDE_LEFT && team != CGameManager::ETeamSide::SIDE_RIGHT) { return; }

	std::list<CAudience*>::iterator itr = m_list.GetEnd();
	while (m_list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CAudience* pAudience = (*itr);	// �ϋq���

		// �w��`�[���ł͂Ȃ��ꍇ����
		if (pAudience->m_team != team) { continue; }

		// �X�y�V������Ԃ�ݒ�
		pAudience->SetSpecial();
	}
}

//==========================================================================
// �S�ޏ�̐ݒ菈��
//==========================================================================
void CAudience::SetDespawnAll(CGameManager::ETeamSide team, const int nNumDespawn)
{
	// �`�[�����ݒ肳��Ă��Ȃ��ꍇ������
	if (team != CGameManager::ETeamSide::SIDE_LEFT && team != CGameManager::ETeamSide::SIDE_RIGHT) { return; }

	CAudience::EObjType type = OBJTYPE_NONE;	// �I�u�W�F�N�g���

	if (nNumDespawn > 0)
	{ // �폜�����w�肳��Ă���ꍇ

		if		(m_aNumWatchAll[team] > (int)(MAX_WATCH * (RATE_HIGH + RATE_LOW)))	{ type = OBJTYPE_ANIM; }		// �A�j���[�V����
		else if	(m_aNumWatchAll[team] > (int)(MAX_WATCH * (RATE_HIGH)))				{ type = OBJTYPE_LOWPOLY; }		// ���[�|��
		else																		{ type = OBJTYPE_HIGHPOLY; }	// �n�C�|��
	}

	int nCurDespawn = 0;	// ���݂̑ޏ�l��
	std::list<CAudience*>::iterator itr = m_list.GetEnd();
	while (m_list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CAudience* pAudience = (*itr);	// �ϋq���

		// �w��`�[���ł͂Ȃ��ꍇ����
		if (pAudience->m_team != team) { continue; }

		// �ޏ��ݒ�
		if (!pAudience->SetDespawn(type)) { continue; }	// ���ɑޏꒆ�̏ꍇ�͎���

		if (nNumDespawn > 0)
		{ // �폜�����w�肳��Ă���ꍇ

			// ���݂̑ޏ�l�������Z
			nCurDespawn++;

			// �����̐l�����ޏꂳ�����ꍇ������
			if (nCurDespawn == nNumDespawn) { return; }

			// ���̍폜�ΏۃI�u�W�F�N�g��ނ�ݒ�
			if		(m_aNumWatchAll[team] > (int)(MAX_WATCH * (RATE_HIGH + RATE_LOW)))	{ type = OBJTYPE_ANIM; }		// �A�j���[�V����
			else if	(m_aNumWatchAll[team] > (int)(MAX_WATCH * (RATE_HIGH)))				{ type = OBJTYPE_LOWPOLY; }		// ���[�|��
			else																		{ type = OBJTYPE_HIGHPOLY; }	// �n�C�|��
		}
	}
}

//==========================================================================
// �SNTR�̐ݒ菈��
//==========================================================================
void CAudience::SetNTRAll(CGameManager::ETeamSide team)
{
	// �`�[�����ݒ肳��Ă��Ȃ��ꍇ������
	if (team != CGameManager::ETeamSide::SIDE_LEFT && team != CGameManager::ETeamSide::SIDE_RIGHT) { return; }

	CAudience::EObjType type = OBJTYPE_NONE;	// �I�u�W�F�N�g���

	int nCurDespawn = 0;	// ���݂̑ޏ�l��
	std::list<CAudience*>::iterator itr = m_list.GetEnd();
	while (m_list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CAudience* pAudience = (*itr);	// �ϋq���

		// �w��`�[���ł͂Ȃ��ꍇ����
		//if (pAudience->m_team != team) { continue; }

		// NTR�ݒ�
		if (!pAudience->SetNTR(team)) { continue; }	// ���ɑޏꒆ�̏ꍇ�͎���

	}
}

//==========================================================================
// �����Ԃ̍X�V����
//==========================================================================
int CAudience::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CCharmManager* pCharm = CCharmManager::GetInstance();	// ���e�}�l�[�W���[
	bool bHype = false;										// ����オ��t���O

	if (pCharm != nullptr)
	{
		bHype = pCharm->IsHype(GetTeam());
	}
	else if (GET_MANAGER->GetScene()->GetMode() == CScene::MODE::MODE_RESULT)
	{// ���U���g�͊m�萷��オ��
		bHype = true;
	}

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

		if (bHype)
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

		// �X�|�[���I�����̐ݒ�
		EndSettingSpawn();

		// ��Ԃ�J�ڂ�����
		if (bHype)	{ m_state = STATE_JUMP; }	// ����オ���Ă���Ȃ琷��オ���Ԃɂ���
		else		{ m_state = STATE_NORMAL; }	// ����ȊO�Ȃ�ʏ��Ԃɂ���
	}

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(move);		// �ړ���

	return 0;	// ���̊֐��̕Ԃ�l�͎g�p���Ȃ�
}

//==========================================================================
// �ʏ��Ԃ̍X�V����
//==========================================================================
int CAudience::UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CCharmManager* pCharm = CCharmManager::GetInstance();	// ���e�}�l�[�W���[
	bool bHype = false;										// ����オ��t���O

	if (pCharm != nullptr)
	{
		bHype = pCharm->IsHype(GetTeam());
	}
	else if (GET_MANAGER->GetScene()->GetMode() == CScene::MODE::MODE_RESULT)
	{// ���U���g�͊m�萷��オ��
		bHype = true;
	}

	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 move = GetMove();	// �ړ���

	// �d�͂̍X�V
	UpdateGravity(&pos, &move, fDeltaTime, fDeltaRate, fSlowRate);

	if (bHype)
	{ // ����オ���Ă���ꍇ

		// ����オ���Ԃɂ���
		m_state = STATE_JUMP;
	}

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(move);		// �ړ���

	return 0;	// ���̊֐��̕Ԃ�l�͎g�p���Ȃ�
}

//==========================================================================
// ����オ���Ԃ̍X�V����
//==========================================================================
int CAudience::UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CCharmManager* pCharm = CCharmManager::GetInstance();	// ���e�}�l�[�W���[
	bool bHype = false;										// ����オ��t���O

	if (pCharm != nullptr)
	{
		bHype = pCharm->IsHype(GetTeam());
	}
	else if (GET_MANAGER->GetScene()->GetMode() == CScene::MODE::MODE_RESULT)
	{// ���U���g�͊m�萷��オ��
		bHype = true;
	}

	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 move = GetMove();	// �ړ���

	// �d�͂̍X�V
	if (UpdateGravity(&pos, &move, fDeltaTime, fDeltaRate, fSlowRate))
	{ // ���n�����ꍇ

		if (!bHype)
		{ // ����オ���Ă��Ȃ��ꍇ

			// �ʏ��Ԃɂ���
			m_state = STATE_NORMAL;
		}
		else
		{ // ����オ���Ă���ꍇ

			// �c�ړ��ʂ�^����
			move.y = m_fJumpLevel;
		}
	}

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(move);		// �ړ���

	return 0;	// ���̊֐��̕Ԃ�l�͎g�p���Ȃ�
}

//==========================================================================
// �X�y�V������Ԃ̍X�V����
//==========================================================================
int CAudience::UpdateSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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

	return 0;	// ���̊֐��̕Ԃ�l�͎g�p���Ȃ�
}

//==========================================================================
// �ޏ��Ԃ̍X�V����
//==========================================================================
int CAudience::UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 move = GetMove();	// �ړ���

	// �o�ߎ��Ԃ����Z
	m_fTimeState += fDeltaTime * fSlowRate;

	// �ϐ�ʒu�������ʒu�Ɉړ� (Y���W�͖�������)
	MyLib::Vector3 posDest = UtilFunc::Correction::EasingLinear(m_posDespawnStart, m_posDespawn, 0.0f, TIME_DESPAWN, m_fTimeState);
	pos.x = posDest.x;
	pos.z = posDest.z;

	// �d�͂̍X�V
	UpdateGravity(&pos, &move, fDeltaTime, fDeltaRate, fSlowRate);

	if (m_fTimeState >= TIME_DESPAWN)
	{ // ���Ԃ��o�߂����ꍇ

		// �o�ߎ��Ԃ�������
		m_fTimeState = 0.0f;

		// ����ʒu�ɕ␳
		pos.x = m_posDespawn.x;
		pos.z = m_posDespawn.z;

		// ���g���폜����
		Kill();
		return 0;	// ���̊֐��̕Ԃ�l�͎g�p���Ȃ�
	}

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(move);		// �ړ���

	return 0;	// ���̊֐��̕Ԃ�l�͎g�p���Ȃ�
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

	if (pPos->y < m_fLandY)
	{ // �ϋq���n�ʂɖ�����Ă���ꍇ

		// �ϋq�̈ʒu��␳
		pPos->y = m_fLandY;
		return true;
	}

	return false;
}
