//=============================================================================
// 
//  �^�C�}�[����(�^��) [timer_result.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "timer_result.h"
#include "manager.h"
#include "multinumber.h"
#include "debugproc.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"

#include "2D_effect.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\number\\number_oradano.png";	// �e�N�X�`���̃t�@�C��
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\result\\cleartime.png";

	const MyLib::Vector3 DEFAULT_POSITION = MyLib::Vector3(1100.0f, 100.0f, 0.0f);	// �����ʒu
	const D3DXVECTOR2 SIZE_NUMBER = D3DXVECTOR2(30.0f, 30.0f);
	const float DSTANCE_TIMER = SIZE_NUMBER.x * 2.25f;

	const float SIZE_HEIGHT = 50.0f;	// �c���̃T�C�Y
	const float MOVEVALUE_TEXT = 3.0f;	// �e�L�X�g�̈ړ���
	const float MOVEVALUE_TIME = 9.0f;	// �^�C���̈ړ���
	const float DSTANCE_MULTIPLAY = 2.25f;
}

namespace StateTime
{
	const float WAIT = 0.5f;	// �ҋ@
	const float EMPHASIZE = 1.0f;	// ����
	const int SCROLLEND_WAITFRAME = 60;
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CTimer_Result::STATE_FUNC CTimer_Result::m_StateFunc[] =
{
	&CTimer_Result::StateScrollText,	// ��������
	&CTimer_Result::StateSrollVoid,		// ��ԑ���
	&CTimer_Result::StateScrollTime,	// �^�C������
	&CTimer_Result::StateFinish,		// �I��
	&CTimer_Result::StateEmphasize,		// ����
	&CTimer_Result::StateNone,			// �Ȃɂ��Ȃ�

};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTimer_Result::CTimer_Result(int nPriority) : CTimer(nPriority)
{
	// �l�̃N���A
	m_fStateTime = 0.0f;		// ��ԃJ�E���^�[
	m_state = State::STATE_SCROLL_TEXT;			// ���
	m_fMoveTextLen = 0.0f;	// �e�L�X�g�̈ړ�����
	m_fMoveTimeLen = 0.0f;	// �^�C���̈ړ�����
	m_bFinish = false;		// �I��
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTimer_Result::~CTimer_Result()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTimer_Result::Init()
{
	//=============================
	// ��������
	//=============================
	CreateText();

	// ������
	CTimer::Init();
	m_pos = m_pText->GetPosition() + MyLib::Vector3(m_pText->GetSizeOrigin().x * 2.0f + 30.0f, 0.0f, 0.0f);

	// �A���J�[�|�C���g�����ɂ���
	for (int i = 0; i < 3; i++)
	{
		CMultiNumber* pMultiNumber = m_pClearTime[i];

		// UV�����ݒ�OFF
		pMultiNumber->SetEnableAutoUVSetting(true);

		CNumber** pNumber = pMultiNumber->GetNumber();

		for (int j = 0; j < pMultiNumber->GetDigit(); j++)
		{
			pNumber[j]->SetType(CObject::TYPE_OBJECT2D);
			CObject2D* pObj2D = pNumber[j]->GetObject2D();
			pObj2D->SetSize(D3DXVECTOR2(0.0f, pObj2D->GetSizeOrigin().y));
			pObj2D->SetAnchorType(CObject2D::AnchorPoint::LEFT);
		}
	}

	// �^�C�}�[
	ApplyTimer();


	// ��ԑJ��
	SetState(State::STATE_NONE);

	return S_OK;
}


//==========================================================================
// ��������
//==========================================================================
void CTimer_Result::CreateText()
{
	// ����
	m_pText = CObject2D::Create(6);
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	m_pText->BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_HEIGHT);
	m_pText->SetSize(D3DXVECTOR2(0.0f, size.y));
	m_pText->SetSizeOrigin(size);

	// �ʒu�ݒ�
	m_pText->SetPosition(MyLib::Vector3(200.0f, 350.0f, 0.0f));
	
	// �A���J�[�|�C���g�̐ݒ�
	m_pText->SetAnchorType(CObject2D::AnchorPoint::LEFT);
}

//==========================================================================
// �I������
//==========================================================================
void CTimer_Result::Uninit()
{
	CTimer::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTimer_Result::Update()
{
	// �X�V����
	CTimer::Update();

	// ��ԍX�V
	UpdateState();
}

//==========================================================================
// �^�C�}�[���f
//==========================================================================
void CTimer_Result::ApplyTimer()
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

		// �ʒu�ݒ�
		MyLib::Vector3 pos = m_pos;
		pos.x += (m_pClearTime[0]->GetNumber()[0]->GetSize().x * DSTANCE_MULTIPLAY) * (2 - i);
		m_pClearTime[i]->SetPosition(pos);

	}
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CTimer_Result::UpdateState()
{

	// ��ԃ^�C�}�[
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// �X�L�b�v
//==========================================================================
void CTimer_Result::Skip()
{
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{
		// ���[�h�ݒ�
		SetState(CTimer_Result::State::STATE_FINISH);
	}
}

//==========================================================================
// ��������
//==========================================================================
void CTimer_Result::StateScrollText()
{
	// �T�C�Y�擾
	D3DXVECTOR2 size = m_pText->GetSize(), sizeOrigin = m_pText->GetSizeOrigin();

	// �e�L�X�g�ړ��������Z
	m_fMoveTextLen += MOVEVALUE_TEXT;
	m_fMoveTextLen = UtilFunc::Transformation::Clamp(m_fMoveTextLen, 0.0f, sizeOrigin.x);

	if (m_fMoveTextLen >= sizeOrigin.x)
	{
		// ��ԑJ��
		SetState(State::STATE_SCROLL_VOID);
	}

	// �T�C�Y�ݒ�
	size.x = m_fMoveTextLen;
	m_pText->SetSize(size);

	// �e�N�X�`�����W�ݒ�
	D3DXVECTOR2* pTex = m_pText->GetTex();
	pTex[1].x = pTex[3].x = (size.x / sizeOrigin.x);

	// �X�L�b�v
	Skip();
}

//==========================================================================
// ��ԑ���
//==========================================================================
void CTimer_Result::StateSrollVoid()
{
	if (m_fStateTime >= StateTime::WAIT)
	{
		// ��ԑJ��
		SetState(State::STATE_SCROLL_TIME);

		// �T�E���h�Đ�
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
	}

	// �X�L�b�v
	Skip();
}

//==========================================================================
// �^�C������
//==========================================================================
void CTimer_Result::StateScrollTime()
{
	// �A���J�[�|�C���g�����ɂ���
	D3DXVECTOR2 size, sizeOrigin;
	MyLib::Vector3 pos;
	D3DXVECTOR2* pTex = nullptr;

	// �e�L�X�g�ړ��������Z
	m_fMoveTimeLen += MOVEVALUE_TIME;


	// ��_�̈ʒu
	float basePoint = (m_fMoveTimeLen + m_pClearTime[0]->GetNumber()[0]->GetPosition().x);


	for (int i = 0; i < 3; i++)
	{
		// �i���o�[�擾
		CMultiNumber* pMultiNumber = m_pClearTime[i];
		CNumber** pNumber = pMultiNumber->GetNumber();

		for (int j = 0; j < pMultiNumber->GetDigit(); j++)
		{
			// ������2D�I�u�W�F�N�g�擾
			CObject2D* pObj2D = pNumber[j]->GetObject2D();
			size = pObj2D->GetSize();
			sizeOrigin = pObj2D->GetSizeOrigin();
			pos = pObj2D->GetPosition();
			pTex = pObj2D->GetTex();

			// �ʒu�ɂ���Ă̊����v�Z
			float ratio = 0.0f;
			if (basePoint > pos.x)
			{
				ratio = (basePoint - pos.x) / sizeOrigin.x;
				ratio = UtilFunc::Transformation::Clamp(ratio, 0.0f, 1.0f);
			}

			// �����ɂ���ăT�C�Y�ύX
			size.x = ratio * sizeOrigin.x;
			pObj2D->SetSize(size);

			// �e�N�X�`���ݒ�
			float fNum = pNumber[j]->GetNum() * 0.1f;
			pTex[0].x = pTex[2].x = fNum;
			pTex[1].x = pTex[3].x = (fNum + 0.1f) * ratio;
		}
	}


	// �Ō�̕���������I�����
	if (m_pClearTime[0]->GetNumber()[1]->GetPosition().x + (StateTime::SCROLLEND_WAITFRAME * MOVEVALUE_TEXT) < basePoint)
	{
		// ��ԑJ��
		SetState(State::STATE_FINISH);
	}

	// �X�L�b�v
	Skip();
}

//==========================================================================
// �I��
//==========================================================================
void CTimer_Result::StateFinish()
{
	// �I���t���O
	m_bFinish = true;

	//=============================
	// �e�L�X�g�ύX
	//=============================
	{
		// �T�C�Y�ݒ�
		m_pText->SetSize(m_pText->GetSizeOrigin());

		// �e�N�X�`���ݒ�
		D3DXVECTOR2* pTex = m_pText->GetTex();
		pTex[1].x = pTex[3].x = 1.0f;
	}

	//=============================
	// �^�C���ύX
	//=============================
	for (int i = 0; i < 3; i++)
	{
		// �i���o�[�擾
		CMultiNumber* pMultiNumber = m_pClearTime[i];
		CNumber** pNumber = pMultiNumber->GetNumber();

		for (int j = 0; j < pMultiNumber->GetDigit(); j++)
		{
			// ������2D�I�u�W�F�N�g�擾
			CObject2D* pObj2D = pNumber[j]->GetObject2D();
			D3DXVECTOR2* pTex = pObj2D->GetTex();
			pObj2D->SetSize(pObj2D->GetSizeOrigin());

			// �e�N�X�`���ݒ�
			float fNum = pNumber[j]->GetNum() * 0.1f;
			pTex[0].x = pTex[2].x = fNum;
			pTex[1].x = pTex[3].x = fNum + 0.1f;
		}
	}


	// ��ԑJ��
	SetState(State::STATE_EMPHASIZE);
}

//==========================================================================
// ����
//==========================================================================
void CTimer_Result::StateEmphasize()
{
	D3DXVECTOR2 size, sizeOrigin;

	for (int i = 0; i < 3; i++)
	{
		// �i���o�[�擾
		CMultiNumber* pMultiNumber = m_pClearTime[i];
		CNumber** pNumber = pMultiNumber->GetNumber();

		for (int j = 0; j < pMultiNumber->GetDigit(); j++)
		{
			// ������2D�I�u�W�F�N�g�擾
			CObject2D* pObj2D = pNumber[j]->GetObject2D();
			size = pObj2D->GetSize();
			sizeOrigin = pObj2D->GetSizeOrigin();

			float ratio = m_fStateTime / StateTime::EMPHASIZE;
			size.x = sizeOrigin.x + sinf(D3DX_PI * ratio) * (sizeOrigin.x * 0.5f);
			size.y = sizeOrigin.y + sinf(D3DX_PI * ratio) * (sizeOrigin.y * 0.5f);

			pMultiNumber->SetKerning(size.y);
			pObj2D->SetSize(size);
		}
	}


	if (m_fStateTime >= StateTime::EMPHASIZE)
	{
		// ��ԑJ��
		SetState(State::STATE_NONE);
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CTimer_Result::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTimer_Result::Draw()
{

}
