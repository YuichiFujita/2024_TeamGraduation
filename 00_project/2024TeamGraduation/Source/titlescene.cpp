//=============================================================================
// 
// �^�C�g���V�[������ [titlescene.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "titlescene.h"
#include "manager.h"

// �V�[��
#include "title_susuru.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitleScene::CTitleScene()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTitleScene::~CTitleScene()
{

}

//==========================================================================
// ��������
//==========================================================================
CTitleScene* CTitleScene::Create(CTitle::ESceneType sceneType)
{
	// �������̊m��
	CTitleScene* pScene = nullptr;

	switch (sceneType)
	{
	case CTitle::ESceneType::SCENETYPE_SUSURU:	// SUSURU�V�[��
		pScene = DEBUG_NEW CTitle_SUSURU;
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
void CTitleScene::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}
