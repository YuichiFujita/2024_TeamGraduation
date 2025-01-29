//=============================================================================
// 
//  �`���[�g���A������ [tutorial.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "calculation.h"
#include "sound.h"
#include "MyEffekseer.h"
#include "camera.h"
#include "tutorialscreen.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CTutorial* CTutorial::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTutorial::CTutorial() : 
	m_pTutorialScreen	(nullptr)	// �`���[�g���A�����
{
	// �l�̃N���A
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTutorial::~CTutorial()
{

}

//==========================================================================
// ��������
//==========================================================================
CTutorial* CTutorial::Create()
{
	if (m_pThisPtr == nullptr)
	{// �܂��������Ă��Ȃ�������

		// �C���X�^���X����
		m_pThisPtr = DEBUG_NEW CTutorial;
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTutorial::Init()
{
	// ����������
	if (FAILED(CScene::Init()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	// ��ʐ���
	m_pTutorialScreen = CTutorialScreen::Create();
	if (m_pTutorialScreen == nullptr) return E_FAIL;

	// ����
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTutorial::Uninit()
{
	m_pThisPtr = nullptr;

	// �I������
	CScene::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTutorial::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �t�F�[�h���̏ꍇ������
	if (GET_MANAGER->GetFade()->GetState() != CFade::STATE_NONE) { return; }

	// ���͏��擾
	CInputGamepad* pPad = CInputGamepad::GetInstance();
	if (pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_A).bInput ||
		pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_B).bInput ||
		pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_START).bInput)
	{
		// �J��
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_TITLE);
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTutorial::Draw()
{

}
