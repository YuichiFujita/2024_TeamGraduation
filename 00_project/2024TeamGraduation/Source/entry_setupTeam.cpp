//=============================================================================
// 
// �`�[�����ݒ菈�� [entry_setupTeam.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "entry_setupTeam.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"

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
CEntry_SetUpTeam::CEntry_SetUpTeam() : CEntryScene()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEntry_SetUpTeam::~CEntry_SetUpTeam()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEntry_SetUpTeam::Init()
{
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEntry_SetUpTeam::Uninit()
{
}

//==========================================================================
// �X�V����
//==========================================================================
void CEntry_SetUpTeam::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, i))
		{// �G���g���[�A�R���g���[���[����


		}

		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, i))
		{// �`�[������

			m_TeamSide[i];		// �`�[���T�C�h
		}
	}

}

//==========================================================================
// �Z�[�u����
//==========================================================================
void CEntry_SetUpTeam::Save()
{
	// �t�@�C�����J��
	std::ofstream File(TEXTFILE);
	if (!File.is_open()) {
		return;
	}

	// �e�L�X�g�t�@�C�����ڎ�
	File << TOP_LINE			<< std::endl;
	File << "# �`�[�����ݒ�"	<< std::endl;
	File << TOP_LINE			<< std::endl;


	// �T�C�h���̐l��
	File << TEXT_LINE			<< std::endl;
	File << "# �v���C���[��"	<< std::endl;
	File << TEXT_LINE			<< std::endl;
	File << "PLAYERNUM = "	<< 
		m_vecPlayerNum[CGameManager::ETeamSide::SIDE_LEFT] << 
		" " <<
		m_vecPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT] << std::endl;
	
	File << "" << std::endl;


	// �v���C���[�C���f�b�N�X
	File << TEXT_LINE << std::endl;
	File << "# �v���C���[�C���f�b�N�X" << std::endl;
	File << TEXT_LINE << std::endl;
	
	// ��
	File << "LEFT_ID = ";
	for (int i = 0; i < CGameManager::MAX_SIDEPLAYER; i++)
	{
		if (static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size()) - 1 >= i)
		{// �T�C�Y��
			File << m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][i];
		}
		else
		{
			File << -1;
		}
		File << " ";

	}
	File << std::endl;

	// �E
	File << "RIGHT_ID = ";
	for (int i = 0; i < CGameManager::MAX_SIDEPLAYER; i++)
	{
		if (static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size()) - 1 >= i)
		{// �T�C�Y��
			File << m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][i];
		}
		else
		{
			File << -1;
		}
		File << " ";
	}
	File << std::endl;


	// �t�@�C�������
	File.close();
}

//==========================================================================
// �f�o�b�O����
//==========================================================================
void CEntry_SetUpTeam::Debug()
{
	//=============================
	// �Z�[�u
	//=============================s
	if (ImGui::Button("Save"))
	{
		Save();
	}

	//=============================
	// �����ύX
	//=============================
	//--------------------------
	// ��
	//--------------------------
	{
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Entry Left:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##left", ImGuiDir_Left) &&
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size() > 1)
		{// �폜
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].pop_back();
		}
		ImGui::SameLine(0.0f);

		if (ImGui::ArrowButton("##right", ImGuiDir_Right) &&
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size() < CGameManager::MAX_SIDEPLAYER)
		{// �ǉ�
			int size = m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size();
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].push_back(size);
		}
		ImGui::SameLine();
		ImGui::PopID();

		// �T�C�Y
		int segmentSize = static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size());
		ImGui::Text("%d", segmentSize);
	}

	//--------------------------
	// �E
	//--------------------------
	{
		ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Entry Right:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##left", ImGuiDir_Left) &&
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size() > 1)
		{// �폜
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].pop_back();
		}
		ImGui::SameLine(0.0f);

		if (ImGui::ArrowButton("##right", ImGuiDir_Right) &&
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size() < CGameManager::MAX_SIDEPLAYER)
		{// �ǉ�
			int size = m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size();
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].push_back(size);
		}
		ImGui::SameLine();
		ImGui::PopID();

		// �T�C�Y
		int segmentSize = static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size());
		ImGui::Text("%d", segmentSize);
	}


	//=============================
	// �����ύX
	//=============================
	ImGui::Text("Side Player");
	ImGui::SliderInt("SIDE_LEFT", &m_vecPlayerNum[CGameManager::ETeamSide::SIDE_LEFT], 1, CGameManager::MAX_SIDEPLAYER);
	ImGui::SliderInt("SIDE_RIGHT", &m_vecPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT], 1, CGameManager::MAX_SIDEPLAYER);

}
