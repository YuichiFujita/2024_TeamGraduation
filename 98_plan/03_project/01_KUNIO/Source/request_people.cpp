//=============================================================================
// 
//  �˗��l���� [request_people.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "request_people.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "model.h"
#include "player.h"
#include "camera.h"
#include "3D_Effect.h"
#include "shadow.h"
#include "sound.h"
#include "objectX.h"
#include "limitarea.h"
#include "debugproc.h"
#include "baggage.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\character\\mob\\person_01\\setup_human.txt";
}

namespace STATE_TIME
{
	const float FADEOUT = 0.6f;	// ���S����
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
// ��Ԋ֐�
CRequestPeople::STATE_FUNC CRequestPeople::m_StateFunc[] =
{
	&CRequestPeople::StateNone,	// �Ȃ�
	&CRequestPeople::StateFadeIn,	// �t�F�[�h�C��
	&CRequestPeople::StateFadeOut,	// �t�F�[�h�A�E�g
	&CRequestPeople::StateWait,		// �ҋ@
	&CRequestPeople::StatePass,		// �p�X
	&CRequestPeople::StateByeBye,	// �o�C�o�C
};


//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CListManager<CRequestPeople> CRequestPeople::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CRequestPeople::CRequestPeople(int nPriority) : CObjectChara(nPriority)
{
	// �l�̃N���A
	m_state = STATE::STATE_NONE;	// ���
	m_Oldstate = m_state;	// �O��̏��
	m_fStateTime = 0.0f;		// ��ԑJ�ڃJ�E���^�[
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	
	m_pShadow = nullptr;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CRequestPeople::~CRequestPeople()
{

}

//==========================================================================
// ��������
//==========================================================================
CRequestPeople* CRequestPeople::Create(const MyLib::Vector3& pos)
{
	// �������̊m��
	CRequestPeople* pPeople = DEBUG_NEW CRequestPeople;

	if (pPeople != nullptr)
	{
		// �ʒu�ݒ�
		pPeople->SetPosition(pos);
		pPeople->CObject::SetOriginPosition(pos);

		// �e�L�X�g�ǂݍ���
		HRESULT hr = pPeople->LoadText(FILENAME);
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
HRESULT CRequestPeople::Init()
{
	// �e��ϐ��̏�����
	m_state = STATE::STATE_PASS;	// ���
	m_Oldstate = m_state;
	m_fStateTime = 0.0f;			// ��ԑJ�ڃJ�E���^�[

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_ENEMY);

	// �����ݒ�
	SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));

	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// �f�t�H���g�ݒ�
	CMotion* pMotion = GetMotion();
	if (pMotion != nullptr)
	{
		pMotion->Set(MOTION::MOTION_PASS);
	}

	return S_OK;
}

//==========================================================================
// �e�L�X�g�ǂݍ���
//==========================================================================
HRESULT CRequestPeople::LoadText(const std::string& pFileName)
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
void CRequestPeople::Uninit()
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
void CRequestPeople::Kill()
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
void CRequestPeople::Update()
{
	// ���S�̔���
	if (IsDeath() == true)
	{// ���S�t���O�������Ă�����
		return;
	}

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
}

//==========================================================================
// �����蔻��
//==========================================================================
void CRequestPeople::Collision()
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

	if (300.0f > pos.y)
	{// �n�ʂ̕���������荂��������

		// �n�ʂ̍����ɕ␳
		pos.y = 300.0f;
		
		// �W�����v�g�p�\�ɂ���
		move.y = 0.0f;
	}

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);
}

//==========================================================================
// ��ԍX�V����
//==========================================================================
void CRequestPeople::UpdateState()
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
void CRequestPeople::StateNone()
{
	// �F�ݒ�
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fStateTime = 0.0f;
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CRequestPeople::StateFadeIn()
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
void CRequestPeople::StateFadeOut()
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
// �ҋ@
//==========================================================================
void CRequestPeople::StateWait()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	if (pMotion->GetType() != MOTION::MOTION_BYEBYE)
	{
		// ���[�V�����ݒ�
		pMotion->Set(MOTION::MOTION_BYEBYE);
	}
}

//==========================================================================
// �p�X
//==========================================================================
void CRequestPeople::StatePass()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	int nType = pMotion->GetType();
	if (nType != MOTION::MOTION_PASS)
	{
		// ���[�V�����ݒ�
		pMotion->Set(MOTION::MOTION_PASS);
	}

	nType = pMotion->GetType();
	if ((nType == MOTION::MOTION_PASS && pMotion->IsFinish()) ||
		nType != MOTION::MOTION_PASS)
	{// �p�X�I�� or �p�X�ȊO

		// ���[�V�����ݒ�
		pMotion->Set(MOTION::MOTION_BYEBYE);
		m_state = STATE::STATE_BYEBYE;
	}
}

//==========================================================================
// �o�C�o�C
//==========================================================================
void CRequestPeople::StateByeBye()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	int nType = pMotion->GetType();

	if (nType != MOTION::MOTION_BYEBYE)
	{
		// ���[�V�����ݒ�
		pMotion->Set(MOTION::MOTION_BYEBYE);
	}

}

//==========================================================================
// ���[�V�����̐ݒ�
//==========================================================================
void CRequestPeople::SetMotion(int motionIdx)
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
void CRequestPeople::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	int motionType = pMotion->GetType();

	switch (motionType)
	{
	case MOTION::MOTION_PASS:
	{
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);
		if (nCntATK == 0)
		{
			pBaggage->SetState(CBaggage::STATE::STATE_APPEARANCE);
			// SE�Đ�
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_TORIDASU);
		}
		else if (nCntATK == 1)
		{
			pBaggage->SetState(CBaggage::STATE::STATE_PASS);
			pBaggage->SetAwayStartPosition(pMotion->GetAttackPosition(GetModel(), ATKInfo));

			// SE�Đ�
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_NAGERU);
		}
	}
	break;

	default:
		break;
	}
}

//==========================================================================
// �U�����蒆����
//==========================================================================
void CRequestPeople::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	int motionType = pMotion->GetType();

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// ����̈ʒu
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), *pATKInfo);


	switch (motionType)
	{
	case MOTION::MOTION_PASS:
	{
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);

		if (pBaggage->GetState() != CBaggage::STATE::STATE_PASS)
		{
			pBaggage->SetPosition(weponpos);
		}
	}
		break;

	default:
		break;
	}


	if (pATKInfo->fRangeSize == 0.0f)
	{
		return;
	}

	if (pATKInfo->bEndAtk)
	{
		return;
	}

#if _DEBUG
	CEffect3D::Create(
		weponpos,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		pATKInfo->fRangeSize,
		10,
		CEffect3D::MOVEEFFECT_NONE,
		CEffect3D::TYPE_NORMAL);
#endif

	

}

//==========================================================================
// �`�揈��
//==========================================================================
void CRequestPeople::Draw()
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
void CRequestPeople::SetState(STATE state)
{
	m_state = state;
}
