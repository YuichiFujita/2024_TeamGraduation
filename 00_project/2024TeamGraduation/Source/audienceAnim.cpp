//==========================================================================
// 
//  �ϋq_�A�j���[�V����3D���� [audienceAnim.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "audienceAnim.h"
#include "gameManager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int PRIORITY = mylib_const::PRIORITY_DEFAULT;		// �D�揇��
	const char* SETUP_TXT = "data\\CHARACTER\\frisk.txt";	// �v���C���[�Z�b�g�A�b�v�e�L�X�g

	const int LEFT_LINE[]  = { (int)CAudienceAnim::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };		// �`�[���T�C�h���Ƃ̍����C��
	const int RIGHT_LINE[] = { (int)CGameManager::CENTER_LINE, (int)CAudienceAnim::MAX_RIGHT_LINE };	// �`�[���T�C�h���Ƃ̉E���C��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CAudienceAnim::CAudienceAnim(EObjType type, CGameManager::TeamSide team) : CAudience(type, team, PRIORITY, CObject::LAYER_DEFAULT),
	m_pAnimChara	(nullptr)	// �L�����N�^�[�A�j���[�V�������
{
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CAudienceAnim::~CAudienceAnim()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CAudienceAnim::Init()
{
	// �e�N���X�̏�����
	if (FAILED(CAudience::Init()))
	{ // �������Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �����_���Ɋϐ�ʒu��ݒ�
	int nIdxTeam = GetTeam() - 1;
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(LEFT_LINE[nIdxTeam], RIGHT_LINE[nIdxTeam]);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random((int)NEAR_LINE, (int)FAR_LINE);
	SetWatchPosition(posWatch);

	// �����_���ɐ����ʒu��ݒ�
	MyLib::Vector3 posSpawn = posWatch;
	float fTurn = ((bool)(rand() % 2)) ? 1.0f : -1.0f;
	posSpawn.x = CAudience::SPAWN_SIDE_LINE * fTurn;
	SetSpawnPosition(posSpawn);

	// �I�u�W�F�N�g�L�����N�^�[�A�j���[�V�����̐���
	m_pAnimChara = CObjectCharaAnim::Create(posSpawn);
	if (m_pAnimChara == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �L�����N�^�[���̊���
	m_pAnimChara->BindCharaData(SETUP_TXT);

	// ���[�V������ݒ�
	m_pAnimChara->SetMotion(1);

	// �A�j���[�V����3D�̎����X�V/�����`��/�����j�������Ȃ���ނɂ���
	m_pAnimChara->SetType(CObject::TYPE::TYPE_NONE);

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CAudienceAnim::Uninit()
{
	// �e�N���X�̏I��
	CAudience::Uninit();

	// �I�u�W�F�N�g�L�����N�^�[�A�j���[�V�����̏I��
	SAFE_UNINIT(m_pAnimChara);
}

//==========================================================================
// �폜����
//==========================================================================
void CAudienceAnim::Kill()
{
	// �e�N���X�̍폜
	CAudience::Kill();

	// �I�u�W�F�N�g�L�����N�^�[�A�j���[�V�����̏I��
	SAFE_UNINIT(m_pAnimChara);
}

//==========================================================================
// �X�V����
//==========================================================================
void CAudienceAnim::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	int nCurMotion = m_pAnimChara->GetMotion();	// TODO

	if (m_pAnimChara != nullptr)
	{
		// ����ł���ꍇ������
		if (IsDeath()) { return; }

		// ���[�V�������ݒ肳��Ă��Ȃ��ꍇ������
		if (nCurMotion == -1) { return; }

		int nAnimMotion = m_pAnimChara->GetMotion();	// ���ݍĐ����̃��[�V����
		if (m_pAnimChara->IsLoop())
		{ // ���[�v���郂�[�V�����������ꍇ

			if (nAnimMotion != nCurMotion)
			{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

				// ���݂̃��[�V�����̐ݒ�
				m_pAnimChara->SetMotion(nCurMotion);
			}
		}

		// �I�u�W�F�N�g�L�����N�^�[�A�j���[�V�����̍X�V
		m_pAnimChara->Update(fDeltaTime, fDeltaRate, fSlowRate);

#if 0
		switch (m_pAnimChara->GetMotion())
		{ // ���[�V�����̎�ނ��Ƃ̏���
		case MOTION_IDOL_U:	// ��ҋ@���[�V����
		case MOTION_IDOL_D:	// ���ҋ@���[�V����
		case MOTION_IDOL_L:	// ���ҋ@���[�V����
		case MOTION_IDOL_R:	// �E�ҋ@���[�V����
		case MOTION_MOVE_U:	// ��ړ����[�V����
		case MOTION_MOVE_D:	// ���ړ����[�V����
		case MOTION_MOVE_L:	// ���ړ����[�V����
		case MOTION_MOVE_R:	// �E�ړ����[�V����
			break;
		}
#endif
	}

	// �e�N���X�̍X�V
	CAudience::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CAudienceAnim::Draw()
{
	// �I�u�W�F�N�g�L�����N�^�[�A�j���[�V�����̕`��
	m_pAnimChara->Draw();

	// �e�N���X�̕`��
	CAudience::Draw();
}
