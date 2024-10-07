//=============================================================================
// 
// �S�[���Q�[���e�L�X�g���� [goalgametext.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "goalgametext.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
#include "camera.h"
#include "game.h"

//==========================================================================
// �}�N����`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\battlewin\\message.png";
	const float MULTIPLY_SIZE = 8.0f;	// �T�C�Y�̔{��
	const float TIME_EXPANSION = 0.6f;	// �g��
	const float TIME_EXPNONE = 1.0f;	// �g��㉽�����Ȃ�
	const float TIME_FADEOUT = 0.3f;	// �t�F�[�h�A�E�g����
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CGoalGameText::STATE_FUNC CGoalGameText::m_StateFuncList[] =
{
	&CGoalGameText::StateExpansion,	// �g��
	&CGoalGameText::StateExpNone,	// �g��㉽�����Ȃ�
	&CGoalGameText::StateFadeOut,	// �t�F�[�h�A�E�g���
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CGoalGameText::CGoalGameText(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_state = STATE_EXPANSION;		// ���
	m_fStateTimer = 0.0f;			// ��ԃ^�C�}�[
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CGoalGameText::~CGoalGameText()
{

}

//==========================================================================
// ��������
//==========================================================================
CGoalGameText* CGoalGameText::Create()
{
	// �������̊m��
	CGoalGameText* pEffect = DEBUG_NEW CGoalGameText;

	if (pEffect != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		pEffect->Init();
	}

	return pEffect;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CGoalGameText::Init()
{
	// ����������
	if (FAILED(CObject2D::Init()))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// ��ނ̐ݒ�
	SetType(TYPE_OBJECT2D);

	// �e�N�X�`���̊��蓖��
	int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nTexIdx);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nTexIdx);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 400.0f);
	SetSize(size * 8.0f);
	SetSizeOrigin(size);

	// �ʒu�ݒ�
	SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));

	// �J�n
	m_fStateTimer = 0.0f;
	m_state = eState::STATE_EXPANSION;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CGoalGameText::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CGoalGameText::Update()
{
	// ��ԍX�V
	UpdateState();

	if (IsDeath())return;;

	// ���_���W�̐ݒ�
	SetVtx();
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CGoalGameText::UpdateState()
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// ��ԕʏ���
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// �g��
//==========================================================================
void CGoalGameText::StateExpansion()
{
	// ����
	float ratio = UtilFunc::Correction::EaseInExpo(0.0f, 1.0f, 0.0f, TIME_EXPANSION, m_fStateTimer);

	// �����x�ݒ�
	SetAlpha(ratio);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();
	size.x = (sizeOrigin.x * MULTIPLY_SIZE) + (sizeOrigin.x - (sizeOrigin.x * MULTIPLY_SIZE)) * ratio;
	size.y = (sizeOrigin.y * MULTIPLY_SIZE) + (sizeOrigin.y - (sizeOrigin.y * MULTIPLY_SIZE)) * ratio;
	SetSize(size);

	// ���Ԍo��
	if (m_fStateTimer >= TIME_EXPANSION)
	{
		m_fStateTimer = 0.0f;
		m_state = STATE_EXPNONE;
	}
}

//==========================================================================
// �������
//==========================================================================
void CGoalGameText::StateExpNone()
{
	
	if (m_fStateTimer >= TIME_EXPNONE)
	{
		m_fStateTimer = 0.0f;
		m_state = eState::STATE_FADEOUT;
		return;
	}
}

//==========================================================================
// �t�F�[�h�A�E�g���
//==========================================================================
void CGoalGameText::StateFadeOut()
{
	
	// �s�����x�X�V
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, TIME_FADEOUT, m_fStateTimer);
	SetAlpha(alpha);

	if (m_fStateTimer >= TIME_FADEOUT)
	{
		m_fStateTimer = 0.0f;
		Uninit();
		return;
	}
}
