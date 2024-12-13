//============================================================
//
//	�Q�[�W2D���� [gauge2D.cpp]
//	Author�FKai Takada
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gauge2D.h"
#include "manager.h"
#include "texture.h"
#include "ball.h"
#include "player.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int MAX_VERTEX = 12;	// ���_��
	const int PRIORITY	 = 5;	// �Q�[�W2D�̗D�揇��
	const char* PASS_BAR = "data\\TEXTURE\\gauge\\bar_01.jpg";		// �p�X(�Q�[�W)
	const char* PASS_FRAME = "data\\TEXTURE\\gauge\\frame.png";		// �p�X(�g)
	const char* PASS_ASSIST = "data\\TEXTURE\\gauge\\assist.png";	// �A�V�X�g(�{�^��)
	const float FRAME_RAT = 1.1f;	// �g�̑傫���{��(�Q�[�W�́Z�{)
}

// max������
namespace Bright
{
	const MyLib::PosGrid3 END = MyLib::PosGrid3(318, 80, 100);		// �I���F
	const MyLib::PosGrid3 START = MyLib::PosGrid3(350, 20, 100);	// �J�n�F
	const float END_TIME = 0.5f;									// �I������
}

// max������
namespace Assist
{
	const MyLib::PosGrid3 END = MyLib::PosGrid3(45, 80, 100);		// �I���F
	const MyLib::PosGrid3 START = MyLib::PosGrid3(60, 20, 100);		// �J�n�F
}

//************************************************************
//	�ÓI�֐�
//************************************************************
float CGauge2D::m_fBrightTime = 0.0f;						// max�̎�����F
float CGauge2D::m_fBrightTimeEnd = Bright::END_TIME;		// max�̎�����F�I������

//************************************************************
//	�q�N���X [CGauge2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGauge2D::CGauge2D(const float nFrame) : CObject(PRIORITY, CObject::LAYER::LAYER_2D),
	m_fFrame			(nFrame),									// �\���l�̕ϓ��t���[���萔
	m_team				(CGameManager::ETeamSide::SIDE_NONE),		// �`�[��
	m_state				(STATE_NONE),								// ���
	m_fNumGauge			(0),										// �\���l
	m_fChange			(0.0f),										// �Q�[�W�ϓ���
	m_fStateTime		(0),										// ��ԊǗ��J�E���^�[
	m_fMaxNumGauge		(0),										// �\���l�̍ő�l
	m_fCurrentNumGauge	(0.0f),										// ���ݕ\���l
	m_bDrawFrame		(false),									// �g�\����
	m_pBg				(nullptr),									// �w�i
	m_pBar				(nullptr),									// �Q�[�W
	m_pFrame			(nullptr),									// �t���[��
	m_pAssist			(nullptr)									// �{�^���A�V�X�g
{

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
	m_pAssist = CObject2D::Create(PRIORITY);				// �{�^���A�V�X�g

	SetType(CObject::TYPE::TYPE_UI);
	
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
	SAFE_UNINIT(m_pAssist);

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
		UtilFunc::Transformation::ValueNormalize(m_fCurrentNumGauge, m_fMaxNumGauge, 0.0f);

		// ���_���̐ݒ�
		m_pBar->SetSize(size);

		if (m_fStateTime <= 0)
		{ // �J�E���^�[�� 0�ȉ��ɂȂ����ꍇ

			// �ʏ��Ԃɂ���
			m_state = STATE_NONE;
		}
	}

	if (m_fCurrentNumGauge == m_fMaxNumGauge)
	{// �Q�[�WMAX��

		// �o�[�𔭌�������
		BrightBar();
	}
	else
	{// ����ȊO

		// �F�ݒ�
		m_pBar->SetColor(m_pBar->GetOriginColor());
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
		m_pFrame == nullptr||
		m_pAssist == nullptr)
	{// �P�ł��Ȃ�������
		MyAssert::CustomAssert(false, "Gauge2D: �Ȃ�Ŗ�����H");
	}

	// �����̈ʒu��ݒ�
	CObject::SetPosition(rPos);

	// TODO: ���点
	m_pBg->SetPosition(rPos);
	m_pBar->SetPosition(rPos);
	m_pFrame->SetPosition(rPos);
	m_pAssist->SetPosition(rPos);
}

//============================================================
//	�e�N�X�`�����W�̐ݒ�
//============================================================
void CGauge2D::SetTexUV(const std::vector<D3DXVECTOR2>& uv)
{
	if (m_pBg == nullptr ||
		m_pBar == nullptr ||
		m_pFrame == nullptr)
	{// �P�ł��Ȃ�������
		MyAssert::CustomAssert(false, "Gauge2D: �Ȃ�Ŗ�����H");
	}

	m_pBg->SetTexUV(uv);
	m_pBar->SetTexUV(uv);
	m_pFrame->SetTexUV(uv);
	m_pAssist->SetTexUV(uv);
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
	m_pAssist->BindTexture(pTex->Regist(PASS_ASSIST));
}

//============================================================
// �����T�C�Y�ݒ�
//============================================================
void CGauge2D::InitSize()
{
	m_pBg->SetSizeOrigin(m_pBg->GetSize());
	m_pBar->SetSizeOrigin(m_pBar->GetSize());
	m_pFrame->SetSizeOrigin(m_pFrame->GetSize());
	m_pAssist->SetSizeOrigin(m_pAssist->GetSize());
}

//============================================================
// �Q�[�W����
//============================================================
void CGauge2D::BrightBar()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	CPlayer* pPlayer = pBall->GetPlayer();

	MyLib::Vector3 end = Bright::END;
	MyLib::Vector3 start = Bright::START;
	MyLib::Vector3 easingBar = MyLib::Vector3();
	MyLib::Vector3 easingAssist = MyLib::Vector3();

	if (m_fBrightTime >= m_fBrightTimeEnd * 0.5f)
	{// �����𒴂�����

		easingBar = UtilFunc::Correction::EasingLinear(end, start, m_fBrightTimeEnd * 0.5f, m_fBrightTimeEnd, m_fBrightTime);

		if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_IN ||
			pPlayer->GetTeam() == m_team)
		{// �{�[�������w���삪�����Ă���Ƃ�

			easingAssist = UtilFunc::Correction::EasingLinear(Assist::END, Assist::START, m_fBrightTimeEnd * 0.5f, m_fBrightTimeEnd, m_fBrightTime);
		}
	}
	else
	{
		easingBar = UtilFunc::Correction::EasingLinear(start, end, 0.0f, m_fBrightTimeEnd * 0.5f, m_fBrightTime);
	
		if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_IN ||
			pPlayer->GetTeam() == m_team)
		{// �{�[�������w���삪�����Ă���Ƃ�

			easingAssist = UtilFunc::Correction::EasingLinear(Assist::START, Assist::END, 0.0f, m_fBrightTimeEnd * 0.5f, m_fBrightTime);
		}
	}

	// �C�[�W���O�����l��Color�^�ɕϊ�
	MyLib::Color colBar = UtilFunc::Transformation::HSVtoRGB(easingBar.x, easingBar.y, easingBar.z);
	MyLib::Color colAssist = UtilFunc::Transformation::HSVtoRGB(easingAssist.x, easingAssist.y, easingAssist.z);

	// �F�ݒ�
	m_pBar->SetColor(colBar);
	m_pAssist->SetColor(colAssist);
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
	m_pAssist->SetAnchorType(type);
}

//============================================================
//	�Q�[�W�傫���̐ݒ菈��
//============================================================
void CGauge2D::SetSize(const MyLib::Vector2& rSize)
{
	MyLib::Vector2 sizeFrame = rSize * FRAME_RAT;
	MyLib::Vector2 sizeAssist = rSize;
	sizeAssist.x *= 0.2f;

	m_pBg->SetSize(rSize);
	m_pBar->SetSize(rSize);
	m_pFrame->SetSize(sizeFrame);
	m_pAssist->SetSize(sizeAssist);
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
	m_pBar->SetOriginColor(rCol);
}

//============================================================
//	���Q�[�W�F�̐ݒ菈��
//============================================================
void CGauge2D::SetColorBack(const D3DXCOLOR& rCol)
{
	// �����̔w�i�F����
	m_pBg->SetColor(rCol);
	m_pBg->SetOriginColor(rCol);
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
