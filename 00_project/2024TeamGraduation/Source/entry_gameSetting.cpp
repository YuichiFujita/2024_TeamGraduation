//=============================================================================
// 
// �Q�[���ڍאݒ菈�� [entry_gameSetting.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "entry_gameSetting.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"
#include "fade.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTFILE = "data\\TEXT\\entry\\setupTeam.txt";
	const std::string TOP_LINE = "#==============================================================================";	// �e�L�X�g�̃��C��
	const std::string TEXT_LINE = "#------------------------------------------------------------------------------";	// �e�L�X�g�̃��C��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEntry_GameSetting::CEntry_GameSetting() : CEntryScene()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEntry_GameSetting::~CEntry_GameSetting()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEntry_GameSetting::Init()
{
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEntry_GameSetting::Uninit()
{
	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CEntry_GameSetting::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�̍X�V
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// ��U�V�[���؂�ւ� TODO : �S���G���g���[���Ă���Ƃ��ɂ���
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pKey->GetTrigger(DIK_RETURN))
	{
		// �J��
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_GAME);
	}
}

//==========================================================================
// �f�o�b�O����
//==========================================================================
void CEntry_GameSetting::Debug()
{
	if (ImGui::TreeNode("GameSetting"))
	{
		//=============================
		// �Z�[�u
		//=============================s
		if (ImGui::Button("Save"))
		{
			
		}

		// TODO : ���ԂƂ��ύX����

		ImGui::TreePop();
	}
}
