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

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int POINT_WAVECLEAR = 5;		// �E�F�[�u�N���A�̃|�C���g
	const float POSR_Y_APPROACH_COEF = 0.3f;	//�J�������߂Â��ۂ̊���
	const float POSR_Y_PULL_SCREEN_POS = 210.0f;	// �J�����������n�߂�X�N���[�����W
	const float POSR_Y_APPROACH_SCREEN_POS = SCREEN_HEIGHT * 0.5f;	// �J�������߂Â��n�߂�X�N���[�����W
	const float POSR_YDEST_BAGGTOPLAYER_RATIO = 0.4f;	// �ו��ƃv���C���[�����̊����iposRYDest�j
	const int GUIDE_NUM = 100;

	const int CHANGE_BASEPOINT[] =	// �|�C���g�ύX����
	{
		40,	// AAA
		20,	// BBB
		10,	// CCC
		1,	// DDD
	};
	const float DEFAULT_INTERVAL_AIRSPAWN = 2.3f;	// �f�t�H�̋�C�����Ԋu
	const float DEFAULT_INTERVAL_FLOWLEAFSPAWN = 0.8f;	// �f�t�H�̗����t�����Ԋu
	const float DEFAULT_INTERVAL_LEAFSPAWN = 2.0f;		// �f�t�H�̍~��t�����Ԋu
}

namespace SceneTime
{
	const float RequestStart = 3.5f;	// �˗��J�n
}

namespace Court
{//�h�b�W�{�[���R�[�g���

	const D3DXVECTOR3 SIZE = D3DXVECTOR3(600.0f, 0.0f, 800.0f);		// �T�C�Y
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CGameManager::CGameManager()
{
	// �l�̃N���A
	m_SceneType = SCENE_MAIN;	// �V�[���̎��
	m_OldSceneType = SCENE_MAIN;	// �V�[���̎��
	m_bControll = false;		// ����ł��邩
	m_fSceneTimer = 0.0f;		// �V�[���^�C�}�[

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
	m_bControll = true;			// ����ł��邩

#if _DEBUG
	m_SceneType = SceneType::SCENE_MAIN;	// �V�[���̎�� 
#else
	m_SceneType = SceneType::SCENE_START;	// �V�[���̎�� 
#endif

	m_OldSceneType = m_SceneType;

	m_courtSize = Court::SIZE;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CGameManager::Uninit()
{
	
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
