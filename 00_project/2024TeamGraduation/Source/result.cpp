//=============================================================================
// 
//  ���U���g���� [result.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "player.h"
#include "sound.h"
#include "game.h"
#include "resultmanager.h"

#include "blackframe.h"
#include "camera.h"

//=============================================================================
// �萔��`
//=============================================================================
namespace
{
	const D3DXCOLOR WIN_COLOR = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	const D3DXCOLOR LOSE_COLOR = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	const float START_LENGTH = 300.0f; // ��������
	const float END_LENGTH = 2000.0f; // �ڕW����

	//���f���̔z�u�ʒu
	const MyLib::Vector3 CENTER = MyLib::Vector3(0.0f, 0.0f, -2000.0f);
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CResult* CResult::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CResult::CResult() : m_pResultManager(nullptr)
{
	// �l�̃N���A
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CResult::~CResult()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CResult::Init()
{
	//�v���C���[�������Z�b�g
	CManager::GetInstance()->SetNumPlayer(0);

	// ����������
	if (FAILED(CScene::Init()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	// ���U���g�}�l�[�W������
	SAFE_UNINIT(m_pResultManager);
	m_pResultManager = CResultManager::Create();

	// BGM�Đ�
	CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_RESULT);

	// ����
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CResult::Uninit()
{
	SAFE_UNINIT(m_pResultManager);

	// �I������
	CScene::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CResult::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �V�[���̍X�V����
	CScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	if (m_pResultManager != nullptr)
	{
		m_pResultManager->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// �C���v�b�g���擾
	CInputKeyboard * pKey = CInputKeyboard::GetInstance();
	CInputGamepad *pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pKey->GetTrigger(DIK_RETURN))
	{
		// �J��
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_TITLE);
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CResult::Draw()
{

}

