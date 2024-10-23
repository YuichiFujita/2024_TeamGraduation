//==========================================================================
// 
//  �ϋq���� [audience.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "audience.h"

// �h����
#include "audienceAnim.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CAudience::CAudience(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_type	(OBJTYPE_ANIM)	// �I�u�W�F�N�g���
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CAudience::~CAudience()
{

}

//==========================================================================
// ��������
//==========================================================================
CAudience* CAudience::Create(EObjType type)
{
	// �������̊m��
	CAudience* pAudience = nullptr;
	switch (type)
	{ // �I�u�W�F�N�g��ނ��Ƃ̏���
	case CAudience::OBJTYPE_ANIM:
		pAudience = DEBUG_NEW CAudienceAnim;
		break;

	default:
		assert(false);
		break;
	}

	if (pAudience != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pAudience->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pAudience);
			return nullptr;
		}

		// �I�u�W�F�N�g�̎��
		pAudience->m_type = type;
	}

	return pAudience;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CAudience::Init()
{
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CAudience::Uninit()
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �폜����
//==========================================================================
void CAudience::Kill()
{
	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CAudience::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	static float fRotSin = 0.0f;

	fRotSin += 0.035f;
	UtilFunc::Transformation::RotNormalize(fRotSin);

	pos.y = sinf(fRotSin) * 250.0f + 250.0f;
	rot.z = fRotSin;

	SetPosition(pos);
	SetRotation(rot);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CAudience::Draw()
{

}
