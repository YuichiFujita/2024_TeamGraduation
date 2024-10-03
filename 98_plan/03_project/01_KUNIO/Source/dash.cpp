//=============================================================================
// 
//  �_�b�V������ [dash.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "dash.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\key\\A.png";	// �e�N�X�`���̃t�@�C��
	const float TIME_INTERVAL = 0.3f;
	const float TIME_DASH = 0.2f;
	const float MOVE_VELOCITY = 5.0f;
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CDash::STATE_FUNC CDash::m_StateFuncList[] =
{
	&CDash::StateNone,	// �Ȃ�
	&CDash::StateDash,	// �_�b�V��
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CDash::CDash(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_fDashInterval = 0.0f;		// �_�b�V���̃C���^�[�o��
	m_state = State::STATE_NONE;
	memset(m_nCntTrigger, 0, sizeof(m_nCntTrigger));	// �g���K�[�̃J�E���g

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CDash::~CDash()
{

}

//==========================================================================
// ��������
//==========================================================================
CDash* CDash::Create()
{
	// �������̊m��
	CDash* pObj = DEBUG_NEW CDash;

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
HRESULT CDash::Init()
{

	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SAMPLE);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 50.0f);
#endif
	SetSize(size);
	SetSizeOrigin(size);


	// �ʒu�A�����ݒ�͕K�v������Βǉ�
	SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CDash::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CDash::Update()
{
	// ��ԍX�V
	UpdateState();

	// �擾
	MyLib::Vector3 pos = GetPosition(), move = GetMove();

	// �ړ�
	pos += move;
	move += (MyLib::Vector3() - move) * 0.15f;

	// ���ݒ�
	SetPosition(pos);
	SetMove(move);

	for (int i = 0; i < Angle::ANGLE_MAX; i++)
	{
		ImGui::Text("[%d] : %d", i, m_nCntTrigger[i]);
	}

	// �X�V����
	CObject2D::Update();
}

//==========================================================================
// �g���K�[
//==========================================================================
CDash::MyStruct CDash::Trigger(Angle angle)
{
	MyStruct info;
	info.bDash = false;

	if (m_nCntTrigger[angle] == 1)
	{// �_�b�V��
		info.bDash = true;
		info.angle = angle;

		m_fStateTime = 0.0f;
		m_state = State::STATE_DASH;
	}

	// �^�C�}�[���Z�b�g
	m_fDashInterval = TIME_INTERVAL;

	// �g���K�[�̃J�E���g
	m_nCntTrigger[angle] = (m_nCntTrigger[angle] + 1) % 2;

	return info;
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CDash::UpdateState()
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// ���s
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CDash::StateNone()
{
	m_fStateTime = 0.0f;

	// �C���v�b�g�擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �_�b�V������
	MyStruct info;

	// �X�e�B�b�N�̌���
	float stickAngle = pPad->GetStickRotL(0);

	bool bMove = false;
	if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) || pPad->GetStickMoveL(0).y > 0)
	{// ��
		bMove = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) || pPad->GetStickMoveL(0).x < 0)
		{// ����
			info.angle = Angle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) || pPad->GetStickMoveL(0).x > 0)
		{// �E��
			info.angle = Angle::ANGLE_RIGHTUP;
		}
		else
		{// ��
			info.angle = Angle::ANGLE_UP;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) || pPad->GetStickMoveL(0).y < 0)
	{// ��
		bMove = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) || pPad->GetStickMoveL(0).x < 0)
		{// ����
			info.angle = Angle::ANGLE_LEFTDW;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) || pPad->GetStickMoveL(0).x > 0)
		{// �E��
			info.angle = Angle::ANGLE_RIGHTDW;
		}
		else
		{// ��
			info.angle = Angle::ANGLE_DOWN;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) || pPad->GetStickMoveL(0).x > 0)
	{// �E
		bMove = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) || pPad->GetStickMoveL(0).y < 0)
		{// �E��
			info.angle = Angle::ANGLE_RIGHTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) || pPad->GetStickMoveL(0).y > 0)
		{// �E��
			info.angle = Angle::ANGLE_RIGHTDW;
		}
		else
		{// �E
			info.angle = Angle::ANGLE_RIGHT;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) || pPad->GetStickMoveL(0).x < 0)
	{// ��
		bMove = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) || pPad->GetStickMoveL(0).y < 0)
		{// ����
			info.angle = Angle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) || pPad->GetStickMoveL(0).y > 0)
		{// ����
			info.angle = Angle::ANGLE_LEFTDW;
		}
		else
		{// ��
			info.angle = Angle::ANGLE_LEFT;
		}
	}

	if (bMove)
	{
		MyLib::Vector3 move;
		float division = (D3DX_PI * 2.0f) / Angle::ANGLE_MAX;
		move.x += cosf((D3DX_PI * -0.5f) + division * info.angle) * MOVE_VELOCITY;
		move.y += sinf((D3DX_PI * -0.5f) + division * info.angle) * MOVE_VELOCITY;
		SetMove(move);
	}

	info = MyStruct();
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
		(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0))
	{// ��

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
			(/*pPad->GetLStickTrigger(CInputGamepad::STICK_X) && */pPad->GetStickMoveL(0).x < 0))
		{// ����
			info = Trigger(Angle::ANGLE_LEFTUP);
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
			(/*pPad->GetLStickTrigger(CInputGamepad::STICK_X) && */pPad->GetStickMoveL(0).x > 0))
		{// �E��
			info = Trigger(Angle::ANGLE_RIGHTUP);
		}
		else
		{// ��
			info = Trigger(Angle::ANGLE_UP);
		}
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
		(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y < 0))
	{// ��

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
			(/*pPad->GetLStickTrigger(CInputGamepad::STICK_X) && */pPad->GetStickMoveL(0).x < 0))
		{// ����
			info = Trigger(Angle::ANGLE_LEFTDW);
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
			(/*pPad->GetLStickTrigger(CInputGamepad::STICK_X) && */pPad->GetStickMoveL(0).x > 0))
		{// �E��
			info = Trigger(Angle::ANGLE_RIGHTDW);
		}
		else
		{// ��
			info = Trigger(Angle::ANGLE_DOWN);
		}
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0))
	{// �E

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
			(/*pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && */pPad->GetStickMoveL(0).y < 0))
		{// �E��
			info = Trigger(Angle::ANGLE_RIGHTUP);
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
			(/*pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && */pPad->GetStickMoveL(0).y > 0))
		{// �E��
			info = Trigger(Angle::ANGLE_RIGHTDW);
		}
		else
		{// �E
			info = Trigger(Angle::ANGLE_RIGHT);
		}
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0))
	{// ��

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
			(/*pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && */pPad->GetStickMoveL(0).y < 0))
		{// ����
			info = Trigger(Angle::ANGLE_LEFTUP);
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
			(/*pPad->GetLStickTrigger(CInputGamepad::STICK_Y) &&*/ pPad->GetStickMoveL(0).y > 0))
		{// ����
			info = Trigger(Angle::ANGLE_LEFTDW);
		}
		else
		{// ��
			info = Trigger(Angle::ANGLE_LEFT);
		}
	}

	

	// �_�b�V������
	if (info.bDash)
	{
		MyLib::Vector3 move;
		float division = (D3DX_PI * 2.0f) / Angle::ANGLE_MAX;
		move.x += cosf((D3DX_PI * -0.5f) + division * info.angle) * 25.0f;
		move.y += sinf((D3DX_PI * -0.5f) + division * info.angle) * 25.0f;
		SetMove(move);
		memset(m_nCntTrigger, 0, sizeof(m_nCntTrigger));
	}

	// �P�\���炵�Ă���
	m_fDashInterval -= CManager::GetInstance()->GetDeltaTime();
	if (m_fDashInterval <= 0.0f)
	{
		// �g���K�[�̃J�E���g
		memset(m_nCntTrigger, 0, sizeof(m_nCntTrigger));
	}
}

//==========================================================================
// �_�b�V��
//==========================================================================
void CDash::StateDash()
{
	if (m_fStateTime >= TIME_DASH)
	{
		m_fStateTime = 0.0f;
		m_state = State::STATE_NONE;
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CDash::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}
