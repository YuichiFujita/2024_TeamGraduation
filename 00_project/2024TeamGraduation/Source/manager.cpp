//=============================================================================
// 
//  �}�l�[�W������ [manager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "object2D.h"
#include "object3D.h"
#include "objectBillboard.h"
#include "objectX.h"
#include "texture.h"
#include "calculation.h"
#include "lostrssmanager.h"

#include "pause.h"
#include "fade.h"
#include "instantfade.h"
#include "blackframe.h"
#include "light.h"
#include "camera.h"
#include "edit.h"
#include "resultmanager.h"
#include "rankingmanager.h"
#include "loadmanager.h"
#include "Imguimanager.h"
#include "fog.h"


//#include <vlc/vlc.h>

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float TIME_LOAD = 2.0f;	// �K�{���[�h����

#if _DEBUG
	const CScene::MODE STARTMODE = CScene::MODE::MODE_GAME;
#else
	const CScene::MODE STARTMODE = CScene::MODE::MODE_TITLE;
#endif
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CManager *CManager::m_pManager = nullptr;		// �}�l�[�W���̃I�u�W�F�N�g
bool CManager::m_bDisp_ImGui = false;			// ImGui�̕`�攻��
bool CManager::m_bDisp_BoxColliders = false;	// �����蔻��{�b�N�X�̕`�攻��
bool CManager::m_bDisp_CheckPoint = true;		// �`�F�b�N�|�C���g�̕`�攻��

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CManager::CManager()
{
	m_pRenderer = nullptr;			// �����_���[
	m_pInput = nullptr;				// ����
	m_pSound = nullptr;				// �T�E���h
	m_pDebugProc = nullptr;			// �f�o�b�O�\��
	m_pLight = nullptr;				// ���C�g
	m_pCamera = nullptr;			// �J����
	m_pTexture = nullptr;			// �e�N�X�`��
	m_pXLoad = nullptr;				// X�t�@�C��
	m_pEdit = nullptr;				// �G�f�B�b�g
	m_pScene = nullptr;				// �V�[��
	m_pFade = nullptr;				// �t�F�[�h
	m_pInstantFade = nullptr;		// �J�ڂȂ��t�F�[�h
	m_pPause = nullptr;				// �|�[�Y
	m_pResultManager = nullptr;		// ���U���g�}�l�[�W��
	m_pRankingManager = nullptr;	// �����L���O�}�l�[�W��
	m_pMyEffekseer = nullptr;		// �G�t�F�N�V�A
	m_bWireframe = false;			// ���C���[�t���[��
	m_bHitStop = false;				// �q�b�g�X�g�b�v�̔���
	m_nCntHitStop = 0;				// �q�b�g�X�g�b�v�̃J�E���^�[
	m_OldMode = CScene::MODE_NONE;	// �O��̃��[�h
	m_CurrentTime = 0;				// ���ݎ���
	m_OldTime = 0;					// �ߋ��̎���
	m_fDeltaTime = 0.0f;			// �o�ߎ���
	m_nNumPlayer = 0;				// �v���C���[�̐�
	m_fLoadTimer = 0.0f;			// ���[�h�̃^�C�}�[
	m_bLoadComplete = false;		// ���[�h�����̃t���O
	m_bFirstLoad = false;			// ���񃍁[�h
	m_bDisp_2D = false;				// 2D�̕\��
	m_bDisp_UI = true;				// UI�̕\��
	m_bWindowed = true;

	// ���[�h�t���O���Z�b�g
	m_bLoadComplete = false;

	// ���[�h�̃t�F�[�h�ݒ�t���O
	m_bLoadFadeSet = false;
	m_bNowLoading = false;				// ���[�h�����̃t���O
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CManager::~CManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CManager *CManager::Create()
{
	if (m_pManager == nullptr)
	{// �܂��������Ă��Ȃ�������

		// �}�l�[�W���̃C���X�^���X����
		m_pManager = DEBUG_NEW CManager;
	}
	else
	{
		// �C���X�^���X�擾
		m_pManager->GetInstance();
	}

	return m_pManager;
}

//==========================================================================
// �C���X�^���X�擾
//==========================================================================
CManager *CManager::GetInstance()
{
	return m_pManager;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{

	HRESULT hr;
	m_bHitStop = false;		// �q�b�g�X�g�b�v�̔���
	m_nCntHitStop = 0;		// �q�b�g�X�g�b�v�̃J�E���^�[
	m_nNumPlayer = 0;		// �v���C���[�̐�

	//**********************************
	// ���X�g���郊�\�[�X�̃}�l�[�W���[
	//**********************************
	CLostResourceManager::GetInstance();	//�擾�͂��Ȃ����ǐ����͂���

	//**********************************
	// ����
	//**********************************
	// ����
	m_pInput = CInput::Create(hInstance, hWnd);


	//**********************************
	// �����_���[
	//**********************************
	if (m_pRenderer != nullptr)
	{// �m�ۂ���Ă�����

		return E_FAIL;
	}

	// �������m��
	m_pRenderer = DEBUG_NEW CRenderer;

	if (m_pRenderer != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		hr = m_pRenderer->Init(hWnd, bWindow);
		if (FAILED(hr))
		{// ���������������s�����ꍇ
			return E_FAIL;
		}
	}


	// Imgui�̏�����
	ImguiMgr::Init(hWnd, m_pRenderer->GetDevice());

	//**********************************
	// �T�E���h
	//**********************************
	m_pSound = CSound::Create(hWnd);
	if (m_pSound == nullptr)
	{
		return E_FAIL;
	}


	//**********************************
	// �f�o�b�O�\��
	//**********************************
	if (m_pDebugProc != nullptr)
	{// �m�ۂ���Ă�����
		return E_FAIL;
	}

	// �������m��
	m_pDebugProc = DEBUG_NEW CDebugProc;

	if (m_pDebugProc != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		hr = m_pDebugProc->Init(hInstance, hWnd);
		if (FAILED(hr))
		{// ���������������s�����ꍇ
			return E_FAIL;
		}
	}


	//**********************************
	// ���C�g
	//**********************************
	if (m_pLight != nullptr)
	{// �m�ۂ���Ă�����
		return E_FAIL;
	}

	// �������m��
	m_pLight = DEBUG_NEW CLight;

	if (m_pLight != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		hr = m_pLight->Init();
		if (FAILED(hr))
		{// ���������������s�����ꍇ
			return E_FAIL;
		}
	}


	//**********************************
	// �J����
	//**********************************
	if (m_pCamera != nullptr)
	{// �m�ۂ���Ă�����
		return E_FAIL;
	}

	// �������m��
	m_pCamera = DEBUG_NEW CCamera;

	if (m_pCamera != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		hr = m_pCamera->Init();
		if (FAILED(hr))
		{// ���������������s�����ꍇ
			return E_FAIL;
		}
		m_pCamera->Reset(STARTMODE);

		// �J�����̃}�g���b�N�X�ݒ�
		CInputMouse::GetInstance()->SetCameraMtx(m_pCamera->GetMtxViewPtr(), m_pCamera->GetMtxProjectionPtr());
	}

	//**********************************
	// �G�t�F�N�V�A
	//**********************************
	m_pMyEffekseer = CMyEffekseer::Create();
	if (m_pMyEffekseer == nullptr)
	{
		return E_FAIL;
	}

	//**********************************
	// �e�N�X�`��
	//**********************************
	m_pTexture = CTexture::Create();
	if (m_pTexture == nullptr)
	{
		return E_FAIL;
	}

	//**********************************
	// X�t�@�C��
	//**********************************
	m_pXLoad = CXLoad::Create();
	if (m_pXLoad == nullptr)
	{
		return E_FAIL;
	}

	//**********************************
	// �t�F�[�h
	//**********************************
	m_pFade = CFade::Create();
	if (m_pFade == nullptr)
	{// ���s���Ă�����
		return E_FAIL;
	}


	// ���[�h�t���O���Z�b�g
	m_bLoadComplete = false;
	m_bLoadFadeSet = false;	// ���[�h�̃t�F�[�h�ݒ�t���O
	m_bNowLoading = true;

	// �V�[���̃��[�h���J�n
	GetLoadManager()->LoadScene(CScene::MODE_NONE);





#if 0
	/* VLC�G���W����ǂݍ��� */
	libvlc_instance_t* instance = libvlc_new(0, NULL);
	if (instance == NULL) {
		std::cerr << "libvlc_new failed: " << libvlc_errmsg() << std::endl;
		return -1;
	}

	/* �V�������ڂ��쐬���� */
	libvlc_media_t* media = libvlc_media_new_path(instance, "data\\MOVIE\\ohayo01mayu.mp3"); // �t�@�C���p�X (���΃p�X)
	// media = libvlc_media_new_path(instance, "C:\\path/sample.mp3");          // �t�@�C���p�X (��΃p�X)
	// media = libvlc_media_new_location(instance, "http://example.com/sample.mp3"); // URL

	/* ���f�B�A�v���[���[�̍Đ������쐬���� */
	libvlc_media_player_t* media_player = libvlc_media_player_new_from_media(media);

	/* ���f�B�A��ێ�����K�v���Ȃ��Ȃ������߁A���f�B�A�̎Q�ƃJ�E���g������������ */
	libvlc_media_release(media);

	/* ���f�B�A�v���[���[���Đ����� */
	libvlc_media_player_play(media_player);

	/* �ҋ@���� */
	while (libvlc_state_t::libvlc_Ended != libvlc_media_player_get_state(media_player))
	{
		Sleep(100);
	}

	/* �Đ����~�߂� */
	libvlc_media_player_stop(media_player);

	/* ���f�B�A�v���[���[��������� */
	libvlc_media_player_release(media_player);

	libvlc_release(instance);
#endif





	return S_OK;
}

//==========================================================================
// �ǂݍ���
//==========================================================================
void CManager::Load()
{

	// �S�Ẵe�N�X�`���ǂݍ���
	m_pTexture->LoadAll();

	// �S�Ẵ��f���ǂݍ���
	m_pXLoad->LoadAll();

	//**********************************
	// �J�ڂȂ��t�F�[�h
	//**********************************
	m_pInstantFade = CInstantFade::Create();
	if (m_pInstantFade == nullptr)
	{
		return;
	}

	//**********************************
	// �|�[�Y
	//**********************************
	m_pPause = CPause::Create(STARTMODE);
	if (m_pPause == nullptr)
	{
		return;
	}

	//**********************************
	// ���U���g�}�l�[�W��
	//**********************************
	m_pResultManager = CResultManager::Create();
	if (m_pResultManager == nullptr)
	{
		return;
	}

	//**********************************
	// �����L���O�}�l�[�W��
	//**********************************
	m_pRankingManager = CRankingManager::Create();
	if (m_pRankingManager == nullptr)
	{
		return;
	}

	// ���[�h�ݒ�
	NoLoadSetMode(STARTMODE);
}

//==========================================================================
// ���̃��[�h�ݒ�
//==========================================================================
void CManager::SetMode(CScene::MODE mode)
{
	GetLoadManager()->UnLoad();

	// ���̃��[�h�ݒ�
	NoLoadSetMode(mode);

	// ���[�h�̃^�C�}�[���Z�b�g
	m_fLoadTimer = 0.0f;

	// ���[�h�t���O���Z�b�g
	m_bLoadComplete = false;
	m_bLoadFadeSet = false;				// ���[�h�̃t�F�[�h�ݒ�t���O
	m_bNowLoading = true;

	// �V�[���̃��[�h���J�n
	GetLoadManager()->LoadScene(mode);
}

//==========================================================================
// ���̃��[�h�ݒ�
//==========================================================================
void CManager::NoLoadSetMode(CScene::MODE mode)
{
	// ���Z�b�g
	Reset(mode);

	// ��������
	m_pScene = CScene::Create(mode);

	// ���[�h�̃^�C�}�[���Z�b�g
	m_fLoadTimer = 0.0f;
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CManager::Reset(CScene::MODE mode)
{
	m_bHitStop = false;		// �q�b�g�X�g�b�v�̔���
	m_nCntHitStop = 0;		// �q�b�g�X�g�b�v�̃J�E���^�[

	// �O��̃��[�h�ݒ�
	m_OldMode = GetMode();

	if (mode == CScene::MODE_GAME && m_pResultManager != nullptr && m_pRankingManager != nullptr)
	{// ���̃��[�h���Q�[����������

		// �X�R�A��񃊃Z�b�g
		m_pResultManager->Reset();
		m_pRankingManager->Reset();
	}

	// BGM�X�g�b�v
	if (m_pSound != nullptr)
	{
		m_pSound->StopSound();
	}

	// �������̊m�ۂ�����Ă�����
	if (m_pScene != nullptr)
	{
		// �I������
		m_pScene->Uninit();
		m_pScene->Kill();
		delete m_pScene;
		m_pScene = nullptr;
	}

	// �S�ẴI�u�W�F�N�g�j��
	CObject::ReleaseAll();

	// �}�b�v
	//MyMap::Release();

	// �|�[�Y�󋵓���ւ�
	if (m_pPause != nullptr)
	{
		ChangePauseMode(mode);

		if (m_pPause->IsPause())
		{// �|�[�Y����������
			m_pPause->SetPause();
		}
	}

	// �J�����̏�񃊃Z�b�g
	if (m_pCamera != nullptr)
	{
		m_pCamera->Reset(mode);
	}
}

//==========================================================================
// ���݂̃��[�h�擾
//==========================================================================
CScene::MODE CManager::GetMode()
{
	// ���[�h�擾
	if (m_pScene != nullptr)
	{
		return m_pScene->GetMode();
	}
	else
	{
		return CScene::MODE_NONE;
	}
}

//==========================================================================
// �I������
//==========================================================================
void CManager::Uninit()
{
	if (m_pScene != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pScene->Kill();
	}

	// �S�ẴI�u�W�F�N�g�j��
	CObject::ReleaseAll();

	// BGM�X�g�b�v
	m_pSound->StopSound();

	// ���͋@��̔j��
	if (m_pInput != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pInput->Release();
	}

	// �����_���[�̔j��
	if (m_pRenderer != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pRenderer->Uninit();

		// �������̊J��
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// Imgui�̏I��
	ImguiMgr::Uninit();

	// ���C�g�̔j��
	if (m_pLight != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pLight->Uninit();

		// �������̊J��
		delete m_pLight;
		m_pLight = nullptr;
	}

	// �J�����̔j��
	if (m_pCamera != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pCamera->Uninit();

		// �������̊J��
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pMyEffekseer != nullptr)
	{
		// �I������
		m_pMyEffekseer->Uninit();
		m_pMyEffekseer = nullptr;
	}

	// �T�E���h�̔j��
	if (m_pSound != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pSound->Uninit();

		// �������̊J��
		delete m_pSound;
		m_pSound = nullptr;
	}

	// �f�o�b�O�\���̔j��
	if (m_pDebugProc != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pDebugProc->Uninit();

		// �������̊J��
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}

	// �G�f�B�b�g�̔j��
	if (m_pEdit != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		m_pEdit = nullptr;
	}

	//**********************************
	// �S�Ẵe�N�X�`���j��
	//**********************************
	if (m_pTexture != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pTexture->Unload();

		// �������̊J��
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pXLoad != nullptr)
	{
		// �I������
		m_pXLoad->Uninit();

		// �������̊J��
		m_pXLoad = nullptr;
	}

	if (m_pScene != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	// �t�F�[�h�̔j��
	if (m_pFade != nullptr)
	{// �������̊m�ۂ�����Ă�����

		// �I������
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

	// �J�ڂȂ��t�F�[�h�̔j��
	if (m_pInstantFade != nullptr)
	{// �������̊m�ۂ�����Ă�����

		// �I������
		m_pInstantFade->Uninit();
		delete m_pInstantFade;
		m_pInstantFade = nullptr;
	}

	if (m_pPause != nullptr)
	{// �������̊m�ۂ�����Ă�����

		// �I������
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = nullptr;
	}

	if (m_pResultManager != nullptr)
	{// �������̊m�ۂ�����Ă�����

		// �I������
		m_pResultManager->Uninit();
		m_pResultManager = nullptr;
	}

	if (m_pRankingManager != nullptr)
	{// �������̊m�ۂ�����Ă�����

		// �I������
		m_pRankingManager->Uninit();
		delete m_pRankingManager;
		m_pRankingManager = nullptr;
	}

	// ���X�g���郊�\�[�X�Ǘ��}�l�[�W���[�j��
	CLostResourceManager::Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CManager::Update()
{
	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();

	// �ߋ��̎��ԕۑ�
	m_OldTime = m_CurrentTime;

	// �o�ߎ���
	m_CurrentTime = timeGetTime();
	m_fDeltaTime = (float)(m_CurrentTime - m_OldTime) / 1000;

	// Imgui�̍X�V
	ImguiMgr::Update();

	// ���񃍁[�h����
	if (!m_bFirstLoad)
	{
		bool bComplete = GetLoadManager()->IsLoadComplete();

		if (bComplete &&
			!m_bLoadFadeSet)
		{
			CManager::GetInstance()->GetInstantFade()->SetFade();
			m_bLoadFadeSet = true;	// �t�F�[�h�̃Z�b�g�t���O
		}

		// ���[�h�}�l�[�W���̍X�V
		GetLoadManager()->Update();

		if (m_bLoadFadeSet)
		{// �t�F�[�h���ݒ肳��Ă���

			// �J�ڂȂ��t�F�[�h�̍X�V����
			m_pInstantFade->Update();

			if (m_pInstantFade->GetState() == CInstantFade::STATE_FADECOMPLETION)
			{
				m_bLoadComplete = true;	// ���[�h����
				m_bNowLoading = false;	// ���[�h���t���O�I�t
				m_bFirstLoad = true;	// ���񃍁[�h�I��
				SetMode(STARTMODE);
			}
		}
		return;
	}

	if (m_bNowLoading)
	{
		// ���[�h��
		if (!m_bLoadComplete)
		{
			// ���[�h���ԉ��Z
			m_fLoadTimer += m_fDeltaTime;
		}

		bool bComplete = GetLoadManager()->IsLoadComplete();

		if (bComplete &&
			m_fLoadTimer >= TIME_LOAD &&
			!m_bLoadFadeSet)
		{// ���[�h�����̏�������

			CManager::GetInstance()->GetInstantFade()->SetFade();
			m_bLoadFadeSet = true;	// �t�F�[�h�̃Z�b�g�t���O
		}

		if (m_bLoadFadeSet)
		{// �t�F�[�h���ݒ肳��Ă���
			if (m_pInstantFade->GetState() == CInstantFade::STATE_FADECOMPLETION)
			{
				m_bLoadComplete = true;	// ���[�h����
				m_bNowLoading = false;	// ���[�h���t���O�I�t
			}
		}

		// ���[�h�}�l�[�W���̍X�V
		if (!m_bLoadComplete)
		{
			GetLoadManager()->Update();
		}
	}

	// �t�F�[�h�̍X�V����
	m_pFade->Update();

	// �J�ڂȂ��t�F�[�h�̍X�V����
	m_pInstantFade->Update();

	if (!m_bLoadComplete)
	{
		return;
	}

	if (m_bLoadComplete)
	{

		// ���͋@��̍X�V����
		m_pInput->Update();

		if ((pInputKeyboard->GetTrigger(DIK_P) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_START, 0)) &&
			m_pFade->GetState() == CFade::STATE_NONE &&
			(GetMode() == CScene::MODE_GAME || GetMode() == CScene::MODE::MODE_GAMETUTORIAL))
		{// �t�F�[�h������Ȃ��Ƃ�

			// �T�E���h�Đ�
			m_pSound->PlaySound(CSound::LABEL::LABEL_SE_CURSOR_END);
			m_pPause->SetPause();
		}

		if (m_pPause != nullptr)
		{
			// �|�[�Y�󋵎擾
			bool bPause = m_pPause->IsPause();
			CCameraMotion* pCamMotion = m_pCamera->GetCameraMotion();
			if (pCamMotion != nullptr)
			{
				pCamMotion->SetEnableSystemPause(bPause);
			}


			// �|�[�Y�̍X�V����
			if (bPause)
			{// �|�[�Y����������
				m_pPause->Update();

				if (!GetLoadManager()->IsLoadComplete())
				{
					return;
				}
//#if _DEBUG

				// �J�����̍X�V����
				m_pCamera->Update();
//#endif

				return;
			}
		}

		if (m_bHitStop)
		{// �q�b�g�X�g�b�v���͍X�V��~

			// �q�b�g�X�g�b�v�J�E���^�[���Z
			m_nCntHitStop--;

			if (m_nCntHitStop <= 0)
			{// �J�E���^�[���Ȃ��Ȃ�����
				m_bHitStop = false;
			}
		}
		else
		{
			m_nCntHitStop = 20;
		}

#if _DEBUG

		// �`�攻��
		if (pInputKeyboard->GetTrigger(DIK_F1))
		{// F1�Ń��C���[�t���[���؂�ւ�
			m_bDisp_ImGui = m_bDisp_ImGui ? false : true;
		}
		ImGui::Checkbox("Disp BoxColliders", &m_bDisp_BoxColliders);
		ImGui::Checkbox("Disp CheckPoint", &m_bDisp_CheckPoint);
		ImGui::Checkbox("Disp 2D", &m_bDisp_2D);

		if (pInputKeyboard->GetTrigger(DIK_F2))
		{// F2�Ń��C���[�t���[���؂�ւ�
			m_bWireframe = m_bWireframe ? false : true;
		}

		if (pInputKeyboard->GetTrigger(DIK_F6))
		{
			MyFog::ToggleFogFrag();
		}

		//if (pInputKeyboard->GetTrigger(DIK_F8))
		//{// F8�ŃG�f�B�b�g���[�h�ؑւ�

		//	if (m_pEdit == nullptr)
		//	{// nullptr��������

		//		// �G�f�B�b�g�̐�������
		//		m_pEdit = CEdit::Create();
		//	}
		//	else
		//	{
		//		// �I��������
		//		m_pEdit->Release();
		//		m_pEdit = nullptr;
		//	}
		//}
#endif


		if (m_pScene != nullptr)
		{
			m_pScene->Update();
		}

		// �����_���[�̍X�V����
		if (m_pRenderer != nullptr)
		{
			m_pRenderer->Update();
		}

		// ���C�g�̍X�V����
		m_pLight->Update();

		// �J�����̍X�V����
		m_pCamera->Update();

		// �f�o�b�O�\���̍X�V����
		m_pDebugProc->Update();
	}
}

//==========================================================================
// �f�B�X�v���C���[�h�ύX
//==========================================================================
void CManager::ChangeDisplayMode(bool bWindow)
{
	m_bWindowed = bWindow;

	if (m_bWindowed)
	{// �E�B���h�E���[�h
		m_pRenderer->SetDisplayMode(CRenderer::DISPLAYMODE::MODE_WINDOW);
	}
	else
	{// �E�B���h�E���X���[�h
		m_pRenderer->SetDisplayMode(CRenderer::DISPLAYMODE::MODE_FULLSCREEN);
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CManager::Draw()
{
	/*if (!m_bLoadComplete)
	{
		GetLoadManager()->Draw();
		return;
	}*/

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = m_pRenderer->GetDevice();

	if (m_bWireframe)
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	// ���߂郂�[�h
	}

	// �����_���[�̕`�揈��
	m_pRenderer->Draw();

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// ���߂郂�[�h
}

//==========================================================================
// �v���C���[�̐��擾
//==========================================================================
int CManager::GetNumPlayer()
{
	return m_nNumPlayer;
}

//==========================================================================
// �v���C���[�̐��ݒ�
//==========================================================================
void CManager::SetNumPlayer(int nNum)
{
	// �l�̐��K������
	UtilFunc::Transformation::ValueNormalize(nNum, mylib_const::MAX_PLAYER, 0);

	m_nNumPlayer = nNum;
}

//==========================================================================
// �|�[�Y�̎�ސ؂�ւ�
//==========================================================================
void CManager::ChangePauseMode(CScene::MODE mode)
{
	switch (mode)
	{
	case CScene::MODE::MODE_GAME:
	case CScene::MODE::MODE_GAMETUTORIAL:

		// �폜
		if (m_pPause!= nullptr)
		{
			m_pPause->Kill();
			delete m_pPause;
			m_pPause = nullptr;
		}

		// �Đ���
		m_pPause = CPause::Create(mode);
		break;

	default:
		break;
	}
}

//==========================================================================
// �o�ߎ��Ԏ擾
//==========================================================================
float CManager::GetDeltaTime()
{
	return m_fDeltaTime;
}

//==========================================================================
// �q�b�g�X�g�b�v�̐ݒ�
//==========================================================================
void CManager::SetEnableHitStop(int nCntHitStop)
{
	m_bHitStop = true;
	m_nCntHitStop = nCntHitStop;
}

//==========================================================================
// �����_���[�̎擾
//==========================================================================
CRenderer *CManager::GetRenderer()
{
	return m_pRenderer;
}

//==========================================================================
// �f�o�b�O�\���̎擾
//==========================================================================
CDebugProc *CManager::GetDebugProc()
{
	return m_pDebugProc;
}

//==========================================================================
// ���C�g�̎擾
//==========================================================================
CLight *CManager::GetLight()
{
	return m_pLight;
}

//==========================================================================
// �J�����̎擾
//==========================================================================
CCamera *CManager::GetCamera()
{
	return m_pCamera;
}

//==========================================================================
// �G�f�B�b�g�̎擾
//==========================================================================
CEdit *CManager::GetEdit()
{
	return m_pEdit;
}

//==========================================================================
// �t�F�[�h�̎擾
//==========================================================================
CFade *CManager::GetFade()
{
	return m_pFade;
}

//==========================================================================
// �J�ڂȂ��t�F�[�h�̎擾
//==========================================================================
CInstantFade *CManager::GetInstantFade()
{
	return m_pInstantFade;
}

//==========================================================================
// �|�[�Y�̎擾
//==========================================================================
CPause *CManager::GetPause()
{
	return m_pPause;
}

//==========================================================================
// ���U���g�}�l�[�W���擾
//==========================================================================
CResultManager *CManager::GetResultManager()
{
	return m_pResultManager;
}

//==========================================================================
// �����L���O�}�l�[�W���̃I�u�W�F�N�g
//==========================================================================
CRankingManager *CManager::GetRankingManager()
{
	return m_pRankingManager;
}

//==========================================================================
// �V�[���擾
//==========================================================================
CScene *CManager::GetScene()
{
	return m_pScene;
}

