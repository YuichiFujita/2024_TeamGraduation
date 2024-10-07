//=============================================================================
// 
//  �}�b�v�u���b�N�Ǘ����� [map_block.cpp]
//  Author : Ibuki Okusada
// 
//=============================================================================
#include "map_block.h"
#include "checkpoint.h"
#include "map_obstacle.h"
#include "map_obstacleManager.h"
#include "waterstone.h"
#include "objectX.h"
#include "frontobj_manager.h"
#include "edit_map.h"
#include "judgeitem.h"
#include "judgeitemManager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\map\\block.txt";
	const std::string FILE_CHECKPOINT = "data\\TEXT\\map\\checkpoint.bin";	// �`�F�b�N�|�C���g�̃Z�[�u�t�@�C��
	const std::string FILE_OBSTACLE = "data\\TEXT\\map\\obstacle.bin";		// ��Q���̃Z�[�u�t�@�C��
	const std::string FILE_MAP = "data\\TEXT\\map\\map.bin";				// �}�b�v�̃Z�[�u�t�@�C��
	const std::string FILE_WATERSTONE = "data\\TEXT\\map\\waterstone.bin";		// ������̃Z�[�u�t�@�C��
	const std::string FILE_JUDGE = "data\\TEXT\\map\\judge.bin";			// �W���b�W�̃Z�[�u�t�@�C��
	const std::string FILE_LEVEL = "data\\TEXT\\map\\level.bin";			// ���x���̃Z�[�u�t�@�C��
	const int NUM_CHUNK = 5;	// �`�����N�̐�

}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CListManager<CMapBlock> CMapBlock::m_List = {};
CListManager<CMapBlockInfo> CMapBlock::m_InfoList = {};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMapBlock::CMapBlock()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMapBlock::~CMapBlock()
{

}

//==========================================================================
// ��������
//==========================================================================
CListManager<CMapBlockInfo>* CMapBlock::Load()
{
	if (m_InfoList.GetNumAll() > 0) {
		return &m_InfoList;
	}

#if 0
	// �t�@�C�����J��
	std::ifstream File(FILENAME);
	if (!File.is_open()) {
		return &m_InfoList;
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

		// �X�g���[���쐬
		std::istringstream lineStream(line);

		if (line.find("BLOCKSET") != std::string::npos)
		{// MODELSET�Ŕz�u���ǂݍ���

			// �������ă��X�g�̊Ǘ�����
			CMapBlockInfo* pBlock = DEBUG_NEW CMapBlockInfo;
			pBlock->Init();
			pBlock->Load(&File);
			m_InfoList.Regist(pBlock);
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{	// �I������
			break;
		}
	}

	// �t�@�C�������
	File.close();
#else
	LoadBin();
#endif

	return &m_InfoList;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMapBlock::Init()
{
	m_InfoList.Init();
	m_CheckpointList.Init();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMapBlock::Uninit()
{
	// �`�F�b�N�|�C���g�̏I��
	m_CheckpointList.Uninit();

	// ��Q���̏I��
	m_ObstacleList.Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CMapBlock::Kill()
{
	// �}�b�v�u���b�N�̏I��
	std::vector<CMapBlock*> vecDeleteBlock;
	for (int i = 0; i < m_List.GetNumAll(); i++)
	{
		CMapBlock* pBlock = m_List.GetData(i);
		vecDeleteBlock.push_back(pBlock);
	}

	for (int i = 0; i < vecDeleteBlock.size(); i++)
	{

		vecDeleteBlock[i]->Uninit();
		m_List.Delete(vecDeleteBlock[i]);

		delete vecDeleteBlock[i];
		vecDeleteBlock[i] = nullptr;
	}

	// �u���b�N�z�u���̏I��
	std::vector<CMapBlockInfo*> vecDeleteBlockInfo;
	for (int i = 0; i < m_InfoList.GetNumAll(); i++)
	{
		CMapBlockInfo* pBlockInfo = m_InfoList.GetData(i);
		vecDeleteBlockInfo.push_back(pBlockInfo);
	}

	for (int i = 0; i < vecDeleteBlockInfo.size(); i++)
	{
		vecDeleteBlockInfo[i]->Uninit();
		m_InfoList.Delete(vecDeleteBlockInfo[i]);

		delete vecDeleteBlockInfo[i];
		vecDeleteBlockInfo[i] = nullptr;
	}

}

//==========================================================================
// �X�V����
//==========================================================================
void CMapBlock::Update()
{

}

//==========================================================================
// �`�F�b�N�|�C���g�Z�[�u
//==========================================================================
void CMapBlock::SaveBin_CheckPoint()
{
	// �t�@�C�����J��
	std::ofstream File(FILE_CHECKPOINT, std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	// �擪��ۑ�
	std::list<CMapBlockInfo*>::iterator itr = m_InfoList.GetEnd();
	CMapBlockInfo* pObj = nullptr;
	
	std::vector<std::vector<float>> savedata;

	// �f�[�^�R�s�[
	while (m_InfoList.ListLoop(itr))
	{
		savedata.push_back((*itr)->GetCheckpointInfo());
	}

	// �O���x�N�g���̃T�C�Y����������
	size_t outer_size = savedata.size();
	File.write(reinterpret_cast<const char*>(&outer_size), sizeof(outer_size));


	for (const auto& inner : savedata)
	{
		// �f�[�^�̌����v�Z
		size_t vecSize = inner.size();

		// �x�N�g���̃T�C�Y���Z�[�u
		File.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

		// �x�N�g�����̗v�f���Z�[�u
		File.write(reinterpret_cast<const char*>(inner.data()), vecSize * sizeof(float));
	}



	// �t�@�C�������
	File.close();
}

//==========================================================================
// ��Q���Z�[�u
//==========================================================================
void CMapBlock::SaveBin_Obstacle()
{
	// �t�@�C�����J��
	std::ofstream File(FILE_OBSTACLE, std::ios::binary);
	if (!File.is_open()) {
		return;
	}


	// �擪��ۑ�
	std::list<CMapBlockInfo*>::iterator itr = m_InfoList.GetEnd();
	CMapBlockInfo* pObj = nullptr;

	std::vector<std::vector<CMapBlockInfo::SObsacleInfo>> savedata;

	// �f�[�^�R�s�[
	while (m_InfoList.ListLoop(itr))
	{
		savedata.push_back((*itr)->GetObstacleInfo());
	}

	// �O���x�N�g���̃T�C�Y����������
	size_t outer_size = savedata.size();
	File.write(reinterpret_cast<const char*>(&outer_size), sizeof(outer_size));


	for (const auto& inner : savedata)
	{
		// �f�[�^�̌����v�Z
		size_t vecSize = inner.size();

		// �x�N�g���̃T�C�Y���Z�[�u
		File.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

		// �x�N�g�����̗v�f���Z�[�u
		File.write(reinterpret_cast<const char*>(inner.data()), vecSize * sizeof(CMapBlockInfo::SObsacleInfo));
	}


	// �t�@�C�������
	File.close();
}

//==========================================================================
// �}�b�v�Z�[�u
//==========================================================================
void CMapBlock::SaveBin_Map()
{
	// �t�@�C�����J��
	std::ofstream File(FILE_MAP, std::ios::binary);
	if (!File.is_open()) {
		return;
	}


	// �擪��ۑ�
	std::list<CMapBlockInfo*>::iterator itr = m_InfoList.GetEnd();
	CMapBlockInfo* pObj = nullptr;

	std::vector<std::vector<CMapBlockInfo::SObsacleInfo>> savedata;

	// �f�[�^�R�s�[
	while (m_InfoList.ListLoop(itr))
	{
		savedata.push_back((*itr)->GetMapInfo());
	}

	// �O���x�N�g���̃T�C�Y����������
	size_t outer_size = savedata.size();
	File.write(reinterpret_cast<const char*>(&outer_size), sizeof(outer_size));


	for (const auto& inner : savedata)
	{
		// �f�[�^�̌����v�Z
		size_t vecSize = inner.size();

		// �x�N�g���̃T�C�Y���Z�[�u
		File.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

		// �x�N�g�����̗v�f���Z�[�u
		File.write(reinterpret_cast<const char*>(inner.data()), vecSize * sizeof(CMapBlockInfo::SObsacleInfo));
	}


	// �t�@�C�������
	File.close();
}

//==========================================================================
// ������Z�[�u
//==========================================================================
void CMapBlock::SaveBin_WaterStone()
{
	// �t�@�C�����J��
	std::ofstream File(FILE_WATERSTONE, std::ios::binary);
	if (!File.is_open()) {
		return;
	}


	// �擪��ۑ�
	std::list<CMapBlockInfo*>::iterator itr = m_InfoList.GetEnd();
	CMapBlockInfo* pObj = nullptr;

	std::vector<std::vector<CWaterStone_Manager::SStoneInfo>> savedata;

	// �f�[�^�R�s�[
	while (m_InfoList.ListLoop(itr))
	{
		savedata.push_back((*itr)->GetWaterStoneInfo());
	}

	// �O���x�N�g���̃T�C�Y����������
	size_t outer_size = savedata.size();
	File.write(reinterpret_cast<const char*>(&outer_size), sizeof(outer_size));


	for (const auto& inner : savedata)
	{
		// �f�[�^�̌����v�Z
		size_t vecSize = inner.size();

		// �x�N�g���̃T�C�Y���Z�[�u
		File.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

		// �x�N�g�����̗v�f���Z�[�u
		File.write(reinterpret_cast<const char*>(inner.data()), vecSize * sizeof(CWaterStone_Manager::SStoneInfo));
	}


	// �t�@�C�������
	File.close();
}

//==========================================================================
// �W���b�W�Z�[�u
//==========================================================================
void CMapBlock::SaveBin_Judge()
{
	// �t�@�C�����J��
	std::ofstream File(FILE_JUDGE, std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	// �擪��ۑ�
	std::list<CMapBlockInfo*>::iterator itr = m_InfoList.GetEnd();
	CMapBlockInfo* pObj = nullptr;

	std::vector < std::vector<std::vector<CMapBlockInfo::SJudgeInfo>>> savedata;

	// �f�[�^�R�s�[
	while (m_InfoList.ListLoop(itr))
	{
		savedata.push_back((*itr)->GetJudgeInfo());
	}

	// ��ԊO����vector�̃T�C�Y��ۑ�
	size_t outer_size = savedata.size();
	File.write(reinterpret_cast<const char*>(&outer_size), sizeof(size_t));

	for (const auto& inner_vector : savedata)
	{
		// ���Ԃ�vector�̃T�C�Y��ۑ�
		size_t middle_size = inner_vector.size();
		File.write(reinterpret_cast<const char*>(&middle_size), sizeof(size_t));

		for (const auto& inner_inner_vector : inner_vector) 
		{
			// ������vector�̃T�C�Y��ۑ�
			size_t inner_size = inner_inner_vector.size();
			File.write(reinterpret_cast<const char*>(&inner_size), sizeof(size_t));

			// ������vector�̃f�[�^�ifloat�̔z��j���o�C�i���ŕۑ�
			File.write(reinterpret_cast<const char*>(inner_inner_vector.data()), inner_size * sizeof(CMapBlockInfo::SJudgeInfo));
		}
	}


	// �t�@�C�������
	File.close();
}

//==========================================================================
// ���x���Z�[�u
//==========================================================================
void CMapBlock::SaveBin_Level()
{
	// �t�@�C�����J��
	std::ofstream File(FILE_LEVEL, std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	// �擪��ۑ�
	std::list<CMapBlockInfo*>::iterator itr = m_InfoList.GetEnd();
	CMapBlockInfo* pObj = nullptr;

	std::vector<int> savedata;

	// �f�[�^�R�s�[
	while (m_InfoList.ListLoop(itr))
	{
		savedata.push_back((*itr)->GetLevel());
	}

	// �O���x�N�g���̃T�C�Y����������
	size_t outer_size = savedata.size();
	File.write(reinterpret_cast<const char*>(&outer_size), sizeof(outer_size));


	for (const auto& inner : savedata)
	{
		// �v�f���Z�[�u
		File.write(reinterpret_cast<const char*>(&inner), sizeof(int));
	}


	// �t�@�C�������
	File.close();
}

//==========================================================================
// �Z�[�u
//==========================================================================
void CMapBlock::SaveBin()
{
	// �`�F�b�N�|�C���g�Z�[�u
	SaveBin_CheckPoint();

	// ��Q���Z�[�u
	SaveBin_Obstacle();

	// �}�b�v�Z�[�u
	SaveBin_Map();

	// ������Z�[�u
	SaveBin_WaterStone();

	// �W���b�W�Z�[�u
	SaveBin_Judge();

	// ���x���Z�[�u
	SaveBin_Level();
}

//==========================================================================
// ���[�h
//==========================================================================
void CMapBlock::LoadBin()
{
	// �`�F�b�N�|�C���g���[�h
	std::vector<std::vector<float>> checkpoint = LoadBin_CheckPoint();
	if (checkpoint.empty())
	{
		checkpoint.emplace_back();
		checkpoint.back().push_back(0.0f);
	}
	size_t checkpointSize = checkpoint.size();

	// ��Q�����[�h
	std::vector<std::vector<CMapBlockInfo::SObsacleInfo>> obstacle = LoadBin_Obstacle();
	if (obstacle.empty())
	{
		obstacle.emplace_back();
		obstacle.back().push_back(CMapBlockInfo::SObsacleInfo());
	}

	// �����⃍�[�h
	std::vector<std::vector<CWaterStone_Manager::SStoneInfo>> waterstone = LoadBin_WaterStone();
	if (waterstone.empty())
	{
		waterstone.emplace_back();
		waterstone.back().push_back(CWaterStone_Manager::SStoneInfo());
	}

	// �W���b�W���[�h
	std::vector<std::vector<std::vector<CMapBlockInfo::SJudgeInfo>>> judge = LoadBin_Judge();
	if (judge.empty())
	{
		judge.emplace_back();
		judge.back().push_back(std::vector<CMapBlockInfo::SJudgeInfo>());
		judge.back().back().push_back(CMapBlockInfo::SJudgeInfo());
	}

	// �}�b�v���[�h
	std::vector<std::vector<CMapBlockInfo::SObsacleInfo>> map = LoadBin_Map();
	if (map.empty())
	{
		map.emplace_back();
		map.back().push_back(CMapBlockInfo::SObsacleInfo());
		map.resize(checkpointSize);
	}

	// ���x�����[�h
	std::vector<int> level = LoadBin_Level(checkpointSize);
	if (level.empty())
	{
		level.resize(checkpointSize);
	}

	// �S�폜
	m_InfoList.KillAll();

	// �u���b�N����
	int i = 0;
	for (int i = 0; i < static_cast<int>(checkpointSize); i++)
	{
		// �������ă��X�g�̊Ǘ�����
		CMapBlockInfo* pBlock = DEBUG_NEW CMapBlockInfo;
		pBlock->Init();

		pBlock->SetCheckpointInfo(checkpoint[i]);	// �`�F�b�N�|�C���g�ݒ�
		pBlock->SetObstacleInfo(obstacle[i]);		// ��Q���ݒ�
		pBlock->SetMapInfo(map[i]);					// �}�b�v�ݒ�
		pBlock->SetLevel(level[i]);					// ���x���ݒ�

		if (static_cast<int>(waterstone.size()) > i)
		{
			pBlock->SetWaterStoneInfo(waterstone[i]);
		}

		if (static_cast<int>(judge.size()) > i)
		{
			pBlock->SetJudgeInfo(judge[i]);
		}

		m_InfoList.Regist(pBlock);
	}
}

//==========================================================================
// �`�F�b�N�|�C���g�ǂݍ���
//==========================================================================
std::vector<std::vector<float>> CMapBlock::LoadBin_CheckPoint()
{
	// �t�@�C�����J��
	std::ifstream File(FILE_CHECKPOINT, std::ios::binary);
	if (!File.is_open()) {
		// ��O����
		return std::vector<std::vector<float>>();
	}

	// �܂��A�O���̃x�N�g���̃T�C�Y��ǂݍ���
	size_t outer_size;
	File.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));

	std::vector<std::vector<float>> loaddata(outer_size);

	// �e�����̃x�N�g���ɑ΂��ăf�[�^��ǂݍ���
	for (auto& inner_vector : loaddata)
	{
		// �����̃x�N�g���̃T�C�Y��ǂݍ���
		size_t inner_size;
		File.read(reinterpret_cast<char*>(&inner_size), sizeof(inner_size));

		inner_vector.resize(inner_size);

		// �f�[�^�ǂݍ���
		File.read(reinterpret_cast<char*>(inner_vector.data()), inner_size * sizeof(float));
	}

	// �t�@�C�������
	File.close();

	return loaddata;
}

//==========================================================================
// ��Q���ǂݍ���
//==========================================================================
std::vector<std::vector<CMapBlockInfo::SObsacleInfo>> CMapBlock::LoadBin_Obstacle()
{
	// �t�@�C�����J��
	std::ifstream File(FILE_OBSTACLE, std::ios::binary);
	if (!File.is_open()) {
		// ��O����
		return std::vector<std::vector<CMapBlockInfo::SObsacleInfo>>();
	}

	// �܂��A�O���̃x�N�g���̃T�C�Y��ǂݍ���
	size_t outer_size;
	File.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));

	std::vector<std::vector<CMapBlockInfo::SObsacleInfo>> loaddata(outer_size);

	// �e�����̃x�N�g���ɑ΂��ăf�[�^��ǂݍ���
	for (auto& inner_vector : loaddata)
	{
		// �����̃x�N�g���̃T�C�Y��ǂݍ���
		size_t inner_size;
		File.read(reinterpret_cast<char*>(&inner_size), sizeof(inner_size));

		inner_vector.resize(inner_size);

		// �f�[�^�ǂݍ���
		File.read(reinterpret_cast<char*>(inner_vector.data()), inner_size * sizeof(CMapBlockInfo::SObsacleInfo));
	}

	// �t�@�C�������
	File.close();

	return loaddata;
}

//==========================================================================
// �}�b�v�ǂݍ���
//==========================================================================
std::vector<std::vector<CMapBlockInfo::SObsacleInfo>> CMapBlock::LoadBin_Map()
{
	// �t�@�C�����J��
	std::ifstream File(FILE_MAP, std::ios::binary);
	if (!File.is_open()) {
		// ��O����
		return std::vector<std::vector<CMapBlockInfo::SObsacleInfo>>();
	}

	// �܂��A�O���̃x�N�g���̃T�C�Y��ǂݍ���
	size_t outer_size;
	File.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));

	std::vector<std::vector<CMapBlockInfo::SObsacleInfo>> loaddata(outer_size);

	// �e�����̃x�N�g���ɑ΂��ăf�[�^��ǂݍ���
	for (auto& inner_vector : loaddata)
	{
		// �����̃x�N�g���̃T�C�Y��ǂݍ���
		size_t inner_size;
		File.read(reinterpret_cast<char*>(&inner_size), sizeof(inner_size));

		inner_vector.resize(inner_size);

		// �f�[�^�ǂݍ���
		File.read(reinterpret_cast<char*>(inner_vector.data()), inner_size * sizeof(CMapBlockInfo::SObsacleInfo));
	}

	// �t�@�C�������
	File.close();

	return loaddata;
}

//==========================================================================
// ������ǂݍ���
//==========================================================================
std::vector<std::vector<CWaterStone_Manager::SStoneInfo>> CMapBlock::LoadBin_WaterStone()
{
	// �t�@�C�����J��
	std::ifstream File(FILE_WATERSTONE, std::ios::binary);
	if (!File.is_open()) {
		// ��O����
		return std::vector<std::vector<CWaterStone_Manager::SStoneInfo>>();
	}

	// �܂��A�O���̃x�N�g���̃T�C�Y��ǂݍ���
	size_t outer_size;
	File.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));

	std::vector<std::vector<CWaterStone_Manager::SStoneInfo>> loaddata(outer_size);

	// �e�����̃x�N�g���ɑ΂��ăf�[�^��ǂݍ���
	for (auto& inner_vector : loaddata)
	{
		// �����̃x�N�g���̃T�C�Y��ǂݍ���
		size_t inner_size;
		File.read(reinterpret_cast<char*>(&inner_size), sizeof(inner_size));

		inner_vector.resize(inner_size);

		// �f�[�^�ǂݍ���
		File.read(reinterpret_cast<char*>(inner_vector.data()), inner_size * sizeof(CWaterStone_Manager::SStoneInfo));
	}

	// �t�@�C�������
	File.close();

	return loaddata;
}

//==========================================================================
// �W���b�W�ǂݍ���
//==========================================================================
std::vector<std::vector<std::vector<CMapBlockInfo::SJudgeInfo>>> CMapBlock::LoadBin_Judge()
{
	// �t�@�C�����J��
	std::ifstream File(FILE_JUDGE, std::ios::binary);
	if (!File.is_open()) {
		// ��O����
		return std::vector<std::vector<std::vector<CMapBlockInfo::SJudgeInfo>>>();
	}

	// �܂��A�O���̃x�N�g���̃T�C�Y��ǂݍ���
	size_t outer_size;
	File.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));

	std::vector<std::vector<std::vector<CMapBlockInfo::SJudgeInfo>>> loaddata(outer_size);

	// �e�����̃x�N�g���ɑ΂��ăf�[�^��ǂݍ���
	for (auto& inner_vector : loaddata)
	{
		// ���Ԃ̃x�N�g���̃T�C�Y��ǂݍ���
		size_t middle_size;
		File.read(reinterpret_cast<char*>(&middle_size), sizeof(middle_size));

		inner_vector.resize(middle_size);

		for (auto& inner_inner_vector : inner_vector)
		{
			// �����̃x�N�g���̃T�C�Y��ǂݍ���
			size_t inner_size;
			File.read(reinterpret_cast<char*>(&inner_size), sizeof(inner_size));

			inner_inner_vector.resize(inner_size);

			// �f�[�^�ǂݍ���
			File.read(reinterpret_cast<char*>(inner_inner_vector.data()), inner_size * sizeof(CMapBlockInfo::SJudgeInfo));
		}
	}

	// �t�@�C�������
	File.close();

	return loaddata;
}

//==========================================================================
// ���x���ǂݍ���
//==========================================================================
std::vector<int> CMapBlock::LoadBin_Level(size_t courseSize)
{
	// �t�@�C�����J��
	std::ifstream File(FILE_LEVEL, std::ios::binary);
	if (!File.is_open()) {
		// ��O����

		std::vector<int> returndata(courseSize);
		return returndata;
	}

	// �x�N�g���̃T�C�Y��ǂݍ���
	size_t outer_size;
	File.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));

	std::vector<int> loaddata(outer_size);

	// �x�N�g���ɑ΂��ăf�[�^��ǂݍ���
	for (auto& inner : loaddata)
	{
		// �f�[�^�ǂݍ���
		File.read(reinterpret_cast<char*>(&inner), sizeof(int));
	}

	// �t�@�C�������
	File.close();

	return loaddata;
}

//==========================================================================
// �z�u�ʒu�ݒ�
//==========================================================================
void CMapBlock::Set(const int Idx, const MyLib::Vector3& startpos, float startlength)
{
	CMapBlockInfo* pInfo = m_InfoList.GetData(Idx);
	if (pInfo == nullptr)return;

	// �I�u�W�F�N�g�̔z�u
	{
		CMap_ObstacleManager* pManager = CMap_ObstacleManager::GetInstance();

		for (const auto& it : pInfo->GetObstacleInfo())
		{
			CMap_Obstacle* pObj = CMap_Obstacle::Create(pManager->GetObstacleInfo(it.nType), it.pos + startpos);
			pObj->SetRotation(it.rot);
			pObj->SetScale(it.scale);
		}
	}

	// �}�b�v�̔z�u
	{
		// ��Q���̃��X�g�擾
		std::vector<CMapBlockInfo::SObsacleInfo> mapInfo = pInfo->GetMapInfo();
		for (int i = 0; i < static_cast<int>(mapInfo.size()); i++)
		{
			// �u���b�N�̏�Q�����
			CMapBlockInfo::SObsacleInfo blockinfo = mapInfo[i];

			CObjectX* pObj = CObjectX::Create(CEdit_Map::GetModelFileName()[blockinfo.nType], blockinfo.pos + startpos);
			pObj->SetRotation(blockinfo.rot);
			pObj->SetScale(blockinfo.scale);
			pObj->CalWorldMtx();
			pObj->SetType(CObject::TYPE_OBJECTX);
			CFrontObjManager::GetInstance()->Regist(pObj);
		}
	}

	// ��̔z�u
	{
		for (const auto& it : pInfo->GetWaterStoneInfo())
		{
			CWaterStone_Manager::SStoneInfo info = it;
			info.pos += startpos;
			CWaterStone* pObj = CWaterStone::Create(info);
		}
	}

	// �`�F�b�N�|�C���g�̔z�u
	{
		for (const auto& it : pInfo->GetCheckpointInfo())
		{
			CCheckpoint* p = CCheckpoint::Create(it + startlength);
			m_CheckpointList.Regist(p);
		}
	}

	// �W���b�W�A�C�e���̔z�u
	{
		for (const auto& vecjudge : pInfo->GetJudgeInfo())
		{
			// �W���b�W�A�C�e���}�l�[�W������
			CJudgeItemManager::Create(vecjudge, startlength, Idx);
		}
	}

	m_List.Regist(this);
}

//==========================================================================
// ���ǉ�
//==========================================================================
void CMapBlock::AddInfo()
{
	// �������ă��X�g�̊Ǘ�����
	CMapBlockInfo* pBlock = DEBUG_NEW CMapBlockInfo;
	pBlock->Init();
	m_InfoList.Regist(pBlock);
}

//==========================================================================
// ���폜
//==========================================================================
void CMapBlock::SubInfo()
{
	m_InfoList.Delete(m_InfoList.GetData(m_InfoList.GetNumAll() - 1));
}

//==========================================================================
// �f�[�^�̍폜
//==========================================================================
void CMapBlock::DataDelete()
{
	//// �`�F�b�N�|�C���g�̏I��
	//for (int i = 0; i < m_CheckpointList.GetNumAll(); i++)
	//{
	//	m_CheckpointList.GetData(i)->Uninit();
	//}

	//m_CheckpointList.Uninit();


	//// ��Q���̏I��
	//for (int i = 0; i < m_ObstacleList.GetNumAll(); i++)
	//{
	//	m_ObstacleList.GetData(i)->Uninit();
	//}

	//m_ObstacleList.Uninit();
}

//void CMapBlock::DataDelete()
//{
//
//}
//
//void CMapBlock::InfoDelete()
//{
//
//}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMapBlockInfo::CMapBlockInfo()
{
	m_nLevel = 0;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMapBlockInfo::~CMapBlockInfo()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMapBlockInfo::Init()
{
	m_MapList.clear();
	m_CheckpointList.clear();
	m_ObstacleList.clear();
	m_WaterStoneList.clear();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMapBlockInfo::Uninit()
{
	m_CheckpointList.clear();
	m_ObstacleList.clear();
	m_WaterStoneList.clear();

}

//==========================================================================
// ���[�h
//==========================================================================
void CMapBlockInfo::Load(std::ifstream* pFile)
{
	// �R�����g�p
	std::string hoge;

	// �f�[�^�ǂݍ���
	std::string line;

	while (line.find("END_BLOCKSET") == std::string::npos)
	{
		std::getline(*pFile, line);

		if (line.find("CHECKPOINTSET") != std::string::npos)
		{// �`�F�b�N�|�C���g�z�u

			float length;

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>	// ��
				length;	// �z�u���̎��

			// �������ĊǗ�
			m_CheckpointList.push_back(length);

			continue;
		}

		if (line.find("LEVELSET") != std::string::npos)
		{// ��Փx�ݒ�

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>	// ��
				m_nLevel;	// ��Փx�̐ݒ�

			continue;
		}

		if (line.find("MODELSET") != std::string::npos)
		{// POS�ňʒu

			// ��Q�����ǂݍ���
			ObstacleLoad(pFile);

			continue;
		}
	}
}

//==========================================================================
// ��Q���ǂݍ���
//==========================================================================
void CMapBlockInfo::ObstacleLoad(std::ifstream* pFile)
{
	// �ǂݍ��ݏ��
	int type;
	MyLib::Vector3 pos, rot, scale(1.0f);
	CMap_ObstacleManager* pManager = CMap_ObstacleManager::GetInstance();

	// �R�����g�p
	std::string hoge;

	// �f�[�^�ǂݍ���
	std::string line;

	while (line.find("END_MODELSET") == std::string::npos)
	{
		std::getline(*pFile, line);

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
	if (type < static_cast<int>(pManager->GetObstacleInfo().size()))
	{
		SObsacleInfo info;
		info.pos = pos;
		info.rot = rot;
		info.scale = scale;
		info.nType = type;
		m_ObstacleList.push_back(info);
	}
}