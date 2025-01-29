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
#include "string2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY	 = 5;	// �Q�[�W2D�̗D�揇��
	const char* PASS_BAR = "data\\TEXTURE\\gauge\\bar_01.jpg";		// �p�X(�Q�[�W)
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
	const MyLib::PosGrid3 END = MyLib::PosGrid3(175, 40, 100);		// �I���F
	const MyLib::PosGrid3 START = MyLib::PosGrid3(227, 0, 97);		// �J�n�F
	const float END_ALPHA = 1.0f;									// �I���F(�����x)
	const float START_ALPHA = 0.45f;								// �J�n�F(�����x)
	const float END_TIME = 2.2f;									// �I������
}

// �`�[����
namespace Name
{
	const MyLib::Vector3 POS[] = { MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, 95.0f, 0.0f), MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, 95.0f, 0.0f) };	// ���_�ʒu
	const char*	FONT	= "data\\FONT\\Kazesawa Bold.ttf";	// �t�H���g�p�X
	const bool	ITALIC	= false;							// �C�^���b�N
	const float	HEIGHT	= 19.0f;							// �����c��
	const D3DXCOLOR COL	= MyLib::color::Black();			// �F
}

//************************************************************
//	�ÓI�֐�
//************************************************************
float CGauge2D::m_fBrightTime = 0.0f;					// max�̎�����F
float CGauge2D::m_fBrightTimeEnd = Bright::END_TIME;	// max�̎�����F�I������
float CGauge2D::m_fAssistTime = 0.0f;					// �A�V�X�g�J�E���^�[
float CGauge2D::m_fAssistTimeEnd = Assist::END_TIME;	// �A�V�X�g�J�E���^�[

//************************************************************
//	�q�N���X [CGauge2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGauge2D::CGauge2D(const float nFrame) : CObject(PRIORITY, CObject::LAYER::LAYER_2D),
	m_fFrame			(nFrame),								// �\���l�̕ϓ��t���[���萔
	m_team				(CGameManager::ETeamSide::SIDE_NONE),	// �`�[��
	m_state				(STATE_NONE),	// ���
	m_fNumGauge			(0.0f),			// �\���l
	m_fChange			(0.0f),			// �Q�[�W�ϓ���
	m_fStateTime		(0.0f),			// ��ԊǗ��J�E���^�[
	m_fMaxNumGauge		(0.0f),			// �\���l�̍ő�l
	m_fCurrentNumGauge	(0.0f),			// ���ݕ\���l
	m_pBg				(nullptr),		// �w�i
	m_pBar				(nullptr),		// �Q�[�W
	m_pFrame			(nullptr),		// �t���[��
	m_pAssist			(nullptr),		// �{�^���A�V�X�g
	m_pTeamName			(nullptr),		// �`�[����
	m_offsetFrame		(VEC3_ZERO),	// �t���[���I�t�Z�b�g
	m_offsetAssist		(VEC3_ZERO),	// �A�V�X�g�I�t�Z�b�g
	m_offsetName		(VEC3_ZERO)		// �`�[�����I�t�Z�b�g
{
	m_fSizeFrame = FRAME_RAT;
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
	// �|���S���̐���
	m_pBg = CObject2D::Create(PRIORITY);		// �w�i
	m_pBar = CObject2D::Create(PRIORITY);		// �Q�[�W
	m_pFrame = CObject2D::Create(PRIORITY);		// �t���[��
	m_pAssist = CObject2D::Create(PRIORITY);	// �{�^���A�V�X�g

	// �`�[�����̐���
	m_pTeamName = CString2D::Create
	( // ����
		Name::FONT,		// �t�H���g�p�X
		Name::ITALIC,	// �C�^���b�N
		L"����������",	// �w�蕶����
		VEC3_ZERO,		// ���_�ʒu
		Name::HEIGHT,	// �����c��
		XALIGN_CENTER,	// ���z�u
		VEC3_ZERO,		// ���_����
		Name::COL		// �F
	);
	if (m_pTeamName == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// 
	m_pTeamName->SetType(CObject::TYPE::TYPE_UI);

	// ���E��~���ł������I
	SetEnablePosibleMove_WorldPause(true);
	m_pBg->SetEnablePosibleMove_WorldPause(true);
	m_pBar->SetEnablePosibleMove_WorldPause(true);
	m_pFrame->SetEnablePosibleMove_WorldPause(true);
	m_pAssist->SetEnablePosibleMove_WorldPause(true);
	m_pTeamName->SetEnablePosibleMove_WorldPause(true);

	// ��ނ�UI�ɂ���
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

	// �`�[�����̔j��
	SAFE_UNINIT(m_pTeamName);

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

	ImGui::Text("GaugeUpdate");

	// �Q�[�W�̐ݒ�
	if (m_state == STATE_CHANGE)
	{ // �̗͂��ϓ����̏ꍇ

		// �J�E���^�[�����Z
		m_fStateTime -= fDeltaTime * fSlowRate;

		// ���݂̃Q�[�W�ɕϓ��ʂ����Z
		m_fCurrentNumGauge += m_fChange;

		// ���݂̕\���l��␳
		UtilFunc::Transformation::ValueNormalize(m_fCurrentNumGauge, m_fMaxNumGauge, 0.0f);

		// ���_���̐ݒ�
		m_pBar->SetSize(size);

		if (m_fStateTime <= 0.0f)
		{
			// �ʏ��Ԃɂ���
			m_state = STATE_NONE;
		}
	}

	if (m_fCurrentNumGauge == m_fMaxNumGauge)
	{ // �Q�[�WMAX��

		// �o�[�𔭌�������
		BrightBar();

		// �A�V�X�g�𔭌�������
		BrightAssist();
	}
	else
	{ // ����ȊO

		// �F�ݒ�
		m_pBar->SetColor(m_pBar->GetOriginColor());
		m_pAssist->SetAlpha(0.0f);
	}
}

//============================================================
//	�`�揈��
//============================================================
void CGauge2D::Draw()
{


}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CGauge2D::SetEnableDisp(const bool bDisp)
{
	m_pBg->SetEnableDisp(bDisp);
	m_pBar->SetEnableDisp(bDisp);
	m_pAssist->SetEnableDisp(bDisp);
	m_pFrame->SetEnableDisp(bDisp);
	m_pTeamName->SetEnableDisp(bDisp);
}

//============================================================
//	�ʒu�̏���������
//============================================================
void CGauge2D::InitPosition()
{
	m_pBg->SetOriginPosition(m_pBg->GetPosition());
	m_pBar->SetOriginPosition(m_pBar->GetPosition());
	m_pAssist->SetOriginPosition(m_pAssist->GetPosition());
	m_pFrame->SetOriginPosition(m_pFrame->GetPosition());
	m_pTeamName->SetOriginPosition(m_pTeamName->GetPosition());
}

//============================================================
//	�傫���̏���������
//============================================================
void CGauge2D::InitSize()
{
	m_pBg->SetSizeOrigin(m_pBg->GetSize());
	m_pBar->SetSizeOrigin(m_pBar->GetSize());
	m_pAssist->SetSizeOrigin(m_pAssist->GetSize());
	m_pFrame->SetSizeOrigin(m_pFrame->GetSize());
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CGauge2D::SetPosition(const MyLib::Vector3& rPos)
{
	if (m_pBg == nullptr
	||  m_pBar == nullptr
	||  m_pFrame == nullptr
	||  m_pAssist == nullptr
	||  m_pTeamName == nullptr)
	{ // �P�ł��Ȃ�������

		MyAssert::CustomAssert(false, "Gauge2D: �Ȃ�Ŗ�����H");
	}

	// �����̈ʒu��ݒ�
	CObject::SetPosition(rPos);

	// ���Έʒu��ݒ�
	SetPositionRelative();
}

//============================================================
//	�e�N�X�`�����W�̐ݒ�
//============================================================
void CGauge2D::SetTexUV(const std::vector<D3DXVECTOR2>& uv)
{
	if (m_pBg == nullptr
	||  m_pBar == nullptr
	||  m_pFrame == nullptr
	||  m_pAssist == nullptr
	||  m_pTeamName == nullptr)
	{ // �P�ł��Ȃ�������

		MyAssert::CustomAssert(false, "Gauge2D: �Ȃ�Ŗ�����H");
	}

	m_pBg->SetTexUV(uv);
	m_pBar->SetTexUV(uv);
	m_pFrame->SetTexUV(uv);
}

//============================================================
//	��������
//============================================================
CGauge2D* CGauge2D::Create
(
	const float fMax,						// �ő�\���l
	const float fFrame,						// �\���l�ϓ��t���[��
	const std::string& rTeamName,			// �`�[����
	const MyLib::Vector3& rPos,				// �ʒu
	const MyLib::Vector3& rOffsetFrame,		// �t���[���I�t�Z�b�g
	const MyLib::Vector3& rOffsetAssist,	// �A�V�X�g�I�t�Z�b�g
	const MyLib::Vector3& rOffsetName,		// �`�[�����I�t�Z�b�g
	const MyLib::Vector2& rSizeGauge,		// �Q�[�W�傫��
	const MyLib::Vector2& rSizeFrame,		// �t���[���傫��
	const MyLib::Vector2& rSizeAssist,		// �A�V�X�g�傫��
	const D3DXCOLOR& rColFront,				// �\�Q�[�W�F
	const D3DXCOLOR& rColBack 				// ���Q�[�W�F
)
{
	// �Q�[�W2D�̐���
	CGauge2D* pGauge2D = DEBUG_NEW CGauge2D(fFrame);
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

		// �Q�[�W�ő�l��ݒ�
		pGauge2D->SetMaxNum(fMax);

		// �`�[������ݒ�
		pGauge2D->m_pTeamName->SetString(rTeamName);

		// �ʒu��ݒ�
		pGauge2D->SetPosition(rPos);

		// �ʒu��������
		pGauge2D->InitPosition();

		// �I�t�Z�b�g��ݒ�
		pGauge2D->SetOffsetFrame(rOffsetFrame);		// �t���[���I�t�Z�b�g
		pGauge2D->SetOffsetAssist(rOffsetAssist);	// �A�V�X�g�I�t�Z�b�g
		pGauge2D->SetOffsetTeamName(rOffsetAssist);	// �`�[�����I�t�Z�b�g

		// �傫����ݒ�
		pGauge2D->SetSizeGauge(rSizeGauge);		// �Q�[�W�傫��
		pGauge2D->SetSizeFrame(rSizeFrame);		// �t���[���傫��
		pGauge2D->SetSizeAssist(rSizeAssist);	// �A�V�X�g�傫��

		// �傫����������
		pGauge2D->InitSize();

		// �F��ݒ�
		pGauge2D->SetColorFront(rColFront);	// �\�Q�[�W�F
		pGauge2D->SetColorBack(rColBack);	// ���Q�[�W�F

		// �m�ۂ����A�h���X��Ԃ�
		return pGauge2D;
	}
}

//============================================================
//	�w�i�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CGauge2D::BindTextureBG(const int nTexID)
{
	// �w�i�e�N�X�`���̊���
	m_pBg->BindTexture(nTexID);
}

//============================================================
//	�Q�[�W�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CGauge2D::BindTextureBar(const int nTexID)
{
	// �Q�[�W�e�N�X�`���̊���
	m_pBar->BindTexture(nTexID);
}

//============================================================
//	�t���[���e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CGauge2D::BindTextureFrame(const int nTexID)
{
	// �t���[���e�N�X�`���̊���
	m_pFrame->BindTexture(nTexID);
}

//============================================================
//	�A�V�X�g�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CGauge2D::BindTextureAssist(const int nTexID)
{
	// �A�V�X�g�e�N�X�`���̊���
	m_pAssist->BindTexture(nTexID);
}

//============================================================
//	�Q�[�W����
//============================================================
void CGauge2D::BrightBar()
{
	MyLib::Vector3 start = Bright::START;	// �J�n�F
	MyLib::Vector3 end = Bright::END;		// �I���F
	MyLib::Vector3 easingBar =VEC3_ZERO;	// �Q�[�W�F
	if (m_fBrightTime >= m_fBrightTimeEnd * 0.5f)
	{
		easingBar = UtilFunc::Correction::EasingLinear(end, start, m_fBrightTimeEnd * 0.5f, m_fBrightTimeEnd, m_fBrightTime);
	}
	else
	{
		easingBar = UtilFunc::Correction::EasingLinear(start, end, 0.0f, m_fBrightTimeEnd * 0.5f, m_fBrightTime);
	}

	// �C�[�W���O�����l��Color�^�ɕϊ�
	MyLib::Color colBar = UtilFunc::Transformation::HSVtoRGB(easingBar.x, easingBar.y, easingBar.z);

	// �F�ݒ�
	m_pBar->SetColor(colBar);
}

//============================================================
//	�A�V�X�g����
//============================================================
void CGauge2D::BrightAssist()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();	// �{�[�����
	CPlayer* pPlayer = pBall->GetPlayer();		// �v���C���[���
	MyLib::Vector3 easingAssist = VEC3_ZERO;	// �A�V�X�g�F
	float fAlphaAssist = 0.0f;					// �A�V�X�g�����x
	if (m_fAssistTime >= m_fAssistTimeEnd * 0.5f)
	{
		if (pPlayer != nullptr)
		{
			if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_IN
			&&  pPlayer->GetTeam() == m_team)
			{ // �{�[�������w���삪�����Ă���Ƃ�

				easingAssist = UtilFunc::Correction::EasingLinear(Assist::END, Assist::START, m_fAssistTimeEnd * 0.5f, m_fAssistTimeEnd, m_fAssistTime);
				fAlphaAssist = UtilFunc::Correction::EasingLinear(Assist::END_ALPHA, Assist::START_ALPHA, m_fAssistTimeEnd * 0.5f, m_fAssistTimeEnd, m_fAssistTime);
			}
		}
	}
	else
	{
		if (pPlayer != nullptr)
		{
			if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_IN
			&&  pPlayer->GetTeam() == m_team)
			{ // �{�[�������w���삪�����Ă���Ƃ�

				easingAssist = UtilFunc::Correction::EasingLinear(Assist::START, Assist::END, 0.0f, m_fAssistTimeEnd * 0.5f, m_fAssistTime);
				fAlphaAssist = UtilFunc::Correction::EasingLinear(Assist::START_ALPHA, Assist::END_ALPHA, 0.0f, m_fAssistTimeEnd * 0.5f, m_fAssistTime);
			}
		}
	}

	// �C�[�W���O�����l��Color�^�ɕϊ�
	MyLib::Color colAssist = UtilFunc::Transformation::HSVtoRGB(easingAssist.x, easingAssist.y, easingAssist.z);
	colAssist.a = fAlphaAssist;

	// �F�ݒ�
	m_pAssist->SetColor(colAssist);
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CGauge2D::SetPositionRelative()
{
	MyLib::Vector3 pos = GetPosition();

	m_pBg->SetPosition(pos);
	m_pBar->SetPosition(pos);
	m_pFrame->SetPosition(pos + m_offsetFrame);
	m_pAssist->SetPosition(pos + m_offsetAssist);
	m_pTeamName->SetPosition(pos + m_offsetName);
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
	m_fStateTime = 0.0f;	// ��ԊǗ��J�E���^�[
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
void CGauge2D::SetSizeGaugeRadius(const float fRadius)
{
	// �T�C�Y
	MyLib::Vector2 size = m_pBar->GetSizeOrigin();
	size.x *= fRadius;

	m_pBar->SetSize(size);
}

//============================================================
//	�Q�[�W�傫���̐ݒ菈��
//============================================================
void CGauge2D::SetSizeGauge(const MyLib::Vector2& rSize)
{
	m_pBg->SetSize(rSize);
	m_pBar->SetSize(rSize);
}

//============================================================
//	�t���[���傫���̐ݒ菈��
//============================================================
void CGauge2D::SetSizeFrame(const MyLib::Vector2& rSize)
{
	m_pFrame->SetSize(rSize);
}

//============================================================
//	�A�V�X�g�傫���̐ݒ菈��
//============================================================
void CGauge2D::SetSizeAssist(const MyLib::Vector2& rSize)
{
	m_pAssist->SetSize(rSize);
}

//============================================================
//	�t���[���I�t�Z�b�g�̐ݒ菈��
//============================================================
void CGauge2D::SetOffsetFrame(const MyLib::Vector3& rOffset)
{
	// �����̃t���[���I�t�Z�b�g����
	m_offsetFrame = rOffset;

	// ���Έʒu��ݒ�
	SetPositionRelative();
}

//============================================================
//	�A�V�X�g�I�t�Z�b�g�̐ݒ菈��
//============================================================
void CGauge2D::SetOffsetAssist(const MyLib::Vector3& rOffset)
{
	// �����̃A�V�X�g�I�t�Z�b�g����
	m_offsetAssist = rOffset;

	// ���Έʒu��ݒ�
	SetPositionRelative();
}

//============================================================
//	�`�[�����I�t�Z�b�g�̐ݒ菈��
//============================================================
void CGauge2D::SetOffsetTeamName(const MyLib::Vector3& rOffset)
{
	// �����̃A�V�X�g�I�t�Z�b�g����
	m_offsetName = rOffset;

	// ���Έʒu��ݒ�
	SetPositionRelative();
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
//	���z�u�̐ݒ菈��
//============================================================
void CGauge2D::SetAlignX(const EAlignX alignX)
{
	// ���z�u�̐ݒ�
	m_pTeamName->SetAlignX(alignX);
}

//============================================================
//	������̐ݒ菈��
//============================================================
void CGauge2D::SetString(const std::string& rStr)
{
	// ������̐ݒ�
	m_pTeamName->SetString(rStr);
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
//	�f�o�b�O
//============================================================
void CGauge2D::Debug()
{
	if (ImGui::TreeNode("Gauge2D"))
	{
		ImGui::DragFloat("m_fSizeFrame", &m_fSizeFrame, 0.01f, 0.0f, 0.0f, "%.2f");

		SetSizeFrame(m_pFrame->GetSizeOrigin() * m_fSizeFrame);

		MyLib::Vector3 pos = MyLib::Vector3(30.0f, 670.0f, 0.0f);
		if (m_team == CGameManager::ETeamSide::SIDE_RIGHT) pos.x = 1250.0f;

		pos.x *= m_fSizeFrame;
		m_pFrame->SetPosition(pos);

		ImGui::TreePop();
	}
}
