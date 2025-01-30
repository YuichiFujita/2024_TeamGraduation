//==========================================================================
// 
//  �Q�[�����|�[�Y���� [pause_game.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "pause_game.h"
#include "manager.h"
#include "fade.h"
#include "gamemanager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\pause\\retry.png";	// �e�N�X�`���t�@�C��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPause_Game::CPause_Game()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPause_Game::~CPause_Game()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPause_Game::Init()
{
	// ����������
	CPause::Init();

	// �e�N�X�`���؂�ւ�
	int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);
	m_aObject2D[VTX::VTX_RETRY]->BindTexture(nTexIdx);

	return S_OK;
}

//==========================================================================
// �X�V����
//==========================================================================
void CPause_Game::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �L�[�{�[�h���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();

	// �Q�[���p�b�h���擾
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (CManager::GetInstance()->GetFade()->GetState() == CFade::STATE_NONE
	&&  CGameManager::GetInstance()->GetType() == CGameManager::ESceneType::SCENE_MAIN)
	{
		if (pKey->GetTrigger(DIK_P) ||
			pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_START).bInput)
		{// �|�[�Y
			m_bPause = !m_bPause;

			// �T�E���h�̍Đ�
			PLAY_SOUND(CSound::ELabel::LABEL_SE_DECIDE_00);
		}
	}

	// �e�̍X�V
	CPause::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���[�h�ʌ��菈��
//==========================================================================
void CPause_Game::DecideByMode()
{
	// �T�E���h�Đ�
	//CSound::GetInstance()->PlaySound(CSound::LABEL_SE_DONE);

	// �Q�[�������g���C����
	CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_GAME);
}
