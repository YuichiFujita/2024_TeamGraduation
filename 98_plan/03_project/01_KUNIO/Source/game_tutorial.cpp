//=============================================================================
// 
//  �Q�[������ [game_tutorial.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "manager.h"
#include "game_tutorial.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "fade.h"
#include "camera.h"
#include "input.h"

#include "player.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CGameTutorial::CGameTutorial()
{
	// �l�̃N���A

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CGameTutorial::~CGameTutorial()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CGameTutorial::Init()
{
	
	// ����������
	CGame::Init();
	
	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;

	// ����
	return S_OK;
}

//==========================================================================
// ���[�h�ʏ�����
//==========================================================================
void CGameTutorial::InitByMode()
{
	
}

//==========================================================================
// �I������
//==========================================================================
void CGameTutorial::Uninit()
{
	// �I������
	CGame::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CGameTutorial::Update()
{
	// �X�V
	CGame::Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CGameTutorial::Draw()
{
	// �`��
	CGame::Draw();
}