//=============================================================================
// 
//  �S�ˏ��� [stretcher.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "stretcher.h"
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
	const char* MODEL = "data\\MODEL\\komono\\stretcher.x";
	const float VELOCITY_MOVE = 10.0f;	// �ړ����x
	const MyLib::Vector3 STARTPOS = MyLib::Vector3(-3000.0f, 0.0f, 0.0f);	// �J�n�n�_
	const float DISTANCE_UP = 50.0f;	// �ςފԊu
}

namespace StateTime
{
	const float GO = 1.0f;		// ������
	const float BACK = 2.0f;	// �߂�
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CStretcher::STATE_FUNC CStretcher::m_StateFunc[] =	// ��Ԋ֐�
{
	&CStretcher::StateNone,		// �Ȃ�
	&CStretcher::StateGo,		// ������
	&CStretcher::StateCollect,	// ���
	&CStretcher::StateBack,		// �߂�
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CStretcher* CStretcher::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CStretcher::CStretcher(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_fStateTime = 0.0f;			// ��ԃJ�E���^�[
	m_state = EState::STATE_NONE;	// ���

	// ���̑��ϐ�
	m_pCollectPlayer.clear();		// �������v���C���[
	m_pCarryPlayer.clear();			// �^�ԃv���C���[
	m_pLeftNurse = nullptr;			// ���̐搶
	m_pRightNurse = nullptr;		// �E�̐搶
	m_pShadow = nullptr;			// �e
	m_bEndSetting = false;			// �I���̃Z�b�e�B���O

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CStretcher::~CStretcher()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CStretcher *CStretcher::Create(CPlayer* pPlayer)
{
	if (m_pThisPtr != nullptr)
	{
		// �v���C���[�ǉ�
		m_pThisPtr->AddCollectPlayer(pPlayer);
		return m_pThisPtr;
	}

	// �������̊m��
	m_pThisPtr = DEBUG_NEW CStretcher;

	if (m_pThisPtr != nullptr)
	{
		// ����������
		m_pThisPtr->Init();

		// �v���C���[�ǉ�
		m_pThisPtr->AddCollectPlayer(pPlayer);
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CStretcher::Init()
{
	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ���
	SetState(EState::STATE_NONE);

	// ����������
	HRESULT hr = CObjectX::Init(MODEL);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// ��
	m_fWidth = GetVtxMax().x;

	// �e����
	if (m_pShadow == nullptr)
	{
		m_pShadow = CShadow::Create(this, 50.0f);
	}

	// ���̐搶
	if (m_pLeftNurse == nullptr)
	{
		m_pLeftNurse = CSchoolNurse::Create(this);
		m_pLeftNurse->SetPosition(STARTPOS);
	}

	// �E�̐搶
	if (m_pRightNurse == nullptr)
	{
		m_pRightNurse = CSchoolNurse::Create(this);
		m_pRightNurse->SetPosition(STARTPOS);
	}

	// �ʒu�ݒ�
	SetPosition(STARTPOS);
	m_startPos = STARTPOS;

	// �o��
	SetState(EState::STATE_GO);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CStretcher::Uninit()
{
	m_pThisPtr = nullptr;

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CStretcher::Kill()
{
	// �e������
	SAFE_UNINIT(m_pShadow);

	// �I������
	CStretcher::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CStretcher::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ����I���̐ݒ�
	CollectEndSetting();

	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// �搶�̈ʒu�␳
	PosAdjNurse();

	// �v���C���[�^��
	MyLib::Vector3 pos = GetPosition();
	for (const auto& player : m_pCarryPlayer)
	{
		player->SetPosition(pos);
		pos.y += DISTANCE_UP;
	}
}

//==========================================================================
// �搶�̈ʒu�␳
//==========================================================================
void CStretcher::PosAdjNurse()
{
	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 setpos = MyLib::Vector3(pos.x, 0.0f, pos.z);

	// �ʒu����o��
	setpos.x += sinf(D3DX_PI * 0.5f + rot.y) * m_fWidth;
	setpos.z += cosf(D3DX_PI * 0.5f + rot.y) * m_fWidth;
	if (m_pLeftNurse != nullptr)
	{
		m_pLeftNurse->SetPosition(setpos);

		// �����ݒ�
		m_pLeftNurse->SetRotation(MyLib::Vector3(0.0f, setpos.AngleXZ(pos), 0.0f));
	}

	// ���Z�b�g
	setpos = MyLib::Vector3(pos.x, 0.0f, pos.z);

	// �ʒu����o��
	setpos.x += sinf(D3DX_PI * 0.5f + rot.y) * -m_fWidth;
	setpos.z += cosf(D3DX_PI * 0.5f + rot.y) * -m_fWidth;
	if (m_pRightNurse != nullptr)
	{
		m_pRightNurse->SetPosition(setpos);

		// �����ݒ�
		m_pRightNurse->SetRotation(MyLib::Vector3(0.0f, setpos.AngleXZ(pos), 0.0f));
	}
}

//==========================================================================
// ����I���̐ݒ�
//==========================================================================
void CStretcher::CollectEndSetting()
{
	// �ݒ肵�Ȃ��Ƃ��͔�����
	if (!m_bEndSetting) return;

	// �J�n�ʒu�X�V
	m_startPos = GetPosition();

	// �v�f�폜
	m_pCarryPlayer.push_back((*m_pCollectPlayer.begin()));	// �^�ԃv���C���[
	m_pCollectPlayer.erase(m_pCollectPlayer.begin());

	if (m_pCollectPlayer.empty())
	{// �v�f�S�����Ȃ��Ȃ��Ă���

		// �߂�
		SetState(EState::STATE_BACK);

		// �搶���߂�
		m_pLeftNurse->SetState(CSchoolNurse::EState::STATE_BACK);
		m_pRightNurse->SetState(CSchoolNurse::EState::STATE_BACK);
	}
	else
	{
		// �ďo��
		SetState(EState::STATE_GO);
	}

	// �I���̃Z�b�e�B���O
	m_bEndSetting = false;

}

//==========================================================================
// �������v���C���[�擾
//==========================================================================
CPlayer* CStretcher::GetCollectPlayer()
{
	if (m_pCollectPlayer.empty())
	{// �v�f�S�����Ȃ��Ȃ��Ă���
		return nullptr;
	}

	return (*m_pCollectPlayer.begin());
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CStretcher::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += fDeltaTime * fSlowRate;

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CStretcher::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ������
//==========================================================================
void CStretcher::StateGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �擪�擾
	std::vector<CPlayer*>::iterator itr = m_pCollectPlayer.begin();
	CPlayer* pPlayer = (*itr);

	// ����Ɍ�����
	MyLib::Vector3 playerPos = pPlayer->GetPosition();

	// �S�˂��猩���v���C���[�ւ̌���
	float fRotY = GetPosition().AngleXZ(playerPos) + D3DX_PI * 0.5f;
	SetRotation(MyLib::Vector3(0.0f, fRotY, 0.0f));

	// �v���C���[�Ɍ����Ĉړ�
	MyLib::Vector3 pos = UtilFunc::Correction::EasingEaseOut(m_startPos, playerPos, 0.0f, StateTime::GO, m_fStateTime);
	SetPosition(pos);

	if (m_fStateTime >= StateTime::GO)
	{// ���Ԍo��

		// �搶������ɂ���
		m_pLeftNurse->SetState(CSchoolNurse::EState::STATE_COLLECT);
		m_pRightNurse->SetState(CSchoolNurse::EState::STATE_COLLECT);

		// ��ԑJ��
		SetState(EState::STATE_COLLECT);
	}
}

//==========================================================================
// ���
//==========================================================================
void CStretcher::StateCollect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// �߂�
//==========================================================================
void CStretcher::StateBack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �S�˂��猩���I���n�_
	float fRotY = GetPosition().AngleXZ(STARTPOS) + D3DX_PI * 0.5f;
	SetRotation(MyLib::Vector3(0.0f, fRotY, 0.0f));

	// �v���C���[�Ɍ����Ĉړ�
	MyLib::Vector3 pos = UtilFunc::Correction::EasingEaseOut(m_startPos, STARTPOS, 0.0f, StateTime::BACK, m_fStateTime);
	SetPosition(pos);

	if (m_fStateTime >= StateTime::BACK)
	{// ���Ԍo��

		// �搶������
		m_pLeftNurse->Kill();
		m_pRightNurse->Kill();

		// ����
		Kill();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CStretcher::Draw()
{
	// �`��
	CObjectX::Draw();
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CStretcher::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}
