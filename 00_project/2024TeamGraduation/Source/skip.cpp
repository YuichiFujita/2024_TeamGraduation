//============================================================
//
//	�X�L�b�v���쏈�� [skip.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "skip.h"
#include "renderer.h"
#include "manager.h"
#include "sound.h"
#include "fade.h"
#include "texture.h"
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE = "data\\TEXTURE\\skip000.png";	// �X�L�b�v����\���e�N�X�`��
	const int	PRIORITY	 = 6;		// �X�L�b�v����\���̗D�揇��
	const float	DISP_TIME	 = 4.0f;	// ���S�s�����t���[��
	const float	FADE_LEVEL	 = 0.05f;	// �����x�̃t���[���ϓ���
	const float WIDTH		 = 77.0f * 2.4f;	// �X�L�b�v����̕\���傫��
	const MyLib::Vector3 POS = MyLib::Vector3(1092.0f, 720.0f - 673.0f, 0.0f);	// �X�L�b�v����̕\���ʒu
}

//************************************************************
//	�e�N���X [CSkip] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSkip::CSkip() : 
	m_pControl	(nullptr),		// ����̏��
	m_state		(STATE_NONE),	// ���
	m_fCurTime	(0.0f)			// ��ԊǗ��J�E���^�[
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSkip::~CSkip()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSkip::Init()
{
	// �����o�ϐ���������
	m_pControl	= nullptr;		// ����̏��
	m_state		= STATE_NONE;	// ���
	m_fCurTime	= 0.0f;			// ��ԊǗ��J�E���^�[

	// ������̐���
	m_pControl = CObject2D::Create(PRIORITY);
	if (m_pControl == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();	// �e�N�X�`�����
	int texID = pTexture->Regist(TEXTURE_FILE);
	m_pControl->BindTexture(texID);

	// �ʒu��ݒ�
	m_pControl->SetPosition(POS);

	// ���������ɑ傫���ݒ�
	MyLib::Vector2 size = pTexture->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, WIDTH);
	m_pControl->SetSize(size);

	// �����x��ݒ�
	m_pControl->SetAlpha(0.0f);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSkip::Uninit()
{
	// ������̏I��
	SAFE_UNINIT(m_pControl);
}

//============================================================
//	�X�V����
//============================================================
void CSkip::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_state != STATE_NONE)
	{ // �������Ȃ���Ԃł͂Ȃ��ꍇ

		D3DXCOLOR colCont = m_pControl->GetColor();	// ����\���̐F
		switch (m_state)
		{ // ��Ԃ��Ƃ̏���
		case STATE_FADEOUT:

			// ���l�����Z
			colCont.a += FADE_LEVEL * fDeltaRate * fSlowRate;
			if (colCont.a >= 1.0f)
			{ // �s�����ɂȂ����ꍇ

				// �����x��␳
				colCont.a = 1.0f;

				// �\����Ԃɂ���
				m_state = STATE_DISP;
			}

			break;

		case STATE_DISP:

			// �J�E���^�[�����Z
			m_fCurTime += fDeltaTime * fSlowRate;
			if (m_fCurTime > DISP_TIME)
			{ // �\�����Ԃ𒴂����ꍇ

				// �J�E���^�[������
				m_fCurTime = 0;

				// �t�F�[�h�C����Ԃɂ���
				m_state = STATE_FADEIN;
			}

			break;

		case STATE_FADEIN:

			// ���l�����Z
			colCont.a -= FADE_LEVEL * fDeltaRate * fSlowRate;
			if (colCont.a <= 0.0f)
			{ // �����ɂȂ����ꍇ

				// �����x��␳
				colCont.a = 0.0f;

				// �������Ȃ���Ԃɂ���
				m_state = STATE_NONE;
			}

			break;

		default:
			assert(false);
			break;
		}

		// �F�𔽉f
		m_pControl->SetColor(colCont);
	}
}

//============================================================
//	�`�揈��
//============================================================
void CSkip::Draw()
{

}

//============================================================
//	�\���ݒ菈��
//============================================================
void CSkip::SetDisp()
{
	// ���ɕ\�����̏ꍇ������
	if (m_state == STATE_DISP) { return; }

	// �J�E���^�[��������
	m_fCurTime = 0.0f;

	// �t�F�[�h�A�E�g��Ԃɂ���
	m_state = STATE_FADEOUT;
}

//============================================================
//	�\���擾����
//============================================================
bool CSkip::IsDisp()
{
	// �\���󋵂�ݒ�
	bool bDisp = (m_state == STATE_DISP || m_state == EState::STATE_FADEOUT);

	// �\���󋵂�Ԃ�
	return bDisp;
}

//============================================================
//	��������
//============================================================
CSkip *CSkip::Create()
{
	// �X�L�b�v����̐���
	CSkip *pSkip = DEBUG_NEW CSkip;
	if (pSkip == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �X�L�b�v����̏�����
		if (FAILED(pSkip->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �X�L�b�v����̔j��
			SAFE_DELETE(pSkip);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pSkip;
	}
}

//============================================================
//	�j������
//============================================================
void CSkip::Release(CSkip *&prSkip)
{
	// �X�L�b�v����̏I��
	assert(prSkip != nullptr);
	prSkip->Uninit();

	// �������J��
	SAFE_DELETE(prSkip);
}
