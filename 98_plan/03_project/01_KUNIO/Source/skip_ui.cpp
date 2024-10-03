//=============================================================================
// 
//  �X�L�b�vUI���� [skip_ui.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "skip_ui.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "object_circlegauge2D.h"
#include "game.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\key\\B.png";
	const float TIME_FADEOUT_RATIO = 0.3f;
	const float DEFAULT_LIFE = 1.0f;
	const float TIME_PRESS = 1.5f;
}

namespace StateTime
{
	const float FADEOUT = 0.3f;
}


//==========================================================================
// �R���X�g���N�^
//==========================================================================
CSkip_UI::CSkip_UI(int nPriority, CObject::LAYER layer) : CObject(nPriority, layer)
{
	// �l�̃N���A
	m_state = State::STATE_NONE;				// ���
	m_fStateTime = 0.0f;	// ��ԃ^�C�}�[

	m_fPressTimer = 0.0f;			// ��������
	m_bCompleteSkip = false;		// �X�L�b�v����
	m_pButton = nullptr;			// �{�^��
	m_pObjCircleGauge = nullptr;	// �~�Q�[�W
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CSkip_UI::~CSkip_UI()
{

}

//==========================================================================
// ��������
//==========================================================================
CSkip_UI* CSkip_UI::Create()
{
	// �������̊m��
	CSkip_UI* pObj = DEBUG_NEW CSkip_UI;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CSkip_UI::Init()
{
	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �ʒu
	SetPosition(MyLib::Vector3(1100.0f, 670.0f, 0.0f));

	m_state = State::STATE_FADEOUT;	// ���
	m_fStateTime = 0.0f;	// ��ԃ^�C�}�[

// UI����
	CreateUI();

	return S_OK;
}

//==========================================================================
// UI����
//==========================================================================
void CSkip_UI::CreateUI()
{
	CreateButton();
	CreateGauge();
}

//==========================================================================
// �{�^���쐬
//==========================================================================
void CSkip_UI::CreateButton()
{
	if (m_pButton != nullptr)
	{
		m_pButton->SetAlpha(1.0f);
		return;
	}

	// �쐬
	m_pButton = CObject2D::Create(GetPriority());

	// ��ނ̐ݒ�
	m_pButton->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	m_pButton->BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 32.0f);
	m_pButton->SetSize(size);
	m_pButton->SetSizeOrigin(size);

	// �ʒu
	m_pButton->SetPosition(GetPosition());
	m_pButton->SetVtx();
}

//==========================================================================
// �Q�[�W�쐬
//==========================================================================
void CSkip_UI::CreateGauge()
{
	if (m_pObjCircleGauge != nullptr) return;

	// �쐬
	m_pObjCircleGauge = CObjectCircleGauge2D::Create(32, 40.0f);
	m_pObjCircleGauge->SetType(CObject::TYPE::TYPE_OBJECT2D);

	m_pObjCircleGauge->SetPosition(GetPosition());
}

//==========================================================================
// �I������
//==========================================================================
void CSkip_UI::Uninit()
{
	// �������
	Release();
}

//==========================================================================
// �폜
//==========================================================================
void CSkip_UI::Kill()
{
	// UI�폜
	KillUI();

	// �������
	Release();
}

//==========================================================================
// UI�폜
//==========================================================================
void CSkip_UI::KillUI()
{
	if (m_pButton != nullptr)
	{
		m_pButton->Uninit();
		m_pButton = nullptr;
	}

	if (m_pObjCircleGauge != nullptr)
	{
		m_pObjCircleGauge->Uninit();
		m_pObjCircleGauge = nullptr;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CSkip_UI::Update()
{
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pGamepad = CInputGamepad::GetInstance();
	
	float deltaTime = CManager::GetInstance()->GetDeltaTime();

	if (pKey->GetPress(DIK_O) ||
		pGamepad->GetPress(CInputGamepad::BUTTON::BUTTON_B, 0))
	{// ������

		// ��Ԑݒ�
		m_state = State::STATE_NONE;
		m_fStateTime = 0.0f;


		// �^�C�}�[���Z
		m_fPressTimer += deltaTime;
		if (m_fPressTimer >= TIME_PRESS)
		{
			m_bCompleteSkip = true;
			m_fPressTimer = TIME_PRESS;
		}

		// UI�쐬
		CreateButton();
		CreateGauge();

		if (m_pObjCircleGauge != nullptr)
		{
			m_pObjCircleGauge->SetRate(m_fPressTimer / TIME_PRESS);
			m_pObjCircleGauge->SetVtx();
		}

	}
	else
	{// �����ĂȂ�

		// ��Ԑݒ�
		m_state = State::STATE_FADEOUT;

		// �^�C�}�[���Z�b�g
		m_fPressTimer = 0.0f;

		// �Q�[�W���폜
		if (m_pObjCircleGauge != nullptr)
		{
			m_pObjCircleGauge->Uninit();
			m_pObjCircleGauge = nullptr;
		}
	}

	if (m_state == State::STATE_FADEOUT)
	{
		StateFadeout();
	}

	
}


//==========================================================================
// �o�����
//==========================================================================
void CSkip_UI::StateFadeout()
{
	if (m_pButton == nullptr) return;

	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// �s�����x�ݒ�
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);
	m_pButton->SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		// UI�폜
		KillUI();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CSkip_UI::Draw()
{
	
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CSkip_UI::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}
