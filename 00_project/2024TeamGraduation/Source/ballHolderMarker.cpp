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
#if 0
#define DISP
#endif

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_CIRCLE = "data\\TEXTURE\\ballmarker\\circle.png";	// �~�̃e�N�X�`��
	const std::string TEXTURE_ARROW = "data\\TEXTURE\\ballmarker\\arrow.png";	// ���̃e�N�X�`��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBallHolderMarker::CBallHolderMarker(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_pPlayer(nullptr),	// �{�[�����������Ă�v���C���[�̃|�C���^
	m_fArrowTime(0.0f)	// ���̃^�C�}�[
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

	// �~����
	if (FAILED(CreateCircle()))
	{
		return E_FAIL;
	}

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

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_CIRCLE);
	m_pCircle->BindTexture(texID);

	// �T�C�Y�ݒ�
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 135.0f);
	m_pCircle->SetSize(MyLib::Vector3(size.x, 0.0f, size.y));
	m_pCircle->SetSizeOrigin(m_pCircle->GetSize());

	return S_OK;
}

//==========================================================================
// ��󐶐�
//==========================================================================
HRESULT CBallHolderMarker::CreateArrow()
{
	// ��������
	m_pArrow = CObject3D::Create(GetPriority());
	if (m_pArrow == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pArrow->CObject::SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_ARROW);
	m_pArrow->BindTexture(texID);

	// �T�C�Y�ݒ�
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 60.0f);
	m_pArrow->SetSize(MyLib::Vector3(size.x, 0.0f, size.y));
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
	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CBallHolderMarker::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#ifdef DISP
	// �v���C���[���Ȃ��ƕ`��؂�
	bool bDisp = (m_pPlayer == nullptr) ? false : true;
	SetEnableDisp(bDisp);

	// �`�悵�Ȃ��Ƃ��͍X�V����
	if (!bDisp) return;

	// �v���C���[�̈ʒu�ɐݒ�
	MyLib::Vector3 pos = m_pPlayer->GetPosition();
	pos.y = 0.01f;
	SetPosition(pos);

	// �~�Ɩ��̈ʒu�ݒ�
	m_pCircle->SetPosition(pos);
	m_pArrow->SetPosition(pos);
#else
	SetEnableDisp(false);
#endif
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
	m_pArrow->SetEnableDisp(bDisp);
}
