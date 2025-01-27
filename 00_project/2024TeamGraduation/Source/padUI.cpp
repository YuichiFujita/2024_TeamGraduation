//==========================================================================
// 
//  �p�b�hUI���� [padUI.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "padUI.h"
#include "manager.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\entry\\playerMarker003.png";	// �e�N�X�`��

	// �����
	namespace arrow
	{
		const float				SIZE = 25.0f;	// �傫��
		const MyLib::Vector3	SPACE = MyLib::Vector3(70.0f, 10.0f, 0.0f);	// ��
	}
}

//==========================================================================
//	�R���X�g���N�^
//==========================================================================
CPadUI::CPadUI(int nPriority) : CObject2D_Anim(nPriority)
{

}

//==========================================================================
//	�f�X�g���N�^
//==========================================================================
CPadUI::~CPadUI()
{

}

//==========================================================================
// ��������
//==========================================================================
CPadUI* CPadUI::Create(
	const MyLib::Vector3& pos,	// �ʒu
	const float width,			// ����
	const int pattern,			// �p�^�[����
	int nPriority				// �D�揇��
)
{
	// �������̊m��
	CPadUI* pObj = DEBUG_NEW CPadUI(nPriority);

	if (pObj != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pObj->Init()))
		{ // �������Ɏ��s�����ꍇ
			SAFE_UNINIT(pObj);
			return nullptr;
		}

		// �������
		pObj->SetPosition(pos);			// �ʒu
		pObj->SetOriginPosition(pos);	// ���_�ʒu
		pObj->SetPatternAnim(pattern);	// �p�^�[����
		pObj->SetSizeByWidth(width);	// �T�C�Y
	}

	return pObj;
}

//==========================================================================
//	����������
//==========================================================================
HRESULT CPadUI::Init()
{
	// UV�������ݒ�
	SetDivisionUV(mylib_const::MAX_PLAYER, 2);

	//	����������
	CObject2D_Anim::Init();

	// ��ނ��I�u�W�F�N�g2D�ɂ���
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist(TEXTURE);
	BindTexture(nTexID);

	// �����Đ���OFF�ɂ���
	SetEnableAutoPlay(false);

	// ���_���ݒ�
	SetVtx();

	// ���UI�̐���
	if (FAILED(CreateArrowUI()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// ���UI�̐���
//==========================================================================
HRESULT CPadUI::CreateArrowUI()
{
	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
	{
		// ���̐���
		m_apArrow[i] = CArrowUI::Create
		( // ����
			(CArrowUI::EDirection)i,	// ����
			pos,						// �ʒu
			arrow::SIZE,				// �T�C�Y
			MyLib::color::White(1.0f),	// �F
			GetPriority()				// �D�揇��
		);

		if (m_apArrow[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ
			return E_FAIL;
		}

		// �I�t�Z�b�g�ʒu�ݒ�
		m_apArrow[i]->SetOffset(arrow::SPACE);
		m_apArrow[i]->SetOffsetOrigin(m_apArrow[i]->GetOffset());
	}

	return S_OK;
}

//==========================================================================
//	�I������
//==========================================================================
void CPadUI::Uninit()
{
	// �I�u�W�F�N�g��j��
	Release();
}

//==========================================================================
//	�폜����
//==========================================================================
void CPadUI::Kill()
{
	for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
	{
		// ���̏I��
		SAFE_KILL(m_apArrow[i]);
	}

	// ���g�̏I��
	CPadUI::Uninit();
}

//==========================================================================
//	�X�V����
//==========================================================================
void CPadUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�̍X�V
	CObject2D_Anim::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ����
//==========================================================================
void CPadUI::Decide()
{
	// ����̃p�^�[���ɕύX
	SetPatternAnim(GetPatternAnim() + GetDivisionU());
}

//==========================================================================
// �L�����Z��
//==========================================================================
void CPadUI::Cancel()
{
	// �L�����Z���̃p�^�[���ɕύX
	SetPatternAnim(GetPatternAnim() - GetDivisionU());
}

//==========================================================================
// ��������T�C�Y�ݒ�
//==========================================================================
void CPadUI::SetSizeByWidth(const float width)
{
	// �摜�T�C�Y�擾
	CTexture* pTexture = CTexture::GetInstance();
	MyLib::Vector2 size = pTexture->GetImageSize(GetIdxTexture());

	// �������Ŋ���
	size.x *= (1.0f / (float)GetDivisionU());
	size.y *= (1.0f / (float)GetDivisionV());

	// �c�������ɃT�C�Y�v�Z
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, width);
	SetSize(size);
}

//==========================================================================
// �A�N�V�����ݒ�
//==========================================================================
void CPadUI::SetAction(CArrowUI::EDirection dir)
{
	// �I�����ړ�
	m_apArrow[dir]->SetState(CArrowUI::EState::STATE_SELECTMOVE);
}

//==========================================================================
// ���̕`��ݒ�
//==========================================================================
void CPadUI::SetEnableDispArrow(bool bDisp)
{
	for (const auto& arrow : m_apArrow)
	{
		if (arrow == nullptr) continue;
		arrow->SetEnableDisp(bDisp);
	}
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CPadUI::SetPosition(const MyLib::Vector3& pos)
{
	CObject::SetPosition(pos);

	// ���̈ʒu������
	for (const auto& arrow : m_apArrow)
	{
		arrow->SetPosition(pos);
		arrow->SetOriginPosition(pos);
	}
}

//========================================================================================
// �`��󋵐ݒ�
//========================================================================================
void CPadUI::SetEnableDisp(bool bDisp)
{
	// �`��󋵐ݒ�
	CObject::SetEnableDisp(bDisp);

	// �`��󋵐ݒ�
	for (const auto& arrow : m_apArrow)
	{
		arrow->SetEnableDisp(bDisp);
	}
}
