//==========================================================================
// 
//  �J�b�g�C������ [cutin.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "cutin.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\key\\A.png";	// �e�N�X�`���̃t�@�C��
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CCutIn::STATE_FUNC CCutIn::m_StateFuncList[] =
{
	nullptr,				// �Ȃɂ��Ȃ����
	&CCutIn::UpdateTime,	// ���Ԍo�ߏ��
	nullptr,				// �I�����
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCutIn::CCutIn(int nPriority) : CObject2D(nPriority),
	m_fStateTime	(0.0f),			// ��ԃJ�E���^�[
	m_state			(STATE_TIME)	// ���
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCutIn::~CCutIn()
{

}

//==========================================================================
// ��������
//==========================================================================
CCutIn* CCutIn::Create()
{
	// �������̊m��
	CCutIn* pObj = DEBUG_NEW CCutIn;
	if (pObj != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pObj->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCutIn::Init()
{
	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SAMPLE);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 160.0f);

#else	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 160.0f);
#endif

	SetSize(size);
	SetSizeOrigin(size);

	// ��ʒ����̈ʒu�ɂ���
	SetPosition(MyLib::Vector3(100.0f, 100.0f, 0.0f));

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCutIn::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CCutIn::Kill()
{
	// �폜����
	CObject2D::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CCutIn::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԃJ�E���^�[���Z
	m_fStateTime += fDeltaTime * fSlowRate;

	if (m_StateFuncList[m_state] != nullptr)
	{ // �֐�������ꍇ

		// ��ԕʏ���
		(this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// �X�V����
	CObject2D::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CCutIn::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}

//==========================================================================
// ���Ԍo�ߏ�Ԃ̍X�V����
//==========================================================================
void CCutIn::UpdateTime(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�}�[�����Z
	m_fStateTime += fDeltaTime;
	if (m_fStateTime >= 3.0f)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fStateTime = 0.0f;

		// �I����Ԃɂ���
		m_state = STATE_END;
	}
}
