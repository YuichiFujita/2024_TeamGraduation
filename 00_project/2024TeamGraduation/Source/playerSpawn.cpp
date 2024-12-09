//==========================================================================
// 
//  �o�ꉉ�o�p�v���C���[���� [playerSpawn.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerSpawn.h"
#include "manager.h"
#include "playerSpawnManager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerSpawn::CPlayerSpawn(const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase, int nPriority) :
	CPlayer(typeTeam, typeArea, typeBase, nPriority)
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerSpawn::~CPlayerSpawn()
{
	
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayerSpawn::Init()
{
	// �e�N���X�̏�����
	HRESULT hr = CPlayer::Init();
	if (FAILED(hr)) { return E_FAIL; }

	// �v���C���[�o�ꉉ�o�}�l�[�W���[�Ɋ���
	CPlayerSpawnManager* pManager = CPlayerSpawnManager::GetInstance();	// �v���C���[�o�ꉉ�o�}�l�[�W���[
	if (pManager != nullptr) { pManager->RegistPlayer(this); }			// �}�l�[�W���[������ꍇ�o�^

	// ���[�V�����ݒ�
	GetMotion()->Set(EMotion::MOTION_WALK_SPAWN);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerSpawn::Uninit()
{
	// �e�N���X�̏I��
	CPlayer::Uninit();

	// �v���C���[�o�ꉉ�o�}�l�[�W���[����폜
	CPlayerSpawnManager* pManager = CPlayerSpawnManager::GetInstance();	// �v���C���[�o�ꉉ�o�}�l�[�W���[
	if (pManager != nullptr) { pManager->DeletePlayer(this); }			// �}�l�[�W���[������ꍇ�폜
}

//==========================================================================
// �폜
//==========================================================================
void CPlayerSpawn::Kill()
{
	// �e�N���X�̏I��
	CPlayer::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerSpawn::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ߋ��̈ʒu�ۑ�
	SetOldPosition(GetPosition());

	// �e�N���X�̍X�V����
	CObjectChara::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CPlayerSpawn::Draw()
{
	// �e�N���X�̕`��
	CPlayer::Draw();
}
