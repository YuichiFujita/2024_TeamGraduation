//=============================================================================
// 
//  ���S�v���C���[���� [deadplayer.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "deadplayer.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"
#include "limitarea.h"
#include "debugproc.h"


//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float MOVE_VELOCITY = 15.0f;	// �ړ����x
	const int TURN_RIGHT = 100;	// ���E�Ԋu
	const std::string CHARAFILE = "data\\TEXT\\character\\player\\tyuuni\\setup_raou.txt";	// �L�����N�^�[�t�@�C��
}

namespace STATE_TIME
{
	const float UP = 5.0f;	// �㏸����
	const float FADEOUT = 1.5f;	// ���S����
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
// ��Ԋ֐�
CDeadPlayer::STATE_FUNC CDeadPlayer::m_StateFunc[] =
{
	&CDeadPlayer::StateNone,	// �Ȃ�
	&CDeadPlayer::StateUP,		// �㏸
	&CDeadPlayer::StateFadeOut,	// �t�F�[�h�A�E�g
};

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CListManager<CDeadPlayer> CDeadPlayer::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CDeadPlayer::CDeadPlayer(int nPriority) : CObjectChara(nPriority)
{
	// �l�̃N���A
	m_state = STATE::STATE_NONE;	// ���
	m_Oldstate = m_state;	// �O��̏��
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �}�e���A���̐F

	m_fStateTime = 0.0f;		// ��ԑJ�ڃJ�E���^�[
	m_pShadow = nullptr;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CDeadPlayer::~CDeadPlayer()
{

}

//==========================================================================
// ��������
//==========================================================================
CDeadPlayer* CDeadPlayer::Create(const MyLib::Vector3& pos)
{
	// �������̊m��
	CDeadPlayer* pPeople = DEBUG_NEW CDeadPlayer;

	if (pPeople != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �ʒu�ݒ�
		pPeople->SetPosition(pos);
		pPeople->CObject::SetOriginPosition(pos);

		// ����������
		pPeople->Init();
	}

	return pPeople;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CDeadPlayer::Init()
{

	// �e�L�X�g�ǂݍ���
	HRESULT hr = LoadText(CHARAFILE);
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// �e��ϐ��̏�����
	m_state = STATE::STATE_UP;	// ���
	m_Oldstate = m_state;
	m_fStateTime = 0.0f;			// ��ԑJ�ڃJ�E���^�[

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_PLAYER);

	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// ���V�ݒ�
	CMotion* pMotion = GetMotion();
	if (pMotion != nullptr)
	{
		pMotion->Set(MOTION::MOTION_DEAD);
	}

	// ���V�G�t�F�N�g
	m_pEfkEffect = CEffekseerObj::Create(
		CMyEffekseer::EFKLABEL::EFKLABEL_PHOTON,
		GetPosition(), 0.0f, 0.0f, 100.0f, false);

	return S_OK;
}

//==========================================================================
// �e�L�X�g�ǂݍ���
//==========================================================================
HRESULT CDeadPlayer::LoadText(const std::string& filename)
{
	// �L�����쐬
	HRESULT hr = SetCharacter(filename);
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CDeadPlayer::Uninit()
{
	
	// �e������
	if (m_pShadow != nullptr)
	{
		m_pShadow = nullptr;
	}

	// �G�t�F�N�g�폜
	if (m_pEfkEffect != nullptr)
	{
		m_pEfkEffect->Uninit();
		m_pEfkEffect = nullptr;
	}

	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectChara::Uninit();
}

//==========================================================================
// ���S����
//==========================================================================
void CDeadPlayer::Kill()
{
	
	// �e������
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// �G�t�F�N�g�폜
	if (m_pEfkEffect != nullptr)
	{
		m_pEfkEffect->Uninit();
		m_pEfkEffect = nullptr;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CDeadPlayer::Update()
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
// ��ԍX�V����
//==========================================================================
void CDeadPlayer::UpdateState()
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
void CDeadPlayer::StateNone()
{
	// �F�ݒ�
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fStateTime = 0.0f;
}

//==========================================================================
// �㏸
//==========================================================================
void CDeadPlayer::StateUP()
{
	// �F�ݒ�
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);

	if (m_fStateTime >= STATE_TIME::UP)
	{
		m_state = STATE::STATE_FADEOUT;
		m_fStateTime = 0.0f;
		return;
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CDeadPlayer::StateFadeOut()
{
	// �F�ݒ�
	m_mMatcol.a = (1.0f - m_fStateTime / STATE_TIME::FADEOUT) * 0.6f;

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
void CDeadPlayer::LimitArea()
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
void CDeadPlayer::SetMotion(int motionIdx)
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
void CDeadPlayer::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	return;
}

//==========================================================================
// �U�����蒆����
//==========================================================================
void CDeadPlayer::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
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
void CDeadPlayer::Draw()
{
	// �I�u�W�F�N�g�L�����̕`��
	CObjectChara::Draw(m_mMatcol);
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CDeadPlayer::SetState(STATE state)
{
	m_state = state;
}
