//=============================================================================
// 
//  �Q�[������ [game.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "manager.h"
#include "game.h"
#include "game_tutorial.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "fade.h"
#include "camera.h"
#include "pause.h"
#include "texture.h"

#include "input.h"
#include "player.h"
#include "score.h"
#include "timer.h"
#include "map.h"
#include "elevation.h"
#include "sound.h"
#include "stage.h"

#include "player.h"
#include "limitarea.h"
#include "particle.h"
#include "myeffekseer.h"
#include "flower_bud.h"
#include "edit_map.h"
#include "objectLine.h"
#include "goalflag.h"
#include "checkpoint.h"
#include "map_obstacleManager.h"
#include "baggageManager.h"
#include "judgezoneManager.h"
#include "stencilshadow.h"

#include "sample_obj3D.h"
#include "course.h"
#include "waterfield.h"
#include "stonewall.h"
#include "stonewall_front.h"

#include "2D_Effect.h"
#include "waterripple.h"
#include "meshbubble.h"
#include "waterstone.h"
#include "map_ui.h"
#include "waterstoneManager.h"
#include "spline.h"
#include "courseManager.h"
#include "peoplemanager.h"
#include "subtitle.h"
#include "receiver_people.h"
#include "splashwater_manager.h"
#include "ascensionCylinder.h"
#include "tree.h"
#include "scroll.h"
#include "suffocation.h"
#include "leaf_flow.h"
#include "controlkeydisp.h"

#if _DEBUG
#include "stagecleartext.h"
#include "goalgametext.h"
#endif
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
	m_pScore = nullptr;				// �X�R�A�̃I�u�W�F�N�g
	m_pTimer = nullptr;				// �^�C�}�[�̃I�u�W�F�N�g
	m_pLimitArea = nullptr;			// �G���A�����̃I�u�W�F�N�g
	m_pStage = nullptr;				// �X�e�[�W�̃I�u�W�F�N�g
	m_pGameManager = nullptr;		// �Q�[���}�l�[�W���̃I�u�W�F�N�g
	m_EditType = EDITTYPE_OFF;		// �G�f�B�b�g�̎��
	m_bEdit = false;				// �G�f�B�b�g�̔���
	m_clear = false;				// �N���A����
	m_fMaxRokOnDistance = 0.0f;		// ���b�N�I���̍ő勗��
	m_pEdit = nullptr;				// �G�f�B�^�[
	m_pObstacleManager = nullptr;	// ��Q���}�l�[�W��
	m_pBaggageManager = nullptr;	// �ו��}�l�[�W��
	m_pCourse = nullptr;			// �R�[�X�̃I�u�W�F�N�g
	m_pCourseManager = nullptr;		// �R�[�X�}�l�[�W���̃I�u�W�F�N�g
	m_pJudgeZoneManager = nullptr;	// ����]�[���}�l�[�W��
	m_pWaterStoneManager = nullptr;	// �����΃}�l�[�W��
	m_pPeopleManager = nullptr;		// �l�}�l�[�W��
	m_pMapUI = nullptr;				// �}�b�vUI
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

		case CScene::MODE::MODE_GAMETUTORIAL:
			m_pThisPtr = DEBUG_NEW CGameTutorial;
			break;
		}
	}

	return m_pThisPtr;
}

//==========================================================================
// �C���X�^���X�擾
//==========================================================================
CGame* CGame::GetInstance()
{
	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CGame::Init()
{
	// �G�f�B�b�g����OFF
	m_bEdit = false;

	// ���b�N�I���̍ő勗��
	m_fMaxRokOnDistance = mylib_const::MAX_ROCKONDISTANCE_GAME;

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

	// �ו��}�l�[�W��
	m_pBaggageManager = CBaggageManager::Create();

	//**********************************
	// �v���C���[
	//**********************************
	// �L��������
	for (int nCntPlayer = 0; nCntPlayer < 1; nCntPlayer++)
	{
		if (CPlayer::Create(nCntPlayer) == nullptr)
		{
			return E_FAIL;
		}
	}

	//=============================
	// �X�e�[�W
	//=============================
	//m_pStage = CStage::Create("data\\TEXT\\stage\\info.txt");

	CManager::GetInstance()->GetCamera()->Reset(CScene::MODE_GAME);

	// �N���A�̔���
	SetEnableClear(true);

	//=============================
	// �^�C�}�[
	//=============================
	m_pTimer = CTimer::Create(CTimer::Type::TYPE_NORMAL);

	//=============================
	// ��Q���}�l�[�W��
	//=============================
	m_pObstacleManager = CMap_ObstacleManager::Create();


	//=============================
	// �R�[�X�}�l�[�W��
	//=============================
	m_pCourseManager = CCourseManager::Create();

	//=============================
	// ��
	//=============================
	MyLib::Vector3 treepos2(-2723.0f, 1500.0f, 12128.0f);
	for (int nCnt = 0; nCnt <= 19; nCnt++)
	{
		CTree::Create(treepos2);
		treepos2.x += 2000.0f;
	}

	MyLib::Vector3 treepos(-2223.0f, 900.0f, 10128.0f);
	for (int nCnt = 0; nCnt <= 20; nCnt++)
	{
		CTree::Create(treepos);
		treepos.x += 2000.0f;
	}


	//=============================
	// �X�e���V���e
	//=============================
	CStencilShadow::Create();

	//CWaterField::Create(CWaterField::TYPE::TYPE_NORMAL);
	CWaterField::Create(CWaterField::TYPE::TYPE_RIGHT);
	CWaterField::Create(CWaterField::TYPE::TYPE_LEFT);

	//CCheckpoint::Load("data\\TEXT\\map\\checkpoint.txt");

	// �`�F�b�N�|�C���g�ʉ߃��Z�b�g
	CCheckpoint::ResetSaveID();

	//=============================
	// �S�[���쐬
	//=============================
	CGoalflagX::Create(m_pCourse->GetCourceLength() * RATIO_SETGOAL);
	//CGoalflagX::Create(m_pCourse->GetCourceLength() * 0.975f);

	//=============================
	// �͂���쐬
	//=============================
	{
		MyLib::Vector3 pos = MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(),
			m_pCourse->GetCourceLength() * RATIO_SETGOAL, 0.0f);
		pos.y = 300.0f;
		pos.x += 3000.0f;
		CReceiverPeople* pReceiverPeople = CReceiverPeople::Create(pos);
		pReceiverPeople->SetState(CReceiverPeople::STATE::STATE_WAIT);
		m_pGameManager->SetReceiverPeople(pReceiverPeople);
	}

	//=============================
	// ����]�[���}�l�[�W��
	//=============================
	/*m_pJudgeZoneManager = CJudgeZoneManager::Create();
	m_pJudgeZoneManager->Load("data\\TEXT\\judgezonelist\\judgezonelist_01.txt");*/

	//=============================
	// �}�b�vUI�쐬
	//=============================
	m_pMapUI = CMapUI::Create();

	CPlayer* pPlayer = CPlayer::GetListObj().GetData(0);
	m_pMapUI->BindPlayer(pPlayer);

	//=============================
	// �����΃}�l�[�W��
	//=============================
	m_pWaterStoneManager = CWaterStone_Manager::Create();

	//=============================
	// �l�}�l�[�W��
	//=============================
	m_pPeopleManager = CPeopleManager::Create(CPeopleManager::Type::TYPE_GAME);

	// BGM�Đ�
	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_BGM_GAME);
	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_BGM_WATER_FLOW);

	// �t�B�[�h�o�b�N�G�t�F�N�g���Z�b�g
	CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(
		0.0f,
		1.0f,
		0.01f);

	// ����
	return S_OK;
}

//==========================================================================
// ���[�h�ʏ�����
//==========================================================================
void CGame::InitByMode()
{
#if 0
	//**********************************
	// �G�̋��_
	//**********************************
	m_pEnemyBase = CEnemyBase::Create("data\\TEXT\\enemydata\\base.txt");
	if (m_pEnemyBase == nullptr)
	{
		return;
	}

	//**********************************
	// �G�}�l�[�W��
	//**********************************
	m_pEnemyManager = CEnemyManager::Create("data\\TEXT\\enemydata\\manager.txt");
	if (m_pEnemyManager == nullptr)
	{
		return;
	}
#endif
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

	// �X�R�A�̔j��
	if (m_pScore != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pScore->Uninit();

		// �������̊J��
		delete m_pScore;
		m_pScore = nullptr;
	}

	// �^�C�}�[�̔j��
	if (m_pTimer != nullptr)
	{
		// �I������
		CResultManager::SetClearTime(m_pTimer->GetTime());
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

	// �X�e�[�W�̔j��
	if (m_pStage != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pStage->Uninit();
		delete m_pStage;
		m_pStage = nullptr;
	}

	if (m_pGameManager != nullptr)
	{
		// �I������
		m_pGameManager->Uninit();
		delete m_pGameManager;
		m_pGameManager = nullptr;
	}


	// ��Q���}�l�[�W��
	if (m_pObstacleManager != nullptr)
	{
		m_pObstacleManager->Uninit();
		m_pObstacleManager = nullptr;
	}

	// �ו��}�l�[�W��
	if (m_pBaggageManager != nullptr)
	{
		m_pBaggageManager->Uninit();
		m_pBaggageManager = nullptr;
	}

	// ����]�[���}�l�[�W��
	if (m_pJudgeZoneManager != nullptr)
	{
		m_pJudgeZoneManager->Uninit();
		m_pJudgeZoneManager = nullptr;
	}

	// �����΃}�l�[�W��
	if (m_pWaterStoneManager != nullptr)
	{
		m_pWaterStoneManager->Uninit();
		m_pWaterStoneManager = nullptr;
	}

	// �R�[�X�}�l�[�W��
	if (m_pCourseManager != nullptr)
	{
		m_pCourseManager->Uninit();
		m_pCourseManager = nullptr;
	}

	// �l�}�l�[�W��
	if (m_pPeopleManager != nullptr)
	{
		m_pPeopleManager->Uninit();
		m_pPeopleManager = nullptr;
	}

	// �}�b�vUI
	if (m_pMapUI != nullptr)
	{
		m_pMapUI = nullptr;
		CMapUI::Release();
	}


	if (CManager::GetInstance() != nullptr)
	{
		if (CManager::GetInstance()->GetRenderer() != nullptr)
		{
			// �t�B�[�h�o�b�N�G�t�F�N�g���Z�b�g
			CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(
				0.0f,
				1.0f,
				0.01f);
		}
	}

	// �R�[�X
	m_pCourse = nullptr;

	// �I������
	CScene::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CGame::Update()
{

	// �Q�[���}�l�[�W��
	if (m_pGameManager != nullptr)
	{
		// �X�V����
		m_pGameManager->Update();
	}

	// �l�}�l�[�W��
	if (m_pPeopleManager != nullptr)
	{
		m_pPeopleManager->Update();
	}

	CManager::GetInstance()->GetDebugProc()->Print(
		"���݂̃��[�h�F�y�Q�[���z\n"
		"�؂�ւ��F�y F �z\n\n");

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();

	if (m_pScore != nullptr &&
		CManager::GetInstance()->GetEdit() == nullptr)
	{
		// �X�R�A�̍X�V����
		m_pScore->Update();
	}

#if _DEBUG

	if (ImGui::TreeNode("Water Ripple"))
	{
		static float height = 50.0f, velocity = 2.0f, thickness = 20.0f;
		static int life = 60;
		static int block = 64;
		static float blocksize = 10.0f;


		ImGui::DragInt("BLOCK", &block, 1);
		ImGui::DragFloat("BLOCK SIZE", &blocksize, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("height", &height, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("velocity", &velocity, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("thickness", &thickness, 1.0f, 0.0f, 0.0f, "%.2f");
		ImGui::DragInt("life", &life, 1);

		if (pInputKeyboard->GetTrigger(DIK_2))
		{
			CWaterRipple::Create(block, blocksize, MyLib::Vector3(0.0f, -5.0f, -800.0f), height, velocity, thickness, life);
		}
		ImGui::TreePop();
	}


	if (ImGui::TreeNode("Bubble"))
	{
		static float destRadius = 13.5f;
		static int posRange = 80, createIdx = 1;

		ImGui::DragInt("Pos Range", &posRange, 1);
		ImGui::DragInt("Create Idx", &createIdx, 1);
		ImGui::DragFloat("destRadius", &destRadius, 0.5f, 0.0f, 0.0f, "%.2f");

		if (pInputKeyboard->GetPress(DIK_3))
		{
			for (int i = 0; i < createIdx; i++)
			{
				int x = UtilFunc::Transformation::Random(-posRange, posRange);
				int z = UtilFunc::Transformation::Random(-posRange, posRange);

				float randmoveX = UtilFunc::Transformation::Random(-50, 50) * 0.01f;
				float randmoveY = UtilFunc::Transformation::Random(-20, 20) * 0.01f;
				float randRadius = UtilFunc::Transformation::Random(-20, 20) * 0.01f;
				float randDestRadius = UtilFunc::Transformation::Random(-30, 30) * 0.1f;
				float randCycle = UtilFunc::Transformation::Random(-20, 20) * 0.001f;


				CMeshBubble::Create(
					MyLib::Vector3(x, -5.0f, z),
					MyLib::Vector3(8.0f + randmoveX, 3.0f + randmoveY, 0.0f),
					1.0f + randRadius,
					destRadius + randDestRadius,
					0.08f + randCycle);
			}

		}

		ImGui::TreePop();
	}




	// �G�f�B�b�g�؂�ւ�����
	ChangeEdit();

	if (pInputKeyboard->GetTrigger(DIK_F4))
	{// F4�ŃG�f�B�b�g�؂�ւ�

		// �؂�ւ�
		m_EditType = (EditType)(((int)m_EditType + 1) % (int)EDITTYPE_MAX);	// �Ǐ]�̎��

		// ���Z�b�g
		EditReset();

		// �G�f�B�b�g����ON
		m_bEdit = true;

		switch (m_EditType)
		{
		case CGame::EDITTYPE_OFF:	// �S���I�t
			// �G�f�B�b�g����OFF
			m_bEdit = false;
			break;

		}
	}
#endif


	// �X�e�[�W�̍X�V
	if (m_pStage != nullptr)
	{
		m_pStage->Update();
	}


	// �^�C�}�[�X�V
	if (m_pTimer != nullptr &&
		!CManager::GetInstance()->GetPause()->IsPause())
	{
		m_pTimer->Update();
	}

#if _DEBUG

	if (pInputKeyboard->GetTrigger(DIK_F))
	{
		CManager::GetInstance()->GetResultManager()->SetClearTime(m_pTimer->GetTime());

		// ���[�h�ݒ�
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RESULT);
	}

	// ����
	if (ImGui::TreeNode("Create"))
	{
		if (ImGui::Button("CScroll"))
		{
			CScroll::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f), 1.0f, 250.0f, 900.0f, true);
		}

		if (ImGui::Button("CSuffocation"))
		{
			CSuffocation::Create();
		}

		if (ImGui::Button("CLeaf"))
		{
			CLeaf::Create(CManager::GetInstance()->GetCamera()->GetPositionR() + MyLib::Vector3(0.0f, 300.0f, 0.0f), CLeaf::Type::TYPE_FALL);
		}

		if (ImGui::Button("CLeafFlow"))
		{
			CLeaf::Create(MyLib::Vector3(-500.0f, 10.0f, UtilFunc::Transformation::Random(-300, 300)), CLeaf::Type::TYPE_FLOW);
		}

		if (ImGui::Button("CStageClearText"))
		{
			CStageClearText::Create(MyLib::Vector3(640.0f, 400.0f, 0.0f));
		}

		if (ImGui::Button("CGoalGameText"))
		{
			CGoalGameText::Create();
		}

		ImGui::TreePop();
	}

#endif

	// �V�[���̍X�V
	CScene::Update();
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
// �X�R�A�̎擾
//==========================================================================
CScore *CGame::GetScore()
{
	return m_pScore;
}

//==========================================================================
// �X�e�[�W�̎擾
//==========================================================================
CStage *CGame::GetStage()
{
	return m_pStage;
}

//==========================================================================
// �Q�[���}�l�[�W���̎擾
//==========================================================================
CGameManager *CGame::GetGameManager()
{
	return m_pGameManager;
}

//==========================================================================
// ���Z�b�g����
//==========================================================================
void CGame::ResetBeforeBoss()
{
#if 0
	// �X�e�[�W�̔j��
	if (m_pStage != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �I������
		m_pStage->Release();
		delete m_pStage;
		m_pStage = nullptr;
	}

	// �G�̋��_
	if (m_pEnemyBase != nullptr)
	{
		m_pEnemyBase->Uninit();
		delete m_pEnemyBase;
		m_pEnemyBase = nullptr;
	}

	// �G���A����
	if (m_pLimitArea != nullptr)
	{
		m_pLimitArea->Uninit();
		m_pLimitArea = nullptr;
	}

	// �X�e�[�W
	m_pStage = CStage::Create("data\\TEXT\\stage\\boss_info.txt");

	//**********************************
	// �G�̋��_
	//**********************************
	m_pEnemyBase = CEnemyBase::Create("data\\TEXT\\enemydata\\base_boss.txt");
	if (m_pEnemyBase == nullptr)
	{// nullptr��������
		return;
	}
#endif
	// ���b�N�I���̍ő勗��
	m_fMaxRokOnDistance = mylib_const::MAX_ROCKONDISTANCE_BOSS;
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
