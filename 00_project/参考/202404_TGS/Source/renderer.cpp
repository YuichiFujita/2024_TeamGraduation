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
#include "calculation.h"
#include "map_ui.h"
#include "lostrssmanager.h"

namespace
{
	const D3DXCOLOR ALPHACOLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	const D3DXCOLOR NONE_ALPHACOLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	const D3DXVECTOR2 NORMALSIZE = D3DXVECTOR2(640.0f, 360.0f);
	const D3DXVECTOR2 MINISIZE = D3DXVECTOR2(640.0f, 360.0f) * 1.0f;
	const float FULLSCREEN_MINISIZE_RATIO = 1.0f;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CRenderer::CRenderer()
{
	m_pD3D = nullptr;			// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	m_pD3DDevice = nullptr;		// Direct3D�f�o�C�X�ւ̃|�C���^
	m_bResetWait = false;		// ���Z�b�g�҂���ԉ���
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

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�i�E�B���h�E�j
	ZeroMemory(&m_d3dppWindow, sizeof(m_d3dppWindow));							// �p�����[�^�̃[���N���A
	m_d3dppWindow.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	m_d3dppWindow.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	m_d3dppWindow.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	m_d3dppWindow.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	m_d3dppWindow.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	m_d3dppWindow.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	m_d3dppWindow.AutoDepthStencilFormat = D3DFMT_D24S8;					// �f�o�C�X�o�b�t�@�Ƃ���16bit���g��
	m_d3dppWindow.Windowed = bWindow;									// �E�B���h�E���[�h
	m_d3dppWindow.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	m_d3dppWindow.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��
	m_d3dppWindow.hDeviceWindow = hWnd;

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�i�t���X�N�j
	ZeroMemory(&m_d3dppFull, sizeof(m_d3dppFull));
	//<��ʃT�C�Y�͌�Őݒ�>
	m_d3dppFull.BackBufferFormat = d3ddm.Format;		//�o�b�N�o�b�t�@�̌`��
	m_d3dppFull.BackBufferCount = 1;					//�o�b�N�o�b�t�@�̐�
	m_d3dppFull.SwapEffect = D3DSWAPEFFECT_DISCARD;		//�_�u���o�b�t�@�̐؂�ւ��i�����j
	m_d3dppFull.EnableAutoDepthStencil = TRUE;			//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	m_d3dppFull.AutoDepthStencilFormat = D3DFMT_D24S8;	//�f�v�X�o�b�t�@�Ƃ���24�r�b�g�A�X�e���V���o�b�t�@�Ƃ���8�r�b�g�g�p
	m_d3dppFull.Windowed = TRUE;						//�E�B���h�E���[�h
	m_d3dppFull.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	m_d3dppFull.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//�C���^�[�o��
	m_d3dppFull.hDeviceWindow = hWnd;

	// �f�B�X�v���C�T�C�Y�擾
	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	int displayWidth = desktop.right;
	int displayHeight = desktop.bottom;

	// �f�B�X�v���C�ƃQ�[���̃A�X�y�N�g��v�Z
	float displayAspect = (float)displayWidth / (float)displayHeight;
	float gameAspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	if (displayAspect > gameAspect)
	{//����
		m_d3dppFull.BackBufferWidth = displayWidth * ((float)SCREEN_HEIGHT / (float)displayHeight);			//��ʃT�C�Y�i���j
		m_d3dppFull.BackBufferHeight = SCREEN_HEIGHT;		//��ʃT�C�Y�i�����j
	}
	else
	{//�c���E�ς��Ȃ�
		m_d3dppFull.BackBufferWidth = SCREEN_WIDTH;			//��ʃT�C�Y�i���j
		m_d3dppFull.BackBufferHeight = displayHeight * ((float)SCREEN_WIDTH / (float)displayWidth);		//��ʃT�C�Y�i�����j
	}

	//�f�B�X�v���C���[�h�ݒ�
	m_dispMode = (bWindow == TRUE) ? CRenderer::DISPLAYMODE::MODE_WINDOW : CRenderer::DISPLAYMODE::MODE_FULLSCREEN;

	// �f�B�X�v���C���[�h�ݒ�
	SetDisplayMode(m_dispMode);
	m_bResetWait = false;

	// Direct3D�f�o�C�X�̐���
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&m_d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �����_���[�X�e�[�g�ݒ�
	ResetRendererState();

	// �}���`�^�[�Q�b�g�����_���[�̏�����
	InitMTRender();

	CLostResourceManager::GetInstance()->Regist(this);

	return S_OK;
}

//==========================================================================
// �����_���[�X�e�[�g�ݒ�
//==========================================================================
void CRenderer::ResetRendererState()
{
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
}


//==========================================================================
// �}���`�^�[�Q�b�g�����_���[�̏�����
//==========================================================================
void CRenderer::InitMTRender()
{
	// �ۑ��p�o�b�t�@
	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;

	// ���݂̉�ʕ��擾
	float screen_width = m_d3dpp.BackBufferWidth;
	float screen_height = m_d3dpp.BackBufferHeight;
	
	// �����_�����O�^�[�Q�b�g�p�e�N�X�`���̐���
	for (int i = 0; i < 2; i++)
	{
		m_pD3DDevice->CreateTexture(screen_width, screen_height,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_X8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_Multitarget.pTextureMT[i],
			NULL);


		// �e�N�X�`�������_�����O�p�C���^�[�t�F�[�X�̐���
		m_Multitarget.pTextureMT[i]->GetSurfaceLevel(0, &m_Multitarget.pRenderMT[i]);
	}

	// �e�N�X�`�������_�����O�pZ�o�b�t�@�̐���
	m_pD3DDevice->CreateDepthStencilSurface(
		screen_width, screen_height,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_Multitarget.pZBuffMT,
		NULL);

	// ���݂̃����_�����O�^�[�Q�b�g���擾(�ۑ�)
	m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

	// ���݂�Z�o�b�t�@���擾(�ۑ�)
	m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

	// �����_�����O�^�[�Q�b�g�𐶐������e�N�X�`���ɐݒ�
	m_pD3DDevice->SetRenderTarget(0, m_Multitarget.pRenderMT[0]);

	// Z�o�b�t�@�𐶐�����Z�o�b�t�@�ɐݒ�
	m_pD3DDevice->SetDepthStencilSurface(m_Multitarget.pZBuffMT);

	// �����_�����O�^�[�Q�b�g�p�̃e�N�X�`���̃N���A
	m_pD3DDevice->Clear(
		0, nullptr,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0);


	// �����_�����O�^�[�Q�b�g�����ɖ߂�
	m_pD3DDevice->SetRenderTarget(0, pRenderDef);

	// Z�o�b�t�@�����ɖ߂�
	m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);

	// ���u�������_�����O�^�[�Q�b�g�EZ�o�b�t�@�̊J���i��������Ȃ��ƃ��������[�N���N����j
	if (pRenderDef != nullptr)
	{
		pRenderDef->Release();
		pRenderDef = nullptr;
	}
	if (pZBuffDef != nullptr)
	{
		pZBuffDef->Release();
		pZBuffDef = nullptr;
	}

	// �e�N�X�`���p�r���[�|�[�g�̐ݒ�
	m_Multitarget.viewportMT.X = 0;						// �`�悷���ʂ̍���X���W
	m_Multitarget.viewportMT.Y = 0;						// �`�悷���ʂ̍���Y���W
	m_Multitarget.viewportMT.Width = screen_width;		// �`�悷���ʂ̕�
	m_Multitarget.viewportMT.Height = screen_height;	// �`�悷���ʂ̍���
	m_Multitarget.viewportMT.MinZ = 0.0f;
	m_Multitarget.viewportMT.MaxZ = 1.0f;


	m_pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_Multitarget.pVtxBuff,
		nullptr);



	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_Multitarget.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �X�N���[���\����Ԃɉ����Ēʏ�̃T�C�Y�ύX
	D3DXVECTOR2 size;
	if (m_dispMode == CRenderer::DISPLAYMODE::MODE_WINDOW)
	{
		size = NORMALSIZE;
	}
	else if (m_dispMode == CRenderer::DISPLAYMODE::MODE_FULLSCREEN)
	{
		// ���݂̉�ʕ��擾
		float screen_width = m_d3dpp.BackBufferWidth;
		float screen_height = m_d3dpp.BackBufferHeight;

		size = D3DXVECTOR2(screen_width * 0.5f, screen_height * 0.5f);
	}
	

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = (screen_width * 0.5f) - size.x;
	pVtx[0].pos.y = (screen_height * 0.5f) - size.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (screen_width * 0.5f) + size.x;
	pVtx[1].pos.y = (screen_height * 0.5f) - size.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (screen_width * 0.5f) - size.x;
	pVtx[2].pos.y = (screen_height * 0.5f) + size.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (screen_width * 0.5f) + size.x;
	pVtx[3].pos.y = (screen_height * 0.5f) + size.y;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = ALPHACOLOR;
	pVtx[1].col = ALPHACOLOR;
	pVtx[2].col = ALPHACOLOR;
	pVtx[3].col = ALPHACOLOR;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N���b�N
	m_Multitarget.pVtxBuff->Unlock();

}

//==========================================================================
// �I������
//==========================================================================
void CRenderer::Uninit()
{
	for (int cnt = 0; cnt < 2; cnt++)
	{
		if (m_Multitarget.pTextureMT[cnt] != nullptr)
		{
			m_Multitarget.pTextureMT[cnt]->Release();
			m_Multitarget.pTextureMT[cnt] = nullptr;
		}
		if (m_Multitarget.pRenderMT[cnt] != nullptr)
		{
			m_Multitarget.pRenderMT[cnt]->Release();
			m_Multitarget.pRenderMT[cnt] = nullptr;
		}
	}

	if (m_Multitarget.pZBuffMT != nullptr)
	{
		m_Multitarget.pZBuffMT->Release();
		m_Multitarget.pZBuffMT = nullptr;
	}

	if (m_Multitarget.pVtxBuff != nullptr)
	{
		m_Multitarget.pVtxBuff->Release();
		m_Multitarget.pVtxBuff = nullptr;
	}

	// Direct3D�f�o�C�X�̔j��
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	CLostResourceManager::GetInstance()->Remove(this);
}

//==========================================================================
// �X�V����
//==========================================================================
void CRenderer::Update()
{
	// �S�Ă̍X�V
	CObject::UpdateAll();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CRenderer::Draw()
{
	static bool bDisp = true;

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

//#if _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_F1))
	{// �`��؂�ւ�
		bDisp = bDisp ? false : true;
	}
//#endif

	// ��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	m_pD3DDevice->Clear
	(	0, nullptr,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0
	);

	// �J�����̐ݒ�
	CCamera* pCamerea = CManager::GetInstance()->GetCamera();

	// �ؑ֗p
	LPDIRECT3DTEXTURE9 pTextureWk;	// �ؑ֗p�p�e�N�X�`��
	LPDIRECT3DSURFACE9 pRenderWk;	// �e�N�X�`�������_�����O�p�C���^�[�t�F�[�X

	// �`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// �`�悪���������Ƃ�

		// �t�H�O������
		MyFog::DisableFog(m_pD3DDevice);

		// �t�H�O�L��
		MyFog::SetFog();

		D3DVIEWPORT9 viewportDef;	//�r���[�|�[�g�ۑ�

		// ���݂̃r���[�|�[�g���擾
		m_pD3DDevice->GetViewport(&viewportDef);

		if (CManager::GetInstance()->IsLoadComplete())
		{

			// �ۑ��p�o�b�t�@
			LPDIRECT3DSURFACE9 pRenderDef = nullptr, pZBuffDef = nullptr;
			D3DXMATRIX mtxView, mtxProjection;

			// �f�t�H���g�̃����_���[�^�[�Q�b�g���擾
			GetDefaultRenderTarget(&pRenderDef, &pZBuffDef, &mtxView, &mtxProjection);


			// �^�[�Q�b�g�ؑ�
			CManager::GetInstance()->GetRenderer()->ChangeTarget(pCamerea->GetPositionV(), pCamerea->GetPositionR(), MyLib::Vector3(0.0f, 1.0f, 0.0f));

			// �e�N�X�`��[0]�̃N���A
			m_pD3DDevice->Clear(
				0, nullptr,
				(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
				D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
				1.0f,
				0);

			// �S�Ă̕`��
			CObject::DrawAll();

			// �t�B�[�h�o�b�N�G�t�F�N�g�Ƀe�N�X�`��[1]��\��t���ĕ`��
			float multi = UtilFunc::Correction::EasingLinear(m_MultitargetInfo.fMulti, m_MultitargetInfo.fStartMulti, m_MultitargetInfo.fTimer);
			float alpha = UtilFunc::Correction::EasingLinear(m_MultitargetInfo.fStartColAlpha, m_MultitargetInfo.fColAlpha, m_MultitargetInfo.fTimer);
			// �X�N���[���\����Ԃɉ����Ēʏ�̃T�C�Y�ύX
			if (m_dispMode == CRenderer::DISPLAYMODE::MODE_WINDOW)
			{
				DrawMultiTargetScreen(1, D3DXCOLOR(ALPHACOLOR.r, ALPHACOLOR.g, ALPHACOLOR.b, alpha), MINISIZE * multi);
			}
			else if (m_dispMode == CRenderer::DISPLAYMODE::MODE_FULLSCREEN)
			{
				// ���݂̉�ʕ��擾
				float screen_width = m_d3dpp.BackBufferWidth;
				float screen_height = m_d3dpp.BackBufferHeight;
				D3DXVECTOR2 screen_size = D3DXVECTOR2(screen_width * 0.5f, screen_height * 0.5f) * FULLSCREEN_MINISIZE_RATIO;

				DrawMultiTargetScreen(1, D3DXCOLOR(ALPHACOLOR.r, ALPHACOLOR.g, ALPHACOLOR.b, alpha), screen_size * multi);
			}

			// �J�����̐ݒ�
			CManager::GetInstance()->GetCamera()->SetCamera();
			// �����_�[�^�[�Q�b�g�����Ƃɖ߂�
			CManager::GetInstance()->GetRenderer()->ChangeRendertarget(pRenderDef, pZBuffDef, mtxView, mtxProjection);

			// �X�N���[���\����Ԃɉ����Ēʏ�̃T�C�Y�ύX
			if (m_dispMode == CRenderer::DISPLAYMODE::MODE_WINDOW)
			{
				DrawMultiTargetScreen(0, NONE_ALPHACOLOR, NORMALSIZE);
			}
			else if (m_dispMode == CRenderer::DISPLAYMODE::MODE_FULLSCREEN)
			{
				// ���݂̉�ʕ��擾
				float screen_width = m_d3dpp.BackBufferWidth;
				float screen_height = m_d3dpp.BackBufferHeight;

				DrawMultiTargetScreen(0, NONE_ALPHACOLOR, D3DXVECTOR2(screen_width * 0.5f, screen_height * 0.5f));
			}

			// �e�N�X�`��0��1�̐ؑ�
			pTextureWk = m_Multitarget.pTextureMT[0];
			m_Multitarget.pTextureMT[0] = m_Multitarget.pTextureMT[1];
			m_Multitarget.pTextureMT[1] = pTextureWk;

			pRenderWk = m_Multitarget.pRenderMT[0];
			m_Multitarget.pRenderMT[0] = m_Multitarget.pRenderMT[1];
			m_Multitarget.pRenderMT[1] = pRenderWk;

			// ���u�������_�����O�^�[�Q�b�g�EZ�o�b�t�@�̊J���i��������Ȃ��ƃ��������[�N���N����j
			if (pRenderDef != nullptr)
			{
				pRenderDef->Release();
				pRenderDef = nullptr;
			}
			if (pZBuffDef != nullptr)
			{
				pZBuffDef->Release();
				pZBuffDef = nullptr;
			}

			// �f�o�b�O�\���̕`�揈��
			CManager::GetInstance()->GetDebugProc()->Draw();

			// �e�L�X�g�̐ݒ�
			CDebugProc::SetText();

			// �|�[�Y�`�揈��
			if (bDisp)
			{
				if (CManager::GetInstance()->GetPause() != nullptr)
				{
					CManager::GetInstance()->GetPause()->Draw();
				}
			}

		}
		else
			// ���[�h�}�l�[�W���̍X�V
		{
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

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ��i�����Ƀf�o�C�X���X�g���m�j
	HRESULT hr = m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
	if (hr == D3DERR_DEVICELOST || m_bResetWait)
	{
		ResetDevice();
	}

	// �}���`�^�[�Q�b�g����
	if (m_MultitargetInfo.bActive) {
		SetMultiTarget();
	}
}

//==========================================================================
// �}���`�^�[�Q�b�g��ʂ̕`��
//==========================================================================
void CRenderer::DrawMultiTargetScreen(int texIdx, const D3DXCOLOR& col, const D3DXVECTOR2& size)
{
	// ���݂̉�ʕ��擾
	float screen_width = m_d3dpp.BackBufferWidth;
	float screen_height = m_d3dpp.BackBufferHeight;

	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_Multitarget.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = (screen_width * 0.5f) - size.x;
	pVtx[0].pos.y = (screen_height * 0.5f) - size.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (screen_width * 0.5f) + size.x;
	pVtx[1].pos.y = (screen_height * 0.5f) - size.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (screen_width * 0.5f) - size.x;
	pVtx[2].pos.y = (screen_height * 0.5f) + size.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (screen_width * 0.5f) + size.x;
	pVtx[3].pos.y = (screen_height * 0.5f) + size.y;
	pVtx[3].pos.z = 0.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N���b�N
	m_Multitarget.pVtxBuff->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	m_pD3DDevice->SetStreamSource(0, m_Multitarget.pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	m_pD3DDevice->SetTexture(0, m_Multitarget.pTextureMT[texIdx]);

	// �|���S���̕`��
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================================
// �f�o�C�X�̎擾
//==========================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice() const
{
	return m_pD3DDevice;
}

HRESULT CRenderer::SetFullScreen()
{
	m_pD3DDevice->Reset(&m_d3dpp);

	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h

	// Direct3D�f�o�C�X�̔j��
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}


	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));							// �p�����[�^�̃[���N���A

	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	m_d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	m_d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	m_d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�o�C�X�o�b�t�@�Ƃ���16bit���g��
	m_d3dpp.Windowed = TRUE;										// �E�B���h�E���[�h
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��


	ResetWnd();

	// Direct3D�f�o�C�X�̐���
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		GetWnd(),
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&m_d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			GetWnd(),
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				GetWnd(),
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// ����\��ݒ�
	timeBeginPeriod(1);

	// �E�C���h�E�̕\��
	ShowWindow(GetWnd(), GetCmbShow());		// �E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(GetWnd());				// �N���C�A���g�̈���X�V

	return S_OK;
}

//==========================================================================
// �f�t�H���g�̃����_�[�^�[�Q�b�g�擾
//==========================================================================
void CRenderer::GetDefaultRenderTarget(LPDIRECT3DSURFACE9* pRender, LPDIRECT3DSURFACE9* pZBuff, D3DXMATRIX* viewport, D3DXMATRIX* projection)
{
	// ���݂̃����_�����O�^�[�Q�b�g���擾(�ۑ�)
	m_pD3DDevice->GetRenderTarget(0, &(*pRender));

	// ���݂�Z�o�b�t�@���擾(�ۑ�)
	m_pD3DDevice->GetDepthStencilSurface(&(*pZBuff));

	m_pD3DDevice->GetTransform(D3DTS_VIEW, &(*viewport));
	m_pD3DDevice->GetTransform(D3DTS_PROJECTION, &(*projection));
}

//==========================================================================
// �����_�����O�^�[�Q�b�g�̐ؑ�
//==========================================================================
void CRenderer::ChangeRendertarget(LPDIRECT3DSURFACE9 pRender, LPDIRECT3DSURFACE9 pZBuff, D3DXMATRIX viewport, D3DXMATRIX projection)
{
	// �����_�����O�^�[�Q�b�g�𐶐������e�N�X�`���ɐݒ�
	m_pD3DDevice->SetRenderTarget(0, pRender);

	// Z�o�b�t�@�𐶐�����Z�o�b�t�@�ɐݒ�
	m_pD3DDevice->SetDepthStencilSurface(pZBuff);

	// �e�N�X�`�������_�����O�p�̃r���[�|�[�g��ݒ�
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &viewport);

	// �e�N�X�`�������_�����O�p�̃v���W�F�N�V�����}�g���b�N�X��ݒ�
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &projection);
}

//==========================================================================
// �����_�����O�^�[�Q�b�g�̐ؑ�
//==========================================================================
void CRenderer::ChangeTarget(MyLib::Vector3 posV, MyLib::Vector3 posR, MyLib::Vector3 vecU)
{
	D3DXMATRIX mtxview, mtxProjection;

	// ���݂̉�ʕ��擾
	float screen_width = m_d3dpp.BackBufferWidth;
	float screen_height = m_d3dpp.BackBufferHeight;

	// �����_�����O�^�[�Q�b�g�𐶐������e�N�X�`���ɐݒ�
	m_pD3DDevice->SetRenderTarget(0, m_Multitarget.pRenderMT[0]);

	// Z�o�b�t�@�𐶐�����Z�o�b�t�@�ɐݒ�
	m_pD3DDevice->SetDepthStencilSurface(m_Multitarget.pZBuffMT);

	// �e�N�X�`�������_�����O�p�̃r���[�|�[�g��ݒ�
	m_pD3DDevice->SetViewport(&m_Multitarget.viewportMT);


	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(
		&mtxProjection,
		D3DXToRadian(45.0f),	// ����p
		(float)screen_width / (float)screen_height,	// �A�X�y�N�g��
		10.0f,		// ��O�̐���
		100000.0f);	// ���s���̐���

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxview);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(
		&mtxview,
		&posV,	// ���_
		&posR,	// �����_
		&vecU);	// ������x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxview);
}

//==========================================================================
// �}���`�^�[�Q�b�g��ʂ̕`�攻��
//==========================================================================
void CRenderer::SetEnableDrawMultiScreen(float fGoalAlpha, float fGoalMulti, float fTimer)
{ 
	// �p�����[�^�[�̐ݒ�
	m_MultitargetInfo.fTimer = 0.0f;
	if (fTimer != 0.0f){ m_MultitargetInfo.fAddTimer = 1.0f / fTimer; }	// 1�t���[�����Ƃ̉��Z�ʐݒ�
	else { m_MultitargetInfo.fAddTimer = 1.0f; }	// 0�Ȃ瑦
	m_MultitargetInfo.fStartColAlpha = m_MultitargetInfo.fColAlpha;
	m_MultitargetInfo.fColAlpha = fGoalAlpha;
	m_MultitargetInfo.fStartMulti = m_MultitargetInfo.fMulti;
	m_MultitargetInfo.fMulti = fGoalMulti;
	m_MultitargetInfo.bActive = true;

	return;
}

//==========================================================================
// �}���`�^�[�Q�b�g��ʂ̕`�撲��
//==========================================================================
void CRenderer::SetMultiTarget()
{
	m_MultitargetInfo.fTimer += m_MultitargetInfo.fAddTimer;

	if (m_MultitargetInfo.fTimer >= 1.0f) { 
		m_MultitargetInfo.bActive = false; 
	}
}

//=================================
// �f�B�X�v���C���[�h�ݒ�
//=================================
void CRenderer::SetDisplayMode(DISPLAYMODE mode)
{
	// ���[�h�ݒ�
	D3DDISPLAYMODE d3ddm;
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));

	// ���݂̃X�N���[�����[�h���擾
	m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	//��ʃT�C�Y�擾
	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);

	// ���[�h�ɉ����ăv���[���e�[�V�����p�����[�^�̐ݒ�
	if (mode == CRenderer::DISPLAYMODE::MODE_WINDOW)
	{// �E�B���h�E���[�h
		m_d3dpp = m_d3dppWindow;

		// �E�B���h�E�X�^�C���E�ʒu�T�C�Y�ύX
		SetWindowLong(GetWnd(), GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(GetWnd(), HWND_TOP, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOZORDER | SWP_FRAMECHANGED);

		// �ĕ`��̋���
		ShowWindow(GetWnd(), SW_SHOWNORMAL);
		UpdateWindow(GetWnd());
	}
	else if (mode == CRenderer::DISPLAYMODE::MODE_FULLSCREEN)
	{// �t���X�N���[�����[�h
		m_d3dpp = m_d3dppFull;

		//��ʃT�C�Y�擾
		RECT desktop;
		GetWindowRect(GetDesktopWindow(), &desktop);

		// �E�B���h�E�X�^�C���E�ʒu�T�C�Y�ύX
		SetWindowLong(GetWnd(), GWL_STYLE, WS_POPUP | WS_VISIBLE);
		SetWindowPos(GetWnd(), HWND_TOP, 0, 0, desktop.right, desktop.bottom, SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	m_bResetWait = true;
	m_dispMode = mode;
}

//=================================
// �f�o�C�X���Z�b�g
//=================================
void CRenderer::ResetDevice()
{
	// ���\�[�X����������
	CLostResourceManager::GetInstance()->Backup();

	// �f�o�C�X���Z�b�g
	HRESULT hr = m_pD3DDevice->TestCooperativeLevel();
	if (hr == S_OK || hr == D3DERR_DEVICENOTRESET)
	{
		hr = m_pD3DDevice->Reset(&m_d3dpp);
		if (FAILED(hr))
		{
			if (hr == D3DERR_INVALIDCALL)
			{
				assert(false);
			}

			return;
		}

		// �����_���[�X�e�[�g�ݒ�
		ResetRendererState();

		// ���\�[�X�Đ���
		CLostResourceManager::GetInstance()->Restore();
		m_bResetWait = false;
	}
}

//========================
// �o�b�N�A�b�v
//========================
void CRenderer::Backup()
{
	for (int cnt = 0; cnt < 2; cnt++)
	{
		if (m_Multitarget.pTextureMT[cnt] != nullptr)
		{
			m_Multitarget.pTextureMT[cnt]->Release();
			m_Multitarget.pTextureMT[cnt] = nullptr;
		}
		if (m_Multitarget.pRenderMT[cnt] != nullptr)
		{
			m_Multitarget.pRenderMT[cnt]->Release();
			m_Multitarget.pRenderMT[cnt] = nullptr;
		}
	}

	if (m_Multitarget.pZBuffMT != nullptr)
	{
		m_Multitarget.pZBuffMT->Release();
		m_Multitarget.pZBuffMT = nullptr;
	}

	if (m_Multitarget.pVtxBuff != nullptr)
	{
		m_Multitarget.pVtxBuff->Release();
		m_Multitarget.pVtxBuff = nullptr;
	}
}

//========================
// ����
//========================
void CRenderer::Restore()
{
	InitMTRender();
}