//=============================================================================
// 
//  �v���C���[�R���g���[������ [playerUsercontrol_action.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playerUsercontrol_action.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "ball.h"
#include "playerAction.h"
#include "teamStatus.h"
#include "playerStatus.h"
#include "playerManager.h"
#include "debugproc.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float TAPTIME = 0.125f;		// �^�b�v�̓��͎���
	const float TAPRATE_MIN = 0.6f;	// �^�b�v�̍ŏ�����
	const float TAPRATE_MAX = 1.0f;	// �^�b�v�̍ő劄��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerUserControlAction::CPlayerUserControlAction()
{

}

//==========================================================================
// �L���b�`
//==========================================================================
void CPlayerUserControlAction::Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (player->GetBall() != nullptr) return;

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	CMotion* pMotion = player->GetMotion();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx()))
	{
		// �A�N�V�����p�^�[���ύX
		CPlayer::EMotion motion = CPlayer::EMotion::MOTION_CATCH_STANCE;
		if (player->IsJump())
		{
			motion = CPlayer::EMotion::MOTION_CATCH_STANCE_JUMP;
		}
		SetPattern(player, motion, CPlayer::EAction::ACTION_CATCH);
	}
}

//==========================================================================
// ����
//==========================================================================
void CPlayerUserControlAction::Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();

	if (pBall == nullptr) return;

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	CMotion* pMotion = player->GetMotion();

	// �^�b�v�擾
	CInputGamepad::STapInfo tapInfo = pPad->GetTap(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx(), TAPTIME);

	if (pKey->GetTrigger(DIK_K) ||
		tapInfo.bInput)
	{// �^�b�v�͈͂̓p�X
		SetPattern(player, CPlayer::EMotion::MOTION_THROW_PASS, CPlayer::EAction::ACTION_THROW);
	}

	if (pKey->GetTrigger(DIK_RETURN) ||
		tapInfo.bComplete)
	{// �^�b�v�͈͂𒴂��Ă����ꍇ�͓���
		ThrowSetting(player);
	}
}

//==========================================================================
// �W�����v
//==========================================================================
void CPlayerUserControlAction::Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	
	if (!player->IsJump() &&
		(pKey->GetTrigger(DIK_SPACE) || 
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, player->GetMyPlayerIdx())))
	{// �W�����v���ĂȂ� && �W�����v�{�^��

		// �W�����v�g���K�[ON
		SetEnableJumpTrigger(true);

		// �W�����v�ݒ�
		JumpSetting(player);
	}

	if (pKey->GetPress(DIK_SPACE) ||
		pPad->GetPress(CInputGamepad::BUTTON::BUTTON_A, player->GetMyPlayerIdx()))
	{// �W�����v�{�^���z�[���h�ŏ㏸
		JumpFloat(player);
	}

	if ((!pKey->GetPress(DIK_SPACE) && !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_A, player->GetMyPlayerIdx())))
	{// �W�����v�{�^��������

		// �W�����v�g���K�[OFF
		SetEnableJumpTrigger(false);
	}
}

//==========================================================================
// �X�y�V����
//==========================================================================
void CPlayerUserControlAction::Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();
	CGameManager* pGameManager = CGameManager::GetInstance();
	CGameManager::ETeamSide playerTeam = player->GetTeam();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(playerTeam);
	
	// �X�y�V�����Q�[�WMAX�{�{�[��������
	if (pBall == nullptr ||	!pTeamStatus->IsMaxSpecial()) return;

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_X) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_LB, player->GetMyPlayerIdx()))
	{
		//����
		SpecialSetting(player, pBall, pTeamStatus);
	}
}

//==========================================================================
// ���e�{�^��
//==========================================================================
void CPlayerUserControlAction::Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	player->GetActionPattern()->SetEnableCharm(false);

	if (pKey->GetPress(DIK_C) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_RB, player->GetMyPlayerIdx()))
	{
		//���e�A�N�V���������������
		player->GetActionPattern()->SetEnableCharm(true);
	}
}

//==========================================================================
// ���[�U�[�ύX
//==========================================================================
void CPlayerUserControlAction::UserChange(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#if 0
	// �C���v�b�g���擾
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �v���C���[���쌠�C���f�b�N�X�擾
	const int nPadIdx = player->GetMyPlayerIdx();

	// �E�X�e�B�b�N�̃g���K�[���͌��m�t���O
	const bool bInput = pPad->GetRStickTrigger(nPadIdx, CInputGamepad::STICK_AXIS::STICK_X)
					 || pPad->GetRStickTrigger(nPadIdx, CInputGamepad::STICK_AXIS::STICK_Y);

	if (bInput && !CPlayerManager::GetInstance()->IsUserChange())
	{

		MyLib::Vector3 posThis = player->GetPosition();	// ���g�̃v���C���[�ʒu
		CPlayer* pChange = nullptr;						// �ύX�v���C���[
		float fRotMinDiff = D3DX_PI * 2.0f;				// 
		float fRotStick = pPad->GetStickRotR(nPadIdx);	// �X�e�B�b�N����

		// TODO�F�����G�t�F�N�g
#if 0
		MyLib::Vector3 posStick = player->GetPosition();	// ���g�̃v���C���[�ʒu
		posStick.x += sinf(fRotStick) * 150.0f;
		posStick.z += cosf(fRotStick) * 150.0f;

		// ���o
		CEffect3D::Create(
			posStick + MyLib::Vector3(0.0f, 50.0f, 0.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			MyLib::color::White(),
			20.0f, 6.0f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

		CListManager<CPlayer> list = CPlayerManager::GetInstance()->GetInList(player->GetTeam());	// ����v���C���[���X�g
		std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
		while (list.ListLoop(itr))
		{ // ���X�g���̗v�f�����J��Ԃ�

			CPlayer* pItrPlayer = (*itr);	// �v���C���[���

			// ���Ƀ��[�U�[�̏ꍇ����
			if (pItrPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER) { continue; }

			float fRotSpace = pItrPlayer->GetPosition().AngleXZ(posThis);
			float fRotStickA = (fRotStick + D3DX_PI);
			float fRotSpaceA = (fRotSpace + D3DX_PI);
			float fRotDiff = fabsf(fRotStickA - fRotSpaceA);

			// TODO�F�����G�t�F�N�g
#if 0
			MyLib::Vector3 posDir = player->GetPosition();	// ���g�̃v���C���[�ʒu
			posDir.x += sinf(fRotSpace) * 125.0f;
			posDir.z += cosf(fRotSpace) * 125.0f;

			// ���o
			CEffect3D::Create(
				posDir + MyLib::Vector3(0.0f, 50.0f, 0.0f),
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				MyLib::color::Red(),
				15.0f, 6.0f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

			if (pChange == nullptr || fRotDiff < fRotMinDiff)
			{ // ��荷���������������̏ꍇ

				// ����̍���������ۑ�
				fRotMinDiff = fRotDiff;

				// �ύX����v���C���[��ۑ�
				pChange = pItrPlayer;
			}
		}

		if (pChange != nullptr)
		{ // �ύX�Ώۂ����݂���ꍇ

			// TODO�F�����G�t�F�N�g
#if 0
			float fRotChange = pChange->GetPosition().AngleXZ(posThis);
			MyLib::Vector3 posChange = player->GetPosition();	// ���g�̃v���C���[�ʒu
			posChange.x += sinf(fRotChange) * 175.0f;
			posChange.z += cosf(fRotChange) * 175.0f;

			// ���o
			CEffect3D::Create(
				posChange + MyLib::Vector3(0.0f, 50.0f, 0.0f),
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				MyLib::color::Blue(),
				10.0f, 6.0f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

			// �x�[�X�̓���ւ�
			CPlayerManager::GetInstance()->SwapBase(player, pChange);

			CPlayerManager::GetInstance()->SetEnableUserChange(true);
		}
	}
#else
	// �ŗǖڕW���
	struct SBestDest
	{
		CPlayer* pPlayer;	// ���݂̖ڕW�v���C���[
		float fMinValue;	// ���݂̍ŗǒl
	};

	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();	// �v���C���[�}�l�[�W���[
	CInputGamepad* pPad = CInputGamepad::GetInstance();				// �p�b�h���
	const int nPadIdx = player->GetMyPlayerIdx();					// �v���C���[���쌠�C���f�b�N�X

	// �E�X�e�B�b�N�̃g���K�[���͌��m�t���O
	const bool bInput = pPad->GetRStickTrigger(nPadIdx, CInputGamepad::STICK_AXIS::STICK_X)
					 || pPad->GetRStickTrigger(nPadIdx, CInputGamepad::STICK_AXIS::STICK_Y);

	//if (bInput && !pPlayerManager->IsUserChange())
	{ // �E�X�e�B�b�N�����͂���Ă��銎�A�܂��N�����t���[���ł��̑�������Ă��Ȃ��ꍇ

		CListManager<CPlayer> list = pPlayerManager->GetInList(player->GetTeam());	// ����v���C���[���X�g
		std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
		MyLib::Vector3 posThis = player->GetPosition();		// ����v���C���[�ʒu
		float fRotStick = pPad->GetStickRotR(nPadIdx);		// �X�e�B�b�N����
		float fRotStickA = fRotStick;

		// TODO�F�X�e�B�b�N�����̃f�o�b�O�\��
		GET_MANAGER->GetDebugProc()->Print("�X�e�B�b�N�����F%f\n", fRotStickA);

		// ������̍ŗǖڕW���
		SBestDest bestRot;
		bestRot.pPlayer = nullptr;
		bestRot.fMinValue = D3DX_PI * 2.0f;

		// ������̍ŗǖڕW���
		SBestDest bestDis;
		bestDis.pPlayer = nullptr;
		bestDis.fMinValue = CGameManager::GetInstance()->GetHalfCourtDiagonal();

		while (list.ListLoop(itr))
		{ // ���X�g���̗v�f�����J��Ԃ�

			CPlayer* pItrPlayer = (*itr);	// �v���C���[���

			// ���Ƀ��[�U�[�̏ꍇ����
			if (pItrPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER) { continue; }

#if 1

#if 1
			float fRotSpace = pItrPlayer->GetPosition().AngleXZ(posThis);	// ����v���C���[���猩��AI�̌���
#else
			float fRotSpace = posThis.AngleXZ(pItrPlayer->GetPosition());	// ����v���C���[���猩��AI�̌���
#endif
			float fRotSpaceA = fRotSpace + D3DX_PI;
			float fRotDiff = fRotStickA - fRotSpaceA;

			// TODO�F���������̃f�o�b�O�\��
			GET_MANAGER->GetDebugProc()->Print("�`�h�����F%f\n", fRotSpaceA);
			GET_MANAGER->GetDebugProc()->Print("���������F%f\n", fRotDiff);

			// ������̍ŗǖڕW���X�V
			if (bestRot.pPlayer == nullptr || bestRot.fMinValue < fRotDiff)
			{ // ���ŗǒl���������ꍇ

				// ����̍ŗǒl��ۑ�
				bestRot.fMinValue = fRotDiff;

				// �ڕW�v���C���[��ۑ�
				bestRot.pPlayer = pItrPlayer;
			}
#endif

#if 0
			// ������̍ŗǖڕW���X�V
			if (bestDis.pPlayer == nullptr || bestDis.fMinValue < )
			{ // ���ŗǒl���������ꍇ

				// ����̍ŗǒl��ۑ�
				bestDis.fMinValue = ;

				// �ڕW�v���C���[��ۑ�
				bestDis.pPlayer = pItrPlayer;
			}
#endif
		}

		// �ŗǂ̖ڕW�v���C���[�ɑ��쌠��n��
#if 0
		if (pChange != nullptr)
		{ // �ύX�Ώۂ����݂���ꍇ

			// �x�[�X�̓���ւ�
			pPlayerManager->SwapBase(player, pChange);

			// ���[�U�[�ύX����ς݂ɂ���
			pPlayerManager->SetEnableUserChange(true);
		}
#endif
	}
#endif
}
