//=============================================================================
// 
//  ���C������ [main.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include <stdio.h>
#include "main.h"
#include "manager.h"
#include "MyEffekseer.h"
#include "loadmanager.h"



//==========================================================================
// �v���g�^�C�v�錾
//==========================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void LoadInBackground();

//==========================================================================
// �O���[�o���ϐ��錾
//==========================================================================
int g_nCountFPS;
CLoadManager* LoadManager = nullptr;
HWND hWnd;	// �E�C���h�E�n���h��(���ʎq)
HINSTANCE g_hInstance;
RECT g_Rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// ��ʃT�C�Y�̍\����
int g_CmbShow;

//==========================================================================
// ���C���֐�
//==========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmbLine, int nCmbShow)
{
	g_hInstance = hInstance;
	g_CmbShow = nCmbShow;

	// ���������[�N���o��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					// WNDCLASS�̃������T�C�Y
		CS_CLASSDC,							// �E�C���h�E�̃X�^�C��
		WindowProc,							// �E�C���h�E�v���V�[�W��
		0,									// 0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,									// 0�ɂ���(�ʏ�͎g�p���Ȃ�)
		g_hInstance,							// �C���X�^���X�n���h��
		LoadIcon(nullptr, IDI_APPLICATION),	// �^�X�N�o�[�̃A�C�R��
		LoadCursor(nullptr, IDC_ARROW),		// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),			// �N���C�A���g�̈�̔w�i�F
		nullptr,								// ���j���[�o�[
		CLASS_NAME,							// �E�C���h�E�N���X�̖��O
		LoadIcon(nullptr, IDI_APPLICATION)		// �t�@�C���̃A�C�R��
	};

	MSG msg;	// ���b�Z�[�W���i�[����ϐ�

	// �E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&g_Rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�C���h�E�̐���
	ResetWnd();

	DWORD dwCurrentTime;					// ���ݎ���
	DWORD dwExecLastTime;					// �Ō�ɏ�����������
	DWORD dwFrameCount;						// �t���[���J�E���g
	DWORD dwFPSLastTime;					// �Ō��FPS���v����������

	// �����̎��ݒ�
	srand((unsigned int)time(0));

	// �}�l�[�W���̐���
	CManager *pManager = nullptr;

	// �}�l�[�W���̐���
	if (pManager == nullptr)
	{// nullptr��������

		// �������m��
		pManager = CManager::Create();
	}

	if (LoadManager != nullptr)
	{
		LoadManager->Uninit();
		delete LoadManager;
		LoadManager = nullptr;
	}

	// ���[�h�}�l�[�W���̐���
	LoadManager = CLoadManager::Create();

	if (pManager != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		if (FAILED(pManager->Init(g_hInstance, hWnd, TRUE)))
		{// ���������������s�����ꍇ

			return -1;
		}
	}

	g_nCountFPS = 0;

	// ����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;				// ����������
	dwExecLastTime = timeGetTime();	// ���ݎ������擾(�ۑ�)

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, g_CmbShow);		// �E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);				// �N���C�A���g�̈���X�V

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// ���b�Z�[�W���[�v
	while(1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{// Window�̏���
			if (msg.message == WM_QUIT)
			{// WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				// ���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);		// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);		// �E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{// DirectX�̏���

			dwCurrentTime = timeGetTime();	// ���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5�b�o��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;		// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;					// �t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60����1�b�o��

				// �����J�n�̎���[���ݎ���]��ۑ�
				dwExecLastTime = dwCurrentTime;

				if (pManager != nullptr)
				{// �������̊m�ۂ��o���Ă�����

					// �X�V����
					pManager->Update();

					// �`�揈��
					pManager->Draw();
				}
				

				// �t���[���J�E���g�����Z
				dwFrameCount++;
			}
		}
	}

	if (pManager != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		pManager->Uninit();

		// �������̊J��
		delete pManager;
		pManager = nullptr;
	}

	if (LoadManager != nullptr)
	{
		LoadManager->Uninit();
		delete LoadManager;
		LoadManager = nullptr;
	}
	

	// ����\��߂�
	timeEndPeriod(1);

	// �E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);
		
	return (int)msg.wParam;
}

// �o�b�N�O���E���h�ł̃��[�h�������s���֐�
void LoadInBackground()
{
	// �����Ƀ��[�h�����̃R�[�h��ǉ�
	// ��: �e�N�X�`���⃂�f���̓ǂݍ��݂Ȃ�
}

//==========================================================================
// �E�C���h�E�v���V�[�W��
//==========================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;	// �Ԃ�l���i�[

	switch (uMsg)
	{
	case WM_DESTROY:				// �E�C���h�E�j���̃��b�Z�[�W
		// WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:				// �L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:				// ESC�L�[����

			// ���b�Z�[�W�{�b�N�X��\��
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{
				// �E�C���h�E��j������(VK_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:				// �o�c��
		// ���b�Z�[�W�{�b�N�X��\��
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

		if (nID == IDYES)
		{
			// �E�C���h�E��j������(VK_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;			// 0��Ԃ��ďI����h��
		}
		break;

	case WM_LBUTTONDOWN:		// �}�E�X���N���b�N
		// �ΏۃE�C���h�E�Ƀt�H�[�J�X�����킹��
		SetFocus(hWnd);
		break;

	case WM_TIMER:				// �^�C�}�[���s�̃��b�Z�[�W
		break;
	}

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// ����̏�����Ԃ�
}

CLoadManager* GetLoadManager()
{
	return LoadManager;
}

//==========================================================================
// FPS�擾
//==========================================================================
int GetFPS()
{
	return g_nCountFPS;
}

HWND GetWnd() { return hWnd; }

void ResetWnd()
{
	// �E�C���h�E�̐���
	hWnd = CreateWindowEx(
		0,									// �g���E�C���h�E�X�^�C��
		CLASS_NAME,							// �E�C���h�E�N���X�̖��O
		WINDOW_NAME,						// �E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,				// �E�C���h�E�X�^�C��
		CW_USEDEFAULT,						// �E�C���h�E�̍���X���W
		CW_USEDEFAULT,						// �E�C���h�E�̍���Y���W
		(g_Rect.right - g_Rect.left),			// �E�C���h�E�̕�
		(g_Rect.bottom - g_Rect.top),			// �E�C���h�E�̍���
		nullptr,								// �e�E�C���h�E�̃n���h��
		nullptr,								// ���j���[�n���h���܂��͎q�E�C���h�EID
		g_hInstance,							// �C���X�^���X�n���h��
		nullptr								// �E�C���h�E�쐬�f�[�^
	);
	int n = 0;
}

int GetCmbShow()
{
	return g_CmbShow;
}
