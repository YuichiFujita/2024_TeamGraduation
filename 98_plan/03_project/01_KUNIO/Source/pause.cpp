//=============================================================================
// 
//  �|�[�Y���� [pause.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "pause.h"
#include "object2D.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "calculation.h"

// �h���N���X
#include "pause_game.h"
#include "pause_tutorial.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE[] =	// �e�N�X�`���t�@�C��
	{
		"",
		"data\\TEXTURE\\pause\\window.png",
		"data\\TEXTURE\\pause\\resume.png",
		"data\\TEXTURE\\pause\\retry.png",
		"data\\TEXTURE\\pause\\end.png",
	};
	const float DIS_POSY = 130.0f;	// �I�����̊Ԋu
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPause::CPause()
{
	// �l�̃N���A
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		m_aObject2D[nCntVtx] = nullptr;	// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	}
	m_bPause = false;	// �|�[�Y�̔���
	m_nSelect = 0;									// �I����
	m_fFlashTime = 0.0f;	// �_�Ŏ���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPause::~CPause()
{

}

//==========================================================================
// ��������
//==========================================================================
CPause* CPause::Create(CScene::MODE mode)
{
	// �������m��
	CPause* pFade = nullptr;

	switch (mode)
	{
	case CScene::MODE_GAME:
		pFade = DEBUG_NEW CPause_Game;
		break;

	case CScene::MODE_GAMETUTORIAL:
		pFade = DEBUG_NEW CPause_Tutorial;
		break;

	default:
		pFade = DEBUG_NEW CPause_Game;
		break;
	}

	if (pFade != nullptr)
	{
		// ����������
		if (FAILED(pFade->Init()))
		{
			return nullptr;
		}
	}

	return pFade;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPause::Init()
{
	// ��������
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		// ��������
		m_aObject2D[nCntVtx] = CObject2D::Create(7);

		if (m_aObject2D[nCntVtx] == nullptr)
		{// ���s���Ă�����
			return E_FAIL;
		}
		m_aObject2D[nCntVtx]->SetType(CObject::TYPE::TYPE_NONE);

		// �e�N�X�`���̊��蓖��
		int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE[nCntVtx]);

		// �e�N�X�`���̊��蓖��
		m_aObject2D[nCntVtx]->BindTexture(nTexIdx);

		if (nCntVtx == VTX_FADE)
		{// �����̎�
			m_aObject2D[nCntVtx]->SetSize(D3DXVECTOR2(640.0f, 360.0f));				// �T�C�Y
			m_aObject2D[nCntVtx]->SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));	// �ʒu
			m_aObject2D[nCntVtx]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.4f));		// �F�ݒ�
		}
		else if (nCntVtx == VTX_WINDOW)
		{// �E�B���h�E�̎�
			m_aObject2D[nCntVtx]->SetSize(D3DXVECTOR2(640.0f, 360.0f));				// �T�C�Y
			m_aObject2D[nCntVtx]->SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));	// �ʒu
			m_aObject2D[nCntVtx]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// �F�ݒ�
		}
		else
		{// �I����

			D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nTexIdx);
			size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
			m_aObject2D[nCntVtx]->SetSize(size);	// �T�C�Y
			m_aObject2D[nCntVtx]->SetPosition(MyLib::Vector3(640.0f, 430.0f + ((nCntVtx - VTX_RETRY) * DIS_POSY), 0.0f));	// �ʒu
		}
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPause::Uninit()
{
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_aObject2D[nCntVtx] != nullptr)
		{
			// �I������
			m_aObject2D[nCntVtx]->Uninit();
			delete m_aObject2D[nCntVtx];
			m_aObject2D[nCntVtx] = nullptr;
		}
	}
}

//==========================================================================
// �폜
//==========================================================================
void CPause::Kill()
{
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_aObject2D[nCntVtx] != nullptr)
		{
			// �I������
			m_aObject2D[nCntVtx]->Uninit();
			m_aObject2D[nCntVtx] = nullptr;
		}
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CPause::Update()
{
	if (m_bPause == false)
	{// �|�[�Y������Ȃ�������
		return;
	}

	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// �t�F�[�h������Ȃ���
		return;
	}

	// �_�Ŏ��ԍX�V
	m_fFlashTime += CManager::GetInstance()->GetDeltaTime();

	// �F�X�V
	UpdateColor();

	// �I�����X�V
	UpdateSelect();

	// �X�V����
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_aObject2D[nCntVtx] != nullptr)
		{
			// �X�V����
			m_aObject2D[nCntVtx]->Update();

			// ���_���W�X�V
			m_aObject2D[nCntVtx]->SetVtx();
		}
	}
}

//==========================================================================
// �F�X�V
//==========================================================================
void CPause::UpdateColor()
{
	for (int i = VTX_CONTINUE; i < VTX_MAX; i++)
	{
		D3DXCOLOR col = m_aObject2D[i]->GetColor();
		if (m_nSelect + VTX_CONTINUE == i)
		{
			col = UtilFunc::Transformation::HSVtoRGB(0.0f, 0.0f, 0.7f + fabsf(sinf(D3DX_PI * (m_fFlashTime / 1.0f)) * 0.3f));
		}
		else
		{
			// �����ۂ�����
			col = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
		}
		m_aObject2D[i]->SetColor(col);
	}
}

//==========================================================================
// �I�����X�V
//==========================================================================
void CPause::UpdateSelect()
{
	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	// �Q�[���p�b�h���擾
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	if (pInputKeyboard->GetTrigger(DIK_W) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
		(pInputGamepad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pInputGamepad->GetStickMoveL(0).y > 0))
	{// ��n�������ꂽ

		// �p�^�[��No.���X�V
		m_nSelect = (m_nSelect + (MENU_MAX - 1)) % MENU_MAX;

		// �T�E���h�Đ�
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_CURSOR_MOVE);
	}
	else if (pInputKeyboard->GetTrigger(DIK_S) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON_DOWN, 0) ||
		(pInputGamepad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pInputGamepad->GetStickMoveL(0).y < 0))
	{// ���n�������ꂽ

		// �p�^�[��No.���X�V
		m_nSelect = (m_nSelect + 1) % MENU_MAX;

		// �T�E���h�Đ�
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_CURSOR_MOVE);
	}


	if (pInputKeyboard->GetTrigger(DIK_RETURN) || 
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{// ���肪�����ꂽ

		Decide();
	}
	
}

//==========================================================================
// ���菈��
//==========================================================================
void CPause::Decide()
{
	switch (m_nSelect)
	{
	case MENU_RETURNGAME:
		SetPause();
		break;

	case MENU_RETRY:

		// ���[�h�ʌ��菈��
		DecideByMode();
		break;

	case MENU_RETURNTITLE:

		// �T�E���h�Đ�

		// �^�C�g���ɖ߂�
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
		break;
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CPause::Draw()
{
	if (m_bPause == false)
	{// �|�[�Y������Ȃ�������
		return;
	}

	// �`�揈��
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_aObject2D[nCntVtx] != nullptr)
		{
			// �`�揈��
			m_aObject2D[nCntVtx]->Draw();
		}
	}
}

//==========================================================================
// �|�[�Y�̐ݒ�
//==========================================================================
void CPause::SetPause()
{
	// �g�p�󋵕ύX
	m_bPause = m_bPause ? false : true;
	m_nSelect = 0;									// �I����
}

//==========================================================================
// �g�p�󋵎擾
//==========================================================================
bool CPause::IsPause()
{
	return m_bPause;
}
