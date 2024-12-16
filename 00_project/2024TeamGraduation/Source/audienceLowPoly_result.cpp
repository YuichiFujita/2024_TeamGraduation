//==========================================================================
// 
//  �ϋq_���[�|���L�������U���g�w�b�_�[ [audienceLowPoly_result.cpp]
//  Author : Kai Takada
// 
//==========================================================================
#include "audienceLowPoly_result.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	namespace Side
	{// ��
		const int RANGE = 325;	// ���E�̃��C����
		const int MID_LEFT_LINE[]	= { 0.0f, RANGE };													// �`�[���T�C�h���Ƃ̒���(��)���C��
		const int MID_RIGHT_LINE[]	= { RANGE, 0.0f };													// �`�[���T�C�h���Ƃ̒���(�E)���C��
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CAudience::MAX_RIGHT_LINE - RANGE };	// �`�[���T�C�h���Ƃ̍����C��
		const int RIGHT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE + RANGE, (int)CAudience::MAX_RIGHT_LINE };	// �`�[���T�C�h���Ƃ̉E���C��
		const int NEAR_LINE		= -1060;	// ��O�̐����ʒu���
		const int FAR_LINE		= 1060;		// ���̐����ʒu���
	}

	namespace Far
	{// ��
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };	// �`�[���T�C�h���Ƃ̍����C��
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)CAudience::MAX_RIGHT_LINE };	// �`�[���T�C�h���Ƃ̉E���C��
		const int NEAR_LINE		= 1100;	// ��O�̐����ʒu���
		const int FAR_LINE		= 1300;	// ���̐����ʒu���
	}

	namespace Down
	{// ��O
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };	// �`�[���T�C�h���Ƃ̍����C��
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)CAudience::MAX_RIGHT_LINE };	// �`�[���T�C�h���Ƃ̉E���C��
		const int NEAR_LINE		= -1300;	// ��O�̐����ʒu���
		const int FAR_LINE		= -1100;	// ���̐����ʒu���
	}
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CAudienceLowPolyResult::CAudienceLowPolyResult(EObjType type, CGameManager::ETeamSide team) : CAudienceLowPoly(type, team)
{
	m_side = EAreaSide::SIDE_NONE;		// �ϐ�T�C�h
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CAudienceLowPolyResult::~CAudienceLowPolyResult()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CAudienceLowPolyResult::Init()
{
	// �e�N���X�̏�����
	CAudienceLowPoly::Init();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CAudienceLowPolyResult::Uninit()
{
	// �e�N���X�̏I��
	CAudienceLowPoly::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CAudienceLowPolyResult::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�N���X�̍X�V
	CAudienceLowPoly::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CAudienceLowPolyResult::Draw()
{
	// �e�N���X�̕`��
	CAudienceLowPoly::Draw();
}

//==========================================================================
// �ϐ�ʒu�̌v�Z���� (��)
//==========================================================================
void CAudienceLowPolyResult::CalcWatchPositionFar()
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
void CAudienceLowPolyResult::CalcWatchPositionSide()
{
	// �����_���ō��E����
	int nLR = UtilFunc::Transformation::Random(0, 1);

	// �����_���Ɋϐ�ʒu��ݒ�
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	if (nLR == 0)
	{// �[��
		posWatch.x = (float)UtilFunc::Transformation::Random(Side::LEFT_LINE[nIdxTeam], Side::RIGHT_LINE[nIdxTeam]);
	}
	else
	{// ������
		posWatch.x = (float)UtilFunc::Transformation::Random(Side::MID_LEFT_LINE[nIdxTeam], Side::MID_RIGHT_LINE[nIdxTeam]);
	
		m_side = EAreaSide::SIDE_REVERSE;
	}
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random(Side::NEAR_LINE, Side::FAR_LINE);
	SetWatchPosition(posWatch);
}

//==========================================================================
// �ϐ�ʒu�̌v�Z���� (��O)
//==========================================================================
void CAudienceLowPolyResult::CalcWatchPositionNear()
{
	// �����_���Ɋϐ�ʒu��ݒ�
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Down::LEFT_LINE[nIdxTeam], Down::RIGHT_LINE[nIdxTeam]);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random(Down::NEAR_LINE, Down::FAR_LINE);
	SetWatchPosition(posWatch);
}

//==========================================================================
// �X�|�[���I�����̐ݒ�
//==========================================================================
void CAudienceLowPolyResult::EndSettingSpawn()
{
	// �e�N���X
	CAudienceLowPoly::EndSettingSpawn();

	// �T�C�h���t�Ȃ�t������
	if (GetArea() == EArea::AREA_SIDE &&
		m_side == EAreaSide::SIDE_REVERSE)
	{
		MyLib::Vector3 rot = GetRotation();

		rot.y += D3DX_PI;
		UtilFunc::Transformation::RotNormalize(rot);

		// ���������Ɍ�����
		SetRotation(rot);
	}
}
