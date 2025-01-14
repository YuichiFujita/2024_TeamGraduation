//=============================================================================
// 
//  �h���X�A�b�v�ݒ菈�� [entry_dressup.cpp]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
// 
//=============================================================================
#include "entry_dressup.h"
#include "manager.h"
#include "fade.h"
#include "entry_setupTeam.h"
#include "playerManager.h"
#include "dressupUI.h"
#include "entryRuleManager.h"
#include "inputKeyButton.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTFILE	= "data\\TEXT\\entry\\setupTeam.txt";
	const std::string TOP_LINE	= "#==============================================================================";	// �e�L�X�g�̃��C��
	const std::string TEXT_LINE	= "#------------------------------------------------------------------------------";	// �e�L�X�g�̃��C��

	namespace ui
	{
		namespace left
		{
			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, VEC3_SCREEN_CENT.y, 0.0f);	// �����S�ʒu
			const float OFFSET_X = 210.0f;	// X���W�I�t�Z�b�g
		}

		namespace right
		{
			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, VEC3_SCREEN_CENT.y, 0.0f);	// �E���S�ʒu
			const float OFFSET_X = 210.0f;	// X���W�I�t�Z�b�g
		}
	}
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEntry_Dressup::CEntry_Dressup() : CEntryScene(),
	m_pRuleManager	(nullptr),		// ���[���}�l�[�W���[
	m_state			(STATE_DRESSUP)	// ���
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEntry_Dressup::~CEntry_Dressup()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEntry_Dressup::Init()
{
	// �O��̒����ւ��Ǎ�
	Load();

	// �`�[���Z�b�g�A�b�v���̎擾
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) { return E_FAIL; }

	int nCurLeft = 0;	// ���݂̍��v���C���[��
	int nCurRight = 0;	// ���݂̉E�v���C���[��
	int nMaxLeft = pSetupTeam->GetPlayerNum(CGameManager::ETeamSide::SIDE_LEFT);	// ���v���C���[����
	int nMaxRight = pSetupTeam->GetPlayerNum(CGameManager::ETeamSide::SIDE_RIGHT);	// �E�v���C���[����
	for (int i = 0; i < CGameManager::MAX_PLAYER; i++)
	{ // �v���C���[�l�����J��Ԃ�

		// �`�[���w�肪�Ȃ��ꍇ����
		CGameManager::ETeamSide team = pSetupTeam->GetTeamSide(i);	// �`�[���T�C�h
		if (team == CGameManager::ETeamSide::SIDE_NONE) { continue; }

		CDressupUI* pDressup = nullptr;	// �h���X�A�b�vUI���
		MyLib::Vector3 posUI;	// UI�ʒu
		switch (team)
		{ // �`�[�����Ƃ̏���
		case CGameManager::ETeamSide::SIDE_LEFT:
		{
			// UI�̈ʒu���v�Z
			posUI = ui::left::POS;
			posUI.x = ui::left::POS.x - (ui::left::OFFSET_X * (float)(nMaxLeft - 1)) * 0.5f + (ui::left::OFFSET_X * (float)nCurLeft);

			// �`�[�������o�[�������Z
			nCurLeft++;
			break;
		}
		case CGameManager::ETeamSide::SIDE_RIGHT:
		{
			// UI�̈ʒu���v�Z
			posUI = ui::right::POS;
			posUI.x = ui::right::POS.x - (ui::right::OFFSET_X * (float)(nMaxRight - 1)) * 0.5f + (ui::right::OFFSET_X * (float)nCurRight);

			// �`�[�������o�[�������Z
			nCurRight++;
			break;
		}
		default:
			assert(false);
			break;
		}

		// �h���X�A�b�vUI�̐���
		pDressup = CDressupUI::Create(this, i, posUI);
		if (pDressup == nullptr)
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}

		// �h���X�A�b�vUI�̒ǉ�
		m_vecDressInfo.push_back(pDressup);
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEntry_Dressup::Uninit()
{
	// �G���g���[���[���̔j��
	SAFE_REF_RELEASE(m_pRuleManager);

	for (auto& rInfo : m_vecDressInfo)
	{ // �v�f�����J��Ԃ�

		// �h���X�A�b�vUI�̏I��
		SAFE_UNINIT(rInfo);
	}

	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CEntry_Dressup::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case EState::STATE_DRESSUP:

		// �`�[���ݒ�J��
		if (TransSetupTeam())
		{ // �J�ڂ���ꍇ

			return;
		}

		// �Q�[���ݒ�J��
		TransSetting();
		break;

	case EState::STATE_SETTING:

		// ���[���}�l�[�W���[�̍X�V
		assert(m_pRuleManager != nullptr);
		m_pRuleManager->Update(fDeltaTime, fDeltaRate, fSlowRate);
		break;

	case EState::STATE_END:

		// �Z�[�u����
		Save();

		// �Q�[����ʂɑJ�ڂ���
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_GAME);
		break;

	default:
		assert(false);
		break;
	}

	// �e�̍X�V
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CEntry_Dressup::SetState(const EState state)
{
	// ��Ԃ�ݒ�
	m_state = state;
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_DRESSUP:

		// �G���g���[���[���̔j��
		SAFE_REF_RELEASE(m_pRuleManager);
		break;

	case STATE_SETTING:

		if (m_pRuleManager == nullptr)
		{
			// �G���g���[���[���̐���
			m_pRuleManager = CEntryRuleManager::Create(this);
		}
		break;

	case STATE_END:
		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// �`�[���ݒ�J��
//==========================================================================
bool CEntry_Dressup::TransSetupTeam()
{
	// �����������Ă���ꍇ������
	if (IsAllReady()) { return false; }

	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���
	if (pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_B))
	{
		// �G���g���[���[���̔j��
		SAFE_REF_RELEASE(m_pRuleManager);

		for (auto& rInfo : m_vecDressInfo)
		{ // �v�f�����J��Ԃ�

			// �h���X�A�b�vUI�̏I��
			SAFE_UNINIT(rInfo);
		}

		// �`�[���ݒ�V�[���֑J��
		CEntry::GetInstance()->ChangeEntryScene(CEntry::ESceneType::SCENETYPE_SETUPTEAM);

		return true;
	}

	return false;
}

//==========================================================================
// �Q�[���ݒ�J��
//==========================================================================
void CEntry_Dressup::TransSetting()
{
	// ���������ł��Ă��Ȃ��ꍇ������
	if (!IsAllReady()) { return; }

	CInputGamepad* pPad = CInputGamepad::GetInstance();	// �p�b�h���
	if (pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_X))
	{
		// �Q�[���ݒ��ԂɑJ��
		SetState(EState::STATE_SETTING);
	}
}

//==========================================================================
// �����S�����t���O�̎擾����
//==========================================================================
bool CEntry_Dressup::IsAllReady()
{
	for (auto& rInfo : m_vecDressInfo)
	{ // �v�f�����J��Ԃ�

		// �������I����Ă���ꍇ����
		if (rInfo->IsReady()) { continue; }

		// ������������Ԃ�
		return false;
	}

	// �����S������Ԃ�
	return true;
}

//==========================================================================
// �Z�[�u����
//==========================================================================
void CEntry_Dressup::Save()
{
	// �v���C���[�}�l�[�W���擾
	CPlayerManager* pPlayerMgr = CPlayerManager::GetInstance();

	// �ǂݍ��ݏ��
	std::vector<CPlayerManager::LoadInfo> vecSaveInfo[CGameManager::ETeamSide::SIDE_MAX];

	// ����̃T�C�Y
	int size = static_cast<int>(m_vecDressInfo.size());
	for (int i = 0; i < size; i++)
	{
		// ���g�̑��삷��C���f�b�N�X�ԍ��擾
		int controllIdx = m_vecDressInfo[i]->GetMyPlayerIdx();
		CGameManager::ETeamSide side = m_vecDressInfo[i]->GetTeam();

		// �ǂݍ��ݏ��
		vecSaveInfo[side].emplace_back();
		CPlayerManager::LoadInfo* pLoadInfo = &vecSaveInfo[side].back();
		pLoadInfo->nControllIdx = controllIdx;								// ���g�̑��삷��C���f�b�N�X�ԍ��擾
		pLoadInfo->nHair = m_vecDressInfo[i]->GetHairNowIdx();				// ���̃C���f�b�N�X�ԍ�
		pLoadInfo->nAccessory = m_vecDressInfo[i]->GetAccessoryNowIdx();	// �A�N�Z�̃C���f�b�N�X�ԍ�
		pLoadInfo->nFace = m_vecDressInfo[i]->GetFaceNowIdx();				// ��̃C���f�b�N�X�ԍ�
		pLoadInfo->eBody = m_vecDressInfo[i]->GetBodyType();				// �̌^
		pLoadInfo->eHanded = m_vecDressInfo[i]->GetHandedness();			// ������
	}

	// �Z�[�u����
	pPlayerMgr->Save(vecSaveInfo[CGameManager::ETeamSide::SIDE_LEFT], vecSaveInfo[CGameManager::ETeamSide::SIDE_RIGHT]);

	// �Q�[���ݒ�̕ۑ�
	assert(m_pRuleManager != nullptr);
	CEntryRuleManager::SRule rule = m_pRuleManager->GetRule();	// ���[��
	CEntryRuleManager::SaveSetting(&rule);
}

//==========================================================================
// ���[�h
//==========================================================================
void CEntry_Dressup::Load()
{
	// �t�@�C�����J��
	std::ifstream File(TEXTFILE);
	if (!File.is_open()) {
		return;
	}

	// �R�����g�p
	std::string hoge;

	// �f�[�^�ǂݍ���
	std::string line;
	while (std::getline(File, line))
	{
		// �R�����g�̓X�L�b�v
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("PLAYERNUM") != std::string::npos)
		{// PLAYERNUM�Ń`�[�����̃v���C���[���ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);
			
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// �t�@�C�������
	File.close();
}

//==========================================================================
// �f�o�b�O����
//==========================================================================
void CEntry_Dressup::Debug()
{

	//--------------------------
	// �V�[���J��
	//--------------------------
	if (IsAllReady() &&
		CInputKeyboard::GetInstance()->GetTrigger(DIK_X))
	{
		// �Q�[���ݒ��ԂɑJ��
		SetState(EState::STATE_SETTING);
	}

	if (ImGui::TreeNode("SetUpTeam"))
	{
		//=============================
		// �Z�[�u
		//=============================s
		if (ImGui::Button("Save"))
		{
			Save();
		}

		// ����̃T�C�Y
		int size = static_cast<int>(m_vecDressInfo.size());
		for (int i = 0; i < size; i++)
		{
			std::string treename = "Info : " + std::to_string(i);	// �c���[��
			if (ImGui::TreeNode(treename.c_str()))
			{
				const auto& info = m_vecDressInfo[i];

				ImGui::Text("editType[%s]", magic_enum::enum_name(info->GetTypeEdit()));
				ImGui::Text("changeType[%s]", magic_enum::enum_name(info->GetTypeChange()));
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}
