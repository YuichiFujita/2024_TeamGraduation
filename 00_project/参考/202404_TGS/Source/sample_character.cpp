//=============================================================================
// 
//  �T���v��_�L�����N�^�[���� [sample_character.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "sample_character.h"
#include "camera.h"
#include "manager.h"
#include "debugproc.h"
#include "calculation.h"
#include "shadow.h"
#include "game.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string CHARAFILE = "data\\TEXT\\character\\player\\tyuuni\\setup_player.txt";	// �L�����N�^�[�t�@�C��
	
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CSample_Character::STATE_FUNC CSample_Character::m_StateFunc[] =
{
	&CSample_Character::StateNone,		// �Ȃ�
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CSample_Character> CSample_Character::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CSample_Character::CSample_Character(int nPriority) : CObjectChara(nPriority)
{
	// �l�̃N���A
	m_state = STATE_NONE;	// ���
	m_mMatcol = mylib_const::DEFAULT_COLOR;		// �}�e���A���̐F
	
	// �|�C���^
	m_pShadow = nullptr;	// �e�̃|�C���^
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CSample_Character::~CSample_Character()
{

}

//==========================================================================
// ��������
//==========================================================================
CSample_Character* CSample_Character::Create()
{
	// �������m��
	CSample_Character* pObj = DEBUG_NEW CSample_Character;

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
HRESULT CSample_Character::Init()
{
	// ��ނ̐ݒ�
	SetType(TYPE_PLAYER);

	m_state = STATE_NONE;	// ���

	// �L�����쐬
	HRESULT hr = SetCharacter(CHARAFILE);
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// ���蓖��
	m_List.Regist(this);

	
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CSample_Character::Uninit()
{
	// �e������
	if (m_pShadow != nullptr)
	{
		m_pShadow = nullptr;
	}

	// �I������
	CObjectChara::Uninit();

	// �폜
	m_List.Delete(this);
}

//==========================================================================
// �I������
//==========================================================================
void CSample_Character::Kill()
{
	// �e������
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// �I������
	CObjectChara::Uninit();

	// �폜
	m_List.Delete(this);
}

//==========================================================================
// �X�V����
//==========================================================================
void CSample_Character::Update()
{
	
	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	if (pInputKeyboard->GetTrigger(DIK_F5))
	{// F5�Ń��Z�b�g
		SetPosition(MyLib::Vector3(0.0f, 0.0f, -1000.0f));
		SetMove(MyLib::Vector3(0.0f, 0.0f, 0.0f));
	}

	// �G�f�B�b�g���͔�����
	if (CGame::GetInstance()->GetEditType() != CGame::GetInstance()->EDITTYPE_OFF)
	{
		return;
	}

	// �ߋ��̈ʒu�ۑ�
	SetOldPosition(GetPosition());

	// �e�̍X�V����
	CObjectChara::Update();

	// ��ԍX�V
	UpdateState();

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// �e�̈ʒu�X�V
	if (m_pShadow != nullptr)
	{
		m_pShadow->SetPosition(MyLib::Vector3(pos.x, m_pShadow->GetPosition().y, pos.z));
	}

}

//==========================================================================
// �U��������
//==========================================================================
void CSample_Character::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	int nType = pMotion->GetType();

	// ����̈ʒu
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), ATKInfo);
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	switch (nType)
	{
	case MOTION::MOTION_DEFAULT:
		// �U�����̏���������
		break;

	default:
		break;
	}
}

//==========================================================================
// �U�����蒆����
//==========================================================================
void CSample_Character::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
{
	if (pATKInfo->fRangeSize == 0.0f) return;

	if (pATKInfo->bEndAtk) return;

	// ���[�V�����擾
	CMotion* pMotion = GetMotion();

	// ����̈ʒu
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), *pATKInfo);

	CEffect3D* pEffect = nullptr;
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CSample_Character::UpdateState()
{
	// ���̏�Ԑݒ�

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CSample_Character::StateNone()
{
	// �F�ݒ�
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CSample_Character::Draw()
{

	// �`�揈��
	// ��Ԃɕ����Ďg��������
	/*
	CObjectChara::Draw(m_mMatcol);

	CObjectChara::Draw(m_mMatcol.a);

	CObjectChara::Draw();
	*/
}
