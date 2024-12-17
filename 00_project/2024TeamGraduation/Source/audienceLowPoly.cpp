//==========================================================================
// 
//  �ϋq_���[�|���L�������� [audienceLowPoly.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "audienceLowPoly.h"
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

	namespace Side
	{
		const int LEFT_LINE[]	= { -2000, 1675 };	// �`�[���T�C�h���Ƃ̍����C��
		const int RIGHT_LINE[]	= { -1675, 2000 };	// �`�[���T�C�h���Ƃ̉E���C��
		const int NEAR_LINE		= -1060;	// ��O�̐����ʒu���
		const int FAR_LINE		= 1060;		// ���̐����ʒu���
	}

	namespace Far
	{
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };	// �`�[���T�C�h���Ƃ̍����C��
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)CAudience::MAX_RIGHT_LINE };	// �`�[���T�C�h���Ƃ̉E���C��
		const int NEAR_LINE		= 1100;	// ��O�̐����ʒu���
		const int FAR_LINE		= 1300;	// ���̐����ʒu���
	}

	namespace Down
	{
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };	// �`�[���T�C�h���Ƃ̍����C��
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)CAudience::MAX_RIGHT_LINE };	// �`�[���T�C�h���Ƃ̉E���C��
		const int NEAR_LINE		= -1300;	// ��O�̐����ʒu���
		const int FAR_LINE		= -1100;	// ���̐����ʒu���
	}
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
// �ϐ�ʒu�v�Z���X�g
CAudienceLowPoly::WATCH_POS_FUNC CAudienceLowPoly::m_CalcWatchPositionFunc[] =
{
	&CAudienceLowPoly::CalcWatchPositionFar,	// ���ϐ�ʒu
	&CAudienceLowPoly::CalcWatchPositionSide,	// ���ϐ�ʒu
	&CAudienceLowPoly::CalcWatchPositionNear,	// ��O�ϐ�ʒu
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CAudienceLowPoly::CAudienceLowPoly(EObjType type, CGameManager::ETeamSide team) : CAudience(type, team, PRIORITY, CObject::LAYER_DEFAULT),
	m_pChara	(nullptr),	// �L�����N�^�[���
	m_pLight	(nullptr)	// �y�����C�g���
{
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CAudienceLowPoly::~CAudienceLowPoly()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CAudienceLowPoly::Init()
{
	// �e�N���X�̏�����
	if (FAILED(CAudience::Init()))
	{ // �������Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �ϐ�G���A�̐ݒ�
	SetArea((int)(rand() % EArea::AREA_MAX));

	// �ϐ�ʒu��ݒ肵�A���������擾
	(this->*(m_CalcWatchPositionFunc[GetArea()]))();

	// �`�[�����Ƃɐ���������ݒ�
	float fTurn = 0.0f;		// �����W��
	if ((bool)GetTeam())	{ fTurn =  1.0f; }	// �E����̓���ɂ���
	else					{ fTurn = -1.0f; }	// ������̓���ɂ���

	// ����/�ޏ�ʒu�̐ݒ�
	MyLib::Vector3 posSpawn = GetWatchPosition();
	posSpawn.x = CAudience::SPAWN_SIDE_LINE * fTurn;
	SetSpawnPosition(posSpawn);		// �����ʒu
	SetDespawnPosition(posSpawn);	// �ޏ�ʒu

	// �I�u�W�F�N�g�L�����N�^�[�̐���
	if (FAILED(CreateCharacter(posSpawn, MyLib::Vector3(0.0f, HALF_PI * fTurn, 0.0f))))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �y�����C�g�̐���
	if (FAILED(CreatePenLight()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �e�̐���
	if (FAILED(CreateShadow(this)))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CAudienceLowPoly::Uninit()
{
	// �e�N���X�̏I��
	CAudience::Uninit();

	// �I�u�W�F�N�g�L�����N�^�[�̏I��
	SAFE_UNINIT(m_pChara);
}

//==========================================================================
// �폜����
//==========================================================================
void CAudienceLowPoly::Kill()
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
void CAudienceLowPoly::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CAudienceLowPoly::Draw()
{
	if (m_pChara != nullptr)
	{
		// TODO�F���[�|�����ʗp
		// �I�u�W�F�N�g�L�����N�^�[�̕`��
#if 0
		m_pChara->Draw();
#else
		m_pChara->Draw(MyLib::color::Blue());
#endif
	}

	// �e�N���X�̕`��
	CAudience::Draw();
}

//==========================================================================
// �����Ԃ̍X�V����
//==========================================================================
int CAudienceLowPoly::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
int CAudienceLowPoly::UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
int CAudienceLowPoly::UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ����オ���Ԃ̍X�V
	CAudience::UpdateJump(fDeltaTime, fDeltaRate, fSlowRate);

	// ���C�g�̎����`����I�t�ɂ���
	m_pLight->SetEnableDisp(false);

	// �W�����v���[�V������Ԃ�
	return EMotion::MOTION_JUMP;
}

//==========================================================================
// �X�y�V������Ԃ̍X�V����
//==========================================================================
int CAudienceLowPoly::UpdateSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �X�y�V������Ԃ̍X�V
	CAudience::UpdateSpecial(fDeltaTime, fDeltaRate, fSlowRate);

	// ���C�g�̎����`����I���ɂ���
	m_pLight->SetEnableDisp(true);

	// ���C�g�̈ʒu�𓪂̏�ɂ���
	m_pLight->SetPosition(GetPosition() + MyLib::Vector3(0.0f, 230.0f, 0.0f));	// TODO�F��ŐU�����肳����

	// �W�����v���[�V������Ԃ�
	return EMotion::MOTION_JUMP;
}

//==========================================================================
// �ޏ��Ԃ̍X�V����
//==========================================================================
int CAudienceLowPoly::UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
bool CAudienceLowPoly::SetDespawn(EObjType type)
{
	// �ޏ�̐ݒ�
	bool bDespawn = CAudience::SetDespawn(type);
	if (bDespawn)
	{ // �ޏꂵ���ꍇ

		// �ޏ�ʒu���쐬
		MyLib::Vector3 posDespawn = GetSpawnPosition();	// �ޏ�ʒu

		// ������ޏ������
		SetRotation(MyLib::Vector3(0.0f, MyLib::Vector3().AngleXZ(posDespawn), 0.0f));
	}

	// �ޏ�t���O��Ԃ�
	return bDespawn;
}

//==========================================================================
// NTR�̐ݒ菈��
//==========================================================================
bool CAudienceLowPoly::SetNTR()
{
	// �����Ԃɂ���
	CAudience::SetNTR();

	// ����J�n�ʒu��ۑ�
	SetSpawnPosition(GetPosition());	// ���݂̈ʒu

	// �ϐ�ʒu��ݒ肵�A���������擾
	(this->*(m_CalcWatchPositionFunc[GetArea()]))();

	return true;
}

//==========================================================================
// ���[�V�����̐ݒ菈��
//==========================================================================
void CAudienceLowPoly::SetMotion(const int nMotion)
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
void CAudienceLowPoly::EndSettingSpawn()
{
	// �L�����N�^�[��񂪂Ȃ��ꍇ������
	if (m_pChara == nullptr) { return; }

	const float fGapRotY = UtilFunc::Transformation::Random(-100, 100) * 0.001f;	// �����̃u��
	switch (GetArea())
	{ // �ϐ�G���A���Ƃ̏���
	case EArea::AREA_FAR:
	{
		// �����𐳖ʂɌ�����
		m_pChara->SetRotation(MyLib::Vector3(0.0f, fGapRotY, 0.0f));
		break;
	}
	case EArea::AREA_SIDE:
	{
		float fTurn = 0.0f;		// �����W��
		if ((bool)GetTeam())	{ fTurn =  1.0f; }	// �E����̓���ɂ���
		else					{ fTurn = -1.0f; }	// ������̓���ɂ���

		// ���������Ɍ�����
		m_pChara->SetRotation(MyLib::Vector3(0.0f, (HALF_PI * fTurn) - fGapRotY, 0.0f));
		break;
	}
	case EArea::AREA_NEAR:
	{
		// ���������Ɍ�����
		m_pChara->SetRotation(MyLib::Vector3(0.0f, D3DX_PI - fGapRotY, 0.0f));
		break;
	}
	default:
		assert(false);
		break;
	};
}

//==========================================================================
// �L�����N�^�[����
//==========================================================================
HRESULT CAudienceLowPoly::CreateCharacter(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot)
{
	// �I�u�W�F�N�g�L�����N�^�[�̐���
	m_pChara = CObjectChara::Create(SETUP_TXT);
	if (m_pChara == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �ʒu�̐ݒ�
	m_pChara->SetPosition(rPos);

	// �����̐ݒ�
	m_pChara->SetRotation(rRot);

	// ���[�V�����̐ݒ�
	m_pChara->GetMotion()->Set(EMotion::MOTION_SPAWN);

	// ���[�|���L�����̎����X�V/�����`��/�����j�������Ȃ���ނɂ���
	m_pChara->SetType(CObject::TYPE::TYPE_NONE);

	return S_OK;
}

//==========================================================================
// �y�����C�g����
//==========================================================================
HRESULT CAudienceLowPoly::CreatePenLight()
{
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

	return S_OK;
}

//==========================================================================
// �ϐ�ʒu�̌v�Z���� (��)
//==========================================================================
void CAudienceLowPoly::CalcWatchPositionFar()
{
	// �����_���Ɋϐ�ʒu��ݒ�
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Far::LEFT_LINE[nIdxTeam], Far::RIGHT_LINE[nIdxTeam]);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random(Far::NEAR_LINE, Far::FAR_LINE);
	SetWatchPosition(posWatch);
}

//==========================================================================
// �ϐ�ʒu�̌v�Z���� (��)
//==========================================================================
void CAudienceLowPoly::CalcWatchPositionSide()
{
	// �����_���Ɋϐ�ʒu��ݒ�
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Side::LEFT_LINE[nIdxTeam], Side::RIGHT_LINE[nIdxTeam]);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random(Side::NEAR_LINE, Side::FAR_LINE);
	SetWatchPosition(posWatch);
}

//==========================================================================
// �ϐ�ʒu�̌v�Z���� (��O)
//==========================================================================
void CAudienceLowPoly::CalcWatchPositionNear()
{
	// �����_���Ɋϐ�ʒu��ݒ�
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Down::LEFT_LINE[nIdxTeam], Down::RIGHT_LINE[nIdxTeam]);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random(Down::NEAR_LINE, Down::FAR_LINE);
	SetWatchPosition(posWatch);
}
