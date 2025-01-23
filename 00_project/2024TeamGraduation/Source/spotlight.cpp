//==========================================================================
// 
//  �X�|�b�g���C�g���� [spotlight.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "spotlight.h"
#include "lightPoint.h"
#include "EffekseerObj.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int	PRIORITY	= 4;		// �D�揇��
	const float	LIGHT_RANGE	= 600.0f;	// �����͈�
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CSpotLight::CSpotLight() : CObject(PRIORITY, CObject::LAYER::LAYER_DEFAULT),
	m_pEffect	(nullptr)	// ���C�g�G�t�F�N�g
{
	// �����o�ϐ����N���A
	for (int i = 0; i < NUM_LIGHT; i++)
	{ // ���C�g�����J��Ԃ�

		m_apLight[i] = nullptr;	// ���C�g���
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CSpotLight::~CSpotLight()
{

}

//==========================================================================
// ��������
//==========================================================================
CSpotLight* CSpotLight::Create()
{
	// �������̊m��
	CSpotLight* pObj = DEBUG_NEW CSpotLight;
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
HRESULT CSpotLight::Init()
{
	for (int i = 0; i < NUM_LIGHT; i++)
	{ // ���C�g�����J��Ԃ�

		// ���C�g�̐���
		m_apLight[i] = CLightPoint::Create();
		if (m_apLight[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}

		// �g�U����ݒ�
		m_apLight[i]->SetDiffuse(MyLib::color::White());

		// �����͈͂�ݒ�
		m_apLight[i]->SetRange(LIGHT_RANGE);
	}

	// �G�t�F�N�g�̐���
	m_pEffect = CEffekseerObj::Create
	( // ����
		CMyEffekseer::EEfkLabel::EFKLABEL_SPOTLIGHT,
		VEC3_ZERO,
		VEC3_ZERO,
		VEC3_ZERO,
		17.0f,
		false
	);
	if (m_pEffect == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CSpotLight::Uninit()
{
	for (int i = 0; i < NUM_LIGHT; i++)
	{ // ���C�g�����J��Ԃ�

		// ���C�g�̏I��
		SAFE_UNINIT(m_apLight[i]);
	}

	// �G�t�F�N�g�̏I��
	SAFE_UNINIT(m_pEffect);
}

//==========================================================================
// �폜
//==========================================================================
void CSpotLight::Kill()
{
	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CSpotLight::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �`�揈��
//==========================================================================
void CSpotLight::Draw()
{

}

//==========================================================================
// ���C�g�ʒu�̐ݒ菈��
//==========================================================================
void CSpotLight::SetLightPosition(const MyLib::Vector3& rPos)
{
	float fRot = 0.0f;	// �I�t�Z�b�g����
	for (int i = 0; i < NUM_LIGHT; i++)
	{ // ���C�g�����J��Ԃ�

		if (m_apLight[i] != nullptr)
		{
			// ���C�g�ʒu�̐ݒ�
			MyLib::Vector3 posOffset = MyLib::Vector3(sinf(fRot) * 80.0f, 160.0f, cosf(fRot) * 80.0f);	// �I�t�Z�b�g
			m_apLight[i]->SetPosition(rPos + posOffset);
		}

		// �I�t�Z�b�g��������]
		fRot += HALF_PI;
	}

	// �G�t�F�N�g�ʒu�̐ݒ�
	m_pEffect->SetPosition(rPos);
}

