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

	namespace Far
	{
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };	// �`�[���T�C�h���Ƃ̍����C��
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)CAudience::MAX_RIGHT_LINE };	// �`�[���T�C�h���Ƃ̉E���C��
		const int NEAR_LINE		= 1360;	// ��O�̐����ʒu���
		const int FAR_LINE		= 1700;	// ���̐����ʒu���
	}

	namespace Up
	{
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };	// �`�[���T�C�h���Ƃ̍����C��
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)CAudience::MAX_RIGHT_LINE };	// �`�[���T�C�h���Ƃ̉E���C��
		const int NEAR_LINE		= 1700;		// ��O�̐����ʒu���
		const int FAR_LINE		= 1900;		// ���̐����ʒu���
		const float LANDY		= 540.0f;	// ���nY���W
	}

	namespace Down
	{
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };	// �`�[���T�C�h���Ƃ̍����C��
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)CAudience::MAX_RIGHT_LINE };	// �`�[���T�C�h���Ƃ̉E���C��
		const int NEAR_LINE		= -1700;	// ��O�̐����ʒu���
		const int FAR_LINE		= -1360;	// ���̐����ʒu���
	}
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
// �ϐ�ʒu�v�Z���X�g
CAudienceAnim::WATCH_POS_FUNC CAudienceAnim::m_CalcWatchPositionFunc[] =
{
	&CAudienceAnim::CalcWatchPositionFar,	// ���ϐ�ʒu
	&CAudienceAnim::CalcWatchPositionUp,	// ��ϐ�ʒu
	&CAudienceAnim::CalcWatchPositionNear,	// ��O�ϐ�ʒu
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CAudienceAnim::CAudienceAnim(EObjType type, CGameManager::ETeamSide team) : CAudience(type, team, PRIORITY, CObject::LAYER_DEFAULT),
	m_pAnimChara	(nullptr),			// �L�����N�^�[�A�j���[�V�������
	m_pLight		(nullptr),			// �y�����C�g���
	m_idolMotion	(MOTION_IDOL_U),	// �ҋ@���[�V����
	m_jumpMotion	(MOTION_IDOL_U),	// �W�����v���[�V����
	m_moveMotion	(MOTION_IDOL_U)		// �ړ����[�V����
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

	// �ϐ�G���A�̐ݒ�
	SetArea((int)(rand() % EArea::AREA_MAX));

	// �ϐ�ʒu�̐ݒ�
	(this->*(m_CalcWatchPositionFunc[GetArea()]))();

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

	// �����ʒu�̐ݒ�
	MyLib::Vector3 posSpawn = GetWatchPosition();
	posSpawn.x = CAudience::SPAWN_SIDE_LINE * fTurn;
	SetSpawnPosition(posSpawn);

	// �ޏ�ʒu�̐ݒ�
	MyLib::Vector3 posDespawn = posSpawn;
	posDespawn.x = -posSpawn.x;
	SetDespawnPosition(posDespawn);

	// �A�j���[�V�����L�����N�^�[�̐���
	if (FAILED(CreateAnimCharacter(posSpawn)))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �y�����C�g�̐���
	if (FAILED(CreatePenLight()))
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
	return m_idolMotion;
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
	return m_jumpMotion;
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
	return m_jumpMotion;
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

//==========================================================================
// �L�����N�^�[����
//==========================================================================
HRESULT CAudienceAnim::CreateAnimCharacter(const MyLib::Vector3& rPos)
{
	// �I�u�W�F�N�g�L�����N�^�[�A�j���[�V�����̐���
	m_pAnimChara = CObjectCharaAnim::Create(rPos);
	if (m_pAnimChara == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �L�����N�^�[���̊���
	m_pAnimChara->BindCharaData(SETUP_TXT);

	// ���[�V�����̐ݒ�
	m_pAnimChara->SetMotion(m_moveMotion);

	// �A�j���[�V����3D�̎����X�V/�����`��/�����j�������Ȃ���ނɂ���
	m_pAnimChara->SetType(CObject::TYPE::TYPE_NONE);

	return S_OK;
}

//==========================================================================
// �y�����C�g����
//==========================================================================
HRESULT CAudienceAnim::CreatePenLight()
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
void CAudienceAnim::CalcWatchPositionFar()
{
	// �����_���Ɋϐ�ʒu��ݒ�
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Far::LEFT_LINE[nIdxTeam], Far::RIGHT_LINE[nIdxTeam]);
	posWatch.y = GetLandY();
	posWatch.z = (float)UtilFunc::Transformation::Random(Far::NEAR_LINE, Far::FAR_LINE);
	SetWatchPosition(posWatch);

	// ���[�V�����̐ݒ�
	m_idolMotion = MOTION_IDOL_D;	// ���ҋ@���[�V����
	m_jumpMotion = MOTION_JUMP_D;	// ���W�����v���[�V����
}

//==========================================================================
// �ϐ�ʒu�̌v�Z���� (��)
//==========================================================================
void CAudienceAnim::CalcWatchPositionUp()
{
	// ���nY���W�̐ݒ�
	SetLandY(Up::LANDY);

	// �����_���Ɋϐ�ʒu��ݒ�
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Up::LEFT_LINE[nIdxTeam], Up::RIGHT_LINE[nIdxTeam]);
	posWatch.y = GetLandY();
	posWatch.z = (float)UtilFunc::Transformation::Random(Up::NEAR_LINE, Up::FAR_LINE);
	SetWatchPosition(posWatch);

	// ���[�V�����̐ݒ�
	m_idolMotion = MOTION_IDOL_D;	// ���ҋ@���[�V����
	m_jumpMotion = MOTION_JUMP_D;	// ���W�����v���[�V����
}

//==========================================================================
// �ϐ�ʒu�̌v�Z���� (��O)
//==========================================================================
void CAudienceAnim::CalcWatchPositionNear()
{
	// �����_���Ɋϐ�ʒu��ݒ�
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Down::LEFT_LINE[nIdxTeam], Down::RIGHT_LINE[nIdxTeam]);
	posWatch.y = GetLandY();
	posWatch.z = (float)UtilFunc::Transformation::Random(Down::NEAR_LINE, Down::FAR_LINE);
	SetWatchPosition(posWatch);

	// ���[�V�����̐ݒ�
	m_idolMotion = MOTION_IDOL_U;	// ��ҋ@���[�V����
	m_jumpMotion = MOTION_JUMP_U;	// ��W�����v���[�V����
}
