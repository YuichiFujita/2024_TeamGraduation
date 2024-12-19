//=============================================================================
// 
//  �h���X�A�b�v�ݒ菈�� [entry_dressup.cpp]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
// 
//=============================================================================
#include "entry_dressup.h"
#include "entry_setupTeam.h"
#include "playerManager.h"
#include "dressupUI.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTFILE	= "data\\TEXT\\entry\\setupTeam.txt";
	const std::string TOP_LINE	= "#==============================================================================";	// �e�L�X�g�̃��C��
	const std::string TEXT_LINE	= "#------------------------------------------------------------------------------";	// �e�L�X�g�̃��C��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEntry_Dressup::CEntry_Dressup() : CEntryScene()
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

	int nNumPlayer = pSetupTeam->GetAllPlayerNum();	// �v���C���[�l��
	for (int i = 0; i < nNumPlayer; i++)
	{ // �v���C���[�l�����J��Ԃ�

		// TODO
		MyLib::Vector3 pos = MyLib::Vector3(160.0f, 450.0f, 0.0f);
		MyLib::Vector3 offset = MyLib::Vector3(320.0f * (float)i, 0.0f, 0.0f);

		// �h���X�A�b�vUI�̐���
		CDressupUI* pDressup = CDressupUI::Create(i, pos + offset);
		if (pDressup == nullptr) { return E_FAIL; }

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
	for (auto& info : m_vecDressInfo)
	{
		// �h���X�A�b�vUI�̏I��
		SAFE_UNINIT(info);
	}

	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CEntry_Dressup::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�̍X�V
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �`�[�����ݒ�
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) return;

	// ����̃T�C�Y
	for (auto& info : m_vecDressInfo)
	{
		// �h���X�A�b�vUI�̍X�V
		info->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// ��U�V�[���؂�ւ� TODO : �S���`�F�b�N���Ă���Ƃ��ɂ���
	if (pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_X))
	{
		// �Z�[�u����
		Save();

		CEntry::GetInstance()->ChangeEntryScene(CEntry::ESceneType::SCENETYPE_GAMESETTING);
	}
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
