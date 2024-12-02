//=============================================================================
// 
// �h���X�A�b�v�ݒ菈�� [entry_dressup.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "entry_dressup.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "entry_setupTeam.h"
#include "dressup.h"
#include "playerManager.h"

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
	// �ǂݍ���
	Load();

	// �`�[�����ݒ�
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) return E_FAIL;

	// ���v���C���[���擾
	int nPlyerNum = pSetupTeam->GetAllPlayerNum();

	// �����ւ���񃊃T�C�Y
	m_vecDressupInfo.resize(nPlyerNum);

	// ��������
	for (int i = 0; i < nPlyerNum; i++)
	{
		// �G���g���[�����ԍ�
		int entryIdx = pSetupTeam->GetEntryIdx(i);

		CGameManager::ETeamSide side;
		if (entryIdx >= 0)
		{// User
			side = pSetupTeam->GetTeamSide(entryIdx);
		}
		else
		{// AI
			side = pSetupTeam->GetTeamSide(i);
		}

		// �v���C���[����
		MyLib::Vector3 pos = MyLib::Vector3(-400.0f, 0.0f, 0.0f);
		MyLib::Vector3 offset = MyLib::Vector3(200.0f * i, 0.0f, 0.0f);
		m_vecDressupInfo[i].pPlayer = CPlayer::Create
		(
			pos + offset, 						// �ʒu
			side,								// �`�[���T�C�h
			CPlayer::EHuman::HUMAN_ENTRY,		// �|�W�V����
			CPlayer::EBody::BODY_NORMAL,		// �̌n
			CPlayer::EHandedness::HAND_R		// ������
		);
		m_vecDressupInfo[i].pPlayer->CObject::SetOriginPosition(pos + offset);

		// �C���f�b�N�X�㏑��
		m_vecDressupInfo[i].pPlayer->SetMyPlayerIdx(entryIdx);

		// �������ւ�����
		m_vecDressupInfo[i].pHair = CDressup::Create(
			CDressup::EType::TYPE_HAIR,		// �����ւ��̎��
			m_vecDressupInfo[i].pPlayer,	// �ύX����v���C���[
			CPlayer::ID_HAIR);				// �ύX�ӏ��̃C���f�b�N�X
		
		// �A�N�Z�؂�ւ�����
		m_vecDressupInfo[i].pAccessory = CDressup::Create(
			CDressup::EType::TYPE_ACCESSORY,		// �����ւ��̎��
			m_vecDressupInfo[i].pPlayer,	// �ύX����v���C���[
			CPlayer::ID_ACCESSORY);			// �ύX�ӏ��̃C���f�b�N�X

		// ��؂�ւ�����
		m_vecDressupInfo[i].pFace = CDressup::Create(
			CDressup::EType::TYPE_FACE,		// �����ւ��̎��
			m_vecDressupInfo[i].pPlayer,	// �ύX����v���C���[
			CPlayer::ID_FACE);				// �ύX�ӏ��̃C���f�b�N�X

		// ���삷��C���f�b�N�X�ݒ�
		m_vecDressupInfo[i].pHair->SetControllIdx(entryIdx);
		m_vecDressupInfo[i].pAccessory->SetControllIdx(entryIdx);
		m_vecDressupInfo[i].pFace->SetControllIdx(entryIdx);
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEntry_Dressup::Uninit()
{
	for (auto& info : m_vecDressupInfo)
	{
		info.pHair->Uninit();
		info.pAccessory->Uninit();
		info.pFace->Uninit();
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
	int size = static_cast<int>(m_vecDressupInfo.size());
	for (int i = 0; i < size; i++)
	{
		// �G���g���[�����ԍ���
		int entryIdx = pSetupTeam->GetEntryIdx(i);
		if (entryIdx < 0) continue;
		if (entryIdx >= mylib_const::MAX_PLAYER) continue;

		// �G�f�B�b�g�����ޕύX
		ChangeEditType(i, entryIdx);

		switch (m_vecDressupInfo[i].editType)
		{
		case EEditType::EDIT_PROCESS:

			switch (m_vecDressupInfo[i].changeType)
			{
			case EChangeType::TYPE_HAIR:

				// ���X�V
				m_vecDressupInfo[i].pHair->Update(fDeltaTime, fDeltaRate, fSlowRate);
				break;

			case EChangeType::TYPE_ACCESSORY:

				// �A�N�Z�X�V
				m_vecDressupInfo[i].pAccessory->Update(fDeltaTime, fDeltaRate, fSlowRate);
				break;

			case EChangeType::TYPE_FACE:

				// ��X�V
				m_vecDressupInfo[i].pFace->Update(fDeltaTime, fDeltaRate, fSlowRate);
				break;

			case EChangeType::TYPE_BODY:

				// �̌^�ύX
				ChangeBodyType(i, entryIdx);
				break;

			case EChangeType::TYPE_HANDEDNESS:

				// ������ύX
				ChangeHandedness(i, entryIdx);
				break;

			default:
				break;
			}
			break;

		case EEditType::EDIT_CHANGETYPE:
			// �ύX����ӏ��̎�ޕύX
			ChangeChangeType(i, entryIdx);
			break;

		default:
			assert(false);
			break;
		}
	}


	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// ��U�V�[���؂�ւ� TODO : �S���`�F�b�N���Ă���Ƃ��ɂ���
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, 0))
	{
		// �Z�[�u����
		Save();

		CEntry::GetInstance()->ChangeEntryScene(CEntry::ESceneType::SCENETYPE_GAMESETTING);
	}

}

//==========================================================================
// �G�f�B�b�g�����ޕύX
//==========================================================================
void CEntry_Dressup::ChangeEditType(int nLoop, int nControllIdx)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, nControllIdx))
	{// �ύX����ӏ��̎�ޕύX�֕ύX

		m_vecDressupInfo[nLoop].editType = EEditType::EDIT_PROCESS;
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, nControllIdx))
	{// ���ۂ̕ύX��ʂ�

		m_vecDressupInfo[nLoop].editType = EEditType::EDIT_CHANGETYPE;
	}
}

//==========================================================================
// �ύX����ӏ��̎�ޕύX
//==========================================================================
void CEntry_Dressup::ChangeChangeType(int nLoop, int nControllIdx)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nControllIdx))
	{// ���[�v

		// ���֕ύX
		int changeType = (m_vecDressupInfo[nLoop].changeType + 1) % EChangeType::TYPE_MAX;
		m_vecDressupInfo[nLoop].changeType = static_cast<EChangeType>(changeType);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nControllIdx))
	{// �t���[�v

		// �O�֕ύX
		int changeType = (m_vecDressupInfo[nLoop].changeType + (EChangeType::TYPE_MAX - 1)) % EChangeType::TYPE_MAX;
		m_vecDressupInfo[nLoop].changeType = static_cast<EChangeType>(changeType);
	}
}

//==========================================================================
// �̌^�ύX
//==========================================================================
void CEntry_Dressup::ChangeBodyType(int nLoop, int nControllIdx)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �̌^�擾
	CPlayer::EBody body = m_vecDressupInfo[nLoop].pPlayer->GetBodyType();
	CPlayer::EBody oldBody = body;

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nControllIdx))
	{// ���[�v

		// ���֕ύX
		int afterBody = (body + 1) % CPlayer::EBody::BODY_MAX;
		body = static_cast<CPlayer::EBody>(afterBody);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nControllIdx))
	{// �t���[�v

		// �O�֕ύX
		int deforeBody = (body + (CPlayer::EBody::BODY_MAX - 1)) % CPlayer::EBody::BODY_MAX;
		body = static_cast<CPlayer::EBody>(deforeBody);
	}

	if (body != oldBody)
	{// �ύX����Ă�����

		// �v���C���[�Đ���
		ReCreatePlayer(nLoop, m_vecDressupInfo[nLoop].pPlayer->GetHandedness(), body);
	}

}

//==========================================================================
// ������ύX
//==========================================================================
void CEntry_Dressup::ChangeHandedness(int nLoop, int nControllIdx)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// ������擾
	CPlayer::EHandedness handedness = m_vecDressupInfo[nLoop].pPlayer->GetHandedness();
	CPlayer::EHandedness oldHandedness = handedness;

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, nControllIdx))
	{// ���[�v

		// ���֕ύX
		int afterHandedness = (handedness + 1) % CPlayer::EHandedness::HAND_MAX;
		handedness = static_cast<CPlayer::EHandedness>(afterHandedness);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, nControllIdx))
	{// �t���[�v

		// �O�֕ύX
		int deforeHandedness = (handedness + (CPlayer::EHandedness::HAND_MAX - 1)) % CPlayer::EHandedness::HAND_MAX;
		handedness = static_cast<CPlayer::EHandedness>(deforeHandedness);
	}

	if (handedness != oldHandedness)
	{// �ύX����Ă�����

		// �v���C���[�Đ���
		ReCreatePlayer(nLoop, handedness, m_vecDressupInfo[nLoop].pPlayer->GetBodyType());
	}
}

//==========================================================================
// �v���C���[�Đ���
//==========================================================================
void CEntry_Dressup::ReCreatePlayer(int i, CPlayer::EHandedness handedness, CPlayer::EBody body)
{
	// �`�[�����ݒ�
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) return;

	// ���ۑ�
	MyLib::Vector3 pos = m_vecDressupInfo[i].pPlayer->CObject::GetOriginPosition();

	// �폜
	m_vecDressupInfo[i].pPlayer->Kill();

	// �G���g���[�����ԍ�
	int entryIdx = pSetupTeam->GetEntryIdx(i);

	CGameManager::ETeamSide side;
	if (entryIdx >= 0)
	{// User
		side = pSetupTeam->GetTeamSide(entryIdx);
	}
	else
	{// AI
		side = pSetupTeam->GetTeamSide(i);
	}

	// �Đ���
	m_vecDressupInfo[i].pPlayer = CPlayer::Create
	(
		pos, 								// �ʒu
		side,								// �`�[���T�C�h
		CPlayer::EHuman::HUMAN_ENTRY,		// �|�W�V����
		body,								// �̌n
		handedness							// ������
	);

	// �C���f�b�N�X�㏑��
	m_vecDressupInfo[i].pPlayer->SetMyPlayerIdx(entryIdx);

	// ���̈ʒu�ݒ�
	m_vecDressupInfo[i].pPlayer->CObject::SetPosition(pos);
	m_vecDressupInfo[i].pPlayer->CObject::SetOriginPosition(pos);

	// �h���X�A�b�v�ɍĊ��蓖��
	m_vecDressupInfo[i].pHair->BindObjectCharacter(m_vecDressupInfo[i].pPlayer);
	m_vecDressupInfo[i].pAccessory->BindObjectCharacter(m_vecDressupInfo[i].pPlayer);
	m_vecDressupInfo[i].pFace->BindObjectCharacter(m_vecDressupInfo[i].pPlayer);

	m_vecDressupInfo[i].pHair->ReRegist();
	m_vecDressupInfo[i].pAccessory->ReRegist();
	m_vecDressupInfo[i].pFace->ReRegist();
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
	int size = static_cast<int>(m_vecDressupInfo.size());
	for (int i = 0; i < size; i++)
	{
		// ���g�̑��삷��C���f�b�N�X�ԍ��擾
		int controllIdx = m_vecDressupInfo[i].pPlayer->GetMyPlayerIdx();
		CGameManager::ETeamSide side = m_vecDressupInfo[i].pPlayer->GetTeam();

		// �ǂݍ��ݏ��
		vecSaveInfo[side].emplace_back();
		CPlayerManager::LoadInfo* pLoadInfo = &vecSaveInfo[side].back();
		pLoadInfo->nControllIdx = controllIdx;									// ���g�̑��삷��C���f�b�N�X�ԍ��擾
		pLoadInfo->nHair = m_vecDressupInfo[i].pHair->GetNowIdx();				// ���̃C���f�b�N�X�ԍ�
		pLoadInfo->nAccessory = m_vecDressupInfo[i].pAccessory->GetNowIdx();	// �A�N�Z�̃C���f�b�N�X�ԍ�
		pLoadInfo->nFace = m_vecDressupInfo[i].pFace->GetNowIdx();				// ��̃C���f�b�N�X�ԍ�
		pLoadInfo->eBody = m_vecDressupInfo[i].pPlayer->GetBodyType();			// �̌^
		pLoadInfo->eHanded = m_vecDressupInfo[i].pPlayer->GetHandedness();		// ������
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
		int size = static_cast<int>(m_vecDressupInfo.size());
		for (int i = 0; i < size; i++)
		{
			std::string treename = "Info : " + std::to_string(i);	// �c���[��
			if (ImGui::TreeNode(treename.c_str()))
			{
				const auto& info = m_vecDressupInfo[i];

				ImGui::Text("editType[%s]", magic_enum::enum_name(info.editType));
				ImGui::Text("changeType[%s]", magic_enum::enum_name(info.changeType));
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}
