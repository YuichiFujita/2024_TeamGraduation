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
#include "result.h"
#include "tutorial.h"
#include "ranking.h"

// TODO
#include "manager.h"
#include "debugproc.h"
#include "char2D.h"
#include "string2D.h"
#include "text2D.h"
#include "timer.h"
#include "timerUI.h"
#include "timeUI.h"
#include "3D_effect.h"
#include "2D_effect.h"

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
	// TODO
#if 0
	CChar2D::Create("data\\FONT\\��S�V�b�N.otf", false, L'��', SCREEN_CENT + D3DXVECTOR3(100.0f, 0.0f, 0.0f) * 1.0f);
	CChar2D::Create("data\\FONT\\��S�V�b�N.otf", false, L'a',  SCREEN_CENT + D3DXVECTOR3(100.0f, 0.0f, 0.0f) * 2.0f);
	CChar2D::Create("data\\FONT\\��S�V�b�N.otf", false, L'0',  SCREEN_CENT + D3DXVECTOR3(100.0f, 0.0f, 0.0f) * 0.0f);
#endif

	// TODO
#if 0
	CString2D::Create("data\\FONT\\��S�V�b�N.otf", false, L"abcdefghijklmnopqrstuvwxyz", SCREEN_CENT, 60.0f, CString2D::XALIGN_RIGHT);
#endif

	// TODO
#if 0
	CText2D* p = CText2D::Create("data\\FONT\\��S�V�b�N.otf", false, SCREEN_CENT, 60.0f, 140.0f, CString2D::XALIGN_LEFT, CText2D::YALIGN_TOP);
	p->AddString(L"abcdefghijklmnopqrstuvwxyz");
	p->AddString(L"������������������������������");
	p->AddString(L"����/�{�{�{�[�{�E�{�[�{�{");
#endif

	// TODO
#if 0
	CTimer::Create(0.0f, 120.0f);
	CTimer::Create(120.0f, 120.0f);
#endif

	// TODO
#if 0
	CTimerUI* p = CTimerUI::Create(145.0f, 145.0f, SCREEN_CENT, D3DXVECTOR2(60.0f, 60.0f), D3DXVECTOR2(60.0f, 60.0f), D3DXVECTOR2(60.0f, 0.0f), D3DXVECTOR2(60.0f, 0.0f), CTimeUI::XALIGN_LEFT);
	p->Start();	// TODO�F�^�C�}�[�j���ł��Ȃ��������̕\������
#endif

	// TODO
#if 0
	CTimeUI::Create(145.0f, SCREEN_CENT, D3DXVECTOR2(60.0f, 60.0f), D3DXVECTOR2(60.0f, 60.0f), D3DXVECTOR2(60.0f, 0.0f), D3DXVECTOR2(60.0f, 0.0f));
#endif

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
void CScene::Update(const float fDeltaTime)
{
	if (GET_INPUTKEY->GetTrigger(DIK_0))
	{
		// �G�t�F�N�g2D
#if 0
		CEffect2D* p = CEffect2D::Create(MyLib::Vector3(0.0f, 200.0f, 0.0f), MyLib::Vector3(250.0f, 0.0f, 0.0f), MyLib::color::White(), 120.0f, 1.0f, 0, CEffect2D::TYPE::TYPE_BLACK);
#endif

		// �G�t�F�N�V�A
#if 0
		CEffekseerObj* p = CEffekseerObj::Create(CMyEffekseer::EFKLABEL::EFKLABEL_HIT, VEC3_ZERO, VEC3_ZERO, VEC3_ZERO, 50.0f);
#endif
	}

	if (GET_INPUTKEY->GetTrigger(DIK_9))
	{
		// TODO�F�G�t�F�N�g3D
#if 0
		CEffect3D* p = CEffect3D::Create(VEC3_ZERO, MyLib::Vector3(0.0f, 600.0f, 0.0f), MyLib::color::White(), 120.0f, 10.0f, 0, CEffect3D::TYPE::TYPE_BLACK, 0.1f);
		p->SetGravityValue(10.0f);
#endif

		// TODO�F
#if 0

#endif
	}

	int* pFps = GetDebugFps();
	if (GET_INPUTKEY->GetPress(DIK_UP))
	{
		*pFps += 1;
	}
	if (GET_INPUTKEY->GetPress(DIK_DOWN))
	{
		*pFps -= 1;
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CScene::Draw()
{
	
}

//==========================================================================
// ���݂̃��[�h�擾
//==========================================================================
CScene::MODE CScene::GetMode()
{
	return m_mode;
}
