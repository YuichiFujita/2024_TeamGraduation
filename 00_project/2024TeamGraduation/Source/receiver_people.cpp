//=============================================================================
// 
//  �͂��揈�� [receiver_people.cpp]
//  Author : Ibuki Okusada
// 
//=============================================================================
#include "receiver_people.h"
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
#include "receiver_people.h"
#include "catchresult.h"
#include "player.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\character\\mob\\person_01\\setup_human.txt";
	const float HEAD_POSITION = 300.0f;
	const float CATCH_RANGE = 500.0f;
	const float NEAR_RANGE = 1000.0f;
	const float WALK_TIMER = (1.0f / 120.0f);
	const MyLib::Vector3 START_ROT = MyLib::Vector3(0.0f, D3DX_PI * 0.5f, 0.0f);
	const float LOOKBACK_TIMER = (1.0f / 30.0f);
}

namespace STATE_TIME
{
	const float FADEOUT = 0.6f;	// ���S����
	const float DIVE = 1.0f;	// �_�C�u����
	const float CHASE = 0.5f;	// �ǂ�����
	const float CATCH = 0.3f;	// �L���b�`
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
// ��Ԋ֐�
CReceiverPeople::STATE_FUNC CReceiverPeople::m_StateFunc[] =
{
	&CReceiverPeople::StateNone,	// �Ȃ�
	&CReceiverPeople::StateFadeIn,	// �t�F�[�h�C��
	&CReceiverPeople::StateFadeOut,	// �t�F�[�h�A�E�g
	&CReceiverPeople::StateWait,	// �ҋ@
	&CReceiverPeople::StateGet,		// �L���b�`
	&CReceiverPeople::StateReturn,	// ���V�[�u
	&CReceiverPeople::StateYabai,	// ��΂�
	&CReceiverPeople::StateDive,	// �_�C�u
	&CReceiverPeople::StateDrown,	// �M���
	&CReceiverPeople::StateByeBye,	// �o�C�o�C
	&CReceiverPeople::StateWalk,	// ����
};


//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CListManager<CReceiverPeople> CReceiverPeople::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CReceiverPeople::CReceiverPeople(int nPriority) : CObjectChara(nPriority)
{
	// �l�̃N���A
	m_state = STATE::STATE_NONE;	// ���
	m_Oldstate = m_state;	// �O��̏��
	m_fStateTime = 0.0f;		// ��ԑJ�ڃJ�E���^�[
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_StartPos = 0.0f;
	m_fMoveTimer = 0.0f;
	m_pShadow = nullptr;
	m_bEnd = false;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CReceiverPeople::~CReceiverPeople()
{

}

//==========================================================================
// ��������
//==========================================================================
CReceiverPeople* CReceiverPeople::Create(const MyLib::Vector3& pos)
{
	// �������̊m��
	CReceiverPeople* pPeople = DEBUG_NEW CReceiverPeople;

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
HRESULT CReceiverPeople::Init()
{
	// �e��ϐ��̏�����
	m_state = STATE::STATE_GET;	// ���
	m_Oldstate = m_state;
	m_fStateTime = 0.0f;			// ��ԑJ�ڃJ�E���^�[

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_ENEMY);

	// �����ݒ�
	SetRotation(START_ROT);

	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// �f�t�H���g�ݒ�
	CMotion* pMotion = GetMotion();
	if (pMotion != nullptr)
	{
		pMotion->Set(MOTION::MOTION_DROWN);
	}

	// �V��
	MyLib::Vector3 setpos(GetPosition());

	int max = 5;
	float distance = 400.0f;
	setpos.z -= distance * (max / 2) + distance;
	for (int i = 0; i < max; i++)
	{
		setpos.z += distance;

		// ����
		CObjectX* pObj = CObjectX::Create("data\\MODEL\\map_object\\sanbashi.x", setpos, 0.0f);
		pObj->SetType(CObject::TYPE::TYPE_OBJECTX);
		pObj->SetScale(3.0f);
	}
	return S_OK;
}

//==========================================================================
// �e�L�X�g�ǂݍ���
//==========================================================================
HRESULT CReceiverPeople::LoadText(const std::string& pFileName)
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
void CReceiverPeople::Uninit()
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
void CReceiverPeople::Kill()
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
void CReceiverPeople::Update()
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
void CReceiverPeople::Collision()
{
	return;
	if (m_state == STATE::STATE_DIVE ||
		m_state == STATE::STATE_DROWN)
	{
		return;
	}

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
void CReceiverPeople::UpdateState()
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
void CReceiverPeople::StateNone()
{
	// �F�ݒ�
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fStateTime = 0.0f;
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CReceiverPeople::StateFadeIn()
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
void CReceiverPeople::StateFadeOut()
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
void CReceiverPeople::StateWait()
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

	// �S�[�����̂ݏ��ݒ�
	if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
	{
		return;
	}

	// �ו��̌��݈ʒu���r����
	CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);

	// �������擾
	MyLib::Vector3 bagpos = pBaggage->GetPosition();
	MyLib::Vector3 mypos = GetPosition();
	float distance = bagpos.DistanceXZ(mypos);

	if (!(distance <= NEAR_RANGE) && GetPosition().x <= bagpos.x)
	{// �߂�

		m_Distance = DISTANCE::DISTANCE_FAR;

		SetState(STATE::STATE_WALK);

		// ��Ԑݒ�
		pBaggage->SetState(CBaggage::STATE::STATE_RECEIVE);
		pBaggage->SetState(CBaggage::STATE::STATE_FALL);
		pBaggage->SetIsFall(true);
		pBaggage->SetMove(0.0f);
	}

	// �͂����̂�
	if (pBaggage->GetState() != CBaggage::STATE::STATE_SEND)
	{
		return;
	}


	// �L���b�`��������
	if (bagpos.y <= 0.0f)
	{// ���ɗ������Ă���
		m_StartPos = GetPosition();
		m_StartRot = GetRotation();

		// �ړ����ԃ��Z�b�g
		m_fMoveTimer = 0.0f;

		// ������ݒ�
		if (distance <= NEAR_RANGE)
		{// �߂�

			if (bagpos.y <= 0.0f)
			{
				m_Distance = DISTANCE::DISTANCE_NEAR;
				SetState(STATE::STATE_YABAI);

				// �ו��̌��݈ʒu���r����
				CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);

				// ��Ԑݒ�
				pBaggage->SetState(CBaggage::STATE::STATE_RECEIVE);
				pBaggage->SetState(CBaggage::STATE::STATE_FALL);
			}
		}
		else
		{// ����

			m_Distance = DISTANCE::DISTANCE_FAR;

			if (m_StartPos.x >= bagpos.x)
			{// �ו����͂��Ă��Ȃ�
				SetState(STATE::STATE_YABAI);

				// �h���b�v�ݒ�
				pBaggage->SetState(CBaggage::STATE::STATE_FALL);
			}
			else
			{// �ʂ�߂���
				SetState(STATE::STATE_WALK);

				// ��Ԑݒ�
				pBaggage->SetState(CBaggage::STATE::STATE_RECEIVE);
				pBaggage->SetState(CBaggage::STATE::STATE_FALL);
				pBaggage->SetIsFall(true);
				pBaggage->SetMove(0.0f);
			}
		}
	}
	else if (bagpos.y <= mypos.y + HEAD_POSITION && distance <= CATCH_RANGE)
	{// �L���b�`�ł���
		m_StartPos = GetPosition();
		SetState(STATE::STATE_GET);
	}
}

//==========================================================================
// �p�X
//==========================================================================
void CReceiverPeople::StatePass()
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
// �L���b�`
//==========================================================================
void CReceiverPeople::StateGet()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	int nType = pMotion->GetType();

	if (nType != MOTION::MOTION_GET)
	{
		// ���[�V�����ݒ�
		pMotion->Set(MOTION::MOTION_GET);

		// �����_��������
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->GetCameraMotion()->SetPosition(GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f));
		pCamera->GetCameraMotion()->SetEnablePause(true);
		pCamera->SetPositionR(GetPosition());
		pCamera->SetDistance(1200.0f);

		// �������ς���
		MyLib::Vector3 setrot = pCamera->GetRotation();
		setrot.z = D3DX_PI * -0.25f;
		pCamera->SetRotation(setrot);


		// �ׂ��G�t�F�N�g
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_SMASH,
			GetPosition(), MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
	}
	nType = pMotion->GetType();

	// �ו����擾
	CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);
	pBaggage->SetMove(0.0f);

	if ((nType == MOTION::MOTION_GET && pMotion->IsFinish()) ||
		nType != MOTION::MOTION_GET)
	{// �p�X�I�� or �p�X�ȊO

		// ���[�V�����ݒ�
		if (!m_bEnd)
		{
			CCatchResult::Create(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CCatchResult::TYPE::TYPE_OK);
			m_bEnd = true;

			// �ו����
			pBaggage->SetState(CBaggage::STATE::STATE_FALL);
			pBaggage->SetIsFall(true);
		}
	}


}

//==========================================================================
// ���V�[�u
//==========================================================================
void CReceiverPeople::StateReturn()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	if (pMotion->GetType() != MOTION::MOTION_RETURN)
	{
		// ���[�V�����ݒ�
		pMotion->Set(MOTION::MOTION_RETURN);

	}

	// �U�����擾
	CMotion::Info aInfo = pMotion->GetInfo(pMotion->GetType());
	CMotion::AttackInfo* AttackInfo = aInfo.AttackInfo[0];
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), *AttackInfo);

	int nType = pMotion->GetType();
	m_fMoveTimer += CManager::GetInstance()->GetDeltaTime();
	if (nType == MOTION::MOTION_RETURN && (int)pMotion->GetAllCount() >= AttackInfo->nMinCnt && (int)pMotion->GetAllCount() <= AttackInfo->nMaxCnt)
	{// ���^�[���̍U�����蒆

		// �ו��̌��݈ʒu���r����
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);
		MyLib::Vector3 bagpos = pBaggage->GetPosition(), bagOriginPos = pBaggage->GetOriginPosition();

		// �U�����擾
		CMotion::Info aInfo = pMotion->GetInfo(pMotion->GetType());
		CMotion::AttackInfo* AttackInfo = aInfo.AttackInfo[0];
		MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), *AttackInfo);

		// ��̈ʒu�܂Ő��`���
		bagpos = UtilFunc::Correction::EasingLinear(m_StartPos, weponpos, 0.0f, STATE_TIME::CATCH, m_fMoveTimer);
		pBaggage->SetPosition(bagpos);

		// ���V�[�u�G�t�F�N�g
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_HIT,
			weponpos, MyLib::Vector3(0.0f,D3DX_PI * 0.5f, 0.0f), 0.0f, 20.0f, true);
	}

	// �I���m�F
	if (((pMotion->GetType() == MOTION::MOTION_RETURN && pMotion->IsFinish()) ||
		pMotion->GetType() != MOTION::MOTION_RETURN) && !m_bEnd)
	{// �p�X�I�� or �p�X�ȊO
		m_bEnd = true;
		CCatchResult::Create(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CCatchResult::TYPE::TYPE_FAIL);
	}

	//// �����𒲐�
	//{
	//	m_fMoveTimer += CManager::GetInstance()->GetDeltaTime();
	//	MyLib::Vector3 rot = UtilFunc::Correction::EasingEaseIn(m_StartRot, START_ROT, 0.0f, 1.0f, m_fMoveTimer);
	//	SetRotation(rot);
	//}
}

//==========================================================================
// ��΂�
//==========================================================================
void CReceiverPeople::StateYabai()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	int type = pMotion->GetType();
	if (type != MOTION::MOTION_YABAI)
	{
		// ���[�V�����ݒ�
		pMotion->Set(MOTION::MOTION_YABAI);

		// �����_��������
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->GetCameraMotion()->SetPosition(GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f));
		pCamera->GetCameraMotion()->SetEnablePause(true);
		pCamera->SetPositionR(GetPosition());

		// �������ς���
		MyLib::Vector3 setrot = pCamera->GetRotation();
		setrot.z = D3DX_PI * -0.05f;
		pCamera->SetRotation(setrot);
	}

	// �_�C�u�ݒ�
	if (type == MOTION::MOTION_YABAI &&
		pMotion->IsFinish())
	{
		m_state = STATE::STATE_DIVE;
		m_fStateTime = 0.0f;
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_DIVE);


		// �ו��̌��݈ʒu���r����
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);

		// �����_���ו���
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->GetCameraMotion()->SetPosition(pBaggage->GetPosition());
		pCamera->SetPositionR(pBaggage->GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f));

		// �������ς���
		MyLib::Vector3 setrot = pCamera->GetRotation();
		setrot.z = D3DX_PI * -0.1f;
		pCamera->SetRotation(setrot);
	}
}

//==========================================================================
// �_�C�u
//==========================================================================
void CReceiverPeople::StateDive()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	if (pMotion->GetType() != MOTION::MOTION_DIVE)
	{
		// ���[�V�����ݒ�
		pMotion->Set(MOTION::MOTION_DIVE);

	}

	// �ړ����ԉ��Z
	m_fMoveTimer += CManager::GetInstance()->GetDeltaTime();

	// �ו��擾
	CBaggage* pBag = CBaggage::GetListObj().GetData(0);
	MyLib::Vector3 bagpos = pBag->GetPosition();
	bagpos.y = 0.0f;

	// �ʒu�ݒ�
	MyLib::Vector3 pos = UtilFunc::Correction::EasingLinear(m_StartPos, bagpos, 0.0f, STATE_TIME::DIVE, m_fMoveTimer);
	SetPosition(pos);

	// �����𒲐�
	MyLib::Vector3 rot = GetRotation();
	rot.y = m_StartPos.AngleXZ(bagpos);
	SetRotation(rot);


	// ��ԑJ��
	if (m_fMoveTimer >= STATE_TIME::DIVE)
	{
		// ��ԑJ��
		SetState(STATE::STATE_DROWN);
		m_fMoveTimer = 0.0f;

		// �����𒲐�
		SetRotation(START_ROT);

		// ���G�t�F�N�g
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_SPRAYWATER_MINI,
			pos, MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
	}

}

//==========================================================================
// �M���
//==========================================================================
void CReceiverPeople::StateDrown()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	if (pMotion->GetType() != MOTION::MOTION_DROWN)
	{
		// ���[�V�����ݒ�
		pMotion->Set(MOTION::MOTION_DROWN);
	}


	// �ړ����ԉ��Z
	m_fMoveTimer += CManager::GetInstance()->GetDeltaTime();

	if (m_fMoveTimer >= 2.0f &&
		(
		((pMotion->GetType() == MOTION::MOTION_DROWN && pMotion->IsFinish()) ||
		pMotion->GetType() != MOTION::MOTION_DROWN) && !m_bEnd
			))
	{// �p�X�I�� or �p�X�ȊO
		m_bEnd = true;
		CCatchResult::Create(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CCatchResult::TYPE::TYPE_FAIL);
	}

	// �ו��̈ʒu�ɂ��낦��
	CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);
	MyLib::Vector3 bagpos = pBaggage->GetPosition();
	bagpos.y = 0.0f;
	SetPosition(bagpos);
}

//==========================================================================
// �o�C�o�C
//==========================================================================
void CReceiverPeople::StateByeBye()
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

	if (((pMotion->GetType() == MOTION::MOTION_BYEBYE && pMotion->IsFinish()) ||
		pMotion->GetType() != MOTION::MOTION_BYEBYE) && !m_bEnd)
	{// �p�X�I�� or �p�X�ȊO
		m_bEnd = true;
		CCatchResult::Create(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CCatchResult::TYPE::TYPE_OK);
	}

	// �����𒲐�
	{
		m_fMoveTimer += LOOKBACK_TIMER;
		MyLib::Vector3 rot = UtilFunc::Correction::EasingEaseIn(m_StartRot, START_ROT, 0.0f, 1.0f, m_fMoveTimer);
		SetRotation(rot);
	}
}

//==========================================================================
// ���s
//==========================================================================
void CReceiverPeople::StateWalk()
{
	// �ړ����ԉ��Z
	m_fMoveTimer += CManager::GetInstance()->GetDeltaTime();

	// �ו��擾
	CBaggage* pbag = CBaggage::GetListObj().GetData(0);
	MyLib::Vector3 bagpos = pbag->GetPosition();
	MyLib::Vector3 pos = UtilFunc::Correction::EasingEaseIn(m_StartPos, bagpos, 0.0f, STATE_TIME::CHASE, m_fMoveTimer);
	SetPosition(pos);

	if (m_Distance == DISTANCE::DISTANCE_FAR && m_fMoveTimer >= STATE_TIME::CHASE)
	{
		SetState(STATE::STATE_RETURN);
		m_fMoveTimer = 0.0f;
		m_StartPos = bagpos;

		// �����_��������
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->GetCameraMotion()->SetPosition(CPlayer::GetListObj().GetData(0)->GetPosition());
		pCamera->GetCameraMotion()->SetEnablePause(true);
		pCamera->SetPositionR(GetPosition());
		pCamera->SetDistance(2500.0f);

		// �������ς���
		MyLib::Vector3 setrot = pCamera->GetRotation();
		setrot.z = D3DX_PI * 0.05f;
		pCamera->SetRotation(setrot);

		// �u�Ԉړ��G�t�F�N�g����
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_TP,
			pos, MyLib::Vector3(0.0f, D3DX_PI * 0.5f, 0.0f), 0.0f, 30.0f, true);

		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_TELEPORTATION);
		return;
	}

	// �����𒲐�
	if(m_Distance == DISTANCE::DISTANCE_NEAR)
	{
		MyLib::Vector3 targetrot = 0.0f;
		float movetimer = UtilFunc::Transformation::Clamp(m_fMoveTimer, 0.0f, 0.5f);
		targetrot.y = m_StartPos.AngleXZ(bagpos);
		
		MyLib::Vector3 rot = UtilFunc::Correction::EasingEaseIn(m_StartRot, targetrot, 0.0f, 0.5f, movetimer);
		SetRotation(rot);

		// ���[�V�����ݒ�
		SetMotion(MOTION::MOTION_WALK);
	}
	else
	{
		SetRotation(MyLib::Vector3(0.0f, D3DX_PI * 0.5f, 0.0f));

		// ���[�V�����ݒ�
		SetMotion(MOTION::MOTION_RETURN);
	}

	// �I���m�F
	if (m_fMoveTimer < 1.0f) { return; }

	m_fMoveTimer = 0.0f;
	m_StartRot = GetRotation();

	// ��Ԃ������ɂ���ĕύX
	switch (m_Distance)
	{
	case DISTANCE::DISTANCE_NEAR:
	{
		SetState(STATE::STATE_YABAI);

		// �ו��̌��݈ʒu���r����
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);

		// ��Ԑݒ�
		pBaggage->SetState(CBaggage::STATE::STATE_RECEIVE);
		pBaggage->SetState(CBaggage::STATE::STATE_FALL);
		pBaggage->SetIsFall(true);
		pBaggage->SetMove(0.0f);
	}
		break;

	case DISTANCE::DISTANCE_FAR:
	{
		if (m_StartPos.x >= bagpos.x)
		{// �ו����͂��Ă��Ȃ�
			SetState(STATE::STATE_DROWN);
		}
		else 
		{// �ʂ�߂���
			SetState(STATE::STATE_RETURN);
		}
	}
		break;

	default:
	{
		SetState(STATE::STATE_BYEBYE);
	}
		break;
	}
}

//==========================================================================
// ���[�V�����̐ݒ�
//==========================================================================
void CReceiverPeople::SetMotion(int motionIdx)
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
void CReceiverPeople::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
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
		}
		else if (nCntATK == 1)
		{
			pBaggage->SetState(CBaggage::STATE::STATE_PASS);
			pBaggage->SetAwayStartPosition(pMotion->GetAttackPosition(GetModel(), ATKInfo));
		}
	}
	break;

	case MOTION::MOTION_GET:
	{
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);
		pBaggage->SetAwayStartPosition(pMotion->GetAttackPosition(GetModel(), ATKInfo));
	}
	break;

	case MOTION::MOTION_RETURN:
	{
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);
		pBaggage->SetAwayStartPosition(pMotion->GetAttackPosition(GetModel(), ATKInfo));
		pBaggage->SetForce(0.0f);
		pBaggage->SetState(CBaggage::STATE::STATE_RETURN);
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->SetPositionRDest(pCamera->GetPositionR());
		pCamera->SetPositionVDest(pCamera->GetPositionV());
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_SMASHATTACK);
	}
	break;

	default:
		break;
	}
}

//==========================================================================
// �U�����蒆����
//==========================================================================
void CReceiverPeople::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
{
	// �S�[�����̂ݏ��ݒ�
	if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
	{
		return;
	}

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
	case MOTION::MOTION_BYEBYE:
	{
		// �΂��΂���Ԃ���
		if (m_state != STATE::STATE_BYEBYE) {
			return;
		}

		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);

		if (pBaggage->GetState() != CBaggage::STATE::STATE_PASS)
		{
			pBaggage->SetPosition(weponpos);
		}
	}
	break;

	case MOTION::MOTION_GET:
	{
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);

		if (pBaggage->GetState() != CBaggage::STATE::STATE_PASS)
		{
			pBaggage->SetPosition(weponpos);
		}
	}
	break;

	case MOTION::MOTION_RETURN:
	{
		
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
void CReceiverPeople::Draw()
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
void CReceiverPeople::SetState(STATE state)
{
	m_state = state;
}
