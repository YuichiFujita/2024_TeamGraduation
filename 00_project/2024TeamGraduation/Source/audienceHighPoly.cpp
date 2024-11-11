//==========================================================================
// 
//  �ϋq_�n�C�|���L�������� [audienceHighPoly.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "audienceHighPoly.h"
#include "gameManager.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* SETUP_TXT = "data\\TEXT\\character\\audience\\setup_player.txt";	// �v���C���[�Z�b�g�A�b�v�e�L�X�g
	const char* LIGHT_PATH = "data\\MODEL\\penlight.x";		// �y�����C�g�̃��f���p�X
	const int PRIORITY = mylib_const::PRIORITY_DEFAULT;		// �D�揇��

	const int LEFT_LINE[]  = { (int)CAudienceHighPoly::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };		// �`�[���T�C�h���Ƃ̍����C��
	const int RIGHT_LINE[] = { (int)CGameManager::CENTER_LINE, (int)CAudienceHighPoly::MAX_RIGHT_LINE };	// �`�[���T�C�h���Ƃ̉E���C��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CAudienceHighPoly::CAudienceHighPoly(EObjType type, CGameManager::TeamSide team) : CAudience(type, team, PRIORITY, CObject::LAYER_DEFAULT),
	m_pChara	(nullptr),	// �L�����N�^�[���
	m_pLight	(nullptr)	// �y�����C�g���
{
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CAudienceHighPoly::~CAudienceHighPoly()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CAudienceHighPoly::Init()
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
		fTurn = 1.0f;
	}
	else
	{
		// ������̓���ɂ���
		fTurn = -1.0f;
	}

	// �����ʒu��ݒ�
	MyLib::Vector3 posSpawn = posWatch;
	posSpawn.x = CAudience::SPAWN_SIDE_LINE * fTurn;
	SetSpawnPosition(posSpawn);

	// �I�u�W�F�N�g�L�����N�^�[�̐���
	if (FAILED(CreateCharacter(posSpawn)))
	{
		return E_FAIL;
	}
	m_pChara->SetRotation(MyLib::Vector3(0.0f, HALF_PI * fTurn, 0.0f));	// �����ݒ�

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	return S_OK;
}

//==========================================================================
// �L�����N�^�[����
//==========================================================================
HRESULT CAudienceHighPoly::CreateCharacter(const MyLib::Vector3& pos)
{
	// �I�u�W�F�N�g�L�����N�^�[�̐���
	m_pChara = CObjectChara::Create(SETUP_TXT);
	if (m_pChara == nullptr)
	{ // �����Ɏ��s�����ꍇ
		return E_FAIL;
	}

	// �ʒu�ݒ�
	m_pChara->SetPosition(pos);

	// ���[�V������ݒ�
	m_pChara->GetMotion()->Set(EMotion::MOTION_SPAWN);

	// �n�C�|���L�����̎����X�V/�����`��/�����j�������Ȃ���ނɂ���
	m_pChara->SetType(CObject::TYPE::TYPE_NONE);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CAudienceHighPoly::Uninit()
{
	// �e�N���X�̏I��
	CAudience::Uninit();

	// �I�u�W�F�N�g�L�����N�^�[�̏I��
	SAFE_UNINIT(m_pChara);
}

//==========================================================================
// �폜����
//==========================================================================
void CAudienceHighPoly::Kill()
{
	// �e�N���X�̍폜
	CAudience::Kill();

	// �I�u�W�F�N�g�L�����N�^�[�̏I��
	if (m_pChara != nullptr)
	{
		m_pChara->Kill();
		m_pChara = nullptr;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CAudienceHighPoly::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pChara != nullptr)
	{
		// �I�u�W�F�N�g�L�����N�^�[�̍X�V
		m_pChara->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// �e�N���X�̍X�V
	CAudience::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CAudienceHighPoly::Draw()
{
	// �e�N���X�̕`��
	CAudience::Draw();
}

//==========================================================================
// �����Ԃ̍X�V����
//==========================================================================
int CAudienceHighPoly::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����Ԃ̍X�V
	CAudience::UpdateSpawn(fDeltaTime, fDeltaRate, fSlowRate);

	// ���C�g�̎����`����I�t�ɂ���
	m_pLight->SetEnableDisp(false);

	// �ړ����[�V������Ԃ�
	return EMotion::MOTION_SPAWN;
}

//==========================================================================
// �ʏ��Ԃ̍X�V����
//==========================================================================
int CAudienceHighPoly::UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ʏ��Ԃ̍X�V
	CAudience::UpdateNormal(fDeltaTime, fDeltaRate, fSlowRate);

	// ���C�g�̎����`����I�t�ɂ���
	m_pLight->SetEnableDisp(false);

	// �ҋ@���[�V������Ԃ�
	return EMotion::MOTION_DEF;
}

//==========================================================================
// ����オ���Ԃ̍X�V����
//==========================================================================
int CAudienceHighPoly::UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
int CAudienceHighPoly::UpdateSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
int CAudienceHighPoly::UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ޏ��Ԃ̍X�V
	CAudience::UpdateDespawn(fDeltaTime, fDeltaRate, fSlowRate);

	// ���C�g�̎����`����I�t�ɂ���
	m_pLight->SetEnableDisp(false);

	// �ړ����[�V������Ԃ�
	return EMotion::MOTION_DESPAWN;
}

//==========================================================================
// �ޏ�̐ݒ菈��
//==========================================================================
bool CAudienceHighPoly::SetDespawn()
{
	// �ޏ�ʒu���쐬
	MyLib::Vector3 posDespawn = GetSpawnPosition();	// �ޏ�ʒu
	posDespawn.x = -GetSpawnPosition().x;			// X���W�𔽓]������

	// ������ޏ������
	SetRotation(MyLib::Vector3(0.0f, MyLib::Vector3().AngleXZ(posDespawn), 0.0f));

	// �ޏ�̐ݒ菈��
	return CAudience::SetDespawn();
}

//==========================================================================
// ���[�V�����̐ݒ菈��
//==========================================================================
void CAudienceHighPoly::SetMotion(const int nMotion)
{
	// �L�����N�^�[��񂪂Ȃ��ꍇ������
	if (m_pChara == nullptr) { return; }
	CMotion* pMotion = m_pChara->GetMotion();

	int nAnimMotion = pMotion->GetType();	// ���ݍĐ����̃��[�V����
	if (nAnimMotion != nMotion)
	{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

		// ���݂̃��[�V�����̐ݒ�
		pMotion->Set(nMotion);
	}
}

//==========================================================================
// �X�|�[���I�����̐ݒ�
//==========================================================================
void CAudienceHighPoly::EndSettingSpawn()
{
	// �L�����N�^�[��񂪂Ȃ��ꍇ������
	if (m_pChara == nullptr) { return; }

	// �����𐳖ʂɌ�����
	m_pChara->SetRotation(MyLib::Vector3(0.0f, UtilFunc::Transformation::Random(-100, 100) * 0.001f, 0.0f));
}
