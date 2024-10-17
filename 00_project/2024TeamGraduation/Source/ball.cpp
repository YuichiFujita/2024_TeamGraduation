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
	const char* MODEL = "data\\MODEL\\dadgeball\\dodgeball.x";	// �{�[�����f��
	const float RADIUS = 7.0f;		// ���a
	const float REV_MOVE = 0.1f;	// �ړ��ʂ̕␳�W��
	const float MAX_DIS = 1000.0f;	// �z�[�~���O����ő勗��

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

	// ������Ԃɂ���
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
	m_fMoveSpeed = 30.0f;

	// TODO�F��
	float fRot = pPlayer->GetRotation().y + D3DX_PI;
	MyLib::Vector3 vec = MyLib::Vector3(sinf(fRot), 0.0f, cosf(fRot));
	vec.Normal();
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
	m_fMoveSpeed = 40.0f;

	// TODO�F��
	float fRot = pPlayer->GetRotation().y + D3DX_PI;
	MyLib::Vector3 vec = MyLib::Vector3(sinf(fRot), -0.25f, cosf(fRot));
	vec.Normal();
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
	MyLib::Vector3 vec = MyLib::Vector3(sinf(fRot), 0.01f, cosf(fRot));
	vec.Normal();
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

		// �����J��
		Fall();
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
	SetPosition(m_pPlayer->GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f));	// �v���C���[���
}

//==========================================================================
// �U����Ԃ̍X�V����
//==========================================================================
void CBall::UpdateThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 vecMove = GetMove();	// �ړ���

	if (m_pTarget == nullptr)
	{
		// �ړ�
		UpdateMove(&pos, &vecMove, fDeltaRate, fSlowRate);
	}
	else
	{
		MyLib::Vector3 posTarget = m_pTarget->GetPosition();	// �^�[�Q�b�g�ʒu

		// ���݌������擾
		float fCurAngle = vecMove.GetHorizontalAngle(VEC3_ZERO);
		UtilFunc::Transformation::RotNormalize(fCurAngle);

		// �ڕW�������擾
		float fDestAngle = posTarget.AngleXZ(pos);
		UtilFunc::Transformation::RotNormalize(fDestAngle);

		// �����������v�Z
		float fDiffAngle = fDestAngle - fCurAngle;
		UtilFunc::Transformation::RotNormalize(fDiffAngle);

		// ���݌������X�V
		fCurAngle += fDiffAngle * 0.5f;
		UtilFunc::Transformation::RotNormalize(fCurAngle);

		// ���݌�������ړ��x�N�g�����Čv�Z
		vecMove = MyLib::Vector3(sinf(fCurAngle), -0.25f, cosf(fCurAngle));
		vecMove.Normal();

		// �ړ�
		UpdateMove(&pos, &vecMove, fDeltaRate, fSlowRate);
	}

	// �n�ʂ̒��n
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // ���n�����ꍇ

		// �����J��
		Fall();
	}

	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer(&pos);

	// ���𔽉f
	SetPosition(pos);	// �ʒu
	SetMove(vecMove);	// �ړ���
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

	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer(&pos);

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
	m_fGravity -= mylib_const::GRAVITY * fDeltaRate * fSlowRate;

	// �ʒu���ړ�������
	*pPos += (*pMove * m_fMoveSpeed) * fDeltaRate * fSlowRate;
	pPos->y += m_fGravity * fDeltaRate * fSlowRate;

	// �ړ��ʂ�����������
	m_fMoveSpeed += (0.0f - m_fMoveSpeed) * (REV_MOVE * fDeltaRate * fSlowRate);
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
		bool bHit = UtilFunc::Collision::CylinderCircleCylinder
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
			pPlayer->Hit(this);
			return true;
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

	float fMinDis = MAX_DIS;	// �{�[������߂��v���C���[
	CPlayer* pTarget = nullptr;	// �ڕW�^�[�Q�b�g

	MyLib::Vector3 posBall = GetPosition();	// �{�[���ʒu
	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���
		if (pPlayer == m_pPlayer) { continue; }	// �������v���C���[�{�l�̏ꍇ����

		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// �v���C���[�ʒu

		// TODO�F�����Ɏ��E���̔���

		// �v���C���[�Ԃ̋����̑���
		float fCurDis = posPlayer.Distance(posBall);
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

	// �U����Ԃɂ���
	m_state = STATE_THROW;

	// �z�[�~���O�Ώۂ̐ݒ�
	m_pTarget = CollisionThrow();

	// �L���b�`���Ă����v���C���[��j��
	m_pPlayer = nullptr;

	// �v���C���[����ۑ����̃{�[����j��
	pPlayer->SetBall(nullptr);
}

//==========================================================================
// ��������
//==========================================================================
void CBall::Fall(void)
{
	// ������Ԃɂ���
	m_state = STATE_FALL;

	// �z�[�~���O�Ώۂ̏�����
	m_pTarget = nullptr;

	// �`�[���̏�����
	m_typeTeam = CGameManager::SIDE_NONE;

	// �U���̏�����
	m_typeAtk = ATK_NONE;
}
