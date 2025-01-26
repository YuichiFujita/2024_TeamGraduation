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
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\visual\\startText.png";	// �e�N�X�`���̃t�@�C��
	const MyLib::Vector3 INIT_POS = MyLib::Vector3(-VEC3_SCREEN_SIZE.x, 360.0f, 0.0f);		// �����ʒu
	const MyLib::Vector3 MIDDLE_POS = MyLib::Vector3(VEC3_SCREEN_CENT.x, INIT_POS.y, 0.0f);	// ���Ԉʒu
	const MyLib::Vector3 DEST_POS = MyLib::Vector3(VEC3_SCREEN_SIZE.x * 2.0f, INIT_POS.y, 0.0f);	// �ڕW�ʒu
	const float WIDTH = 380.0f;	// ����
}

namespace StateTime
{
	const float FADEIN	= 0.4f;	// �t�F�[�h�C��
	const float WAIT	= 0.9f;	// �ҋ@
	const float FADEOUT	= 0.4f;	// �t�F�[�h�A�E�g
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CStartText::STATE_FUNC CStartText::m_StateFuncList[] =
{
	&CStartText::StateNone,		// �Ȃɂ��Ȃ����
	&CStartText::StateFadeIn,	// �t�F�[�h�C�����
	&CStartText::StateWait,		// �ҋ@���
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

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, WIDTH);
	SetSize(size);
	SetSizeOrigin(size);

	// ��ʒ����̈ʒu�ɂ���
	SetPosition(INIT_POS);

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
	// �w�i�̈ʒu���ړ�
	MyLib::Vector3 posGameSet = UtilFunc::Correction::EaseOutBack(INIT_POS, MIDDLE_POS, 0.0f, StateTime::FADEIN, m_fStateTime, 1.0f);
	SetPosition(posGameSet);

	if (m_fStateTime >= StateTime::FADEIN)
	{
		// ��ԑJ��
		SetState(EState::STATE_WAIT);
	}
}

//==========================================================================
// �ҋ@
//==========================================================================
void CStartText::StateWait()
{
	if (m_fStateTime >= StateTime::WAIT)
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
	// �w�i�̈ʒu���ړ�
	MyLib::Vector3 posGameSet = UtilFunc::Correction::EasingCubicOut(MIDDLE_POS, DEST_POS, 0.0f, StateTime::FADEOUT, m_fStateTime);
	SetPosition(posGameSet);

	if (m_fStateTime >= StateTime::FADEOUT)
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
