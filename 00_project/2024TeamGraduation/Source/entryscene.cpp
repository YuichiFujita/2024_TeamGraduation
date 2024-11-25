//=============================================================================
// 
// �G���g���[�V�[������ [entryscene.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "entryscene.h"
#include "manager.h"

// �V�[��
#include "entry_setupTeam.h"
#include "entry_dressup.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEntryScene::CEntryScene()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEntryScene::~CEntryScene()
{

}

//==========================================================================
// ��������
//==========================================================================
CEntryScene* CEntryScene::Create(CEntry::ESceneType sceneType)
{
	// �������̊m��
	CEntryScene* pScene = nullptr;

	switch (sceneType)
	{
	case CEntry::SCENETYPE_SETUPTEAM:
		pScene = DEBUG_NEW CEntry_SetUpTeam;
		break;

	case CEntry::SCENETYPE_DRESSUP:
		pScene = DEBUG_NEW CEntry_Dressup;
		break;

	case CEntry::SCENETYPE_GAMESETTING:
		break;

	default:
		break;
	}

	if (pScene != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pScene->Init()))
		{ // �������Ɏ��s�����ꍇ
			SAFE_UNINIT(pScene);
			return nullptr;
		}
	}

	return pScene;
}

//==========================================================================
// �X�V����
//==========================================================================
void CEntryScene::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	Debug();
}
