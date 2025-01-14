//==========================================================================
// 
//  ���UI���� [arrowUI.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "arrowUI.h"
#include "manager.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\entry\\Arrow_Twin.png";	// �e�N�X�`��
	const float DISTANCE_SELECTMOVE = 15.0f;	// �I�����ړ���
	const float VALUE_FLOAT = 3.0f;				// �ӂ�ӂ��

	namespace StateTime
	{
		const float LOOP = 1.0f;		// ���[�v����
		const float SELECTMOVE = 0.1f;	// �I�����ړ�
	}
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CArrowUI::STATE_FUNC CArrowUI::m_StateFunc[] =	// ��Ԋ֐�
{
	&CArrowUI::StateNone,		// �Ȃ�
	&CArrowUI::StateNormal,		// �ʏ�
	&CArrowUI::StateSelectMove,	// �I�����ړ�
};

//==========================================================================
//	�R���X�g���N�^
//==========================================================================
CArrowUI::CArrowUI(int nPriority) : CObject2D_Anim(nPriority),
	m_state			(EState::STATE_NONE),		// ���
	m_fStateTime	(0.0f),						// ��ԃ^�C�}�[
	m_direction		(EDirection::DIRECTION_L),	// ����
	m_fValueFloat	(VALUE_FLOAT)				// �ӂ�ӂ��
{

}

//==========================================================================
//	�f�X�g���N�^
//==========================================================================
CArrowUI::~CArrowUI()
{

}

//==========================================================================
// ��������
//==========================================================================
CArrowUI* CArrowUI::Create(
	EDirection dir,				// ����
	const MyLib::Vector3& pos,	// �ʒu
	const float width,			// ����
	const MyLib::Color& color,	// �F
	int nPriority				// �D�揇��
)
{
	// �������̊m��
	CArrowUI* pObj = DEBUG_NEW CArrowUI(nPriority);

	if (pObj != nullptr)
	{
		// �������
		pObj->m_direction = dir;		// ����
		pObj->SetPosition(pos);			// �ʒu
		pObj->SetOriginPosition(pos);	// ���_�ʒu
		pObj->SetColor(color);			// �F

		// �N���X�̏�����
		if (FAILED(pObj->Init()))
		{ // �������Ɏ��s�����ꍇ
			SAFE_UNINIT(pObj);
			return nullptr;
		}

		// �T�C�Y�ݒ�
		pObj->SetSizeByWidth(width);	// �T�C�Y
	}

	return pObj;
}

//==========================================================================
//	����������
//==========================================================================
HRESULT CArrowUI::Init()
{
	// UV�������ݒ�
	SetDivisionUV((int)EDirection::DIRECTION_MAX, 1);

	//	����������
	CObject2D_Anim::Init();

	// ��ނ��I�u�W�F�N�g2D�ɂ���
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist(TEXTURE);
	BindTexture(nTexID);

	// �p�^�[����ݒ�
	SetPatternAnim((int)m_direction);

	// �����Đ���OFF�ɂ���
	SetEnableAutoPlay(false);

	// ���_���ݒ�
	SetVtx();

	// ��Ԑݒ�
	SetState(EState::STATE_NORMAL);

	return S_OK;
}

//==========================================================================
//	�I������
//==========================================================================
void CArrowUI::Uninit()
{
	// �I�u�W�F�N�g��j��
	Release();
}

//==========================================================================
//	�폜����
//==========================================================================
void CArrowUI::Kill()
{
	// ���g�̏I��
	CArrowUI::Uninit();
}

//==========================================================================
//	�X�V����
//==========================================================================
void CArrowUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// �e�̍X�V
	CObject2D_Anim::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CArrowUI::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�}�[�X�V
	m_fStateTime += fDeltaTime * fDeltaRate * fSlowRate;

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �ʏ�
//==========================================================================
void CArrowUI::StateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#if 0	// �ӂ�ӂ킳������؂�������
	// ����
	float ratio = m_fStateTime / StateTime::LOOP;

	// �ʒu�X�V
	MyLib::Vector3 pos = GetOriginPosition();

	// �ӂ�ӂ�ʎZ�o
	float moveValue = sinf(D3DX_PI * (m_fStateTime / StateTime::LOOP)) * m_fValueFloat;

	// �ړ�
	pos.x += (m_direction == EDirection::DIRECTION_L) ? -moveValue : moveValue;

	// �ʒu�ݒ�
	SetPosition(pos);
#endif
}

//==========================================================================
// �I�����ړ�
//==========================================================================
void CArrowUI::StateSelectMove(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ʒu�X�V
	MyLib::Vector3 posDest = GetOriginPosition();
	posDest.x += (m_direction == EDirection::DIRECTION_L) ? -DISTANCE_SELECTMOVE : DISTANCE_SELECTMOVE;

	// �C�[�W���O�ړ�
	MyLib::Vector3 pos;
	if (m_fStateTime <= StateTime::SELECTMOVE * 0.5f)
	{// ���̂���
		pos = UtilFunc::Correction::EasingLinear(GetOriginPosition(), posDest, 0.0f, StateTime::SELECTMOVE * 0.5f, m_fStateTime);
	}
	else
	{// �߂�
		pos = UtilFunc::Correction::EasingLinear(posDest, GetOriginPosition(), StateTime::SELECTMOVE * 0.5f, StateTime::SELECTMOVE, m_fStateTime);
	}
	SetPosition(pos);

	if (m_fStateTime >= StateTime::SELECTMOVE)
	{// ����
		SetState(EState::STATE_NORMAL);
	}
}

//========================================================================================
// ��Ԑݒ�
//========================================================================================
void CArrowUI::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//========================================================================================
// ��������T�C�Y�ݒ�
//========================================================================================
void CArrowUI::SetSizeByWidth(const float width)
{
	// �摜�T�C�Y�擾
	CTexture* pTexture = CTexture::GetInstance();
	MyLib::Vector2 size = pTexture->GetImageSize(GetIdxTexture());

	// �c�������ɃT�C�Y�v�Z
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, width);
	SetSize(size);
}
