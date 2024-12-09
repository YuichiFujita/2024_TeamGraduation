//=============================================================================
// 
//  �����_���[���� [renderer.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "debugproc.h"
#include "camera.h"
#include "fade.h"
#include "instantfade.h"
#include "blackframe.h"
#include "pause.h"
#include "input.h"
#include "loadmanager.h"
#include "Imguimanager.h"
#include "fog.h"
#include "renderTexture.h"
#include "renderTextureManager.h"
#include "screen.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CRenderer::CRenderer() :
	m_pRenderTextureManager	(nullptr),	// �����_�[�e�N�X�`���}�l�[�W���[
	m_pRenderScene	(nullptr),	// �V�[�������_�[�e�N�X�`��
	m_pDrawScreen	(nullptr),	// �X�N���[���`��|���S��
	m_pDefSurScreen	(nullptr),	// ���̕`��T�[�t�F�C�X�ۑ��p
	m_pD3D			(nullptr),	// Direct3D�I�u�W�F�N�g
	m_pD3DDevice	(nullptr)	// Direct3D�f�o�C�X
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CRenderer::~CRenderer()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�������[�h

	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// �p�����[�^�̃[���N���A
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				// �[�x�o�b�t�@�F24bit, �X�e���V���o�b�t�@�F8bit���g�p
	d3dpp.Windowed = bWindow;									// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// Direct3D�f�o�C�X�̐���
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// �����̎��ݒ�
	srand((unsigned int)time(0));

	// �����_�[�e�N�X�`���}�l�[�W���[�̐���
	m_pRenderTextureManager = CRenderTextureManager::Create();
	if (m_pRenderTextureManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CRenderer::Uninit()
{
	// �����_�[�e�N�X�`���̔j��
	SAFE_REF_RELEASE(m_pRenderScene);

	// �X�N���[���`��|���S���̏I��
	SAFE_UNINIT(m_pDrawScreen);

	// ���̃X�N���[���`��T�[�t�F�C�X�̔j��
	SAFE_RELEASE(m_pDefSurScreen);

	// Direct3D�f�o�C�X�̔j��
	SAFE_RELEASE(m_pD3DDevice);

	// Direct3D�I�u�W�F�N�g�̔j��
	SAFE_RELEASE(m_pD3D);
}

//==========================================================================
// �X�V����
//==========================================================================
void CRenderer::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �S�Ă̍X�V
	CObject::UpdateAll(fDeltaTime, fDeltaRate, fSlowRate);

	// �X�N���[���`��|���S���̍X�V
	assert(m_pDrawScreen != nullptr);
	m_pDrawScreen->Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CRenderer::Draw()
{
	CManager* pManager = GET_MANAGER;			// �}�l�[�W���[
	CCamera*  pCamera = pManager->GetCamera();	// �J����
	HRESULT   hr;	// �ُ�I���̊m�F�p

	if (CManager::GetInstance()->IsLoadComplete())
	{ // ���[�h�ς݂̏ꍇ

		// �S�����_�[�e�N�X�`���̍쐬
		m_pRenderTextureManager->Draw();
	}

	// �h��Ԃ����[�h��ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// �|���S����h��Ԃ�

	// �X�N���[���`��T�[�t�F�C�X�����ɖ߂�
	hr = m_pD3DDevice->SetRenderTarget(0, m_pDefSurScreen);
	assert(SUCCEEDED(hr));

	// �o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
	m_pD3DDevice->Clear
	(
		0,
		nullptr,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0
	);
	assert(SUCCEEDED(hr));

	// �`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// �`�悪���������Ƃ�

		// �t�H�O������
		MyFog::DisableFog(m_pD3DDevice);

		// �t�H�O�L��
		MyFog::SetFog();

		// ���݂̃r���[�|�[�g���擾
		D3DVIEWPORT9 viewportDef;	//�r���[�|�[�g�ۑ�
		m_pD3DDevice->GetViewport(&viewportDef);

		if (CManager::GetInstance()->IsLoadComplete())
		{ // ���[�h�ς݂̏ꍇ

			// �J�����̐ݒ�
			assert(pCamera != nullptr);
			pCamera->SetCamera();

			// �X�N���[���`��|���S���̕`��
			assert(m_pDrawScreen != nullptr);
			m_pDrawScreen->Draw();

			// �f�o�b�O�\���̕`�揈��
			CManager::GetInstance()->GetDebugProc()->Draw();

			// �e�L�X�g�̐ݒ�
			CDebugProc::SetText();
		}
		else
		{ // ���[�h���̏ꍇ

			// ���[�h�}�l�[�W���̕`��
			GetLoadManager()->Draw();
		}

		// �J�ڂȂ��t�F�[�h�`�揈��
		CInstantFade* pfalsefade = CManager::GetInstance()->GetInstantFade();
		if (pfalsefade != nullptr)
		{
			pfalsefade->Draw();
		}

		// �t�F�[�h�`�揈��
		CFade* pfade = CManager::GetInstance()->GetFade();
		if (pfade != nullptr)
		{
			pfade->Draw();
		}

		// �r���[�|�[�g�����ɖ߂�
		m_pD3DDevice->SetViewport(&viewportDef);

		// �`��I��
		m_pD3DDevice->EndScene();
	}

	// Imgui�̕`��
	ImguiMgr::Draw();

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//============================================================
//	�����_�[�e�N�X�`���[��������
//============================================================
HRESULT CRenderer::CreateRenderTexture()
{
	// ���̃X�N���[���`��T�[�t�F�C�X��ۑ�
	HRESULT hr = m_pD3DDevice->GetRenderTarget(0, &m_pDefSurScreen);
	if (FAILED(hr))
	{ // �T�[�t�F�C�X�擾�Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �V�[�������_�[�e�N�X�`���̐���
	m_pRenderScene = CRenderTexture::Create(CRenderTextureManager::LAYER_MAIN, &CObject::DrawAll);
	if (m_pRenderScene == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �X�N���[���`��|���S���̐���
	m_pDrawScreen = CScreen::Create(m_pRenderScene->GetTextureIndex());
	if (m_pDrawScreen == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	�����_�[�e�N�X�`���`�揈��
//============================================================
void CRenderer::DrawRenderTexture(LPDIRECT3DSURFACE9* pSurface, ADrawFunc pDrawFunc)
{
	CManager *pManager = GET_MANAGER;			// �}�l�[�W���[
	CCamera  *pCamera  = pManager->GetCamera();	// �J����

	D3DVIEWPORT9 viewportDef;	// �J�����̃r���[�|�[�g�ۑ��p
	HRESULT hr;	// �ُ�I���̊m�F�p

	if (GET_MANAGER->IsWireframe())
	{ // ���C���[�t���[����ON�̏ꍇ

		// �h��Ԃ����[�h��ݒ�
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	// ���C���[�t���[��
	}

	// �`��T�[�t�F�C�X���X�N���[���`��T�[�t�F�C�X�ɕύX
	hr = m_pD3DDevice->SetRenderTarget(0, *pSurface);
	assert(SUCCEEDED(hr));

	// �o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
	hr = m_pD3DDevice->Clear
	(
		0,
		nullptr,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0
	);
	assert(SUCCEEDED(hr));

	// �e�N�X�`���쐬�p�̕`��
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{ // �`��J�n�����������ꍇ

		// ���݂̃r���[�|�[�g���擾
		m_pD3DDevice->GetViewport(&viewportDef);

		// �J�����̐ݒ�
		assert(pCamera != nullptr);
		pCamera->SetCamera();

		// �I�u�W�F�N�g�̑S�`��
		assert(pDrawFunc != nullptr);
		pDrawFunc();

		// �r���[�|�[�g�����ɖ߂�
		m_pD3DDevice->SetViewport(&viewportDef);

		// �`��I��
		hr = m_pD3DDevice->EndScene();
		assert(SUCCEEDED(hr));
	}

	// �h��Ԃ����[�h�����ɖ߂�
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// �|���S����h��Ԃ�
}

//============================================================
//	�X�N���[���V�F�[�_�[�t���O�̐ݒ菈��
//============================================================
void CRenderer::SetEnableShader(const bool bShader)
{
	// �V�F�[�_�[�t���O�̐ݒ�
	m_pDrawScreen->SetEnableShader(bShader);
}

//============================================================
//	�X�N���[���V�F�[�_�[�t���O�̎擾����
//============================================================
bool CRenderer::IsShader() const
{
	// �V�F�[�_�[�t���O�̎擾
	return m_pDrawScreen->IsShader();
}

//============================================================
//	�����_�[�e�N�X�`���C���f�b�N�X�̎擾����
//============================================================
int CRenderer::GetRenderTextureIndex() const
{
	// �����_�[�e�N�X�`���C���f�b�N�X�̎擾
	return m_pRenderScene->GetTextureIndex();
}
