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

	const float RADIUS = 60.0f;	// TODO�F�����
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

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CAudience::CAudience(EObjType type, int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_fJumpLevel (UtilFunc::Transformation::Random(MIN_JUMP * 100, MAX_JUMP * 100) * 0.01f),	// �W�����v��
	m_type		 (type),		// �I�u�W�F�N�g���
	m_state		 (STATE_SPAWN)	// ���
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
CAudience* CAudience::Create(EObjType type)
{
	// �������̊m��
	CAudience* pAudience = nullptr;
	switch (type)
	{ // �I�u�W�F�N�g��ނ��Ƃ̏���
	case CAudience::OBJTYPE_ANIM:
		pAudience = DEBUG_NEW CAudienceAnim(type);
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
// �����Ԃ̍X�V����
//==========================================================================
void CAudience::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 move = GetMove();	// �ړ���



	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(move);		// �ړ���
}

//==========================================================================
// �ʏ��Ԃ̍X�V����
//==========================================================================
void CAudience::UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ����オ���Ԃ̍X�V����
//==========================================================================
void CAudience::UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 move = GetMove();	// �ړ���

	// �d�͂�^����
	move.y -= mylib_const::GRAVITY * fDeltaRate * fSlowRate;

	// �ʒu�ɏd�͂𔽉f
	pos.y += move.y * fDeltaRate * fSlowRate;

	if (pos.y - RADIUS < CGameManager::FIELD_LIMIT)
	{ // �{�[�����n�ʂɖ�����Ă���ꍇ

		// �{�[���̈ʒu��␳
		pos.y = CGameManager::FIELD_LIMIT + RADIUS;

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



	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(move);		// �ړ���
}
