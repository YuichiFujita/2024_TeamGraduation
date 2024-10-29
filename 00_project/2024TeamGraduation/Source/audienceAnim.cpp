//==========================================================================
// 
//  �ϋq_�A�j���[�V����3D���� [audienceAnim.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "audienceAnim.h"
#include "gameManager.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\frisk.txt";	// �v���C���[�Z�b�g�A�b�v�e�L�X�g
	const char* LIGHT_PATH = "data\\MODEL\\penlight.x";		// �y�����C�g�̃��f���p�X
	const int PRIORITY = mylib_const::PRIORITY_DEFAULT;		// �D�揇��

	const int LEFT_LINE[]  = { (int)CAudienceAnim::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };		// �`�[���T�C�h���Ƃ̍����C��
	const int RIGHT_LINE[] = { (int)CGameManager::CENTER_LINE, (int)CAudienceAnim::MAX_RIGHT_LINE };	// �`�[���T�C�h���Ƃ̉E���C��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CAudienceAnim::CAudienceAnim(EObjType type, CGameManager::TeamSide team) : CAudience(type, team, PRIORITY, CObject::LAYER_DEFAULT),
	m_pAnimChara	(nullptr),		// �L�����N�^�[�A�j���[�V�������
	m_pLight		(nullptr),		// �y�����C�g���
	m_moveMotion	(MOTION_IDOL)	// �ړ����[�V����
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

	// �y�����C�g�̐���
	m_pLight = CObjectX::Create(LIGHT_PATH, VEC3_ZERO);
	if (m_pLight == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ�ݒ�
	m_pLight->SetType(CObject::TYPE::TYPE_OBJECTX);

	// �����`��t���O���I�t�ɂ���
	m_pLight->SetEnableDisp(false);

	// TODO�F�X�P�[��������ŏ���
#if 1
	m_pLight->SetScale(MyLib::Vector3(1.0f, 0.4f, 1.0f));
#endif

	// �����_���Ɋϐ�ʒu��ݒ�
	int nIdxTeam = GetTeam() - 1;
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(LEFT_LINE[nIdxTeam], RIGHT_LINE[nIdxTeam]);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random((int)NEAR_LINE, (int)FAR_LINE);
	SetWatchPosition(posWatch);

	// �����_���ɐ���������ݒ�
	float fTurn = 0.0f;	// �����W��
	if ((bool)(rand() % 2))
	{
		// �E����̓���ɂ���
		m_moveMotion = MOTION_MOVE_L;
		fTurn = 1.0f;
	}
	else
	{
		// ������̓���ɂ���
		m_moveMotion = MOTION_MOVE_R;
		fTurn = -1.0f;
	}

	// �����ʒu��ݒ�
	MyLib::Vector3 posSpawn = posWatch;
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
	m_pAnimChara->SetMotion(m_moveMotion);

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
	if (m_pAnimChara != nullptr)
	{
		// �I�u�W�F�N�g�L�����N�^�[�A�j���[�V�����̍X�V
		m_pAnimChara->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// �e�N���X�̍X�V
	CAudience::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CAudienceAnim::Draw()
{
	if (m_pAnimChara != nullptr)
	{
		LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X���

		// �A���t�@�e�X�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 180);

		// �I�u�W�F�N�g�L�����N�^�[�A�j���[�V�����̕`��
		m_pAnimChara->Draw();

		// �A���t�@�e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	}

	// �e�N���X�̕`��
	CAudience::Draw();
}

//==========================================================================
// �����Ԃ̍X�V����
//==========================================================================
int CAudienceAnim::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����Ԃ̍X�V
	CAudience::UpdateSpawn(fDeltaTime, fDeltaRate, fSlowRate);

	// ���C�g�̎����`����I�t�ɂ���
	m_pLight->SetEnableDisp(false);

	// �ړ����[�V������Ԃ�
	return m_moveMotion;
}

//==========================================================================
// �ʏ��Ԃ̍X�V����
//==========================================================================
int CAudienceAnim::UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ʏ��Ԃ̍X�V
	CAudience::UpdateNormal(fDeltaTime, fDeltaRate, fSlowRate);

	// ���C�g�̎����`����I�t�ɂ���
	m_pLight->SetEnableDisp(false);

	// �ҋ@���[�V������Ԃ�
	return MOTION_IDOL;
}

//==========================================================================
// ����オ���Ԃ̍X�V����
//==========================================================================
int CAudienceAnim::UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ����オ���Ԃ̍X�V
	CAudience::UpdateJump(fDeltaTime, fDeltaRate, fSlowRate);

	// ���C�g�̎����`����I�t�ɂ���
	m_pLight->SetEnableDisp(false);

	// �W�����v���[�V������Ԃ�
	return MOTION_JUMP;
}

//==========================================================================
// �X�y�V������Ԃ̍X�V����
//==========================================================================
int CAudienceAnim::UpdateSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �X�y�V������Ԃ̍X�V
	CAudience::UpdateSpecial(fDeltaTime, fDeltaRate, fSlowRate);

	// ���C�g�̎����`����I���ɂ���
	m_pLight->SetEnableDisp(true);

	// ���C�g�̈ʒu�𓪂̏�ɂ���
	m_pLight->SetPosition(GetPosition() + MyLib::Vector3(0.0f, 230.0f, 0.0f));	// TODO�F��ŐU�����肳����

	// �W�����v���[�V������Ԃ�
	return MOTION_JUMP;
}

//==========================================================================
// �ޏ��Ԃ̍X�V����
//==========================================================================
int CAudienceAnim::UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ޏ��Ԃ̍X�V
	CAudience::UpdateDespawn(fDeltaTime, fDeltaRate, fSlowRate);

	// ���C�g�̎����`����I�t�ɂ���
	m_pLight->SetEnableDisp(false);

	// �ړ����[�V������Ԃ�
	return m_moveMotion;
}

//==========================================================================
// ���[�V�����̐ݒ菈��
//==========================================================================
void CAudienceAnim::SetMotion(const int nMotion)
{
	// �L�����N�^�[�A�j���[�V������񂪂Ȃ��ꍇ������
	if (m_pAnimChara == nullptr) { return; }

	int nAnimMotion = m_pAnimChara->GetMotion();	// ���ݍĐ����̃��[�V����
	if (m_pAnimChara->IsLoop())
	{ // ���[�v���郂�[�V�����������ꍇ

		if (nAnimMotion != nMotion)
		{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

			// ���݂̃��[�V�����̐ݒ�
			m_pAnimChara->SetMotion(nMotion);
		}
	}
}
