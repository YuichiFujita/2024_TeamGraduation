//=============================================================================
// 
// �Q�[���}�l�[�W������ [gamemanager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "gamemanager.h"
#include "game.h"
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "blackframe.h"
#include "map.h"
#include "calculation.h"
#include "fog.h"
#include "fade.h"
#include "input.h"
#include "ball.h"
#include "collisionLine_Box.h"
#include "teamStatus.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

namespace Court
{//�h�b�W�{�[���R�[�g���

	const D3DXVECTOR3 SIZE = D3DXVECTOR3(600.0f, 100.0f, 800.0f);		// �T�C�Y
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CGameManager::CGameManager()
{
	// �l�̃N���A
	m_SceneType = SCENE_MAIN;	// �V�[���̎��
	m_OldSceneType = SCENE_MAIN;	// �V�[���̎��(�ߋ�)
	m_bControll = false;		// ����ł��邩
	m_fSceneTimer = 0.0f;		// �V�[���^�C�}�[

	memset(&m_pTeamStatus[0], 0, sizeof(m_pTeamStatus));	// �`�[���X�e�[�^�X

	m_courtSize = MyLib::Vector3();
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CGameManager::~CGameManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CGameManager* CGameManager::Create(CScene::MODE mode)
{
	// �������m��
	CGameManager* pManager = nullptr;

	// �C���X�^���X����
	switch (mode)
	{
	case CScene::MODE_GAME:
		pManager = DEBUG_NEW CGameManager;
		break;

	}

	if (pManager != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		HRESULT hr = pManager->Init();

		if (FAILED(hr))
		{// ���s���Ă�����
			return nullptr;
		}
	}

	return pManager;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CGameManager::Init()
{
	m_bControll = true;	// ����ł��邩

	// �R�[�g�T�C�Y
	m_courtSize = Court::SIZE;

#if _DEBUG
	m_SceneType = SceneType::SCENE_MAIN;	// �V�[���̎�� 

	// �R�[�g�T�C�Y�̃{�b�N�X
	if (m_pCourtSizeBox == nullptr)
	{
		m_pCourtSizeBox = CCollisionLine_Box::Create(MyLib::AABB(-m_courtSize, m_courtSize), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	//�`�[���X�e�[�^�X
	CreateTeamStatus();

#else
	m_SceneType = SceneType::SCENE_START;	// �V�[���̎�� 
#endif

	m_OldSceneType = m_SceneType;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CGameManager::Uninit()
{
	// �`�[���X�e�[�^�X
	for (int i = 0; i < TeamType::TYPE_MAX; i++)
	{
		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->Uninit();
			m_pTeamStatus[i] = nullptr;
		}
	}

#if _DEBUG
	// �I��
	if (m_pCourtSizeBox != nullptr)
	{
		m_pCourtSizeBox = nullptr;
	}
#endif
}

//==========================================================================
// �X�V����
//==========================================================================
void CGameManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ������
	m_fSceneTimer += fDeltaTime * fSlowRate;	// �V�[���^�C�}�[
	switch (m_SceneType)
	{
	case CGameManager::SceneType::SCENE_MAIN:
		m_bControll = true;
		CheckJudgeZone();
		ContainPlayerBaggage();
		TurnAway();
		break;

	case CGameManager::SceneType::SCENE_START:
		m_bControll = false;
		SceneStart();
		break;

	case CGameManager::SceneType::SCENE_SKIP:
		m_bControll = false;
		SceneSkip();
		break;

	case CGameManager::SceneType::SCENE_COUNTDOWN:		// �J�E���g�_�E��
		TurnAway();
		break;

	case CGameManager::SceneType::SCENE_MAINCLEAR:
		m_bControll = false;
		SceneGoal();
		break;

	case SceneType::SCENE_MAINRESULT:
		m_bControll = false;
		break;

	case SceneType::SCENE_DURING_MAINRESULT:
		m_bControll = false;
		break;

	case SceneType::SCENE_BEFOREBATTLE:
		m_bControll = false;
		break;

	case SceneType::SCENE_BATTLESTART:
		m_bControll = false;
		break;

	case SceneType::SCENE_SKILLTREE:	// �X�L���c���[
		m_bControll = false;
		break;

	case CGameManager::SceneType::SCENE_BOSS:
		m_bControll = true;
		break;

	case CGameManager::SceneType::SCENE_TRANSITIONWAIT:
		m_bControll = false;
		break;

	case CGameManager::SceneType::SCENE_TRANSITION:
		m_bControll = false;
		SceneTransition();
		break;

	case SceneType::SCENE_RESULT:
		m_bControll = false;
		break;

	case SceneType::SCENE_WAIT_AIRPUSH:
		m_bControll = false;
		SceneWaitAirPush();
		TurnAway();
		break;

	case SceneType::SCENE_GOAL:			// �S�[��
		SceneGoal();
		break;

	case SceneType::SCENE_DEBUG:
		m_bControll = true;
		break;

	default:
		break;
	}

#if _DEBUG	// �f�o�b�O����

	if (ImGui::TreeNode("GameManager"))
	{
		Debug();
		ImGui::TreePop();
	}
#endif
}

//==========================================================================
// �X�^�[�g���̐ݒ�
//==========================================================================
void CGameManager::StartSetting()
{
	
}

//==========================================================================
// �Q�[���N���A���̐ݒ�
//==========================================================================
void CGameManager::GameClearSettings()
{

}

//==========================================================================
// �X�L�b�v
//==========================================================================
void CGameManager::SceneSkip()
{
	
}

//==========================================================================
// �J�n���o
//==========================================================================
void CGameManager::SceneStart()
{
	
}

//==========================================================================
// �Q�[�����U���g�̐ݒ�
//==========================================================================
void CGameManager::GameResultSettings()
{

}

//==========================================================================
// �{�[���̎擾
//==========================================================================
CBall* CGameManager::GetBall()
{
	CListManager<CBall> list = CBall::GetListObj();	// �{�[�����X�g
	assert(list.GetNumAll() == 1);	// �����{�[��������ꍇ�G���[
	return *list.GetBegin();		// �{�[������Ԃ�
}

//==========================================================================
// ���C���J�ڒ�
//==========================================================================
void CGameManager::SceneTransition()
{
	
}

//==========================================================================
// ��C����҂�
//==========================================================================
void CGameManager::SceneWaitAirPush()
{
	
}

//==========================================================================
// �S�[�����
//==========================================================================
void CGameManager::SceneGoal()
{

}

//==========================================================================
// �v���C���[�Ɖו�����ʓ��Ɏ��߂�
//==========================================================================
void CGameManager::ContainPlayerBaggage()
{
	
}

//==========================================================================
// �J��������ɉ��������悤�ɂ���
//==========================================================================
void CGameManager::TurnAway()
{
	
}

//==========================================================================
// ����]�[���m�F
//==========================================================================
void CGameManager::CheckJudgeZone()
{
	

}

//==========================================================================
// �{�X�ݒ�
//==========================================================================
void CGameManager::SetBoss()
{

}

//==========================================================================
// �G�ݒ�
//==========================================================================
void CGameManager::SetEnemy()
{

	

}

//==========================================================================
// �V�[���̎�ސݒ�
//==========================================================================
void CGameManager::SetType(SceneType type)
{
	m_OldSceneType = m_SceneType;
	m_SceneType = type;
}

//==========================================================================
// �V�[���̎�ގ擾
//==========================================================================
CGameManager::SceneType CGameManager::GetType()
{
	return m_SceneType;
}

//==========================================================================
// �R�[�g�ړ�����
//==========================================================================
void CGameManager::PosLimit(MyLib::Vector3& pos)
{
	if (pos.x > m_courtSize.x)
	{
		pos.x = m_courtSize.x;
	}
	else if (pos.x < -m_courtSize.x)
	{
		pos.x = -m_courtSize.x;
	}

	if (pos.z > m_courtSize.z)
	{
		pos.z = m_courtSize.z;
	}
	else if (pos.z < -m_courtSize.z)
	{
		pos.z = -m_courtSize.z;
	}
}

//==========================================================================
// �`�[���X�e�[�^�X����
//==========================================================================
void CGameManager::CreateTeamStatus()
{
	CGameManager::TeamSide side = CGameManager::SIDE_NONE;

	for (int i = 0; i < TYPE_MAX; i++)
	{
		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->Uninit();
			m_pTeamStatus[i] = nullptr;
		}

		m_pTeamStatus[i] = CTeamStatus::Create();

		switch (static_cast<CGameManager::TeamType>(i))
		{
		case TYPE_LEFT:
			side = CGameManager::SIDE_LEFT;
			break;

		case TYPE_RIGHT:
			side = CGameManager::SIDE_RIGHT;
			break;

		default:
			break;
		}
		
		m_pTeamStatus[i]->TeamSetting(side);

	}
}

//==========================================================================
// �f�o�b�O
//==========================================================================
void CGameManager::Debug()
{

	//-----------------------------
	// �R�[�g
	//-----------------------------
	if (ImGui::TreeNode("Court"))
	{
		if (m_pCourtSizeBox == nullptr)
		{
			m_pCourtSizeBox = CCollisionLine_Box::Create(MyLib::AABB(-m_courtSize, m_courtSize), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		if (ImGui::Button("Reset"))
		{// ���Z�b�g
			m_courtSize = Court::SIZE;
		}
		ImGui::SameLine();

		ImGui::DragFloat3("size", (float*)&m_courtSize, 10.0f, 0.0f, 10000.0f, "%.2f");

		// �T�C�Y�ݒ�
		if (m_pCourtSizeBox != nullptr)
		{
			m_pCourtSizeBox->SetAABB(MyLib::AABB(-m_courtSize, m_courtSize));
		}

		// �ʒu�ݒ�
		ImGui::TreePop();
	}

	// �`�[���X�e�[�^�X
	for (int i = 0; i < TeamType::TYPE_MAX; i++)
	{
		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->Debug();
		}
	}
}
