//=============================================================================
// 
//  �G�f�B�b�g���� [edit.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "edit.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "input.h"
#include "player.h"

//==========================================================================
// �X�V����
//==========================================================================
void edit::Update(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ������
	CManager::GetPlayer()->SetPartsRot();
}