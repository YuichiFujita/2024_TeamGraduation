//==========================================================================
// 
//  �ϋq_�A�j���[�V����3D���� [audienceAnim.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "audienceAnim.h"
#include "gameManager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int PRIORITY = mylib_const::PRIORITY_DEFAULT;	// �D�揇��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CAudienceAnim::CAudienceAnim(EObjType type) : CAudience(type, PRIORITY, CObject::LAYER_DEFAULT),
	m_pAnim3D	(nullptr)	// �A�j���[�V����3D���
{
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CAudienceAnim::~CAudienceAnim()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CAudienceAnim::Init()
{
	// �e�N���X�̏�����
	if (FAILED(CAudience::Init()))
	{ // �������Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �����_���Ɋϐ�ʒu��ݒ�
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random((int)MAX_LEFT_LINE, (int)MAX_RIGHT_LINE);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random((int)NEAR_LINE, (int)FAR_LINE);
	SetWatchPosition(posWatch);

	// �����_���ɐ����ʒu��ݒ�
	MyLib::Vector3 posSpawn = posWatch;
	float fTurn = ((bool)(rand() % 2)) ? 1.0f : -1.0f;
	posSpawn.x = CAudience::SPAWN_SIDE_LINE * fTurn;
	SetSpawnPosition(posSpawn);

	// �A�j���[�V����3D�̐���
	m_pAnim3D = CObject3DAnim::Create(posSpawn, 1, 1, 0.0f, false);
	if (m_pAnim3D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �A�j���[�V����3D�̎����X�V/�����`��/�����j�������Ȃ���ނɂ���
	m_pAnim3D->SetType(CObject::TYPE::TYPE_NONE);

	// TODO�F�傫���萔�K�v
	m_pAnim3D->SetSizeOrigin(MyLib::Vector3(40.0f, 60.0f, 0.0f));
	m_pAnim3D->SetSize(MyLib::Vector3(40.0f, 60.0f, 0.0f));

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CAudienceAnim::Uninit()
{
	// �e�N���X�̏I��
	CAudience::Uninit();

	// �A�j���[�V����3D�̏I��
	SAFE_UNINIT(m_pAnim3D);
}

//==========================================================================
// �폜����
//==========================================================================
void CAudienceAnim::Kill()
{
	// �e�N���X�̍폜
	CAudience::Kill();

	// �A�j���[�V����3D�̏I��
	SAFE_UNINIT(m_pAnim3D);
}

//==========================================================================
// �X�V����
//==========================================================================
void CAudienceAnim::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pAnim3D != nullptr)
	{
		// �A�j���[�V����3D�̍X�V
		m_pAnim3D->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// �e�N���X�̍X�V
	CAudience::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CAudienceAnim::Draw()
{
	// �A�j���[�V����3D�̕`��
	m_pAnim3D->Draw();

	// �e�N���X�̕`��
	CAudience::Draw();
}
