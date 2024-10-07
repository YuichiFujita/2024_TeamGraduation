//=============================================================================
// 
// �^�C�g���G���^�[���� [title_select.cpp]
// Author : Ibuki Okusada
// 
//=============================================================================
#include "title_select.h"
#include "title.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
#include "input.h"
#include "fade.h"
#include "keyconfig.h"
#include "title_pressenter.h"
#include "title_optionselect.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float TIME_TUTORIAL_FADEOUT = 0.3f;	// �`���[�g���A���m�F�̃t�F�[�h�A�E�g
	const MyLib::Vector3 SET_POS = MyLib::Vector3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.85f, 0.0f);
	const float HEIGHT = 70.0f;
	const D3DXCOLOR COL_NONESELECT = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);

	D3DXCOLOR GetColorNonSelect(float alpha) {
		return D3DXCOLOR(0.4f, 0.4f, 0.4f, alpha);
	}
}

namespace FILENAME
{
	const std::string TEXTURE[CTitle_Select::SELECT_MAX] =
	{
		"data\\TEXTURE\\title\\start.png",
		"data\\TEXTURE\\title\\option.png",
	};

	const std::string BG = "data\\TEXTURE\\ui_setting\\front.png";
}

namespace StateTime
{
	const float FADE = 0.3f;
	const float DRAW = 0.15f;
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CTitle_Select::STATE_FUNC CTitle_Select::m_StateFunc[] =
{
	&CTitle_Select::StateNone,		// �Ȃ�
	&CTitle_Select::StateFadeIn,	// �t�F�[�h�C��
	&CTitle_Select::StateFadeOut,	// �t�F�[�h�A�E�g
	&CTitle_Select::StateTutorial_FadeOut,		// �`���[�g���A���m�F�̃t�F�[�h�A�E�g
	&CTitle_Select::StateNoActive,	// �������Ȃ�
	&CTitle_Select::StateSetting,	// �ݒ蒆
	&CTitle_Select::StateBack,		// �߂�
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitle_Select::CTitle_Select(float fadetime) : CObject() , m_fFadeOutTime(fadetime)
{
	// �l�̃N���A
	m_state = STATE::STATE_NONE;	// ���
	m_fStateTime = 0.0f;			// ��ԃJ�E���^�[
	m_fSelectDrawTime = 0.0f;		// �I�����̏�������
	m_nSelect = 0;

	for (int i = 0; i < SELECT_MAX; i++)
	{
		m_ap2D[i] = nullptr;
	}
	m_pOptionSelect = nullptr;	// �I�v�V�����̑I����

	m_pSelect = nullptr;
	m_bPress = false;
}

//==========================================================================
// ��������
//==========================================================================
CTitle_Select* CTitle_Select::Create(float fadetime)
{
	// �������̊m��
	CTitle_Select* pScreen = DEBUG_NEW CTitle_Select(fadetime);

	if (pScreen != nullptr)
	{
		// ����������
		pScreen->Init();
	}

	return pScreen;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTitle_Select::Init()
{
	// ��ސݒ�
	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);


	CTexture* pTexture = CTexture::GetInstance();

	MyLib::Vector3 pos = SET_POS;

	//=============================
	// �w�i�𐶐�
	//=============================
	m_pSelect = CObject2D::Create(8);
	m_pSelect->SetType(CObject::TYPE_OBJECT2D);
	m_pSelect->SetPosition(pos);
	m_pSelect->SetAlpha(0.0f);
	m_pSelect->SetAnchorType(CObject2D::AnchorPoint::LEFT);

	// �e�N�X�`���ݒ�
	m_pSelect->BindTexture(pTexture->Regist(FILENAME::BG));
	D3DXVECTOR2 texture = pTexture->GetImageSize(m_pSelect->GetIdxTexture());
	D3DXVECTOR2 setsize = UtilFunc::Transformation::AdjustSizeByHeight(texture, HEIGHT * 1.5f);

	m_pSelect->SetSize(D3DXVECTOR2(0.0f, setsize.y));
	m_pSelect->SetSizeOrigin(setsize);

	//=============================
	// �I�����𐶐�
	//=============================
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] == nullptr)
		{
			// ����
			m_ap2D[i] = CObject2D::Create(8);
			CObject2D* pObj2D = m_ap2D[i];

			pObj2D->SetType(CObject::TYPE_OBJECT2D);
			pObj2D->SetPosition(pos);
			pObj2D->SetAlpha(0.0f);

			// �e�N�X�`���ݒ�
			pObj2D->BindTexture(pTexture->Regist(FILENAME::TEXTURE[i]));
			D3DXVECTOR2 texture = pTexture->GetImageSize(pObj2D->GetIdxTexture());
			pObj2D->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, HEIGHT));
			pObj2D->SetSizeOrigin(pObj2D->GetSize());
		}

		pos.x += SCREEN_WIDTH * 0.4f;
	}

	// ��ԑJ��
	SetState(STATE::STATE_FADEIN);
	m_pSelect->SetPosition(m_ap2D[m_nSelect]->GetPosition() - MyLib::Vector3(m_pSelect->GetSizeOrigin().x, 0.0f, 0.0f));

	// �F�ݒ�
	for (int i = 0; i < SELECT_MAX; i++)
	{
		D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_ap2D[i]->GetAlpha());
		if (m_nSelect != i)
		{
			col = GetColorNonSelect(col.a);
		}
		m_ap2D[i]->SetColor(col);
	}

	// �I�v�V�����I�����̐���
	if (m_pOptionSelect == nullptr)
	{
		m_pOptionSelect = CTitle_OptionSelect::Create();
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTitle_Select::Uninit()
{
	for (int i = 0; i < SELECT_MAX; i++)
	{
		m_ap2D[i] = nullptr;
	}
	m_pSelect = nullptr;

	Release();
}

//==========================================================================
// �폜
//==========================================================================
void CTitle_Select::Kill()
{
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] != nullptr)
		{
			m_ap2D[i]->Uninit();
			m_ap2D[i] = nullptr;
		}
	}

	if (m_pSelect != nullptr)
	{
		m_pSelect->Uninit();
		m_pSelect = nullptr;
	}

	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTitle_Select::Update()
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// �t�F�[�h���͔�����
		return;
	}

	// ��ԑJ�ڃJ�E���^�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// �I����������
	DrawSelect();

	// ��ԕʍX�V����
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// �I����������
//==========================================================================
void CTitle_Select::DrawSelect()
{
	// �������ԉ��Z
	m_fSelectDrawTime += CManager::GetInstance()->GetDeltaTime();

	// �����悤�Ɋg�k
	D3DXVECTOR2 size = m_pSelect->GetSize(), sizeOrigin = m_pSelect->GetSizeOrigin();
	size.x = UtilFunc::Correction::EaseInExpo(0.0f, sizeOrigin.x, 0.0f, StateTime::DRAW, m_fSelectDrawTime);
	m_pSelect->SetSize(size);

	// UV���W�ݒ�
	D3DXVECTOR2* pTex = m_pSelect->GetTex();
	pTex[1].x = pTex[3].x = UtilFunc::Transformation::Clamp(size.x / sizeOrigin.x, 0.0f, 1.0f);
}


//==========================================================================
// �Ȃɂ��Ȃ�
//==========================================================================
void CTitle_Select::StateNone()
{
	// ���͏��擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();
	CKeyConfig* pKeyConfigPad = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKeyConfigKey = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INKEY);

	if (m_bPress)
	{
		if (!pKeyConfigPad->GetPress(INGAME::ACT_BACK))
		{
			m_bPress = false;
		}
		return;
	}

	//=============================
	// ��������
	//=============================
	
	bool bLeft = (pInputGamepad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pInputGamepad->GetStickMoveL(0).x < 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		pInputKeyboard->GetTrigger(DIK_A);

	bool bRight = (pInputGamepad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pInputGamepad->GetStickMoveL(0).x > 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		pInputKeyboard->GetTrigger(DIK_D);
	
	if (bLeft || bRight)
	{
		// �؂�ւ�
		m_nSelect ^= 1;
		m_pSelect->SetPosition(m_ap2D[m_nSelect]->GetPosition() - MyLib::Vector3(m_pSelect->GetSizeOrigin().x, 0.0f, 0.0f));

		// �������ԃ��Z�b�g
		m_fSelectDrawTime = 0.0f;
		m_pSelect->SetSize(D3DXVECTOR2(0.0f, m_pSelect->GetSize().y));

		// �F�ݒ�
		for (int i = 0; i < SELECT_MAX; i++)
		{
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_ap2D[i]->GetAlpha());
			if (m_nSelect != i)
			{
				col = GetColorNonSelect(col.a);
			}
			m_ap2D[i]->SetColor(col);
		}

		// �T�E���h�Đ�
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_CURSOR_MOVE);
		return;
	}

	//=============================
	// ����
	//=============================
	if (pKeyConfigPad->GetTrigger(INGAME::ACT_OK) || pKeyConfigKey->GetTrigger(INGAME::ACT_OK))
	{
		switch (m_nSelect)
		{
		case SELECT::SELECT_START:	// �Q�[���J�n
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE::MODE_GAME);
			break;

		case SELECT::SELECT_OPTION:	// �I�v�V����
			
			// �I�v�V�������莞�̐ݒ�
			DecideOptionSetting();	


			// ��ԑJ��
			SetState(STATE::STATE_FADEOUT);
			break;
		}
	}

	//=============================
	// �߂�
	//=============================
	if (pKeyConfigPad->GetTrigger(INGAME::ACT_BACK) || pKeyConfigKey->GetTrigger(INGAME::ACT_BACK))
	{
		SetState(STATE::STATE_BACK);

		// �v���X�G���^�[�Ăі߂�
		CTitle_PressEnter* pEnter = CTitle::GetInstance()->GetTitlePressEnter();
		if (pEnter != nullptr)
		{
			pEnter->SetState(CTitle_PressEnter::STATE::STATE_FADEIN);
		}

	}
}

//==========================================================================
// �I�v�V�������莞�̐ݒ�
//==========================================================================
void CTitle_Select::DecideOptionSetting()
{
	// ��ԑJ��
	SetState(STATE::STATE_FADEOUT);

	// �I�v�V�����I�����̏�Ԑݒ�
	if (m_pOptionSelect != nullptr)
	{
		m_pOptionSelect->SetState(CTitle_OptionSelect::STATE::STATE_SCROLLWAIT);
	}

}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CTitle_Select::StateFadeIn()
{
	// �s�����x�X�V
	float alpha = m_fStateTime / StateTime::FADE;
	
	if (m_fStateTime >= StateTime::FADE)
	{
		// ��ԑJ��
		SetState(STATE::STATE_NONE);
		alpha = 1.0f;
	}


	// �w�i�̕s�����x
	m_pSelect->SetAlpha(alpha);

	// �I�����̕s�����x
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] != nullptr)
		{
			m_ap2D[i]->SetAlpha(alpha);
		}
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CTitle_Select::StateFadeOut()
{
	// �s�����x�X�V
	float alpha = 1.0f - (m_fStateTime / StateTime::FADE);


	if (m_fStateTime >= StateTime::FADE)
	{
		// ��ԑJ��
		SetState(STATE::STATE_SETTING);
		alpha = 0.0f;
	}

	// �w�i�̕s�����x
	m_pSelect->SetAlpha(alpha);

	// �I�����̕s�����x
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] != nullptr)
		{
			m_ap2D[i]->SetAlpha(alpha);
		}
	}
}

//==========================================================================
// �`���[�g���A���m�F�̃t�F�[�h�A�E�g
//==========================================================================
void CTitle_Select::StateTutorial_FadeOut()
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, TIME_TUTORIAL_FADEOUT, m_fStateTime);

	if (m_fStateTime >= TIME_TUTORIAL_FADEOUT)
	{
		m_fStateTime = 0.0f;
		m_state = STATE_NONE;
	}
}


//==========================================================================
// �������Ȃ�
//==========================================================================
void CTitle_Select::StateNoActive()
{
	
}

//==========================================================================
// �ݒ蒆
//==========================================================================
void CTitle_Select::StateSetting()
{
	m_bPress = true;
}

//==========================================================================
// �߂�
//==========================================================================
void CTitle_Select::StateBack()
{
	// �s�����x�X�V
	float alpha = 1.0f - (m_fStateTime / StateTime::FADE);
	alpha = UtilFunc::Transformation::Clamp(alpha, 0.0f, 1.0f);

	if (m_fStateTime >= StateTime::FADE)
	{
		SetState(STATE::STATE_NOACTIVE);
	}

	// �w�i�̕s�����x
	m_pSelect->SetAlpha(alpha);

	// �I�����̕s�����x
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] != nullptr)
		{
			m_ap2D[i]->SetAlpha(alpha);
		}
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CTitle_Select::SetState(STATE state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// �`��
//==========================================================================
void CTitle_Select::Draw()
{
	return;
}