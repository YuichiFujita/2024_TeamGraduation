//==========================================================================
// 
//  �`���[�g���A���m�F��ʏ��� [tutorialcheck.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "tutorialcheck.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "title_controllwait.h"
#include "tutorialscreen.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	namespace BG
	{
		const std::string TEXTURE = "data\\TEXTURE\\tutorial\\window.png";	// �e�N�X�`��
	}

	namespace Select
	{
		const std::string TEXTURE[CTutorialCheck::ESelect::SELECT_MAX] =	// �e�N�X�`��
		{
			"data\\TEXTURE\\tutorial\\select_yes.png",	// �`���[�g���A������
			"data\\TEXTURE\\tutorial\\select_no.png",	// �`���[�g���A�����Ȃ�
		};

		const MyLib::Vector3 POSITION_SELECT[CTutorialCheck::ESelect::SELECT_MAX] =	// �I�����̈ʒu
		{
			MyLib::Vector3(640.0f - 200.0f, 550.0f, 0.0f),	// �����J�n
			MyLib::Vector3(640.0f + 200.0f, 550.0f, 0.0f),	// �F�X
		};

		const float SIZEHEIGHT = 90.0f;	// �c��
	}

	namespace StateTime
	{
		const float FADE = 0.3f;	// �t�F�[�h
		const float SPAWN = 0.3f;	// �o��
	}
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CTutorialCheck::STATE_FUNC CTutorialCheck::m_StateFunc[] =
{
	&CTutorialCheck::StateNone,		// �Ȃɂ��Ȃ�
	&CTutorialCheck::StateFadeIn,	// �t�F�[�h�C��
	&CTutorialCheck::StateFadeOut,	// �t�F�[�h�A�E�g
};

//==========================================================================
//	�R���X�g���N�^
//==========================================================================
CTutorialCheck::CTutorialCheck(int controllIdx, int nPriority) : CObject2D(nPriority),
	m_nControllIdx		(controllIdx),			// ����C���f�b�N�X
	m_pControllWait		(nullptr),				// ����ҋ@���
	m_state			(EState::STATE_NONE),	// ���
	m_fStateTime	(0.0f),					// ��ԃ^�C�}�[
	m_select		(ESelect::SELECT_YES)	// �I����
{
}

//==========================================================================
//	�f�X�g���N�^
//==========================================================================
CTutorialCheck::~CTutorialCheck()
{

}

//==========================================================================
// ��������
//==========================================================================
CTutorialCheck* CTutorialCheck::Create(int controllIdx, CTitle_ControllWait* pControllWait)
{
	// �������̊m��
	CTutorialCheck* pObj = DEBUG_NEW CTutorialCheck(controllIdx);

	if (pObj != nullptr)
	{
		// �������
		pObj->m_pControllWait = pControllWait;

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
HRESULT CTutorialCheck::Init()
{
	// ��ނ��I�u�W�F�N�g2D�ɂ���
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// ��Ԑݒ�
	SetState(EState::STATE_FADEIN);

	// �w�i����
	if (FAILED(CreateBG()))
	{
		return E_FAIL;
	}

	// �I��������
	if (FAILED(CreateSelect()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �w�i����
//==========================================================================
HRESULT CTutorialCheck::CreateBG()
{
	// ����������
	CObject2D::Init();

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int texID = pTexture->Regist(BG::TEXTURE);
	BindTexture(texID);

	// �摜�T�C�Y�擾
	MyLib::Vector2 size = pTexture->GetImageSize(texID);

	// �c�������ɃT�C�Y�v�Z
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, 300.0f);
	SetSize(size);
	SetSizeOrigin(size);

	// �ʒu�ݒ�
	SetPosition(VEC3_SCREEN_CENT);

	// �F��ݒ�
	SetColor(MyLib::color::White(0.0f));

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// �I��������
//==========================================================================
HRESULT CTutorialCheck::CreateSelect()
{
	// �e�N�X�`���擾
	CTexture* pTexture = CTexture::GetInstance();

	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		// ��������
		m_pSelect[i] = CObject2D::Create(GetPriority() + 1);
		if (m_pSelect[i] == nullptr) { return E_FAIL; }

		// �e�N�X�`����o�^�E����
		int texID = pTexture->Regist(Select::TEXTURE[i]);
		m_pSelect[i]->BindTexture(texID);

		// �ʒu��ݒ�
		m_pSelect[i]->SetPosition(Select::POSITION_SELECT[i]);

		// �摜�T�C�Y�擾
		MyLib::Vector2 size = pTexture->GetImageSize(texID);

		// �c�������ɃT�C�Y�v�Z
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, Select::SIZEHEIGHT);
		m_pSelect[i]->SetSize(size);
		m_pSelect[i]->SetSizeOrigin(m_pSelect[i]->GetSize());

		// �F��ݒ�
		m_pSelect[i]->SetColor(MyLib::color::White(0.0f));
	}

	return S_OK;
}

//==========================================================================
//	�I������
//==========================================================================
void CTutorialCheck::Uninit()
{
	// �I�u�W�F�N�g��j��
	Release();
}

//==========================================================================
//	�폜����
//==========================================================================
void CTutorialCheck::Kill()
{
	// �t�F�[�h
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		SAFE_KILL(m_pSelect[i]);
	}

	// ���g�̏I��
	CTutorialCheck::Uninit();
}

//==========================================================================
//	�X�V����
//==========================================================================
void CTutorialCheck::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �X�V����
	CObject2D::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �F�̍X�V
	UpdateColor();

	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CTutorialCheck::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += fDeltaTime * fSlowRate;

	// �X�V����
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �����Ȃ�
//==========================================================================
void CTutorialCheck::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, m_nControllIdx) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, m_nControllIdx) ||
		pPad->GetLStickTrigger(m_nControllIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_RIGHT) ||
		pPad->GetLStickTrigger(m_nControllIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_LEFT))
	{// �ύX

		// ���݂̑I�����̋t�ɂ���
		m_select = (m_select == ESelect::SELECT_YES) ? ESelect::SELECT_NO : ESelect::SELECT_YES;

		// �T�E���h�̍Đ�
		int soundLabel = UtilFunc::Transformation::Random(CSound::ELabel::LABEL_SE_WRITE01, CSound::ELabel::LABEL_SE_WRITE02);
		PLAY_SOUND((CSound::ELabel)soundLabel);
	}

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, m_nControllIdx))
	{// ����

		if (!GET_MANAGER->GetFade()->IsFade())
		{
			// �T�E���h�̍Đ�
			PLAY_SOUND(CSound::ELabel::LABEL_SE_DECIDE_00);
		}

		Decide();
	}
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CTutorialCheck::StateFadeIn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �s�����x�X�V
	float alpha = UtilFunc::Correction::EasingEaseOut(0.0f, 1.0f, 0.0f, StateTime::FADE, m_fStateTime);
	
	// �ݒ�
	SetAlpha(alpha);
	for (const auto& select : m_pSelect)
	{
		select->SetAlpha(alpha);
	}

	// ���Ԍo��
	if (m_fStateTime >= StateTime::FADE)
	{
		// NONE�ɐݒ�
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CTutorialCheck::StateFadeOut(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �s�����x�X�V
	float alpha = UtilFunc::Correction::EasingEaseOut(1.0f, 0.0f, 0.0f, StateTime::FADE, m_fStateTime);

	// �ݒ�
	SetAlpha(alpha);
	for (const auto& select : m_pSelect)
	{
		select->SetAlpha(alpha);
	}

	// ���Ԍo��
	if (m_fStateTime >= StateTime::FADE)
	{
		// ����ҋ@�̏�Ԑݒ�
		if (m_pControllWait->GetState() == CTitle_ControllWait::EState::STATE_TUTORIALCHECK)
		{
			m_pControllWait->SetState(CTitle_ControllWait::EState::STATE_WAIT);
		}

		// �폜
		Kill();
	}
}

//==========================================================================
// �F�̍X�V
//==========================================================================
void CTutorialCheck::UpdateColor()
{
	// �I�����Ă���ڗ�������
	for (const auto& select : m_pSelect)
	{
		select->SetColor(MyLib::color::White(select->GetAlpha()));
		select->SetSize(select->GetSizeOrigin());
	}

	// �I�����Ă���ڗ�������
	m_pSelect[m_select]->SetColor(MyLib::color::Red(m_pSelect[m_select]->GetAlpha()));
	m_pSelect[m_select]->SetSize(m_pSelect[m_select]->GetSizeOrigin() * 1.2f);
}

//==========================================================================
// ����
//==========================================================================
void CTutorialCheck::Decide()
{
	switch (m_select)
	{
	case CTutorialCheck::SELECT_YES:	// �`���[�g���A������

		// �`���[�g���A������Ԑݒ�
		m_pControllWait->SetState(CTitle_ControllWait::EState::STATE_TUTORIAL);
		break;

	case CTutorialCheck::SELECT_NO:	// �`���[�g���A���݂Ȃ��ŊJ�n

		// �J��
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_ENTRY);
		break;

	default:
		break;
	}
}

//==========================================================================
// �L�����Z��
//==========================================================================
void CTutorialCheck::Cancel()
{
	// �t�F�[�h�A�E�g
	SetState(EState::STATE_FADEOUT);
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CTutorialCheck::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}
