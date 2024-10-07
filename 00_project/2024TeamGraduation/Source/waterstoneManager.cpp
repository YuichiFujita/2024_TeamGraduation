//=============================================================================
// 
//  �}�b�v�̏�Q���}�l�[�W������ [waterstoneManager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "waterstoneManager.h"
#include "waterstone.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXT = "data\\TEXT\\map\\waterstone.bin";	// �ݒu���t�@�C��
}
CWaterStone_Manager* CWaterStone_Manager::m_ThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CWaterStone_Manager::CWaterStone_Manager()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CWaterStone_Manager::~CWaterStone_Manager()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CWaterStone_Manager *CWaterStone_Manager::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// �������̊m��
		m_ThisPtr = DEBUG_NEW CWaterStone_Manager;

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
HRESULT CWaterStone_Manager::Init()
{
	// �ǂݍ���
	//Load();
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CWaterStone_Manager::Uninit()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// �폜
//==========================================================================
void CWaterStone_Manager::Kill()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CWaterStone_Manager::Update()
{
	

}

//==========================================================================
// �Z�[�u
//==========================================================================
void CWaterStone_Manager::Save()
{

	// ��Q���̃��X�g�擾
	CListManager<CWaterStone> list = CWaterStone::GetListObj();

	// �擪��ۑ�
	std::list<CWaterStone*>::iterator itr = list.GetEnd();
	CWaterStone* pObj = nullptr;

	// �Ώ��̃R���e�i
	std::vector<SStoneInfo> vecInfo;

	int i = 0;
	while (list.ListLoop(itr))
	{
		CWaterStone* pObj = *itr;
		MyLib::Vector3 ObjPos = pObj->GetPosition();

		vecInfo.push_back(pObj->GetStoneInfo());
	}



	// �t�@�C�����J��
	std::ofstream File(TEXT, std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	// �f�[�^���o�C�i���t�@�C���ɏ����o��
	File.write(reinterpret_cast<char*>(vecInfo.data()), vecInfo.size() * sizeof(SStoneInfo));

	// �t�@�C�������
	File.close();
}

//==========================================================================
// ���[�h
//==========================================================================
void CWaterStone_Manager::Load()
{
	// �t�@�C�����J��
	std::ifstream File(TEXT, std::ios::binary);
	if (!File.is_open()) {

		// ��O����
		CWaterStone::Create(SStoneInfo());
		Save();
		return;
	}

	// �\���̂̃T�C�Y���擾
	std::streamsize structSize = sizeof(SStoneInfo);

	// �t�@�C���̖����܂Ńf�[�^��ǂݍ���
	File.seekg(0, std::ios::end);
	std::streampos fileSize = File.tellg();
	File.seekg(0, std::ios::beg);

	// �f�[�^�̌����v�Z
	size_t numVectors = fileSize / structSize;


	// �Ώ��̃R���e�i
	std::vector<SStoneInfo> vecInfo;
	vecInfo.resize(numVectors);

	// �t�@�C������f�[�^��ǂݍ���
	File.read(reinterpret_cast<char*>(vecInfo.data()), fileSize);

	// �t�@�C�������
	File.close();


	// �ΐ���
	CreateStone(vecInfo);
	return;
}

//==========================================================================
// �ΐ���
//==========================================================================
void CWaterStone_Manager::CreateStone(const std::vector<SStoneInfo>& vecinfo)
{
	for (const auto& info : vecinfo)
	{
		CWaterStone::Create(info);
	}
}
