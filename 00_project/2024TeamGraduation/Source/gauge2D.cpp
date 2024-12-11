//============================================================
//
//	�Q�[�W2D���� [gauge2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gauge2D.h"
#include "manager.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int MAX_VERTEX = 12;	// ���_��
	const int PRIORITY	 = 5;	// �Q�[�W2D�̗D�揇��
	const char* PASS_BAR = "data\\TEXTURE\\gauge\\bar.png";		// �p�X(�Q�[�W)
	const char* PASS_FRAME = "data\\TEXTURE\\gauge\\frame.png";	// �p�X(�g)
}

//************************************************************
//	�q�N���X [CGauge2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGauge2D::CGauge2D(const float nFrame) : CObject(PRIORITY, CObject::LAYER::LAYER_2D),
	m_fFrame			(nFrame),					// �\���l�̕ϓ��t���[���萔
	m_state				(STATE_NONE),				// ���
	m_fNumGauge			(0),						// �\���l
	m_fChange			(0.0f),						// �Q�[�W�ϓ���
	m_fStateTime		(0),						// ��ԊǗ��J�E���^�[
	m_fMaxNumGauge		(0),						// �\���l�̍ő�l
	m_fCurrentNumGauge	(0.0f),						// ���ݕ\���l
	m_bDrawFrame		(false)						// �g�\����
{
	// �����o�ϐ����N���A
	m_pBg = nullptr;					// �w�i
	m_pBar = nullptr;					// �Q�[�W
	m_pFrame = nullptr;					// �t���[��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CGauge2D::~CGauge2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGauge2D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �����o�ϐ���������
	m_state				= STATE_NONE;				// ���
	m_fNumGauge			= 0;						// �\���l
	m_fChange			= 0.0f;						// �Q�[�W�ϓ���
	m_fStateTime		= 0;						// ��ԊǗ��J�E���^�[
	m_fMaxNumGauge		= 0;						// �\���l�̍ő�l
	m_fCurrentNumGauge	= 0.0f;						// ���ݕ\���l
	m_bDrawFrame		= false;					// �g�\����

	m_pBg = CObject2D::Create(PRIORITY);					// �w�i
	m_pBar = CObject2D::Create(PRIORITY);					// �Q�[�W
	m_pFrame = CObject2D::Create(PRIORITY);					// �t���[��

	m_pBg = CObject2D::Create(PRIORITY);					// �w�i
	m_pBar = CObject2D::Create(PRIORITY);					// �Q�[�W
	m_pFrame = CObject2D::Create(PRIORITY);					// �t���[��

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGauge2D::Uninit()
{
	// �|���S���̔j��
	SAFE_UNINIT(m_pBg);
	SAFE_UNINIT(m_pBar);
	SAFE_UNINIT(m_pFrame);

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�폜����
//============================================================
void CGauge2D::Kill()
{
	// ���g�̏I��
	CGauge2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGauge2D::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pBar == nullptr) return;
	MyLib::Vector2 size = m_pBar->GetSize();

	// �Q�[�W�̐ݒ�
	if (m_state == STATE_CHANGE)
	{ // �̗͂��ϓ����̏ꍇ

		// �J�E���^�[�����Z
		m_fStateTime--;

		// ���݂̃Q�[�W�ɕϓ��ʂ����Z
		m_fCurrentNumGauge += m_fChange;

		// ���݂̕\���l��␳
		UtilFunc::Transformation::ValueNormalize(m_fCurrentNumGauge, (float)m_fMaxNumGauge, 0.0f);

		// ���_���̐ݒ�
		m_pBar->SetSize(size);

		if (m_fStateTime <= 0)
		{ // �J�E���^�[�� 0�ȉ��ɂȂ����ꍇ

			// �ʏ��Ԃɂ���
			m_state = STATE_NONE;
		}
	}
}

//============================================================
//	�`�揈��
//============================================================
void CGauge2D::Draw()
{


}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CGauge2D::SetPosition(const MyLib::Vector3& rPos)
{
	if (m_pBg == nullptr ||
		m_pBar == nullptr ||
		m_pFrame == nullptr)
	{// �P�ł��Ȃ�������
		MyAssert::CustomAssert(false, "Gauge2D: �Ȃ�Ŗ�����H");
	}

	// �����̈ʒu��ݒ�
	CObject::SetPosition(rPos);

	// TODO: ���点
	m_pBg->SetPosition(rPos);
	m_pBar->SetPosition(rPos);
	m_pFrame->SetPosition(rPos);
}

//============================================================
//	��������
//============================================================
CGauge2D* CGauge2D::Create
(
	const float fMax,					// �ő�\���l
	const float fFrame,					// �\���l�ϓ��t���[��
	const MyLib::Vector3& rPos,			// �ʒu
	const MyLib::Vector2& rSizeGauge,	// �Q�[�W�傫��
	const D3DXCOLOR& rColFront,			// �\�Q�[�W�F
	const D3DXCOLOR& rColBack,			// ���Q�[�W�F
	const bool bDrawFrame				// �g�`���
)
{
	// �Q�[�W2D�̐���
	CGauge2D* pGauge2D = new CGauge2D(fFrame);
	if (pGauge2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �Q�[�W2D�̏�����
		if (FAILED(pGauge2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �Q�[�W2D�̔j��
			SAFE_DELETE(pGauge2D);
			return nullptr;
		}

		// �e�N�X�`����o�^�E����
		pGauge2D->BindTexture();

		// �Q�[�W�ő�l��ݒ�
		pGauge2D->SetMaxNum(fMax);

		// �ʒu��ݒ�
		pGauge2D->SetPosition(rPos);

		// �傫����ݒ�
		pGauge2D->SetSize(rSizeGauge);		// �S���傫��
		pGauge2D->InitSize();				// �����傫���ݒ�

		// �F��ݒ�
		pGauge2D->SetColorFront(rColFront);	// �\�Q�[�W�F
		pGauge2D->SetColorBack(rColBack);	// ���Q�[�W�F

		// �g�̕\���󋵂�ݒ�
		pGauge2D->SetEnableDrawFrame(bDrawFrame);

		// �m�ۂ����A�h���X��Ԃ�
		return pGauge2D;
	}
}

//============================================================
//	�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CGauge2D::BindTexture()
{
	CTexture* pTex = CTexture::GetInstance();

	// �e�N�X�`�����蓖��
	m_pBg->BindTexture(pTex->Regist(""));
	m_pBar->BindTexture(pTex->Regist(PASS_BAR));
	m_pFrame->BindTexture(pTex->Regist(PASS_FRAME));
}

//============================================================
// �����T�C�Y�ݒ�
//============================================================
void CGauge2D::InitSize()
{
	m_pBg->SetSizeOrigin(m_pFrame->GetSize());
	m_pBar->SetSizeOrigin(m_pFrame->GetSize());
	m_pFrame->SetSizeOrigin(m_pFrame->GetSize());
}

//============================================================
//	�Q�[�W�̉��Z����
//============================================================
void CGauge2D::AddNum(const float fAdd)
{
	// ���݂̕\���l��ݒ�
	m_fCurrentNumGauge = (float)m_fNumGauge;

	// �\���l�̕ϓ��ʂ����߂�
	m_fChange = fAdd / m_fFrame;

	// ����ݒ�
	m_state = STATE_CHANGE;		// �Q�[�W�ϓ����
	m_fStateTime = m_fFrame;	// ��ԊǗ��J�E���^�[

	// �\���l�����������Z
	m_fNumGauge += fAdd;

	// �\���l�̐���
	UtilFunc::Transformation::ValueNormalize(m_fNumGauge, m_fMaxNumGauge, 0.0f);
}

//============================================================
//	�Q�[�W�̉��Z����
//============================================================
void CGauge2D::SubNum(const float fSub)
{
	// ���݂̕\���l��ݒ�
	m_fCurrentNumGauge = (float)m_fNumGauge;

	// �\���l�̕ϓ��ʂ����߂�
	m_fChange = fSub / m_fFrame;

	// ����ݒ�
	m_state = STATE_CHANGE;		// �Q�[�W�ϓ����
	m_fStateTime = m_fFrame;	// ��ԊǗ��J�E���^�[

	// �\���l�����������Z
	m_fNumGauge += fSub;

	// �\���l�̐���
	UtilFunc::Transformation::ValueNormalize(m_fNumGauge, m_fMaxNumGauge, 0.0f);
}

//============================================================
//	�Q�[�W�̐ݒ菈��
//============================================================
void CGauge2D::SetNum(const float nNum)
{
	// �����̕\���l��ݒ�
	m_fNumGauge = nNum;
	UtilFunc::Transformation::ValueNormalize(m_fNumGauge, m_fMaxNumGauge, 0.0f);	// �\���l�̐���

	// ���݂̕\���l��ݒ�
	m_fCurrentNumGauge = m_fNumGauge;
	UtilFunc::Transformation::ValueNormalize(m_fCurrentNumGauge, m_fMaxNumGauge, 0.0f);	// ���݂̕\���l�̐���

	// ����ݒ�
	m_state = STATE_NONE;	// �Q�[�W�ϓ����
	m_fStateTime = 0;	// ��ԊǗ��J�E���^�[
}

//============================================================
//	�g�I�t�Z�b�g�̐ݒ菈��
//============================================================
void CGauge2D::SetAnchorType(const CObject2D::AnchorPoint& type)
{
	// �A���J�[�|�C���g�ݒ�
	m_pBg->SetAnchorType(type);
	m_pBar->SetAnchorType(type);
	m_pFrame->SetAnchorType(type);
}

//============================================================
//	�Q�[�W�傫���̐ݒ菈��
//============================================================
void CGauge2D::SetSize(const MyLib::Vector2& rSize)
{
	m_pBg->SetSize(rSize);
	m_pBar->SetSize(rSize);
	m_pFrame->SetSize(rSize);
}

//============================================================
//	�Q�[�W�傫���̐ݒ菈��
//============================================================
void CGauge2D::SetSizeGauge(const MyLib::Vector2& rSize)
{
	// �T�C�Y
	m_pBar->SetSize(rSize);
}

//============================================================
//	�Q�[�W�傫���̐ݒ菈��
//============================================================
void CGauge2D::SetSizeGaugeRadius(const float fRadius)
{
	// �T�C�Y
	MyLib::Vector2 size = m_pBar->GetSizeOrigin();
	size.x *= fRadius;

	m_pBar->SetSize(size);
}

//============================================================
//	�g�傫���̐ݒ菈��
//============================================================
void CGauge2D::SetSizeFrame(const MyLib::Vector2& rSize)
{
	// �T�C�Y
	m_pFrame->SetSize(rSize);
}

//============================================================
//	�w�i�傫���̐ݒ菈��
//============================================================
void CGauge2D::SetSizeBg(const MyLib::Vector2& rSize)
{
	// �T�C�Y
	m_pBg->SetSize(rSize);
}

//============================================================
//	�\�Q�[�W�F�̐ݒ菈��
//============================================================
void CGauge2D::SetColorFront(const D3DXCOLOR& rCol)
{
	// �����̃Q�[�W�F����
	m_pBar->SetColor(rCol);
}

//============================================================
//	���Q�[�W�F�̐ݒ菈��
//============================================================
void CGauge2D::SetColorBack(const D3DXCOLOR& rCol)
{
	// �����̔w�i�F����
	m_pBg->SetColor(rCol);
}

//============================================================
//	�Q�[�W�ő�l�̐ݒ菈��
//============================================================
void CGauge2D::SetMaxNum(const float fMax)
{
	// �����̕\���ő�l��ݒ�
	m_fMaxNumGauge = fMax;

	// �Q�[�W�̐ݒ�
	SetNum(m_fMaxNumGauge);
}

//============================================================
//	�g�\���󋵐ݒ菈��
//============================================================
void CGauge2D::SetEnableDrawFrame(const bool bDraw)
{
	// �����̘g�̕\���󋵂�ݒ�
	m_bDrawFrame = bDraw;
}
