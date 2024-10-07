//=============================================================================
// 
//  �V�[������ [scene.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "scene.h"
#include "renderer.h"
#include "calculation.h"
#include "Xload.h"
#include "map.h"
#include "fade.h"
#include "elevation.h"
#include "player.h"
#include "camera.h"
#include "MyEffekseer.h"
#include "fog.h"
#include "edit_map.h"
#include "blackframe.h"

// �J�ڐ�
#include "game.h"
#include "title.h"
#include "result.h"
#include "tutorial.h"
#include "ranking.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	
	const char* MAP_TEXT[] =
	{
		"data\\TEXT\\map\\info.txt",	// �Ȃ�
		"data\\TEXT\\map\\info.txt",	// �^�C�g��
		"data\\TEXT\\map\\info.txt",	// �`���[�g���A��
		"data\\TEXT\\map\\info.txt",	// �Q�[��
		"data\\TEXT\\map\\info.txt",	// �Q�[��
		"data\\TEXT\\map\\map_result.txt",	// ���U���g
		"data\\TEXT\\map\\info.txt",	// �����L���O
	};
	
	const char* ELEVATION_TEXT = "data\\TEXT\\elevation\\field_ingame.txt";
}

#if 1
#define LOADMAP = 0;
#endif

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CElevation *CScene::m_pObject3DMesh = nullptr;		// �I�u�W�F�N�g3D���b�V���̃I�u�W�F�N�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CScene::CScene()
{
	// �ϐ��̃N���A
	m_mode = MODE_TITLE;
	m_pEditMap = nullptr;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CScene::~CScene()
{

}

//==========================================================================
// ��������
//==========================================================================
CScene *CScene::Create(CScene::MODE mode)
{
	// �����p�̃I�u�W�F�N�g
	CScene *pScene = nullptr;

	if (pScene == nullptr)
	{// nullptr��������

		// ��������
		switch (mode)
		{
		case CScene::MODE_TITLE:
			pScene = CTitle::Create();
			break;

		case CScene::MODE_GAME:
		case CScene::MODE::MODE_GAMETUTORIAL:
			pScene = CGame::Create(mode);
			break;

		case MODE_TUTORIAL:
			pScene = CTutorial::Create();
			break;

		case CScene::MODE_RESULT:
			pScene = DEBUG_NEW CResult;
			break;

		case CScene::MODE_RANKING:
			pScene = DEBUG_NEW CRanking;
			break;
		}

		if (pScene != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// ���[�h�̐ݒ�
			pScene->m_mode = mode;
		}

		return pScene;
	}

	return nullptr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CScene::Init()
{

	// �G�t�F�N�g�S�Ē�~
	CMyEffekseer::GetInstance()->StopAll();

	// �t�H�O���Z�b�g
	MyFog::ToggleFogFrag(false);


	//**********************************
	// ���t���[��
	//**********************************
	CBlackFrame::Create();

	//**********************************
	// �}�b�v�̐���
	//**********************************
#ifdef LOADMAP
	if (FAILED(MyMap::Create(MAP_TEXT[m_mode])))
	{// ���s�����ꍇ
		return E_FAIL;
	}
#endif

	//**********************************
	// �}�b�v�̐���
	//**********************************
#ifdef LOADMAP
	m_pEditMap = CEdit_Map_Release::Create(MAP_TEXT[m_mode], CManager::BuildMode::MODE_RELEASE);
#endif

	//**********************************
	// �N���̒n��
	//**********************************
	m_pObject3DMesh = CElevation::Create(ELEVATION_TEXT);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CScene::Uninit()
{
	//**********************************
	// �j���t�F�[�Y
	//**********************************
	// �G�f�B�b�g�}�b�v
	if (m_pEditMap != nullptr)
	{
		m_pEditMap->Uninit();
		m_pEditMap = nullptr;
	}

	// �}�b�v
	//MyMap::Release();

	if (CBlackFrame::GetInstance() != nullptr)
	{
		CBlackFrame::GetInstance()->Uninit();
	}
}

//==========================================================================
// �폜����
//==========================================================================
void CScene::Kill()
{
	// �}�b�v
	MyMap::Release();
}


//==========================================================================
// �X�V����
//==========================================================================
void CScene::Update()
{
	
}

//==========================================================================
// �`�揈��
//==========================================================================
void CScene::Draw()
{
	
}

//==========================================================================
// �V�[���̃��Z�b�g
//==========================================================================
void CScene::ResetScene()
{
	//**********************************
	// �j���t�F�[�Y
	//**********************************
	// �N���̒n��
	if (m_pObject3DMesh != nullptr)
	{
		m_pObject3DMesh->Uninit();
		m_pObject3DMesh = nullptr;
	}

	// �G�f�B�b�g�}�b�v
	if (m_pEditMap != nullptr)
	{
		m_pEditMap->Uninit();
		delete m_pEditMap;
		m_pEditMap = nullptr;
	}

	if (CBlackFrame::GetInstance() != nullptr)
	{
		CBlackFrame::GetInstance()->Uninit();
	}

	// �}�b�v
	MyMap::Release();

	//**********************************
	// �����t�F�[�Y
	//**********************************
	// �}�b�v
	if (FAILED(MyMap::Create(MAP_TEXT[m_mode])))
	{// ���s�����ꍇ
		return;
	}
#ifdef LOADMAP
	m_pEditMap = CEdit_Map_Release::Create(MAP_TEXT[m_mode], CManager::BuildMode::MODE_RELEASE);
#endif

	// �{�X�X�e�[�W�̋N������
	m_pObject3DMesh = CElevation::Create(ELEVATION_TEXT);
}

//==========================================================================
// ���݂̃��[�h�擾
//==========================================================================
CScene::MODE CScene::GetMode()
{
	return m_mode;
}

//==========================================================================
// 3D�I�u�W�F�N�g�̎擾
//==========================================================================
CElevation *CScene::GetElevation()
{
	return m_pObject3DMesh;
}
