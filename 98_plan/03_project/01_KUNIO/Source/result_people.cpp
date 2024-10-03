//=============================================================================
// 
//  �˗��l���� [result_people.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "result_people.h"
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
#include "debugproc.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\character\\mob\\person_01\\setup_human.txt";
}

namespace STATE_TIME
{
	const float THROW = 0.5f;	// ����
	const float FADEOUT = 0.6f;	// ���S����
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
// ��Ԋ֐�
CResultPeople::STATE_FUNC CResultPeople::m_StateFunc[] =
{
	&CResultPeople::StateNone,	// �Ȃ�
	&CResultPeople::StateFadeIn,	// �t�F�[�h�C��
	&CResultPeople::StateFadeOut,	// �t�F�[�h�A�E�g
	&CResultPeople::StateWait,		// �ҋ@
	&CResultPeople::StateResult,	// ���U���g
	&CResultPeople::StateByeBye,	// �o�C�o�C
};


//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CListManager<CResultPeople> CResultPeople::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CResultPeople::CResultPeople(int nPriority) : CObjectChara(nPriority)
{
	// �l�̃N���A
	m_state = STATE::STATE_NONE;	// ���
	m_Oldstate = m_state;			// �O��̏��
	m_fStateTime = 0.0f;			// ��ԑJ�ڃJ�E���^�[
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	
	m_pShadow = nullptr;
	m_pScroll = nullptr;		// ������
	m_fScrollThrowTimer = 0.0f;	// �������^�C�}�[
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CResultPeople::~CResultPeople()
{

}

//==========================================================================
// ��������
//==========================================================================
CResultPeople* CResultPeople::Create(const MyLib::Vector3& pos)
{
	// �������̊m��
	CResultPeople* pPeople = DEBUG_NEW CResultPeople;

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
HRESULT CResultPeople::Init()
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
		pMotion->ResetPose(MOTION::MOTION_RESULT);
		pMotion->Set(MOTION::MOTION_RESULT);
	}

	return S_OK;
}

//==========================================================================
// �e�L�X�g�ǂݍ���
//==========================================================================
HRESULT CResultPeople::LoadText(const std::string& pFileName)
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
void CResultPeople::Uninit()
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
void CResultPeople::Kill()
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
void CResultPeople::Update()
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

	// �������̍X�V����
	UpdateScroll();

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
void CResultPeople::Collision()
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
// �������̍X�V����
//==========================================================================
void CResultPeople::UpdateScroll()
{
	if (m_pScroll == nullptr) return;

	// �ڕW�̈ʒu���܂��ݒ肳��ĂȂ�
	if (m_posDestScroll.IsNearlyZero(0.1f)) return;

	// �����̃^�C�}�[���Z
	m_fScrollThrowTimer += CManager::GetInstance()->GetDeltaTime();

	if (m_fScrollThrowTimer < STATE_TIME::THROW + 0.2f)
	{
		// ���`���
		MyLib::Vector3 pos, posOrigin = m_pScroll->GetOriginPosition();
		pos.x = UtilFunc::Correction::EaseInExpo(posOrigin.x, m_posDestScroll.x, 0.0f, 0.5f, m_fScrollThrowTimer);
		pos.y = UtilFunc::Correction::EaseInExpo(posOrigin.y, m_posDestScroll.y, 0.0f, 0.5f, m_fScrollThrowTimer);
		pos.z = UtilFunc::Correction::EaseInExpo(posOrigin.z, m_posDestScroll.z, 0.0f, 0.5f, m_fScrollThrowTimer);

		// �ʒu�ݒ�
		m_pScroll->SetPosition(pos);

		// �ړ��ʐݒ�
		m_pScroll->SetMove(MyLib::Vector3(0.0f, 4.0f, 0.0f));

	}
	else
	{
		// ���`���
		MyLib::Vector3 pos = m_pScroll->GetPosition();
		MyLib::Vector3 move = m_pScroll->GetMove();
		MyLib::Vector3 rot = m_pScroll->GetRotation();

		// ����
		pos.y += move.y;
		move.y -= 0.6f;

		pos.z -= 5.0f;

		// �����_����]
		rot.y += D3DX_PI * UtilFunc::Transformation::Random(10, 40) * 0.001f;
		rot.x += D3DX_PI * UtilFunc::Transformation::Random(10, 40) * 0.001f;

		// �ʒu�ݒ�
		m_pScroll->SetPosition(pos);
		m_pScroll->SetMove(move);
		m_pScroll->SetRotation(rot);
	}
}

//==========================================================================
// ��ԍX�V����
//==========================================================================
void CResultPeople::UpdateState()
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
void CResultPeople::StateNone()
{
	// �F�ݒ�
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fStateTime = 0.0f;
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CResultPeople::StateFadeIn()
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
void CResultPeople::StateFadeOut()
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
void CResultPeople::StateWait()
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
// ���U���g
//==========================================================================
void CResultPeople::StateResult()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	int nType = pMotion->GetType();
	if (nType != MOTION::MOTION_RESULT)
	{
		// ���[�V�����ݒ�
		pMotion->Set(MOTION::MOTION_RESULT);
	}

	nType = pMotion->GetType();
	if ((nType == MOTION::MOTION_RESULT && pMotion->IsFinish()) ||
		nType != MOTION::MOTION_RESULT)
	{// ���U���g�I�� or ���U���g�ȊO

		// ���[�V�����ݒ�
		pMotion->Set(MOTION::MOTION_BYEBYE);
		m_state = STATE::STATE_BYEBYE;
	}
}

//==========================================================================
// �o�C�o�C
//==========================================================================
void CResultPeople::StateByeBye()
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
void CResultPeople::SetMotion(int motionIdx)
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
void CResultPeople::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
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
	case MOTION::MOTION_RESULT:
	{
		if (nCntATK == 0)
		{
			// ����������
			CreateScroll();
		}
		else if (nCntATK == 1)
		{
			// ���̈ʒu�ݒ�
			m_pScroll->SetOriginPosition(m_pScroll->GetPosition());

			// �ڕW�̈ʒu�ݒ�
			m_posDestScroll = CManager::GetInstance()->GetCamera()->GetPositionV() + MyLib::Vector3(0.0f, 0.0f, 100.0f);

			// �������̓����^�C�}�[
			m_fScrollThrowTimer = 0.0f;

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
void CResultPeople::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
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
	case MOTION::MOTION_RESULT:
	{
		if (m_pScroll != nullptr)
		{
			// �������ʒu�ݒ�
			m_pScroll->SetPosition(weponpos);
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
// ����������
//==========================================================================
void CResultPeople::CreateScroll()
{
	// ����������
	m_pScroll = CObjectX::Create("data\\MODEL\\scroll.x");
}

//==========================================================================
// �`�揈��
//==========================================================================
void CResultPeople::Draw()
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
void CResultPeople::SetState(STATE state)
{
	m_state = state;
}
