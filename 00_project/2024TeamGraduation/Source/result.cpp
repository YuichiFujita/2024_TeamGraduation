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
CResultScore *CResult::m_pResultScore = nullptr;	// ���U���g�X�N���[���̃I�u�W�F�N�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CResult::CResult() : m_clear(false)
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
	m_pResultScore = nullptr;

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

//==========================================================================
// ���U���g�X�N���[���̏��擾
//==========================================================================
CResultScore *CResult::GetResultScore()
{
	return m_pResultScore;
}

