//=============================================================================
// 
//  �v���C���[���� [people.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "people.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "model.h"
#include "player.h"
#include "camera.h"
#include "3D_Effect.h"
#include "shadow.h"
#include "sound.h"
#include "stage.h"
#include "objectX.h"
#include "limitarea.h"
#include "debugproc.h"
#include "motion.h"

// �h���N���X
#include "kite.h"


//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float MOVE_VELOCITY = 15.0f;	// �ړ����x
	const int TURN_RIGHT = 100;	// ���E�Ԋu
}

namespace STATE_TIME
{
	const float FADEOUT = 0.6f;	// ���S����
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
// ��Ԋ֐�
CPeople::STATE_FUNC CPeople::m_StateFunc[] =
{
	&CPeople::StateNone,	// �Ȃ�
	&CPeople::StateFadeIn,	// �t�F�[�h�C��
	&CPeople::StateFadeOut,	// �t�F�[�h�A�E�g
};

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CListManager<CPeople> CPeople::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPeople::CPeople(int nPriority) : CObjectChara(nPriority)
{
	// �l�̃N���A
	m_state = STATE::STATE_NONE;	// ���
	m_Oldstate = m_state;	// �O��̏��
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �}�e���A���̐F
	m_TargetPosition = mylib_const::DEFAULT_VECTOR3;	// �ڕW�̈ʒu

	
	m_fStateTime = 0.0f;		// ��ԑJ�ڃJ�E���^�[
	m_sMotionFrag = SMotionFrag();		// ���[�V�����̃t���O
	m_pShadow = nullptr;
	m_flame = 0;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPeople::~CPeople()
{

}

//==========================================================================
// ��������
//==========================================================================
CPeople* CPeople::Create(const std::string& filename, const MyLib::Vector3& pos, const TYPE& type)
{
	// �������̊m��
	CPeople* pPeople = nullptr;

	switch (type)
	{
	case TYPE::TYPE_KITE:
	case TYPE::TYPE_KITE2:
	case TYPE::TYPE_KITE3:
		pPeople = DEBUG_NEW CKite;
		break;

	default:
		pPeople = DEBUG_NEW CPeople;
		break;
	}

	if (pPeople != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �ʒu�ݒ�
		pPeople->SetPosition(pos);
		pPeople->CObject::SetOriginPosition(pos);

		// �e�L�X�g�ǂݍ���
		HRESULT hr = pPeople->LoadText(filename.c_str());
		if (FAILED(hr))
		{// ���s���Ă�����
			return nullptr;
		}

		// ����������
		pPeople->Init();
	}

	return pPeople;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPeople::Init()
{
	// �e��ϐ��̏�����
	m_state = STATE::STATE_FADEIN;	// ���
	m_Oldstate = m_state;
	m_fStateTime = 0.0f;			// ��ԑJ�ڃJ�E���^�[

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_ENEMY);

	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// �f�t�H���g�ݒ�
	CMotion* pMotion = GetMotion();
	int motionmove = UtilFunc::Transformation::Random(0, pMotion->GetNumMotion() - 1);

	if (pMotion != nullptr)
	{
		pMotion->Set(motionmove);
	}

	float rotY = D3DX_PI * 0.5f;
	if (pMotion->IsGetMove(pMotion->GetType()) == 1)
	{// �ړ����[�V����

		// �����ݒ�
		if (rand() % 2 == 0)
		{
			rotY += D3DX_PI;
		}
	}
	else
	{
		rotY = UtilFunc::Transformation::Random(-31, 31) * 0.1f;
	}
	UtilFunc::Transformation::RotNormalize(rotY);
	SetRotation(MyLib::Vector3(0.0f, rotY, 0.0f));
	SetRotDest(rotY);

	// �ړ����x
	m_fMoveVelocity = MOVE_VELOCITY + UtilFunc::Transformation::Random(-20, 50) * 0.1f;

	// �ړ���
	MyLib::Vector3 move, rot = GetRotation();
	move.x = sinf(D3DX_PI + rot.y) * m_fMoveVelocity;
	move.z = cosf(D3DX_PI + rot.y) * m_fMoveVelocity;
	SetMove(move);

	// �����t���[��
	m_flame = UtilFunc::Transformation::Random(0, TURN_RIGHT / 2);

	return S_OK;
}

//==========================================================================
// �e�L�X�g�ǂݍ���
//==========================================================================
HRESULT CPeople::LoadText(const char *pFileName)
{
	// �L�����쐬
	HRESULT hr = SetCharacter(pFileName);
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPeople::Uninit()
{
	
	// �e������
	if (m_pShadow != nullptr)
	{
		m_pShadow = nullptr;
	}

	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectChara::Uninit();
}

//==========================================================================
// ���S����
//==========================================================================
void CPeople::Kill()
{
	
	// �e������
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CPeople::Update()
{
	// ���S�̔���
	if (IsDeath() == true)
	{// ���S�t���O�������Ă�����
		return;
	}

	CMotion* pMotion = GetMotion();
	MyLib::Vector3 move = GetMove();
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	float deltaTime = CManager::GetInstance()->GetDeltaTime();

	if (pMotion->IsGetMove(pMotion->GetType()) == 1)
	{// �������[�V�����̎�

		// �����␳
		float rotDiff = GetRotDest() - rot.y;
		UtilFunc::Transformation::RotNormalize(rotDiff);

		rot.y += rotDiff * 0.2f;
		UtilFunc::Transformation::RotNormalize(rot.y);

		move.x += sinf(D3DX_PI + rot.y) * (m_fMoveVelocity * deltaTime);
		move.z += cosf(D3DX_PI + rot.y) * (m_fMoveVelocity * deltaTime);

		pos += move;
		m_flame++;

		if (m_flame >= TURN_RIGHT)
		{// �t���[��������

			if (m_flame == TURN_RIGHT)
			{// �t���[��������
				
				// �E��]
				float dest = rot.y + D3DX_PI * 0.5f;
				UtilFunc::Transformation::RotNormalize(dest);
				SetRotDest(dest);
			}

			if (m_flame >= TURN_RIGHT + 20)
			{
				// �E��]
				float dest = rot.y + D3DX_PI * 0.5f;
				UtilFunc::Transformation::RotNormalize(dest);
				SetRotDest(dest);

				// �t���[�����Z�b�g
				m_flame = UtilFunc::Transformation::Random(0, TURN_RIGHT / 2);
			}
		}
	}

	SetMove(move);
	SetPosition(pos);
	SetRotation(rot);

	// �ߋ��̈ʒu�ݒ�
	SetOldPosition(GetPosition());

	// �e�̏���
	CObjectChara::Update();

	// �����蔻��
	Collision();

	// ���S�̔���
	if (IsDeath())
	{// ���S�t���O�������Ă�����
		return;
	}

	// ��ԍX�V
	UpdateState();

	// ���S�̔���
	if (IsDeath())
	{// ���S�t���O�������Ă�����
		return;
	}

	
	// �e�̈ʒu�X�V
	if (m_pShadow != nullptr)
	{
		// �ʒu�擾
		MyLib::Vector3 pos = GetPosition();
		m_pShadow->SetPosition(MyLib::Vector3(pos.x, m_pShadow->GetPosition().y, pos.z));
	}

	if (GetPosition().y <= mylib_const::KILL_Y)
	{
		// �G�̏I������
		Kill();
		Uninit();
		return;
	}

	// ��l�̕�
	LimitArea();
}

//==========================================================================
// �����蔻��
//==========================================================================
void CPeople::Collision()
{
	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// �d�͏���
	move.y -= mylib_const::GRAVITY;

	// �ʒu�X�V
	pos += move;

	// �����␳
	move.x += (0.0f - move.x) * 0.25f;
	move.z += (0.0f - move.z) * 0.25f;

	if (move.x >= 0.1f || move.x <= -0.1f ||
		move.z >= 0.1f || move.z <= -0.1f)
	{// �ړ���
		m_sMotionFrag.bMove = true;
	}
	else
	{
		m_sMotionFrag.bMove = false;
	}

	if (300.0f > pos.y)
	{// �n�ʂ̕���������荂��������

		// �n�ʂ̍����ɕ␳
		pos.y = 300.0f;
		
		// �W�����v�g�p�\�ɂ���
		move.y = 0.0f;
		m_sMotionFrag.bJump = false;
	}

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);
}

//==========================================================================
// ���n���̏���
//==========================================================================
void CPeople::ProcessLanding()
{
	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �W�����v�g�p�\�ɂ���
	move.y = 0.0f;
	m_sMotionFrag.bJump = false;

	// �ړ��ʐݒ�
	SetMove(move);
}

//==========================================================================
// ��ԍX�V����
//==========================================================================
void CPeople::UpdateState()
{
	// �F�ݒ�
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_mMatcol.a);

	// ��ԑJ�ڃJ�E���^�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// �����Ȃ����
//==========================================================================
void CPeople::StateNone()
{
	// �F�ݒ�
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fStateTime = 0.0f;
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CPeople::StateFadeIn()
{
	// �F�ݒ�
	m_mMatcol.a = m_fStateTime / STATE_TIME::FADEOUT;

	if (m_fStateTime >= STATE_TIME::FADEOUT)
	{
		m_state = STATE::STATE_NONE;
		return;
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CPeople::StateFadeOut()
{
	// �F�ݒ�
	m_mMatcol.a = 1.0f - m_fStateTime / STATE_TIME::FADEOUT;

	if (m_fStateTime >= STATE_TIME::FADEOUT)
	{
		// �I������
		Kill();
		Uninit();
		return;
	}
}


//==========================================================================
// ��l�̕�
//==========================================================================
void CPeople::LimitArea()
{
	return;

	// ���g�̒l���擾
	MyLib::Vector3 pos = GetPosition();

	// ��l�̕ǎ擾
	CListManager<CLimitArea> limitareaList = CLimitArea::GetListObj();
	CLimitArea* pLimitArea = nullptr;

	while (limitareaList.ListLoop(&pLimitArea))
	{
		CLimitArea::sLimitEreaInfo info = pLimitArea->GetLimitEreaInfo();

		// ��l�̕ǂ�K�p
		if (pos.x + GetRadius() >= info.fMaxX) { pos.x = info.fMaxX - GetRadius(); }
		if (pos.x - GetRadius() <= info.fMinX) { pos.x = info.fMinX + GetRadius(); }
		if (pos.z + GetRadius() >= info.fMaxZ) { pos.z = info.fMaxZ - GetRadius(); }
		if (pos.z - GetRadius() <= info.fMinZ) { pos.z = info.fMinZ + GetRadius(); }
	}

	// �l��K�p
	SetPosition(pos);

}

//==========================================================================
// ���[�V�����̐ݒ�
//==========================================================================
void CPeople::SetMotion(int motionIdx)
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	pMotion->Set(motionIdx);
}

//==========================================================================
// �U��������
//==========================================================================
void CPeople::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	return;
}

//==========================================================================
// �U�����蒆����
//==========================================================================
void CPeople::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
{
	//return;
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// ����̈ʒu
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), *pATKInfo);

	if (pATKInfo->fRangeSize == 0.0f)
	{
		return;
	}

	if (pATKInfo->bEndAtk)
	{
		return;
	}

#if _DEBUG
	CEffect3D::Create(weponpos, MyLib::Vector3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), pATKInfo->fRangeSize, 10, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

	

}

//==========================================================================
// �`�揈��
//==========================================================================
void CPeople::Draw()
{
	if (m_state == STATE_FADEOUT || m_state == STATE::STATE_FADEIN)
	{
		CObjectChara::Draw(m_mMatcol.a);
	}
	else if (m_mMatcol != D3DXCOLOR(1.0f, 1.0f, 1.0f, m_mMatcol.a))
	{
		// �I�u�W�F�N�g�L�����̕`��
		CObjectChara::Draw(m_mMatcol);
	}
	else
	{
		// �I�u�W�F�N�g�L�����̕`��
		CObjectChara::Draw();
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CPeople::SetState(STATE state)
{
	m_state = state;
}
