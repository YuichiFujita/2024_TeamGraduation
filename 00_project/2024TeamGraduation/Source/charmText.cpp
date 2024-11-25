//==========================================================================
// 
//  ���e�������� [charmText.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "charmText.h"
#include "gameManager.h"
#include "player.h"
#include "object2D.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_FACE = "data\\TEXTURE\\faceicon\\000.png";			// ��A�C�R���̃e�N�X�`��
	const std::string TEXTURE_TEXT = "data\\TEXTURE\\charmtext\\window_full.png";	// �e�L�X�g�̃e�N�X�`��

	const float SIZE_FACE = 50.0f;	// ��A�C�R���̃T�C�Y
	const MyLib::Vector3 OFFSET_TEXT = MyLib::Vector3(SIZE_FACE, 0.0f, 0.0f);	// �e�L�X�g�̃I�t�Z�b�g�ʒu
}


namespace StateTime
{
	const float FADEIN = 0.2f;	// �t�F�[�h�C��
	const float FADEOUT = 0.2f;	// �t�F�[�h�C��
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CCharmText::STATE_FUNC CCharmText::m_StateFunc[] =	// �V�[���֐�
{
	&CCharmText::StateFadeIn,	// �t�F�[�h�C��
	&CCharmText::StateWait,		// �ҋ@
	&CCharmText::StateFadeOut,	// �t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCharmText::CCharmText(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_state(EState::STATE_FADEIN),	// ���
	m_fStateTime(0.0f)				// ��ԃ^�C�}�[
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCharmText::~CCharmText()
{

}

//==========================================================================
// ��������
//==========================================================================
CCharmText* CCharmText::Create()
{
	// �������̊m��
	CCharmText* pMarker = DEBUG_NEW CCharmText;

	if (pMarker != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pMarker->Init()))
		{ // �������Ɏ��s�����ꍇ
			SAFE_UNINIT(pMarker);
			return nullptr;
		}
	}

	return pMarker;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCharmText::Init()
{
	// �I�u�W�F�N�g�̎�ސݒ�
	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// ��A�C�R������
	if (FAILED(CreateFace()))
	{
		return E_FAIL;
	}

	// ��������
	if (FAILED(CreateText()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// ��A�C�R������
//==========================================================================
HRESULT CCharmText::CreateFace()
{
	// ��������
	m_pFace = CObject2D::Create(GetPriority());
	if (m_pFace == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pFace->CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_FACE);
	m_pFace->BindTexture(texID);

	// �T�C�Y�ݒ�
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, SIZE_FACE);
	m_pFace->SetSize(size);
	m_pFace->SetSizeOrigin(size);

	return S_OK;
}

//==========================================================================
// ��������
//==========================================================================
HRESULT CCharmText::CreateText()
{
	// ��������
	m_pText = CObject2D::Create(GetPriority());
	if (m_pText == nullptr) return E_FAIL;

	// ���[���A���J�[�|�C���g�ɂ���
	m_pText->SetAnchorType(CObject2D::AnchorPoint::LEFT);

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pText->CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_TEXT);
	m_pText->BindTexture(texID);

	// �T�C�Y�ݒ�
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_FACE);
	m_pText->SetSize(size);
	m_pText->SetSizeOrigin(size);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCharmText::Uninit()
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �폜����
//==========================================================================
void CCharmText::Kill()
{
	// ��A�C�R��
	if (m_pFace != nullptr)
	{
		m_pFace->Kill();
		m_pFace = nullptr;
	}

	// ����
	if (m_pText != nullptr)
	{
		m_pText->Kill();
		m_pText = nullptr;
	}

	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CCharmText::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	MyLib::Vector3 pos = GetPosition();

	// �ʒu�ݒ�
	m_pFace->SetPosition(pos);
	m_pText->SetPosition(pos + OFFSET_TEXT);
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CCharmText::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�}�[���Z
	m_fStateTime += fDeltaTime * fSlowRate;

	// �V�[���ʍX�V
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CCharmText::StateFadeIn()
{

	if (m_fStateTime >= StateTime::FADEIN)
	{

	}
}

//==========================================================================
// �ҋ@
//==========================================================================
void CCharmText::StateWait()
{

}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CCharmText::StateFadeOut()
{

}

//==========================================================================
// �`�揈��
//==========================================================================
void CCharmText::Draw()
{

}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CCharmText::SetState(EState state)
{
	m_fStateTime = 0.0f;
	m_state = state;
}

//==========================================================================
// �`��󋵂̐ݒ菈��
//==========================================================================
void CCharmText::SetEnableDisp(bool bDisp)
{
	// ���N���X�̕`��󋵐ݒ�
	CObject::SetEnableDisp(bDisp);

	// �`��󋵐ݒ�
	m_pFace->SetEnableDisp(bDisp);
	m_pText->SetEnableDisp(bDisp);
}
