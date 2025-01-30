//=============================================================================
// 
//  �v���C���[AI�R���g���[��_���ߏ��� [playerAIcontrol_start.cpp]
//  Author : TakeruOgasawara
// 
//=============================================================================
#include "playerAIcontrol_start.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "playerAction.h"

// player
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol_idle.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControlStart::CPlayerAIControlStart()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAIControlStart::~CPlayerAIControlStart()
{

}

//==========================================================================
// ����
//==========================================================================
CPlayerAIControlStart* CPlayerAIControlStart::Create(CPlayer* player)
{
	if (!player) assert(("�v���C���[��񂪖�����", false));

	// �������̊m��
	CPlayerAIControlStart* pObj = nullptr;

	if (pObj == nullptr)
	{
		pObj = DEBUG_NEW CPlayerAIControlStart;

		// �N���X�̏�����
		if (FAILED(pObj->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//================================================================================
// ����������
//================================================================================
HRESULT CPlayerAIControlStart::Init()
{
	// �e�N���X�̏�����
	CPlayerAIControlMode::Init();

	return S_OK;
}

//================================================================================
// �I������
//================================================================================
void CPlayerAIControlStart::Uninit()
{
	// �e�N���X�̏I������
	CPlayerAIControlMode::Uninit();
}

//================================================================================
// �X�V����
//================================================================================
void CPlayerAIControlStart::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �s��������������
	Gooooo();

	// �e�N���X�̍X�V�i�Ō���ɐݒu�j
	CPlayerAIControlMode::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �s��������������
//==========================================================================
void CPlayerAIControlStart::Gooooo()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (!pBall)
	{// �{�[�����Ȃ��ꍇ
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
		SetActionFlag(EActionFlag::ACTION_NONE);
		SetMode(EMode::MODE_IDLE);
		return;
	}

	if (IsPicksUpBall() ||									// �������߂��v���C���[������
		pBall->GetState() != CBall::EState::STATE_SPAWN)	// �{�[����������Ԃł͂Ȃ��ꍇ
	{
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
		SetActionFlag(EActionFlag::ACTION_NONE);
		SetMode(EMode::MODE_IDLE);
		return;
	}

	CPlayer* pPlayer = pBall->GetPlayer();
	if (pPlayer)
	{// �N�����{�[�����������ꍇ
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
		SetActionFlag(EActionFlag::ACTION_NONE);
		SetMode(EMode::MODE_IDLE);
		return;
	}

	// �{�[��������
	SeeBall();

	SetActionFlag(EActionFlag::ACTION_JUMP);
	SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);
}

//==========================================================================
// �{�[��������
//==========================================================================
void CPlayerAIControlStart::SeeBall()
{
	// �{�[�����̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// �{�[���Ƃ̊p�x
	float angle = pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// �p�x�̐ݒ�
	pAI->SetRotDest(angle);
}


//==========================================================================
// �N���{�[�������ɍs���܂����H
//==========================================================================
bool CPlayerAIControlStart::IsPicksUpBall()
{
	float fMyDis = 1000000.0f;	// �����̃{�[���Ƃ̋���

	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	// �����̏��擾
	CGameManager::ETeamSide typeTeam = pAI->GetTeam();	// �`�[���^�C�v
	MyLib::Vector3 Mypos = pAI->GetPosition();	// �ʒu���

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return true;
	if (pBall->GetPlayer()) return true;

	// �{�[���ʒu�擾
	MyLib::Vector3 posBall = pBall->GetPosition();
	fMyDis = Mypos.DistanceXZ(posBall);	// ��������{�[���Ƃ̋���

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���

		if (pPlayer == pAI) continue;		// �����̏ꍇ
		if (pPlayer->GetTeam() != typeTeam) continue;		// �`�[�����Ⴄ�ꍇ
		if ((pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER) ||
			(pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT))
		{// �O��&&���[�U�[&&�����̏ꍇ
			continue;
		}

		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// �v���C���[�ʒu

		// �`�[������{�[���Ƃ̋��������߂�
		float fLength = posPlayer.DistanceXZ(posBall);

		if (fLength < fMyDis)
		{ // ���߂������v���C���[�������ꍇ
			return true;
		}
	}

	return false;
}
