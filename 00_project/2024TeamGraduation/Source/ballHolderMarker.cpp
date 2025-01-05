//==========================================================================
// 
//  �{�[�������}�[�J�[���� [ballHolderMarker.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "ballHolderMarker.h"
#include "gameManager.h"
#include "player.h"
#include "object3D.h"

// TODO�F�{�[���}�[�J�[�\��
#if 1
#define DISP
#endif

// �O���~�����̍X�V
#if 1
#define ONLY_OUT
#endif

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_CIRCLE = "data\\TEXTURE\\ballmarker\\circle.png";			// �~�̃e�N�X�`��
	const std::string TEXTURE_OUTCIRCLE = "data\\TEXTURE\\ballmarker\\outcircle.png";	// �O���~�̃e�N�X�`��
	const std::string TEXTURE_ARROW = "data\\TEXTURE\\ballmarker\\arrow.png";			// ���̃e�N�X�`��
	const float SIZE_CIRCLE = 60.0f;	// �~�̃T�C�Y

#ifdef ONLY_OUT
	const float SIZE_OUTCIRCLE = SIZE_CIRCLE;	// �O���~�̃T�C�Y
#else
	const float SIZE_OUTCIRCLE = SIZE_CIRCLE * 1.2f;	// �O���~�̃T�C�Y
#endif // ONLY_OUT

	const float SIZE_ARROW = 60.0f;	// ���̃T�C�Y
	float MULTIPLY_SIZE_CIRCLE = 1.2f;			// �~�̃T�C�Y�̔{��
}

namespace Timer
{
	float CIRCLE_LOOP = 0.5f;	// �~�̃��[�v����
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBallHolderMarker::CBallHolderMarker(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_pCircle		(nullptr),	// �~
	m_pOutCircle	(nullptr),	// �O���~
	m_pArrow		(nullptr),	// ���
	m_pPlayer		(nullptr),	// �{�[�����������Ă�v���C���[�̃|�C���^
	m_fCircleTime	(0.0f),		// �~�̃^�C�}�[
	m_fArrowTime	(0.0f)		// ���̃^�C�}�[
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBallHolderMarker::~CBallHolderMarker()
{

}

//==========================================================================
// ��������
//==========================================================================
CBallHolderMarker* CBallHolderMarker::Create(CPlayer* pPlayer)
{
	// �������̊m��
	CBallHolderMarker* pMarker = DEBUG_NEW CBallHolderMarker;

	if (pMarker != nullptr)
	{
		// �v���C���[���蓖��
		pMarker->m_pPlayer = pPlayer;

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
HRESULT CBallHolderMarker::Init()
{
	// �I�u�W�F�N�g�̎�ސݒ�
	CObject::SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �O���~����
	if (FAILED(CreateOutCircle()))
	{
		return E_FAIL;
	}

	// �~����
	if (FAILED(CreateCircle()))
	{
		return E_FAIL;
	}

	// TODO : �`�[���̐F�ɐݒ肷��
	m_pCircle->SetColor(MyLib::color::Cyan());
	m_pOutCircle->SetColor(MyLib::color::Cyan());

	// ��󐶐�
	if (FAILED(CreateArrow()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �~����
//==========================================================================
HRESULT CBallHolderMarker::CreateCircle()
{
	// ��������
	m_pCircle = CObject3D::Create(GetPriority());
	if (m_pCircle == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pCircle->CObject::SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �����ݒ�
	m_pCircle->SetRotation(MyLib::Vector3(D3DX_PI * 0.5f, 0.0f, 0.0f));

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_CIRCLE);
	m_pCircle->BindTexture(texID);

	// �T�C�Y�ݒ�
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, SIZE_CIRCLE);
	m_pCircle->SetSize(size);
	m_pCircle->SetSizeOrigin(m_pCircle->GetSize());

	return S_OK;
}

//==========================================================================
// �O���~����
//==========================================================================
HRESULT CBallHolderMarker::CreateOutCircle()
{
	// ��������
	m_pOutCircle = CObject3D::Create(GetPriority());
	if (m_pOutCircle == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pOutCircle->CObject::SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �����ݒ�
	m_pOutCircle->SetRotation(MyLib::Vector3(D3DX_PI * 0.5f, 0.0f, 0.0f));

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_OUTCIRCLE);
	m_pOutCircle->BindTexture(texID);

	// �T�C�Y�ݒ�
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, SIZE_OUTCIRCLE);
	m_pOutCircle->SetSize(size);
	m_pOutCircle->SetSizeOrigin(m_pOutCircle->GetSize());

	return S_OK;
}

//==========================================================================
// ��󐶐�
//==========================================================================
HRESULT CBallHolderMarker::CreateArrow()
{
	// ��������
	m_pArrow = CObject3D::Create(GetPriority() + 1);
	if (m_pArrow == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pArrow->CObject::SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �A���J�[�|�C���g
	m_pArrow->SetAnchorType(CObject3D::EAnchorPoint::UNDER_CENTER);

	// �A���J�[�̊���
	m_pArrow->SetAnchorRate(0.4f);

	// �����ݒ�
	m_pArrow->SetRotation(MyLib::Vector3(D3DX_PI * 0.5f, 0.0f, 0.0f));

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_ARROW);
	m_pArrow->BindTexture(texID);

	// �T�C�Y�ݒ�
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, SIZE_ARROW);
	m_pArrow->SetSize(size);
	m_pArrow->SetSizeOrigin(m_pArrow->GetSize());

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CBallHolderMarker::Uninit()
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �폜����
//==========================================================================
void CBallHolderMarker::Kill()
{
	// �~
	SAFE_KILL(m_pCircle);

	// �O���~
	SAFE_KILL(m_pOutCircle);

	// ���
	SAFE_KILL(m_pArrow);

	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CBallHolderMarker::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#if _DEBUG

	if (ImGui::TreeNode("BallHolderMarker"))
	{
		// ����
		ImGui::DragFloat("CIRCLE_LOOP", &Timer::CIRCLE_LOOP, 0.01f);

		// �g�k�̔{��
		ImGui::DragFloat("MULTIPLY_SIZE_CIRCLE", &MULTIPLY_SIZE_CIRCLE, 0.01f);

		// �A���J�[�̊���
		float anchorRate = m_pArrow->GetAnchorRate();
		ImGui::DragFloat("ANCHOR_RATE", &anchorRate, 0.01f);
		m_pArrow->SetAnchorRate(anchorRate);

		ImGui::TreePop();
	}
#endif // _DEBUG



	// �g�k�̍X�V
	UpdateScaling(fDeltaTime, fDeltaTime, fSlowRate);

#ifdef DISP
	// �v���C���[���Ȃ��ƕ`��؂�
	bool bDisp = (m_pPlayer == nullptr) ? false : true;

	// �֗^���Ȃ��L�����ł��`��؂�
	if (m_pPlayer != nullptr)
	{
		bDisp = (m_pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_NONE) ? false : bDisp;
	}

	// �`��󋵐ݒ菈��
	SetEnableDisp(bDisp);

	// �`�悵�Ȃ��Ƃ��͍X�V����
	if (!bDisp) return;


	// �v���C���[�̈ʒu�ɐݒ�
	MyLib::Vector3 pos = m_pPlayer->GetPosition();
	pos.y = 2.0f;
	SetPosition(pos);

	// �~�Ɩ��̈ʒu�ݒ�
	m_pOutCircle->SetPosition(pos);
	m_pCircle->SetPosition(pos + MyLib::Vector3(0.0f, 0.1f, 0.0f));	// �~�͏�����
	m_pArrow->SetPosition(pos + MyLib::Vector3(0.0f, 0.5f, 0.0f));	// ���͉~�̂���ɏ�

	// ���̌����ݒ�
	MyLib::Vector3 rot = m_pArrow->GetRotation();
	rot.z = -m_pPlayer->GetRotation().y + D3DX_PI;
	UtilFunc::Transformation::RotNormalize(rot.z);
	m_pArrow->SetRotation(rot);

#else
	SetEnableDisp(false);
#endif
}

//==========================================================================
// �g�k�̍X�V
//==========================================================================
void CBallHolderMarker::UpdateScaling(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �~�̃^�C�}�[���Z
	m_fCircleTime += fDeltaTime * fSlowRate;

	// ���̃T�C�Y�擾
	MyLib::Vector3 sizeCircle = m_pCircle->GetSizeOrigin(), sizeOutCircle = m_pOutCircle->GetSizeOrigin();

	float multiply = 1.0f;	// �{��
	if (m_fCircleTime <= Timer::CIRCLE_LOOP * 0.5f)
	{// �g��
		multiply = UtilFunc::Correction::EaseOutExpo(1.0f, MULTIPLY_SIZE_CIRCLE, 0.0f, Timer::CIRCLE_LOOP * 0.5f, m_fCircleTime);
	}
	else
	{// �k��
		multiply = UtilFunc::Correction::EasingEaseIn(MULTIPLY_SIZE_CIRCLE, 1.0f, Timer::CIRCLE_LOOP * 0.5f, Timer::CIRCLE_LOOP, m_fCircleTime);
	}

	// �T�C�Y�ݒ�
#ifndef ONLY_OUT
	m_pCircle->SetSize(sizeCircle * multiply);
#endif // !ONLY_OUT
	m_pOutCircle->SetSize(sizeOutCircle * multiply);

	if (m_fCircleTime >= Timer::CIRCLE_LOOP)
	{// 1���Ń��Z�b�g
		m_fCircleTime = 0.0f;
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CBallHolderMarker::Draw()
{

}

//==========================================================================
// �`��󋵂̐ݒ菈��
//==========================================================================
void CBallHolderMarker::SetEnableDisp(bool bDisp)
{
	// ���N���X�̕`��󋵐ݒ�
	CObject::SetEnableDisp(bDisp);

	// �~�Ɩ��̕`��󋵐ݒ�
	m_pCircle->SetEnableDisp(bDisp);
	m_pOutCircle->SetEnableDisp(bDisp);
	m_pArrow->SetEnableDisp(bDisp);
}
