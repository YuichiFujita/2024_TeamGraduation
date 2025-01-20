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
#include "inputKeyButton.h"
#include "string2D.h"
#include "dressupUI.h"
#include "selectUI.h"
#include "object2D.h"
#include "entryRuleManager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTFILE	= "data\\TEXT\\entry\\setupTeam.txt";
	const std::string TOP_LINE	= "#==============================================================================";	// �e�L�X�g�̃��C��
	const std::string TEXT_LINE	= "#------------------------------------------------------------------------------";	// �e�L�X�g�̃��C��
	const int PRIORITY = 6;	// �D�揇��

	namespace ui
	{
		namespace left
		{
			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, VEC3_SCREEN_CENT.y - 40.0f, 0.0f);	// �����S�ʒu
			const float OFFSET_X = 210.0f;	// X���W�I�t�Z�b�g
		}

		namespace right
		{
			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, VEC3_SCREEN_CENT.y - 40.0f, 0.0f);	// �E���S�ʒu
			const float OFFSET_X = 210.0f;	// X���W�I�t�Z�b�g
		}

		namespace name
		{
			const MyLib::Vector3 POS[] = { MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, 75.0f, 0.0f), MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, 75.0f, 0.0f) };	// ���_�ʒu
			const char*	FONT		= "data\\FONT\\�ʂ˂�������������v7��.ttf";	// �t�H���g�p�X
			const bool	ITALIC		= false;					// �C�^���b�N
			const float	HEIGHT		= 42.0f;					// �����c��
			const EAlignX ALIGN_X	= XALIGN_CENTER;			// ���z�u
			const D3DXCOLOR COL		= MyLib::color::Black();	// �F
		}

		namespace back
		{
			const MyLib::Vector3 POS	= MyLib::Vector3(VEC3_SCREEN_CENT.x - 420.0f, 640.0f, 0.0f);	// �ʒu
			const MyLib::Vector2 SIZE	= MyLib::Vector2(180.0f, 45.0f);	// �傫��
		}

		namespace enter
		{
			const MyLib::Vector3 POS	= MyLib::Vector3(VEC3_SCREEN_CENT.x + 420.0f, 640.0f, 0.0f);	// �ʒu
			const MyLib::Vector2 SIZE	= MyLib::Vector2(180.0f, 45.0f);	// �傫��
		}
	}
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEntry_Dressup::CEntry_Dressup() : CEntryScene(),
	m_pRuleManager	(nullptr),		// ���[���}�l�[�W���[
	m_pBack			(nullptr),		// �߂���
	m_pEnter		(nullptr),		// ������
	m_state			(STATE_DRESSUP)	// ���
{
	// �����o�ϐ����N���A
	memset(&m_apTeamName[0], 0, sizeof(m_apTeamName));	// �`�[�������
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
	int nMaxLeft = pSetupTeam->GetPlayerNum(CGameManager::ETeamSide::SIDE_LEFT);	// ���v���C���[����
	for (int nPlayerIdx = 0; nPlayerIdx < CGameManager::MAX_PLAYER; nPlayerIdx++)
	{ // �v���C���[�l�����J��Ԃ�

		// �`�[���w�肪�Ȃ��ꍇ����
		CGameManager::ETeamSide team = pSetupTeam->GetTeamSide(nPlayerIdx);	// �`�[���T�C�h
		if (team != CGameManager::ETeamSide::SIDE_LEFT) { continue; }

		// UI�̈ʒu���v�Z
		MyLib::Vector3 posUI = ui::left::POS;	// UI�ʒu
		posUI.x = ui::left::POS.x - (ui::left::OFFSET_X * (float)(nMaxLeft - 1)) * 0.5f + (ui::left::OFFSET_X * (float)nCurLeft);

		// �`�[�������o�[�������Z
		nCurLeft++;

		// �����ւ�UI�̐���
		CDressupUI* pDressup = CDressupUI::Create(this, nPlayerIdx, posUI);
		if (pDressup == nullptr)
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}

		// �����ւ�UI�̒ǉ�
		m_vecDressInfo.push_back(pDressup);

		const int nPadIdx = pSetupTeam->PlayerIdxToPadIdx(nPlayerIdx);	// ���쌠�C���f�b�N�X
		if (nPadIdx > -1)
		{ // �v���C���[�̏ꍇ

			// �I��UI�̐���
			CSelectUI* pSelect = CSelectUI::Create(team, nCurLeft - 1, nPadIdx, posUI);
			if (pSelect == nullptr)
			{ // �����Ɏ��s�����ꍇ

				return E_FAIL;
			}

			// �I��UI�̒ǉ�
			m_vecSelect.push_back(pSelect);
		}
	}

	int nCurRight = 0;	// ���݂̉E�v���C���[��
	int nMaxRight = pSetupTeam->GetPlayerNum(CGameManager::ETeamSide::SIDE_RIGHT);	// �E�v���C���[����
	for (int nPlayerIdx = 0; nPlayerIdx < CGameManager::MAX_PLAYER; nPlayerIdx++)
	{ // �v���C���[�l�����J��Ԃ�

		// �`�[���w�肪�Ȃ��ꍇ����
		CGameManager::ETeamSide team = pSetupTeam->GetTeamSide(nPlayerIdx);	// �`�[���T�C�h
		if (team != CGameManager::ETeamSide::SIDE_RIGHT) { continue; }

		// UI�̈ʒu���v�Z
		MyLib::Vector3 posUI = ui::right::POS;	// UI�ʒu
		posUI.x = ui::right::POS.x - (ui::right::OFFSET_X * (float)(nMaxRight - 1)) * 0.5f + (ui::right::OFFSET_X * (float)nCurRight);

		// �`�[�������o�[�������Z
		nCurRight++;

		// �����ւ�UI�̐���
		CDressupUI* pDressup = CDressupUI::Create(this, nPlayerIdx, posUI);
		if (pDressup == nullptr)
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}

		// �����ւ�UI�̒ǉ�
		m_vecDressInfo.push_back(pDressup);

		const int nPadIdx = pSetupTeam->PlayerIdxToPadIdx(nPlayerIdx);	// ���쌠�C���f�b�N�X
		if (nPadIdx > -1)
		{ // �v���C���[�̏ꍇ

			// �I��UI�̐���
			CSelectUI* pSelect = CSelectUI::Create(team, nCurLeft + nCurRight - 1, nPadIdx, posUI);
			if (pSelect == nullptr)
			{ // �����Ɏ��s�����ꍇ

				return E_FAIL;
			}

			// �I��UI�̒ǉ�
			m_vecSelect.push_back(pSelect);
		}
	}

	for (int i = 0; i < CGameManager::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�

		// �`�[�����̐���
		m_apTeamName[i] = CString2D::Create
		( // ����
			ui::name::FONT,		// �t�H���g�p�X
			ui::name::ITALIC,	// �C�^���b�N
			L"�����Ƀ`�[����",	// �w�蕶����
			ui::name::POS[i],	// ���_�ʒu
			ui::name::HEIGHT,	// �����c��
			ui::name::ALIGN_X,	// ���z�u
			VEC3_ZERO,			// ���_����
			ui::name::COL		// �F
		);
		if (m_apTeamName[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}
	}

	// �߂�̐���
	m_pBack = CObject2D::Create(PRIORITY);
	if (m_pBack == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �ʒu��ݒ�
	m_pBack->SetPosition(ui::back::POS);

	// �傫����ݒ�
	m_pBack->SetSize(ui::back::SIZE);

	// ����̐���
	m_pEnter = CObject2D::Create(PRIORITY);
	if (m_pEnter == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �ʒu��ݒ�
	m_pEnter->SetPosition(ui::enter::POS);

	// �傫����ݒ�
	m_pEnter->SetSize(ui::enter::SIZE);

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

		// �����ւ�UI�̏I��
		SAFE_UNINIT(rInfo);
	}

	for (auto& rSelect : m_vecSelect)
	{ // �v�f�����J��Ԃ�

		// �I��UI�̏I��
		SAFE_UNINIT(rSelect);
	}

	for (int i = 0; i < CGameManager::SIDE_MAX; i++)
	{ // �`�[�������J��Ԃ�

		// �`�[�����̏I��
		SAFE_UNINIT(m_apTeamName[i]);
	}

	// �߂�̏I��
	SAFE_UNINIT(m_pBack);

	// ����̏I��
	SAFE_UNINIT(m_pEnter);

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
// �����ւ�UI���������t���O�̐ݒ�
//==========================================================================
void CEntry_Dressup::SetDressUIReady(const int nPlayerIdx, const bool bReady)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ�G���[
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumPlayer()) { assert(false); return; }

	// ���������t���O��Ԃ�
	m_vecDressInfo[nPlayerIdx]->SetReady(bReady);
}

//==========================================================================
// �����ւ�UI�̏��������ς݂��̊m�F
//==========================================================================
bool CEntry_Dressup::IsDressUIReady(const int nPlayerIdx)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ�G���[
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumPlayer()) { assert(false); return false; }

	// ���������t���O��Ԃ�
	return m_vecDressInfo[nPlayerIdx]->IsReady();
}

//==========================================================================
// �����ւ�UI���쌠�̐ݒ�
//==========================================================================
void CEntry_Dressup::SetDressUIControl(const int nPadIdx, const int nPlayerIdx)
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ�G���[
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumPlayer()) { assert(false); return; }

	// �����v���C���[�̒����ւ�UI�ʒu��Ԃ�
	m_vecDressInfo[nPlayerIdx]->SetPadIdx(nPadIdx);
}

//==========================================================================
// �I���\���̊m�F
//==========================================================================
bool CEntry_Dressup::IsSelectOK(const int nPadIdx, const int nPlayerIdx) const
{
	// �����ȊO�̃��[�U�[�̒����ւ�UI�̏ꍇ�I��s��
	const int nSelectPadIdx = m_vecDressInfo[nPlayerIdx]->GetMyPlayerIdx();	// �I��\���̑��쌠�C���f�b�N�X
	if (nSelectPadIdx > -1 && nSelectPadIdx != nPadIdx) { return false; }

	for (const auto& rSelect : m_vecSelect)
	{ // �v�f�����J��Ԃ�

		// ���Ɏ����ȊO�̃��[�U�[���I�𒆂̏ꍇ�I��s��
		if (rSelect->GetSelectIdx().x == nPlayerIdx
		&&  rSelect->GetPadIdx()	  != nPadIdx) { return false; }
	}

	return true;
}

//==========================================================================
// �I��UI�I�𑀍�t���O�̐ݒ�
//==========================================================================
void CEntry_Dressup::SetSelectUISelect(const int nPadIdx, const bool bSelect)
{
	for (auto& rSelect : m_vecSelect)
	{ // �v�f�����J��Ԃ�

		// �I��UI�̑��쌠�C���f�b�N�X�������̑��쌠�C���f�b�N�X�ƈ�v���Ȃ��ꍇ����
		if (rSelect->GetPadIdx() != nPadIdx) { continue; }

		// �I�𑀍�\�t���O�̐ݒ�
		return rSelect->SetSelect(bSelect);
	}
}

//==========================================================================
// �����ւ�UI�ʒu�擾
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetDressUIPosition(const int nPlayerIdx) const
{
	// �v���C���[�C���f�b�N�X���͈͊O�̏ꍇ�G���[
	if (nPlayerIdx <= -1 || nPlayerIdx >= GetNumPlayer()) { assert(false); return VEC3_ZERO; }

	// �����v���C���[�̒����ւ�UI�ʒu��Ԃ�
	return m_vecDressInfo[nPlayerIdx]->GetPosition();
}

//==========================================================================
// ���OUI�ʒu�擾
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetNameUIPosition(const CGameManager::ETeamSide team)
{
	// ������̈ʒu��Ԃ�
	return m_apTeamName[team]->GetPosition();
}

//==========================================================================
// ���OUI�傫���擾
//==========================================================================
MyLib::Vector2 CEntry_Dressup::GetNameUISize(const CGameManager::ETeamSide team)
{
	// ������̑傫����Ԃ�
	return MyLib::Vector2(m_apTeamName[team]->GetStrWidth(), ui::name::HEIGHT);
}

//==========================================================================
// �߂�UI�ʒu�擾
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetBackUIPosition()
{
	// �|���S���̈ʒu��Ԃ�
	return m_pBack->GetPosition();
}

//==========================================================================
// �߂�UI�傫���擾
//==========================================================================
MyLib::Vector2 CEntry_Dressup::GetBackUISize()
{
	// �|���S���̑傫����Ԃ�
	return m_pBack->GetSize();
}

//==========================================================================
// ����UI�ʒu�擾
//==========================================================================
MyLib::Vector3 CEntry_Dressup::GetEnterUIPosition()
{
	// �|���S���̈ʒu��Ԃ�
	return m_pEnter->GetPosition();
}

//==========================================================================
// ����UI�傫���擾
//==========================================================================
MyLib::Vector2 CEntry_Dressup::GetEnterUISize()
{
	// �|���S���̑傫����Ԃ�
	return m_pEnter->GetSize();
}

//==========================================================================
// �Q�[���ݒ�J��
//==========================================================================
bool CEntry_Dressup::TransSetting()
{
	// ���������ł��Ă��Ȃ��ꍇ������
	if (!IsAllReady()) { return false; }

	// �Q�[���ݒ��ԂɑJ��
	SetState(EState::STATE_SETTING);

	return true;
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
		int nPadIdx = m_vecDressInfo[i]->GetPadIdx();
		CGameManager::ETeamSide side = m_vecDressInfo[i]->GetTeam();

		// �ǂݍ��ݏ��
		vecSaveInfo[side].emplace_back();
		CPlayerManager::LoadInfo* pLoadInfo = &vecSaveInfo[side].back();
		pLoadInfo->nControllIdx = nPadIdx;									// ���g�̑��삷��C���f�b�N�X�ԍ��擾
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
