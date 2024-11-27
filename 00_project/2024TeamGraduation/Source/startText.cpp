//==========================================================================
// 
//  �Q�[���X�^�[�g�̕������� [startText.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "startText.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\visual\\startText.jpg";	// �e�N�X�`���̃t�@�C��
}

namespace StateTime
{
	const float FADEIN = 0.3f;	// �t�F�[�h�C��
	const float FADEOUT = 0.3f;	// �t�F�[�h�A�E�g
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CStartText::STATE_FUNC CStartText::m_StateFuncList[] =
{
	&CStartText::StateNone,		// �Ȃɂ��Ȃ����
	&CStartText::StateFadeIn,	// �t�F�[�h�C�����
	&CStartText::StateFadeOut,	// �t�F�[�h�A�E�g���
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CStartText::CStartText(int nPriority) : CObject2D(nPriority),
	m_fStateTime	(0.0f),					// ��ԃJ�E���^�[
	m_state			(EState::STATE_NONE)	// ���
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CStartText::~CStartText()
{

}

//==========================================================================
// ��������
//==========================================================================
CStartText* CStartText::Create()
{
	// �������̊m��
	CStartText* pObj = DEBUG_NEW CStartText;
	if (pObj != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pObj->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CStartText::Init()
{
	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SAMPLE);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 160.0f);

#else	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 160.0f);
#endif

	SetSize(size);
	SetSizeOrigin(size);

	// ��ʒ����̈ʒu�ɂ���
	SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// ��Ԑݒ�
	SetState(EState::STATE_FADEIN);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CStartText::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CStartText::Kill()
{
	// �폜����
	CObject2D::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CStartText::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// �X�V���ɏ����Ă���I��
	if (IsDeath()) return;

	// �X�V����
	CObject2D::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CStartText::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}

//==========================================================================
// ��ԍX�V����
//==========================================================================
void CStartText::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�}�[�����Z
	m_fStateTime += fDeltaTime * fSlowRate;
	
	// ��ԕʏ���
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CStartText::StateFadeIn()
{
	// �s�����x����o��
	float alpha = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, StateTime::FADEIN, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEIN)
	{
		// ��ԑJ��
		SetState(EState::STATE_FADEOUT);
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CStartText::StateFadeOut()
{
	// �s�����x����o��
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADEIN, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEIN)
	{
		// �폜
		Kill();
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CStartText::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}
