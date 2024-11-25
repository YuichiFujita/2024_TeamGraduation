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
#include "player.h"
#include "camera.h"
#include "MyEffekseer.h"
#include "fog.h"
#include "edit_map.h"
#include "blackframe.h"

// �J�ڐ�
#include "game.h"
#include "title.h"
#include "entry.h"
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
		"data\\TEXT\\map\\info.txt",	// �G���g���[
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
CScene* CScene::Create(CScene::MODE mode)
{
	// �����p�̃I�u�W�F�N�g
	CScene* pScene = nullptr;

	// ��������
	switch (mode)
	{
	case CScene::MODE_TITLE:
		pScene = CTitle::Create();
		break;

	case CScene::MODE::MODE_ENTRY:
		pScene = CEntry::Create();
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
void CScene::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#ifdef _DEBUG
	if (GET_INPUTKEY->GetPress(DIK_UP))
	{
		int* pFps = GetDebugFps();
		*pFps += 1;
		UtilFunc::Transformation::ValueNormalize(*pFps, 144, 1);
	}
	if (GET_INPUTKEY->GetPress(DIK_DOWN))
	{
		int* pFps = GetDebugFps();
		*pFps -= 1;
		UtilFunc::Transformation::ValueNormalize(*pFps, 144, 1);
	}

	float fRate = GET_MANAGER->GetSlowRate();
	ImGui::DragFloat("SlowRate", &fRate, 0.01f, 0.0f, 1.0f, "%.2f");
	GET_MANAGER->SetSlowRate(fRate);
#endif
}

//==========================================================================
// �`�揈��
//==========================================================================
void CScene::Draw()
{
	
}

