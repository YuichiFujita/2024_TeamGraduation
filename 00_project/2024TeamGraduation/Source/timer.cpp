//============================================================
//
//	�^�C�}�[���� [timer.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "timer.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 0;	// �^�C�}�[�̗D�揇��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CTimer> CTimer::m_list = {};	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CTimer] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTimer::CTimer() : CObject(PRIORITY, CObject::LAYER::LAYER_TIMER),
	m_funcCount	(nullptr),		// �v���֐��|�C���^
	m_state		(STATE_NONE),	// �v�����
	m_fTime		(0.0f),			// �v������
	m_fLimit	(0.0f),			// ��������
	m_bStop		(false),		// �v����~��
	m_lTime		(0),			// �v���~���b
	m_nMin		(0),			// ��
	m_nSec		(0),			// �b
	m_nMSec		(0)				// �~���b
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CTimer::~CTimer()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTimer::Init(void)
{
	// �����o�ϐ���������
	m_funcCount = nullptr;		// �v���֐��|�C���^
	m_state		= STATE_NONE;	// �v�����
	m_fTime		= 0.0f;			// �v������
	m_fLimit	= 0.0f;			// ��������
	m_bStop		= false;		// �v����~��
	m_lTime		= 0;			// �v���~���b
	m_nMin		= 0;			// ��
	m_nSec		= 0;			// �b
	m_nMSec		= 0;			// �~���b

	// ��ނ��^�C�}�[�ɂ���
	SetType(CObject::TYPE::TYPE_TIMER);

	// ���X�g�ɃI�u�W�F�N�g��ۑ�
	m_list.Regist(this);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTimer::Uninit(void)
{
	// ���X�g����I�u�W�F�N�g��j��
	m_list.Delete(this);

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CTimer::Update(const float fDeltaTime)
{
	switch (m_state)
	{ // �v����Ԃ��Ƃ̏���
	case STATE_NONE:
		break;

	case STATE_COUNT:

		// ��~���̏ꍇ������
		if (m_bStop) { break; }

		// �^�C�����v��
		assert(m_funcCount != nullptr);
		m_funcCount(fDeltaTime);

		break;

	case STATE_END:
		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// ���Ԃ̌v�Z
	CalcTime();
}

//============================================================
//	�`�揈��
//============================================================
void CTimer::Draw(void)
{

}

//============================================================
//	��������
//============================================================
CTimer *CTimer::Create
(
	const float fTime,	// �J�n����
	const float fLimit	// ��������
)
{
	// �^�C�}�[�̐���
	CTimer *pTimer = new CTimer;
	if (pTimer == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �^�C�}�[�̏�����
		if (FAILED(pTimer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �^�C�}�[�̔j��
			SAFE_DELETE(pTimer);
			return nullptr;
		}

		// �J�n���Ԃ�ݒ�
		pTimer->SetTime(fTime);

		// �������Ԃ�ݒ�
		pTimer->SetLimit(fLimit);

		// �m�ۂ����A�h���X��Ԃ�
		return pTimer;
	}
}

//============================================================
//	�S�^�C�}�[�̌v���󋵂̐ݒ菈��
//============================================================
void CTimer::EnableStopAll(const bool bStop)
{
	// TODO�F����Ń��[�v��������Ԃ��H

	std::list<CTimer*>::iterator itr = m_list.GetBegin();	// �擪�C�e���[�^�[

	// �S�^�C�}�[�I�u�W�F�N�g�̌v���󋵂�ݒ�
	while (m_list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		(*itr)->EnableStop(bStop);
	}
}

//============================================================
//	�v���J�n����
//============================================================
void CTimer::Start(void)
{
	if (m_state != STATE_COUNT)
	{ // �^�C���̌v�����ł͂Ȃ��ꍇ

		// ���~��Ԃɂ���
		EnableStop(false);

		// �v���J�n��Ԃɂ���
		m_state = STATE_COUNT;
	}
}

//============================================================
//	�v���I������
//============================================================
void CTimer::End(void)
{
	if (m_state == STATE_COUNT)
	{ // �^�C���̌v�����̏ꍇ

		// ��~��Ԃɂ���
		EnableStop(true);

		// �v���I����Ԃɂ���
		m_state = STATE_END;
	}
}

//============================================================
//	�v����~�̗L�������̐ݒ菈��
//============================================================
void CTimer::EnableStop(const bool bStop)
{
	// �����̒�~�󋵂���
	m_bStop = bStop;
}

//============================================================
//	���Ԃ̉��Z����
//============================================================
void CTimer::AddTime(const float fTime)
{
	// ���Ԃ����Z
	m_fTime += fTime;

	// ���Ԃ�␳
	UtilFunc::Transformation::ValueNormalize(m_fTime, timer::TIME_MIN, timer::TIME_MAX);
}

//============================================================
//	���Ԃ̐ݒ菈��
//============================================================
void CTimer::SetTime(const float fTime)
{
	// ���Ԃ�ݒ�
	m_fTime = fTime;

	// ���Ԃ�␳
	UtilFunc::Transformation::ValueNormalize(m_fTime, timer::TIME_MIN, timer::TIME_MAX);
}

//============================================================
//	�������Ԃ̐ݒ菈��
//============================================================
void CTimer::SetLimit(const float fLimit)
{
	// �������Ԃ�ۑ�
	m_fLimit = fLimit;

	// �������Ԃ�␳
	UtilFunc::Transformation::ValueNormalize(m_fLimit, timer::TIME_MIN, timer::TIME_MAX);

	if (m_fLimit <= 0.0f)
	{ // �������Ԃ��Ȃ��ꍇ

		// �J�E���g�A�b�v�֐���ݒ�
		m_funcCount = std::bind(&CTimer::CountUp, this, std::placeholders::_1);
	}
	else
	{ // ���Ԑ���������ꍇ

		// �����̊J�n���Ԃ�ݒ�
		m_fTime = fLimit;

		// �J�E���g�_�E���֐���ݒ�
		m_funcCount = std::bind(&CTimer::CountDown, this, std::placeholders::_1);
	}
}

//============================================================
//	�J�E���g�A�b�v����
//============================================================
void CTimer::CountUp(const float fDeltaTime)
{
	// �f���^�^�C�������Z
	m_fTime += fDeltaTime;
}

//============================================================
//	�J�E���g�_�E������
//============================================================
void CTimer::CountDown(const float fDeltaTime)
{
	// �f���^�^�C�������Z
	m_fTime -= fDeltaTime;
	if (m_fTime <= 0.0f)
	{  // �J�E���g�I�������ꍇ

		// �^�C����␳
		m_fTime = 0.0f;

		// �v�����I��
		End();
	}
}

//============================================================
//	���Ԃ̌v�Z����
//============================================================
void CTimer::CalcTime(void)
{
	m_lTime	= (DWORD)(m_fTime * 1000.0f);	// �b���~���b�ɕϊ�
	m_nMin	= (int)(m_lTime / 60000);		// �����v�Z
	m_nSec	= (int)(m_lTime / 1000) % 60;	// �b���v�Z
	m_nMSec	= (int)(m_lTime % 1000);		// �~���b���v�Z
}
