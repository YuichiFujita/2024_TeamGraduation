//==========================================================================
// 
//  �A�E�g�Q�[�����|�[�Y���� [pause_outgame.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "pause_outgame.h"
#include "manager.h"
#include "fade.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPause_OutGame::CPause_OutGame()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPause_OutGame::~CPause_OutGame()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPause_OutGame::Init()
{
	// ����������
	CPause::Init();

	return S_OK;
}

//==========================================================================
// �X�V����
//==========================================================================
void CPause_OutGame::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �L�[�{�[�h���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();

	// �Q�[���p�b�h���擾
	CInputGamepad* pPad = CInputGamepad::GetInstance();

#if _DEBUG
	if (CManager::GetInstance()->GetFade()->GetState() == CFade::STATE_NONE)
	{
		if (pKey->GetTrigger(DIK_P) ||
			pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_START, 0))
		{// �|�[�Y
			m_bPause = !m_bPause;
		}
	}
#endif // _DEBUG

	// �e�̍X�V
	CPause::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���[�h�ʌ��菈��
//==========================================================================
void CPause_OutGame::DecideByMode()
{
	// ���g���C����
	CManager::GetInstance()->GetFade()->SetFade(CManager::GetInstance()->GetMode());
}
