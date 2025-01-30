//==========================================================================
// 
//  �`���[�g���A����ʏ��� [tutorialscreen.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "tutorialscreen.h"
#include "manager.h"
#include "fade.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	namespace Fade
	{
		const std::string TEXTURE = "data\\TEXTURE\\entry\\bg_scroll.png";	// �e�N�X�`��
		const float SCROLL_X = 0.00038f;
		const float SCROLL_Y = SCROLL_X * UtilFunc::Calculation::AspectRatio(D3DXVECTOR2(640.0f, 360.0f));
		const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);	// �������F
	}

	namespace BG
	{
		const std::string TEXTURE = "data\\TEXTURE\\entry\\bg.png";	// �e�N�X�`��
	}

	namespace Manual
	{
		const std::string TEXTURE = "data\\TEXTURE\\tutorial\\manual.png";	// �e�N�X�`��
	}

	namespace ManualText
	{
		const std::string TEXTURE[CTutorialScreen::EManualType::TYPE_MAX] =	// �e�N�X�`��
		{
			"data\\TEXTURE\\tutorial\\move.png",		// �ړ�
			"data\\TEXTURE\\tutorial\\blink.png",		// �u�����N
			"data\\TEXTURE\\tutorial\\jump.png",		// �W�����v
			"data\\TEXTURE\\tutorial\\throw.png",		// ����
			"data\\TEXTURE\\tutorial\\pass.png",		// �p�X
			"data\\TEXTURE\\tutorial\\catch.png",		// �L���b�`
			"data\\TEXTURE\\tutorial\\SP.png",			// �X�y�V����
			"data\\TEXTURE\\tutorial\\justcatch.png",	// �W���X�g�L���b�`
		};
		const float START_X = -640.0f;	// �X�^�[�g�ʒu(X)
	}

	namespace StateTime
	{
		const float SPAWN = 0.3f;	// �o��
		const float FADEOUT = 0.3f;	// �t�F�[�h�A�E�g
	}
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CTutorialScreen::STATE_FUNC CTutorialScreen::m_StateFunc[] =
{
	&CTutorialScreen::StateNone,	// �Ȃɂ��Ȃ�
	&CTutorialScreen::StateSpawn,	// �o��
	&CTutorialScreen::StateFadeIn,	// �t�F�[�h�C��
	&CTutorialScreen::StateFadeOut,	// �t�F�[�h�A�E�g
};

//==========================================================================
//	�R���X�g���N�^
//==========================================================================
CTutorialScreen::CTutorialScreen(int nPriority) : CObject2D(nPriority),
	m_state			(EState::STATE_NONE),		// ���
	m_fStateTime	(0.0f),						// ��ԃ^�C�}�[
	m_ManualType	(EManualType::TYPE_MOVE),	// �����̎��
	m_scrollDir		(EScrollDir::SCROLL_R),		// �X�N���[������
	m_pFade			(nullptr),					// �t�F�[�h
	m_pManual		(nullptr)					// ����
{

}

//==========================================================================
//	�f�X�g���N�^
//==========================================================================
CTutorialScreen::~CTutorialScreen()
{

}

//==========================================================================
// ��������
//==========================================================================
CTutorialScreen* CTutorialScreen::Create()
{
	// �������̊m��
	CTutorialScreen* pObj = DEBUG_NEW CTutorialScreen();

	if (pObj != nullptr)
	{
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
HRESULT CTutorialScreen::Init()
{
	// ��ނ��I�u�W�F�N�g2D�ɂ���
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �w�i����
	if (FAILED(CreateBG()))
	{
		return E_FAIL;
	}

	// �t�F�[�h����
	if (FAILED(CreateFade()))
	{
		return E_FAIL;
	}

	// �}�j���A������
	if (FAILED(CreateManual()))
	{
		return E_FAIL;
	}

	// �}�j���A���e�L�X�g����
	if (FAILED(CreateManualText()))
	{
		return E_FAIL;
	}

	// ��Ԑݒ�
	SetState(EState::STATE_FADEIN);
	StateFadeIn(0.0f, 0.0f, 1.0f);

	return S_OK;
}

//==========================================================================
// �w�i����
//==========================================================================
HRESULT CTutorialScreen::CreateBG()
{
	// ����������
	CObject2D::Init();

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist(BG::TEXTURE);
	BindTexture(nTexID);

	// �T�C�Y�ݒ�
	SetSize(MyLib::Vector2(640.0f, 360.0f));

	// �ʒu�ݒ�
	SetPosition(VEC3_SCREEN_CENT);

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// �t�F�[�h����
//==========================================================================
HRESULT CTutorialScreen::CreateFade()
{
	// ��������
	m_pFade = CObject2D::Create(GetPriority() + 1);
	if (m_pFade == nullptr) { return E_FAIL; }

	// �ʒu��ݒ�
	m_pFade->SetPosition(VEC3_SCREEN_CENT);

	// �傫����ݒ�
	m_pFade->SetSize(MyLib::Vector2(640.0f, 360.0f));

	// �F��ݒ�
	m_pFade->SetColor(Fade::INIT_COL);

	// �e�N�X�`����o�^�E����
	CTexture* pTexture = CTexture::GetInstance();
	int texID = pTexture->Regist(Fade::TEXTURE);
	m_pFade->BindTexture(texID);

	return S_OK;
}

//==========================================================================
// ��������
//==========================================================================
HRESULT CTutorialScreen::CreateManual()
{
	// ��������
	m_pManual = CObject2D::Create(GetPriority() + 1);
	if (m_pManual == nullptr) { return E_FAIL; }

	// �ʒu��ݒ�
	m_pManual->SetPosition(VEC3_SCREEN_CENT);

	// �傫����ݒ�
	m_pManual->SetSize(MyLib::Vector2(640.0f, 360.0f));

	// �e�N�X�`����o�^�E����
	CTexture* pTexture = CTexture::GetInstance();
	int texID = pTexture->Regist(Manual::TEXTURE);
	m_pManual->BindTexture(texID);

	return S_OK;
}

//==========================================================================
// �����e�L�X�g����
//==========================================================================
HRESULT CTutorialScreen::CreateManualText()
{
	// ��������
	m_pManualText = CObject2D::Create(GetPriority() + 1);
	if (m_pManualText == nullptr) { return E_FAIL; }

	// �ʒu��ݒ�
	m_pManualText->SetPosition(VEC3_SCREEN_CENT);

	// �傫����ݒ�
	m_pManualText->SetSize(MyLib::Vector2(640.0f, 360.0f));

	// �e�N�X�`����o�^�E����
	CTexture* pTexture = CTexture::GetInstance();
	int texID = pTexture->Regist(ManualText::TEXTURE[m_ManualType]);
	m_pManualText->BindTexture(texID);

	return S_OK;
}

//==========================================================================
//	�I������
//==========================================================================
void CTutorialScreen::Uninit()
{
	// �I�u�W�F�N�g��j��
	Release();
}

//==========================================================================
//	�폜����
//==========================================================================
void CTutorialScreen::Kill()
{
	// �t�F�[�h
	SAFE_KILL(m_pFade);

	// �}�j���A��
	SAFE_KILL(m_pManual);

	// ���g�̏I��
	CTutorialScreen::Uninit();
}

//==========================================================================
//	�X�V����
//==========================================================================
void CTutorialScreen::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �X�V����
	CObject2D::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �w�i�X�V
	UpdateBG(fDeltaTime, fDeltaRate, fSlowRate);

	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// �����̕ύX����
	ChangeManual();
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CTutorialScreen::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += fDeltaTime * fSlowRate;

	// �X�V����
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �����Ȃ�
//==========================================================================
void CTutorialScreen::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
}

//==========================================================================
// �o��
//==========================================================================
void CTutorialScreen::StateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ʒu�X�V
	MyLib::Vector3 pos = m_pManualText->GetPosition();
	float posDestX = SCREEN_WIDTH;	// �ڕW��X

	if (m_fStateTime <= StateTime::SPAWN * 0.5f)
	{
		posDestX = (m_scrollDir == EScrollDir::SCROLL_R) ? (-640.0f - SCREEN_WIDTH) : (SCREEN_WIDTH + 640.0f);	// �ڕW��X
		pos.x = UtilFunc::Correction::EasingEaseIn(m_pManualText->GetOriginPosition().x, posDestX, 0.0f, StateTime::SPAWN * 0.5f, m_fStateTime);
	}
	else
	{
		float startX = (m_scrollDir == EScrollDir::SCROLL_R) ? (SCREEN_WIDTH + 640.0f) : ManualText::START_X;	// �J�n��X
		pos.x = UtilFunc::Correction::EaseOutBack(startX, 640.0f, StateTime::SPAWN * 0.5f, StateTime::SPAWN, m_fStateTime);

		// �e�N�X�`����o�^�E����
		CTexture* pTexture = CTexture::GetInstance();
		int texID = pTexture->Regist(ManualText::TEXTURE[m_ManualType]);
		m_pManualText->BindTexture(texID);
	}
	m_pManualText->SetPosition(pos);

	if (m_fStateTime >= StateTime::SPAWN)
	{
		// NONE�ɐݒ�
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CTutorialScreen::StateFadeIn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �s�����x����o��
	float alpha = UtilFunc::Correction::EasingEaseOut(0.0f, 1.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);

	// �s�����x�ݒ�
	SetAlpha(alpha);
	m_pFade->SetAlpha(alpha);		// �t�F�[�h�̏��
	m_pManual->SetAlpha(alpha);		// �����̏��
	m_pManualText->SetAlpha(alpha);	// �����e�L�X�g�̏��

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CTutorialScreen::StateFadeOut(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �s�����x����o��
	float alpha = UtilFunc::Correction::EasingEaseOut(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);

	// �s�����x�ݒ�
	SetAlpha(alpha);
	m_pFade->SetAlpha(alpha);		// �t�F�[�h�̏��
	m_pManual->SetAlpha(alpha);		// �����̏��
	m_pManualText->SetAlpha(alpha);	// �����e�L�X�g�̏��

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		// �폜
		Kill();
	}
}

//==========================================================================
// �w�i�X�V
//==========================================================================
void CTutorialScreen::UpdateBG(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �t�F�[�h�X�N���[��
	D3DXVECTOR2* pTex = m_pFade->GetTex();
	for (int i = 0; i < 4; i++)
	{
		pTex[i].x += Fade::SCROLL_X;
		pTex[i].y -= Fade::SCROLL_Y;
	}
}

//==========================================================================
// �����̕ύX����
//==========================================================================
void CTutorialScreen::ChangeManual()
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	bool bChange = false;	// �ύX�t���O
	EManualType oldType = m_ManualType;	// �O��̎��

	if (pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT).bInput)
	{// ���y�[�W��

		EManualType old = m_ManualType;
		m_ManualType = (EManualType)UtilFunc::Transformation::Clamp(m_ManualType + 1, 0, EManualType::TYPE_MAX - 1);
		bChange = true;

		// �E�X�N���[��
		m_scrollDir = EScrollDir::SCROLL_R;

		if (old != m_ManualType)
		{
			PLAY_SOUND(CSound::ELabel::LABEL_SE_NEXT);
		}
		else
		{
			PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_HIGH);
		}
	}
	else if (pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_LEFT).bInput)
	{// �O�y�[�W��

		EManualType old = m_ManualType;
		m_ManualType = (EManualType)UtilFunc::Transformation::Clamp(m_ManualType - 1, 0, EManualType::TYPE_MAX - 1);
		bChange = true;

		// ���X�N���[��
		m_scrollDir = EScrollDir::SCROLL_L;

		if (old != m_ManualType)
		{
			PLAY_SOUND(CSound::ELabel::LABEL_SE_BACK);
		}
		else
		{
			PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_HIGH);
		}
	}

	// �ύX����Ă������Ԑݒ�
	if (bChange &&
		m_ManualType != oldType)
	{
		// �o��
		SetState(EState::STATE_SPAWN);

		// ���̈ʒu
		m_pManualText->SetOriginPosition(m_pManualText->GetPosition());
	}

	if (m_state != EState::STATE_FADEIN &&
		pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_A).bInput)
	{// �E�B���h�E����

		if (!GET_MANAGER->GetFade()->IsFade())
		{
			PLAY_SOUND(CSound::ELabel::LABEL_SE_DECIDE_00);
		}

		// �J��
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_ENTRY);
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CTutorialScreen::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}
