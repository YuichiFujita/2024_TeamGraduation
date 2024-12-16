//==========================================================================
// 
//  �^�C�g�����S���� [titleLogo.cpp]
//  Author : Kai Takada
// 
//==========================================================================
#include "titleLogo.h"
#include "object2D.h"
#include "objectX.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string MODEL_LOGO = "data\\MODEL\\title\\logo.x";			// ���S�̃��f��
	const std::string TEXTURE_SUB = "data\\TEXTURE\\title\\player.png";			// ���̃e�N�X�`��
}

namespace StateTime
{
	const float SPAWN = 5.0f;	// �o��
	const float LOOP_ROTATION = 3.0f;	// ���[�v����
	const float LOOP = 1.0f;	// ���[�v����
	const float START = 1.0f;	// �J�n
}

namespace Logo
{
	const MyLib::Vector3 POS_ORIGIN = MyLib::Vector3(0.0f, 600.0f, 600.0f);				// �����ʒu
	const MyLib::Vector3 POS_DEFAULT = MyLib::Vector3(0.0f, 100.0f, 0.0f);				// �ʏ�ʒu
	const MyLib::Vector3 VALUE_ROTATION = MyLib::Vector3(0.0f, D3DX_PI * 0.05f, 0.0f);	// ����
	const float VALUE_FLOAT = 20.0f;													// ����l
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CTitleLogo::STATE_FUNC CTitleLogo::m_StateFunc[] =	// ��Ԋ֐�
{
	&CTitleLogo::StateNone,		// �Ȃ�
	&CTitleLogo::StateSpawn,	// �o��
	&CTitleLogo::StateLoop,		// ���[�v
	&CTitleLogo::StateStart,	// �J�n
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitleLogo::CTitleLogo(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_state			(EState::STATE_NONE),	// ���
	m_fStateTime	(0.0f),					// ��ԃ^�C�}�[
	m_fTime			(0.0f)					// ���̃^�C�}�[
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTitleLogo::~CTitleLogo()
{

}

//==========================================================================
// ��������
//==========================================================================
CTitleLogo* CTitleLogo::Create()
{
	// �������̊m��
	CTitleLogo* pLogo = DEBUG_NEW CTitleLogo;

	if (pLogo != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pLogo->Init()))
		{ // �������Ɏ��s�����ꍇ
			SAFE_UNINIT(pLogo);
			return nullptr;
		}
	}

	return pLogo;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTitleLogo::Init()
{
	// �I�u�W�F�N�g�̎�ސݒ�
	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �吶��
	if (FAILED(CreateMain()))
	{
		return E_FAIL;
	}

	// ������
	if (FAILED(CreateSub()))
	{
		return E_FAIL;
	}

	// ��Ԑݒ�
	SetState(EState::STATE_SPAWN);

	return S_OK;
}

//==========================================================================
// �吶��
//==========================================================================
HRESULT CTitleLogo::CreateMain()
{
	// ��������
	m_pMain = CObjectX::Create(MODEL_LOGO, Logo::POS_ORIGIN);
	if (m_pMain == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pMain->CObject::SetType(CObject::TYPE::TYPE_OBJECTX);

	return S_OK;
}

//==========================================================================
// ������
//==========================================================================
HRESULT CTitleLogo::CreateSub()
{
	// ��������
	m_pSub = CObject2D::Create(GetPriority());
	if (m_pSub == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pSub->CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SUB);
	m_pSub->BindTexture(texID);

	// �T�C�Y�ݒ�
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, SCREEN_WIDTH * 0.2f);
	m_pSub->SetSize(MyLib::Vector2(size.x, size.y));
	m_pSub->SetSizeOrigin(m_pSub->GetSize());

	// �ʒu�ݒ�
	m_pSub->SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.3f, 0.0f));

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTitleLogo::Uninit()
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �폜����
//==========================================================================
void CTitleLogo::Kill()
{
	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTitleLogo::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// �|���S���X�V
	UpdateMain();
	UpdateSub();
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CTitleLogo::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�}�[�X�V
	m_fStateTime += fDeltaTime * fDeltaRate * fSlowRate;

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �o��
//==========================================================================
void CTitleLogo::StateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ʒu�X�V
	MyLib::Vector3 pos = UtilFunc::Correction::EasingLinear(Logo::POS_ORIGIN, Logo::POS_DEFAULT, 0.0f, StateTime::START, m_fStateTime);
	m_pMain->SetPosition(pos);

	if (m_fStateTime >= StateTime::SPAWN)
	{// �J��
		SetState(EState::STATE_LOOP);
	}
}

//==========================================================================
// ���[�v
//==========================================================================
void CTitleLogo::StateLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ����
	float ratio = m_fStateTime / StateTime::LOOP;

	// �ʒu�X�V
	MyLib::Vector3 pos = Logo::POS_DEFAULT;

	// ����
	pos.y += sinf(D3DX_PI * (m_fStateTime / StateTime::LOOP)) * Logo::VALUE_FLOAT;
	m_pMain->SetPosition(pos);

	// ��]
	MyLib::Vector3 rot = sinf(D3DX_PI * m_fStateTime / StateTime::LOOP_ROTATION) * Logo::VALUE_ROTATION;
	m_pMain->SetRotation(rot);

}

//==========================================================================
// �J�n
//==========================================================================
void CTitleLogo::StateStart(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ��X�V
//==========================================================================
void CTitleLogo::UpdateMain()
{
	
}

//==========================================================================
// ���X�V
//==========================================================================
void CTitleLogo::UpdateSub()
{
	// �ʒu�h��
	MyLib::Vector3 pos = m_pSub->GetPosition();
	MyLib::Vector3 posDef = MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	pos = posDef * m_fTime;

	// �ʒu�ݒ�
	m_pSub->SetPosition(pos);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTitleLogo::Draw()
{

}

//==========================================================================
// �`��󋵂̐ݒ菈��
//==========================================================================
void CTitleLogo::SetEnableDisp(bool bDisp)
{
	// ���N���X�̕`��󋵐ݒ�
	CObject::SetEnableDisp(bDisp);

	// ��ƕ��̕`��󋵐ݒ�
	m_pMain->SetEnableDisp(bDisp);
	m_pSub->SetEnableDisp(bDisp);
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CTitleLogo::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}
