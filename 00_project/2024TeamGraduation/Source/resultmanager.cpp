//=============================================================================
// 
// ���U���g�}�l�[�W������ [resultmanager.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "resultmanager.h"
#include "result.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CResultManager* CResultManager::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CResultManager::CResultManager()
{
	m_fTension = 0.0f;										// ����オ��l
	m_teamWin = CGameManager::ETeamSide::SIDE_NONE;			// �����`�[��
	memset(&m_fCharmValue[0], 0, sizeof(m_fCharmValue));	// ���e�l
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CResultManager::~CResultManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CResultManager* CResultManager::Create()
{
	if (m_pThisPtr != nullptr)
	{// ���ɂ���ꍇ
		return m_pThisPtr;
	}

	// �C���X�^���X����
	m_pThisPtr = DEBUG_NEW CResultManager;

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
HRESULT CResultManager::Init()
{
	//�`�[���X�e�[�^�X
	Load();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CResultManager::Uninit()
{

	// ���g�̊J��
	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CResultManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ŏ������`�[���h�[��
	// ���̌ヂ�e�l�����h�[��
	// ����オ��l�h�[��




#if _DEBUG	// �f�o�b�O����

	if (ImGui::TreeNode("GameManager"))
	{
		Debug();
		ImGui::TreePop();
	}
#endif
}

//==========================================================================
// �`�[���X�e�[�^�X�ǂݍ���
//==========================================================================
void CResultManager::Load()
{
	// �t�@�C�����J��
	std::ifstream File(ResultManager::TEXTFILE);
	if (!File.is_open()) {
		return;
	}

	// �R�����g�p
	std::string hoge;

	int winteam = 0;
	int i = 0;

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

		if (line.find("WIN") != std::string::npos)
		{// PLAYERNUM�Ń`�[�����̃v���C���[���ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				winteam;	// �����`�[��

			m_teamWin = static_cast<CGameManager::ETeamSide>(winteam);
		}

		if (line.find("TENSION") != std::string::npos)
		{// PLAYERNUM�Ń`�[�����̃v���C���[���ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				m_fTension;	// ����オ��l
		}

		if (line.find("CHARMVALUE") != std::string::npos)
		{// PLAYERNUM�Ń`�[�����̃v���C���[���ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				m_fCharmValue[i];	// ���e�l

			i++;
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
// �f�o�b�O
//==========================================================================
void CResultManager::Debug()
{
#if _DEBUG

	// �^�C�}�[

	if (ImGui::TreeNode("Timer"))
	{
		ImGui::Text("bStop");

		ImGui::TreePop();
	}

#endif
}
