//=============================================================================
// 
//  �Q�[������ [game.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "fade.h"
#include "camera.h"
#include "pause.h"
#include "texture.h"

#include "input.h"
#include "map.h"
#include "sound.h"

#include "playerUser.h"
#include "playerAI.h"
#include "ball.h"
#include "particle.h"
#include "myeffekseer.h"
#include "edit_map.h"
#include "objectLine.h"

#include "2D_Effect.h"
#include "controlkeydisp.h"

namespace
{
	const float RATIO_SETGOAL = 0.825f;	// �S�[���ݒu�̊���
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CGame* CGame::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CGame::CGame()
{
	// �l�̃N���A
	m_pGameManager = nullptr;		// �Q�[���}�l�[�W���̃I�u�W�F�N�g
	m_EditType = EDITTYPE_OFF;		// �G�f�B�b�g�̎��
	m_bEdit = false;				// �G�f�B�b�g�̔���
	m_clear = false;				// �N���A����
	m_pEdit = nullptr;				// �G�f�B�^�[
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CGame::~CGame()
{

}

//==========================================================================
// ��������
//==========================================================================
CGame* CGame::Create(CScene::MODE mode)
{
	if (m_pThisPtr == nullptr)
	{// �܂��������Ă��Ȃ�������

		// �C���X�^���X����
		switch (mode)
		{
		case CScene::MODE_GAME:
			m_pThisPtr = DEBUG_NEW CGame;
			break;
		}
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CGame::Init()
{
	// �G�f�B�b�g����OFF
	m_bEdit = false;

	// �v���C���[�̐��ݒ�
	CManager::GetInstance()->SetNumPlayer(1);

	// ����������
	if (FAILED(CScene::Init()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	//**********************************
	// �Q�[���}�l�[�W��
	//**********************************
	m_pGameManager = CGameManager::Create(GetMode());

	// ���[�h�ʏ���������
	InitByMode();

	//**********************************
	// �v���C���[
	//**********************************
	// �{�[������
	CBall::Create(MyLib::Vector3(0.0f, 1000.0f, 0.0f));

	// �v���C���[User����
	CPlayerUser* pUser = CPlayerUser::Create(CGameManager::SIDE_LEFT, MyLib::Vector3(-500.0f, 0.0f, 0.0f));
	if (pUser == nullptr)
	{
		return E_FAIL;
	}

	// �v���C���[AI����
	for (int i = 0; i < 1; i++)
	{
		CPlayerAI* pAI = CPlayerAI::Create(CGameManager::SIDE_RIGHT, MyLib::Vector3(500.0f, 0.0f, 0.0f));
		if (pAI == nullptr)
		{
			return E_FAIL;
		}
	}

	// �J�����̃��Z�b�g
	CManager::GetInstance()->GetCamera()->Reset(CScene::MODE_GAME);

	// �N���A�̔���
	SetEnableClear(true);

	// BGM�Đ�
	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_BGM_GAME);

	return S_OK;
}

//==========================================================================
// ���[�h�ʏ�����
//==========================================================================
void CGame::InitByMode()
{

}

//==========================================================================
// �I������
//==========================================================================
void CGame::Uninit()
{
	m_pThisPtr = nullptr;

	// �G�f�B�b�g�̔p��
	if (m_pEdit != nullptr)
	{
		m_pEdit->Uninit();
		m_pEdit = nullptr;
	}

	if (m_pGameManager != nullptr)
	{
		// �I������
		m_pGameManager->Uninit();
		delete m_pGameManager;
		m_pGameManager = nullptr;
	}


	// �I������
	CScene::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CGame::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �Q�[���}�l�[�W��
	if (m_pGameManager != nullptr)
	{
		// �X�V����
		m_pGameManager->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();


#if _DEBUG
	// �G�f�B�b�g�؂�ւ�����
	ChangeEdit();
#endif

#if _DEBUG
	// ����
	if (ImGui::TreeNode("Create"))
	{
		
		// �c���[�I�[
		ImGui::TreePop();
	}
#endif

	// �V�[���̍X�V
	CScene::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �G�f�B�b�g�؂�ւ�����
//==========================================================================
void CGame::ChangeEdit()
{
	static bool no_titlebar = false;
	static bool no_scrollbar = true;
	static bool no_menu = true;
	static bool no_move = true;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = true;
	static bool unsaved_document = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;

	// �G�f�B�b�g���j���[
	ImGui::Begin("Edit", 0, window_flags);
	{
		if (ImGui::CollapsingHeader("Window options"))
		{
			if (ImGui::BeginTable("split", 3))
			{
				ImGui::TableNextColumn(); ImGui::Checkbox("No titlebar", &no_titlebar);
				ImGui::TableNextColumn(); ImGui::Checkbox("No scrollbar", &no_scrollbar);
				ImGui::TableNextColumn(); ImGui::Checkbox("No menu", &no_menu);
				ImGui::TableNextColumn(); ImGui::Checkbox("No move", &no_move);
				ImGui::TableNextColumn(); ImGui::Checkbox("No resize", &no_resize);
				ImGui::TableNextColumn(); ImGui::Checkbox("No collapse", &no_collapse);
				ImGui::TableNextColumn(); ImGui::Checkbox("No close", &no_close);
				ImGui::TableNextColumn(); ImGui::Checkbox("No nav", &no_nav);
				ImGui::TableNextColumn(); ImGui::Checkbox("No background", &no_background);
				ImGui::TableNextColumn(); ImGui::Checkbox("No bring to front", &no_bring_to_front);
				ImGui::TableNextColumn(); ImGui::Checkbox("Unsaved document", &unsaved_document);
				ImGui::EndTable();
			}
		}

		// �e�L�X�g
		static const char* items[] = { "OFF", "Map", "Obstacle", "Course", "WaterStone", "JudgeZone"};
		int selectedItem = m_EditType;

		// [�O���[�v]�G�f�B�b�g�؂�ւ�
		if (ImGui::CollapsingHeader("Change Edit Mode"))
		{
			// [���W�I�{�^��]���[�V�����؂�ւ�
			for (int i = 0; i < EditType::EDITTYPE_MAX; i++)
			{
				if (ImGui::RadioButton(items[i], &selectedItem, i))
				{
					if (i != 0)
					{
						// �f�o�b�O���[�h
						m_pGameManager->SetType(CGameManager::SceneType::SCENE_DEBUG);
					}
					else
					{
						// �f�o�b�O���[�h
						m_pGameManager->SetType(m_pGameManager->GetOldType());
					}

					// �G�f�B�b�g�I��
					EditReset();
					m_EditType = static_cast<EditType>(selectedItem);

					// ����
					if (m_pEdit != nullptr)
					{
						m_pEdit->Uninit();
						m_pEdit = nullptr;
					}

					m_pEdit = CEdit::Create(m_EditType);
				}
			}
		}

		if (m_pEdit != nullptr) {
			m_pEdit->Update();
		}

	}
	ImGui::End();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CGame::Draw()
{

}

//==========================================================================
// �Q�[���}�l�[�W���̎擾
//==========================================================================
CGameManager *CGame::GetGameManager()
{
	return m_pGameManager;
}

//==========================================================================
// �G�f�B�^�[���Z�b�g����
//==========================================================================
void CGame::EditReset()
{
	if (m_pEdit != nullptr)
	{
		m_pEdit->Uninit();
		m_pEdit = nullptr;
	}
}
