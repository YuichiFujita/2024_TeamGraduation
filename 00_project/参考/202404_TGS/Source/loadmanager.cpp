//=============================================================================
// 
//  ���[�h�}�l�[�W�� [loadmanager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "loadmanager.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "loadscreen.h"
#include "fade.h"
#include "instantfade.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CLoadManager::CLoadManager()
{
	m_bLoadComplete = false;
	m_ModeNext = CScene::MODE_NONE;
	m_pLoadScreen = nullptr;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CLoadManager::~CLoadManager()
{
	// ���[�h�X���b�h���܂������Ă���Ȃ�ҋ@
	if (m_LoadingThread.joinable()) 
	{
		m_LoadingThread.join();
	}
}

//==========================================================================
// ��������
//==========================================================================
CLoadManager* CLoadManager::Create()
{

	// �������̊m��
	CLoadManager* pLoadManager = DEBUG_NEW CLoadManager;

	if (pLoadManager != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		pLoadManager->Init();
	}

	return pLoadManager;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CLoadManager::Init()
{
	// ���[�h�X���b�h���܂������Ă���Ȃ�ҋ@
	if (m_LoadingThread.joinable()) 
	{
		m_LoadingThread.join();
	}
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CLoadManager::Uninit()
{
	if (m_pLoadScreen != nullptr)
	{
		m_pLoadScreen->Kill();
		delete m_pLoadScreen;
		m_pLoadScreen = nullptr;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CLoadManager::Update()
{
	if (m_pLoadScreen != nullptr)
	{
		m_pLoadScreen->Update();
	}
}

//==========================================================================
// ���[�h���
//==========================================================================
void CLoadManager::UnLoad()
{
	if (m_LoadingThread.joinable())
	{
		m_LoadingThread.join();
	}
}

//==========================================================================
// ���[�h���Z�b�g
//==========================================================================
void CLoadManager::ResetLoad()
{
	// ���[�h���Z�b�g
	ResetInternalLoad();
}

//==========================================================================
// �V�����V�[�����Z�b�g����O�̃��Z�b�g
//==========================================================================
void CLoadManager::ResetInternalLoad()
{
	// �I����҂�
	if (m_LoadingThread.joinable())
	{
		m_LoadingThread.join();
	}

	// ���[�h�������t���O�����Z�b�g
	m_bLoadComplete = false;
}

//==========================================================================
// �V�[���̃��[�h���J�n
//==========================================================================
void CLoadManager::LoadScene(CScene::MODE mode)
{
	// ���̃��[�h
	m_ModeNext = mode;

	if (m_pLoadScreen == nullptr)
	{
		m_pLoadScreen = CLoadScreen::Create();
	}

	if (m_LoadingThread.joinable())
	{
		// �X���b�h����������܂őҋ@
		m_LoadingThread.join();
	}

	// ���[�h������
	ResetLoad();

	// ���[�h���Ăюn�܂�̂Ńt���O�����Z�b�g
	m_bLoadComplete = false;

    // ���[�h�����̊J�n
	m_LoadingThread = std::thread(&CLoadManager::LoadInBackground, this);

	// �T�u�X���b�h���f�^�b�`
	m_LoadingThread.detach();
}

//==========================================================================
// ���[�h��ʂ�񓯊��ŕ\��
//==========================================================================
void CLoadManager::LoadInBackground()
{
	// ���[�h���Ăюn�܂�̂Ńt���O�����Z�b�g
	m_bLoadComplete = false;

	try
	{
		// �ǂݍ��ݏ���
		Load();
	}
	catch (const std::exception& e)
	{// ��O
		return;
	}

	if (m_LoadingThread.joinable())
	{
		m_LoadingThread.join();
	}

	// ���[�h�����t���O���Z�b�g
	m_bLoadComplete = true;
}

//==========================================================================
// ���[�h����
//==========================================================================
void CLoadManager::Load()
{
	// �V�[���̏���������
	if (m_ModeNext != CScene::MODE_NONE)
	{
		CManager::GetInstance()->GetScene()->Init();
	}
	else
	{
		CManager::GetInstance()->Load();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CLoadManager::Draw()
{
	if (m_pLoadScreen != nullptr)
	{
		m_pLoadScreen->Draw();
	}
}

//==========================================================================
// ���[�h�������������ǂ�����Ԃ�
//==========================================================================
bool CLoadManager::IsLoadComplete()
{
	return m_bLoadComplete;
}