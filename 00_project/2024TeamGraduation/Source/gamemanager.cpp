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
#include "audience.h"
#include "gymWallManager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	//�h�b�W�{�[���R�[�g���
	namespace Court
	{
		const D3DXVECTOR3 SIZE = D3DXVECTOR3(950.0f, 100.0f, 560.0f);	// �T�C�Y
	}
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

	m_pGymWallManager = nullptr;	// �W���ǃ}�l�W���[

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

	// �W���ǃ}�l�[�W������
	m_pGymWallManager = CGymWallManager::Create();

#if _DEBUG
	m_SceneType = ESceneType::SCENE_MAIN;	// �V�[���̎�� 

	// �R�[�g�T�C�Y�̃{�b�N�X
	if (m_pCourtSizeBox == nullptr)
	{
		m_pCourtSizeBox = CCollisionLine_Box::Create(MyLib::AABB(-m_courtSize, m_courtSize), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

#else
	m_SceneType = ESceneType::SCENE_START;	// �V�[���̎�� 
#endif

	//�`�[���X�e�[�^�X
	CreateTeamStatus();

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
	// TODO : UpdateScene�I�Ȃ̂ł܂Ƃ߂���
	m_fSceneTimer += fDeltaTime * fSlowRate;	// �V�[���^�C�}�[
	
	// TODO : �֐����X�g�ɂ�����
	switch (m_SceneType)
	{
	case CGameManager::ESceneType::SCENE_MAIN:
		m_bControll = true;
		UpdateAudience();
		break;

	case CGameManager::ESceneType::SCENE_START:
		m_bControll = false;
		SceneStart();
		break;

	case CGameManager::ESceneType::SCENE_BEFOREBATTLE:
		m_bControll = false;
		break;

	case CGameManager::ESceneType::SCENE_BATTLESTART:
		m_bControll = false;
		break;

	case CGameManager::ESceneType::SCENE_SPECIAL_STAG:
		m_bControll = false;
		UpdateSpecialStag();
		break;

	case ESceneType::SCENE_DEBUG:
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
// �J�n���o
//==========================================================================
void CGameManager::SceneStart()
{
	m_SceneType = ESceneType::SCENE_MAIN;	// �V�[���̎�� 

#if _DEBUG
	// �R�[�g�T�C�Y�̃{�b�N�X
	if (m_pCourtSizeBox == nullptr)
	{
		m_pCourtSizeBox = CCollisionLine_Box::Create(MyLib::AABB(-m_courtSize, m_courtSize), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
#endif
	//�`�[���X�e�[�^�X
	CreateTeamStatus();
}

//==========================================================================
// �ϋq�X�V
//==========================================================================
void CGameManager::UpdateAudience()
{
	GET_MANAGER->GetDebugProc()->Print("\n----------------- �ϋq��� -----------------\n");
	for (int i = 0; i < 2; i++)
	{
		CTeamStatus::SCharmInfo info = m_pTeamStatus[i]->GetCharmInfo();	// ���e���
		float fMoteRate = info.fValue / info.fValueMax;				// ���e����
		int nNumAudience = (int)(CAudience::MAX_WATCH * fMoteRate);	// ���݂̊ϋq��

		// �ϋq����ݒ�
		CAudience::SetNumWatch(nNumAudience, (CGameManager::TeamSide)(i + 1));
		GET_MANAGER->GetDebugProc()->Print("�y�`�[��0%d�ϋq�z[%d]\n", i, nNumAudience);
	}
}

//==========================================================================
// �X�y�V�������o�X�V
//==========================================================================
void CGameManager::UpdateSpecialStag()
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
// �V�[���̎�ސݒ�
//==========================================================================
void CGameManager::SetType(ESceneType type)
{
	m_OldSceneType = m_SceneType;
	m_SceneType = type;
}

//==========================================================================
// �R�[�g�ړ�����
//==========================================================================
bool CGameManager::SetPosLimit(MyLib::Vector3& pos)
{
	bool bHit = false;

	if (pos.x > m_courtSize.x)
	{
		pos.x = m_courtSize.x;
		bHit = true;
	}
	else if (pos.x < -m_courtSize.x)
	{
		pos.x = -m_courtSize.x;
		bHit = true;
	}

	if (pos.z > m_courtSize.z)
	{
		pos.z = m_courtSize.z;
		bHit = true;
	}
	else if (pos.z < -m_courtSize.z)
	{
		pos.z = -m_courtSize.z;
		bHit = true;
	}

	return bHit;
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

		switch (static_cast<CGameManager::TeamType>(i + 1))
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
#if _DEBUG

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
		//-----------------------------
		// �R�[�g
		//-----------------------------
		std::string treename = "Special" + std::to_string(i);	// �c���[��
		if (ImGui::TreeNode(treename.c_str()))
		{
			float fValue = m_pTeamStatus[i]->GetSpecialValue();

			ImGui::DragFloat("GaugeValue", (float*)&fValue, 1.0f, 0.0f, m_pTeamStatus[i]->GetSpecialInfo().fValueMax, "%.2f");

			// �ʒu�ݒ�
			ImGui::TreePop();

			m_pTeamStatus[i]->SetSpecialValue(fValue);
		}

		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->Debug();
		}
	}

#endif
}
