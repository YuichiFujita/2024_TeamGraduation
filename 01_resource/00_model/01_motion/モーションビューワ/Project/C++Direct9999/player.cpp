//=============================================================================
// 
//  �v���C���[���� [player.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "player.h"
#include "camera.h"
#include "manager.h"
#include "debugproc.h"
#include "renderer.h"
#include "input.h"
#include "calculation.h"
#include "texture.h"
#include "Xload.h"
#include "model.h"
#include "motion.h"
#include "objectChara.h"
#include "shadow.h"
#include "particle.h"
#include "3D_Effect.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define JUMP			(20.0f * 1.5f)	// �W�����v�͏����l
#define MAX_LIFE		(5)				// �̗�
#define OFFSET_MOVE		(1.0f)
#define SETUP_MOVE		(0.5f)
#define SCALE_MOVE		(0.01f)
#define SPHERE_LIFE		(6)

namespace
{
	const float MIN_ROT = -D3DX_PI * 10.0f;
	const float MAX_ROT = D3DX_PI * 10.0f;
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayer::CPlayer(int nPriority) : CObjectChara(nPriority)
{
	// �l�̃N���A
	m_state = STATE_NONE;	// ���
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �}�e���A���̐F
	m_bJump = false;		// �W�����v�����ǂ���
	m_bMove = false;		// �ړ������ǂ���
	m_bATK = false;			// �U�������ǂ���
	m_bMWait = false;		// ���@�ҋ@�����ǂ���
	m_nCntState = 0;		// ��ԑJ�ڃJ�E���^�[
	m_nLife = 0;			// �̗�
	m_nTexIdx = 0;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nIdxXFile = 0;		// X�t�@�C���̃C���f�b�N�X�ԍ�
	m_pMotion = NULL;		// ���[�V�����̏��

	m_nNowParts = 0;		// ���݂̃p�[�c
	m_nNowMotion = 0;		// ���݂̃��[�V����
	m_nNowKey = 0;			// ���݂̃L�[
	m_nNowAttackIdx = 0;	// ���݂̍U���C���f�b�N�X�ԍ�
	m_fSetUpBuff = 0.0f;	// �Z�b�g�A�b�v�o�t
	m_nNowColliderIdx = 0;	// ���݂̃R���C�_�[�C���f�b�N�X�ԍ�
	memset(&m_CopyData, 0, sizeof(CMotion::Key));	// �R�s�[�p�p�[�c�f�[�^
	m_bStop = false;		// �X�g�b�v���邩

#if ACTION_MODE
	m_Shotmode = SHOTMODE_TRIGGER;	// �ˌ��̎��

#else
	m_Shotmode = SHOTMODE_NONE;	// �ˌ��̎��
#endif
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayer::~CPlayer()
{

}

//==========================================================================
// ��������
//==========================================================================
CPlayer *CPlayer::Create(const std::string pTextFile)
{
	// �����p�̃I�u�W�F�N�g
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{// NULL��������

		// �������̊m��
		pPlayer = DEBUG_NEW CPlayer;

		if (pPlayer->GetID() < 0)
		{// �������m�ۂɎ��s���Ă�����

			delete pPlayer;
			return NULL;
		}

		if (pPlayer != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pPlayer->Init(pTextFile);
		}

		return pPlayer;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayer::Init(void)
{
	return E_FAIL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayer::Init(const std::string pTextFile)
{
	// ��ނ̐ݒ�
	SetType(TYPE_PLAYER);

	m_state = STATE_NONE;	// ���
	m_nCntState = 0;		// ��ԑJ�ڃJ�E���^�[
	m_nLife = MAX_LIFE;		// �̗�
	m_fSetUpBuff = 1.0f;	// �Z�b�g�A�b�v�o�t

	// �L�����쐬
	HRESULT hr = SetCharacter(pTextFile);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// ���[�V�����̐�������
	m_pMotion = CMotion::Create();

	// ���[�V�����̃t�@�C���ǂݍ���
	m_pMotion->ReadText(pTextFile.c_str());

	// �I�u�W�F�N�g�L�����N�^�[�̏��擾
	CObjectChara *pObjChar = GetObjectChara();

	// ���[�V�����̐ݒ�
	m_pMotion->SetModel(pObjChar->GetModel(), pObjChar->GetNumModel(), CObjectChara::GetObjectChara());

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �e�̐���
	m_pShadow = CShadow::Create(pos, 50.0f);

	// ���[�V�����̏��擾
	CMotion::Info aInfo = m_pMotion->GetInfo(m_nNowMotion);

	// �p�[�c���擾
	int nPartsNum = GetObjectChara()->GetNumModel();

	// ���㏑��
	aInfo = m_pMotion->GetInfo(m_nNowMotion);

	for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
	{
		// ����̃��f���擾
		CModel *pModel = GetObjectChara()->GetModel()[nCntParts];
		if (pModel == NULL)
		{
			continue;
		}

		// �ۑ�����Ă�������ɐݒ�
		pModel->SetRotation(aInfo.aKey[m_nNowKey].aParts[nCntParts].rot);
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayer::Uninit(void)
{
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	// �e������
	if (m_pShadow != NULL)
	{
		//m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	// �I������
	CObjectChara::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayer::Update(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	if (pInputKeyboard->GetTrigger(DIK_F5) == true)
	{// F5�Ń��Z�b�g
		SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		CManager::GetCamera()->SetRotation(D3DXVECTOR3(0.0f, 0.0f, -0.2f));	// ����
	}

	CObjectChara::Update();

	// �e�L�X�g
	ImGui::Text("Change Edit Mode");
	if (ImGui::Button("Edit"))
	{
		CManager::SetEditType(CManager::EDITTYPE_EDIT);
	}
	ImGui::SameLine();
	if (ImGui::Button("Play"))
	{
		CManager::SetEditType(CManager::EDITTYPE_PLAY);
	}
	ImGui::SameLine();
	if (ImGui::Button("SetUp"))
	{
		CManager::SetEditType(CManager::EDITTYPE_SETUP);
		// ���Z�b�g
		for (int nCntParts = 0; nCntParts < GetNumModel(); nCntParts++)
		{
			// ����̃��f���擾
			CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
			if (pModel == nullptr)
			{
				continue;
			}
			pModel->SetRotation(mylib_const::DEFAULT_VECTOR3);
			pModel->SetPosition(pModel->GetOriginPotision());
			pModel->SetScale({ 1.0f, 1.0f, 1.0f });
		}
	}
	ImGui::Separator();

#if 0
	if (ImGui::BeginMenu("Change Edit Type"))
	{
		const char* names[] = { "Edit", "Play", "SetUp" };
		int selectedIndex = 0;

		for (int i = 0; i < IM_ARRAYSIZE(names); i++)
		{
			if (ImGui::MenuItem(names[i]))
			{
				selectedIndex = i;
				CManager::SetEditType(static_cast<CManager::EDITTYPE>(selectedIndex));
			}
		}
		ImGui::EndMenu();
	}
#endif

	if (CManager::GetEditType() != CManager::EDITTYPE_PLAY)
	{// �����ԂȂ�
		return;
	}


	if (pInputKeyboard->GetTrigger(DIK_P) == true)
	{// F3�Ń��C���[�t���[���؂�ւ�
		m_bStop = m_bStop ? false : true;
	}

	SetOldPosition(GetPosition());

	// ����
	Controll();

	// �U�����ݒ�
	SetAttackInfo();

	// ���[�V�����̐ݒ菈��
	MotionSet();

	// ���[�V�����̍X�V����
	if (m_bStop == false)
	{
		m_pMotion->Update();
	}
	else
	{

		// ���[�V�����̏��擾
		CMotion::Info aInfo = m_pMotion->GetInfo(m_nNowMotion);

		// �U�����̑����擾
		int nNumAttackInfo = aInfo.nNumAttackInfo;

		for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
		{
			if (aInfo.AttackInfo[nCntAttack] == NULL)
			{// NULL��������
				continue;
			}

			// ���f�����擾
			CModel **pModel = GetObjectChara()->GetModel();
			D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

			// ����̃}�g���b�N�X�擾
			D3DXMATRIX mtxWepon;
			D3DXMatrixIdentity(&mtxWepon);

			if (pModel[aInfo.AttackInfo[nCntAttack]->nCollisionNum] == NULL)
			{// NULL��������
				return;
			}

			// ���肷��p�[�c�̃}�g���b�N�X�擾
			mtxWepon = pModel[aInfo.AttackInfo[nCntAttack]->nCollisionNum]->GetWorldMtx();

			// �ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, aInfo.AttackInfo[nCntAttack]->Offset.x, aInfo.AttackInfo[nCntAttack]->Offset.y, aInfo.AttackInfo[nCntAttack]->Offset.z);
			D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

			// ����̈ʒu
			D3DXVECTOR3 weponpos = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

			D3DXCOLOR col;
			if (nCntAttack == m_nNowAttackIdx)
			{
				col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			}
			else
			{
				col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
			CEffect3D::Create(weponpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, aInfo.AttackInfo[nCntAttack]->fRangeSize, SPHERE_LIFE, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

		}
	}

	// �e�̈ʒu�X�V
	if (m_pShadow != NULL)
	{
		m_pShadow->SetPosition(GetPosition());
	}

	// ���_���ݒ�
	SetVtx();

	// �U������
	Atack();

	// ��ԍX�V
	UpdateState();

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

}

//==========================================================================
// ���쏈��
//==========================================================================
void CPlayer::Controll(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CManager::GetInputGamepad();

	// �J�����̏��擾
	CCamera *pCamera = CManager::GetCamera();

	// �J�����̌����擾
	D3DXVECTOR3 Camerarot = pCamera->GetRotation();

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// �ڕW�̌����擾
	float fRotDest = GetRotDest();

	// ���݂̎�ގ擾
	int nType = m_pMotion->GetType();

	if (m_pMotion->IsGetMove(nType) == 1)
	{// �ړ��\���[�V�����̎�

		// �ړ��ʎ擾
		float fMove = GetVelocity();

		if (nType == MOTION_SPEED_RUSH_MOVE)
		{// �ړ����b�V����
			fMove *= 0.6f;
		}

		if (pInputKeyboard->GetPress(DIK_A) == true || pInputGamepad->GetStickMoveL(0).x < 0)
		{//���L�[�������ꂽ,���ړ�

			// �ړ����ɂ���
			m_bMove = true;

			if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
			{//A+W,����ړ�

				move.x += sinf(-D3DX_PI * 0.25f + rot.z + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.25f + rot.z + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.75f + rot.z + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
			{//A+S,�����ړ�

				move.x += sinf(-D3DX_PI * 0.75f + rot.z + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.75f + rot.z + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.25f + rot.z + Camerarot.y;
			}
			else
			{//A,���ړ�

				move.x += sinf(-D3DX_PI * 0.5f + rot.z + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.5f + rot.z + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.5f + rot.z + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true || pInputGamepad->GetStickMoveL(0).x > 0)
		{//D�L�[�������ꂽ,�E�ړ�

			// �ړ����ɂ���
			m_bMove = true;

			if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
			{//D+W,�E��ړ�

				move.x += sinf(D3DX_PI * 0.25f + rot.z + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.25f + rot.z + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.75f + rot.z + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
			{//D+S,�E���ړ�

				move.x += sinf(D3DX_PI * 0.75f + rot.z + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.75f + rot.z + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.25f + rot.z + Camerarot.y;
			}
			else
			{//D,�E�ړ�

				move.x += sinf(D3DX_PI * 0.5f + rot.z + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.5f + rot.z + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.5f + rot.z + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
		{//W�������ꂽ�A��ړ�

			// �ړ����ɂ���
			m_bMove = true;

			move.x += sinf(D3DX_PI * 0.0f + rot.z + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.0f + rot.z + Camerarot.y) * fMove;
			fRotDest = D3DX_PI * 1.0f + rot.z + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
		{//S�������ꂽ�A���ړ�

			// �ړ����ɂ���
			m_bMove = true;

			move.x += sinf(D3DX_PI * 1.0f + rot.z + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 1.0f + rot.z + Camerarot.y) * fMove;
			fRotDest = D3DX_PI * 0.0f + rot.z + Camerarot.y;
		}
		else
		{
			// �ړ������ǂ���
			m_bMove = false;
		}

		if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
		{//SPACE�������ꂽ,�W�����v

			if (m_bJump == false)
			{//�W�����v������Ȃ��Ƃ�

				m_bJump = true;
				move.y += JUMP;
			}
		}
	}
	else
	{
		if (pInputKeyboard->GetPress(DIK_A) == true || pInputGamepad->GetStickMoveL(0).x < 0)
		{//���L�[�������ꂽ,���ړ�

			if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
			{//A+W,����ړ�
				fRotDest = D3DX_PI * 0.75f + rot.z + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
			{//A+S,�����ړ�
				fRotDest = D3DX_PI * 0.25f + rot.z + Camerarot.y;
			}
			else
			{//A,���ړ�
				fRotDest = D3DX_PI * 0.5f + rot.z + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true || pInputGamepad->GetStickMoveL(0).x > 0)
		{//D�L�[�������ꂽ,�E�ړ�

			if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
			{//D+W,�E��ړ�
				fRotDest = -D3DX_PI * 0.75f + rot.z + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
			{//D+S,�E���ړ�
				fRotDest = -D3DX_PI * 0.25f + rot.z + Camerarot.y;
			}
			else
			{//D,�E�ړ�
				fRotDest = -D3DX_PI * 0.5f + rot.z + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
		{//W�������ꂽ�A��ړ�
			fRotDest = D3DX_PI * 1.0f + rot.z + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
		{//S�������ꂽ�A���ړ�
			fRotDest = D3DX_PI * 0.0f + rot.z + Camerarot.y;
		}
	}



	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true && m_bATK == false)
	{// �U��

		// �U������ON
		m_bATK = true;
	}

	//���݂ƖڕW�̍��������߂�
	float fRotDiff = fRotDest - rot.y;

	//�p�x�̐��K��
	RotNormalize(fRotDiff);

	//�p�x�̕␳������
	rot.y += fRotDiff * 0.1f;

	//�p�x�̐��K��
	RotNormalize(rot.y);

	// �d�͏���
	move.y -= mylib_const::GRAVITY;

	// �ʒu�X�V
	pos += move;

	// �����␳
	move.x += (0.0f - move.x) * 0.25f;
	move.z += (0.0f - move.z) * 0.25f;

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����ݒ�
	SetRotation(rot);

	// �ڕW�̌����ݒ�
	SetRotDest(fRotDest);

	//**********************************
	// �����蔻��
	//**********************************
	Collision();
}

//==========================================================================
// ���[�V�����̐ݒ�
//==========================================================================
void CPlayer::MotionSet(void)
{
	if (m_pMotion->IsFinish() == true)
	{// �I�����Ă�����

		// ���݂̎�ގ擾
		int nType = m_pMotion->GetType();

		if (m_pMotion->GetInfo(nType).nLoop == 1 &&
			nType != MOTION_DEF &&
			nType != MOTION_WALK)
		{
			return;
		}

		if (m_bMove == true && m_bATK == false)
		{// �ړ����Ă�����

			m_bMove = false;	// �ړ�����OFF

			// �ړ����[�V����
			m_pMotion->Set(MOTION_WALK);
		}
		else if (m_bMWait == true)
		{// �ҋ@����������

			// �ҋ@���[�V����
			m_pMotion->Set(MOTION_MAGIC_WAIT);
		}
		else if (m_bATK == true)
		{// �U�����Ă�����

			m_bATK = false;		// �U������OFF

			m_pMotion->Set(m_nNowMotion);
		}
		else
		{
			// �j���[�g�������[�V����
			m_pMotion->Set(MOTION_DEF);
		}
	}
}

//==========================================================================
// �U��
//==========================================================================
void CPlayer::Atack(void)
{

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// ���[�V�����̏��擾
	CMotion::Info aInfo = m_pMotion->GetInfo(m_pMotion->GetType());

	// �U�����̑����擾
	int nNumAttackInfo = aInfo.nNumAttackInfo;

	for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
	{
		if (aInfo.AttackInfo[nCntAttack] == NULL)
		{// NULL��������
			continue;
		}

		if (m_pMotion->GetAllCount() == aInfo.AttackInfo[nCntAttack]->nInpactCnt)
		{// �Ռ��̃J�E���g�Ɠ����Ƃ�

			D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

			if (GetObjectChara()->GetModel()[aInfo.AttackInfo[nCntAttack]->nCollisionNum] == NULL)
			{// NULL��������
				return;
			}

			// ���肷��p�[�c�̃}�g���b�N�X�擾
			D3DXMATRIX mtxWepon = GetObjectChara()->GetModel()[aInfo.AttackInfo[nCntAttack]->nCollisionNum]->GetWorldMtx();

			// �ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, aInfo.AttackInfo[nCntAttack]->Offset.x, aInfo.AttackInfo[nCntAttack]->Offset.y, aInfo.AttackInfo[nCntAttack]->Offset.z);
			D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

			// ����̈ʒu
			D3DXVECTOR3 weponpos = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

			// �p�[�e�B�N������
			CManager::GetParticle()->Create(weponpos, CParticle::TYPE_OFFSETTING);
		}

		// ���[�V�����J�E���^�[�擾
		if (m_pMotion->GetAllCount() > aInfo.AttackInfo[nCntAttack]->nMinCnt && m_pMotion->GetAllCount() < aInfo.AttackInfo[nCntAttack]->nMaxCnt)
		{// �U�����蒆

			// ���f�����擾
			CModel **pModel = GetObjectChara()->GetModel();
			D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

			// ����̃}�g���b�N�X�擾
			D3DXMATRIX mtxWepon;
			D3DXMatrixIdentity(&mtxWepon);

			if (pModel[aInfo.AttackInfo[nCntAttack]->nCollisionNum] == NULL)
			{// NULL��������
				return;
			}

			// ���肷��p�[�c�̃}�g���b�N�X�擾
			mtxWepon = pModel[aInfo.AttackInfo[nCntAttack]->nCollisionNum]->GetWorldMtx();

			// �ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, aInfo.AttackInfo[nCntAttack]->Offset.x, aInfo.AttackInfo[nCntAttack]->Offset.y, aInfo.AttackInfo[nCntAttack]->Offset.z);
			D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

			// ����̈ʒu
			D3DXVECTOR3 weponpos = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

			CEffect3D::Create(weponpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), aInfo.AttackInfo[nCntAttack]->fRangeSize, 4, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
		}
	}
}

//==========================================================================
// �����蔻��
//==========================================================================
void CPlayer::Collision(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// �␳
	/*ValueNormalize(pos.x, SCREEN_WIDTH - size.x, 0.0f + size.x);
	ValueNormalize(pos.y, (float)SCREEN_HEIGHT, 0.0f + size.y);*/

	// ���n�������ǂ���
	bool bLand = false;

	// �ʒu�擾
	D3DXVECTOR3 posOld = GetPosition();

	//// �������擾
	//CElevation *pMeshF = CManager::GetObject3DMesh();
	//pos.y = pMeshF->GetHeight(GetPosition(), bLand);

	//if (posOld.y != pos.y)
	//{
	//	// �W�����v�g�p�\�ɂ���
	//	m_bJump = false;
	//	bLand = true;	// ���n���
	//	move.y = 0.0f;
	//}

	if (pos.y <= 0.0f)
	{// �n�ʂɕt����

		bLand = true;	// ���n���
		pos.y = 0.0f;
		move.y = 0.0f;

		// �ʒu�ݒ�
		SetPosition(pos);
	}

	if (bLand == true)
	{// ���n���Ă���

		// �W�����v�g�p�\�ɂ���
		m_bJump = false;
		move.y = 0.0f;
	}
	
	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����ݒ�
	SetRotation(rot);
}


//==========================================================================
// �q�b�g����
//==========================================================================
bool CPlayer::Hit(const int nValue)
{
	if (m_state != STATE_DEAD && m_state != STATE_DMG)
	{// �_���[�W��t��Ԃ̎�

		// �̗͌��炷
		m_nLife -= nValue;

		if (m_nLife <= 0)
		{// �̗͂��Ȃ��Ȃ�����

			// �����
			m_state = STATE_DEAD;

			// �I������
			Uninit();

			// ����
			return true;
		}

		// �␳
		ValueNormalize(m_nLife, MAX_LIFE, 0);

		// �_���[�W��Ԃɂ���
		m_state = STATE_DMG;

		// �J�ڃJ�E���^�[�ݒ�
		m_nCntState = 10;
	}

	// ����łȂ�
	return false;
}


//==========================================================================
// ��ԍX�V����
//==========================================================================
void CPlayer::UpdateState(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �����擾
	D3DXVECTOR3 rot = GetRotation();

	// �ڕW�̌����擾
	float fRotDest = GetRotDest();

	// �ړ��ʎ擾
	float fMove = GetVelocity();

	// ���݂ƖڕW�̍���
	float fRotDiff = 0.0f;

	// ���݂̎�ގ擾
	int nType = m_pMotion->GetType();

	switch (m_state)
	{
	case STATE_NONE:
		// �F�ݒ�
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;

	case STATE_DMG:

		// ��ԑJ�ڃJ�E���^�[���Z
		m_nCntState--;

		// �F�ݒ�
		m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		if (m_nCntState <= 0)
		{// �J�ڃJ�E���^�[��0�ɂȂ�����

			// �e�ǂ��|����Ԃɂ���
			m_state = STATE_NONE;
			m_nCntState = 0;
		}
		break;

	}

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����ݒ�
	SetRotation(rot);

	// �ڕW�̌����ݒ�
	SetRotDest(fRotDest);
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CPlayer::SetPartsRot(void)
{
#define REPEAT		(2)	// ���s�[�g�̃J�E���g
#define ROT_MOVE	(D3DX_PI * 0.01f)

	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// �G�f�B�b�g�^�C�v�擾
	CManager::EDITTYPE editType = CManager::GetEditType();

	// �p�[�c���擾
	int nPartsNum = GetObjectChara()->GetNumModel();

	// ���[�V��������
	int nMotionNum = m_pMotion->GetNumMotion();

	// ����̃��f���擾
	CModel* pModel = GetObjectChara()->GetModel()[m_nNowParts];

	// ���[�V�����̏��擾
	CMotion::Info aInfo = m_pMotion->GetInfo(m_nNowMotion);


	// �����擾
	D3DXVECTOR3 scale = pModel->GetScale();
	if (editType == CManager::EDITTYPE_EDIT && pInputKeyboard->GetRepeat(DIK_R, REPEAT) == true)
	{// �X�P�[���g��
		scale.x += SCALE_MOVE;
		scale.y += SCALE_MOVE;
		scale.z += SCALE_MOVE;
	}
	else if (editType == CManager::EDITTYPE_EDIT && pInputKeyboard->GetRepeat(DIK_F, REPEAT) == true)
	{// �X�P�[���k��
		scale.x -= SCALE_MOVE;
		scale.y -= SCALE_MOVE;
		scale.z -= SCALE_MOVE;
	}
	if (pInputKeyboard->GetTrigger(DIK_V) == true)
	{// �X�P�[�����Z�b�g
		scale = mylib_const::DEFAULT_SCALE;
	}
	pModel->SetScale(scale);

	// �f�o�b�O���
	CManager::GetDebugProc()->Print("[F3]�F�|�[�Y�o�^\n");
	CManager::GetDebugProc()->Print("[F5]�F�v���C���[�z�u���Z�b�g\n");
	CManager::GetDebugProc()->Print("[F6]�F�|�[�Y�R�s�[\n");
	CManager::GetDebugProc()->Print("[F7]�F�|�[�Y�y�[�X�g\n");

	if (editType == CManager::EDITTYPE_EDIT)
	{
		CManager::GetDebugProc()->Print("[F9]�F���[�V�������Z�[�u[data/TEXT/key_info.txt]\n");
	}
	else if (editType == CManager::EDITTYPE_SETUP)
	{
		CManager::GetDebugProc()->Print("[F9]�F�Z�b�g�A�b�v���Z�[�u[data/TEXT/setup_info.txt]\n");
	}
	CManager::GetDebugProc()->Print("\n");

	CManager::GetDebugProc()->Print("���[�V�����؂�ւ��F[ �� ] [ �� ] �y%d / %d�z\n", m_nNowMotion, nMotionNum);
	CManager::GetDebugProc()->Print("���݂̃L�[�؂�ւ��F[ �P ] [ �Q ] �y%d / %d�z\n", m_nNowKey, aInfo.nNumKey);


	//***********************
	// �Z�[�u
	//***********************
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	// �e�L�X�g
	ImGui::Text("OutPut TextFile");
	if (ImGui::Button("Save Motion") ||
		pInputKeyboard->GetTrigger(DIK_F9))
	{
		SaveNowMotion();
	}
	ImGui::Separator();

#if 1
	// �����o��
	if (ImGui::Button(u8"�Z�[�u"))
	{
		OPENFILENAMEA filename = {};
		char sFilePass[1024] = {};
		// �t�@�C���I���_�C�A���O�̐ݒ�
		filename.lStructSize = sizeof(OPENFILENAMEA);
		filename.hwndOwner = NULL;
		filename.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
		filename.lpstrFile = sFilePass;
		filename.nMaxFile = MAX_PATH;
		filename.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;







		// �J�����g�f�B���N�g�����擾����
		char szCurrentDir[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, szCurrentDir);

		// "data"�t�H���_�̐�΃p�X�����߂�
		std::string strDataDir = szCurrentDir;
		strDataDir += "\\data";

		// ���݂���ꍇ�́AlpstrInitialDir�Ɏw�肷��
		if (GetFileAttributesA(strDataDir.c_str()) != INVALID_FILE_ATTRIBUTES)
		{
			filename.lpstrInitialDir = strDataDir.c_str();
		}
		else
		{
			// "data"�t�H���_�����݂��Ȃ��ꍇ�́A�G���[�������s���Ȃ�
		}





		//// ���΃p�X���΃p�X�ɕϊ�����
		//char szInitialDir[MAX_PATH] = {};
		//GetCurrentDirectoryA(MAX_PATH, szInitialDir);
		//PathCombineA(szInitialDir, szInitialDir, ".\\data");
		//PathCanonicalizeA(szInitialDir, szInitialDir);

		//filename.lpstrInitialDir = szInitialDir;

		// �t�@�C���I���_�C�A���O��\��
		if (GetOpenFileNameA(&filename))
		{
			// �I�����ꂽ�t�@�C���̃p�X��\��
			printf("Selected file: %s\n", sFilePass);
		}
		//�Z�[�u
		if (strcmp(&sFilePass[0], "") != 0)
		{
			// �Z�[�u���鏈��
		}
		else
		{
			//MessageBox(hWnd, "Failed to save", "Failed to save", MB_OK);
		}
	}
#endif

	if (editType != CManager::EDITTYPE_SETUP)
	{
		//***********************
		// �|�[�Y�o�^
		//***********************
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		// �e�L�X�g
		ImGui::Text("Controll Pause");
		if (ImGui::Button("Register Pause") ||
			pInputKeyboard->GetTrigger(DIK_F3))
		{
			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// ����̃��f���擾
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// ���[�V�������̓o�^
				m_pMotion->SetInfoSave(m_nNowMotion, m_nNowKey, nCntParts, pModel->GetRotation());
				m_pMotion->SetInfoSavePos(m_nNowMotion, m_nNowKey, nCntParts, pModel->GetPosition() - pModel->GetOriginPosition());
				m_pMotion->SetInfoSaveScale(m_nNowMotion, m_nNowKey, nCntParts, pModel->GetScale());
			}
			m_pMotion->ResetMoveData(m_nNowMotion, m_nNowKey);
		}
		ImGui::SameLine();

		//***********************
		// �|�[�Y�R�s�[
		//***********************
		if (ImGui::Button("Copy Pause"))
		{
			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// ����̃��f���擾
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// �R�s�[
				m_CopyData.aParts[nCntParts].rot = pModel->GetRotation();
				m_CopyData.aParts[nCntParts].pos = pModel->GetPosition();
			}
		}
		ImGui::SameLine();

		//***********************
		// �|�[�Y�y�[�X�g
		//***********************
		if (ImGui::Button("Paste Pause"))
		{
			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// ����̃��f���擾
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// �ۑ�����Ă�������ɐݒ�
				pModel->SetRotation(m_CopyData.aParts[nCntParts].rot);
				pModel->SetPosition(m_CopyData.aParts[nCntParts].pos);
			}
		}
		ImGui::Separator();


		// �e�L�X�g
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::Text("Change Motion / Key");

		//***********************
		// ���[�V�����؂�ւ�
		//***********************
		// [�O���[�v]���[�V�����؂�ւ�
		if (ImGui::CollapsingHeader("ChangeMotion"))
		{
			// [���W�I�{�^��]���[�V�����؂�ւ�
			for (int i = 0; i < m_pMotion->GetNumMotion(); i++)
			{
				if (ImGui::RadioButton(m_pMotion->GetInfo(i).filename.c_str(), &m_nNowMotion, i))
				{
					// ���㏑��
					aInfo = m_pMotion->GetInfo(m_nNowMotion);

					for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
					{
						// ����̃��f���擾
						CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
						if (pModel == NULL)
						{
							continue;
						}

						// �ۑ�����Ă�������ɐݒ�
						pModel->SetPosition(pModel->GetOriginPotision() + aInfo.aKey[m_nNowKey].aParts[nCntParts].pos);
						pModel->SetRotation(aInfo.aKey[m_nNowKey].aParts[nCntParts].rot);
						pModel->SetScale(aInfo.aKey[m_nNowKey].aParts[nCntParts].scale);
					}
				}
			}
		}

		if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
		{// �p�[�c�؂�ւ�
			m_nNowMotion = (m_nNowMotion + (nMotionNum - 1)) % nMotionNum;

			// ���㏑��
			aInfo = m_pMotion->GetInfo(m_nNowMotion);

			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// ����̃��f���擾
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// �ۑ�����Ă�������ɐݒ�
				pModel->SetPosition(pModel->GetOriginPotision() + aInfo.aKey[m_nNowKey].aParts[nCntParts].pos);
				pModel->SetRotation(aInfo.aKey[m_nNowKey].aParts[nCntParts].rot);
				pModel->SetScale(aInfo.aKey[m_nNowKey].aParts[nCntParts].scale);
			}
		}
		else if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
		{// �p�[�c�؂�ւ�
			m_nNowMotion = (m_nNowMotion + 1) % nMotionNum;

			// ���㏑��
			aInfo = m_pMotion->GetInfo(m_nNowMotion);

			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// ����̃��f���擾
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// �ۑ�����Ă�������ɐݒ�
				pModel->SetPosition(pModel->GetOriginPotision() + aInfo.aKey[m_nNowKey].aParts[nCntParts].pos);
				pModel->SetRotation(aInfo.aKey[m_nNowKey].aParts[nCntParts].rot);
				pModel->SetScale(aInfo.aKey[m_nNowKey].aParts[nCntParts].scale);
			}
		}


		//***********************
		// ���݂̃L�[�ύX
		//***********************
		ImGui::SetNextItemWidth(140.0f);
		// [�X���C�_�[]���݂̃L�[�ݒ�
		if (ImGui::SliderInt("NowKey", &m_nNowKey, 0, aInfo.nNumKey - 1))
		{
			// ���㏑��
			aInfo = m_pMotion->GetInfo(m_nNowMotion);

			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// ����̃��f���擾
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// �ۑ�����Ă�������ɐݒ�
				pModel->SetRotation(aInfo.aKey[m_nNowKey].aParts[nCntParts].rot);
				pModel->SetPosition(pModel->GetOriginPotision() + aInfo.aKey[m_nNowKey].aParts[nCntParts].pos);
				pModel->SetScale(aInfo.aKey[m_nNowKey].aParts[nCntParts].scale);
			}
		}

		if (pInputKeyboard->GetTrigger(DIK_1) == true)
		{// �L�[�؂�ւ�
			m_nNowKey = (m_nNowKey + (aInfo.nNumKey - 1)) % aInfo.nNumKey;

			// ���㏑��
			aInfo = m_pMotion->GetInfo(m_nNowMotion);

			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// ����̃��f���擾
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// �ۑ�����Ă�������ɐݒ�
				pModel->SetRotation(aInfo.aKey[m_nNowKey].aParts[nCntParts].rot);
				pModel->SetPosition(pModel->GetOriginPotision() + aInfo.aKey[m_nNowKey].aParts[nCntParts].pos);
				pModel->SetScale(aInfo.aKey[m_nNowKey].aParts[nCntParts].scale);
			}
		}
		else if (pInputKeyboard->GetTrigger(DIK_2) == true)
		{// �L�[�؂�ւ�
			m_nNowKey = (m_nNowKey + 1) % aInfo.nNumKey;

			// ���㏑��
			aInfo = m_pMotion->GetInfo(m_nNowMotion);

			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// ����̃��f���擾
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// �ۑ�����Ă�������ɐݒ�
				pModel->SetRotation(aInfo.aKey[m_nNowKey].aParts[nCntParts].rot);
				pModel->SetPosition(pModel->GetOriginPotision() + aInfo.aKey[m_nNowKey].aParts[nCntParts].pos);
				pModel->SetScale(aInfo.aKey[m_nNowKey].aParts[nCntParts].scale);
			}
		}


		//***********************
		// �L�[�����ύX
		//***********************
		ImGui::SameLine();
		ImGui::SetCursorPosX(250.0f); // �{�^���̉��ʒu��ݒ�
		ImGui::SetNextItemWidth(140.0f);

		// [�e�L�X�g�{�b�N�X]�L�[�̑���
		ImGui::InputInt("NumKey", &aInfo.nNumKey, 1);
		ValueNormalize(aInfo.nNumKey, MAX_KEY, 1);
		m_pMotion->ChangeKeyNum(m_nNowMotion, aInfo.nNumKey);
		ImGui::Separator();
	}


	if (editType == CManager::EDITTYPE_SETUP)
	{
		//***********************
		// ���[�V�����؂�ւ�
		//***********************
		if (pInputKeyboard->GetTrigger(DIK_4) == true)
		{// �L�[�t���[������

			// ���Z
			m_fSetUpBuff += 0.1f;
		}
		else if (pInputKeyboard->GetTrigger(DIK_3) == true)
		{// �L�[�t���[������

			// ���Z
			m_fSetUpBuff -= 0.1f;
		}

		// �������̓e�L�X�g�{�b�N�X���g�����ϐ��̒���
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::SetNextItemWidth(140.0f);
		ImGui::InputFloat("Multipy SetUp", &m_fSetUpBuff, 0.1f, 1.0f);
		ImGui::Separator();

		// �l�̐��K��
		if (m_fSetUpBuff < 0.0f)
		{
			m_fSetUpBuff = 0.1f;
		}

		CManager::GetDebugProc()->Print("�Z�b�g�A�b�v�{���F�y%f�z\n", m_fSetUpBuff);
	}


	//***********************
	// �����ݒ�
	//***********************
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	// [�O���[�v]�����ݒ�
	float windowWidth = 130.0f;
	if (ImGui::CollapsingHeader("Rotation"))
	{
		// ���Z�b�g
		if (ImGui::Button("ALL RESET"))
		{
			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// ����̃��f���擾
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == nullptr)
				{
					continue;
				}
				pModel->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}

		// [�X���C�_�[]�����ݒ�
		for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
		{
			// ����̃��f���擾
			CModel* pModel = GetModel()[nCntParts];
			if (pModel == nullptr)
			{
				continue;
			}

			// �����擾
			D3DXVECTOR3 rot = pModel->GetRotation();

			ImGui::Text(pModel->GetModelName().c_str());
			ImGui::SameLine();

			// X�̌���
			ImGui::PushID(nCntParts); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &rot.x, ROT_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
				ValueNormalize(rot.x, MAX_ROT, MIN_ROT);
			}
			ImGui::PopID();

			ImGui::PushID(nCntParts); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &rot.y, ROT_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
				ValueNormalize(rot.y, MAX_ROT, MIN_ROT);
			}
			ImGui::PopID();

			ImGui::PushID(nCntParts); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &rot.z, ROT_MOVE, 0.0f, 0.0f, "%.2f");
				ValueNormalize(rot.z, MAX_ROT, MIN_ROT);
			}
			ImGui::PopID();

			// �����ݒ�
			pModel->SetRotation(rot);
		}
	}


	//***********************
	// �ʒu�ݒ�
	//***********************
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	// [�O���[�v]�ʒu�ݒ�
	windowWidth = 130.0f;
	if (ImGui::CollapsingHeader("Position"))
	{
		// ���Z�b�g
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		if (ImGui::Button("ALL RESET"))
		{
			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// ����̃��f���擾
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == nullptr)
				{
					continue;
				}
				pModel->SetPosition(pModel->GetOriginPotision());
			}
		}
		ImGui::PopID();

		// [�X���C�_�[]�ʒu�ݒ�
		for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
		{
			// ����̃��f���擾
			CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
			if (pModel == NULL)
			{
				continue;
			}

			// �ʒu�擾
			D3DXVECTOR3 pos = pModel->GetPosition() - pModel->GetOriginPosition();

			ImGui::Text(pModel->GetModelName().c_str());
			ImGui::SameLine();

			// X�̌���
			ImGui::PushID(nCntParts + nPartsNum); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &pos.x, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			ImGui::PushID(nCntParts + nPartsNum); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &pos.y, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			ImGui::PushID(nCntParts + nPartsNum); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &pos.z, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();

			// �ʒu�ݒ�
			pModel->SetPosition(pos + pModel->GetOriginPosition());
		}
	}


	//***********************
	// �X�P�[���ݒ�
	//***********************
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	// [�O���[�v]�X�P�[���ݒ�
	windowWidth = 130.0f;
	if (ImGui::CollapsingHeader("Scale"))
	{
		// ���Z�b�g
		if (ImGui::Button("ALL RESET"))
		{
			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// ����̃��f���擾
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == nullptr)
				{
					continue;
				}
				pModel->SetScale({1.0f, 1.0f, 1.0f});
			}
		}

		// [�X���C�_�[]�X�P�[���ݒ�
		for (int nCntParts = 0, nID = nPartsNum + nPartsNum; nCntParts < nPartsNum; nCntParts++, nID++)
		{
			// ����̃��f���擾
			CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
			if (pModel == NULL)
			{
				continue;
			}

			// �ʒu�擾
			D3DXVECTOR3 scale = pModel->GetScale();

			ImGui::Text(pModel->GetModelName().c_str());
			ImGui::SameLine();

			// X�̌���
			ImGui::PushID(nID); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("scale", &scale.x, 0.01f, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();

			scale.y = scale.x;
			scale.z = scale.x;

			// �ʒu�ݒ�
			pModel->SetScale(scale);
		}
	}

#if 0
	//***********************
	// �f�o�b�O�\��
	//***********************
	CManager::GetDebugProc()->Print("\n------------------[ �G�f�B�b�g��� ]------------------\n");

	D3DXVECTOR3 posdebug = pModel->GetPosition() - pModel->GetOriginPosition();
	D3DXVECTOR3 posscale = pModel->GetScale();

	CManager::GetDebugProc()->Print(
		"��ʒu     [%d]�F�i%f, %f, %f�j��\n"
		"��X�P�[�� [%d]�F�i%f, %f, %f�j��\n", m_nNowParts, posdebug.x, posdebug.y, posdebug.z, m_nNowParts, posscale.x, posscale.y, posscale.z);


	/*if (editType == CManager::EDITTYPE_SETUP)
	{
		D3DXVECTOR3 pos = GetObjectChara()->GetModel()[nNow]->GetPosition();

		CManager::GetDebugProc()->Print("\n-----------[ �ʒu��� ]-----------\n");
		CManager::GetDebugProc()->Print(
			"�� [%d]�F�i%f, %f, %f�j��\n", m_nNowParts, pos.x, pos.y, pos.z);
		nPartsNum = 3;
	}*/

	if (editType == CManager::EDITTYPE_SETUP)
	{
		for (int nCnt = 0; nCnt < nPartsNum; nCnt++)
		{
			// ����̃��f���擾
			CModel* pModel = GetObjectChara()->GetModel()[nCnt];
			if (pModel == NULL)
			{
				continue;
			}

			// �����擾
			D3DXVECTOR3 pos;

			if (pModel != NULL)
			{// ����̃��f����NULL����Ȃ�������

				// �����擾
				pos = pModel->GetPosition();
			}
			else
			{
				pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			// �f�o�b�O���
			CManager::GetDebugProc()->Print(
				" [%d]�F(%f, %f, %f)\n", nCnt, pos.x, pos.y, pos.z);
		}
	}

	CManager::GetDebugProc()->Print("\n-----------[ ������� ]-----------\n");
	for (int nCnt = 0; nCnt < nPartsNum; nCnt++)
	{
		// ����̃��f���擾
		CModel* pModel = GetObjectChara()->GetModel()[nCnt];
		if (pModel == NULL)
		{
			continue;
		}

		// �����擾
		D3DXVECTOR3 rotParts;

		if (pModel != NULL)
		{// ����̃��f����NULL����Ȃ�������

			// �����擾
			rotParts = pModel->GetRotation();
		}
		else
		{
			rotParts = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		// �f�o�b�O���
		CManager::GetDebugProc()->Print(
			" [%d]�F(%f, %f, %f)\n", nCnt, rotParts.x, rotParts.y, rotParts.z);
	}
#endif
}

//==========================================================================
// �U�����ݒ�
//==========================================================================
void CPlayer::SetAttackInfo(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ���[�V�����̏��擾
	CMotion::Info aInfo = m_pMotion->GetInfo(m_nNowMotion);

	// �p�[�c���擾
	int nPartsNum = GetObjectChara()->GetNumModel();

	// ���[�V��������
	int nMotionNum = m_pMotion->GetNumMotion();

	// ���݂̎�ގ擾
	int nType = m_pMotion->GetType();

	CManager::GetDebugProc()->Print("\n++----------------------------------------------++\n");

	if (nType == 0)
	{
		CManager::GetDebugProc()->Print(" MOTION�F[%d] DEFAULT\n", nType);
	}
	else if (nType == 1)
	{
		CManager::GetDebugProc()->Print(" MOTION�F[%d] WALK\n", nType);
	}
	else if (nType == 2)
	{
		CManager::GetDebugProc()->Print(" MOTION�F[%d] ATTACK\n", nType);
	}

	CManager::GetDebugProc()->Print(
		" KEY   �F[%d / %d]\n"
		" FRAME �F[%d / %d] [%d / %d]\n"
		"++----------------------------------------------++\n",
		m_pMotion->GetNowPattern(), aInfo.nNumKey, 
		m_pMotion->GetFrameCount(), aInfo.aKey[m_pMotion->GetNowPattern()].nFrame, 
		m_pMotion->GetAllCount(), aInfo.nMaxAllFrame);

	CManager::GetDebugProc()->Print("���[�V�����؂�ւ��y%d / %d�z\n", m_nNowMotion, nMotionNum);
	CManager::GetDebugProc()->Print("���݂̃L�[�؂�ւ��y%d / %d�z\n", m_nNowKey, aInfo.nNumKey);
	CManager::GetDebugProc()->Print("���݂̃L�[�t���[���ύX�yFRAME�F%d�z\n", aInfo.aKey[m_nNowKey].nFrame);
	CManager::GetDebugProc()->Print("�L�����Z���\�t���[���ύX�yFRAME�F%d�z\n", aInfo.nCancelableFrame);
	CManager::GetDebugProc()->Print("�R���{�\�t���[���ύX�yFRAME�F%d�z\n", aInfo.nCombolableFrame);
	CManager::GetDebugProc()->Print("[P]�F�|�[�Y��Ԑ؂�ւ�\n");
	CManager::GetDebugProc()->Print("[F7]�F�J�����Ǐ]�؂�ւ�\n");


	// �E�B���h�E����
	ImGui::Begin("AttackInfo");
	{
		//***********************
		// ���[�V�����؂�ւ�
		//***********************
		// [�O���[�v]���[�V�����؂�ւ�
		if (ImGui::CollapsingHeader("ChangeMotion"))
		{
			// [���W�I�{�^��]���[�V�����؂�ւ�
			for (int i = 0; i < m_pMotion->GetNumMotion(); i++)
			{
				if (ImGui::RadioButton(m_pMotion->GetInfo(i).filename.c_str(), &m_nNowMotion, i))
				{
					// ���㏑��
					aInfo = m_pMotion->GetInfo(m_nNowMotion);
					m_pMotion->Set(m_nNowMotion);
					m_nNowAttackIdx = 0;	// �U���̃C���f�b�N�X���Z�b�g
				}
			}
		}

		if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
		{// �p�[�c�؂�ւ�
			m_nNowMotion = (m_nNowMotion + (nMotionNum - 1)) % nMotionNum;

			// ���㏑��
			aInfo = m_pMotion->GetInfo(m_nNowMotion);
			m_pMotion->Set(m_nNowMotion);
			m_nNowAttackIdx = 0;	// �U���̃C���f�b�N�X���Z�b�g
		}
		else if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
		{// �p�[�c�؂�ւ�
			m_nNowMotion = (m_nNowMotion + 1) % nMotionNum;

			// ���㏑��
			aInfo = m_pMotion->GetInfo(m_nNowMotion);
			m_pMotion->Set(m_nNowMotion);
			m_nNowAttackIdx = 0;	// �U���̃C���f�b�N�X���Z�b�g
		}

		if (m_nNowMotion == 1)
		{
			m_bMove = true;
		}


		//***********************
		// ���݂̃L�[�ύX
		//***********************
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::SetNextItemWidth(140.0f);
		// [�X���C�_�[]���݂̃L�[�ݒ�
		if (ImGui::SliderInt("NowKey", &m_nNowKey, 0, aInfo.nNumKey - 1))
		{
			// ���㏑��
			aInfo = m_pMotion->GetInfo(m_nNowMotion);

			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// ����̃��f���擾
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// ���㏑��
				aInfo = m_pMotion->GetInfo(m_nNowMotion);
			}
		}

		if (pInputKeyboard->GetTrigger(DIK_1) == true)
		{// �L�[�؂�ւ�
			m_nNowKey = (m_nNowKey + (aInfo.nNumKey - 1)) % aInfo.nNumKey;

			// ���㏑��
			aInfo = m_pMotion->GetInfo(m_nNowMotion);
		}
		else if (pInputKeyboard->GetTrigger(DIK_2) == true)
		{// �L�[�؂�ւ�
			m_nNowKey = (m_nNowKey + 1) % aInfo.nNumKey;

			// ���㏑��
			aInfo = m_pMotion->GetInfo(m_nNowMotion);
		}
		ImGui::SameLine();


		//***********************
		// �L�[�t���[���ύX
		//***********************
		ImGui::SetCursorPosX(250.0f); // �{�^���̉��ʒu��ݒ�
		ImGui::SetNextItemWidth(140.0f);
		// [�e�L�X�g�{�b�N�X]�L�[�Đ��t���[���̑���
		ImGui::InputInt("Key Frame", &aInfo.aKey[m_nNowKey].nFrame, 1, 10);
		ValueNormalize(aInfo.aKey[m_nNowKey].nFrame, 99999, 1);

		// �L�[�Đ��t���[���ύX
		m_pMotion->ChangeKeyFrame(m_nNowMotion, m_nNowKey, aInfo.aKey[m_nNowKey].nFrame);

		if (pInputKeyboard->GetRepeat(DIK_UP, REPEAT) == true)
		{// �L�[�t���[������

			// ���Z
			aInfo.aKey[m_nNowKey].nFrame++;

			// �l�̐��K��
			ValueNormalize(aInfo.aKey[m_nNowKey].nFrame, 99999, 1);

			// �L�[���ύX
			m_pMotion->ChangeKeyFrame(m_nNowMotion, m_nNowKey, aInfo.aKey[m_nNowKey].nFrame);
		}
		else if (pInputKeyboard->GetRepeat(DIK_DOWN, REPEAT) == true)
		{// �L�[�t���[������

			// ���Z
			aInfo.aKey[m_nNowKey].nFrame--;

			// �l�̐��K��
			ValueNormalize(aInfo.aKey[m_nNowKey].nFrame, 99999, 1);

			// �L�[���ύX
			m_pMotion->ChangeKeyFrame(m_nNowMotion, m_nNowKey, aInfo.aKey[m_nNowKey].nFrame);
		}


		//***********************
		// �L�����Z���\�t���[���ύX
		//***********************
		// [�X���C�_�[]�L�����Z���\�t���[���ݒ�
		if (ImGui::SliderInt("Cancelable Frame", &aInfo.nCancelableFrame, -1, aInfo.nMaxAllFrame))
		{
			// �L�����Z���\�t���[���ύX
			m_pMotion->ChangeCancelableFrame(m_nNowMotion, aInfo.nCancelableFrame);
		}


		//***********************
		// �R���{�\�t���[���ύX
		//***********************
		// [�X���C�_�[]�R���{�\�t���[���ݒ�
		if (ImGui::SliderInt("Combolable Frame", &aInfo.nCombolableFrame, -1, aInfo.nMaxAllFrame))
		{
			// �R���{�\�t���[���ύX
			m_pMotion->ChangeCombolableFrame(m_nNowMotion, aInfo.nCombolableFrame);
		}
		ImGui::Separator();


		//***********************
		// �U�����̑���
		//***********************
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		// �e�L�X�g
		ImGui::Text("AttackInfo Num: %d", aInfo.nNumAttackInfo);
		ImGui::SameLine();

		// [�{�^��]�U�����̑���
		if (ImGui::Button("Sub"))
		{
			// �������Z
			m_pMotion->SubNumAttackInfo(m_nNowMotion);
		}
		ImGui::SameLine();
		if (ImGui::Button("Add"))
		{
			// �������Z
			m_pMotion->AddNumAttackInfo(m_nNowMotion);
		}


		//***********************
		// ���݂̍U�����؂�ւ�
		//***********************
		if (ImGui::SliderInt("Now AttackInfo", &m_nNowAttackIdx, 0, aInfo.nNumAttackInfo - 1))
		{

		}
		ImGui::Separator();


		//***********************
		// �U������ݒ�
		//***********************
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		// �e�L�X�g
		ImGui::Text("Collision FrameRange");

		bool bSlide = false;

		// [�X���C�_�[]�U������̍ŏ��l
		ImGui::SetNextItemWidth(130.0f);
		if (ImGui::SliderInt("Min Frame", &aInfo.AttackInfo[m_nNowAttackIdx]->nMinCnt, 0, aInfo.nMaxAllFrame))
		{
			bSlide = true;
		}
		ImGui::SameLine();

		ImGui::SetNextItemWidth(130.0f);
		if (ImGui::SliderInt("MAX Frame", &aInfo.AttackInfo[m_nNowAttackIdx]->nMaxCnt, 0, aInfo.nMaxAllFrame))
		{
			bSlide = true;
		}

		if (bSlide)
		{
			// ���f�����擾
			CModel** pModel = GetObjectChara()->GetModel();

			if (pModel[aInfo.AttackInfo[m_nNowAttackIdx]->nCollisionNum] != NULL)
			{
				D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

				// ����̃}�g���b�N�X�擾
				D3DXMATRIX mtxWepon;
				D3DXMatrixIdentity(&mtxWepon);

				// ���肷��p�[�c�̃}�g���b�N�X�擾
				mtxWepon = pModel[aInfo.AttackInfo[m_nNowAttackIdx]->nCollisionNum]->GetWorldMtx();

				// �ʒu�𔽉f����
				D3DXMatrixTranslation(&mtxTrans, aInfo.AttackInfo[m_nNowAttackIdx]->Offset.x, aInfo.AttackInfo[m_nNowAttackIdx]->Offset.y, aInfo.AttackInfo[m_nNowAttackIdx]->Offset.z);
				D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

				// ����̈ʒu
				D3DXVECTOR3 weponpos = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

				CEffect3D::Create(
					weponpos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
					aInfo.AttackInfo[m_nNowAttackIdx]->fRangeSize,
					SPHERE_LIFE,
					CEffect3D::MOVEEFFECT_NONE,
					CEffect3D::TYPE_NORMAL);
			}
		}


		//***********************
		// �Ռ��̃J�E���g
		//***********************
		// [�X���C�_�[]�Ռ��̃J�E���g
		if (ImGui::SliderInt("Impact Frame", &aInfo.AttackInfo[m_nNowAttackIdx]->nInpactCnt, -1, aInfo.nMaxAllFrame))
		{

		}


		//***********************
		// ����̃T�C�Y
		//***********************
		if (ImGui::DragFloat("Collision Radius", &aInfo.AttackInfo[m_nNowAttackIdx]->fRangeSize, 1.0f, 0.0f, 0.0f, "%.2f"))
		{
			// �l�̐��K��
			ValueNormalize(aInfo.AttackInfo[m_nNowAttackIdx]->fRangeSize, 9999.0f, 0.0f);
		}

		//***********************
		// ��������p�[�c�؂�ւ�
		//***********************
		if (ImGui::SliderInt("CollisionDetector", &aInfo.AttackInfo[m_nNowAttackIdx]->nCollisionNum, 0, nPartsNum))
		{

		}


		//***********************
		// �I�t�Z�b�g�ʒu�ݒ�
		//***********************
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		// [�O���[�v]�ʒu�ݒ�
		float windowWidth = 130.0f;
		if (ImGui::CollapsingHeader("Position"))
		{
			// ���Z�b�g
			if (ImGui::Button("ALL RESET"))
			{
				aInfo.AttackInfo[m_nNowAttackIdx]->Offset.x = 0.0f;
				aInfo.AttackInfo[m_nNowAttackIdx]->Offset.y = 0.0f;
				aInfo.AttackInfo[m_nNowAttackIdx]->Offset.z = 0.0f;
			}

			// [�X���C�_�[]�ʒu�ݒ�
			// X
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &aInfo.AttackInfo[m_nNowAttackIdx]->Offset.x, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();

			// Y
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &aInfo.AttackInfo[m_nNowAttackIdx]->Offset.y, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();

			// Z
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &aInfo.AttackInfo[m_nNowAttackIdx]->Offset.z, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
		}
	}
	ImGui::End();





	


	// �I�t�Z�b�g�ݒ�
	CManager::GetDebugProc()->Print("\n---------------------------------[�U�����]---------------------------------\n");
	CManager::GetDebugProc()->Print(" �����蔻��̑����y NUM�F%d �z\n", aInfo.nNumAttackInfo);
	CManager::GetDebugProc()->Print(" ���݂̓����蔻��y %d / %d �z\n", m_nNowAttackIdx, aInfo.nNumAttackInfo);
	CManager::GetDebugProc()->Print(" ���_����̃I�t�Z�b�g�y %f, %f, %f �z\n", aInfo.AttackInfo[m_nNowAttackIdx]->Offset.x, aInfo.AttackInfo[m_nNowAttackIdx]->Offset.y, aInfo.AttackInfo[m_nNowAttackIdx]->Offset.z);
	CManager::GetDebugProc()->Print(" ��������J�E���g�y %d �` %d �z\n", aInfo.AttackInfo[m_nNowAttackIdx]->nMinCnt, aInfo.AttackInfo[m_nNowAttackIdx]->nMaxCnt);
	CManager::GetDebugProc()->Print(" �Ռ��̃J�E���g�y %d �z\n", aInfo.AttackInfo[m_nNowAttackIdx]->nInpactCnt);
	CManager::GetDebugProc()->Print(" ����̃T�C�Y�y %f �z\n", aInfo.AttackInfo[m_nNowAttackIdx]->fRangeSize);
	CManager::GetDebugProc()->Print(" �����蔻������p�[�c�y %d �z\n", aInfo.AttackInfo[m_nNowAttackIdx]->nCollisionNum);

	// �U�����ݒ�
	if (aInfo.AttackInfo[m_nNowAttackIdx] != NULL)
	{
		m_pMotion->ChangeAttackInfo(m_nNowMotion, m_nNowAttackIdx, *aInfo.AttackInfo[m_nNowAttackIdx]);
	}
}

//==========================================================================
// ���̓����蔻��ݒ�
//==========================================================================
void CPlayer::SetSphereCollider(void)
{

	// �p�[�c���擾
	int nPartsNum = GetNumModel();

	SphereCollider collider = GetNowSphereCollider(m_nNowColliderIdx);

	//***********************
	// �U�����̑���
	//***********************
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	// �e�L�X�g
	ImGui::Text("Collider Num: %d", GetNumCollider());
	ImGui::SameLine();

	// [�{�^��]�U�����̑���
	if (ImGui::Button("Sub"))
	{
		// �������Z
		SubCollider();
	}
	ImGui::SameLine();
	if (ImGui::Button("Add"))
	{
		// �������Z
		AddCollider(SphereCollider());
	}

	//***********************
	// ���݂̃R���C�_�[�؂�ւ�
	//***********************
	if (GetNumCollider() <= 0)
	{
		// �e�L�X�g
		ImGui::Text("NO Collider");
	}
	else
	{
		if (ImGui::SliderInt("Now Collider", &m_nNowColliderIdx, 0, GetNumCollider() - 1))
		{
			collider = GetNowSphereCollider(m_nNowColliderIdx);
		}
	}
	ImGui::Separator();



	//***********************
	// ����̃T�C�Y
	//***********************
	if (ImGui::DragFloat("Collision Radius", &collider.radius, 1.0f, 0.0f, 0.0f, "%.2f"))
	{
		if (collider.radius <= 1.0f)
		{
			collider.radius = 1.0f;
		}
	}

	//***********************
	// ��������p�[�c�؂�ւ�
	//***********************
	if (ImGui::SliderInt("Collision Detector", &collider.nParentPartsIdx, 0, nPartsNum - 1))
	{

	}

	//***********************
	// �I�t�Z�b�g�ʒu�ݒ�
	//***********************
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	// [�O���[�v]�ʒu�ݒ�
	float windowWidth = 130.0f;
	if (ImGui::CollapsingHeader("Collider Position"))
	{
		// ���Z�b�g
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		if (ImGui::Button("ALL RESET"))
		{
			collider.offset.x = 0.0f;
			collider.offset.y = 0.0f;
			collider.offset.z = 0.0f;
		}
		ImGui::PopID();

		// [�X���C�_�[]�ʒu�ݒ�
		// X
		ImGui::SetNextItemWidth(windowWidth);
		ImGui::DragFloat("x", &collider.offset.x, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
		ImGui::SameLine();

		// Y
		ImGui::SetNextItemWidth(windowWidth);
		ImGui::DragFloat("y", &collider.offset.y, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
		ImGui::SameLine();

		// Z
		ImGui::SetNextItemWidth(windowWidth);
		ImGui::DragFloat("z", &collider.offset.z, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
	}



	// ����̃��f���擾
	CModel* pModel = GetObjectChara()->GetModel()[collider.nParentPartsIdx];

	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	// ����̃}�g���b�N�X�擾
	D3DXMATRIX mtxWepon;
	D3DXMatrixIdentity(&mtxWepon);

	// ���肷��p�[�c�̃}�g���b�N�X�擾
	mtxWepon = pModel->GetWorldMtx();

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, collider.offset.x, collider.offset.y, collider.offset.z);
	D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

	// ����̈ʒu
	D3DXVECTOR3 weponpos = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

	collider.center = weponpos;


	SetNowSphereCollider(m_nNowColliderIdx, collider);

	for (int i = 0; i < GetNumCollider(); i++)
	{
		SphereCollider collider = GetNowSphereCollider(i);

		// ����̃��f���擾
		CModel* pModel = GetObjectChara()->GetModel()[collider.nParentPartsIdx];

		D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

		// ����̃}�g���b�N�X�擾
		D3DXMATRIX mtxWepon;
		D3DXMatrixIdentity(&mtxWepon);

		// ���肷��p�[�c�̃}�g���b�N�X�擾
		mtxWepon = pModel->GetWorldMtx();

		// �ʒu�𔽉f����
		D3DXMatrixTranslation(&mtxTrans, collider.offset.x, collider.offset.y, collider.offset.z);
		D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

		// ����̈ʒu
		D3DXVECTOR3 weponpos = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

		collider.center = weponpos;

		SetNowSphereCollider(i, collider);

		D3DXCOLOR col = mylib_const::DEFAULT_COLOR;
		if (i == m_nNowColliderIdx)
		{
			col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		GetMeshSphere(i)->SetColor(col);
	}
	ImGui::Separator();










	//***********************
	// ���S�ʒu�ݒ�
	//***********************
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::Text("Center Position");

	int centerID = GetCenterPartsIdx();
	//***********************
	// ���S�̃p�[�c�؂�ւ�
	//***********************
	if (ImGui::SliderInt("CenterPartsID", &centerID, 0, nPartsNum - 1))
	{

	}
	SetCenterPartsIdx(centerID);

	D3DXVECTOR3 centerOffset = GetCenterOffset();

	// [�O���[�v]�ʒu�ݒ�
	if (ImGui::CollapsingHeader("Center Position"))
	{
		// ���Z�b�g
		ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		if (ImGui::Button("ALL RESET"))
		{
			centerOffset.x = 0.0f;
			centerOffset.y = 0.0f;
			centerOffset.z = 0.0f;
		}

		// [�X���C�_�[]�ʒu�ݒ�
		// X
		ImGui::SetNextItemWidth(windowWidth);
		ImGui::DragFloat("x", &centerOffset.x, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
		ImGui::SameLine();

		// Y
		ImGui::SetNextItemWidth(windowWidth);
		ImGui::DragFloat("y", &centerOffset.y, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
		ImGui::SameLine();

		// Z
		ImGui::SetNextItemWidth(windowWidth);
		ImGui::DragFloat("z", &centerOffset.z, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
		ImGui::PopID();
	}
	SetCenterOffset(centerOffset);
}

//==========================================================================
// ���݂̃��[�V�����Z�[�u
//==========================================================================
void CPlayer::SaveNowMotion(void)
{

	// �G�f�B�b�g�^�C�v�擾
	CManager::EDITTYPE editType = CManager::GetEditType();

	if (editType == CManager::EDITTYPE_EDIT)
	{
		// ���݂̃��[�V�����Z�[�u
		SaveMotionInfo();
	}
	else if (editType == CManager::EDITTYPE_SETUP)
	{
		// ���݂̃Z�b�g�A�b�v���Z�[�u
		SaveSetUpInfo();
		SaveSphereColliders();
	}
}

//==========================================================================
// ���݂̃��[�V�����Z�[�u
//==========================================================================
void CPlayer::SaveMotionInfo(void)
{
	FILE *pFile = NULL;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen("data\\TEXT\\key_info.txt", "w");

	if (pFile == NULL)
	{// �t�@�C�����J���Ȃ������ꍇ
		return;
	}

	// ���[�V�����̏��擾
	CMotion::Info aInfo = m_pMotion->GetInfo(m_nNowMotion);

	// �p�[�c���擾
	int nPartsNum = GetObjectChara()->GetNumModel();

	fprintf(pFile,
		"#==============================================================================\n"
		"#\n"
		"# ���[�V�������X�N���v�g�t�@�C��[%s]\n"
		"# Author : ���n �Ή�\n"
		"#\n"
		"#==============================================================================\n", m_pMotion->GetMotionFilename(m_nNowMotion).c_str());

	fprintf(pFile,
		"MOTIONSET\n"
		"\tLOOP = %d\t\t# ���[�v���邩�ǂ���[0:���[�v���Ȃ� / 1:���[�v����]\n"
		"\tNUM_KEY = %d\t\t# �L�[��\n"
		"\tMOVE = %d\t\t# �ړ��ł��邩�ǂ���[0:�ړ��ł��Ȃ� / 1:�ړ��ł���]\n"
		"\tCANCELABLE = %d\t# �L�����Z���\�t���[��[-1�F�L�����Z���s��]\n"
		"\tCOMBOLABLE = %d\t# �R���{�\�t���[��[-1�F�L�����Z���s��]\n\n"
		, aInfo.nLoop, aInfo.nNumKey, aInfo.nMove, aInfo.nCancelableFrame, aInfo.nCombolableFrame);

	fprintf(pFile,
		"\t#===================\n"
		"\t# �U�����\n"
		"\t#===================\n");
	for (int nCntAttack = 0; nCntAttack < aInfo.nNumAttackInfo; nCntAttack++)
	{
		fprintf(pFile,
			"\tATTACKINFO\n"
			"\t\tCOLLISIONPARTS = %d\n"
			"\t\tATTACKRANGE = %.2f\n"
			"\t\tOFFSET = %.2f %.2f %.2f\n"
			"\t\tATTACKCOUNT = %d %d\n"
			"\t\tINPACTCOUNT = %d\n"
			"\t\tDAMAGE = %d\n"
			"\tEND_ATTACKINFO\n\n",
			aInfo.AttackInfo[nCntAttack]->nCollisionNum, aInfo.AttackInfo[nCntAttack]->fRangeSize, aInfo.AttackInfo[nCntAttack]->Offset.x, aInfo.AttackInfo[nCntAttack]->Offset.y, aInfo.AttackInfo[nCntAttack]->Offset.z,
			aInfo.AttackInfo[nCntAttack]->nMinCnt, aInfo.AttackInfo[nCntAttack]->nMaxCnt, aInfo.AttackInfo[nCntAttack]->nInpactCnt, aInfo.AttackInfo[nCntAttack]->nDamage);
	}

	for (int nCntKey = 0; nCntKey < aInfo.nNumKey; nCntKey++)
	{
		fprintf(pFile, "\tKEYSET\t\t\t# --- << KEY : %d / %d >> ---\n", nCntKey, aInfo.nNumKey);
		fprintf(pFile, "\t\tFRAME = %d\n", aInfo.aKey[nCntKey].nFrame);

		for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
		{// �p�[�c�����J��Ԃ�

			fprintf(pFile,
				"\t\tPARTS\t# ----- [ %d ] -----\n"
				"\t\t\tROT = %.2f %.2f %.2f\n"
				"\t\t\tPOS = %.2f %.2f %.2f\n"
				"\t\t\tSCALE = %.2f %.2f %.2f\n"
				"\t\tEND_PARTS\n\n", nCntParts,
				aInfo.aKey[nCntKey].aParts[nCntParts].rot.x, aInfo.aKey[nCntKey].aParts[nCntParts].rot.y, aInfo.aKey[nCntKey].aParts[nCntParts].rot.z,
				aInfo.aKey[nCntKey].aParts[nCntParts].pos.x, aInfo.aKey[nCntKey].aParts[nCntParts].pos.y, aInfo.aKey[nCntKey].aParts[nCntParts].pos.z,
				aInfo.aKey[nCntKey].aParts[nCntParts].scale.x, aInfo.aKey[nCntKey].aParts[nCntParts].scale.y, aInfo.aKey[nCntKey].aParts[nCntParts].scale.z);
		}
		fprintf(pFile, "\tEND_KEYSET\n\n");
	}
	fprintf(pFile, "END_MOTIONSET\n\n");

	//�t�@�C�������
	fclose(pFile);
}

//==========================================================================
// ���݂̃Z�b�g�A�b�v���Z�[�u
//==========================================================================
void CPlayer::SaveSetUpInfo(void)
{
	FILE *pFile = NULL;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen("data\\TEXT\\setup_info.txt", "w");

	if (pFile == NULL)
	{// �t�@�C�����J���Ȃ������ꍇ
		return;
	}

	// ���[�V�����̏��擾
	CMotion::Info aInfo = m_pMotion->GetInfo(m_nNowMotion);

	// �p�[�c���擾
	int nPartsNum = GetObjectChara()->GetNumModel();

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# �L�����N�^�[���\n"
		"#------------------------------------------------------------------------------\n");

	fprintf(pFile,
		"CHARACTERSET\n\n"
		"\tCENTERSET\n"
		"\t\tPARTS = %d\t\t# ���S�ɂ���p�[�c\n"
		"\t\tOFFSET = %.2f %.2f %.2f\t\t# �I�t�Z�b�g�ʒu\n"
		"\tEND_CENTERSET\n\n"
		"\tVELOCITY = %.2f\t\t# �ړ���\n"
		"\tJUMP = %.2f\t\t# �W�����v��\n"
		"\tRADIUS = %.2f\t\t# ���a\n"
		"\tHEIGHT = %.2f\t\t# ����\n"
		"\tLIFE = %d\t\t# �̗�\n", 
		GetCenterPartsIdx(),
		GetCenterOffset().x,
		GetCenterOffset().y,
		GetCenterOffset().z,
		GetVelocity(), 0.00f, GetRadius(), 0.00f, 5);

	for (int nCntParts = 0; nCntParts < GetObjectChara()->GetNumModel(); nCntParts++)
	{
		// ����̃��f���擾
		CModel *pModel = GetObjectChara()->GetModel()[nCntParts];
		if (pModel == NULL)
		{// NULL�Ŕ�����
			continue;
		}

		// �ʒu�擾
		D3DXVECTOR3 posParts = pModel->GetPosition();
		D3DXVECTOR3 rotParts = pModel->GetRotation();
		posParts *= m_fSetUpBuff;

		fprintf(pFile,
			"\n"
			"\tPARTSSET\n"
			"\t\tINDEX = %d\n"
			"\t\tPARENT = %d\n"
			"\t\tPOS = %.2f %.2f %.2f\n"
			"\t\tROT = %.2f %.2f %.2f\n"
			"\t\tSTART = %d\n"
			"\tEND_PARTSSET\n", nCntParts, pModel->GetParentIdx(),
			posParts.x, posParts.y, posParts.z,
			rotParts.x, rotParts.y, rotParts.z,
			1);
	}

	fprintf(pFile, "END_CHARACTERSET\n");

	//�t�@�C�������
	fclose(pFile);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CPlayer::Draw(void)
{
	int nNowParts = m_nNowParts;
	if (CManager::GetEditType() == CManager::EDITTYPE_PLAY)
	{// �v���C��ԂȂ�
		nNowParts = -1;
	}

	CObjectChara::Draw(nNowParts, 0.6f);
}
