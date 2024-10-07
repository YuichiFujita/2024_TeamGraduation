//=============================================================================
// 
//  �������� [subtitle.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "subtitle.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"
#include "player.h"
#include "baggage.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	
}

// ��ԕʃ^�C�}�[
namespace StateTime
{
	const float FADEIN = 0.15f;		// �t�F�[�h�C��
	const float FADEOUT = 0.15f;		// �t�F�[�h�A�E�g
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CSubTitle::STATE_FUNC CSubTitle::m_StateFuncList[] =
{
	&CSubTitle::StateNone,		// �Ȃ�
	&CSubTitle::StateFadeIn,	// �t�F�[�h�C��
	&CSubTitle::StateFadeout,	// �t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CSubTitle::CSubTitle(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_state = State::STATE_NONE;	// ���
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[
	m_fLifeTimer = 0.0f;	// ��������
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CSubTitle::~CSubTitle()
{

}

//==========================================================================
// ��������
//==========================================================================
CSubTitle* CSubTitle::Create(const MyLib::Vector3& pos, float life)
{
	// �������̊m��
	CSubTitle* pObj = DEBUG_NEW CSubTitle;

	if (pObj != nullptr)
	{
		pObj->m_fLifeTimer = life;
		pObj->SetPosition(pos);

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CSubTitle::Init()
{

	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// �e��ϐ�������
	m_state = State::STATE_FADEIN;	// ���
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);



	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CSubTitle::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CSubTitle::Update()
{
	// �������Ԍ��Z
	m_fLifeTimer -= CManager::GetInstance()->GetDeltaTime();

	if (m_fLifeTimer <= 0.0f &&
		m_state != State::STATE_FADEOUT)
	{
		m_state = State::STATE_FADEOUT;
		m_fStateTime = 0.0f;
	}

	// ��ԍX�V
	UpdateState();

	if (CManager::GetInstance()->GetInstantFade()->GetState() != CInstantFade::STATE::STATE_NONE)
	{
		Uninit();
	}

	if (IsDeath())
	{
		return;
	}

	// �X�V����
	CObject2D::Update();
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CSubTitle::UpdateState()
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// ���s
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CSubTitle::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CSubTitle::StateFadeIn()
{
	float alpha = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, StateTime::FADEIN, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		SetAlpha(1.0f);

		// ��ԃ^�C�}�[���Z�b�g
		m_fStateTime = 0.0f;
		m_state = State::STATE_NONE;
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CSubTitle::StateFadeout()
{
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		// ��ԃ^�C�}�[���Z�b�g
		m_fStateTime = 0.0f;

		Uninit();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CSubTitle::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CSubTitle::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// �������蓖��
//==========================================================================
void CSubTitle::BindSubtitle(const std::string& texfile)
{
	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(texfile);
	BindTexture(texID);
}

//==========================================================================
// ��������ɃT�C�Y�ݒ�
//==========================================================================
void CSubTitle::SetSizeByHeight(const float height)
{

	// �c�������ɃT�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(GetIdxTexture());
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, height);

	// �T�C�Y�ݒ�
	SetSize(size);
	SetSizeOrigin(size);
}
