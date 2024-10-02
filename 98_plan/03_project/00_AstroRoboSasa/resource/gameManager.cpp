//============================================================
//
//	�Q�[���}�l�[�W���[���� [gameManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gameManager.h"
#include "manager.h"
#include "fade.h"
#include "scene.h"
#include "sceneGame.h"
#include "cinemaScope.h"
#include "timerUI.h"
#include "retentionManager.h"

#include "player.h"
#include "enemy.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int GAMEEND_WAIT_FRAME = 180;	// ���U���g��ʂւ̑J�ڗ]�C�t���[��
}

//************************************************************
//	�e�N���X [CGameManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGameManager::CGameManager() :
	m_state	(STATE_NONE)	// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGameManager::~CGameManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGameManager::Init(void)
{
	// �����o�ϐ���������
	m_state = STATE_NORMAL;	// ���

	// �v���C���[�̐���
	CPlayer::Create();

	// �G�̐���
	CEnemy::Create(D3DXVECTOR3(-100.0f, 500.0f, 0.0f));
	CEnemy::Create(D3DXVECTOR3(-200.0f, 500.0f, 0.0f));
	CEnemy::Create(D3DXVECTOR3(-300.0f, 500.0f, 0.0f));

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGameManager::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CGameManager::Update(const float fDeltaTime)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:
	case STATE_NORMAL:
		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	��Ԑݒ菈��
//============================================================
void CGameManager::SetState(const EState state)
{
	// ��Ԃ�ݒ�
	m_state = state;
}

//============================================================
//	��Ԏ擾����
//============================================================
CGameManager::EState CGameManager::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	���U���g��ʑJ�ڏ���
//============================================================
void CGameManager::TransitionResult(const CRetentionManager::EWin win)
{
	// �t�F�[�h���̏ꍇ������
	if (GET_MANAGER->GetFade()->GetState() != CFade::FADE_NONE) { return; }

	// ���U���g����ۑ�
	GET_RETENTION->SetResult(win, 0.0f);

	// ���U���g��ʂɑJ��
	GET_MANAGER->SetFadeScene(CScene::MODE_RESULT, GAMEEND_WAIT_FRAME);
}

//============================================================
//	��������
//============================================================
CGameManager *CGameManager::Create(void)
{
	// �Q�[���}�l�[�W���[�̐���
	CGameManager *pGameManager = new CGameManager;
	if (pGameManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �Q�[���}�l�[�W���[�̏�����
		if (FAILED(pGameManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �Q�[���}�l�[�W���[�̔j��
			SAFE_DELETE(pGameManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pGameManager;
	}
}

//============================================================
//	�j������
//============================================================
void CGameManager::Release(CGameManager *&prGameManager)
{
	// �Q�[���}�l�[�W���[�̏I��
	assert(prGameManager != nullptr);
	prGameManager->Uninit();

	// �������J��
	SAFE_DELETE(prGameManager);
}

//============================================================
//	��ʊO�Ƃ̓����蔻��
//============================================================
bool CGameManager::CollisionScreen(D3DXVECTOR3* pPos, D3DXVECTOR3* pMove)
{
	if (pPos->y > 690.0f)
	{
		pPos->y = 690.0f;
		if (pMove != nullptr)
		{
			pMove->y = -20.0f;
		}
		return true;
	}

	if (pPos->x < -625.0f)
	{
		pPos->x = -625.0f;
		if (pMove != nullptr)
		{
			pMove->x = 20.0f;
		}
		return true;
	}

	if (pPos->x > 625.0f)
	{
		pPos->x = 625.0f;
		if (pMove != nullptr)
		{
			pMove->x = -20.0f;
		}
		return true;
	}

	return false;
}
