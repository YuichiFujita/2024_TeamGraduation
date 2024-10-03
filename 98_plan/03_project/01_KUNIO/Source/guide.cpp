//=============================================================================
// 
//  �������� [guide.cpp]
//  Author : �����V����
// 
//=============================================================================
#include "guide.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"
#include "player.h"
#include "baggage.h"
#include "controlkeydisp.h"
#include "keyconfig.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE[] =
	{
		"data\\TEXTURE\\guide\\guide_000.png",	// �J�n
		"data\\TEXTURE\\battlewin\\guide.png",	// �S�[��
	};
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
CGuide::STATE_FUNC CGuide::m_StateFuncList[] =
{
	&CGuide::StateNone,		// �Ȃ�
	&CGuide::StateFadeIn,	// �t�F�[�h�C��
	&CGuide::StateFadeout,	// �t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CGuide::CGuide(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_state = State::STATE_NONE;	// ���
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[
	m_fLifeTimer = 0.0f;	// ��������
	m_pKeyDisp = nullptr;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CGuide::~CGuide()
{

}

//==========================================================================
// ��������
//==========================================================================
CGuide* CGuide::Create(Type type)
{
	// �������̊m��
	CGuide* pObj = DEBUG_NEW CGuide;

	if (pObj != nullptr)
	{
		// �e�N�X�`���ݒ�
		int texID = CTexture::GetInstance()->Regist(TEXTURE[type]);
		pObj->BindTexture(texID);

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CGuide::Init()
{

	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.5f, 0.0f));

	// �e��ϐ�������
	m_state = State::STATE_FADEIN;	// ���
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �c�������ɃT�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(GetIdxTexture());
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, 100);

	// �T�C�Y�ݒ�
	SetSize(size);
	SetSizeOrigin(size);

	// �J�n�{�^��������
	if(m_pKeyDisp == nullptr)
	{
		CKeyConfig* pkeyconfig = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
		MyLib::Vector3 keypos = GetPosition();
		keypos.x -= (size.x * 0.5f + size.y * 0.75f);
		float range = size.y * 0.65f;
		m_pKeyDisp = CControlKeyDisp::Create(keypos, 0.0f, range, range, pkeyconfig->GetKey(INGAME::ACT_AIR));
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CGuide::Uninit()
{
	if (m_pKeyDisp != nullptr)
	{
		m_pKeyDisp = nullptr;
	}

	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �폜����
//==========================================================================
void CGuide::Kill()
{
	if (m_pKeyDisp != nullptr)
	{
		m_pKeyDisp->Uninit();
		m_pKeyDisp = nullptr;
	}

	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CGuide::Update()
{
	// ��ԍX�V
	UpdateState();

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
void CGuide::UpdateState()
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// ���s
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CGuide::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CGuide::StateFadeIn()
{
	float alpha = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, StateTime::FADEIN, m_fStateTime);
	SetAlpha(alpha);

	if (m_pKeyDisp != nullptr)
	{
		m_pKeyDisp->SetAlpha(alpha);
	}

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		SetAlpha(1.0f);

		if (m_pKeyDisp != nullptr)
		{
			m_pKeyDisp->SetAlpha(1.0f);
		}

		// ��ԃ^�C�}�[���Z�b�g
		m_fStateTime = 0.0f;
		m_state = State::STATE_NONE;
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CGuide::StateFadeout()
{
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);
	SetAlpha(alpha);

	if (m_pKeyDisp != nullptr)
	{
		m_pKeyDisp->SetAlpha(alpha);
	}

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
void CGuide::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CGuide::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}