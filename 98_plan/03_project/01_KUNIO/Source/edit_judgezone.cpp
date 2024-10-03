//=============================================================================
// 
//  ����]�[���G�f�B�^�\���� [edit_judgezone.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "edit_judgezone.h"
#include "judge.h"
#include "judgezoneManager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEdit_JudgeZone::CEdit_JudgeZone()
{
	// �l�̃N���A
	m_bHoverWindow = false;	// �}�E�X�̃E�B���h�E�z�o�[����
	for (int cntborder = 0; cntborder < CJudge::BORDER::MAX; cntborder++)
	{
		for (int cntjudge = 0; cntjudge < CJudge::JUDGE::JUDGE_MAX; cntjudge++)
		{
			m_abEnable[cntborder][cntjudge] = true;
		}
		m_aConditionType[cntborder] = CJudge::CONDITIONTYPE::TYPE_NONE;
	}
	m_JudgeZoneData.conditionTop.type = CJudge::CONDITIONTYPE::TYPE_NONE;
	m_JudgeZoneData.conditionUnder.type = CJudge::CONDITIONTYPE::TYPE_NONE;
	m_pSelectZone = nullptr;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEdit_JudgeZone::~CEdit_JudgeZone()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEdit_JudgeZone::Init()
{

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEdit_JudgeZone::Uninit()
{
	// �I������
	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_JudgeZone::Update()
{
	// �E�B���h�E�̃}�E�X�z�o�[����
	ImGuiHoveredFlags frag = 128;
	m_bHoverWindow = ImGui::IsWindowHovered(frag);

	// �]�[���I��
	SelectZone();

	// �t�@�C������
	FileControl();

	// ����]�[���ݒ�
	SetJudgeZone();
}

//==========================================================================
// �]�[���I��
//==========================================================================
void CEdit_JudgeZone::SelectZone()
{
	std::map<std::string, CJudgeZone*> item;

	CListManager<CJudgeZone> list = CJudgeZone::GetListObj();
	auto itr = list.GetEnd();

	// �����\��
	int cnt = 0;
	while (list.ListLoop(itr))
	{
		CJudgeZone::SJudgeZone zone = (*itr)->GetZone();
		std::string str = "<" + std::to_string(zone.start) + "~" + std::to_string(zone.end) + ">";
		item.insert(std::pair<std::string, CJudgeZone*>(str, (*itr)));
		cnt++;
	}

	// �V�K
	item.insert(std::pair<std::string, CJudgeZone*>("<Create>", nullptr));

	// ���X�g�\��
	static std::string strSelect = "<Create>";
	if (ImGui::BeginCombo("Edit Zone", strSelect.c_str()))
	{
		for (auto itrDisp = item.begin(); itrDisp != item.end(); itrDisp++)
		{
			bool isSelect = (strSelect == (*itrDisp).first);
			if (ImGui::Selectable((*itrDisp).first.c_str(), isSelect))
			{
				strSelect = (*itrDisp).first;
			}
			cnt++;
		}
		ImGui::EndCombo();
	}

	// �I�u�W�F�N�g�I��
	m_pSelectZone = item[strSelect];

	//�I�����Ă���]�[��������Ȃ�擾
	if (m_pSelectZone != nullptr)
	{
		m_JudgeZoneData.zone = m_pSelectZone->GetZone();
		m_pSelectZone->SetInfo(CJudge::BORDER::TOP, m_JudgeZoneData.conditionTop);
		m_pSelectZone->SetInfo(CJudge::BORDER::UNDER, m_JudgeZoneData.conditionUnder);
	}
}

//==========================================================================
// �t�@�C������
//==========================================================================
void CEdit_JudgeZone::FileControl()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	CJudgeZoneManager* manager = CJudgeZoneManager::GetInstance();
	float width = 150.0f;
	
	// ���X�g
	ImGui::SeparatorText("List");
	if (ImGui::Button("SaveList"))
	{// �ۑ�
		OPENFILENAMEA filename = {};
		char sFilePass[1024] = {};
		filename = CreateOFN("\\data\\TEXT\\judgezonelist", sFilePass);

		// �t�@�C���I���_�C�A���O��\���E�Z�[�u
		if (GetOpenFileNameA(&filename) && strcmp(&sFilePass[0], "") != 0)
		{
			manager->Save(sFilePass);
		}
	}
	ImGui::SameLine();

	if (ImGui::Button("LoadList"))
	{// �ǂݍ���
		OPENFILENAMEA filename = {};
		char sFilePass[1024] = {};
		filename = CreateOFN("\\data\\TEXT\\judgezonelist", sFilePass);

		// �t�@�C���I���_�C�A���O��\���E���[�h
		if (GetOpenFileNameA(&filename) && strcmp(&sFilePass[0], "") != 0)
		{
			manager->ReleaseAll();
			manager->Load(sFilePass);
		}
	}

	// �P��
	ImGui::SeparatorText("Single");
	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save & Apply"))
	{// �ۑ�
		OPENFILENAMEA filename = {};
		char sFilePass[1024] = {};
		filename = CreateOFN("\\data\\TEXT\\judgezone", sFilePass);

		// �t�@�C���I���_�C�A���O��\���E�Z�[�u
		if (GetOpenFileNameA(&filename) && strcmp(&sFilePass[0], "") != 0)
		{
			manager->SaveZone(sFilePass, m_JudgeZoneData.zone, m_JudgeZoneData.conditionTop, m_JudgeZoneData.conditionUnder);

			// �V�K�Ȃ琶��
			if (m_pSelectZone == nullptr)
			{
				CJudgeZone* pJudgeZone = CJudgeZone::Create(m_JudgeZoneData.zone.start, m_JudgeZoneData.zone.end, m_JudgeZoneData.zone.borderHeight);
				pJudgeZone->SetInfo(CJudge::BORDER::TOP, m_JudgeZoneData.conditionTop);
				pJudgeZone->SetInfo(CJudge::BORDER::UNDER, m_JudgeZoneData.conditionTop);
				pJudgeZone->SetPath(sFilePass);
			}
		}
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Load"))
	{// �ǂݍ���
		OPENFILENAMEA filename = {};
		char sFilePass[1024] = {};
		filename = CreateOFN("\\data\\TEXT\\judgezone", sFilePass);

		// �t�@�C���I���_�C�A���O��\���E���[�h
		if (GetOpenFileNameA(&filename) && strcmp(&sFilePass[0], "") != 0)
		{
			manager->LoadZone(sFilePass);
		}
	}
}

//==========================================================================
// ����]�[���ݒ�
//==========================================================================
void CEdit_JudgeZone::SetJudgeZone()
{
	if (ImGui::TreeNode("JudgeZone"))
	{
		ImGui::DragFloat("Start", &m_JudgeZoneData.zone.start, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("End", &m_JudgeZoneData.zone.end, 0.01f, m_JudgeZoneData.zone.start, 1.0f);
		ImGui::DragFloat("Border", &m_JudgeZoneData.zone.borderHeight);
		
		if (ImGui::TreeNode("Condition_Top"))
		{// �㑤����
			SetCondition(m_JudgeZoneData.conditionTop,m_abEnable[CJudge::BORDER::TOP],m_aConditionType[CJudge::BORDER::TOP]);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Condition_Under"))
		{// ��������
			SetCondition(m_JudgeZoneData.conditionUnder,m_abEnable[CJudge::BORDER::UNDER], m_aConditionType[CJudge::BORDER::UNDER]);

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	//�I�����Ă���]�[��������Ȃ�ݒ�
	if (m_pSelectZone != nullptr)
	{
		m_pSelectZone->SetZone(m_JudgeZoneData.zone);
		m_pSelectZone->SetInfo(CJudge::BORDER::TOP, m_JudgeZoneData.conditionTop);
		m_pSelectZone->SetInfo(CJudge::BORDER::UNDER, m_JudgeZoneData.conditionUnder);
	}
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CEdit_JudgeZone::SetCondition(CJudge::SJudgeCondition& condition, bool* pEnable, CJudge::CONDITIONTYPE& conditionType)
{
	static const char* items[] = { "None", "HitNum" };
	int selectedItem = conditionType;

	// ���
	for (int cnt = 0; cnt < CJudge::CONDITIONTYPE::TYPE_MAX; cnt++)
	{
		if (ImGui::RadioButton(items[cnt], &selectedItem, cnt))
		{
			conditionType = static_cast<CJudge::CONDITIONTYPE>(selectedItem);
		}
	}
	condition.type = conditionType;

	// �L���E�����ݒ�㐔�l�ݒ�
	// =========AAA=========
	ImGui::Checkbox("Enable_AAA", &pEnable[0]);
	if (!pEnable[0]) ImGui::BeginDisabled(true);
	
		ImGui::InputInt("AAA_Num", &condition.judgeParam[CJudge::JUDGE::JUDGE_AAA]);
	
	if (!pEnable[0])
	{
		ImGui::EndDisabled();
		condition.judgeParam[CJudge::JUDGE::JUDGE_AAA] = -1;
	}
	// =========AAA=========

	// =========BBB=========
	ImGui::Checkbox("Enable_BBB", &pEnable[1]);
	if (!pEnable[1]) ImGui::BeginDisabled(true);
	
		ImGui::InputInt("BBB_Num", &condition.judgeParam[CJudge::JUDGE::JUDGE_BBB]);
	
	if (!pEnable[1])
	{
		ImGui::EndDisabled();
		condition.judgeParam[CJudge::JUDGE::JUDGE_BBB] = -1;
	}
	// =========BBB=========

	// =========CCC=========
	ImGui::Checkbox("Enable_CCC", &pEnable[2]);
	if (!pEnable[2]) ImGui::BeginDisabled(true);
	
		ImGui::InputInt("CCC_Num", &condition.judgeParam[CJudge::JUDGE::JUDGE_CCC]);
	
	if (!pEnable[2])
	{
		ImGui::EndDisabled();
		condition.judgeParam[CJudge::JUDGE::JUDGE_CCC] = -1;
	}
	// =========CCC=========

	// =========DDD=========
	ImGui::Checkbox("Enable_DDD", &pEnable[3]);
	if (!pEnable[3]) ImGui::BeginDisabled(true);
	
		ImGui::InputInt("DDD_Num", &condition.judgeParam[CJudge::JUDGE::JUDGE_DDD]);
	
	if (!pEnable[3])
	{
		ImGui::EndDisabled();
		condition.judgeParam[CJudge::JUDGE::JUDGE_DDD] = -1;
	}
	// =========DDD=========
}

//==========================================================================
// �t�@�C�����J���E�B���h�E�ɕK�v�ȏ�񐶐�
//==========================================================================
OPENFILENAMEA CEdit_JudgeZone::CreateOFN(std::string relativeDir, char* filePass)
{
	OPENFILENAMEA filename = {};
	// �t�@�C���I���_�C�A���O�̐ݒ�
	filename.lStructSize = sizeof(OPENFILENAMEA);
	filename.hwndOwner = NULL;
	filename.lpstrFilter = "�e�L�X�g�t�@�C��\0*.txt\0�摜�t�@�C��\0*.bmp;.jpg\0���ׂẴt�@�C��\0.*\0\0";
	filename.lpstrFile = filePass;
	filename.nMaxFile = MAX_PATH;
	filename.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;


	// �J�����g�f�B���N�g�����擾����
	char szCurrentDir[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, szCurrentDir);

	// "data"�t�H���_�̐�΃p�X�����߂�
	std::string strDataDir = szCurrentDir;
	strDataDir += relativeDir;

	// ���݂���ꍇ�́AlpstrInitialDir�Ɏw�肷��
	if (GetFileAttributesA(strDataDir.c_str()) != INVALID_FILE_ATTRIBUTES)
	{
		filename.lpstrInitialDir = strDataDir.c_str();
	}

	return filename;
}
