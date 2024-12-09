//=============================================================================
// 
//  �����҃Z�b�g���� [reporterSet.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "reporterSet.h"
#include "manager.h"
#include "calculation.h"
#include "schoolnurse.h"
#include "player.h"
#include "shadow.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string MODEL_DESK = "data\\MODEL\\komono\\Livedesk.x";	// ������
	const std::string MODEL_CHAIR = "data\\MODEL\\komono\\Liveseat.x";	// ������
	const std::string MODEL_MIC = "data\\MODEL\\komono\\stamdmic.x";	// �����}�C�N
	const float VELOCITY_MOVE = 10.0f;	// �ړ����x
	const MyLib::Vector3 STARTPOS = MyLib::Vector3(-3000.0f, 0.0f, 0.0f);	// �J�n�n�_
	const float DISTANCE_UP = 50.0f;	// �ςފԊu
}

namespace OffSet
{
	const MyLib::Vector3 DESK = MyLib::Vector3(0.0f, 0.0f, -44.0f);	// �����猩���֎q�̃I�t�Z�b�g
	const MyLib::Vector3 CHAIR = MyLib::Vector3(0.0f, 0.0f, 60.0f);	// �����猩���֎q�̃I�t�Z�b�g
	const MyLib::Vector3 MIC = MyLib::Vector3(0.0f, 73.0f, 0.0f);	// �����猩���}�C�N�̃I�t�Z�b�g
}

namespace StateTime
{
	const float GO = 1.0f;		// ������
	const float BACK = 2.0f;	// �߂�
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CReporterSet::STATE_FUNC CReporterSet::m_StateFunc[] =	// ��Ԋ֐�
{
	&CReporterSet::StateNone,		// �Ȃ�
	&CReporterSet::StateGo,		// ������
	&CReporterSet::StateCollect,	// ���
	&CReporterSet::StateBack,		// �߂�
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CReporterSet::CReporterSet(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_fStateTime = 0.0f;			// ��ԃJ�E���^�[
	m_state = EState::STATE_NONE;	// ���

	// ���̑��ϐ�
	m_pShadow = nullptr;			// �e

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CReporterSet::~CReporterSet()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CReporterSet *CReporterSet::Create(const MyLib::Vector3& pos, const CGameManager::ETeamSide& side)
{
	
	// �������̊m��
	CReporterSet* pObj = DEBUG_NEW CReporterSet;

	if (pObj != nullptr)
	{
		// �������
		pObj->SetPosition(pos + OffSet::DESK);		// �ʒu�ݒ�
		pObj->m_TeamSide = side;	// �`�[���T�C�h

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CReporterSet::Init()
{
	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ����������
	HRESULT hr = CObjectX::Init(MODEL_DESK);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// �֎q����
	CreateChair();

	// �}�C�N����
	CreateMic();

	// �e����
	if (m_pShadow == nullptr)
	{
		m_pShadow = CShadow::Create(this, 50.0f);
	}

	// �o��
	SetState(EState::STATE_GO);

	return S_OK;
}

//==========================================================================
// �֎q����
//==========================================================================
void CReporterSet::CreateChair()
{
	// ��������
	m_pChair = CObjectX::Create(MODEL_CHAIR, GetPosition() + OffSet::CHAIR, MyLib::Vector3());

	// ��ނ̐ݒ�
	m_pChair->CObject::SetType(TYPE_OBJECTX);
}

//==========================================================================
// �}�C�N����
//==========================================================================
void CReporterSet::CreateMic()
{
	// ��������
	m_pMic = CObjectX::Create(MODEL_MIC, GetPosition() + OffSet::MIC, MyLib::Vector3());

	// ��ނ̐ݒ�
	m_pMic->CObject::SetType(TYPE_OBJECTX);
}

//==========================================================================
// �I������
//==========================================================================
void CReporterSet::Uninit()
{
	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CReporterSet::Kill()
{
	// �e������
	SAFE_UNINIT(m_pShadow);

	// �����Z�b�g
	SAFE_UNINIT(m_pChair);
	SAFE_UNINIT(m_pMic);

	// �I������
	CReporterSet::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CReporterSet::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CReporterSet::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += fDeltaTime * fSlowRate;

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CReporterSet::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ������
//==========================================================================
void CReporterSet::StateGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
	if (m_fStateTime >= StateTime::GO)
	{// ���Ԍo��

		// ��ԑJ��
		SetState(EState::STATE_COLLECT);
	}
}

//==========================================================================
// ���
//==========================================================================
void CReporterSet::StateCollect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// �߂�
//==========================================================================
void CReporterSet::StateBack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
	if (m_fStateTime >= StateTime::BACK)
	{// ���Ԍo��


		// ����
		Kill();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CReporterSet::Draw()
{
	// �`��
	CObjectX::Draw();
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CReporterSet::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}
