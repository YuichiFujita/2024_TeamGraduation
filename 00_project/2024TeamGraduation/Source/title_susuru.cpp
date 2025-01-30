//=============================================================================
// 
// SUSURU�V�[������ [title_susuru.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "title_susuru.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "dressup.h"
#include "playerManager.h"
#include "titlestudent.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\title\\susuru.png";		// �e�N�X�`��
	const MyLib::Vector2 TEXTSIZE = MyLib::Vector2((SCREEN_WIDTH / (CTitle_SUSURU::NUM_TEXT + 1)) * 0.75f, SCREEN_HEIGHT * 0.5f);	// �T�C�Y
}

namespace StateTime
{
	const float WAIT = 0.2f;		// �ҋ@
	float SCROLL[CTitle_SUSURU::NUM_TEXT] = // ����
	{
		2.7f,	// ���͂���
		3.4f,	// �ł��A�C�Â����񂾂�
		2.1f,	// ���̊w���Ńg�b�v��ڎw�����߂ɂ�
		1.25f,	// �u���e�v�Ȃ��Ⴂ���Ȃ�
		3.2f,	// �X�|�[�c���׋���
		2.3f,	// ���e�Ȃ���Ӗ����Ȃ�
		2.5f,	// �K�����̊w���̒��_
		2.85f,	// ����u���e���v�̍���
		1.5f,	// �҂��Ă����
		4.0f,	// ���̋P�����͂�
	};
	const float END = 2.0f;		// �I��
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CTitle_SUSURU::STATE_FUNC CTitle_SUSURU::m_StateFunc[] =	// ��Ԋ֐�
{
	&CTitle_SUSURU::StateWait,		// �ҋ@
	&CTitle_SUSURU::StateScroll,	// ��������
	&CTitle_SUSURU::StateEnd,		// �I��
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitle_SUSURU::CTitle_SUSURU() : CTitleScene()
{
	// ���
	m_state = EState::STATE_WAIT;		// ���
	m_fStateTime = 0.0f;				// ��Ԏ���

	// ���̑�
	m_nIdxScroll = 0;		// ���镶���C���f�b�N�X
	memset(&m_apText, 0, sizeof(m_apText));	// ����
	m_pSUSURU = nullptr;	// SUSURU����
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTitle_SUSURU::~CTitle_SUSURU()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTitle_SUSURU::Init()
{
	// ����������
	if(FAILED(CTitleScene::Init())) return E_FAIL;

	// �e�L�X�g����
	if (FAILED(CreateText())) return E_FAIL;

	// SUSURU���񐶐�
	if (FAILED(CreateSUSURU())) return E_FAIL;

	// ���镶���C���f�b�N�X
	m_nIdxScroll = -1;

	// BGM�Đ�
	CSound::GetInstance()->PlaySound(CSound::ELabel::LABEL_BGM_TITLE_SUSURU);

	return S_OK;
}

//==========================================================================
// �e�L�X�g����
//==========================================================================
HRESULT CTitle_SUSURU::CreateText()
{
	// ����
	float u = (1.0f / NUM_TEXT);
	for (int i = 0; i < NUM_TEXT; i++)
	{
		// ����
		m_apText[i] = CObject2D::Create(4);
		if (m_apText[i] == nullptr) return E_FAIL;

		// ��ސݒ�
		m_apText[i]->SetType(CObject::TYPE::TYPE_OBJECT2D);
		m_apText[i]->SetAnchorType(CObject2D::AnchorPoint::TOP_CENTER);

		// �e�N�X�`���ݒ�
		CTexture* pTexture = CTexture::GetInstance();
		int texID = CTexture::GetInstance()->Regist(TEXTURE);
		m_apText[i]->BindTexture(texID);

		// �T�C�Y�ݒ�
		m_apText[i]->SetSize(MyLib::Vector2(TEXTSIZE.x, 0.0f));
		m_apText[i]->SetSizeOrigin(TEXTSIZE);

		// �ʒu�ݒ�
		m_apText[i]->SetPosition(MyLib::Vector3(-TEXTSIZE.x * 0.5f, 0.0f, 0.0f) + MyLib::Vector3(SCREEN_WIDTH - TEXTSIZE.x * 1.5f * i, 0.0f, 0.0f));

		// UV
		D3DXVECTOR2* pTex = m_apText[i]->GetTex();
		pTex[0].x = pTex[2].x = 0.9f - u * i;
		pTex[1].x = pTex[3].x = 0.9f - (u * i) + u;
		int nnn = 0;
	}
	return S_OK;
}

//==========================================================================
// SUSURU���񐶐�
//==========================================================================
HRESULT CTitle_SUSURU::CreateSUSURU()
{

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTitle_SUSURU::Uninit()
{
	delete this;
}

//==========================================================================
// �폜����
//==========================================================================
void CTitle_SUSURU::Kill()
{
	// �e�L�X�g
	for (int i = 0; i < CTitle_SUSURU::NUM_TEXT; i++)
	{
		SAFE_KILL(m_apText[i]);
	}

	// SUSURU
	SAFE_KILL(m_pSUSURU);

	// �I������
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTitle_SUSURU::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (GET_MANAGER->GetInstantFade()->GetState() == CInstantFade::EState::STATE_FADEIN)
	{// �t�F�[�hIN�͔�����
		return;
	}

	// �e�̍X�V
	CTitleScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_A).bInput ||
		pKey->GetTrigger(DIK_RETURN))
	{
		// �V�[���؂�ւ�
		CTitle::GetInstance()->ChangeScene(CTitle::ESceneType::SCENETYPE_CONTROLLWAIT);
	}

#if _DEBUG

	for(int i = 0; i < CTitle_SUSURU::NUM_TEXT; i++)
	{
		std::string str = "SCROLL[" + std::to_string(i) + "]";
		ImGui::DragFloat(str.c_str(), &StateTime::SCROLL[i], 0.01f);
	}

	if (ImGui::Button("Reset"))
	{
		// ���镶���C���f�b�N�X
		m_nIdxScroll = -1;

		// ���
		m_state = EState::STATE_WAIT;	// ���
		m_fStateTime = 0.0f;			// ��Ԏ���

		// BGM�Đ�
		CSound::GetInstance()->StopSound(CSound::ELabel::LABEL_SE_AI);

		for (const auto& text : m_apText)
		{
			// �T�C�Y�ݒ�
			MyLib::Vector2 size = text->GetSize();
			size.y = 0.0f;
			text->SetSize(size);
		}
	}

#endif // _DEBUG

}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CTitle_SUSURU::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += fDeltaTime * fSlowRate;

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �ҋ@
//==========================================================================
void CTitle_SUSURU::StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fStateTime >= StateTime::WAIT)
	{
		m_fStateTime -= StateTime::WAIT;
		m_state = EState::STATE_SCROLL;

		if (m_nIdxScroll < 0)
		{// AI�ǂݏグ�J�n
			PLAY_SOUND(CSound::ELabel::LABEL_SE_AI);
		}

		// ���镶���X�V
		m_nIdxScroll++;

		if (m_nIdxScroll >= CTitle_SUSURU::NUM_TEXT)
		{// �S�čX�V

			// �I�����
			m_state = EState::STATE_END;	// ���
			m_fStateTime = 0.0f;			// ��Ԏ���
		}
	}
}

//==========================================================================
// ��������
//==========================================================================
void CTitle_SUSURU::StateScroll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��������
	MyLib::Vector2 size = m_apText[m_nIdxScroll]->GetSize();

	// �T�C�Y�ݒ�
	size.y = UtilFunc::Correction::EasingLinear(0.0f, TEXTSIZE.y, 0.0f, StateTime::SCROLL[m_nIdxScroll], m_fStateTime);
	m_apText[m_nIdxScroll]->SetSize(size);


	// UV���T�C�Y�ɍ��킹��
	D3DXVECTOR2* pTex = m_apText[m_nIdxScroll]->GetTex();
	pTex[2].y = pTex[3].y = m_apText[m_nIdxScroll]->GetSize().y / m_apText[m_nIdxScroll]->GetSizeOrigin().y;

	if (m_fStateTime >= StateTime::SCROLL[m_nIdxScroll])
	{
		m_fStateTime -= StateTime::SCROLL[m_nIdxScroll];
		m_state = EState::STATE_WAIT;
	}
}

//==========================================================================
// �I��
//==========================================================================
void CTitle_SUSURU::StateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fStateTime >= StateTime::END)
	{
		CTitle::GetInstance()->ChangeScene(CTitle::ESceneType::SCENETYPE_CONTROLLWAIT);
	}
}
