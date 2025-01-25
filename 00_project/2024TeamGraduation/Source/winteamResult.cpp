//==========================================================================
// 
//  �����`�[������ [winteamResult.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "winteamResult.h"
#include "manager.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\entry\\area.png";	// �e�N�X�`��
	const std::string TEXTURE_AUDIENCE = "data\\TEXTURE\\result\\audience.png";	// �e�N�X�`��
	const MyLib::Vector3 SPAWNPOS_AUDIENCE = MyLib::Vector3(640.0f, 900.0f, 0.0f);
	const float JUMPVALUE = 50.0f;
}

namespace StateTime
{
	const float SPAWN = 0.5f;	// �o��
	const float LOOP = 0.5f;	// ���[�v����
	const float FADEOUT = 0.5f;	// �J�n
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CWinTeamResult::STATE_FUNC CWinTeamResult::m_StateFunc[] =	// ��Ԋ֐�
{
	&CWinTeamResult::StateNone,		// �Ȃ�
	&CWinTeamResult::StateSpawn,	// �o��
	&CWinTeamResult::StateLoop,		// ���[�v
	&CWinTeamResult::StateFadeOut,	// �t�F�[�h�A�E�g
};

//==========================================================================
//	�R���X�g���N�^
//==========================================================================
CWinTeamResult::CWinTeamResult(int nPriority) : CObject2D_Anim(nPriority)
{

}

//==========================================================================
//	�f�X�g���N�^
//==========================================================================
CWinTeamResult::~CWinTeamResult()
{

}

//==========================================================================
// ��������
//==========================================================================
CWinTeamResult* CWinTeamResult::Create(CGameManager::ETeamSide winSIde)
{
	// �������̊m��
	CWinTeamResult* pObj = DEBUG_NEW CWinTeamResult;

	if (pObj != nullptr)
	{
		// �����`�[��
		pObj->m_winTeam = winSIde;

		// �N���X�̏�����
		if (FAILED(pObj->Init()))
		{ // �������Ɏ��s�����ꍇ
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
//	����������
//==========================================================================
HRESULT CWinTeamResult::Init()
{
	// UV�������ݒ�
	SetDivisionUV(1, 2);

	//	����������
	CObject2D_Anim::Init();

	// ��ނ��I�u�W�F�N�g2D�ɂ���
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist(TEXTURE);
	BindTexture(nTexID);

	// �傫���ݒ�
	SetSizeByWidth(400.0f);
	SetSizeOrigin(GetSize());

	// �p�^�[���ݒ�
	SetPatternAnim(m_winTeam);

	// �����Đ���OFF�ɂ���
	SetEnableAutoPlay(false);

	// ���_���ݒ�
	SetVtx();

	// �ʒu�ݒ�
	SetPosition(VEC3_SCREEN_CENT);

	// ��Ԑݒ�
	SetState(EState::STATE_SPAWN);

	// �ϋq����
	if (FAILED(CreateAudience()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �ϋq�̐���
//==========================================================================
HRESULT CWinTeamResult::CreateAudience()
{
	// �ϋq�̐���
	m_pAudience = CObject2D_Anim::Create(
		MyLib::Vector3(640.0f, 480.0f, 0.0f),	// �ʒu
		2,				// U
		2,				// V
		0.1f,			// �C���^�[�o��
		false,
		GetPriority() - 1
	);

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist(TEXTURE_AUDIENCE);
	m_pAudience->BindTexture(nTexID);

	// �T�C�Y�ݒ�
	// �摜�T�C�Y�擾
	MyLib::Vector2 size = pTexture->GetImageSize(GetIdxTexture());

	// �������Ŋ���
	size.x *= (1.0f / (float)m_pAudience->GetDivisionU());
	size.y *= (1.0f / (float)m_pAudience->GetDivisionV());

	// �c�������ɃT�C�Y�v�Z
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 640.0f);
	m_pAudience->SetSize(size);

	// �s�����x�ݒ�
	m_pAudience->SetAlpha(0.8f);

	return S_OK;
}

//==========================================================================
//	�I������
//==========================================================================
void CWinTeamResult::Uninit()
{
	// �I������
	CObject2D_Anim::Uninit();

	// �ϋq
	SAFE_UNINIT(m_pAudience);
}

//==========================================================================
//	�폜����
//==========================================================================
void CWinTeamResult::Kill()
{
	// �ϋq
	SAFE_KILL(m_pAudience);

	// ���g�̏I��
	CWinTeamResult::Uninit();
}

//==========================================================================
//	�X�V����
//==========================================================================
void CWinTeamResult::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	if (IsDeath()) return;

	// �e�̍X�V
	CObject2D_Anim::Update(fDeltaTime, fDeltaRate, fSlowRate);
}


//==========================================================================
// ��ԍX�V
//==========================================================================
void CWinTeamResult::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�}�[�X�V
	m_fStateTime += fDeltaTime * fDeltaRate * fSlowRate;

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �o��
//==========================================================================
void CWinTeamResult::StateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����v�Z
	float ratio = UtilFunc::Correction::EasingCubicOut(0.3f, 1.0f, 0.0f, StateTime::SPAWN, m_fStateTime);

	// �T�C�Y
	SetSize(GetSizeOrigin() * ratio);

	// �s�����x
	SetAlpha(ratio);


	// �ϋq�o��
	MyLib::Vector3 setpos = SPAWNPOS_AUDIENCE + (m_pAudience->GetOriginPosition() - SPAWNPOS_AUDIENCE) * ratio;
	m_pAudience->SetPosition(setpos);

	if (m_fStateTime >= StateTime::SPAWN)
	{// �J��
		SetState(EState::STATE_LOOP);
	}
}

//==========================================================================
// ���[�v
//==========================================================================
void CWinTeamResult::StateLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����v�Z
	float cos = cosf(D3DX_PI * (m_fStateTime / StateTime::LOOP));
	float ratio = 0.8f + (1.0f - 0.8f) * cos;

	// �ϋq�o��
	MyLib::Vector3 setpos = m_pAudience->GetOriginPosition();
	setpos.y += JUMPVALUE;

	const float loopTime = StateTime::LOOP * 0.5f;
	cos = cosf(D3DX_PI * (m_fStateTime / loopTime));
	setpos.y += JUMPVALUE * cos;
	m_pAudience->SetPosition(setpos);
	
	if (m_fStateTime >= StateTime::LOOP * 2.0f)
	{
		m_fStateTime = 0.0f;
	}

	// �T�C�Y
	SetSize(GetSizeOrigin() * ratio);
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CWinTeamResult::StateFadeOut(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����v�Z
	float ratio = UtilFunc::Correction::EaseInExpo(1.0f, 0.3f, 0.0f, StateTime::FADEOUT, m_fStateTime);

	// �T�C�Y
	SetSize(GetSizeOrigin() * ratio);

	// �s�����x
	SetAlpha(ratio);


	// �ϋq�ޏ�
	ratio = UtilFunc::Correction::EaseOutExpo(0.0f, 1.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);
	MyLib::Vector3 setpos = m_pAudience->GetOriginPosition() + (SPAWNPOS_AUDIENCE - m_pAudience->GetOriginPosition()) * ratio;
	m_pAudience->SetPosition(setpos);

	if (m_fStateTime >= StateTime::FADEOUT)
	{// �J��
		Kill();
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CWinTeamResult::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;

	if (state == EState::STATE_FADEOUT)
	{
		m_pAudience->SetOriginPosition(m_pAudience->GetPosition());
	}
}

//==========================================================================
// ��������T�C�Y�ݒ�
//==========================================================================
void CWinTeamResult::SetSizeByWidth(const float width)
{
	// �摜�T�C�Y�擾
	CTexture* pTexture = CTexture::GetInstance();
	MyLib::Vector2 size = pTexture->GetImageSize(GetIdxTexture());

	// �������Ŋ���
	size.x *= (1.0f / (float)GetDivisionU());
	size.y *= (1.0f / (float)GetDivisionV());

	// �c�������ɃT�C�Y�v�Z
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, width);
	SetSize(size);
}
