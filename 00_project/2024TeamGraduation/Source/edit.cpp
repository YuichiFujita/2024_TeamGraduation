//=============================================================================
// 
//  エディタ―処理 [edit.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "edit.h"
#include "manager.h"
#include "calculation.h"

// 派生クラス
#include "edit_map.h"
#include "edit_obstacle.h"
#include "edit_course.h"
#include "edit_waterstone.h"
#include "edit_judgezone.h"
#include "edit_mapblock.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CEdit::CEdit()
{
	// 値のクリア
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CEdit::~CEdit()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CEdit* CEdit::Create(CGame::EditType type)
{
	// 生成用インスタンス
	CEdit* pMarker = nullptr;

	switch (type)
	{
	case CGame::EDITTYPE_OFF:
		return nullptr;
		break;

	case CGame::EDITTYPE_MAP:
		pMarker = DEBUG_NEW CEdit_Map;
		break;

	case CGame::EditType::EDITTYPE_OBSTACLE:
		pMarker = DEBUG_NEW CEdit_Obstacle;
		break;

	case CGame::EditType::EDITTYPE_COURSE:
		pMarker = DEBUG_NEW CEdit_Course;
		break;

	case CGame::EditType::EDITTYPE_WATERSTONE:
		pMarker = DEBUG_NEW CEdit_WaterStone;
		break;

	case CGame::EditType::EDITTYPE_JUDGEZONE:
		pMarker = DEBUG_NEW CEdit_JudgeZone;
		break;

	default:
		break;
	}

	if (pMarker != nullptr)
	{
		// 初期化処理
		pMarker->Init();
	}

	return pMarker;
}


//==========================================================================
// 終了処理
//==========================================================================
//void CEdit::Uninit()
//{
//	delete this;
//}