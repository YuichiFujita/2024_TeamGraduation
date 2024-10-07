//=============================================================================
// 
//  �}�b�v�̏�Q���}�l�[�W������ [map_obstacleManager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "map_obstacleManager.h"
#include "map_obstacle.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXT = "data\\TEXT\\map\\obstacle.txt";	// �ݒu���X�N���v�g�t�@�C��
	const std::string TEXT_LINE = "#------------------------------------------------------------------------------";	// �e�L�X�g�̃��C��
}
CMap_ObstacleManager* CMap_ObstacleManager::m_ThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMap_ObstacleManager::CMap_ObstacleManager()
{
	m_ObstacleInfo.clear();	// ��Q�����

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMap_ObstacleManager::~CMap_ObstacleManager()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CMap_ObstacleManager *CMap_ObstacleManager::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// �������̊m��
		m_ThisPtr = DEBUG_NEW CMap_ObstacleManager;

		if (m_ThisPtr != nullptr)
		{
			// ����������
			m_ThisPtr->Init();
		}
	}

	return m_ThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMap_ObstacleManager::Init()
{
#if 0
	SObstacleInfo info;

	MyLib::Collider_BOX box;
	info.boxcolliders.push_back(box);

	// �t�@�C���ɃL�����N�^�[�̃f�[�^����������
	std::ofstream outFile("data/TEXT/mapobstacle/stone/collider.json");
	if (!outFile.is_open())
	{
		return E_FAIL;
	}

	nlohmann::json outputData;
	info.to_json(outputData);

	// �t�@�C���Ƀf�[�^����������
	outFile << std::setw(4) << outputData << std::endl;

	outFile.close();
#endif

	// �ǂݍ���
	Load();
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMap_ObstacleManager::Uninit()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// �폜
//==========================================================================
void CMap_ObstacleManager::Kill()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CMap_ObstacleManager::Update()
{
	

}

//==========================================================================
// �Z�[�u
//==========================================================================
void CMap_ObstacleManager::Save()
{
	// ���X�g�擾
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// �擪��ۑ�
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	// �t�@�C�����J��
	std::ofstream File(TEXT);
	if (!File.is_open()) {
		return;
	}


	// �e�L�X�g�t�@�C�����ڎ�
	File << TEXT_LINE << std::endl;
	File << " �e�L�X�g�t�@�C����" << std::endl;
	File << TEXT_LINE << std::endl;

	int i = 0;
	for (const auto& info : m_ObstacleInfo)
	{
		// �e�L�X�g�t�@�C����
		File << "TEXT_FILENAME = " << info.textFile << "\t\t# [" << i << "]" << std::endl;
		i++;
	}
	File << "" << std::endl;

	File << TEXT_LINE << std::endl;
	File << " ���f���̔z�u" << std::endl;
	File << TEXT_LINE << std::endl;
	while (list.ListLoop(itr))
	{
		pObj = (*itr);

		// ��Q�����擾
		MyLib::Vector3 pos = pObj->GetPosition(), rot = pObj->GetRotation(), scale = pObj->GetScale();
		SObstacleInfo info = pObj->GetObstacleInfo();
		std::string text = info.textFile;

		const auto& obItr = std::find_if(m_ObstacleInfo.begin(), m_ObstacleInfo.end(), [&text](const SObstacleInfo& string) {return string.textFile == text; });
		int type = (obItr - m_ObstacleInfo.begin());

		// ���f�����
		File << "MODELSET" << std::endl;
		File << "\tTYPE = " << type << std::endl;
		File << "\tPOS = " << std::fixed << std::setprecision(2) << pos.x << " " << pos.y << " " << pos.z << std::endl;
		File << "\tROT = " << std::fixed << std::setprecision(2) << rot.x << " " << rot.y << " " << rot.z << std::endl;
		File << "\tSCALE = " << std::fixed << std::setprecision(2) << scale.x << " " << scale.y << " " << scale.z << std::endl;
		File << "END_MODELSET" << std::endl;
		File << "" << std::endl;
	}

	// �t�@�C�������
	File << "\nEND_SCRIPT\t\t# ���̍s�͐�Ώ����Ȃ����ƁI" << std::endl;
	File.close();

}

//==========================================================================
// ���Z�[�u
//==========================================================================
void CMap_ObstacleManager::SaveInfo()
{
	for (const auto& info : m_ObstacleInfo)
	{
		// �t�@�C�����J��
		std::ofstream File(info.textFile);
		if (!File.is_open()) {
			return;
		}

		File << "#====================================================================================================" << std::endl;
		File << "#" << std::endl;
		File << "# ��Q���X�N���v�g�t�@�C�� [setup.txt]" << std::endl;
		File << "# Author : ���n �Ή�" << std::endl;
		File << "#" << std::endl;
		File << "#====================================================================================================" << std::endl;
		File << "SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI\n" << std::endl;

		{
			// ���f���t�@�C�����ڎ�
			File << TEXT_LINE << std::endl;
			File << " ���f���t�@�C����" << std::endl;
			File << TEXT_LINE << std::endl;

			// ���f���t�@�C����
			File << "MODEL_FILENAME = " << info.modelFile << std::endl;
			File << "" << std::endl;
		}

		{
			// ���f�����ڎ�
			File << TEXT_LINE << std::endl;
			File << " ���f�����" << std::endl;
			File << TEXT_LINE << std::endl;

			// ���f�����
			File << "SETUP" << std::endl;
			File << "\tIS_AIR = " << info.setup.isAir << "\t\t# ��C��ʂ���[0:�ʂ��Ȃ� / 1:�ʂ�]" << std::endl;
			File << "\tIS_MOVE = " << info.setup.isMove << "\t\t# �������������邩[0:���� / 1:�����Ȃ�]" << std::endl;
			File << "END_SETUP" << std::endl;
			File << "" << std::endl;
		}

		{
			// �R���C�_�[�ڎ�
			File << TEXT_LINE << std::endl;
			File << " �R���C�_�[���" << std::endl;
			File << TEXT_LINE << std::endl;

			// �R���C�_�[�t�@�C����
			File << "COLLIDER_FILENAME = " << info.colliderFile << std::endl;
			File << "" << std::endl;
		}


		// �t�@�C�������
		File << "\nEND_SCRIPT\t\t# ���̍s�͐�Ώ����Ȃ����ƁI" << std::endl;
		File.close();

		// �t�@�C���ɃL�����N�^�[�̃f�[�^����������
		std::ofstream outFile(info.colliderFile);
		if (!outFile.is_open())
		{
			return;
		}

		nlohmann::json outputData;
		info.to_json(outputData);

		// �t�@�C���Ƀf�[�^����������
		outFile << std::setw(4) << outputData << std::endl;
		outFile.close();
	}
}

//==========================================================================
// ���[�h
//==========================================================================
void CMap_ObstacleManager::Load()
{
	// �t�@�C�����J��
	std::ifstream File(TEXT);
	if (!File.is_open()) {
		return;
	}

	// �R�����g�p
	std::string hoge;

	// �t�@�C����
	std::vector<std::string> filename;

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

		// �X�g���[���쐬
		std::istringstream lineStream(line);

		if (line.find("TEXT_FILENAME") != std::string::npos)
		{// TEXT_FILENAME�Ń��f�����ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// �ǉ�
			filename.emplace_back();

			// ���n��
			lineStream >>
				hoge >>
				hoge >>			// ��
				filename.back();// ���f���t�@�C����

			// ���ǂݍ���
			LoadInfo(filename.back());
			continue;
		}

		if (line.find("MODELSET") != std::string::npos)
		{// MODELSET�Ŕz�u���ǂݍ���

			// �ǂݍ��ݏ��
			int type;
			MyLib::Vector3 pos, rot, scale(1.0f);

			while (line.find("END_MODELSET") == std::string::npos)
			{
				std::getline(File, line);
				if (line.find("TYPE") != std::string::npos)
				{// TYPE�Ŕz�u���̎��

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// ���n��
					lineStream >>
						hoge >>
						hoge >>	// ��
						type;	// �z�u���̎��
					continue;
				}

				if (line.find("POS") != std::string::npos)
				{// POS�ňʒu

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// ���n��
					lineStream >>
						hoge >>
						hoge >>						// ��
						pos.x >> pos.y >> pos.z;	// �ʒu
					continue;
				}

				if (line.find("ROT") != std::string::npos)
				{// ROT�Ō���

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// ���n��
					lineStream >>
						hoge >>
						hoge >>						// ��
						rot.x >> rot.y >> rot.z;	// ����
					continue;
				}

				if (line.find("SCALE") != std::string::npos)
				{// SCALE�Ŋg�嗦

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// ���n��
					lineStream >>
						hoge >>
						hoge >>						// ��
						scale.x >> scale.y >> scale.z;	// �g�嗦
					continue;
				}
			}

			// ����
			if (type < static_cast<int>(m_ObstacleInfo.size()))
			{
				CMap_Obstacle* pObj = CMap_Obstacle::Create(m_ObstacleInfo[type], pos);
				pObj->SetRotation(rot);
				pObj->SetScale(scale);
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

//==========================================================================
// ��񃍁[�h
//==========================================================================
void CMap_ObstacleManager::LoadInfo(const std::string& file)
{
	// �t�@�C�����J��
	std::ifstream File(file);
	if (!File.is_open()) {
		return;
	}

	// �R�����g�p
	std::string hoge;

	// ��Q�����
	SObstacleInfo obstacleInfo;
	obstacleInfo.textFile = file;

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

		// �X�g���[���쐬
		std::istringstream lineStream(line);


		if (line.find("MODEL_FILENAME") != std::string::npos)
		{// MODEL_FILENAME�Ń��f�����ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>		// ��
				hoge >>		// ��
				obstacleInfo.modelFile;	// ���f���t�@�C����
			continue;
		}

		if (line.find("COLLIDER_FILENAME") != std::string::npos)
		{// COLLIDER_FILENAME�ŃR���C�_�[�ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>			// ��
				obstacleInfo.colliderFile;	// �R���C�_�[��


			//=============================
			// �R���C�_�[�ǂݍ���
			//=============================
			{
				// �t�@�C������JSON��ǂݍ���
				std::ifstream filejson(obstacleInfo.colliderFile);
				if (!filejson.is_open())
				{
					return;
				}

				nlohmann::json jsonData;
				filejson >> jsonData;	// json�f�[�^��^����

				// json�f�[�^����ǂݍ���
				obstacleInfo.from_json(jsonData);


				int n = 0;
			}
			continue;
		}

		if (line.find("SETUP") != std::string::npos)
		{// SETUP�ŏ��ǂݍ���

			while (line.find("END_SETUP") == std::string::npos)
			{
				std::getline(File, line);
				if (line.find("IS_AIR") != std::string::npos)
				{// IS_AIR�ŋ�C�ђʓǂݍ���

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// ���n��
					lineStream >>
						hoge >>
						hoge >>	// ��
						obstacleInfo.setup.isAir;	// ��C�ђ�
					continue;
				}

				if (line.find("IS_MOVE") != std::string::npos)
				{// IS_MOVE�œ����ǂݍ���

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// ���n��
					lineStream >>
						hoge >>
						hoge >>	// ��
						obstacleInfo.setup.isMove;	// ����
					continue;
				}
			}
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	if (obstacleInfo.boxcolliders.empty())
	{
		obstacleInfo.boxcolliders.push_back(MyLib::Collider_BOX());
	}

	// ���ǉ�
	obstacleInfo.type = m_ObstacleInfo.size();
	m_ObstacleInfo.push_back(obstacleInfo);

	// �t�@�C�������
	File.close();
}

//==========================================================================
// �R���C�_�[�ǉ�
//==========================================================================
void CMap_ObstacleManager::AddCollider(int idx)
{
	if (static_cast<int>(m_ObstacleInfo.size()) <= idx) {
		return;
	}

	m_ObstacleInfo[idx].boxcolliders.push_back(MyLib::Collider_BOX());
}

//==========================================================================
// �R���C�_�[�폜
//==========================================================================
void CMap_ObstacleManager::SubCollider(int idx)
{
	if (static_cast<int>(m_ObstacleInfo[idx].boxcolliders.size()) >= 1)
	{
		m_ObstacleInfo[idx].boxcolliders.pop_back();
	}
}
