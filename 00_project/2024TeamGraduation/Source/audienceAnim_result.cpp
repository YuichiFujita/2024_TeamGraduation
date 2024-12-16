//==========================================================================
// 
//  �ϋq_�A�j���[�V����3D���U���g�w�b�_�[ [audienceAnim_result.cpp]
//  Author : Kai Takada
// 
//==========================================================================
#include "audienceAnim_result.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
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
		const float LANDY		= 545.0f;	// ���nY���W
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
// �R���X�g���N�^
//==========================================================================
CAudienceAnimResult::CAudienceAnimResult(EObjType type, CGameManager::ETeamSide team) : CAudienceAnim(type, team)
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CAudienceAnimResult::~CAudienceAnimResult()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CAudienceAnimResult::Init()
{
	// �e�N���X�̏�����
	CAudienceAnim::Init();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CAudienceAnimResult::Uninit()
{
	// �e�N���X�̏I��
	CAudienceAnim::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CAudienceAnimResult::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�N���X�̍X�V
	CAudienceAnim::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CAudienceAnimResult::Draw()
{
	// �e�N���X�̕`��
	CAudienceAnim::Draw();
}

//==========================================================================
// �ϐ�ʒu�̌v�Z���� (��)
//==========================================================================
void CAudienceAnimResult::CalcWatchPositionFar()
{
	// �����_���Ɋϐ�ʒu��ݒ�
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Far::LEFT_LINE[nIdxTeam], Far::RIGHT_LINE[nIdxTeam]);
	posWatch.y = GetLandY();
	posWatch.z = (float)UtilFunc::Transformation::Random(Far::NEAR_LINE, Far::FAR_LINE);
	SetWatchPosition(posWatch);

	// ���[�V�����̐ݒ�
	SetIdolMotion(MOTION_IDOL_D);	// ���ҋ@���[�V����
	SetJumpMotion(MOTION_JUMP_D);	// ���W�����v���[�V����
}

//==========================================================================
// �ϐ�ʒu�̌v�Z���� (��)
//==========================================================================
void CAudienceAnimResult::CalcWatchPositionUp()
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
	SetIdolMotion(MOTION_IDOL_D);	// ���ҋ@���[�V����
	SetJumpMotion(MOTION_JUMP_D);	// ���W�����v���[�V����
}

//==========================================================================
// �ϐ�ʒu�̌v�Z���� (��O)
//==========================================================================
void CAudienceAnimResult::CalcWatchPositionNear()
{
	// �����_���Ɋϐ�ʒu��ݒ�
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Down::LEFT_LINE[nIdxTeam], Down::RIGHT_LINE[nIdxTeam]);
	posWatch.y = GetLandY();
	posWatch.z = (float)UtilFunc::Transformation::Random(Down::NEAR_LINE, Down::FAR_LINE);
	SetWatchPosition(posWatch);

	// ���[�V�����̐ݒ�
	SetIdolMotion(MOTION_IDOL_U);	// ��ҋ@���[�V����
	SetJumpMotion(MOTION_JUMP_U);	// ��W�����v���[�V����
}
