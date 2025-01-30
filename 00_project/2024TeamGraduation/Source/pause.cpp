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
#include "pause_outgame.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE[] =	// �e�N�X�`���t�@�C��
	{
		"",
		"",
		"data\\TEXTURE\\pause\\resume.png",
		"data\\TEXTURE\\pause\\retry.png",
		"data\\TEXTURE\\pause\\end.png",
	};
	const float DIS_POSY = 130.0f;	// �I�����̊Ԋu
	const float POSY = VEC3_SCREEN_CENT.y;
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

	case CScene::MODE::MODE_TUTORIAL:
		pFade = DEBUG_NEW CPause_Tutorial;
		break;

	default:
		pFade = DEBUG_NEW CPause_OutGame;
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

		// �e�N�X�`���̊��蓖��
		int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE[nCntVtx]);

		// �e�N�X�`���̊��蓖��
		m_aObject2D[nCntVtx]->BindTexture(nTexIdx);

		if (nCntVtx == VTX_FADE)
		{// �����̎�
			m_aObject2D[nCntVtx]->SetSize(D3DXVECTOR2(640.0f, 360.0f));					// �T�C�Y
			m_aObject2D[nCntVtx]->SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));	// �ʒu
			m_aObject2D[nCntVtx]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.4f));			// �F�ݒ�
		}
		else if (nCntVtx == VTX_WINDOW)
		{// �E�B���h�E�̎�
			m_aObject2D[nCntVtx]->SetSize(D3DXVECTOR2(640.0f, 360.0f));					// �T�C�Y
			m_aObject2D[nCntVtx]->SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));	// �ʒu
			m_aObject2D[nCntVtx]->SetColor(MyLib::color::Black(0.4f));			// �F�ݒ�
		}
		else
		{// �I����

			// ������ɃT�C�Y�ݒ�
			D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nTexIdx);
			size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
			m_aObject2D[nCntVtx]->SetSize(size);	// �T�C�Y
			m_aObject2D[nCntVtx]->SetPosition(MyLib::Vector3(640.0f, POSY + ((nCntVtx - VTX_RETRY) * DIS_POSY), 0.0f));	// �ʒu
		}
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPause::Uninit()
{
	
}

//==========================================================================
// �폜
//==========================================================================
void CPause::Kill()
{
	
}

//==========================================================================
// �X�V����
//==========================================================================
void CPause::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �`��ݒ�
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_aObject2D[nCntVtx] == nullptr) continue;

		m_aObject2D[nCntVtx]->SetEnableDisp(m_bPause);
	}

	if (!m_bPause)
	{// �|�[�Y������Ȃ�������
		m_nSelect = MENU::MENU_RETURNGAME;
		return;
	}

	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// �t�F�[�h������Ȃ���
		return;
	}

	// �_�Ŏ��ԍX�V
	m_fFlashTime += fDeltaTime * fSlowRate;

	// �F�X�V
	UpdateColor();

	// �I�����X�V
	UpdateSelect();

	// �|�[�Y���Ȃ̂ŗ�O�I�ɍX�V
	// �X�V����
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_aObject2D[nCntVtx] != nullptr)
		{
			// �X�V����
			m_aObject2D[nCntVtx]->Update(fDeltaTime, fDeltaRate, fSlowRate);

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
		{// ����
			col = MyLib::color::White();
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
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();

	// �Q�[���p�b�h���擾
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_W) ||
		pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_UP).bInput ||
		pPad->GetAllLStickTrigger(CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_UP))
	{// ��n�������ꂽ

		// �p�^�[��No.���X�V
		m_nSelect = (m_nSelect + (MENU_MAX - 1)) % MENU_MAX;

		// �T�E���h�̍Đ�
		PLAY_SOUND(CSound::ELabel::LABEL_SE_CURSOR);
	}
	else if (pKey->GetTrigger(DIK_S) ||
			 pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_DOWN).bInput ||
			 pPad->GetAllLStickTrigger(CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_DOWN))
	{// ���n�������ꂽ

		// �p�^�[��No.���X�V
		m_nSelect = (m_nSelect + 1) % MENU_MAX;

		// �T�E���h�̍Đ�
		PLAY_SOUND(CSound::ELabel::LABEL_SE_CURSOR);
	}


	if (pKey->GetTrigger(DIK_RETURN) || 
		pPad->GetAllTrigger(CInputGamepad::BUTTON_A).bInput)
	{// ���肪�����ꂽ

		Decide();

		// �T�E���h�̍Đ�
		PLAY_SOUND(CSound::ELabel::LABEL_SE_DECIDE_00);
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
		SetPause(false);
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
	
}

