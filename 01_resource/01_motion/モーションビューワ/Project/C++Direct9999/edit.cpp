//=============================================================================
// 
//  エディット処理 [edit.cpp]
//  Author : 相馬靜雅
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
// 更新処理
//==========================================================================
void edit::Update(void)
{
	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// 動かす
	CManager::GetPlayer()->SetPartsRot();
}