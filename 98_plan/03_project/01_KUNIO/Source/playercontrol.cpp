//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playercontrol.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "debugproc.h"
#include "keyconfig_gamepad.h"
#include "map_obstacle.h"
#include "collisionLine_Box.h"
#include "waterripple.h"
#include "course.h"
#include "spline.h"
#include "EffekseerObj.h"
#include "deadplayer.h"
#include "suffocation.h"
#include "goalgametext.h"
#include "guide.h"

// �L�[�R���t�B�O
#include "keyconfig_keyboard.h"
#include "keyconfig.h"

namespace
{
	const float MULTIPLIY_DASH = 1.5f;		// �_�b�V���̔{��
	const float LENGTH_AUTOFACE = 200.0f;	// �����Ō�������
	const float LENGTH_COLLISIONRANGE = 500.0f;		// �����蔻�肷��͈͂̒���
	const float RATIO_COLLISIONRANGE = 0.3f;		// �͈͂̒����̍ŏ�����
	const float LENGTH_COLLISIONHEIGHT = 1000.0f;	// �����蔻�肷�鍂�����
	float ADD_HEIGHT = 25.0f;					// �����̉��Z��
	const float MIN_HEIGHT = 100.0f;
	const float HEIGHT_VELOCITY = 10.0f;
	float MAX_SURHEIGHT = 100.0f;
	float SURHEIGHT_VELOCITY = (10.0f);
	int DEFAULT_WATERRIPPLE_INTERVAL = 21;	// ���g��̃C���^�[�o��
	const float HEIGHT_MOVETIMER = (1.0f / 0.5f);	// �����ω��J�ڃ^�C�}�[
	const float COMMAND_HEIGHT = 200.0f;	// ����
	float INTERVAL_BRESSEFFECT = 0.32f;	// ���o���܂ł̊Ԋu
	const float DEFAULT_BRESSSCALE_EFFECT = 90.0f;	// �f�t�H�̑��G�t�F�N�g�X�P�[��
	const float MIN_RATIO_HEIGHT_BRESS = 0.2f;	// ���̍����̍ŏ�����
	const float GOAL_INER = 0.02f;
	const float GOAL_GRAVITY = -0.7f;
	const int GOAL_AIRTIMER = 180;
	const float TIME_GOALWAIT = 2.0f;	// �S�[���Q�[�����̑ҋ@����
}

//==========================================================================
// �u���[�\��(�}���`�^�[�Q�b�g�����_�����O)�p�萔��`
//==========================================================================
namespace MULTITARGET
{
	// OFF��
	const float OFF_ALPHA = (0.0f);		// �ڕW�����x
	const float OFF_MULTI = (1.0f);		// �ڕW�{��
	const float OFF_TIMER = (150.0f);	// �J�ڃ^�C�}�[
}

#define GEKIMUZU (true)

//==========================================================================
// �ʏ�ړ�
//==========================================================================
void CPlayerControlMove::Move(CPlayer* player)
{
	if (CGame::GetInstance()->GetGameManager()->GetType() == CGameManager::SceneType::SCENE_GOAL) return;

	// �C���v�b�g���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	// �_�b�V������
	bool bDash = false;
	if (pInputGamepad->GetPress(CInputGamepad::BUTTON_LB, player->GetMyPlayerIdx()) &&
		pInputGamepad->IsTipStick())
	{// ���X�e�B�b�N���|��Ă�ꍇ
		//bDash = true;
	}
	player->SetEnableDash(bDash);


	// �ړ��ʎ擾
	float fMove = player->GetVelocity();
	if (bDash){
		// �_�b�V���{���|����
		fMove *= MULTIPLIY_DASH;
	}


	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();
	Camerarot.y -= pCamera->GetOriginRotation().y;

	// �ڕW�̌����擾
	float fRotDest = player->GetRotDest();

	// �ړ��ʎ擾
	MyLib::Vector3 move = player->GetMove();

	// ���[�V�������擾
	CMotion* pMotion = player->GetMotion();
	int nMotionType = pMotion->GetType();
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	// ��Ԏ擾
	CPlayer::STATE state = player->GetState();

	// �ړ�����
	int angle = 0;
	m_nIntervalAddRippleCounter = 1;

	// �ړ�����
	CPlayer::ANGLE moveAngle = CPlayer::ANGLE::NONE;

	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		state != CPlayer::STATE::STATE_DEAD &&
		state != CPlayer::STATE::STATE_DEADWAIT &&
		state != CPlayer::STATE::STATE_RETURN &&
		state != CPlayer::STATE::STATE_RESTART)
	{// �ړ��\���[�V�����̎�

		move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * (fMove * 0.5f);
		move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * (fMove * 0.5f);

#if _DEBUG
		if (pInputKeyboard->GetPress(DIK_A))
		{// ���ړ�

			// �ړ����ɂ���
			motionFrag.bMove = true;

			if (pInputKeyboard->GetPress(DIK_W))
			{// ����ړ�

				move.x += sinf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{// �����ړ�

				move.x += sinf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{// ���ړ�

				move.x += sinf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{// �E�ړ�

			// �ړ����ɂ���
			motionFrag.bMove = true;

			if (pInputKeyboard->GetPress(DIK_W))
			{// �E��ړ�

				move.x += sinf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{// �E���ړ�

				move.x += sinf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{// �E�ړ�

				move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
			}

			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
		}
		else if (pInputKeyboard->GetPress(DIK_W))
		{// ��ړ�

			// �ړ����ɂ���
			motionFrag.bMove = true;
			move.x += sinf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
			fRotDest = D3DX_PI * 1.0f + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S))
		{// ���ړ�

			// �ړ����ɂ���
			motionFrag.bMove = true;
			move.x += sinf(D3DX_PI * 1.0f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 1.0f + Camerarot.y) * fMove;
			fRotDest = D3DX_PI * 0.0f + Camerarot.y;
		}
		else
		{
			motionFrag.bMove = false;
		}
#endif

		// �ړ����ɂ���
		motionFrag.bMove = true;

		if (pInputGamepad->IsTipStick())
		{// ���X�e�B�b�N���|��Ă�ꍇ

			// �ړ����ɂ���
			motionFrag.bMove = true;

			// �X�e�B�b�N�̌����擾
			float stickrot = pInputGamepad->GetStickRotL(player->GetMyPlayerIdx());
			UtilFunc::Transformation::RotNormalize(stickrot);

#if 0
			// �ړ��ʂƌ����ݒ�
			move.x += sinf(stickrot + Camerarot.y) * fMove;
			move.z += cosf(stickrot + Camerarot.y) * fMove;
			fRotDest = D3DX_PI + stickrot + Camerarot.y;
#endif
			// �ړ�����
			angle = (stickrot <= 0.0f) ? -1 : 1;
			moveAngle = (static_cast<CPlayer::ANGLE>(angle) == 1) ? CPlayer::ANGLE::RIGHT : CPlayer::ANGLE::LEFT;

			if (angle == -1)
			{
				fMove *= 0.3f;
				m_nIntervalAddRippleCounter = 2;
			}
			else
			{
				fMove *= 0.35f;
			}
			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * (fMove * angle);
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * (fMove * angle);
			fRotDest = angle * (-D3DX_PI * 0.5f) + Camerarot.y;

			// �ړ������ݒ�
			player->SetMoveAngle(moveAngle);
		}

		// �W�����v�󋵎擾
		bool bJump = player->IsJump();

		if (motionFrag.bMove &&
			!bJump)
		{// �L�����Z���\ && �ړ���

			// ���[�V�����I������
			pMotion->ToggleFinish(true);

			// �ړ����[�V����
			if (bDash)
			{
				//pMotion->Set(CPlayer::MOTION::MOTION_DASH);
			}
			else
			{
				pMotion->Set(CPlayer::MOTION::MOTION_WALK);
			}
		}

#if 0
		if (!bJump &&
			(pInputKeyboard->GetTrigger(DIK_SPACE) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx())))
		{// �W�����v

			bJump = true;
			motionFrag.bJump = true;
			move.y += 17.0f;

			pMotion->Set(CPlayer::MOTION::MOTION_JUMP);

			// �T�E���h�Đ�
			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_JUMP);
		}
#endif

		// �W�����v����ݒ�
		player->SetEnableJump(bJump);

	}
	else if (
		pMotion->IsGetMove(nMotionType) == 0 &&	// �ړ��\�ȃ��[�V�������擾
		state != CPlayer::STATE::STATE_DEAD &&
		state != CPlayer::STATE::STATE_RETURN &&
		state != CPlayer::STATE::STATE_RESTART)
	{
		if (pInputKeyboard->GetPress(DIK_A))
		{//���L�[�������ꂽ,���ړ�

			if (pInputKeyboard->GetPress(DIK_W))
			{//A+W,����ړ�
				fRotDest = D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{//A+S,�����ړ�
				fRotDest = D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//A,���ړ�
				fRotDest = D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{//D�L�[�������ꂽ,�E�ړ�

			if (pInputKeyboard->GetPress(DIK_W))
			{//D+W,�E��ړ�
				fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{//D+S,�E���ړ�
				fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//D,�E�ړ�
				fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_W))
		{//W�������ꂽ�A��ړ�
			fRotDest = D3DX_PI * 1.0f + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S))
		{//S�������ꂽ�A���ړ�
			fRotDest = D3DX_PI * 0.0f + Camerarot.y;
		}
	}

	// ���[�V�����t���O�ݒ�
	player->SetMotionFrag(motionFrag);

	// �ړ������ݒ�
	player->SetMoveAngle(moveAngle);


#if _DEBUG
	if (!pInputGamepad->GetPress(CInputGamepad::BUTTON::BUTTON_BACK, 0) &&
		!pInputKeyboard->GetPress(DIK_S))
#endif
	{
		// �ړ��ʐݒ�
		player->SetMove(move);
	}

	/*static float height = 30.0f, velocity = 3.1f, thickness = 10.0f;
	static int life = 45;
	static int block = 80, interval = 20;
	static float blocksize = 2.5f;*/

#if 0
	static float height = 31.5f, velocity = 4.0f, thickness = 16.0f;
	static int life = 53;
	static int block = 64;
	static float blocksize = 4.7f;
#else
	static float height = 72.0f, velocity = 3.9f, thickness = 13.0f;
	static int life = 43;
	static int block = 64;
	static float blocksize = 4.5f;
#endif

#if _DEBUG
	ImGui::DragInt("INTERVAL", &m_nIntervalWaterRipple, 1);
	ImGui::DragInt("BLOCK", &block, 1);
	ImGui::DragFloat("BLOCK SIZE", &blocksize, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("height", &height, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("velocity", &velocity, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("thickness", &thickness, 1.0f, 0.0f, 0.0f, "%.2f");
	ImGui::DragInt("life", &life, 1);
#endif

	bool bCreateRipple = false;	// �g�䐶���t���O

	// ���g��J�E���^�[���Z�Ԋu�X�V
	m_nAddRippleCounter = (m_nAddRippleCounter + 1) % m_nIntervalAddRippleCounter;

	if (m_nAddRippleCounter == 0)
	{// ���g��J�E���^�[���Z�Ԋu�����

		// �g��J�E���^�[���Z
		m_nCntWaterRipple = (m_nCntWaterRipple + 1) % m_nIntervalWaterRipple;

		if (m_nCntWaterRipple == 0)
		{
			bCreateRipple = true;
		}

		// ������
		if (angle == 1)
		{
			m_nCntWaterRipple = (m_nCntWaterRipple + 1) % m_nIntervalWaterRipple;

			if (m_nCntWaterRipple == 0)
			{
				bCreateRipple = true;
			}
		}

		// �g��
		if (bCreateRipple)
		{
			MyLib::Vector3 setpos = player->GetPosition();
			setpos.y -= 5.0f;

			// ��������
			if (moveAngle != CPlayer::ANGLE::LEFT)
			{
				CWaterRipple::Create(block, blocksize, setpos, height, velocity, thickness, life);
			}
			else
			{
				CWaterRipple::Create(block, blocksize, setpos, height, velocity, thickness, 35);
			}

			// �T�E���h�Đ�
			int label = CSound::LABEL::LABEL_SE_PLAYERMOVE01 + UtilFunc::Transformation::Random(0, 7);
			CSound::GetInstance()->PlaySound(static_cast<CSound::LABEL>(label));

			// �C���^�[�o���������_������
			m_nIntervalWaterRipple = DEFAULT_WATERRIPPLE_INTERVAL + UtilFunc::Transformation::Random(-6, 6);
		}
	}
}



//==========================================================================
// �A�N�V�����̃f�X�g���N�^
//==========================================================================
CPlayerControlBaggage::~CPlayerControlBaggage()
{
	if (m_pEffekseerObj != nullptr)
	{
		// SE�X�g�b�v
		CSound::GetInstance()->StopSound(CSound::LABEL::LABEL_SE_BRESS_STREAM);

		// �g���K�[���M
		m_pEffekseerObj->SetTrigger(1);
		m_pEffekseerObj->Uninit();
		m_pEffekseerObj = nullptr;
	}


}

//==========================================================================
// �A�N�V����
//==========================================================================
void CPlayerControlBaggage::Action(CPlayer* player, CBaggage* pBaggage)
{
	if (!EndCheck(pBaggage))
	{
		if (pBaggage->GetState() == CBaggage::STATE::STATE_DEAD)
		{
			// ���g���CUI����
			player->CreateRetryUI();
		}
		return;
	}

	// �S�[����
	{
		CGameManager* pManager = CGame::GetInstance()->GetGameManager();

		if (pManager->GetType() == CGameManager::SceneType::SCENE_GOAL)
		{
			GoalAction(player, pBaggage);
			return;
		}
	}

	// �C���v�b�g���擾
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();
	CKeyConfigManager* pKeyConfigManager = CKeyConfigManager::GetInstance();
	CKeyConfig* pKeyConfigPad = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKeyConfigKeyboard = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INKEY);
	CGameManager* pGameMgr = CGame::GetInstance()->GetGameManager();

	if (pGameMgr->GetType() == CGameManager::SceneType::SCENE_WAIT_AIRPUSH &&
		(pKeyConfigKeyboard->GetTrigger(INGAME::ACTION::ACT_AIR) ||
			pKeyConfigPad->GetTrigger(INGAME::ACTION::ACT_AIR)))
	{// ��C����҂��ŋ�C����

		// ���C���Ɉڍs
		pGameMgr->SetType(CGameManager::SceneType::SCENE_MAIN);
	}

	// ���擾
	MyLib::Vector3 move = player->GetMove();
	MyLib::Vector3 pos = player->GetPosition();
	MyLib::Vector3 posBaggage = pBaggage->GetPosition();
	MyLib::Vector3 posBaggageOrigin = pBaggage->GetOriginPosition();


#if BRESSRANGE
	if (m_pBressRange == nullptr)
	{
		m_pBressRange = CDebugBressRange::Create();
	}
	if (m_pBressHeight == nullptr)
	{
		m_pBressHeight = CDebugBressRange::Create();
		m_pBressHeight->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.3f));
	}
#endif

	static float up = 8.3f, power = 9.0f;
	//static float up = 8.3f, power = 6.8f;
	//static float up = 2.5f, power = 2.0f;
#if _DEBUG
	ImGui::DragFloat("up", &up, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("power", &power, 0.01f, 0.0f, 0.0f, "%.2f");

	ImGui::DragFloat("Add Height", &ADD_HEIGHT, 1.0f, 0.0f, 0.0f, "%.2f");
#endif

	static float starttimeDownheight = 1.5f;	// �~�����n�܂�܂ł̎���
	static float timeDownheight = 2.0f;			// ��������܂ł̎���
	static float ratioMinDownheight = 0.2f;		// �������������̍ĉ��ꊄ��

#if _DEBUG
	ImGui::DragFloat("Start Time DownHeight", &starttimeDownheight, 0.05f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("Time DownHeight", &timeDownheight, 0.05f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("Ratio Min DownHeight", &ratioMinDownheight, 0.01f, 0.0f, 0.0f, "%.2f");	
#endif

	// �ו��̍����Ŋ����ݒ�
	float ratio = (posBaggage.y - posBaggageOrigin.y) / LENGTH_COLLISIONHEIGHT;
	float ratioHeight = 1.0f - ratio;
	ratioHeight = UtilFunc::Transformation::Clamp(ratioHeight, 0.4f, 0.6f);

	// ����
	ratio = UtilFunc::Transformation::Clamp(ratio, 0.3f, 1.0f);

	// ���̓͂����͈�
	float range = ratio * LENGTH_COLLISIONRANGE;

#if BRESSRANGE
	MyLib::Vector3 a = pos, b = pos;
	a.x += range; b.x -= range;
	a.y = posBaggage.y; b.y = posBaggage.y;

	MyLib::Vector3 c = pos;
	c.y = posBaggageOrigin.y + LENGTH_COLLISIONHEIGHT;

	CEffect3D::Create(
		c,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	CEffect3D::Create(
		a,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	CEffect3D::Create(
		b,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	MyLib::Vector3 leftup = posBaggageOrigin, rightup = posBaggageOrigin, leftdw = posBaggageOrigin, rightdw = posBaggageOrigin;
	leftup.y += LENGTH_COLLISIONHEIGHT; leftup.x -= LENGTH_COLLISIONRANGE;
	rightup.y += LENGTH_COLLISIONHEIGHT; rightup.x += LENGTH_COLLISIONRANGE;
	leftdw.x -= LENGTH_COLLISIONRANGE * 0.3f;
	rightdw.x += LENGTH_COLLISIONRANGE * 0.3f;

	// ���͈̔͐���
	if (m_pBressRange != nullptr)
	{
		m_pBressRange->SetRange(leftup, rightup, leftdw, rightdw);
		m_pBressRange->SetPosition(pos);
	}
#endif

#if GEKIMUZU

#if _DEBUG
	if (!pInputGamepad->GetPress(CInputGamepad::BUTTON::BUTTON_BACK, 0) &&
		!CInputKeyboard::GetInstance()->GetPress(DIK_S))
#endif
	{
		// ��������
		if (posBaggage.y <= posBaggageOrigin.y)
		{
			posBaggage.y = posBaggageOrigin.y;
			if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
			{
				MyLib::HitResult_Character hitresult = player->Hit(1);
				m_bLandOld = true;

				// ��������
				if (hitresult.isdeath)
				{
					// ����
					Suffocation(player, pBaggage);
				}
			}
		}
		else if(!pBaggage->IsLand())
		{

			// �O�񂪒��n
			if (m_bLandOld)
			{
				CSound::GetInstance()->StopSound(CSound::LABEL::LABEL_SE_DROWN);
				float multi = 1.0f - static_cast<float>(player->GetLife()) / static_cast<float>(player->GetLifeOrigin());

				// �t�B�[�h�o�b�N�G�t�F�N�gOFF
				CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(
					MULTITARGET::OFF_ALPHA,
					MULTITARGET::OFF_MULTI,
					MULTITARGET::OFF_TIMER * multi);

				// �R���g���[���[�U����~
				pInputGamepad->SetVibMulti(0.0f);
			}

			// �̗͉�
			int setLife = player->GetLife();
			setLife = UtilFunc::Transformation::Clamp(setLife + 1, 0, player->GetLifeOrigin());
			player->SetLife(setLife);

			// �O�񒅒n���Ă��Ȃ���Ԃ�
			m_bLandOld = false;
		}
		else
		{
			// �ʒu�ݒ�
			posBaggage.y = posBaggageOrigin.y;
			if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
			{
				MyLib::HitResult_Character hitresult = player->Hit(1);

				// �O�񒅒n������Ԃ�
				m_bLandOld = true;

				// ��������
				if (hitresult.isdeath)
				{
					// ����
					Suffocation(player, pBaggage);
				}
			}
		}
	}

	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_4))
	{
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_DROWN);
	}

	// �ʒu�ݒ�
	if (pBaggage->GetState() != CBaggage::STATE::STATE_DEAD)
	{
		pBaggage->SetPosition(MyLib::Vector3(pos.x, posBaggage.y, pos.z));
	}

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// �����ݒ�
	MyLib::Vector3 setRot;
	setRot.x += sinf(D3DX_PI * 0.5f + 0.0f) * 1.0f;
	setRot.z += cosf(D3DX_PI * 0.5f + 0.0f) * 1.0f;
	pBaggage->SetRotation(MyLib::Vector3(0.0f, setRot.AngleXZ(0.0f), 0.0f));
#endif

#if BRESSRANGE
	if (m_pBressHeight != nullptr)
	{
		float ratiooo = m_fHeight / LENGTH_COLLISIONHEIGHT;

		float rangeee = ratiooo * LENGTH_COLLISIONRANGE;

		/*leftup.x = -rangeee - (LENGTH_COLLISIONRANGE * 0.3f);
		rightup.x = rangeee + (LENGTH_COLLISIONRANGE * 0.3f);*/
		leftup.y = posBaggageOrigin.y + m_fHeight;
		rightup.y = posBaggageOrigin.y + m_fHeight;
		m_pBressHeight->SetRange(leftup, rightup, leftdw, rightdw);
		m_pBressHeight->SetPosition(pos);
	}
#endif
	//=============================
	// ���G�t�F�N�g
	//=============================
	BressEffect(player, pBaggage);
	bool bKantsu = CollisionObstacle(player, pBaggage);
	if (pKeyConfigKeyboard->GetPress(INGAME::ACTION::ACT_AIR) ||
		pKeyConfigPad->GetPress(INGAME::ACT_AIR))
	{

		// �����̍~�����ԉ��Z
		m_fTimeDownHeight += CManager::GetInstance()->GetDeltaTime();

		if (m_bFall) 
		{// ������

			m_bFall = false;
			pBaggage->SetForce(0.0f);


			if (m_pEffekseerObj != nullptr)
			{
				// SE�X�g�b�v
				CSound::GetInstance()->StopSound(CSound::LABEL::LABEL_SE_BRESS_STREAM);

				// �g���K�[���M
				m_pEffekseerObj->SetTrigger(1);
				m_pEffekseerObj->Uninit();
				m_pEffekseerObj = nullptr;
			}

			// ���G�t�F�N�g����
			MyLib::Vector3 d = pos;
			d.y = posBaggageOrigin.y;

			if (m_pEffekseerObj == nullptr)
			{
				m_pEffekseerObj = CEffekseerObj::Create(
					CMyEffekseer::EFKLABEL::EFKLABEL_BRESS,
					d, 0.0f, 0.0f, DEFAULT_BRESSSCALE_EFFECT, false);
			}

			// SE�Đ�
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_BRESS_STREAM);
		}

		// ���̉��Z�v�Z
		m_fHeightVelocity += (0.0f - m_fHeightVelocity) * 0.2f;
		m_fHeight += ADD_HEIGHT + m_fHeightVelocity;
		m_fHeight = UtilFunc::Transformation::Clamp(m_fHeight, MIN_HEIGHT, LENGTH_COLLISIONHEIGHT);

		// �����̊���
		float ratio = m_fHeight / m_fMaxHeight;

		// SE�̃s�b�`�ύX
		CSound::GetInstance()->SetFrequency(CSound::LABEL::LABEL_SE_BRESS_STREAM, 0.5f + ratio * 1.5f);

		ratio = UtilFunc::Transformation::Clamp(ratio, MIN_RATIO_HEIGHT_BRESS, 1.0f);

		// �U��
		pInputGamepad->SetVibMulti(1.0f * ratio);
		pInputGamepad->SetVibration(CInputGamepad::VIBRATION_STATE::VIBRATION_STATE_AIR, 0);

		if (posBaggage.y <= posBaggageOrigin.y + m_fHeight &&
			posBaggage.x <= pos.x + range &&
			posBaggage.x >= pos.x - range)
		{// �͈͓�

			//if (bKantsu)
			{// ��Q���̋�C�ђʔ���

#if GEKIMUZU
				// �ו��֋�C�ړ��ʉ��Z
				pBaggage->AddForce(MyLib::Vector3(0.0f, up * ratioHeight, 0.0f), player->GetPosition() + move);
#else
				pBaggage->SetMove(MyLib::Vector3(move.x, pBaggage->GetMove().y, move.z));
				pBaggage->AddForce(MyLib::Vector3(ratio * power, up * ratioHeight, 0.0f), player->GetPosition() + move);
#endif
			}
		}
	}
	else
	{
		// �~�����
		m_bFall = true;

		// �����̍~�����ԉ��Z
		m_fTimeDownHeight -= CManager::GetInstance()->GetDeltaTime() * 2.0f;

		m_fHeight -= ADD_HEIGHT * 2.0f;
		m_fHeightVelocity += (m_fHeightVelocity - HEIGHT_VELOCITY) * 0.1f;
		m_fHeightVelocity = UtilFunc::Transformation::Clamp(m_fHeightVelocity, 0.0f, HEIGHT_VELOCITY);
	}

	// �����̍~�����ԕ␳
	m_fTimeDownHeight = UtilFunc::Transformation::Clamp(m_fTimeDownHeight, 0.0f, starttimeDownheight + timeDownheight);

	// ���̍����␳
	m_fHeight = UtilFunc::Transformation::Clamp(m_fHeight, MIN_HEIGHT, LENGTH_COLLISIONHEIGHT);

	// ���̓͂��ő�̍������~�����Ă���
	if (m_fTimeDownHeight >= starttimeDownheight)
	{
		float timeratio = (m_fTimeDownHeight - starttimeDownheight) / timeDownheight;
		timeratio = UtilFunc::Transformation::Clamp(timeratio, 0.0f, 1.0f);


		float height = (1.0f - timeratio) * LENGTH_COLLISIONHEIGHT;
		height = UtilFunc::Transformation::Clamp(height, LENGTH_COLLISIONHEIGHT * ratioMinDownheight, LENGTH_COLLISIONHEIGHT);

		if (m_bFall && m_fHeight <= height)
		{
			m_fHeight = height;
		}
		else if (!m_bFall)
		{
			m_fHeight = height;
		}
	}

	// ������ԍX�V
	if (posBaggage.y <= MIN_HEIGHT)
	{
		m_bFall = true;
	}

	// ���G�t�F�N�g��ԕύX
	if (m_bFall && m_pEffekseerObj != nullptr)
	{
		// ����������
		m_pEffekseerObj->SetTrigger(0);
		m_pEffekseerObj->DeleteLater(1.0f);
		m_pEffekseerObj = nullptr;

		// SE�X�g�b�v
		CSound::GetInstance()->StopSound(CSound::LABEL::LABEL_SE_BRESS_STREAM);
	}


	MyLib::Vector3 d = pos;
	d.y = posBaggageOrigin.y;

	// �ʒu�ݒ�
	if (m_pEffekseerObj != nullptr)
	{
		// �����̊���
		float ratio = m_fHeight / m_fMaxHeight;
		ratio = UtilFunc::Transformation::Clamp(ratio, MIN_RATIO_HEIGHT_BRESS, 1.0f);

		m_pEffekseerObj->SetPosition(d);
		m_pEffekseerObj->SetScale(DEFAULT_BRESSSCALE_EFFECT * ratio);

	}
}

//==========================================================================
// �S�[�����̐ݒ�
//==========================================================================
void CPlayerControlBaggage::GoalSetting()
{
	// ���G�t�F�N�g��ԕύX
	if (m_pEffekseerObj != nullptr)
	{
		// ����������
		m_pEffekseerObj->SetTrigger(0);
		m_pEffekseerObj->DeleteLater(1.0f);
		m_pEffekseerObj = nullptr;

		// SE�X�g�b�v
		CSound::GetInstance()->StopSound(CSound::LABEL::LABEL_SE_BRESS_STREAM);
	}
}

//==========================================================================
// ����
//==========================================================================
void CPlayerControlBaggage::Suffocation(CPlayer* player, CBaggage* pBaggage)
{
	// ���S�̐���
	CDeadPlayer::Create(player->GetPosition());

	// �������
	pBaggage->SetState(CBaggage::STATE::STATE_FALL);

	// ����UI����
	if (m_pSuffocation == nullptr)
	{
		m_pSuffocation = CSuffocation::Create();
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_RAOU);
	}
}

//==========================================================================
// �S�[���A�N�V����
//==========================================================================
void CPlayerControlBaggage::GoalAction(CPlayer* player, CBaggage* pBaggage)
{
	if (pBaggage->GetState() == CBaggage::STATE::STATE_RETURN)
	{
		return;
	}

	// �t�B�[�h�o�b�N�G�t�F�N�g���Z�b�g
	CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(
		0.0f,
		1.0f,
		0.01f);


	// �C���v�b�g���擾
	CKeyConfigManager* pKeyConfigManager = CKeyConfigManager::GetInstance();
	CKeyConfig* pKeyConfigPad = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKeyConfigKeyBoard = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INKEY);
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	CCameraMotion* pCamMotion = pCamera->GetMotion();

	if (m_state != STATE::STATE_RELEASE)
	{// �������ĂȂ��Ƃ�
		pBaggage->SetState(CBaggage::STATE::STATE_GOAL);
	}

	static float up = 0.175f, power = 0.4f;

	//=============================
	// ���G�t�F�N�g
	//=============================
	BressEffect(player, pBaggage);
	if ((m_state == STATE::STATE_WAIT || m_state == STATE::STATE_PRESS) && (pKeyConfigKeyBoard->GetPress(INGAME::ACT_AIR) ||
		pKeyConfigPad->GetPress(INGAME::ACT_AIR)))
	{
		// �ҋ@��Ԏ��A�J��
		if (m_state == STATE::STATE_WAIT)
		{
			pCamMotion->SetMotion(CCameraMotion::MOTION::MOTION_GOALBAG, CCameraMotion::EASING::Linear);

			// �K�C�h�폜
			if (m_pGuide != nullptr)
			{
				m_pGuide->Kill();
				m_pGuide = nullptr;
			}
		}

		m_state = STATE::STATE_PRESS;

		// ������������Z
		m_nGoalTimer--;
		if (m_nGoalTimer <= 0)
		{
			m_state = STATE::STATE_RELEASE;
		}
	}
	// ���͂���Ă��Ȃ�
	else if(
		!pKeyConfigKeyBoard->GetPress(INGAME::ACT_AIR) &&
		!pKeyConfigPad->GetPress(INGAME::ACT_AIR))
	{
		// ��Ԃ��Ƃɐݒ�
		switch (m_state)
		{
		case STATE::STATE_NONE:

			// �ו��������ăJ�������[�V�������I���
			if (pBaggage->GetMove().y == 0.0f && pCamMotion->IsPause())
			{
				m_state = STATE::STATE_WAIT;

				// �S�[���Q�[���e�L�X�g����
				CGoalGameText::Create();

				// �ړI�n�Љ��Ȃ炱��
			}
			m_nGoalTimer = GOAL_AIRTIMER;
			break;

		case STATE::STATE_WAIT:
			m_nGoalTimer = GOAL_AIRTIMER;

			// �S�[���̑ҋ@���ԉ��Z
			m_fGoalWaitTimer += CManager::GetInstance()->GetDeltaTime();
			if (TIME_GOALWAIT <= m_fGoalWaitTimer &&
				m_pGuide == nullptr)
			{// ���Ԍo��

				// ����K�C�h����
				m_pGuide = CGuide::Create(CGuide::Type::GOAL);
			}
			break;

		case STATE::STATE_PRESS:
			m_state = STATE::STATE_RELEASE;
			break;

		case STATE::STATE_RELEASE:
			break;

		default:
			m_state = STATE::STATE_NONE;
			break;
		}		
	}

	MyLib::Vector3 move = pBaggage->GetMove();
	MyLib::Vector3 pos = pBaggage->GetPosition();

	// ��Ԃ��Ƃɐݒ�
	switch (m_state)
	{
	case STATE::STATE_NONE:
	{
		move.y += GOAL_GRAVITY;
	}
		break;

	case STATE::STATE_WAIT:
	{
		move.y += GOAL_GRAVITY;
	}
		break;

	case STATE::STATE_PRESS:
	{
		// �ו��֋�C�ړ��ʉ��Z
		move.x += power;
		move.y += up;

		// ��������
		if (pBaggage->GetPosition().y <= pBaggage->GetOriginPosition().y)
		{
			MyLib::Vector3 move = pBaggage->GetMove();
			MyLib::Vector3 pos = pBaggage->GetPosition();
			pos.y = pBaggage->GetOriginPosition().y;
		}
	}
		break;

	case STATE::STATE_RELEASE:
	{
		if (pBaggage->GetState() != CBaggage::STATE::STATE_FALL)
		{
			move.x += -move.x * GOAL_INER;
			move.z += -move.z * GOAL_INER;
			move.y += GOAL_GRAVITY;
		}

		// ��������
		if (pBaggage->GetPosition().y <= pBaggage->GetOriginPosition().y)
		{
			MyLib::Vector3 move = pBaggage->GetMove();
			MyLib::Vector3 pos = pBaggage->GetPosition();
			pos.y = pBaggage->GetOriginPosition().y;
		}

		if (pBaggage->GetState() != CBaggage::STATE::STATE_FALL)
		{
			pBaggage->SetState(CBaggage::STATE::STATE_SEND);
		}
	}
		break;

	default:
		break;
	}

	// �l�ύX
	if (pBaggage->GetState() != CBaggage::STATE::STATE_FALL)
	{
		pos += move;
	}

	if (m_state != STATE::STATE_RELEASE &&
		pos.y <= pBaggage->GetOriginPosition().y)
	{
		pos.y = pBaggage->GetOriginPosition().y;
		move.y = 0.0f;

	}

	// ���^�[���ȊO�͍X�V
	if (pBaggage->GetState() != CBaggage::STATE::STATE_RETURN)
	{
		if (pBaggage->GetState() != CBaggage::STATE::STATE_FALL)
		{
			pBaggage->SetMove(move);
			pBaggage->SetPosition(pos);
			pCamMotion->SetPosition(pos);
		}
	}
}

//==========================================================================
// ���G�t�F�N�g
//==========================================================================
void CPlayerControlBaggage::BressEffect(CPlayer* player, CBaggage* pBaggage)
{
	// �����ԉ��Z
	m_fBressTimer += CManager::GetInstance()->GetDeltaTime();

	static int createIdx = 4;
	static int Life = 50;
	static int Randrange = 9;
	static int XZrange = 492;
	static int Yrange = 487;
	static float defradius = 61.80f;
	static float gravity = 1.7f;
	static float movefactor = 0.4f;

	// �J���[�G�f�B�b�g
	static ImVec4 myColor = ImVec4(1.0f, 1.0, 1.0, 0.08235f); // RGBA

#if _DEBUG
	if (ImGui::TreeNode("Bress"))
	{
		ImGui::DragInt("CreateNum", &createIdx, 1);
		ImGui::DragFloat("Interval", &INTERVAL_BRESSEFFECT, 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::DragInt("Interval_Randrange", &Randrange, 1);
		ImGui::DragInt("Life", &Life, 1);
		ImGui::DragInt("XZrange", &XZrange, 1);
		ImGui::DragInt("Yrange", &Yrange, 1);
		ImGui::DragFloat("defradius", &defradius, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("gravity", &gravity, 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("Move factor", &movefactor, 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::ColorEdit4("Color", &myColor.x);

		ImGui::TreePop();
	}
#endif

	if (m_fIntervalBress > m_fBressTimer)
	{
		return;
	}

	m_fBressTimer = 0.0f;
	m_fIntervalBress = INTERVAL_BRESSEFFECT + UtilFunc::Transformation::Random(-Randrange, Randrange) * 0.01f;


	// ���擾
	MyLib::Vector3 pos = player->GetPosition();
	MyLib::Vector3 rot = player->GetRotation();
	MyLib::Vector3 posBaggage = pBaggage->GetPosition();
	MyLib::Vector3 posBaggageOrigin = pBaggage->GetOriginPosition();

	// ���̔����n�_
	MyLib::Vector3 bresspos = pos;
	bresspos.y = posBaggageOrigin.y;

	for (int i = 0; i < createIdx; i++)
	{
		MyLib::Vector3 move;
		float randmoveXZ = UtilFunc::Transformation::Random(XZrange, XZrange + 100) * 0.1f;
		float randmoveY = UtilFunc::Transformation::Random(Yrange, Yrange + 20) * 0.1f;
		float randRadius = UtilFunc::Transformation::Random(-10, 10);



		// �ړ��ʌv�Z
		float moveLen = player->GetMoveLength();
		moveLen += move.x;

		// ������̈ʒu�Ƃ̌����ŎZ�o
		MyLib::Vector3 newPosition = MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), moveLen);
		float angle = pos.AngleXZ(newPosition);

		if (i % 2 == 0)
		{
			angle += D3DX_PI;
		}

		move.x = sinf(D3DX_PI * 0.5f + angle) * randmoveXZ;
		move.y = randmoveY;
		move.z = cosf(D3DX_PI * 0.5f + angle) * randmoveXZ;

		// ���a
		float radius = defradius + randRadius;


		CEffect3D* pEffect = CEffect3D::Create(
			bresspos,
			move,
			D3DXCOLOR(myColor.x, myColor.y, myColor.z, myColor.w),
			radius, Life,
			CEffect3D::MOVEEFFECT::MOVEEFFECT_GENSUI,
			CEffect3D::TYPE::TYPE_SMOKE);
		pEffect->SetEnableGravity();
		pEffect->SetGravityValue(-gravity);
		pEffect->SetMoveFactor(movefactor);
	}
}

//==========================================================================
// �ו��̏I������
//==========================================================================
bool CPlayerControlBaggage::EndCheck(CBaggage* pBaggage)
{
	if (pBaggage == nullptr) { return false; }

	if (pBaggage->IsEnd()) { return false; }

	return true;
}

//==========================================================================
// ������Ԃɖ߂�����
//==========================================================================
void CPlayerControlBaggage::Reset(CPlayer* player, CBaggage* pBaggage)
{
	MyLib::Vector3 pos = player->GetPosition();
	MyLib::Vector3 posBaggageOrigin = pBaggage->GetOriginPosition();
	pBaggage->SetPosition(MyLib::Vector3(pos.x, posBaggageOrigin.y, pos.z));
	m_state = STATE::STATE_NONE;

	if (m_pSuffocation != nullptr)
	{
		m_pSuffocation->Uninit();
		m_pSuffocation = nullptr;
	}
}

//==========================================================================
// �G�t�F�N�g��~����
//==========================================================================
void CPlayerControlBaggage::EffectStop()
{
	// �����g�p����Ă���
	if (m_pEffekseerObj != nullptr)
	{
		// ������肯��
		m_pEffekseerObj->SetTrigger(0);
		m_pEffekseerObj->DeleteLater(1.0f);
		m_pEffekseerObj = nullptr;

		// SE�X�g�b�v
		CSound::GetInstance()->StopSound(CSound::LABEL::LABEL_SE_BRESS_STREAM);

		m_fTimeDownHeight = 0.0f;
		m_bFall = true;
	}
}

//==========================================================================
// ��Q������
//==========================================================================
bool CPlayerControlBaggage::CollisionObstacle(CPlayer* player, CBaggage* pBaggage)
{
	// ��Q���̃��X�g�擾
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// �擪��ۑ�
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	// ���擾
	MyLib::Vector3 move = player->GetMove();
	MyLib::Vector3 pos = player->GetPosition();
	MyLib::Vector3 posBaggage = pBaggage->GetPosition();
	MyLib::Vector3 posBaggageOrigin = pBaggage->GetOriginPosition();

	float range = LENGTH_COLLISIONRANGE * RATIO_COLLISIONRANGE;
	MyLib::AABB bressAABB = MyLib::AABB(MyLib::Vector3(-range, 0.0f, -range), MyLib::Vector3(range, LENGTH_COLLISIONHEIGHT, range));

	// �ʒu�ݒ�
	MyLib::Matrix mtx;
	MyLib::Vector3 trans = posBaggage;
	trans.y = posBaggageOrigin.y;

	mtx.Translation(trans);

#if BRESSRANGE
	// �{�b�N�X����
	if (m_pBox == nullptr)
	{
		m_pBox = CCollisionLine_Box::Create(bressAABB, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	}

	if (m_pBox != nullptr)
	{
		m_pBox->SetPosition(trans);
	}
#endif

	while (list.ListLoop(itr))
	{
		CMap_Obstacle* pObj = *itr;

		// ���擾
		CMap_ObstacleManager::SObstacleInfo info = pObj->GetObstacleInfo();

		if (info.setup.isAir == 0) continue;	// ��C�ђʔ���

		MyLib::Vector3 ObjPos = pObj->GetPosition();

		if (posBaggage.y <= ObjPos.y) continue;	// ��Q���Ɖו��̍�������

		for (const auto& collider : info.boxcolliders)
		{
			if (UtilFunc::Collision::IsAABBCollidingWithBox(bressAABB, mtx, MyLib::AABB(collider.vtxMin, collider.vtxMax), collider.worldmtx))
			{
#if BRESSRANGE
				m_pBressRange->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.3f));
#endif
				return false;
			}
		}
	}

#if BRESSRANGE
	m_pBressRange->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
#endif
	return true;
}


//==========================================================================
// ���㑀��
//==========================================================================
float CPlayerControlSurfacing::Surfacing(CPlayer* player)
{
	// �C���v�b�g���擾
	CKeyConfigManager* pKeyConfigManager = CKeyConfigManager::GetInstance();
	CKeyConfig* pKeyConfigPad = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKeyConfigKeyboard = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INKEY);

	// ���㔻��
	bool bUp = false;

	// ���[�V�������擾
	CMotion* pMotion = player->GetMotion();

	//if (pMotion->) {	// ����\�ł͂Ȃ�
	//	return m_fHeight;
	//}

	if (pKeyConfigKeyboard->GetPress(INGAME::ACTION::ACT_UPDOWN) ||
		pKeyConfigPad->GetPress(INGAME::ACTION::ACT_UPDOWN))
	{// ���͂��Ă���
		bUp = true;
	}

#if _DEBUG
	ImGui::DragFloat("SurHeight", &MAX_SURHEIGHT, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("SurSpeed", &SURHEIGHT_VELOCITY, 0.1f, 0.0f, 0.0f, "%.2f");
#endif


	if (bUp) {	// �㏸
		m_fHeight += SURHEIGHT_VELOCITY;

		if (m_fHeight > MAX_SURHEIGHT) {
			m_fHeight = MAX_SURHEIGHT;
		}
	}
	else {	// ���~
		m_fHeight -= SURHEIGHT_VELOCITY;

		if (m_fHeight < 0.0f) {
			m_fHeight = 0.0f;
		}
	}

	return m_fHeight;
}

//==========================================================================
// �g���b�N
//==========================================================================
void CPlayerControlTrick::Trick(CPlayer* player, int& nIdx, bool& bValue)
{
	int type = -1;
	m_pCommandPad->GetSuccess(nIdx, type);
	if (nIdx >= 0) bValue = true;

	CBaggage* bag = player->GetBaggage();

	// ��ނɂ���Č��ʂ�^����
	switch (type)
	{
	case CCommand::TYPE::TYPE_ROTATION:
	{
		// �ו���������]������
		MyLib::Vector3 rot = bag->GetVeloRot();
		rot.x += 1.0f;
		bag->SetVeloRot(rot);
	}
		break;

	case CCommand::TYPE::TYPE_UP:
	{
		// �ו����㏸������
		MyLib::Vector3 move = bag->GetMove();
		move.y += 5.0f;
		bag->SetMove(move);
	}
		break;
	case CCommand::TYPE::TYPE_FLY:
	{
		if (m_fHeightTimer < 1.0f)
		{
			m_fHeightTimer += HEIGHT_MOVETIMER * CManager::GetInstance()->GetDeltaTime();

			if (m_fHeightTimer >= 1.0f)
			{
				m_fHeightTimer = 1.0f;
			}
		}
	}
		break;
	default:
		if (m_fHeightTimer > 0.0f)
		{
			m_fHeightTimer -= HEIGHT_MOVETIMER * CManager::GetInstance()->GetDeltaTime();

			if (m_fHeightTimer <= 0.0f)
			{
				m_fHeightTimer = 0.0f;
			}
		}
		break;
	}

	// ������ݒ肷��
	if(m_fHeightTimer != 0.0f)
	{
		m_fHeight = UtilFunc::Correction::EasingLinear(0.0f, COMMAND_HEIGHT, m_fHeightTimer);
	}
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerControlTrick::Uninit()
{
	if (m_pCommandPad == nullptr) return;
	m_pCommandPad->Uninit();
	delete m_pCommandPad;
	m_pCommandPad = nullptr;
}
