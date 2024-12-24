//==========================================================================
// 
//  �^�C�g�����S���� [titleLogo.cpp]
//  Author : Kai Takada
// 
//==========================================================================
#include "titleLogo.h"
#include "object2D.h"
#include "objectX.h"
#include "manager.h"
#include "camera.h"
#include "instantfade.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string MODEL_LOGO = "data\\MODEL\\title\\logo.x";			// ���S�̃��f��
}

namespace StateTime
{
	const float SPAWN = 1.2f;	// �o��
	const float LOOP_ROTATION = 6.0f;	// ���[�v����
	const float LOOP = 1.0f;	// ���[�v����
	const float START = 1.0f;	// �J�n
}

namespace Rotate
{
	const float INTERVAL = 5.0f;	// �Ԋu
	const float TIME = 2.0f;	// ��]���鎞��
}

namespace Logo
{
	const MyLib::Vector3 POS_ORIGIN = MyLib::Vector3(0.0f, 600.0f, 600.0f);				// �����ʒu
	const MyLib::Vector3 POS_DEFAULT = MyLib::Vector3(0.0f, 250.0f, 0.0f);				// �ʏ�ʒu
	const MyLib::Vector3 VALUE_ROTATION = MyLib::Vector3(0.0f, D3DX_PI * 0.05f, 0.0f);	// ����
	const float VALUE_FLOAT = 10.0f;													// ����l
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
	&CTitleLogo::StateWait,		// �ҋ@
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitleLogo::CTitleLogo(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_state				(EState::STATE_NONE),	// ���
	m_fStateTime		(0.0f),					// ��ԃ^�C�}�[
	m_fRotationTime		(0.0f),					// ��]�^�C�}�[
	m_fIntervalRotate	(0.0f),					// ��]�܂ł̊Ԋu
	m_fRotationY		(0.0f),					// Y����]��
	m_fTime				(0.0f)					// ���̃^�C�}�[
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

	// ��Ԑݒ�
	SetState(EState::STATE_SPAWN);


	// �J�����擾
	CCamera* pCamera = GET_MANAGER->GetCamera();

	// �J�����̌�������
	MyLib::Vector3 cameraRot = pCamera->GetRotation();
	cameraRot.y = 0.0f;
	pCamera->SetRotation(cameraRot);

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
	// ���S����
	SAFE_KILL(m_pMain);

	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTitleLogo::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (GET_MANAGER->GetInstantFade()->GetState() == CInstantFade::EState::STATE_FADEIN)
	{// �t�F�[�hIN�͔�����
		return;
	}

	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// �|���S���X�V
	UpdateMain();
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
	MyLib::Vector3 pos = UtilFunc::Correction::EaseOutBack(Logo::POS_ORIGIN, Logo::POS_DEFAULT, 0.0f, StateTime::SPAWN, m_fStateTime, 3.0f);
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
	// ��]�^�C�}�[���Z
	m_fRotationTime += fDeltaTime * fSlowRate;
	if (m_fIntervalRotate <= m_fRotationTime)
	{
		// 2��]
		m_fRotationY = UtilFunc::Correction::EasingCubicInOut(0.0f, D3DX_PI * 2.0f, m_fIntervalRotate, m_fIntervalRotate + Rotate::TIME, m_fRotationTime);

		if (m_fRotationTime >= m_fIntervalRotate + Rotate::TIME)
		{// �I��

			// �Ԋu�v�Z
			m_fIntervalRotate = Rotate::INTERVAL;
			m_fIntervalRotate += UtilFunc::Transformation::Random(0, 50) * 0.1f;

			// �^�C�}�[���Z�b�g
			m_fRotationTime = 0.0f;
		}
	}

	// ����
	float ratio = m_fStateTime / StateTime::LOOP;

	// �ʒu�X�V
	MyLib::Vector3 pos = Logo::POS_DEFAULT;

	// ����
	pos.y += sinf(D3DX_PI * (m_fStateTime / StateTime::LOOP)) * Logo::VALUE_FLOAT;
	m_pMain->SetPosition(pos);

	// ��]
	MyLib::Vector3 rot = sinf(D3DX_PI * m_fStateTime / StateTime::LOOP_ROTATION) * Logo::VALUE_ROTATION;
	MyLib::Vector3 setrot = rot + MyLib::Vector3(0.0f, m_fRotationY, 0.0f);
	m_pMain->SetRotation(setrot);



	// �J�����擾
	CCamera* pCamera = GET_MANAGER->GetCamera();

	// �J�����̌�������
	MyLib::Vector3 cameraRot = pCamera->GetRotation();
	cameraRot.y = rot.y;
	pCamera->SetRotation(cameraRot);

}

//==========================================================================
// �J�n
//==========================================================================
void CTitleLogo::StateStart(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �ҋ@
//==========================================================================
void CTitleLogo::StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ��X�V
//==========================================================================
void CTitleLogo::UpdateMain()
{
	
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
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CTitleLogo::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}
