//=============================================================================
//
// �R�}���h�O���[�v���� [command_group.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "command_group.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCommandGroup::CCommandGroup()
{
	m_CommandList.clear();
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCommandGroup::Init()
{
	m_CommandList.clear();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCommandGroup::Uninit()
{
	for (auto it = m_CommandList.begin(); it != m_CommandList.end(); it++) {
		if ((*it) == nullptr) continue;
		(*it)->Uninit();
		delete (*it);
		(*it) = nullptr;
	}

	m_CommandList.clear();
}

//==========================================================================
// �����ɓo�^
//==========================================================================
void CCommandGroup::ListIn(CCommand* command)
{
	m_CommandList.push_back(command);
}

//==========================================================================
// �R���t�B�O����
//==========================================================================
CCommandGroup* CCommandGroup::Create(const std::string& string)
{
	CCommandGroup* pGroup = DEBUG_NEW CCommandGroup;

	if (pGroup != nullptr) {
		pGroup->Load(string);
	}

	return pGroup;
}

//==========================================================================
// �R�}���h�����ԍ����擾
//==========================================================================
void CCommandGroup::GetSuccess(int& nMotion, int& nType)
{
	bool value = false;
	for (auto it = m_CommandList.begin(); it != m_CommandList.end(); it++) {

		if ((*it)->GetCommand())
		{
			nMotion = (*it)->GetId();
			nType = (*it)->GetType();
			return;
		}
	}
}

//==========================================================================
// �ǂݍ���
//==========================================================================
void CCommandGroup::Load(const std::string& string)
{
	// �t�@�C�����J��
	std::ifstream File(string);
	if (!File.is_open()) {
		return;
	}

	int id = 0;

	// �R�����g�p
	std::string hoge;

	// �f�[�^�ǂݍ���
	std::string line;
	while (std::getline(File, line))
	{
		CCommandPad* pPad = nullptr;

		// �R�����g�̓X�L�b�v
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		// �X�g���[���쐬
		std::istringstream lineStream(line);

		if (line.find("SET") != std::string::npos)
		{// SET�Ŕz�u���ǂݍ���
			// �ǂݍ��ݏ��
			while (line.find("END_SET") == std::string::npos)
			{
				// �X�g���[���쐬
				std::getline(File, line);
				std::istringstream lineStream(line);

				if (line.find("COMMAND_FILENAME") != std::string::npos)
				{// COMMAND_FILENAME�ŃR�}���h���ǂݍ���
					std::string commandfile;
					commandfile.clear();

					// ���n��
					lineStream >>
						hoge >>
						hoge >>
						commandfile;	// �z�u���̎��

					pPad = DEBUG_NEW CCommandPad;
					pPad->Load(commandfile);
					ListIn(pPad);
				}
				if (line.find("PLAY_MOTION") != std::string::npos)
				{// COMMAND_FILENAME�ŃR�}���h���ǂݍ���
					int id = 1;

					// ���n��
					lineStream >>
						hoge >>
						hoge >>
						id;	// �z�u���̎��

					if (pPad != nullptr)
					{
						pPad->SetId(id);
					}
				}
				if (line.find("TYPE") != std::string::npos)
				{// COMMAND_FILENAME�ŃR�}���h���ǂݍ���
					int id = 1;

					// ���n��
					lineStream >>
						hoge >>
						hoge >>
						id;	// �z�u���̎��

					if (pPad != nullptr)
					{
						pPad->SetType(id);
					}
				}
			}
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// �t�@�C�������
	File.close();
}
