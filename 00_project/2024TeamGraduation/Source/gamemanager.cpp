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
#include "map.h"
#include "calculation.h"
#include "fog.h"
#include "fade.h"
#include "input.h"
#include "ball.h"
#include "collisionLine_Box.h"
#include "teamStatus.h"
#include "audience.h"
#include "gymDoor.h"
#include "gymWallManager.h"
#include "playerSpawnManager.h"
#include "playerManager.h"
#include "charmManager.h"
#include "charmText.h"
#include "timerUI.h"
#include "reporter.h"
#include "resultManager.h"
#include "gauge2D.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TOP_LINE = "#==============================================================================";	// �e�L�X�g�̃��C��
	const std::string TEXT_LINE = "#------------------------------------------------------------------------------";	// �e�L�X�g�̃��C��

	const float END_HYPE_TIME[] = { 9999.0f, 0.0f };	// �����`�[�����莞�̐���オ�莞��

	// �h�b�W�{�[���R�[�g���
	namespace Gym
	{
		const MyLib::Vector3 POS_LEFT = MyLib::Vector3(-972.85f, 0.0f, 1717.35f);	// �h�A���ʒu
		const MyLib::Vector3 POS_RIGHT = MyLib::Vector3(972.85f, 0.0f, 1717.35f);	// �h�A�E�ʒu
		const MyLib::Vector3 POS[] = { POS_LEFT, POS_RIGHT };	// �h�A�ʒu
	}

	// �h�b�W�{�[���R�[�g���
	namespace Court
	{
		const MyLib::Vector3 SIZE = MyLib::Vector3(950.0f, 100.0f, 560.0f);	// �T�C�Y
		const float HALF_DIAGONAL = sqrtf(SIZE.x * SIZE.x + (SIZE.z * 2.0f) * (SIZE.z * 2.0f));	// �`�[�����R�[�g�̑Ίp��
	}

	// �^�C�}�[���
	namespace Timer
	{
		const float RATE = 0.7f;
		const MyLib::Vector3 POS = MyLib::Vector3(640.0f, 80.0f, 0.0f);	// �^�C�}�[�ʒu
		const MyLib::Vector3 POS_BG = Timer::POS + MyLib::Vector3(0.0f, -20.0f, 0.0f);	// �^�C�}�[�ʒu
		const MyLib::Vector2 VAL_SIZE = MyLib::Vector2(0.0f, 28.0f) * RATE;	// �^�C�}�[�����傫��
		const MyLib::Vector2 PART_SIZE = MyLib::Vector2(0.0f, 18.0f) * RATE;	// �^�C�}�[��؂�傫��
		const MyLib::Vector2 VAL_SPACE = MyLib::Vector2(53.0f, 0.0f) * RATE;	// �^�C�}�[������
		const MyLib::Vector2 PART_SPACE = MyLib::Vector2(35.0f, 0.0f) * RATE;	// �^�C�}�[��؂��
		const char* TEXTURE_VALUE = "data\\TEXTURE\\number\\AKR.png";	// �����e�N�X�`���p�X
		const char* TEXTURE_PART = "data\\TEXTURE\\timepart001.png";	// ��؂�e�N�X�`���p�X
		const char* TEXTURE_BG = "data\\TEXTURE\\timer\\bg.png";		// �w�i�e�N�X�`���p�X
		const float HEIGHT_BG = 64.0f;									// �w�i�c��
	}
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CGameManager::SCENE_FUNC CGameManager::m_SceneFunc[] =	// �V�[���֐�
{
	&CGameManager::SceneMain,		// ���C��
	&CGameManager::SceneSpawn,		// �o�ꉉ�o
	&CGameManager::SceneStart,		// �J�n���o
	&CGameManager::SceneSpecial,	// �X�y�V�������o
	&CGameManager::SceneEnd,		// �I�����o
	&CGameManager::SceneDebug,		// �f�o�b�O
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CGameManager* CGameManager::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CGameManager::CGameManager()
{
	// �l�̃N���A
	m_SceneType = SCENE_MAIN;		// �V�[���̎��
	m_OldSceneType = SCENE_MAIN;	// �V�[���̎��(�ߋ�)
	m_bControll = false;			// ����ł��邩
	m_fSceneTimer = 0.0f;			// �V�[���^�C�}�[
	m_courtSize = MyLib::Vector3();	// �R�[�g�T�C�Y
	m_endInfo = SEndInfo();			// �I�������

	m_pGymWallManager = nullptr;		// �W���ǃ}�l�W���[
	m_pCharmManager = nullptr;			// ���e�}�l�[�W��
	m_pCharmValueManager = nullptr;		// ���e�l�}�l�[�W��
	m_pSpecialValueManager = nullptr;	// �X�ؒl�}�l�[�W��
	m_pTimerUI = nullptr;				// �^�C�}�[UI
	m_pTimerBG = nullptr;				// �^�C�}�[�w�i

	memset(&m_apGymDoor[0], 0, sizeof(m_apGymDoor));		// �̈�ق̃h�A
	memset(&m_pTeamStatus[0], 0, sizeof(m_pTeamStatus));	// �`�[���X�e�[�^�X
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
	if (m_pThisPtr != nullptr)
	{// ���ɂ���ꍇ
		return m_pThisPtr;
	}

	// �C���X�^���X����
	switch (mode)
	{
	case CScene::MODE_GAME:
		m_pThisPtr = DEBUG_NEW CGameManager;
		break;
	}

	if (m_pThisPtr != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		HRESULT hr = m_pThisPtr->Init();

		if (FAILED(hr))
		{// ���s���Ă�����
			return nullptr;
		}
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CGameManager::Init()
{
	m_bControll = true;	// ����ł��邩

	// �R�[�g�T�C�Y
	m_courtSize = Court::SIZE;

	for (int i = 0; i < EDoor::DOOR_MAX; i++)
	{ // �h�A�̔z�u�����J��Ԃ�

		// �̈�كh�A����
		m_apGymDoor[i] = CGymDoor::Create(Gym::POS[i]);
		if (m_apGymDoor[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ
	
			return E_FAIL;
		}
	}

	// �W���ǃ}�l�[�W������
	m_pGymWallManager = CGymWallManager::Create();
	if (m_pGymWallManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

#if _DEBUG
	// �R�[�g�T�C�Y�̃{�b�N�X
	if (m_pCourtSizeBox == nullptr)
	{
		m_pCourtSizeBox = CCollisionLine_Box::Create(MyLib::AABB(-m_courtSize, m_courtSize), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
#endif

#if 1	// TODO : ���c�p�ɖ߂�
	// �J�n�V�[���̐ݒ�
	SetSceneType(ESceneType::SCENE_SPAWN);	// �o�ꉉ�o

	// �v���C���[�o�ꉉ�o�}�l�[�W���[����
	if (CPlayerSpawnManager::Create() == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �ʏ�J�����̐ݒ�
	CCamera* pCamera = GET_MANAGER->GetCamera();	// �J�������
	pCamera->SetState(CCamera::STATE_NONE);
#else
	// �J�n�V�[���̐ݒ�
	SetSceneType(ESceneType::SCENE_START);

	// �v���C���[�}�l�[�W���[�̐���
	CPlayerManager::Create(CScene::MODE_GAME);

	// �Ǐ]�J�����̐ݒ�
	CCamera* pCamera = GET_MANAGER->GetCamera();	// �J�������
	pCamera->SetState(CCamera::STATE_FOLLOW);
#endif

	// �`�[���X�e�[�^�X�̐���
	CreateTeamStatus();

	// �O���t�@�C������ݒ肳�ꂽ�^�C����Ǎ�
	CEntryRuleManager::SRule rule;		// ���[��
	CEntryRuleManager::LoadRule(&rule);	// ���[���Ǎ�
	if (rule.fTime > 0.0f)
	{ // ���Ԑ�������������Ȃ��ꍇ

		CTexture* pTexture = CTexture::GetInstance();	// �e�N�X�`�����

		// �^�C�}�[UI�̐���
		m_pTimerUI = CTimerUI::Create
		( // ����
			rule.fTime,			// �J�n����
			rule.fTime,			// ��������
			Timer::POS,			// �ʒu
			Timer::VAL_SIZE.y,	// �����̑傫��
			Timer::PART_SIZE.y,	// ��؂�̑傫��
			Timer::VAL_SPACE,	// �����̋�
			Timer::PART_SPACE	// ��؂�̋�
		);
		if (m_pTimerUI == nullptr)
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}

		// �����e�N�X�`���̍Ċ���
		m_pTimerUI->BindTextureValue(pTexture->Regist(Timer::TEXTURE_VALUE));

		// ��؂�e�N�X�`���̍Ċ���
		m_pTimerUI->BindTexturePart(pTexture->Regist(Timer::TEXTURE_PART));

		// �F�ݒ�
		m_pTimerUI->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// �^�C�}�[UI�̔�\��
		m_pTimerUI->SetEnableDisp(false);

		// �^�C�}�[�w�i�̐���
		m_pTimerBG = CObject2D::Create();
		if (m_pTimerBG == nullptr)
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}

		// �ʒu��ݒ�
		m_pTimerBG->SetPosition(Timer::POS_BG);

		// �e�N�X�`����o�^�E����
		int texID = pTexture->Regist(Timer::TEXTURE_BG);
		m_pTimerBG->BindTexture(texID);

		// �摜�䗦����傫����ݒ�
		MyLib::Vector2 size = pTexture->GetImageSize(texID);
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, Timer::HEIGHT_BG);
		m_pTimerBG->SetSize(size);
		m_pTimerBG->SetSizeOrigin(size);

		// �^�C�}�[�w�i�̔�\��
		m_pTimerBG->SetEnableDisp(false);
	}

	// ���e�}�l�[�W������
	m_pCharmManager = CCharmManager::Create();
	if (m_pCharmManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ���e�l�}�l�[�W������
	m_pCharmValueManager = CCharmValueManager::Create();
	if (m_pCharmValueManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �X�ؒl�}�l�[�W������
	m_pSpecialValueManager = CSpecialValueManager::Create();
	if (m_pSpecialValueManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CGameManager::Uninit()
{
	// �`�[���X�e�[�^�X
	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		SAFE_UNINIT(m_pTeamStatus[i]);
	}

#if _DEBUG
	// �I��
	if (m_pCourtSizeBox != nullptr)
	{
		m_pCourtSizeBox = nullptr;
	}
#endif

	// �v���C���[�}�l�[�W���[
	CPlayerManager* pManager = CPlayerManager::GetInstance();
	SAFE_UNINIT(pManager);

	// �^�C�}�[UI
	SAFE_UNINIT(m_pTimerUI);

	// �^�C�}�[�w�i
	SAFE_UNINIT(m_pTimerBG);

	// ���e�}�l�[�W��
	SAFE_UNINIT(m_pCharmManager);

	// ���e�l�}�l�[�W��
	SAFE_UNINIT(m_pCharmValueManager);

	// �X�ؒl�}�l�[�W��
	SAFE_UNINIT(m_pSpecialValueManager);

	// �ϋq�S����
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		CAudience::SetDespawnAll(static_cast<CGameManager::ETeamSide>(i));
	}

	// ���g�̊J��
	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CGameManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// TODO : UpdateScene�I�Ȃ̂ł܂Ƃ߂���
	m_fSceneTimer += fDeltaTime * fSlowRate;	// �V�[���^�C�}�[
	
	// �V�[���ʍX�V
	(this->*(m_SceneFunc[m_SceneType]))();

	CPlayerManager* pManager = CPlayerManager::GetInstance();
	if (pManager != nullptr)
	{
		// �v���C���[�}�l�[�W���[�X�V
		pManager->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	if (m_pCharmManager != nullptr)
	{
		// ���e�}�l�[�W���[�X�V
		m_pCharmManager->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// �Q�[�W���ԍX�V
	UpdateGaugeTime();

// �B�e�p
#if 0
	if (m_pCharmManager != nullptr)
	{
		m_pCharmManager->SetHypeTime(CGameManager::ETeamSide::SIDE_LEFT, 10.0f);
	}
#endif

#if _DEBUG	// �f�o�b�O����

	if (ImGui::TreeNode("GameManager"))
	{
		Debug();
		ImGui::TreePop();
	}
#endif
}

//==========================================================================
// �h�A�J���t���O�̐ݒ�
//==========================================================================
void CGameManager::SetEnableOpen(const bool bOpen, const float fMoveTime)
{
	for (int i = 0; i < EDoor::DOOR_MAX; i++)
	{ // �h�A�̔z�u�����J��Ԃ�

		// �̈�كh�A�J���t���O�̐ݒ�
		m_apGymDoor[i]->SetEnableOpen(bOpen, fMoveTime);
	}
}

//==========================================================================
// �h�A�J���t���O�̎擾
//==========================================================================
bool CGameManager::IsOpen() const
{
	for (int i = 0; i < EDoor::DOOR_MAX; i++)
	{ // �h�A�̔z�u�����J��Ԃ�

		// �h�A���J������Ă��Ȃ��ꍇ������
		if (!m_apGymDoor[i]->IsOpen()) { return false; }
	}

	return true;
}

//==========================================================================
// �h�A�ғ����t���O�̎擾
//==========================================================================
bool CGameManager::IsMove() const
{
	for (int i = 0; i < EDoor::DOOR_MAX; i++)
	{ // �h�A�̔z�u�����J��Ԃ�

		// �h�A���ғ����ł͂Ȃ��ꍇ������
		if (!m_apGymDoor[i]->IsMove()) { return false; }
	}

	return true;
}

//==========================================================================
// �X�^�[�g���̐ݒ�
//==========================================================================
void CGameManager::StartSetting()
{
	// ���C���֑J��
	SetSceneType(ESceneType::SCENE_MAIN);

	// TODO�F�J�n���o���I�������J�ځI
	{
#if _DEBUG
		// �R�[�g�T�C�Y�̃{�b�N�X
		if (m_pCourtSizeBox == nullptr)
		{
			m_pCourtSizeBox = CCollisionLine_Box::Create(MyLib::AABB(-m_courtSize, m_courtSize), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
#endif

		// �`�[���X�e�[�^�X
		CreateTeamStatus();
		StartTeamStatus();

		// �^�C�}�[�̌v���J�n
		if (m_pTimerUI != nullptr)
		{
			m_pTimerUI->Start();
		}

		// ���C���֑J��
		SetSceneType(ESceneType::SCENE_MAIN);
	}
}

//==========================================================================
// ���C��
//==========================================================================
void CGameManager::SceneMain()
{
	// ����o����
	m_bControll = true;

	// �ϋq�X�V
	UpdateAudience();

	// �������ԍX�V
	UpdateLimitTimer();

	// �`�[���X�e�[�^�X�X�V
	UpdateTeamStatus();
}

//==========================================================================
// �������ԍX�V
//==========================================================================
void CGameManager::UpdateLimitTimer()
{
	// �^�C�}�[�����Ɣ�����
	if (m_pTimerUI == nullptr) return;

	// �I��������
	if (m_pTimerUI->IsEnd())
	{
		SetSceneType(ESceneType::SCENE_END);
	}
}

//==========================================================================
// �o�ꉉ�o
//==========================================================================
void CGameManager::SceneSpawn()
{
	// �v���C���[�o�ꉉ�o�}�l�[�W���[�̎擾
	CPlayerSpawnManager* pManager = CPlayerSpawnManager::GetInstance();
	assert(pManager != nullptr);

	// ���͏��̎擾
	CInputKeyboard* pKey = GET_INPUTKEY;
	CInputGamepad* pPad = GET_INPUTPAD;

	// ����o���Ȃ�
	m_bControll = false;

	bool bInput = pKey->GetTrigger(DIK_RETURN)
			   || pKey->GetTrigger(DIK_SPACE)
			   || pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0)
			   || pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, 0)
			   || pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, 0)
			   || pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_Y, 0);

	if (bInput || pManager->GetState() == CPlayerSpawnManager::EState::STATE_END)
	{ // �X�L�b�v����A�܂��͓o�ꉉ�o���I������ꍇ

		// �o�ꉉ�o�̃X�L�b�v
		SkipSpawn();
	}
}

//==========================================================================
// �J�n���o
//==========================================================================
void CGameManager::SceneStart()
{
	// ����o���Ȃ�
	m_bControll = false;
}

//==========================================================================
// �X�y�V�������o
//==========================================================================
void CGameManager::SceneSpecial()
{
	// ����o���Ȃ�
	m_bControll = false;

	// �X�y�V�������o�X�V
	UpdateSpecial();
}

//==========================================================================
// �I�����o
//==========================================================================
void CGameManager::SceneEnd()
{
	// ����o���Ȃ�
	m_bControll = false;

	// �J��
	if (GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_RESULT))
	{// ��x����

		// �����I������
		EndGame();
	}
}

//==========================================================================
// �f�o�b�O
//==========================================================================
void CGameManager::SceneDebug()
{
	// ����o����
	m_bControll = true;
}

//==========================================================================
// �o�ꉉ�o�X�L�b�v
//==========================================================================
void CGameManager::SkipSpawn()
{
	CPlayerSpawnManager* pManager = CPlayerSpawnManager::GetInstance();	// �v���C���[�o�ꉉ�o�}�l�[�W���[
	assert(pManager != nullptr);

	// �v���C���[�o�ꉉ�o�}�l�[�W���[�̏I��
	SAFE_KILL(pManager);

	// �h�A�߂�
	for (const auto& door : m_apGymDoor)
	{
		door->SetEnableOpen(false);
	}

	if (m_pTimerUI != nullptr)
	{ // ��������Ă���ꍇ

		// �^�C�}�[UI�̕\��
		m_pTimerUI->SetEnableDisp(true);
	}

	if (m_pTimerBG != nullptr)
	{ // ��������Ă���ꍇ

		// �^�C�}�[�w�i�̕\��
		m_pTimerBG->SetEnableDisp(true);
	}

	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->SetEnableGaugeDisp(true);
		}
	}

	// �v���C���[�}�l�[�W���[�̐���
	CPlayerManager::Create(CPlayerManager::EType::TYPE_GAME);

	// ��������
	CReporter::Create(ETeamSide::SIDE_LEFT);
	CReporter::Create(ETeamSide::SIDE_RIGHT);

	// �Ǐ]�J�����̐ݒ�
	CCamera* pCamera = GET_MANAGER->GetCamera();	// �J�������
	pCamera->SetState(CCamera::STATE_FOLLOW);

	// �J�������[�V�����̏I��
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// �J�������[�V�������
	pCameraMotion->SetFinish(true);

	// �J�n���o�֑J��
	SetSceneType(ESceneType::SCENE_START);

	// BGM�Đ�
	CSound* pSound = CSound::GetInstance();
	pSound->StopSound();
	pSound->PlaySound(CSound::ELabel::LABEL_BGM_GAME);
}

//==========================================================================
// �ϋq�X�V
//==========================================================================
void CGameManager::UpdateAudience()
{
#if 1
	GET_MANAGER->GetDebugProc()->Print("\n----------------- �ϋq��� -----------------\n");
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		CTeamStatus::SCharmInfo info = m_pTeamStatus[i]->GetCharmInfo();	// ���e���
		float fMoteRate = info.fValue / info.fValueMax;				// ���e����
		int nNumAudience = (int)(CAudience::MAX_WATCH * fMoteRate);	// ���݂̊ϋq��

		// �ϋq����ݒ�
		CAudience::SetNumWatch(nNumAudience, (CGameManager::ETeamSide)(i));
		GET_MANAGER->GetDebugProc()->Print("�y�`�[��0%d�ϋq�z[%d]\n", i, nNumAudience);
	}
#endif	// �ϋq���o���Ȃ�
}

//==========================================================================
// �X�y�V�������o�X�V
//==========================================================================
void CGameManager::UpdateSpecial()
{

}

//==========================================================================
// �`�[���X�e�[�^�X�X�V
//==========================================================================
void CGameManager::UpdateTeamStatus()
{
	bool bAllDead[ETeamSide::SIDE_MAX] = { false, false };	// �s�k�t���O

	// �e�`�[�����
	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		if (m_pTeamStatus[i] == nullptr) continue;

		// �X�y�V�������Z
		AddSpecialValue(m_pTeamStatus[i]->GetTeam(), CSpecialValueManager::ETypeAdd::ADD_ALLWAYS);

#if _DEBUG
#if 0
		return;
#endif	// �f�o�b�O�p
#endif

		// �S�Ŕ���
		bAllDead[i] = m_pTeamStatus[i]->CheckAllDead();
	}

	if (bAllDead[ETeamSide::SIDE_LEFT] != bAllDead[ETeamSide::SIDE_RIGHT])
	{ // �ǂ��炩���S�ł����ꍇ

		for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
		{ // �`�[�������J��Ԃ�

			// ����オ�莞�Ԃ̐ݒ�
			int nDead = (int)bAllDead[i];	// �����ϊ��������S�t���O
			m_pCharmManager->SetHypeTime((ETeamSide)i, END_HYPE_TIME[i]);
		}
	}
}

//==========================================================================
// �Q�[�W���ԍX�V
//==========================================================================
void CGameManager::UpdateGaugeTime()
{
	// �X�y�V�����Q�[�W�̎��ԍX�V
	float fBrightTime = CGauge2D::GetBrightTime();
	fBrightTime += GET_MANAGER->GetDeltaTime() * GET_MANAGER->GetSlowRate();

	if (fBrightTime >= CGauge2D::GetBrightTimeEnd())
	{// ���[�v
		fBrightTime = 0.0f;
	}
	CGauge2D::SetBrightTime(fBrightTime);
	ImGui::Text("fBrightTime : % .2f", fBrightTime);
}

//==========================================================================
// �{�[���̎擾
//==========================================================================
CBall* CGameManager::GetBall()
{
	CListManager<CBall> list = CBall::GetListObj();	// �{�[�����X�g
	assert(list.GetNumAll() == 1);	// �����{�[��������ꍇ�G���[
	return *list.GetBegin();		// �{�[������Ԃ�
}

//==========================================================================
// �V�[���̎�ސݒ�
//==========================================================================
void CGameManager::SetSceneType(ESceneType type)
{
	m_OldSceneType = m_SceneType;
	m_SceneType = type;
	m_fSceneTimer = 0.0f;
}

//==========================================================================
// �^�C�}�[��~�t���O�ݒ�
//==========================================================================
void CGameManager::SetEnableTimerStop(const bool bStop)
{
	m_pTimerUI->EnableStop(bStop);
}

//==========================================================================
// @brief	�R�[�g�T�C�Y�擾(�`�[��)
// @param	team[in]	�擾�������`�[��
// @param	pos[out]	�擾�����R�[�g�̒��S�_
// @return	�R�[�g�T�C�Y
//==========================================================================
MyLib::Vector3 CGameManager::GetCourtSize(const ETeamSide team, MyLib::Vector3& pos)
{
	// �Б��R�[�g�T�C�Y
	MyLib::Vector3 size = m_courtSize;
	size.x *= 0.5f;

	// �`�[���ɉ������ʒu
	pos = MyLib::Vector3();
	switch (team)
	{
	case SIDE_LEFT:
		pos.x = -size.x;
		break;

	case SIDE_RIGHT:
		pos.x = size.x;
		break;

	default:
		break;
	}

	return size;
}
//==========================================================================
// @brief	�R�[�g���S�n�擾(�`�[��)
// @param	team[in]	�擾�������`�[��
// @return	�擾�����R�[�g�̒��S�_
//==========================================================================
MyLib::Vector3 CGameManager::GetCourtMiddle(const ETeamSide team)
{
	// �Б��R�[�g�T�C�Y
	MyLib::Vector3 size = m_courtSize;
	size.x *= 0.5f;

	// �`�[���ɉ������ʒu
	MyLib::Vector3 pos = MyLib::Vector3();
	switch (team)
	{
	case SIDE_LEFT:
		pos.x = -size.x;
		break;

	case SIDE_RIGHT:
		pos.x = size.x;
		break;

	default:
		pos.x = 0.0f;
		break;
	}

	return pos;
}

//==========================================================================
// @brief	�R�[�g�̉��p�[�ʒu���擾(�`�[��)
// @param	team[in]	�擾�������`�[��
// @param	pos[out]	�擾�����R�[�g�̒��S�_
// @return	�R�[�g�T�C�Y
//==========================================================================
float CGameManager::GetCourtPosPercentage(const ETeamSide team, MyLib::Vector3 pos)
{
	MyLib::Vector3 posCourt = MyLib::Vector3();
	MyLib::Vector3 sizeCourt = GetCourtSize(team, posCourt);

	// �ŏ���l
	float min = posCourt.x - sizeCourt.x;
	float max = posCourt.x + sizeCourt.x;

	// ����
	float fRate = UtilFunc::Transformation::ValueToRate(pos.x, min, max);

	if (team == ETeamSide::SIDE_LEFT)
	{// ���͊������]
		fRate = 1.0f - fRate;
	}

	return fRate;
}

//==========================================================================
// �`�[���R�[�g�̑Ίp���擾
//==========================================================================
float CGameManager::GetHalfCourtDiagonal()
{
	// �`�[���R�[�g�̑Ίp����Ԃ�
	return Court::HALF_DIAGONAL;
}

//==========================================================================
// �R�[�g�ړ�����
//==========================================================================
bool CGameManager::SetPosLimit(MyLib::Vector3& pos, const float fPlusRadius)
{
	bool bHit = false;

	if (pos.x > m_courtSize.x + fPlusRadius)
	{
		pos.x = m_courtSize.x + fPlusRadius;
		bHit = true;
	}
	else if (pos.x < -m_courtSize.x - fPlusRadius)
	{
		pos.x = -m_courtSize.x - fPlusRadius;
		bHit = true;
	}

	if (pos.z > m_courtSize.z + fPlusRadius)
	{
		pos.z = m_courtSize.z + fPlusRadius;
		bHit = true;
	}
	else if (pos.z < -m_courtSize.z - fPlusRadius)
	{
		pos.z = -m_courtSize.z - fPlusRadius;
		bHit = true;
	}

	return bHit;
}

//==========================================================================
// ���e���Z
//==========================================================================
void CGameManager::AddCharmValue(ETeamSide side, CCharmValueManager::ETypeAdd charmType)
{
	MyAssert::CustomAssert(side != CGameManager::ETeamSide::SIDE_NONE, "���e���`�[��is�ǂ��H�H");

	// �`�[���X�e�[�^�X
	float value = CCharmValueManager::GetInstance()->GetAddValue(charmType);
	m_pTeamStatus[(int)side]->AddCharmValue(value);

	assert(m_pCharmManager != nullptr);
	if (m_pCharmManager->GetPrisetHypeTime(charmType) > m_pCharmManager->GetHypeTime(side))
	{ // �ݒ�\��̐���オ�莞�Ԃ����̐���オ�莞�Ԃ�蒷���ꍇ

		// ����オ�莞�Ԃ̐ݒ�
		m_pCharmManager->SetHypeTime(side, charmType);
	}

	// ���e��������
	CCharmText::Create(side);
}

//==========================================================================
// ���e���Z
//==========================================================================
void CGameManager::SubCharmValue(ETeamSide side, CCharmValueManager::ETypeSub charmType)
{
	MyAssert::CustomAssert(side != CGameManager::ETeamSide::SIDE_NONE, "���e���`�[��is�ǂ��H�H");

	// �`�[���X�e�[�^�X
	float value = CCharmValueManager::GetInstance()->GetSubValue(charmType);
	m_pTeamStatus[(int)side]->SubCharmValue(value);

	// ����オ�莞�Ԃ̏�����
	assert(m_pCharmManager != nullptr);
	m_pCharmManager->SetHypeTime(side, 0.0f);
}

//==========================================================================
// �X�y�V�������Z
//==========================================================================
void CGameManager::AddSpecialValue(ETeamSide side, CSpecialValueManager::ETypeAdd ValueType)
{
	MyAssert::CustomAssert(side != CGameManager::ETeamSide::SIDE_NONE, "���e���`�[��is�ǂ��H�H");

	// �`�[���X�e�[�^�X
	float value = CSpecialValueManager::GetInstance()->GetAddValue(ValueType);
	m_pTeamStatus[(int)side]->AddSpecialValue(value);
}

//==========================================================================
// ���C�o���`�[���ɕϊ�
//==========================================================================
CGameManager::ETeamSide CGameManager::RivalTeam(ETeamSide team)
{
	switch (team)
	{
	case CGameManager::SIDE_LEFT:
		return SIDE_RIGHT;
		break;

	case CGameManager::SIDE_RIGHT:
		return SIDE_LEFT;
		break;

	default:
		return SIDE_NONE;
		break;
	}
}

//==========================================================================
// �`�[���X�e�[�^�X����
//==========================================================================
void CGameManager::CreateTeamStatus()
{
	ETeamSide side = CGameManager::SIDE_NONE;

	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->Uninit();
			m_pTeamStatus[i] = nullptr;
		}

		m_pTeamStatus[i] = CTeamStatus::Create();
		m_pTeamStatus[i]->SetEnableGaugeDisp(false);

		// �`�[���ݒ�
		side = static_cast<ETeamSide>(i);
		m_pTeamStatus[i]->TeamSetting(side);
	}
}

//==========================================================================
// �`�[���X�e�[�^�X�J�n��
//==========================================================================
void CGameManager::StartTeamStatus()
{
	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->SetEnableGaugeDisp(true);
		}
	}
}

//==========================================================================
// �`�[���X�e�[�^�X�ۑ�
//==========================================================================
void CGameManager::Save()
{
	// �t�@�C�����J��
	std::ofstream File(ResultManager::TEXTFILE);
	if (!File.is_open()) {
		return;
	}

	// ����
	int winteam = 0;
	float tension = 10.0f;

	// �e�L�X�g�t�@�C�����ڎ�
	File << TOP_LINE << std::endl;
	File << "# �`�[�����" << std::endl;
	File << TOP_LINE << std::endl;
	
	File << TEXT_LINE << std::endl;
	File << "# �������" << std::endl;
	File << TEXT_LINE << std::endl;
	File << "PRELUDE_WIN = " << m_endInfo.m_winteamPrelude << std::endl;
	File << "CONTEST_WIN = " << m_endInfo.m_winteamCharm << std::endl;
	File << "TENSION = "<< m_endInfo.m_fTension << std::endl;
	File << std::endl;

	int i = 0;
	for (const auto& team : m_pTeamStatus)
	{
		if (team == nullptr) continue;

		File << TEXT_LINE << std::endl;
		File << "# �`�[��" << i << std::endl;
		File << TEXT_LINE << std::endl;
		File << "SETTEAM" << std::endl;

		File << "	CHARMVALUE = " << team->GetCharmInfo().fValue << std::endl;
		File << std::endl;

		// ���X�g���[�v
		CListManager<CPlayer> list = CPlayerManager::GetInstance()->GetInList(static_cast<ETeamSide>(i));
		std::list<CPlayer*>::iterator itr = list.GetEnd();
		CPlayer* pObj = nullptr;

		while (list.ListLoop(itr))
		{
			pObj = (*itr);
		
			File << "	SETPLAYER"  << std::endl;

			File << "		HANDED = "		<< pObj->GetHandedness() << std::endl;		// ������
			File << "		BODY = "		<< pObj->GetBodyType() << std::endl;		// �̌^
			File << "		HAIR = "		<< 1 << std::endl;		// �������ւ�
			File << "		ACCESSORY= "	<< 1 << std::endl;		// �A�N�Z�����ւ�
			File << "		FACE = "		<< 1 << std::endl;		// �璅���ւ�

			File << "	END_SETPLAYER" << std::endl;
			File << std::endl;
		}

		File << "END_SETTEAM" << std::endl;
		File << std::endl;

		i++;
	}

	// �t�@�C�������
	File.close();
}

//==========================================================================
// �����I��
//==========================================================================
void CGameManager::EndGame()
{
	// TODO: �e���V���������߂�


	// �����`�[������
	CheckVictory();

	// �������e�ۑ�
	Save();
}

//==========================================================================
// �����`�[������
//==========================================================================
void CGameManager::CheckVictory()
{
	int m_aAlive[ETeamSide::SIDE_MAX] = {};		// ���l��
	int m_aLife[ETeamSide::SIDE_MAX] = {};		// ���̗�

	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		// ���X�g���[�v
		CListManager<CPlayer> list = CPlayerManager::GetInstance()->GetInList(static_cast<ETeamSide>(i));
		std::list<CPlayer*>::iterator itr = list.GetEnd();
		CPlayer* pObj = nullptr;

		while (list.ListLoop(itr))
		{
			pObj = (*itr);
		
			if (pObj->GetState() != CPlayer::EState::STATE_DEAD ||
				pObj->GetState() != CPlayer::EState::STATE_DEAD)
			{// �l�����Z
				m_aAlive[i]++;
			}

			// ���̗͉��Z
			m_aLife[i] += pObj->GetLife();
		}
	}

	if (m_aAlive[ETeamSide::SIDE_LEFT] > m_aAlive[ETeamSide::SIDE_RIGHT])
	{// ��������
		m_endInfo.m_winteamPrelude = ETeamSide::SIDE_LEFT;
	}
	else if(m_aAlive[ETeamSide::SIDE_LEFT] < m_aAlive[ETeamSide::SIDE_RIGHT])
	{// �E������
		m_endInfo.m_winteamPrelude = ETeamSide::SIDE_RIGHT;
	}
	else
	{// ���E����

		// �����̗͍�
		if (m_aLife[ETeamSide::SIDE_LEFT] > m_aLife[ETeamSide::SIDE_RIGHT])
		{// ��������
			m_endInfo.m_winteamPrelude = ETeamSide::SIDE_LEFT;
		}
		else if (m_aLife[ETeamSide::SIDE_LEFT] < m_aLife[ETeamSide::SIDE_RIGHT])
		{// �E������
			m_endInfo.m_winteamPrelude = ETeamSide::SIDE_RIGHT;
		}
		else
		{// ��������
			m_endInfo.m_winteamPrelude = ETeamSide::SIDE_NONE;
		}
	}

	//--------------------------
	//���e
	
	float fCharmL = m_pTeamStatus[CGameManager::ETeamSide::SIDE_LEFT]->GetCharmInfo().fValue;
	float fCharmR = m_pTeamStatus[CGameManager::ETeamSide::SIDE_RIGHT]->GetCharmInfo().fValue;


	// ���e����
	if (fCharmL > fCharmR)
	{// ��������
		m_endInfo.m_winteamCharm = ETeamSide::SIDE_LEFT;
	}
	else if (fCharmL < fCharmR)
	{// �E������
		m_endInfo.m_winteamCharm = ETeamSide::SIDE_RIGHT;
	}
	else
	{
		m_endInfo.m_winteamCharm = ETeamSide::SIDE_NONE;
	}
}

//==========================================================================
// �f�o�b�O
//==========================================================================
void CGameManager::Debug()
{
#if _DEBUG

	//-----------------------------
	// �R�[�g
	//-----------------------------
	if (ImGui::TreeNode("Court"))
	{
		if (m_pCourtSizeBox == nullptr)
		{
			m_pCourtSizeBox = CCollisionLine_Box::Create(MyLib::AABB(-m_courtSize, m_courtSize), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		if (ImGui::Button("Reset"))
		{// ���Z�b�g
			m_courtSize = Court::SIZE;
		}
		ImGui::SameLine();

		ImGui::DragFloat3("size", (float*)&m_courtSize, 10.0f, 0.0f, 10000.0f, "%.2f");

		// �T�C�Y�ݒ�
		if (m_pCourtSizeBox != nullptr)
		{
			m_pCourtSizeBox->SetAABB(MyLib::AABB(-m_courtSize, m_courtSize));
		}

		// �ʒu�ݒ�
		ImGui::TreePop();
	}

	// �^�C�}�[
	if (ImGui::TreeNode("Timer"))
	{

		if (m_pTimerUI != nullptr)
		{
			// ��~�󋵐؂�ւ�
			bool bStop = m_pTimerUI->IsStop();
			ImGui::Checkbox("bStop", &bStop);
			m_pTimerUI->EnableStop(bStop);
		}

		ImGui::TreePop();
	}

	// �`�[���X�e�[�^�X
	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		//-----------------------------
		// �R�[�g
		//-----------------------------
		std::string treename = "Special" + std::to_string(i);	// �c���[��
		if (ImGui::TreeNode(treename.c_str()))
		{
			float fValue = m_pTeamStatus[i]->GetSpecialValue();

			ImGui::DragFloat("GaugeValue", (float*)&fValue, 1.0f, 0.0f, m_pTeamStatus[i]->GetSpecialInfo().fValueMax, "%.2f");

			m_pTeamStatus[i]->SetSpecialValue(fValue);

			// �ʒu�ݒ�
			ImGui::TreePop();
		}

		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->Debug();
		}
	}

	// ����オ����(��u�H)
	if (ImGui::TreeNode("ExciteSetting"))
	{
		if (m_pCharmManager != nullptr)
		{
			if (ImGui::Button("Left"))
			{
				m_pCharmManager->SetHypeTime(CGameManager::ETeamSide::SIDE_LEFT, 10.0f);
			}
			if (ImGui::Button("Right"))
			{
				m_pCharmManager->SetHypeTime(CGameManager::ETeamSide::SIDE_RIGHT, 10.0f);
			}
		}
	
		ImGui::TreePop();
	}

	// �Q�[�W��max����
	if (ImGui::TreeNode("GaugeBright"))
	{
		float fBright = CGauge2D::GetBrightTime();
		float fBrightEnd = CGauge2D::GetBrightTimeEnd();

		ImGui::Text("BrightTime: [%.2f]", fBright);
		ImGui::DragFloat("fBrightEnd", &fBrightEnd, 0.01f, 0.0f, 10.0f, "%.2f");

		CGauge2D::SetBrightTimeEnd(fBrightEnd);

		ImGui::TreePop();
	}

	// ���e�l�}�l�[�W���[
	if (m_pCharmValueManager != nullptr)
	{
		m_pCharmValueManager->Debug();
	}

	// �I��
	if (ImGui::Button("end"))
	{
		SetSceneType(ESceneType::SCENE_END);
	}

	// �x��(L)
	if (ImGui::Button("(Caution: L)"))
	{
		CPlayerManager* pPlrMgr = CPlayerManager::GetInstance();
		pPlrMgr->CautionInAll(CGameManager::ETeamSide::SIDE_LEFT);
	}

	// �x��(R)
	if (ImGui::Button("(Caution: R)"))
	{
		CPlayerManager* pPlrMgr = CPlayerManager::GetInstance();
		pPlrMgr->CautionInAll(CGameManager::ETeamSide::SIDE_RIGHT);
	}

#endif
}
