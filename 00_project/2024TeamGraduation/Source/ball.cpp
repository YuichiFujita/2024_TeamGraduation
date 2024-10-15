//=============================================================================
// 
//  �{�[������ [ball.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "ball.h"
#include "player.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\dadgeball\\dodgeball.x";
	const float RADIUS = 7.0f;
	const float REV_MOVE = 0.1f;

	const char* DEBUG_STATE_PRINT[] =	// �f�o�b�O�\���p���
	{
		"SPAWN ������� (�t���[�{�[��)",
		"CATCH ������� (�v���C���[����)",
		"THROW �U����� (�U������ON)",
		"FALL  ������� (�n�ʗ���)"
	};
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CBall::STATE_FUNC CBall::m_SampleFuncList[] =
{
	&CBall::UpdateSpawn,	// ������Ԃ̍X�V
	&CBall::UpdateCatch,	// ������Ԃ̍X�V
	&CBall::UpdateThrow,	// �U����Ԃ̍X�V
	&CBall::UpdateFall,		// ������Ԃ̍X�V
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CBall> CBall::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBall::CBall(int nPriority) : CObjectX(nPriority),
	m_typeTeam	 (CGameManager::SIDE_NONE),	// �`�[���T�C�h
	m_typeAtk	 (ATK_NONE),	// �U�����
	m_state		 (STATE_SPAWN),	// ���
	m_fStateTime (0.0f),		// ��ԃJ�E���^�[
	m_pPlayer	 (nullptr)		// �v���C���[���
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
	m_List.Regist(this);

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
	m_List.Delete(this);

	// �e�N���X�̏I��
	CObjectX::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CBall::Kill()
{
	// ���X�g����폜
	m_List.Delete(this);

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
// ������Ԃ̍X�V����
//==========================================================================
void CBall::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 move = GetMove();	// �ړ���

	// �ړ�
	UpdateMove(&pos, &move, fDeltaRate, fSlowRate);

	// �n�ʂ̒��n
	if (UpdateLanding(&pos, &move, fDeltaRate, fSlowRate))
	{ // ���n�����ꍇ

		// ������Ԃɂ���
		m_state = STATE_FALL;
	}

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(move);		// �ړ���
}

//==========================================================================
// ������Ԃ̍X�V����
//==========================================================================
void CBall::UpdateCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// TODO�F�����̈ʒu�ɐe�q�t��
	SetPosition(m_pPlayer->GetPosition() + MyLib::Vector3(0.0f, 100.0f, 0.0f));	// �v���C���[���
}

//==========================================================================
// �U����Ԃ̍X�V����
//==========================================================================
void CBall::UpdateThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 move = GetMove();	// �ړ���

	// �ړ�
	UpdateMove(&pos, &move, fDeltaRate, fSlowRate);

	// �n�ʂ̒��n
	if (UpdateLanding(&pos, &move, fDeltaRate, fSlowRate))
	{ // ���n�����ꍇ

		// ������Ԃɂ���
		m_state = STATE_FALL;
	}

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(move);		// �ړ���
}

//==========================================================================
// ������Ԃ̍X�V����
//==========================================================================
void CBall::UpdateFall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 move = GetMove();	// �ړ���

	// �ړ�
	UpdateMove(&pos, &move, fDeltaRate, fSlowRate);

	// �n�ʂ̒��n
	UpdateLanding(&pos, &move, fDeltaRate, fSlowRate);

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(move);		// �ړ���
}

//==========================================================================
// �ړ�����
//==========================================================================
void CBall::UpdateMove(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate)
{
	// �d�͂�^����
	pMove->y -= mylib_const::GRAVITY * fDeltaRate * fSlowRate;

	// �ʒu���ړ�������
	*pPos += *pMove * fDeltaRate * fSlowRate;

	// �ړ��ʂ�����������
	pMove->x += (0.0f - pMove->x) * (REV_MOVE * fDeltaRate * fSlowRate);
	pMove->z += (0.0f - pMove->z) * (REV_MOVE * fDeltaRate * fSlowRate);
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
		return true;
	}

	return false;
}
