//==========================================================================
// 
//  �ϋq_�n�C�|���L�������U���g�w�b�_�[ [audienceHighPoly_result.cpp]
//  Author : Kai Takada
// 
//==========================================================================
#include "audienceHighPoly_result.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	namespace Side
	{
		const int RANGE = 325;	// ���E�̃��C����
		const int MID_LEFT_LINE[] = { RANGE * 0.5f, 0.0f };		// �`�[���T�C�h���Ƃ̒���(��)���C��
		const int MID_RIGHT_LINE[] = { 0.0f, RANGE * 0.5f };	// �`�[���T�C�h���Ƃ̒���(�E)���C��
		const int LEFT_LINE[]	= { -1675, 1350 };				// �`�[���T�C�h���Ƃ̍����C��
		const int RIGHT_LINE[]	= { -1350, 1675 };				// �`�[���T�C�h���Ƃ̉E���C��
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
// �R���X�g���N�^
//==========================================================================
CAudienceHighPolyResult::CAudienceHighPolyResult(CAudienceHighPoly::EObjType type, CGameManager::ETeamSide team) : CAudienceHighPoly(type, team)
{
	m_side = EAreaSide::SIDE_NONE;		// �ϐ�T�C�h
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CAudienceHighPolyResult::~CAudienceHighPolyResult()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CAudienceHighPolyResult::Init()
{
	// �e�N���X�̏�����
	CAudienceHighPoly::Init();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CAudienceHighPolyResult::Uninit()
{
	// �e�N���X�̏I��
	CAudienceHighPoly::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CAudienceHighPolyResult::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�N���X�̍X�V
	CAudienceHighPoly::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CAudienceHighPolyResult::Draw()
{
	// �e�N���X�̕`��
	CAudienceHighPoly::Draw();
}

//==========================================================================
// �ϐ�ʒu�̌v�Z���� (��)
//==========================================================================
void CAudienceHighPolyResult::CalcWatchPositionFar()
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
void CAudienceHighPolyResult::CalcWatchPositionSide()
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
void CAudienceHighPolyResult::CalcWatchPositionNear()
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
void CAudienceHighPolyResult::EndSettingSpawn()
{
	// �e�N���X
	CAudienceHighPoly::EndSettingSpawn();

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
