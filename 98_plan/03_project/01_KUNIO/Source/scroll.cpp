//=============================================================================
// 
//  ���������� [scroll.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "scroll.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "sound.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_ROLL = "data\\TEXTURE\\scroll\\scroll_roll.png";		// ���[�������̃e�N�X�`��
	const std::string TEXTURE_PAPER = "data\\TEXTURE\\scroll\\scroll_paper.png";	// �������̃e�N�X�`��
	const std::string TEXTURE_EDGE = "data\\TEXTURE\\scroll\\scroll_edge.png";		// �[�����̃e�N�X�`��
}

namespace StateTime	// ��ԕʎ���
{
	const float OPEN = 0.75f;	// �I�[�v��
	const float CLOSE = 0.75f;	// �N���[�Y
	const float FADEOUT = 0.3f;	// �t�F�[�h�A�E�g
}


//==========================================================================
// �֐��|�C���^
//==========================================================================
CScroll::STATE_FUNC CScroll::m_StateFunc[] =
{
	&CScroll::StateNone,	// �Ȃ�
	&CScroll::StateOpen,	// �I�[�v��
	&CScroll::StateWait,	// �ҋ@
	&CScroll::StateWaitPress,	// �����ҋ@
	&CScroll::StateClose,	// �N���[�Y
	&CScroll::StateFadeout,	// �t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CScroll::CScroll(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_state = STATE::STATE_NONE;	// ���
	m_fStateTimer = 0.0f;			// ��ԃ^�C�}�[
	m_pPapaer = nullptr;			// ������
	m_pEdge = nullptr;				// �[����
	m_bFinishOpen = false;			// �I�[�v���I������
	m_bAutoWaitPress = false;		// ���������ҋ@����
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CScroll::~CScroll()
{

}

//==========================================================================
// ��������
//==========================================================================
CScroll* CScroll::Create(const MyLib::Vector3& pos, const float toOpenTime, const float height, const float scrollLength, bool bAutoWaitPress, bool bFadeOut, int nPriority)
{
	// �������̊m��
	CScroll* pObj = DEBUG_NEW CScroll(nPriority + 1);

	if (pObj != nullptr)
	{
		// �������ݒ�
		pObj->SetPosition(pos);								// �ʒu
		pObj->SetOriginPosition(pos);						// �ʒu
		pObj->SetSizeOrigin(D3DXVECTOR2(height, height));	// �T�C�Y
		pObj->m_fToOpenTimer = toOpenTime;					// �I�[�v���܂ł̎���
		pObj->m_fScrollLength = scrollLength;				// �������̒���
		pObj->m_bFadeOut = bFadeOut;						// �t�F�[�h�A�E�g����
		pObj->m_bAutoWaitPress = bAutoWaitPress;			// ���������ҋ@����

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CScroll::Init()
{
	// ����������
	CreatePaper();

	// �[��������
	CreateEdge();


	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = pTexture->Regist(TEXTURE_ROLL);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = pTexture->GetImageSize(texID);

	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, GetSizeOrigin().y);
	SetSize(size);
	SetSizeOrigin(size);

	// ���
	m_state = STATE::STATE_NONE;
	m_fStateTimer = 0.0f;

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
void CScroll::CreatePaper()
{
	// ������
	m_pPapaer = CObject2D::Create(GetPriority() - 1);
	if (m_pPapaer == nullptr) return;

	// ��ނ̐ݒ�
	m_pPapaer->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = pTexture->Regist(TEXTURE_PAPER);
	m_pPapaer->BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = pTexture->GetImageSize(texID);

	// ���[�������̏c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, GetSizeOrigin().y);
	size.x = 0.0f;
	m_pPapaer->SetSize(size);
	m_pPapaer->SetSizeOrigin(size);

	// �ʒu�ݒ�
	MyLib::Vector3 rollpos = GetPosition();
	m_pPapaer->SetPosition(rollpos);
}

//==========================================================================
// �[��������
//==========================================================================
void CScroll::CreateEdge()
{
	// �[����
	m_pEdge = CObject2D::Create(GetPriority());
	if (m_pEdge == nullptr) return;

	// ��ނ̐ݒ�
	m_pEdge->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = pTexture->Regist(TEXTURE_EDGE);
	m_pEdge->BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = pTexture->GetImageSize(texID);

	// ���[�������̏c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, GetSizeOrigin().y);
	m_pEdge->SetSize(size);
	m_pEdge->SetSizeOrigin(size);

	// �ʒu�ݒ�
	MyLib::Vector3 rollpos = GetPosition();
	m_pEdge->SetPosition(rollpos + MyLib::Vector3(GetSize().x, 0.0f, 0.0f));

}

//==========================================================================
// �I������
//==========================================================================
void CScroll::Uninit()
{
	m_pPapaer = nullptr;
	m_pEdge = nullptr;

	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CScroll::Kill()
{
	// �������̏I��
	if (m_pPapaer != nullptr)
	{
		m_pPapaer->Uninit();
		m_pPapaer = nullptr;
	}

	// �[�����̏I��
	if (m_pEdge != nullptr)
	{
		m_pEdge->Uninit();
		m_pEdge = nullptr;
	}

	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CScroll::Update()
{
	// �t���O���Z�b�g
	ResetFlag();

	// ��ԍX�V
	UpdateState();

	if (IsDeath()) return;

	// ���[�������̒���
	AdjustRoll();

	// �[�����̒���
	AdjustEdge();

	// �I������
	CObject2D::Update();
}

//==========================================================================
// �t���O���Z�b�g
//==========================================================================
void CScroll::ResetFlag()
{
	m_bFinishOpen = false;
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CScroll::UpdateState()
{
	// ��ԃ^�C�}�[
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// �Ȃɂ��Ȃ�
//==========================================================================
void CScroll::StateNone()
{
	// �I�[�v���X�L�b�v
	OpenSkip();

	if (m_fStateTimer >= m_fToOpenTimer)
	{// �I�[�v���܂ł̎��Ԍo��

		// ��ԑJ��
		SetState(STATE::STATE_OPEN);
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_OPEN);
	}
}

//==========================================================================
// �I�[�v��
//==========================================================================
void CScroll::StateOpen()
{
	// �I�[�v���X�L�b�v
	OpenSkip();

	// �T�C�Y�擾
	D3DXVECTOR2 size = m_pPapaer->GetSize();

	// ���`���
	size.x = UtilFunc::Correction::EasingEaseOut(m_pPapaer->GetSizeOrigin().x, m_fScrollLength * 0.5f, 0.0f, StateTime::OPEN, m_fStateTimer);

	// �T�C�Y�ݒ�
	m_pPapaer->SetSize(size);


	if (m_fStateTimer >= StateTime::OPEN)
	{// ���Ԍo��

		// ��ԑJ��
		if (m_bAutoWaitPress)
		{
			SetState(STATE::STATE_WAITPRESS);
		}
		else
		{
			SetState(STATE::STATE_WAIT);
		}

		// �I�[�v���I������
		m_bFinishOpen = true;
	}
}

//==========================================================================
// �ҋ@
//==========================================================================
void CScroll::StateWait()
{

}

//==========================================================================
// �����ҋ@
//==========================================================================
void CScroll::StateWaitPress()
{
	// �C���v�b�g�擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0))
	{
		// ��ԑJ��
		SetState(STATE::STATE_CLOSE);
	}
}

//==========================================================================
// �N���[�Y
//==========================================================================
void CScroll::StateClose()
{
	// �T�C�Y�擾
	D3DXVECTOR2 size = m_pPapaer->GetSize();

	// ���`���
	size.x = UtilFunc::Correction::EasingEaseIn(m_fScrollLength * 0.5f, 0.0f, 0.0f, StateTime::CLOSE, m_fStateTimer);

	// �T�C�Y�ݒ�
	m_pPapaer->SetSize(size);

	if (m_fStateTimer >= StateTime::CLOSE)
	{// ���Ԍo��

		if (m_bFadeOut)
		{
			SetState(STATE::STATE_FADEOUT);
		}
		else
		{
			Kill();
		}
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CScroll::StateFadeout()
{

	// �s�����x�ݒ�
	m_pPapaer->SetAlpha(UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTimer));

	if (m_fStateTimer >= StateTime::FADEOUT)
	{// ���Ԍo��
		Kill();
	}
}

//==========================================================================
// ���[�������̒���
//==========================================================================
void CScroll::AdjustRoll()
{
	// �ʒu�擾
	MyLib::Vector3 rollPos = GetPosition(), rollPosOrigin = GetOriginPosition();

	// ���`���
	rollPos.x = rollPosOrigin.x - m_pPapaer->GetSize().x;

	// �ʒu�ݒ�
	SetPosition(rollPos);

	// �s�����x�ݒ�
	SetAlpha(m_pPapaer->GetAlpha());

	m_pPapaer->SetVtx();
}

//==========================================================================
// �[�����̒���
//==========================================================================
void CScroll::AdjustEdge()
{
	// �ʒu�擾
	MyLib::Vector3 edgePos = m_pEdge->GetPosition(), rollPosOrigin = GetOriginPosition();

	// ���`���
	edgePos.x = rollPosOrigin.x + m_pPapaer->GetSize().x;

	// �ʒu�ݒ�
	m_pEdge->SetPosition(edgePos);

	// �s�����x�ݒ�
	m_pEdge->SetAlpha(m_pPapaer->GetAlpha());

	m_pEdge->SetVtx();
}

//==========================================================================
// �I�[�v���X�L�b�v
//==========================================================================
void CScroll::OpenSkip()
{
	// �C���v�b�g�擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �X�L�b�v
	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0))
	{
		// ��ԑJ��
		SetState(STATE::STATE_OPEN);
		m_fStateTimer = StateTime::OPEN;
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CScroll::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CScroll::SetState(const STATE& state)
{
	m_state = state;
	m_fStateTimer = 0.0f;
}
