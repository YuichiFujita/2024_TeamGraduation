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
#include "model.h"
#include "dressup.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* SETUP_TXT = "data\\TEXT\\character\\audience\\setup_player.txt";	// �v���C���[�Z�b�g�A�b�v�e�L�X�g
	const char* LIGHT_PATH = "data\\MODEL\\penlight.x";		// �y�����C�g�̃��f���p�X
	const int PRIORITY = mylib_const::PRIORITY_DEFAULT;		// �D�揇��
	const int ID_HAIR = 9;	// ����ID

	namespace Side
	{
		const int LEFT_LINE[]	= { -1675, 1350 };	// �`�[���T�C�h���Ƃ̍����C��
		const int RIGHT_LINE[]	= { -1350, 1675 };	// �`�[���T�C�h���Ƃ̉E���C��
		const int NEAR_LINE		= -960;	// ��O�̐����ʒu���
		const int FAR_LINE		= 960;	// ���̐����ʒu���
	}

	namespace Far
	{
		const int LEFT_LINE[]	= { (int)Side::LEFT_LINE[CGameManager::ETeamSide::SIDE_LEFT], (int)CGameManager::CENTER_LINE };		// �`�[���T�C�h���Ƃ̍����C��
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)Side::RIGHT_LINE[CGameManager::ETeamSide::SIDE_RIGHT] };	// �`�[���T�C�h���Ƃ̉E���C��
		const int NEAR_LINE		= 1000;	// ��O�̐����ʒu���
		const int FAR_LINE		= 1100;	// ���̐����ʒu���
	}

	namespace Down
	{
		const int LEFT_LINE[]	= { (int)Side::LEFT_LINE[CGameManager::ETeamSide::SIDE_LEFT], (int)CGameManager::CENTER_LINE };		// �`�[���T�C�h���Ƃ̍����C��
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)Side::RIGHT_LINE[CGameManager::ETeamSide::SIDE_RIGHT] };	// �`�[���T�C�h���Ƃ̉E���C��
		const int NEAR_LINE		= -1100;	// ��O�̐����ʒu���
		const int FAR_LINE		= -1000;	// ���̐����ʒu���
	}
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
// �ϐ�ʒu�v�Z���X�g
CAudienceHighPoly::WATCH_POS_FUNC CAudienceHighPoly::m_CalcWatchPositionFunc[] =
{
	&CAudienceHighPoly::CalcWatchPositionFar,	// ���ϐ�ʒu
	&CAudienceHighPoly::CalcWatchPositionSide,	// ���ϐ�ʒu
	&CAudienceHighPoly::CalcWatchPositionNear,	// ��O�ϐ�ʒu
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CAudienceHighPoly::CAudienceHighPoly(EObjType type, CGameManager::ETeamSide team) : CAudience(type, team, PRIORITY, CObject::LAYER_DEFAULT),
	m_pChara		(nullptr),	// �L�����N�^�[���
	m_pLight		(nullptr),	// �y�����C�g���
	m_pLightBlur	(nullptr),	// �y�����C�g�̃u���[
	m_pDressUp_Hair	(nullptr),	// �h���X�A�b�v(��)
	m_nIdxLightHand	(UtilFunc::Transformation::Random(3, 4))	// ���C�g����̃C���f�b�N�X
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

	// �y�����C�g�̃u���[����
	if (FAILED(CreatePenLightBlur()))
	{ // �����Ɏ��s�����ꍇ
		return E_FAIL;
	}

	// �e�̐���
	if (FAILED(CreateShadow(this)))
	{ // �����Ɏ��s�����ꍇ
		return E_FAIL;
	}

	// �h���X�A�b�v(��)
	m_pDressUp_Hair = CDressup::Create(
		CDressup::EType::TYPE_HAIR_MII,		// �����ւ��̎��
		m_pChara,						// �ύX����v���C���[
		ID_HAIR);						// �ύX�ӏ��̃C���f�b�N�X
	m_pDressUp_Hair->RandSet();

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CAudienceHighPoly::Uninit()
{
	// �h���X�A�b�v�폜
	SAFE_UNINIT(m_pDressUp_Hair);

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
// �y�����C�g�̍X�V
//==========================================================================
void CAudienceHighPoly::UpdatePenlight(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pChara == nullptr) return;
	if (m_pLight == nullptr) return;

	// ���C�g�̈ʒu�𓪂̏�ɂ���


	// ���肷��p�[�c�擾
	CModel* pModel = m_pChara->GetModel(m_nIdxLightHand);

	// ���肷��p�[�c�̃}�g���b�N�X�擾
	MyLib::Matrix mtxModel = pModel->GetWorldMtx();

	// �}�g���b�N�X�X�V���Ȃ�
	m_pLight->SetEnableUpdateMtx(false);

	// �y�����C�g�̃}�g���b�N�X�v�Z
	MyLib::Vector3 pos = m_pLight->GetPosition();
	MyLib::Vector3 rot = m_pLight->GetRotation();
	MyLib::Matrix mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	// �I�t�Z�b�g�𔽉f����
	mtxTrans.Translation(MyLib::Vector3((m_nIdxLightHand == 3) ? -40.0f : 40.0f, m_pLight->GetVtxMax().y, 0.0f));
	mtxModel.Multiply(mtxTrans, mtxModel);

	// �c�X�P�[���k��
	m_pLight->SetWorldMtx(mtxModel);

	// ���C�g�̈ʒu�ݒ�
	m_pLight->SetPosition(mtxModel.GetWorldPosition());	// TODO�F��ŐU�����肳����
	m_pLightBlur->SetPosition(m_pLight->GetPosition());
}

//==========================================================================
// �`�揈��
//==========================================================================
void CAudienceHighPoly::Draw()
{
	if (m_pChara != nullptr)
	{
		// �I�u�W�F�N�g�L�����N�^�[�̕`��
		m_pChara->Draw();
	}

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
	m_pLightBlur->SetEnableDisp(false);

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
	m_pLightBlur->SetEnableDisp(false);

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
	m_pLightBlur->SetEnableDisp(false);

	// �W�����v���[�V������Ԃ�
	return EMotion::MOTION_JUMP;
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
	m_pLightBlur->SetEnableDisp(true);

	// ���C�g�̈ʒu�𓪂̏�ɂ���
	m_pLight->SetPosition(GetPosition() + MyLib::Vector3(0.0f, 230.0f, 0.0f));	// TODO�F��ŐU�����肳����
	m_pLightBlur->SetPosition(m_pLight->GetPosition());

	// �W�����v���[�V������Ԃ�
	return EMotion::MOTION_SPECIAL;
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
	m_pLightBlur->SetEnableDisp(false);

	// �ړ����[�V������Ԃ�
	return EMotion::MOTION_DESPAWN;
}

//==========================================================================
// �ޏ�̐ݒ菈��
//==========================================================================
bool CAudienceHighPoly::SetDespawn(EObjType type)
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
bool CAudienceHighPoly::SetNTR(CGameManager::ETeamSide team)
{
	// �����Ԃɂ���
	CAudience::SetNTR(team);

	// ����J�n�ʒu��ۑ�
	SetSpawnPosition(GetPosition());	// ���݂̈ʒu

	// �ϐ�ʒu��ݒ肵�A���������擾
	(this->*(m_CalcWatchPositionFunc[GetArea()]))();

	return true;
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

		switch (nMotion)
		{
		case EMotion::MOTION_SPAWN:
		case EMotion::MOTION_JUMP:
		case EMotion::MOTION_DESPAWN:
		case EMotion::MOTION_SPECIAL:
		{
			// �ݒ肷�郂�[�V�����̏��擾
			const CMotionManager::Info& info = pMotion->GetInfo(nMotion);

			// �J�n�L�[
			int nStartKey = 0;
			if (rand() % 2 == 0)
			{
				nStartKey = info.nNumKey / 2;
			}

			// �J�n�t���[��
			float fFrame = (float)(rand() % info.aKey[nStartKey].nFrame);

			// ���݂̃��[�V�����̐ݒ�
			pMotion->Set(nMotion, nStartKey, true, fFrame);
		}
		break;

		case EMotion::MOTION_DEF:
		{
			// �j�u�C�`�ŋt
			int nSetMotion = nMotion;
			if (rand() % 2 == 0)
			{
				nSetMotion = EMotion::MOTION_DEF_INV;
			}

			// �ݒ肷�郂�[�V�����̏��擾
			const CMotionManager::Info& info = pMotion->GetInfo(nSetMotion);

			// �J�n�L�[
			int nStartKey = rand() % info.nNumKey;

			// �J�n�t���[��
			float fFrame = (float)(rand() % info.aKey[nStartKey].nFrame);

			// ���݂̃��[�V�����̐ݒ�
			pMotion->Set(nSetMotion, nStartKey, true, fFrame);
		}
			break;

		default:
			// ���݂̃��[�V�����̐ݒ�
			pMotion->Set(nMotion);
			break;
		}
	}
}

//==========================================================================
// �X�|�[���I�����̐ݒ�
//==========================================================================
void CAudienceHighPoly::EndSettingSpawn()
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
HRESULT CAudienceHighPoly::CreateCharacter(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot)
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

	// �X�P�[�����������_��
	m_pChara->SetScale(1.0f + UtilFunc::Transformation::Random(0, 250) * 0.001f);

	// ���[�V�����̐ݒ�
	SetMotion(EMotion::MOTION_SPAWN);

	// �n�C�|���L�����̎����X�V/�����`��/�����j�������Ȃ���ނɂ���
	m_pChara->SetType(CObject::TYPE::TYPE_NONE);

	return S_OK;
}

//==========================================================================
// �y�����C�g����
//==========================================================================
HRESULT CAudienceHighPoly::CreatePenLight()
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
// �y�����C�g�̃u���[����
//==========================================================================
HRESULT CAudienceHighPoly::CreatePenLightBlur()
{
	// �y�����C�g�̐���
	m_pLightBlur = CEffect3D::Create(
		MyLib::Vector3(0.0f, 0.0f, 0.0f),	// �ʒu
		MyLib::Vector3(0.0f, 0.0f, 0.0f),	// �ړ���
		D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f),	// �F
		200.0f,								// ���a
		0,									// ����
		CEffect3D::MOVEEFFECT_NONE,			// �ړ��̎��
		CEffect3D::TYPE_POINT);				// �G�t�F�N�g�̎��
	if (m_pLightBlur == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �����`��t���O���I�t�ɂ���
	m_pLightBlur->SetEnableDisp(false);

	// �����폜OFF
	m_pLightBlur->SetEnableDeleteLife(false);
	return S_OK;
}

//==========================================================================
// �ϐ�ʒu�̌v�Z���� (��)
//==========================================================================
void CAudienceHighPoly::CalcWatchPositionFar()
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
void CAudienceHighPoly::CalcWatchPositionSide()
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
void CAudienceHighPoly::CalcWatchPositionNear()
{
	// �����_���Ɋϐ�ʒu��ݒ�
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Down::LEFT_LINE[nIdxTeam], Down::RIGHT_LINE[nIdxTeam]);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random(Down::NEAR_LINE, Down::FAR_LINE);
	SetWatchPosition(posWatch);
}
