//=============================================================================
// 
//  �}�l�[�W������ [manager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "object2D.h"
#include "object3D.h"
#include "objectBillboard.h"
#include "objectX.h"
#include "texture.h"
#include "Xload.h"
#include "calculation.h"

#include "light.h"
#include "camera.h"
#include "player.h"
#include "3D_effect.h"
#include "particle.h"
#include "map.h"
#include "edit.h"
#include "meshfield.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CRenderer *CManager::m_pRenderer = NULL;			// �����_���[�̃I�u�W�F�N�g
CInputKeyboard *CManager::m_pInputKeyboard = NULL;	// �L�[�{�[�h�̃I�u�W�F�N�g
CInputGamepad *CManager::m_pInputGamepad = NULL;	// �Q�[���p�b�h�̃I�u�W�F�N�g
CSound *CManager::m_pSound = NULL;					// �T�E���h�̃I�u�W�F�N�g
CInputMouse *CManager::m_pInputMouse = NULL;		// �}�E�X�̃I�u�W�F�N�g
CDebugProc *CManager::m_pDebugProc = NULL;			// �f�o�b�O�\���̃I�u�W�F�N�g
CPlayer *CManager::m_pPlayer = NULL;				// �v���C���[�̃I�u�W�F�N�g
CParticle *CManager::m_pParticle = NULL;			// �p�[�e�B�N���̃I�u�W�F�N�g
CScore *CManager::m_pScore = NULL;					// �X�R�A�̃I�u�W�F�N�g
CTimer *CManager::m_pTimer = NULL;					// �^�C�}�[�̃I�u�W�F�N�g
CBG *CManager::m_pBg = NULL;						// �w�i�̃I�u�W�F�N�g
CLight *CManager::m_pLight = NULL;					// ���C�g�̃I�u�W�F�N�g
CCamera *CManager::m_pCamera = NULL;				// �J�����̃I�u�W�F�N�g
CMap *CManager::m_pMap = NULL;						// �}�b�v�̃I�u�W�F�N�g
CTexture *CManager::m_pTexture = NULL;				// �e�N�X�`���̃I�u�W�F�N�g
CXLoad *CManager::m_pXLoad = NULL;					// X�t�@�C���̃I�u�W�F�N�g
CEdit *CManager::m_pEdit = NULL;					// �G�f�B�b�g�̃I�u�W�F�N�g
CElevation *CManager::m_pObject3DMesh = NULL;	// �I�u�W�F�N�g3D���b�V���̃I�u�W�F�N�g
CEnemyManager *CManager::m_pEnemyManager = NULL;	// �G�}�l�[�W���̃I�u�W�F�N�g
bool CManager::m_bWireframe = false;				// ���C���[�t���[��
CManager::EDITTYPE CManager::m_editType = EDITTYPE_EDIT;		// �G�f�B�b�g�̎��

// �v���g�^�C�v�錾
std::string LoadMotionFileName(void);

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CManager::CManager()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CManager::~CManager()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{

	HRESULT hr;
	m_editType = EDITTYPE_EDIT;

	//**********************************
	// �L�[�{�[�h
	//**********************************
	if (m_pInputKeyboard != NULL)
	{// �m�ۂ���Ă�����
		return E_FAIL;
	}

	// �������m��
	m_pInputKeyboard = DEBUG_NEW CInputKeyboard;

	if (m_pInputKeyboard != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		hr = m_pInputKeyboard->Init(hInstance, hWnd);
		if (FAILED(hr))
		{// ���������������s�����ꍇ

			return E_FAIL;
		}
	}

	
	//**********************************
	// �Q�[���p�b�h
	//**********************************
	if (m_pInputGamepad != NULL)
	{// �m�ۂ���Ă�����
		return E_FAIL;
	}

	// �������m��
	m_pInputGamepad = DEBUG_NEW CInputGamepad;

	if (m_pInputGamepad != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		hr = m_pInputGamepad->Init(hInstance, hWnd);
		if (FAILED(hr))
		{// ���������������s�����ꍇ

			return E_FAIL;
		}
	}


	//**********************************
	// �}�E�X
	//**********************************
	if (m_pInputMouse != NULL)
	{// �m�ۂ���Ă�����
		return E_FAIL;
	}

	// �������m��
	m_pInputMouse = DEBUG_NEW CInputMouse;

	if (m_pInputMouse != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		hr = m_pInputMouse->Init(hInstance, hWnd);
		if (FAILED(hr))
		{// ���������������s�����ꍇ

			return E_FAIL;
		}
	}


	//**********************************
	// �����_���[
	//**********************************
	if (m_pRenderer != NULL)
	{// �m�ۂ���Ă�����

		return E_FAIL;
	}

	// �������m��
	m_pRenderer = DEBUG_NEW CRenderer;

	if (m_pRenderer != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		hr = m_pRenderer->Init(hWnd, TRUE);
		if (FAILED(hr))
		{// ���������������s�����ꍇ

			return E_FAIL;
		}
	}


	//**********************************
	// �T�E���h
	//**********************************
	if (m_pSound != NULL)
	{// �m�ۂ���Ă�����
		return E_FAIL;
	}

	// �������m��
	m_pSound = DEBUG_NEW CSound;

	if (m_pSound != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		hr = m_pSound->Init(hWnd);
		if (FAILED(hr))
		{// ���������������s�����ꍇ

			return E_FAIL;
		}
	}


	//**********************************
	// �f�o�b�O�\��
	//**********************************
	if (m_pDebugProc != NULL)
	{// �m�ۂ���Ă�����
		return E_FAIL;
	}

	// �������m��
	m_pDebugProc = DEBUG_NEW CDebugProc;

	if (m_pDebugProc != NULL)
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
	if (m_pLight != NULL)
	{// �m�ۂ���Ă�����
		return E_FAIL;
	}

	// �������m��
	m_pLight = DEBUG_NEW CLight;

	if (m_pLight != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		hr = m_pLight->Init();
		if (FAILED(hr))
		{// ���������������s�����ꍇ

			return E_FAIL;
		}
	}


	//**********************************
	// �p�[�e�B�N��
	//**********************************
	if (m_pParticle != NULL)
	{// �m�ۂ���Ă�����
		return E_FAIL;
	}

	// �������m��
	m_pParticle = DEBUG_NEW CParticle;

	if (m_pParticle != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		hr = m_pParticle->Init();
		if (FAILED(hr))
		{// ���������������s�����ꍇ

			return E_FAIL;
		}
	}


	//**********************************
	// �J����
	//**********************************
	if (m_pCamera != NULL)
	{// �m�ۂ���Ă�����
		return E_FAIL;
	}

	// �������m��
	m_pCamera = DEBUG_NEW CCamera;

	if (m_pCamera != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		hr = m_pCamera->Init();
		if (FAILED(hr))
		{// ���������������s�����ꍇ

			return E_FAIL;
		}
	}

	//**********************************
	// �S�Ẵe�N�X�`���ǂݍ���
	//**********************************
	if (m_pTexture != NULL)
	{// �m�ۂ���Ă�����
		return E_FAIL;
	}

	// �������m��
	m_pTexture = DEBUG_NEW CTexture;

	if (m_pTexture != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		hr = m_pTexture->Load();
		if (FAILED(hr))
		{// ���������������s�����ꍇ

			return E_FAIL;
		}

		// ����������
		hr = m_pTexture->LoadAll();

		if (FAILED(hr))
		{// ���������������s�����ꍇ

			return E_FAIL;
		}
	}


	//**********************************
	// X�t�@�C��
	//**********************************
	if (m_pXLoad != NULL)
	{// �m�ۂ���Ă�����
		return E_FAIL;
	}

	// �������m��
	m_pXLoad = DEBUG_NEW CXLoad;

	if (m_pXLoad == NULL)
	{// �������̊m�ۂ��o���Ă��Ȃ�������
		return E_FAIL;
	}

	//**********************************
	// �}�b�v
	//**********************************
	MyMap::Create();

	//**********************************
	// �I�u�W�F�N�g����
	//**********************************
	CObject2D *pObject2D = NULL;

	//**********************************
	// �v���C���[
	//**********************************
	// �������m��
	m_pPlayer = m_pPlayer->Create(LoadMotionFileName());
	m_pPlayer->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CManager::Uninit(void)
{
	// �S�ẴI�u�W�F�N�g�j��
	CObject::ReleaseAll();

	// BGM�X�g�b�v
	m_pSound->StopSound();

	// �L�[�{�[�h�̔j��
	if (m_pInputKeyboard != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pInputKeyboard->Uninit();

		// �������̊J��
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// �Q�[���p�b�h�̔j��
	if (m_pInputGamepad != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pInputGamepad->Uninit();

		// �������̊J��
		delete m_pInputGamepad;
		m_pInputGamepad = NULL;
	}

	// �}�E�X�̔j��
	if (m_pInputMouse != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pInputMouse->Uninit();

		// �������̊J��
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// �����_���[�̔j��
	if (m_pRenderer != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pRenderer->Uninit();

		// �������̊J��
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// ���C�g�̔j��
	if (m_pLight != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pLight->Uninit();

		// �������̊J��
		delete m_pLight;
		m_pLight = NULL;
	}

	// �J�����̔j��
	if (m_pCamera != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pCamera->Uninit();

		// �������̊J��
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// �T�E���h�̔j��
	if (m_pSound != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pSound->Uninit();

		// �������̊J��
		delete m_pSound;
		m_pSound = NULL;
	}

	// �f�o�b�O�\���̔j��
	if (m_pDebugProc != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pDebugProc->Uninit();

		// �������̊J��
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	// �p�[�e�B�N���̔j��
	if (m_pParticle != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pParticle->Uninit();

		// �������̊J��
		delete m_pParticle;
		m_pParticle = NULL;
	}

	// �v���C���[�̔j��
	if (m_pPlayer != NULL)
	{// �������̊m�ۂ��o���Ă�����

		m_pPlayer = NULL;
	}

	// �w�i�̔j��
	if (m_pBg != NULL)
	{// �������̊m�ۂ��o���Ă�����

		m_pBg = NULL;
	}

	// �}�b�v�̔j��
	if (m_pMap != NULL)
	{// �������̊m�ۂ��o���Ă�����

		m_pMap = NULL;
	}

	// X�t�@�C���̔j��
	if (m_pXLoad != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pXLoad->Uninit();

		// �������̊J��
		delete m_pXLoad;
		m_pXLoad = NULL;
	}

	// �G�f�B�b�g�̔j��
	if (m_pEdit != NULL)
	{// �������̊m�ۂ��o���Ă�����

		m_pEdit = NULL;
	}

	//**********************************
	// �S�Ẵe�N�X�`���j��
	//**********************************
	if (m_pTexture != NULL)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pTexture->Unload();

		// �������̊J��
		delete m_pTexture;
		m_pTexture = NULL;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CManager::Update(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	//if (pInputKeyboard->GetTrigger(DIK_F3) == true)
	//{// F3�Ń��C���[�t���[���؂�ւ�
	//	m_bWireframe = m_bWireframe ? false : true;
	//}

	if (pInputKeyboard->GetTrigger(DIK_F2) == true)
	{// F2�ŃG�f�B�b�g�؂�ւ�

		m_editType = (EDITTYPE)((m_editType + 1) % EDITTYPE_SETUP);
	}

	if (pInputKeyboard->GetTrigger(DIK_F4) == true)
	{// F4�ŃZ�b�g�A�b�v�ɂ�����
		m_editType = EDITTYPE_SETUP;
	}

	// �L�[�{�[�h�̍X�V����
	m_pInputKeyboard->Update();

	// �Q�[���p�b�h�̍X�V����
	m_pInputGamepad->Update();

	// �}�E�X�̍X�V����
	m_pInputMouse->Update();

	// �����_���[�̍X�V����
	m_pRenderer->Update();

	// ���C�g�̍X�V����
	m_pLight->Update();

	// �J�����̍X�V����
	m_pCamera->Update();

	// �f�o�b�O�\���̍X�V����
	m_pDebugProc->Update();

	// �p�[�e�B�N���̍X�V����
	m_pParticle->Update();

	if (CManager::GetEditType() != CManager::EDITTYPE_PLAY)
	{// �G�f�B�b�g��ԂȂ�
		edit::Update();
	}
	else
	{
		if (pInputKeyboard->GetTrigger(DIK_F7) == true)
		{// F7�������ꂽ,�Ǐ]�؂�ւ�

			m_pCamera->EnableChase();
		}
	}

	if (m_editType == EDITTYPE_SETUP)
	{// �Z�b�g�A�b�v���
		m_pPlayer->SetSphereCollider();
	}

	//// �E�B���h�E
	//ImGui::Begin("ChangeType");
	//{
	//	if (ImGui::BeginMenu("File"))
	//	{
	//		const char* names[] = { "Edit", "Play", "SetUp" };
	//		int selectedIndex = 0;

	//		for (int i = 0; i < IM_ARRAYSIZE(names); i++)
	//		{
	//			if (ImGui::MenuItem(names[i]))
	//			{
	//				selectedIndex = i;
	//				m_editType = static_cast<CManager::EDITTYPE>(selectedIndex);
	//			}
	//		}
	//		ImGui::EndMenu();
	//	}
	//}
	//ImGui::End();


}

//==========================================================================
// �`�揈��
//==========================================================================
void CManager::Draw(void)
{
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
// �����_���[�̎擾
//==========================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//==========================================================================
// �L�[�{�[�h�̎擾
//==========================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//==========================================================================
// �Q�[���p�b�h�̎擾
//==========================================================================
CInputGamepad *CManager::GetInputGamepad(void)
{
	return m_pInputGamepad;
}

//==========================================================================
// �}�E�X�̎擾
//==========================================================================
CInputMouse *CManager::GetInputMouse(void)
{
	return m_pInputMouse;
}

//==========================================================================
// �T�E���h�̎擾
//==========================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}

//==========================================================================
// �f�o�b�O�\���̎擾
//==========================================================================
CDebugProc *CManager::GetDebugProc(void)
{
	return m_pDebugProc;
}

//==========================================================================
// �p�[�e�B�N���̎擾
//==========================================================================
CParticle *CManager::GetParticle(void)
{
	return m_pParticle;
}

//==========================================================================
// �v���C���[�̎擾
//==========================================================================
CPlayer *CManager::GetPlayer(void)
{
	return m_pPlayer;
}

//==========================================================================
// �v���C���[�̎擾
//==========================================================================
CPlayer **CManager::GetPlayer(int n)
{
	return &m_pPlayer;
}

//==========================================================================
// �X�R�A�̎擾
//==========================================================================
CScore *CManager::GetScore(void)
{
	return m_pScore;
}

//==========================================================================
// �^�C�}�[�̎擾
//==========================================================================
CTimer *CManager::GetTimer(void)
{
	return m_pTimer;
}

//==========================================================================
// �w�i�̎擾
//==========================================================================
CBG *CManager::GetBg(void)
{
	return m_pBg;
}

//==========================================================================
// ���C�g�̎擾
//==========================================================================
CLight *CManager::GetLight(void)
{
	return m_pLight;
}

//==========================================================================
// �J�����̎擾
//==========================================================================
CCamera *CManager::GetCamera(void)
{
	return m_pCamera;
}

//==========================================================================
// �e�N�X�`���̎擾
//==========================================================================
CTexture *CManager::GetTexture(void)
{
	return m_pTexture;
}

//==========================================================================
// X�t�@�C���̎擾
//==========================================================================
CXLoad *CManager::GetXLoad(void)
{
	return m_pXLoad;
}

//==========================================================================
// �G�f�B�b�g�̎擾
//==========================================================================
CEdit *CManager::GetEdit(void)
{
	return m_pEdit;
}

//==========================================================================
// 3D�I�u�W�F�N�g�̎擾
//==========================================================================
CElevation *CManager::GetObject3DMesh(void)
{
	return m_pObject3DMesh;
}

//==========================================================================
// �G�}�l�[�W���̎擾
//==========================================================================
CEnemyManager *CManager::GetEnemyManager(void)
{
	return m_pEnemyManager;
}

//==========================================================================
// ���[�V�����t�@�C�����ǂݍ���
//==========================================================================
std::string LoadMotionFileName(void)
{
	FILE *pFile = NULL;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen("data\\TEXT\\filename.txt", "r");

	if (pFile == NULL)
	{//�t�@�C�����J���Ȃ�������
		return NULL;
	}

	char aComment[MAX_COMMENT];	// �ǂݍ��ݗp
	std::string FileName;		// �t�@�C����

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// ���f�����̐ݒ�
		if (strcmp(aComment, "MOTION_FILENAME") == 0)
		{// MOTION_FILENAME��������

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�
			fscanf(pFile, "%s", &aComment[0]);	// �t�@�C����

			// �t�@�C�����ۑ�
			FileName = aComment;
		}

		if (strcmp(aComment, "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���
			break;
		}
	}

	return FileName;
}
