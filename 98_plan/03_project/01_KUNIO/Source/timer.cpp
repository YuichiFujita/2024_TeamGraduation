//=============================================================================
// 
//  �^�C�}�[���� [timer.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "timer.h"
#include "manager.h"
#include "multinumber.h"
#include "debugproc.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"
#include "camera.h"
#include "baggage.h"
#include "baggageManager.h"

// �h����
#include "timer_result.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\number\\number_oradano03.png";	// �e�N�X�`���̃t�@�C��
	const std::string BGTEXTURE = "data\\TEXTURE\\timer\\bg.png";	// �e�N�X�`���̃t�@�C��
	const float DISTANCE = 1300.0f;	// ����
	const MyLib::Vector3 DEFAULT_POSITION = MyLib::Vector3(1070.0f, 70.0f, 0.0f);	// �����ʒu
	const D3DXVECTOR2 SIZE_NUMBER = D3DXVECTOR2(35.0f, 35.0f);
	const float DSTANCE_MULTIPLAY = 2.5f;
	const float ROTY = D3DX_PI * 0.0f;	// Y���p�x
	MyLib::Vector3 CAMERA_POS = MyLib::Vector3(-845.0f, -390.0f, 0.0f);
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CTimer *CTimer::m_pTimer = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �֐����X�g
//==========================================================================
CTimer::STATE_FUNC CTimer::m_StateFuncList[] =
{
	&CTimer::StateWait,		// �ҋ@
	&CTimer::StateGoal,		// �S�[��
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTimer::CTimer(int nPriority)
{
	// �l�̃N���A
	m_nPriority = nPriority;
	m_state = STATE_WAIT;		// ���
	m_fStateTime = 0.0f;		// ��Ԏ���
	m_fTime = 0.0f;				// ����
	m_pos = mylib_const::DEFAULT_VECTOR3;	// �ʒu
	m_posOrigin = mylib_const::DEFAULT_VECTOR3;	// ���̈ʒu
	m_bAddTime = false;			// �^�C�}�[���Z�̃t���O
	m_pMyCamera = nullptr;
	m_pBg = nullptr;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTimer::~CTimer()
{

}

//==========================================================================
// ��������
//==========================================================================
CTimer* CTimer::Create(Type type, int nPriority)
{
	if (m_pTimer != nullptr) return m_pTimer;

	// �������̊m��

	switch (type)
	{
	case TYPE_NORMAL:
		m_pTimer = DEBUG_NEW CTimer;
		break;

	case Type::TYPE_RESULT:
		m_pTimer = DEBUG_NEW CTimer_Result(nPriority);
		break;
	}

	if (m_pTimer != nullptr)
	{
		// ����������
		m_pTimer->Init();
	}

	return m_pTimer;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTimer::Init()
{
	// �e��ϐ�������
	m_pos = DEFAULT_POSITION;
	m_posOrigin = m_pos;	// ���̈ʒu
	m_fTime = 0.0f;	// ����
	m_state = STATE_WAIT;			// ���
	m_bAddTime = false;			// �^�C�}�[���Z�̃t���O

	// �w�i�𐶐����邨
	{
		MyLib::Vector3 bgpos = DEFAULT_POSITION;
		bgpos.x -= 10.0f;
		m_pBg = CObject2D::Create(6);
		m_pBg->SetPosition(bgpos);
		m_pBg->SetType(CObject::TYPE::TYPE_UI);

		// �e�N�X�`���ݒ�
		m_pBg->BindTexture(CTexture::GetInstance()->Regist(BGTEXTURE));
		D3DXVECTOR2 texture = CTexture::GetInstance()->GetImageSize(m_pBg->GetIdxTexture());
		m_pBg->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, SIZE_NUMBER.y * 2.0f));
		m_pBg->SetAlpha(0.7f);
	}

	// ���A�b�A�~���b�̌v�Z
	int time[3];
	time[0] = static_cast<int>(m_fTime) / (60);
	time[1] = static_cast<int>(m_fTime) % (60);
	time[2] = static_cast<int>(m_fTime) % 1000;

	for (int i = 0; i < 3; i++)
	{
		// ����
		m_pClearTime[i] = CMultiNumber::Create(
			m_pos,
			SIZE_NUMBER,
			2,
			CNumber::EObjectType::OBJECTTYPE_2D,
			TEXTURE, false, m_nPriority);
		if (m_pClearTime[i] == nullptr){
			continue;
		}
		CMultiNumber* pNumber = m_pClearTime[i];

		// �ʒu�ݒ�
		MyLib::Vector3 pos = m_pos;
		pos.x -= (SIZE_NUMBER.x * DSTANCE_MULTIPLAY) * i;
		pNumber->SetPosition(pos);

		// �E�񂹂ɐݒ�
		pNumber->SetAlignmentType(CMultiNumber::AlignmentType::ALIGNMENT_RIGHT);

		// �l�̐ݒ�
		pNumber->SetValue(time[i]);
		pNumber->SetType(CObject::TYPE::TYPE_UI);

		for (int i = 0; i < pNumber->GetDigit(); i++)
		{
			CNumber* pThisNum = pNumber->GetNumber(i);
			pThisNum->SetType(CObject::TYPE::TYPE_UI);
		}

	}

	// �`��p�J�����̐����Ɛݒ�
	m_pMyCamera = DEBUG_NEW CCamera;
	m_pMyCamera->Init();
	m_pMyCamera->SetRotation(MyLib::Vector3(0.0f, ROTY, 0.0f));
	m_pMyCamera->SetDistance(DISTANCE);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTimer::Uninit()
{
	if (m_pMyCamera != nullptr)
	{
		m_pMyCamera->Uninit();
		delete m_pMyCamera;
		m_pMyCamera = nullptr;
	}

	delete m_pTimer;
	m_pTimer = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CTimer::Update()
{
	// ��ԕʏ���
	(this->*(m_StateFuncList[m_state]))();

	// �J�����X�V
	SetCamera();

	if (!m_bAddTime)
	{
		return;
	}

	// �^�C�}�[���Z
	m_fTime += CManager::GetInstance()->GetDeltaTime();

	// �^�C�}�[�𕪁A�b�A�~���b�ɕϊ�
	ApplyTimer();
}

//==========================================================================
// �ҋ@���
//==========================================================================
void CTimer::StateWait()
{
	// ���ԃ��Z�b�g
	m_fStateTime = 0.0f;
}

//==========================================================================
// �S�[�����
//==========================================================================
void CTimer::StateGoal()
{
	m_bAddTime = false;
}

//==========================================================================
// �^�C�}�[���f
//==========================================================================
void CTimer::ApplyTimer()
{
	// �^�C�}�[�𕪁A�b�A�~���b�ɕϊ�
	int time[3];
	time[2] = static_cast<int>(m_fTime / 60);
	time[1] = static_cast<int>(m_fTime) % 60;
	time[0] = static_cast<int>((m_fTime - static_cast<int>(m_fTime)) * 1000);
	time[0] /= 10;

	for (int i = 0; i < 3; i++)
	{
		if (m_pClearTime[i] == nullptr) {
			continue;
		}

		// �l�̐ݒ�
		m_pClearTime[i]->SetValue(time[i]);
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTimer::Draw()
{

}

//==========================================================================
// �^�C�}�[�ݒ�Ɣ��f
//==========================================================================
void CTimer::SetTime(const float time)
{
	m_fTime = time;
	ApplyTimer();
}

//==========================================================================
// �J����
//==========================================================================
void CTimer::SetCamera()
{
	// �J�������Ȃ�
	if (m_pMyCamera == nullptr) { return; }

	CBaggage* pBag = CBaggage::GetListObj().GetData(0);

	// �ו����Ȃ�
	if (pBag == nullptr) { return; }

	// ���W���X�V
	MyLib::Vector3 pos = CAMERA_POS;
	m_pMyCamera->SetPositionR(pos);
	m_pMyCamera->SetPositionV();
}