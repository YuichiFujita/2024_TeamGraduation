//==========================================================================
// 
//  �̈�كh�A���� [gymDoor.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "gymDoor.h"
#include "manager.h"
#include "objectX.h"
#include "debugproc.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string MODEL_DOOR[] =	// �h�A���f��
	{
		"data\\MODEL\\gym\\3D_Gym_DoorL.x",
		"data\\MODEL\\gym\\3D_Gym_DoorR.x",
	};

	const float OFFSET_CLOSE = 60.0f;	// ���Ă���ۂ̃h�A�I�t�Z�b�g
	const float OFFSET_OPEN = 175.0f;	// �J���Ă���ۂ̃h�A�I�t�Z�b�g
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CGymDoor::CGymDoor(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_state			(STATE_CLOSE),	// ���
	m_fCurMoveTime	(0.0f),			// ���݂̃h�A�J�^�C�}�[
	m_fEndMoveTime	(0.0f),			// �I���̃h�A�J�^�C�}�[
	m_fCurOffset	(0.0f),			// ���݂̃h�A�z�u�I�t�Z�b�g
	m_fInitOffset	(0.0f),			// �J�n�̃h�A�z�u�I�t�Z�b�g
	m_fDestOffset	(0.0f),			// �ڕW�̃h�A�z�u�I�t�Z�b�g
	m_bOpen			(false)			// �J�t���O
{
	// �����o�ϐ����N���A
	memset(&m_apDoor[0], 0, sizeof(m_apDoor));	// �h�A���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CGymDoor::~CGymDoor()
{

}

//==========================================================================
// ��������
//==========================================================================
CGymDoor* CGymDoor::Create(const MyLib::Vector3& rPos)
{
	// �������̊m��
	CGymDoor* pGymDoor = DEBUG_NEW CGymDoor;
	if (pGymDoor != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pGymDoor->Init()))
		{ // �������Ɏ��s�����ꍇ

			SAFE_UNINIT(pGymDoor);
			return nullptr;
		}

		// �ʒu�̐ݒ�
		pGymDoor->SetPosition(rPos);
	}

	return pGymDoor;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CGymDoor::Init()
{
	// �I�u�W�F�N�g�̎�ސݒ�
	CObject::SetType(CObject::TYPE::TYPE_OBJECT3D);

	for (int i = 0; i < EDoor::DOOR_MAX; i++)
	{ // �h�A�̖������J��Ԃ�

		// �h�A�̐���
		m_apDoor[i] = CObjectX::Create(MODEL_DOOR[i]);
		if (m_apDoor[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}
	}

	// �h�A�����
	SetEnableOpen(false);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CGymDoor::Uninit()
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �폜����
//==========================================================================
void CGymDoor::Kill()
{
	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CGymDoor::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �J������Ȃ��ꍇ�͔�����
	if (m_state != EState::STATE_MOVE) { return; }

	// �o�ߎ��Ԃ����Z
	m_fCurMoveTime += fDeltaTime * fSlowRate;
	if (m_fCurMoveTime >= m_fEndMoveTime)
	{ // �I�����Ԃɓ��B�����ꍇ

		// �J�󋵂�␳
		SetEnableOpen(m_bOpen);
		return;
	}

	// �h�A���I�t�Z�b�g���v�Z
	m_fCurOffset = UtilFunc::Correction::EasingCubicIn(m_fInitOffset, m_fDestOffset, 0.0f, m_fEndMoveTime, m_fCurMoveTime);

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CGymDoor::Draw()
{

}

//==========================================================================
// �`��󋵂̐ݒ菈��
//==========================================================================
void CGymDoor::SetEnableDisp(bool bDisp)
{
	// ���N���X�̕`��󋵐ݒ�
	CObject::SetEnableDisp(bDisp);

	for (int i = 0; i < EDoor::DOOR_MAX; i++)
	{ // �h�A�̖������J��Ԃ�

		// �h�A�̕`��󋵐ݒ�
		m_apDoor[i]->SetEnableDisp(bDisp);
	}
}

//==========================================================================
// �ʒu�̐ݒ菈��
//==========================================================================
void CGymDoor::SetPosition(const MyLib::Vector3& pos)
{
	// ���N���X�̈ʒu�ݒ�
	CObject::SetPosition(pos);

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//==========================================================================
// �h�A�J���t���O�̐ݒ菈��
//==========================================================================
void CGymDoor::SetEnableOpen(const bool bOpen, const float fMoveTime)
{
	// �J�t���O��ۑ�
	m_bOpen = bOpen;

	// �ڕW�̃h�A�z�u�I�t�Z�b�g��ݒ�
	m_fDestOffset = (bOpen) ? OFFSET_OPEN : OFFSET_CLOSE;

	if (fMoveTime <= 0.0f)
	{ // �J���Ԃ����ݒ�̏ꍇ

		// �J���Ƃ̏�Ԃɂ���
		m_state = (bOpen) ? STATE_OPEN : STATE_CLOSE;

		// ���݂̃h�A�z�u�I�t�Z�b�g��ڕW�I�t�Z�b�g�ɕ␳
		m_fCurOffset = m_fDestOffset;
	}
	else
	{ // �J���Ԃ��ݒ肳��Ă���ꍇ

		// �J����Ԃɂ���
		m_state = STATE_MOVE;
	}

	// ���̐ݒ�
	m_fInitOffset = m_fCurOffset;	// �J�n�̃h�A�z�u�I�t�Z�b�g
	m_fEndMoveTime = fMoveTime;		// �I���̃h�A�J�^�C�}�[

	// ���̏�����
	m_fCurMoveTime = 0.0f;	// ���݂̃h�A�J�^�C�}�[

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//==========================================================================
// ���Έʒu�̐ݒ菈��
//==========================================================================
void CGymDoor::SetPositionRelative(void)
{
	MyLib::Vector3 posThis = GetPosition();	// ���g�̈ʒu
	for (int i = 0; i < EDoor::DOOR_MAX; i++)
	{ // �h�A�̖������J��Ԃ�

		// �I�t�Z�b�g�̌v�Z
		float fDir = (i == 0) ? -1.0f : 1.0f;	// �����̌v�Z�W��
		MyLib::Vector3 offset = MyLib::Vector3(m_fCurOffset * fDir, 0.0f, 0.0f);	// �I�t�Z�b�g

		// �h�A�̈ʒu�ݒ�
		m_apDoor[i]->SetPosition(posThis + offset);
	}
}
