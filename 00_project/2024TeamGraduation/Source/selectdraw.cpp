//=============================================================================
// 
//  �I�����̕M���� [selectdraw.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "selectdraw.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\ui_setting\\front.png";
	const float HEIGHT = 50.0f;

}

// ��ԕʃ^�C�}�[
namespace StateTime
{
	const float FADEIN = 0.15f;		// �t�F�[�h�C��
	const float FADEOUT = 0.15f;		// �t�F�[�h�A�E�g
	const float DRAW = 0.15f;
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CSelectDraw::STATE_FUNC CSelectDraw::m_StateFuncList[] =
{
	&CSelectDraw::StateNone,		// �Ȃ�
	&CSelectDraw::StateFadeIn,	// �t�F�[�h�C��
	&CSelectDraw::StateFadeout,	// �t�F�[�h�A�E�g
	&CSelectDraw::StateDrawing,	// ����
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CSelectDraw::CSelectDraw(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_state = State::STATE_NONE;	// ���
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CSelectDraw::~CSelectDraw()
{

}

//==========================================================================
// ��������
//==========================================================================
CSelectDraw* CSelectDraw::Create(const MyLib::Vector3& pos)
{
	// �������̊m��
	CSelectDraw* pObj = DEBUG_NEW CSelectDraw;

	if (pObj != nullptr)
	{
		// �������ݒ�
		pObj->SetPosition(pos);

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CSelectDraw::Init()
{

	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// �e��ϐ�������
	m_state = State::STATE_FADEIN;	// ���
	m_fStateTime = 0.0f;			// ��ԃJ�E���^�[

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);


	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texID);


	// �c�������ɃT�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, HEIGHT);

	// �T�C�Y�ݒ�
	SetSize(D3DXVECTOR2(0.0f, size.y));
	SetSizeOrigin(size);

	// �A���J�[�|�C���g�ݒ�
	SetAnchorType(CObject2D::AnchorPoint::LEFT);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CSelectDraw::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CSelectDraw::Update()
{

	// ��ԍX�V
	UpdateState();

	if (IsDeath()) return;

	// �X�V����
	CObject2D::Update();
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CSelectDraw::UpdateState()
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// ���s
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CSelectDraw::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CSelectDraw::StateFadeIn()
{
	float alpha = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, StateTime::FADEIN, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEIN)
	{
		SetState(State::STATE_NONE);
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CSelectDraw::StateFadeout()
{
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		Uninit();
	}
}

//==========================================================================
// ����
//==========================================================================
void CSelectDraw::StateDrawing()
{
	// �����悤�Ɋg�k
	D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();
	size.x = UtilFunc::Correction::EaseInExpo(0.0f, sizeOrigin.x, 0.0f, StateTime::DRAW, m_fStateTime);
	SetSize(size);

	// UV���W�ݒ�
	D3DXVECTOR2* pTex = GetTex();
	pTex[1].x = pTex[3].x = UtilFunc::Transformation::Clamp(size.x / sizeOrigin.x, 0.0f, 1.0f);

	if (m_fStateTime >= StateTime::DRAW)
	{
		SetState(State::STATE_NONE);
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CSelectDraw::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CSelectDraw::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CSelectDraw::SetPosition(const MyLib::Vector3& pos)
{
	CObject2D::SetPosition(pos - MyLib::Vector3(GetSizeOrigin().x, 0.0f, 0.0f));
}

//==========================================================================
// ���̈ʒu�ݒ�
//==========================================================================
void CSelectDraw::SetOriginPosition(const MyLib::Vector3& pos)
{
	CObject2D::SetOriginPosition(pos - MyLib::Vector3(GetSizeOrigin().x, 0.0f, 0.0f));
}