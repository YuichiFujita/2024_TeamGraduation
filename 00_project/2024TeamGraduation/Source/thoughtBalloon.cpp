//=============================================================================
// 
//  ���������o������ [thoughtBalloon.cpp]
//  Author : ���c�E��
// 
//=============================================================================
#include "thoughtBalloon.h"
#include "renderTexture.h"
#include "balloonFrame.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
#if 0
	namespace text
	{
		const char*	FONT = "data\\FONT\\�ʂ˂�������������v7��.ttf";	// �t�H���g�p�X
		const int	PRIORITY	= 6;			// �e�L�X�g�̗D�揇��
		const bool	ITALIC		= false;		// �C�^���b�N
		const float	CHAR_HEIGHT	= 42.0f;		// �����c��
		const float	LINE_HEIGHT	= 54.0f;		// �s�ԏc��
		const float	WAIT_TIME	= 0.045f;		// �����\���̑ҋ@����
		const EAlignX ALIGN_X	= XALIGN_LEFT;	// ���z�u
		const EAlignY ALIGN_Y	= YALIGN_TOP;	// �c�z�u
	}
#endif
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CThoughtBalloon::CThoughtBalloon(int nPriority) : CObjectBillboard(nPriority),
	m_pRenderScene	(nullptr)	// �V�[�������_�[�e�N�X�`��
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CThoughtBalloon::~CThoughtBalloon()
{

}

//==========================================================================
// ��������
//==========================================================================
CThoughtBalloon* CThoughtBalloon::Create()
{
	// �������̊m��
	CThoughtBalloon* pObj = DEBUG_NEW CThoughtBalloon;
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
HRESULT CThoughtBalloon::Init()
{
	// �I�u�W�F�N�g�r���{�[�h�̏�����
	if (FAILED(CObjectBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �����_�[�e�N�X�`���̐���
	if (FAILED(CreateRenderTexture()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �e�N�X�`���p�I�u�W�F�N�g�̐���
	if (FAILED(CreateTextureObject()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �e�N�X�`���C���f�b�N�X�̐ݒ�
	BindTexture(m_pRenderScene->GetTextureIndex());

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CThoughtBalloon::Uninit()
{
	// �����_�[�e�N�X�`���̔j��
	SAFE_REF_RELEASE(m_pRenderScene);

	// �I�u�W�F�N�g�r���{�[�h�̏I��
	CObjectBillboard::Uninit();
}

//==========================================================================
// �폜����
//==========================================================================
void CThoughtBalloon::Kill()
{
	// ���g�̏I��
	CThoughtBalloon::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CThoughtBalloon::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �I�u�W�F�N�g�r���{�[�h�̍X�V
	CObjectBillboard::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CThoughtBalloon::Draw()
{
	// �I�u�W�F�N�g�r���{�[�h�̕`��
	CObjectBillboard::Draw();
}

//==========================================================================
// �����_�[�e�N�X�`����������
//==========================================================================
HRESULT CThoughtBalloon::CreateRenderTexture()
{
	// �V�[�������_�[�e�N�X�`���̐���
	m_pRenderScene = CRenderTexture::Create
	( // ����
		CRenderTextureManager::LAYER_BALLOON,				// �`�揇���C���[
		std::bind(&CThoughtBalloon::CreateTexture, this)	// �e�N�X�`���쐬�֐��|�C���^
	);
	if (m_pRenderScene == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �e�N�X�`���p�I�u�W�F�N�g��������
//==========================================================================
HRESULT CThoughtBalloon::CreateTextureObject()
{
#if 0
	// �e�L�X�g�̐���
	m_pText = CScrollText2D::Create
	( // ����
		text::FONT,			// �t�H���g�p�X
		text::ITALIC,		// �C�^���b�N
		VEC3_ZERO,			// ���_�ʒu
		text::WAIT_TIME,	// �����\���̑ҋ@����
		text::CHAR_HEIGHT,	// �����c��
		text::LINE_HEIGHT,	// �s�ԏc��
		text::ALIGN_X,		// ���z�u
		text::ALIGN_Y		// �c�z�u
	);
	if (m_pText == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}
#endif

	return S_OK;
}

//==========================================================================
// �e�N�X�`���쐬����
//==========================================================================
void CThoughtBalloon::CreateTexture()
{

}
