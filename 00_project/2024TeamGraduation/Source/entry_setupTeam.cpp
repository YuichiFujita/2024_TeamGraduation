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
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		m_vecAddIdx[i].clear();		// �ǉ����ꂽ�C���f�b�N�X
		m_nMaxChangeIdx[i] = -1;	// �ő吔�ύX����C���f�b�N;
		m_nPlayerNum[i] = 1;		// �v���C���[�̐�
	}

	for (int i = 0; i < CGameManager::MAX_PLAYER; i++)
	{
		m_TeamSide[i] = CGameManager::ETeamSide::SIDE_NONE;	// �`�[���T�C�h
	}

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		m_nEntryIdx[i] = -1;								// �G���g���[�̃C���f�b�N�X
	}
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
	// �ǂݍ���
	Load();

	// �ǉ����ꂽ�C���f�b�N�X���Z�b�g TODO : ���񃊃Z�b�g�����Ⴄ����v����
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		m_vecAddIdx[i].clear();
	}
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEntry_SetUpTeam::Uninit()
{
	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CEntry_SetUpTeam::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�̍X�V
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �`�[���I��
	SelectTeam();

	// �v���C���[�ő吔�ύX
	ChangeMaxPlayer();

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// ��U�V�[���؂�ւ� TODO : �S���G���g���[���Ă���Ƃ��ɂ���
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, 0))
	{
		// �󂢂��Ƃ����AI�ǉ�
		for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
		{
			// �`�[���T�C�h�ǉ�
			for (int j = 0; j < CGameManager::MAX_PLAYER; j++)
			{
				if (m_TeamSide[j] == CGameManager::ETeamSide::SIDE_NONE)
				{// NONE�̐l��ς���

					if ((int)m_vecAddIdx[i].size() <= j &&
						m_nPlayerNum[i] > (int)m_vecAddIdx[i].size())
					{// �T�C�h���̃v���C���[�����傫���� && �`�[�����l�����z���Ă��Ȃ�
						m_TeamSide[j] = (CGameManager::ETeamSide)i;
						m_vecAddIdx[i].push_back(-1);
					}

				}

			}

			//while (m_nPlayerNum[i] >= (int)m_vecAddIdx[i].size())
			//{// �󂫂��Ȃ��Ȃ�܂�
			//	m_vecAddIdx[i].push_back(-1);
			//}
		}

		CEntry::GetInstance()->ChangeEntryScene(CEntry::ESceneType::SCENETYPE_DRESSUP);
	}
}

//==========================================================================
// �`�[���I��
//==========================================================================
void CEntry_SetUpTeam::SelectTeam()
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		//--------------------------
		// �Q��
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, i))
		{// �G���g���[�A�R���g���[���[����

			// �C���f�b�N�X�i�[
			for (int j = 0; j < mylib_const::MAX_PLAYER; j++)
			{
				// �����G���g���[����Ă�͎̂g��Ȃ�
				if (m_nEntryIdx[j] >= 0 && m_nEntryIdx[j] != i) continue;
				m_nEntryIdx[j] = i;
				break;
			}
		}

		// ����̃C���f�b�N�X
		int nowIdx = m_nEntryIdx[i];
		if (nowIdx < 0) continue;	// ���G���g���[�͔�����

		//--------------------------
		// �`�[���ւ�
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nowIdx) &&
			m_TeamSide[nowIdx] != CGameManager::ETeamSide::SIDE_LEFT)
		{// ���ړ�(���[����Ȃ���)

			// ���Ɉړ�
			m_TeamSide[nowIdx] = (m_TeamSide[nowIdx] == CGameManager::ETeamSide::SIDE_NONE) ? CGameManager::ETeamSide::SIDE_LEFT : CGameManager::ETeamSide::SIDE_NONE;
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nowIdx) &&
			m_TeamSide[nowIdx] != CGameManager::ETeamSide::SIDE_RIGHT)
		{// �E�ړ�(�E�[����Ȃ���)

			// �E�Ɉړ�
			m_TeamSide[nowIdx] = (m_TeamSide[nowIdx] == CGameManager::ETeamSide::SIDE_NONE) ? CGameManager::ETeamSide::SIDE_RIGHT : CGameManager::ETeamSide::SIDE_NONE;
		}

		//--------------------------
		// �`�[�����l���ς�
		//--------------------------
		// �ύX����C���f�b�N�X���߂�
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, nowIdx) &&
			m_TeamSide[nowIdx] != CGameManager::ETeamSide::SIDE_NONE &&
			m_nMaxChangeIdx[m_TeamSide[nowIdx]] < 0)
		{// ��ړ�(NONE����Ȃ���)

			// �����̃T�C�h��ύX����p�C���f�b�N�X�ێ�
			int side = m_TeamSide[nowIdx];
			m_nMaxChangeIdx[side] = nowIdx;
		}

		//--------------------------
		// ����!
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, nowIdx) &&
			m_TeamSide[nowIdx] != CGameManager::ETeamSide::SIDE_NONE)
		{// �`�[������ && ���E�ǂ��炩�ɍ��킹�Ă�Ƃ�

			int side = m_TeamSide[nowIdx];
			
			// ���ɂ���ꍇ�͒ǉ����Ȃ�
			const auto& itr = std::find(m_vecAddIdx[side].begin(), m_vecAddIdx[side].end(), nowIdx);
			if (itr == m_vecAddIdx[side].end())
			{// ���݂��Ȃ�
				m_vecAddIdx[side].push_back(nowIdx);
			}
		}
	}
}

//==========================================================================
// �v���C���[�ő吔�ύX
//==========================================================================
void CEntry_SetUpTeam::ChangeMaxPlayer()
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	for (int side = 0; side < CGameManager::ETeamSide::SIDE_MAX; side++)
	{
		// ����ύX����R���g���[���[�ԍ�
		int changeIdx = m_nMaxChangeIdx[side];
		if (changeIdx < 0) continue;

		//--------------------------
		// �ő吔�ύX
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, changeIdx))
		{// ���炷
			m_nPlayerNum[side]--;	// �v���C���[�̐�
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, changeIdx))
		{// ���₷
			m_nPlayerNum[side]++;	// �v���C���[�̐�
		}
		m_nPlayerNum[side] = UtilFunc::Transformation::Clamp(m_nPlayerNum[side], 1, CGameManager::MAX_SIDEPLAYER);

		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, changeIdx))
		{// ���ړ�(�ő吔�ύX����)

			// �����̃T�C�h��ύX����p�C���f�b�N�X����
			m_nMaxChangeIdx[side] = -1;
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
		m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT] << 
		" " <<
		m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT] << std::endl;
	
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
// ���[�h
//==========================================================================
void CEntry_SetUpTeam::Load()
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

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT] >>
				m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT];	// �v���C���[��
		}

		if (line.find("LEFT_ID") != std::string::npos)
		{// LEFT_ID�ō����`�[���̃C���f�b�N�X

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// �j��
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].clear();

			// ���T�C�Y
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].resize(CGameManager::MAX_SIDEPLAYER);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][0] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][1] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][2];	// �v���C���[��
		}

		if (line.find("RIGHT_ID") != std::string::npos)
		{// RIGHT_ID�ŉE���`�[���̃C���f�b�N�X

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// �j��
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].clear();

			// ���T�C�Y
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].resize(CGameManager::MAX_SIDEPLAYER);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][0] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][1] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][2];	// �v���C���[��
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
void CEntry_SetUpTeam::Debug()
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
				m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT]--;		// �v���C���[�̐�
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
		ImGui::SliderInt("SIDE_LEFT", &m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT], 1, CGameManager::MAX_SIDEPLAYER);
		ImGui::SliderInt("SIDE_RIGHT", &m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT], 1, CGameManager::MAX_SIDEPLAYER);

		// �G���g���[�̃C���f�b�N�X
		for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
		{
			ImGui::Text("EntryIdx[%d] : %d", i, m_nEntryIdx[i]);
		}

		ImGui::TreePop();
	}
}

//==========================================================================
// �G���g���[�̃C���f�b�N�X
//==========================================================================
int CEntry_SetUpTeam::GetEntryIdx(int i)
{
	if (i >= mylib_const::MAX_PLAYER) return -1;
	return m_nEntryIdx[i];
}
