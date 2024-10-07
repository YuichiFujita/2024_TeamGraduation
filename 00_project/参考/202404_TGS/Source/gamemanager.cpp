//=============================================================================
// 
// �Q�[���}�l�[�W������ [gamemanager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "gamemanager.h"
#include "game.h"
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "blackframe.h"
#include "stage.h"
#include "map.h"
#include "stagecleartext.h"
#include "limitarea.h"
#include "calculation.h"
#include "fog.h"
#include "fade.h"
#include "beforebattle.h"
#include "stagename.h"
#include "timer.h"
#include "input.h"
#include "baggage.h"
#include "course.h"
#include "judgezoneManager.h"
#include "peoplemanager.h"
#include "spline.h"
#include "request_people.h"
#include "subtitle.h"
#include "receiver_people.h"
#include "skip_ui.h"
#include "countdown_start.h"
#include "guide.h"
#include "leaf_flow.h"
#include "environment.h"


//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int POINT_WAVECLEAR = 5;		// �E�F�[�u�N���A�̃|�C���g
	const float POSR_Y_APPROACH_COEF = 0.3f;	//�J�������߂Â��ۂ̊���
	const float POSR_Y_PULL_SCREEN_POS = 210.0f;	// �J�����������n�߂�X�N���[�����W
	const float POSR_Y_APPROACH_SCREEN_POS = SCREEN_HEIGHT * 0.5f;	// �J�������߂Â��n�߂�X�N���[�����W
	const float POSR_YDEST_BAGGTOPLAYER_RATIO = 0.4f;	// �ו��ƃv���C���[�����̊����iposRYDest�j
	const int GUIDE_NUM = 100;

	const int CHANGE_BASEPOINT[] =	// �|�C���g�ύX����
	{
		40,	// AAA
		20,	// BBB
		10,	// CCC
		1,	// DDD
	};
	const float DEFAULT_INTERVAL_AIRSPAWN = 2.3f;	// �f�t�H�̋�C�����Ԋu
	const float DEFAULT_INTERVAL_FLOWLEAFSPAWN = 0.8f;	// �f�t�H�̗����t�����Ԋu
	const float DEFAULT_INTERVAL_LEAFSPAWN = 2.0f;		// �f�t�H�̍~��t�����Ԋu
}

namespace SceneTime
{
	const float RequestStart = 3.5f;	// �˗��J�n
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CGameManager::CGameManager()
{
	// �l�̃N���A
	m_SceneType = SCENE_MAIN;	// �V�[���̎��
	m_OldSceneType = SCENE_MAIN;	// �V�[���̎��
	m_bEndRush = false;			// ���b�V�����I��������
	m_bControll = false;		// ����ł��邩
	m_bEndNormalStage = false;	// �ʏ�X�e�[�W���I��������
	m_bGameStart = false;		// �Q�[���J�n���̃t���O
	m_nNowStage = 0;			// ���݂̃X�e�[�W
	m_nNumStage = 0;			// �X�e�[�W�̑���
	m_nEvaluationPoint = 0;		// �]���|�C���g
	m_nSaveEvaluationPoint = 0;
	m_fCameraLengthOld = 0;		// �O�̃J�����̋���
	m_fPosRY = 0.0f;
	m_fSceneTimer = 0.0f;		// �V�[���^�C�}�[
	m_pRequestPeople = nullptr;	// �˗��l�̃|�C���^
	m_pReceiverPeople = nullptr;
	m_nJudgeRank = 0;
	m_pSkipUI = nullptr;		// �X�L�b�vUI�̃|�C���^
	m_nGuideTimer = 0;
	m_pGuide = nullptr;
	m_pSpawn_Air = nullptr;			// ��C����
	m_pSpawn_LeafFlow = nullptr;	// �����t����
	m_pSpawn_Leaf = nullptr;		// �~��t����
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CGameManager::~CGameManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CGameManager* CGameManager::Create(CScene::MODE mode)
{
	// �������m��
	CGameManager* pManager = nullptr;

	// �C���X�^���X����
	switch (mode)
	{
	case CScene::MODE_GAME:
		pManager = DEBUG_NEW CGameManager;
		break;

	}

	if (pManager != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		HRESULT hr = pManager->Init();

		if (FAILED(hr))
		{// ���s���Ă�����
			return nullptr;
		}
	}

	return pManager;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CGameManager::Init()
{
	m_bControll = true;			// ����ł��邩
	m_bEndNormalStage = false;	// �ʏ�X�e�[�W���I��������
	m_bGameStart = false;		// �Q�[���J�n���̃t���O

#if _DEBUG
	m_nNowStage = 0;			// ���݂̃X�e�[�W
	m_SceneType = SceneType::SCENE_START;	// �V�[���̎�� 
#else
	m_nNowStage = 0;			// ���݂̃X�e�[�W
	m_SceneType = SceneType::SCENE_START;	// �V�[���̎�� 
#endif

	m_OldSceneType = m_SceneType;

#if 1
	// �˗��l����
	m_pRequestPeople = CRequestPeople::Create(MyLib::Vector3(500.0f, 0.0f, 900.0f));
	m_pRequestPeople->SetState(CRequestPeople::STATE::STATE_WAIT);

	// ���t���[���C��
	CBlackFrame::GetInstance()->SetState(CBlackFrame::STATE::STATE_INCOMPLETION);

	// �J�������[�V�����Đ�
	CManager::GetInstance()->GetCamera()->GetCameraMotion()->SetMotion(CCameraMotion::MOTION::MOTION_PASS, CCameraMotion::EASING::Linear);

	CSubTitle* pSubTitle = CSubTitle::Create(MyLib::Vector3(640.0f, 670.0f, 0.0f), 2.0f);
	pSubTitle->BindSubtitle("data\\TEXTURE\\subtitle\\sample.png");
	pSubTitle->SetSizeByHeight(40.0f);
#else

	m_SceneType = SceneType::SCENE_WAIT_AIRPUSH;	// �V�[���̎�� 
#endif



	// ��C�̐����N���X����
	m_pSpawn_Air = DEBUG_NEW CSpawn_Air(0.0f, DEFAULT_INTERVAL_AIRSPAWN);

	// �����t�����N���X����
	m_pSpawn_LeafFlow = DEBUG_NEW CSpawn_FlowLeaf(0.0f, DEFAULT_INTERVAL_FLOWLEAFSPAWN);

	// �~��t�����N���X����
	m_pSpawn_Leaf = DEBUG_NEW CSpawn_Leaf(0.0f, DEFAULT_INTERVAL_LEAFSPAWN);
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CGameManager::Uninit()
{
	// ��C�̐����N���X
	if (m_pSpawn_Air != nullptr)
	{
		delete m_pSpawn_Air;
		m_pSpawn_Air = nullptr;
	}

	// �����t�����N���X
	if (m_pSpawn_LeafFlow != nullptr)
	{
		delete m_pSpawn_LeafFlow;
		m_pSpawn_LeafFlow = nullptr;
	}

	// �~��t�����N���X
	if (m_pSpawn_Leaf != nullptr)
	{
		delete m_pSpawn_Leaf;
		m_pSpawn_Leaf = nullptr;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CGameManager::Update()
{
	// �f���^�^�C���擾
	float deltaTime = CManager::GetInstance()->GetDeltaTime();

	// ������
	m_fSceneTimer += deltaTime;		// �V�[���^�C�}�[
	switch (m_SceneType)
	{
	case CGameManager::SceneType::SCENE_MAIN:
		m_bControll = true;
		CheckJudgeZone();
		ContainPlayerBaggage();
		TurnAway();
		break;

	case CGameManager::SceneType::SCENE_START:
		m_bControll = false;
		SceneStart();
		break;

	case CGameManager::SceneType::SCENE_SKIP:
		m_bControll = false;
		SceneSkip();
		break;

	case CGameManager::SceneType::SCENE_COUNTDOWN:		// �J�E���g�_�E��
		
		// �X�L�b�vUI�폜
		if (m_pSkipUI != nullptr)
		{
			m_pSkipUI->Kill();
			m_pSkipUI = nullptr;
		}
		TurnAway();
		break;

	case CGameManager::SceneType::SCENE_MAINCLEAR:
		m_bControll = false;
		SceneGoal();
		break;

	case SceneType::SCENE_MAINRESULT:
		m_bControll = false;
		break;

	case SceneType::SCENE_DURING_MAINRESULT:
		m_bControll = false;
		break;

	case SceneType::SCENE_BEFOREBATTLE:
		m_bControll = false;
		break;

	case SceneType::SCENE_BATTLESTART:
		m_bControll = false;
		break;

	case SceneType::SCENE_SKILLTREE:	// �X�L���c���[
		m_bControll = false;
		break;

	case CGameManager::SceneType::SCENE_BOSS:
		m_bControll = true;
		break;

	case CGameManager::SceneType::SCENE_TRANSITIONWAIT:
		m_bControll = false;
		break;

	case CGameManager::SceneType::SCENE_TRANSITION:
		m_bControll = false;
		SceneTransition();
		break;

	case SceneType::SCENE_RESULT:
		m_bControll = false;
		break;

	case SceneType::SCENE_WAIT_AIRPUSH:
		m_bControll = false;
		SceneWaitAirPush();
		TurnAway();
		break;

	case SceneType::SCENE_GOAL:			// �S�[��
		SceneGoal();
		break;

	case SceneType::SCENE_DEBUG:
		m_bControll = true;
		break;

	default:
		break;
	}

	// �W���b�W�����N�̕ϓ����m
	for (int i = 0; i < CJudge::JUDGE::JUDGE_MAX; i++)
	{
		if (m_nEvaluationPoint >= CHANGE_BASEPOINT[i])
		{
			m_nJudgeRank = i;
			CPeopleManager::GetInstance()->SetRank(static_cast<CJudge::JUDGE>(i));
			break;
		}

		m_nJudgeRank = CJudge::JUDGE::JUDGE_MAX;
		CPeopleManager::GetInstance()->SetRank(CJudge::JUDGE::JUDGE_MAX);
	}

#if 1
	// ��C����
	if (m_pSpawn_Air != nullptr)
	{
		m_pSpawn_Air->Update(deltaTime);
	}

	// �����t����
	if (m_pSpawn_LeafFlow != nullptr)
	{
		m_pSpawn_LeafFlow->Update(deltaTime);
	}

	// �~��t����
	if (m_pSpawn_Leaf != nullptr)
	{
		m_pSpawn_Leaf->Update(deltaTime);
	}
#endif
	// �e�L�X�g�̕`��
	CManager::GetInstance()->GetDebugProc()->Print(
		"---------------- �Q�[���}�l�[�W����� ----------------\n"
		"�y���[�h�z[%d]\n"
		"�y�X�e�[�W�z[%d]\n"
		"�y�]���|�C���g�z[%d]\n"
		, m_SceneType, m_nNowStage, m_nEvaluationPoint);
}

//==========================================================================
// �X�^�[�g���̐ݒ�
//==========================================================================
void CGameManager::StartSetting()
{
	// �J�E���g�_�E������
	CManager::GetInstance()->GetCamera()->GetCameraMotion()->SetFinish(true);
	m_SceneType = SceneType::SCENE_COUNTDOWN;

	// �J�E���g�_�E���J�n
	CCountdown_Start::Create();

	// ���t���[���폜
	CBlackFrame::GetInstance()->SetState(CBlackFrame::STATE::STATE_OUTCOMPLETION);

	// �ו�
	CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);
	pBaggage->SetScale(1.0f);
	pBaggage->SetState(CBaggage::STATE::STATE_NONE);

	// �˗��l
	m_pRequestPeople->SetState(CRequestPeople::STATE::STATE_BYEBYE);

	// �X�L�b�vUI�폜
	m_pSkipUI->Kill();
	m_pSkipUI = nullptr;

	CPlayer::GetListObj().GetData(0)->GetMotion()->ResetPose(0);
}

//==========================================================================
// �Q�[���N���A���̐ݒ�
//==========================================================================
void CGameManager::GameClearSettings()
{
	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = playerList.GetData(0);


}


//==========================================================================
// �X�L�b�v
//==========================================================================
void CGameManager::SceneSkip()
{
	// �t�F�[�h�������ȊO�t�F�[�h
	CInstantFade* pInstantFade = CManager::GetInstance()->GetInstantFade();
	if (pInstantFade->GetState() != CInstantFade::STATE::STATE_FADECOMPLETION) return;

	switch (m_OldSceneType)
	{
	case CGameManager::SceneType::SCENE_START:

		// �X�^�[�g���̐ݒ�
		StartSetting();
		break;

	default:
		break;
	}
}

//==========================================================================
// �J�n���o
//==========================================================================
void CGameManager::SceneStart()
{
	if (m_pSkipUI == nullptr)
	{
		m_pSkipUI = CSkip_UI::Create();
	}

	// �J�ڂȂ��t�F�[�h
	CInstantFade* pInstantFade = CManager::GetInstance()->GetInstantFade();
	if (pInstantFade->GetState() != CInstantFade::STATE::STATE_NONE)
	{
		m_pSkipUI->SetState(CSkip_UI::State::STATE_FADEOUT);
		return;
	}

	if (m_pSkipUI != nullptr &&
		m_pSkipUI->IsComplete())
	{// �X�L�b�v����

		// �t�F�[�h�ݒ�
		pInstantFade->SetFade();

		// �X�L�b�v���
		m_OldSceneType = m_SceneType;
		m_SceneType = SceneType::SCENE_SKIP;
		return;
	}

	if (m_fSceneTimer >= SceneTime::RequestStart &&
		m_pRequestPeople->GetState() == CRequestPeople::STATE::STATE_WAIT)
	{
		m_pRequestPeople->SetState(CRequestPeople::STATE::STATE_PASS);
	}
}

//==========================================================================
// �Q�[�����U���g�̐ݒ�
//==========================================================================
void CGameManager::GameResultSettings()
{

	// �N���A�e�L�X�g����
	CStageClearText::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f));


}

//==========================================================================
// ���C���J�ڒ�
//==========================================================================
void CGameManager::SceneTransition()
{
	// �J�ڂȂ��t�F�[�h�̏�Ԏ擾
	CInstantFade::STATE fadestate = CManager::GetInstance()->GetInstantFade()->GetState();

	if (fadestate == CInstantFade::STATE_FADECOMPLETION ||
		!m_bGameStart)
	{// ���������u��

		// BGM�X�g�b�v
		CSound::GetInstance()->StopSound();
		CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_GAME);

		// �Q�[���J�n���̃t���O
		m_bGameStart = true;

		// �J�����擾
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		if (pCamera != nullptr)
		{
			pCamera->SetEnableFollow(true);
		}

		// �G���A�������擾
		CListManager<CLimitArea> limitareaList = CLimitArea::GetListObj();
		CLimitArea* pLimitArea = nullptr;
		while (limitareaList.ListLoop(&pLimitArea))
		{
			pLimitArea->Kill();
		}

		// �G�t�F�N�g�S�Ē�~
		CMyEffekseer::GetInstance()->StopAll();
		CMyEffekseer::GetInstance()->StopAll();

		// �t�H�O���Z�b�g
		MyFog::ToggleFogFrag(false);

		// �v���C���[�擾
		CListManager<CPlayer> playerList = CPlayer::GetListObj();
		CPlayer* pPlayer = playerList.GetData(0);

		// �G�z�u
		SetEnemy();

		// �퓬�����ɑJ��
		CBeforeBattle::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f));
		SetType(SceneType::SCENE_BEFOREBATTLE);
	}
}

//==========================================================================
// ��C����҂�
//==========================================================================
void CGameManager::SceneWaitAirPush()
{
	CTimer* pTimer = CGame::GetInstance()->GetTimer();
	CKeyConfig* pConfigKeyboard = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INKEY);
	CKeyConfig* pConfigPad = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	if (pTimer != nullptr &&
		pConfigPad->GetTrigger(INGAME::ACT_AIR) ||
		pConfigKeyboard->GetTrigger(INGAME::ACT_AIR))
	{
		pTimer->SetEnableAddTime(true);
		SetType(SceneType::SCENE_MAIN);

		m_nGuideTimer = 0;

		if (m_nGuideTimer == 0 && m_pGuide != nullptr)
		{
			m_pGuide->Kill();
			m_pGuide = nullptr;
		}
	}
	else if (pTimer != nullptr)
	{
		m_nGuideTimer++;

		if (m_nGuideTimer >= GUIDE_NUM &&
			m_pGuide == nullptr)
		{
			m_pGuide = CGuide::Create(CGuide::Type::START);
		}

		pTimer->SetEnableAddTime(false);
	}
}

//==========================================================================
// �S�[�����
//==========================================================================
void CGameManager::SceneGoal()
{
	CGame::GetInstance()->GetTimer()->SetState(CTimer::eState::STATE_GOAL);
}

//==========================================================================
// �v���C���[�Ɖו�����ʓ��Ɏ��߂�
//==========================================================================
void CGameManager::ContainPlayerBaggage()
{
	// �J�����擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;
	playerList.ListLoop(&pPlayer);

	// �ו��擾
	CListManager<CBaggage> baggageList = CBaggage::GetListObj();
	CBaggage* pBaggage = nullptr;
	baggageList.ListLoop(&pBaggage);

	// ���߂���̂̍��W
	MyLib::Vector3 posPlayer = pPlayer->GetPosition();
	MyLib::Vector3 posBaggage = pBaggage->GetPosition();
	
	// posR���݂�Y���W�擾
	m_fPosRY = pCamera->GetPositionR().y;

	// �͈͓��Ɏ��߂�
	MyLib::Vector3 screenPos = pCamera->GetScreenPos(posBaggage);

	if (screenPos.y <= POSR_Y_PULL_SCREEN_POS)
	{// ���ʊO�i�����j
		float posRYDest = (posBaggage.y - posPlayer.y) * POSR_YDEST_BAGGTOPLAYER_RATIO;
		if (m_fPosRY <= posRYDest)
		{
			m_fPosRY = posRYDest;
		}
	}
	else if (screenPos.y >= POSR_Y_APPROACH_SCREEN_POS)
	{// ����ʊO�i�����j
		float posRYDest = posBaggage.y;
		m_fPosRY += (posRYDest - m_fPosRY) * POSR_Y_APPROACH_COEF;
		m_fPosRY = UtilFunc::Transformation::Clamp(m_fPosRY, 200.0f, FLT_MAX);
	}

	pCamera->SetAutoMovingPosR(MyLib::Vector3(posBaggage.x, m_fPosRY, posBaggage.z));
}

//==========================================================================
// �J��������ɉ��������悤�ɂ���
//==========================================================================
void CGameManager::TurnAway()
{
	// �J�����擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera->GetCameraMotion()->IsEdit() || !pCamera->IsFollow()) return;

	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;
	playerList.ListLoop(&pPlayer);

	// �ړ���������p�x�Z�o
	float moveLength = pPlayer->GetMoveLength();
	MyLib::Vector3 posDest = MySpline::GetSplinePosition/*_NonLoop*/(CGame::GetInstance()->GetCourse()->GetVecPosition(), moveLength + 20.0f);
	
	
	// ��̈ʒu�Ƃ̌����Z�o
	MyLib::Vector3 posPlayer = pPlayer->GetPosition();
	float angleXZ = posPlayer.AngleXZ(posDest);

	// �v���C���[�̕�����s
	if (posDest.x <= posPlayer.x)
	{
		angleXZ = posPlayer.AngleXZ(posPlayer + MyLib::Vector3(20.0f, 0.0f, 0.0f));
	}
	
	// 90�x��]
	angleXZ += (D3DX_PI * 0.5f);


	// �p�x�ݒ�
	MyLib::Vector3 rot = pCamera->GetRotation();
	pCamera->SetRotation(MyLib::Vector3(rot.x, angleXZ + pCamera->GetOriginRotation().y, rot.z));
}

//==========================================================================
// ����]�[���m�F
//==========================================================================
void CGameManager::CheckJudgeZone()
{
	//// �v���C���[�擾
	//CListManager<CPlayer> playerList = CPlayer::GetListObj();
	//CPlayer* pPlayer = nullptr;
	//playerList.ListLoop(&pPlayer);

	//// �R�[�X�擾
	//CCourse* pCource = CGame::GetInstance()->GetCourse();

	//// �i�s�x�v�Z���Ċm�F
	//float progress = pPlayer->GetMoveLength() / pCource->GetCourceLength();
	//CManager::GetInstance()->GetDebugProc()->Print("�i�s�x:%f\n", progress);
	//CJudgeZoneManager::GetInstance()->Check(progress);
}

//==========================================================================
// �{�X�ݒ�
//==========================================================================
void CGameManager::SetBoss()
{

}

//==========================================================================
// �G�ݒ�
//==========================================================================
void CGameManager::SetEnemy()
{

	// �X�e�[�W�؂�ւ�
	CGame::GetInstance()->GetStage()->ChangeStage("data\\TEXT\\stage\\info.txt");

	// �}�b�v�؂�ւ�
	MyMap::ChangeMap("data\\TEXT\\map\\info.txt");

	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;

	// ���X�g���[�v
	while (playerList.ListLoop(&pPlayer))
	{
		// �ʒu�ݒ�
		pPlayer->SetPosition(MyLib::Vector3(0.0f, 0.0f, -1000.0f));
		pPlayer->SetRotation(MyLib::Vector3(0.0f, D3DX_PI, 0.0f));
		pPlayer->SetRotDest(D3DX_PI);
	}

	// �J�����̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera != nullptr)
	{
		pCamera->Reset(CScene::MODE_GAME);
	}

	// ��ސݒ�
	m_SceneType = SCENE_MAIN;
}

//==========================================================================
// �X�e�[�W�̑����ݒ�
//==========================================================================
void CGameManager::SetNumStage(int nStage)
{ 
	m_nNumStage = nStage;
}

//==========================================================================
// �X�e�[�W�̉��Z
//==========================================================================
void CGameManager::AddNowStage()
{
	// ���Z
	m_nNowStage++;

	if (m_nNumStage <= m_nNowStage)
	{// ���X�e�[�W���𒴂�����

		// �ʏ�X�e�[�W���I������
		m_bEndNormalStage = true;
	}

}

//==========================================================================
// ���݂̃X�e�[�W�擾
//==========================================================================
int CGameManager::GetNowStage()
{
	return m_nNowStage;
}

//==========================================================================
// �V�[���̎�ސݒ�
//==========================================================================
void CGameManager::SetType(SceneType type)
{
	m_OldSceneType = m_SceneType;
	m_SceneType = type;

	if (m_OldSceneType == SceneType::SCENE_GOAL && m_SceneType != SceneType::SCENE_GOAL)
	{
		int s = 0;
	}
}

//==========================================================================
// �V�[���̎�ގ擾
//==========================================================================
CGameManager::SceneType CGameManager::GetType()
{
	return m_SceneType;
}



