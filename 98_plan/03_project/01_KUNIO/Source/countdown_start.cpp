//=============================================================================
// 
//  �J�E���g�_�E������ [countdown_start.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "countdown_start.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"
#include "player.h"
#include "baggage.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE[] =	// �e�N�X�`���̃t�@�C��
	{
		"data\\TEXTURE\\countdown\\countdown_00.png",
		"data\\TEXTURE\\countdown\\countdown_01.png",
		"data\\TEXTURE\\countdown\\countdown_02.png",
		"data\\TEXTURE\\countdown\\countdown_03.png",
	};
	const int MAX_COUNT = 3;	// �J�E���g�̍ő�l
}

// ��ԕʃ^�C�}�[
namespace StateTime
{
	const float DROP_UP = 0.35f;	// ����_��
	const float DROP_WAIT = 0.3f;	// ����_�ҋ@
	const float DROP_DOWN = 0.35f;	// ����_��
	const float COMPLETE = 1.2f;	// ����
	const float FADEOUT = 0.5f;		// �t�F�[�h�A�E�g
}

// ��ԕʈʒu
namespace StatePos
{
	const MyLib::Vector3 ORIGIN_DROP_UP = MyLib::Vector3(640.0f, -120.0f, 0.0f);	// ����(��)�̏����l
	const MyLib::Vector3 DEST_DROP_UP = MyLib::Vector3(640.0f, 360.0f, 0.0f);		// ����(��)�̖ڕW�l
	const MyLib::Vector3 DEST_DROP_DOWN = MyLib::Vector3(640.0f, 880.0f, 0.0f);		// ����(��)�̖ڕW�l
	const MyLib::Vector3 DEST_COMPLETE = MyLib::Vector3(640.0f, 360.0f, 0.0f);		// �����̖ڕW�l
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CCountdown_Start::STATE_FUNC CCountdown_Start::m_StateFuncList[] =
{
	&CCountdown_Start::StateNone,		// �Ȃ�
	&CCountdown_Start::StateDrop_UP,	// ����(��)
	&CCountdown_Start::StateDrop_Wait,	// ����(�ҋ@)
	&CCountdown_Start::StateDrop_DOWN,	// ����(��)
	&CCountdown_Start::StateComplete,	// ����
	&CCountdown_Start::StateFadeout,	// �t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCountdown_Start::CCountdown_Start(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_state = State::STATE_NONE;	// ���
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[
	m_nCount = 0;			// �J�E���g
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCountdown_Start::~CCountdown_Start()
{

}

//==========================================================================
// ��������
//==========================================================================
CCountdown_Start* CCountdown_Start::Create()
{
	// �������̊m��
	CCountdown_Start* pObj = DEBUG_NEW CCountdown_Start;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCountdown_Start::Init()
{

	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// �e��ϐ�������
	m_nCount = MAX_COUNT;	// �J�E���g
	m_state = State::STATE_DROP_UP;	// ���
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE[m_nCount]);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
#endif
	SetSize(size);
	SetSizeOrigin(size);


	// �ʒu�A�����ݒ�͕K�v������Βǉ�

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �T�E���h�Đ�
	CSound::GetInstance()->PlaySound(CSound::LABEL_SE_COUNTDOWN1);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCountdown_Start::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CCountdown_Start::Update()
{
	// ��ԍX�V
	UpdateState();

	if (IsDeath())
	{
		return;
	}

	// �X�V����
	CObject2D::Update();
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CCountdown_Start::UpdateState()
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// ���s
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CCountdown_Start::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// ����(��)
//==========================================================================
void CCountdown_Start::StateDrop_UP()
{
	MyLib::Vector3 pos = GetPosition();

	// �␳�A���X�ɉ���
	pos = UtilFunc::Correction::EaseOutExpo(StatePos::ORIGIN_DROP_UP, StatePos::DEST_DROP_UP, 0.0f, StateTime::DROP_UP, m_fStateTime);
	SetPosition(pos);

	if (m_fStateTime >= StateTime::DROP_UP)
	{
		// �ڕW�l�ɐݒ�
		SetPosition(StatePos::DEST_DROP_UP);

		// ��ԃ^�C�}�[���Z�b�g
		m_fStateTime = 0.0f;

		// �ҋ@��Ԃ֑J��
		m_state = State::STATE_DROP_WAIT;
	}
}

//==========================================================================
// ����(�ҋ@)
//==========================================================================
void CCountdown_Start::StateDrop_Wait()
{
	if (m_fStateTime >= StateTime::DROP_WAIT)
	{
		// ��ԃ^�C�}�[���Z�b�g
		m_fStateTime = 0.0f;

		// ������Ԃ֑J��
		m_state = State::STATE_DROP_DOWN;
	}
}

//==========================================================================
// ����(��)
//==========================================================================
void CCountdown_Start::StateDrop_DOWN()
{
	MyLib::Vector3 pos = GetPosition();

	// �␳�A���X�ɉ���
	pos = UtilFunc::Correction::EaseInExpo(StatePos::DEST_DROP_UP, StatePos::DEST_DROP_DOWN, 0.0f, StateTime::DROP_DOWN, m_fStateTime);
	SetPosition(pos);

	if (m_fStateTime >= StateTime::DROP_DOWN)
	{
		// �ڕW�l�ɐݒ�
		SetPosition(StatePos::DEST_DROP_DOWN);

		// �J�E���g
		m_nCount--;

		// ��ԃ^�C�}�[���Z�b�g
		m_fStateTime = 0.0f;

		// ������Ԃ֑J��
		m_state = State::STATE_DROP_UP;

		if (m_nCount <= 0)
		{
			// ������Ԃ֑J��
			m_state = State::STATE_COMPLETE;

			// �ڕW�l�ɐݒ�
			SetPosition(StatePos::DEST_COMPLETE);

			// �T�E���h�Đ�
			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_COUNTDOWN2);
		}
		else
		{
			// �T�E���h�Đ�
			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_COUNTDOWN1);
		}

		// �e�N�X�`���ݒ�
		int texID = CTexture::GetInstance()->Regist(TEXTURE[m_nCount]);
		BindTexture(texID);

		// �T�C�Y�ݒ�
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

		// �c�������ɃT�C�Y�ݒ�
		if (m_nCount != 0)
		{
			size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
			SetSize(size);
			SetSizeOrigin(size);
		}
		else
		{// �J�n
			size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
			SetSize(size * 8.0f);
			SetSizeOrigin(size);
			SetAlpha(0.0f);
		}
	}
}

//==========================================================================
// ����
//==========================================================================
void CCountdown_Start::StateComplete()
{

	float ratio = UtilFunc::Correction::EaseInExpo(0.0f, 1.0f, 0.0f, StateTime::COMPLETE * 0.8f, m_fStateTime);

	// �����x�ݒ�
	SetAlpha(ratio);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();
	size.x = (sizeOrigin.x * 5.0f) + (sizeOrigin.x - (sizeOrigin.x * 5.0f)) * ratio;
	size.y = (sizeOrigin.y * 5.0f) + (sizeOrigin.y - (sizeOrigin.y * 5.0f)) * ratio;
	SetSize(size);

	if (m_fStateTime >= StateTime::COMPLETE)
	{
		// ��ԃ^�C�}�[���Z�b�g
		m_fStateTime = 0.0f;

		// �t�F�[�h�A�E�g��Ԃ֑J��
		m_state = State::STATE_FADEOUT;

		CBaggage* pBaggage = CPlayer::GetListObj().GetData(0)->GetBaggage();
		pBaggage->SetState(CBaggage::STATE::STATE_NONE);

		// ��C�҂�
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_WAIT_AIRPUSH);
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CCountdown_Start::StateFadeout()
{
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		// ��ԃ^�C�}�[���Z�b�g
		m_fStateTime = 0.0f;

		Uninit();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CCountdown_Start::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}

