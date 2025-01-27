//==========================================================================
// 
//  ���U���g�������� [resultCrown.cpp]
//  Author : Kai Takada
// 
//==========================================================================
#include "resultCrown.h"
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
	const std::string MODEL[] =	// ���f��
	{
		"data\\MODEL\\result\\crown.x",
		"data\\MODEL\\result\\draw.x",
	};
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

namespace Crown
{
	const MyLib::Vector3 POS_ORIGIN = MyLib::Vector3(0.0f, 600.0f, 600.0f);				// �����ʒu
	const MyLib::Vector3 POS_DEFAULT = MyLib::Vector3(0.0f, 250.0f, 0.0f);				// �ʏ�ʒu
	const MyLib::Vector3 VALUE_ROTATION = MyLib::Vector3(0.0f, D3DX_PI * 0.05f, 0.0f);	// ����
	const float VALUE_FLOAT = 10.0f;													// ����l
	const MyLib::Vector3 OFFSET_EFFECT = MyLib::Vector3(0.0f, 20.0f, 0.0f);				// �G�t�F�N�g�̃I�t�Z�b�g
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CResultCrown::STATE_FUNC CResultCrown::m_StateFunc[] =	// ��Ԋ֐�
{
	&CResultCrown::StateNone,		// �Ȃ�
	&CResultCrown::StateSpawn,	// �o��
	&CResultCrown::StateLoop,		// ���[�v
	&CResultCrown::StateStart,	// �J�n
	&CResultCrown::StateWait,		// �ҋ@
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CResultCrown::CResultCrown(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_state				(EState::STATE_NONE),	// ���
	m_fStateTime		(0.0f),					// ��ԃ^�C�}�[
	m_fRotationTime		(0.0f),					// ��]�^�C�}�[
	m_fIntervalRotate	(0.0f),					// ��]�܂ł̊Ԋu
	m_fRotationY		(0.0f),					// Y����]��
	m_fTime				(0.0f),					// ���̃^�C�}�[
	m_result			(EResult::RESULT_WIN),	// ����
	m_pEffect			(nullptr)				// �G�t�F�N�g
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CResultCrown::~CResultCrown()
{

}

//==========================================================================
// ��������
//==========================================================================
CResultCrown* CResultCrown::Create(EResult result)
{
	// �������̊m��
	CResultCrown* pLogo = DEBUG_NEW CResultCrown;

	if (pLogo != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pLogo->Init(result)))
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
HRESULT CResultCrown::Init()
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

	// �G�t�F�N�g����
	m_pEffect = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CROWN,
		GetPosition(),
		MyLib::Vector3(),	// ����
		MyLib::Vector3(),
		35.0f, false);

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CResultCrown::Init(EResult result)
{
	// ���ʐݒ�
	SetResult(result);

	return Init();
}

//==========================================================================
// �吶��
//==========================================================================
HRESULT CResultCrown::CreateMain()
{
	// ��������
	m_pMain = CObjectX::Create(MODEL[m_result], Crown::POS_ORIGIN);
	if (m_pMain == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pMain->CObject::SetType(CObject::TYPE::TYPE_OBJECTX);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CResultCrown::Uninit()
{
	// �G�t�F�N�g�폜
	if (m_pEffect != nullptr)
	{
		m_pEffect->SetTrigger(0);
		m_pEffect->Uninit();
		m_pEffect = nullptr;
	}

	// ���S����
	SAFE_UNINIT(m_pMain);

	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �폜����
//==========================================================================
void CResultCrown::Kill()
{
	// ���S����
	SAFE_KILL(m_pMain);

	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CResultCrown::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (GET_MANAGER->GetInstantFade()->GetState() == CInstantFade::EState::STATE_FADEIN)
	{// �t�F�[�hIN�͔�����
		return;
	}

	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// �|���S���X�V
	UpdateMain();

	// �G�t�F�N�g�̈ʒu�X�V
	if (m_pEffect != nullptr)
	{
		m_pEffect->SetPosition(m_pMain->GetPosition() + Crown::OFFSET_EFFECT);
	}
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CResultCrown::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�}�[�X�V
	m_fStateTime += fDeltaTime * fDeltaRate * fSlowRate;

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �o��
//==========================================================================
void CResultCrown::StateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ʒu�X�V
	MyLib::Vector3 pos = UtilFunc::Correction::EaseOutBack(GetOriginPosition(), Crown::POS_DEFAULT, 0.0f, StateTime::SPAWN, m_fStateTime, 3.0f);
	m_pMain->SetPosition(pos);

	if (m_fStateTime >= StateTime::SPAWN)
	{// �J��
		SetState(EState::STATE_LOOP);
	}
}

//==========================================================================
// ���[�v
//==========================================================================
void CResultCrown::StateLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
	MyLib::Vector3 pos = m_pMain->GetPosition();
	pos.y = Crown::POS_DEFAULT.y;

	// ����
	pos.y += sinf(D3DX_PI * (m_fStateTime / StateTime::LOOP)) * Crown::VALUE_FLOAT;
	m_pMain->SetPosition(pos);

	// ��]
	MyLib::Vector3 rot = sinf(D3DX_PI * m_fStateTime / StateTime::LOOP_ROTATION) * Crown::VALUE_ROTATION;
	MyLib::Vector3 setrot = rot + MyLib::Vector3(0.0f, m_fRotationY, 0.0f);
	m_pMain->SetRotation(setrot);
}

//==========================================================================
// �J�n
//==========================================================================
void CResultCrown::StateStart(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �ҋ@
//==========================================================================
void CResultCrown::StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ��X�V
//==========================================================================
void CResultCrown::UpdateMain()
{
	
}

//==========================================================================
// �`�揈��
//==========================================================================
void CResultCrown::Draw()
{

}

//==========================================================================
// �`��󋵂̐ݒ菈��
//==========================================================================
void CResultCrown::SetEnableDisp(bool bDisp)
{
	// ���N���X�̕`��󋵐ݒ�
	CObject::SetEnableDisp(bDisp);

	// ��ƕ��̕`��󋵐ݒ�
	m_pMain->SetEnableDisp(bDisp);
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CResultCrown::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CResultCrown::SetPosition(const MyLib::Vector3& pos)
{
	// �����̈ʒu�ݒ�
	if (m_pMain != nullptr)
	{
		m_pMain->SetPosition(pos);
	}
	
	// ���̃I�u�W�F�N�g�̈ʒu�ݒ�
	CObject::SetPosition(pos);
}

//==========================================================================
// X���f��
//==========================================================================
void CResultCrown::BindXData(std::string filepass)
{
}
