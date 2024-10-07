//=============================================================================
// 
//  �G�f�B�^�\���� [edit.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "edit.h"
#include "manager.h"
#include "calculation.h"

// �h���N���X
#include "edit_map.h"
#include "edit_obstacle.h"
#include "edit_course.h"
#include "edit_waterstone.h"
#include "edit_judgezone.h"
#include "edit_mapblock.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEdit::CEdit()
{
	// �l�̃N���A
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEdit::~CEdit()
{

}

//==========================================================================
// ��������
//==========================================================================
CEdit* CEdit::Create(CGame::EditType type)
{
	// �����p�C���X�^���X
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
		// ����������
		pMarker->Init();
	}

	return pMarker;
}


//==========================================================================
// �I������
//==========================================================================
//void CEdit::Uninit()
//{
//	delete this;
//}