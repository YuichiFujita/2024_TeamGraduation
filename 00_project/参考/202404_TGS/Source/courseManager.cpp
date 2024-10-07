//=============================================================================
// 
//  �R�[�X�}�l�[�W������ [courseManager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "courseManager.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"

#include "course.h"
#include "waterfield.h"
#include "stonewall.h"
#include "stonewall_front.h"
#include "map_block.h"
#include "spline.h"
#include "frontobj_manager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\map\\chunkdata.bin";
	const int NUM_CHUNK = 5;	// �`�����N�̐�
	const int DECIDE_LEVEL[] =	// ���x��
	{
		0,
		0,
		1,
		1,
		2
	};

	const std::vector<MyLib::Vector3> DEFAULT_SEGMENTPOS =
	{
		//{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 4000.0f, 0.0f, 0.0f },
		{ 8000.0f, 0.0f, 0.0f },
		{ 12000.0f, 0.0f, 0.0f },
		//{ 9000.0f, 0.0f, 0.0f },
	};
	const float DISTANCE_TO_CHUNCK = 1500.0f;	// �`�����N���m�̊Ԋu
}
CCourseManager* CCourseManager::m_ThisPtr = nullptr;	// ���g�̃|�C���^
const float CCourseManager::m_fBlockLength = 9000.0f;	// �u���b�N�̒���

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCourseManager::CCourseManager()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCourseManager::~CCourseManager()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CCourseManager *CCourseManager::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// �������̊m��
		m_ThisPtr = DEBUG_NEW CCourseManager;

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
HRESULT CCourseManager::Init()
{
	CFrontObjManager::Create();
	// �ǂݍ���
	Load();
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCourseManager::Uninit()
{
	CFrontObjManager::GetInstance()->Uninit();

	delete m_ThisPtr;
	m_ThisPtr = nullptr;

	CMapBlock::Kill();
}

//==========================================================================
// �폜
//==========================================================================
void CCourseManager::Kill()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CCourseManager::Update()
{
	

}

//==========================================================================
// �Z�[�u
//==========================================================================
void CCourseManager::Save()
{
	// �t�@�C�����J��
	std::ofstream File(FILENAME, std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	std::vector<std::vector<MyLib::Vector3>> savedata = m_vecAllSegmentPos;

	// �O���x�N�g���̃T�C�Y����������
	size_t outer_size = savedata.size();
	File.write(reinterpret_cast<const char*>(&outer_size), sizeof(outer_size));

	// ���ꂼ��̃u���b�N�̍ŏ��ƍŌ����
	for (auto& data : savedata)
	{
		/*data.erase(data.begin());
		data.pop_back();*/

		// �f�[�^�̌����v�Z
		size_t vecSize = data.size();

		// �x�N�g���̃T�C�Y���Z�[�u
		File.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

		// �x�N�g�����̗v�f���Z�[�u
		File.write(reinterpret_cast<const char*>(data.data()), vecSize * sizeof(MyLib::Vector3));
	}

	// �t�@�C�������
	File.close();

	// �o�C�i���ۑ�
	CMapBlock::SaveBin();

	//// �`�F�b�N�|�C���g�ۑ�
	//CMapBlock::SaveBin_CheckPoint();

	//// ��Q���ۑ�
	//CMapBlock::SaveBin_Obstacle();

	//// ������ۑ�
	//CMapBlock::SaveBin_WaterStone();
}

//==========================================================================
// ���[�h
//==========================================================================
void CCourseManager::Load()
{
	
	//=============================
	// �S�u���b�N���(�R�[�X)��ǂݍ��ށB��_�̈ʒu
	//=============================
	// �t�@�C�����J��
	std::ifstream File(FILENAME, std::ios::binary);
	if (!File.is_open()) {
		// ��O����

		m_vecAllSegmentPos.emplace_back();

		m_vecAllSegmentPos[0] = DEFAULT_SEGMENTPOS;
		/*m_vecAllSegmentPos[0].insert(m_vecAllSegmentPos[0].begin(), MyLib::Vector3(0.0f, 0.0f, 0.0f));
		m_vecAllSegmentPos[0].push_back(DEFAULT_SEGMENTPOS.back());*/

		Save();
		return;
	}



	// �܂��A�O���̃x�N�g���̃T�C�Y��ǂݍ���
	size_t outer_size;
	File.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));


	std::vector<std::vector<MyLib::Vector3>> loaddata(outer_size);

	// �e�����̃x�N�g���ɑ΂��ăf�[�^��ǂݍ���
	for (auto& inner_vector : loaddata)
	{
		// �����̃x�N�g���̃T�C�Y��ǂݍ���
		size_t inner_size;
		File.read(reinterpret_cast<char*>(&inner_size), sizeof(inner_size));

		inner_vector.resize(inner_size);

		// �f�[�^�ǂݍ���
		File.read(reinterpret_cast<char*>(inner_vector.data()), inner_size * sizeof(MyLib::Vector3));
	}

	// �t�@�C�������
	File.close();


	// ���[�h���R�s�[
	m_vecAllSegmentPos = loaddata;


	// �����_���I�o���ꂽ�u���b�N�ɕt������A�`�F�b�N�|�C���g�A��Q���̐���
	// Block�̓ǂݍ���(��Q���A�`�F�b�N�|�C���g)
	CMapBlock::Load();

	for(auto& vec : m_vecAllSegmentPos)
	{
		while (1)
		{
			if (vec[0] != MyLib::Vector3(0.0f, 0.0f, 0.0f))
			{
				vec.erase(vec.begin());
				continue;
			}
			break;
		}

		int i = 0;
		while (1)
		{
			if (vec.size() <= i)
			{
				break;
			}

			if (vec[i].x <= 12000.0f)
			{
				i++;
				continue;
			}

			if (vec[i].x > 12000.0f)
			{
				vec.erase(vec.begin() + i);

				if (vec.size() <= i)
				{
					break;
				}
			}
		}

	}

	// �Z�[�u
	Save();


	//=============================
	// �����_���I�o
	//=============================
	// �����̎��ݒ�
	srand((unsigned int)time(0));
	int segmentSize = static_cast<int>(m_vecAllSegmentPos.size()) - 1;

	std::vector<int> randIdx;

	// �I�o�����C���f�b�N�X�̃u���b�N���
	CListManager<CMapBlockInfo> BlockInfoList = CMapBlock::GetInfoList();
	int nowLevel = 0;

	for (int i = 0; i < NUM_CHUNK; i++)
	{
		int idx = 0;
		while (1)
		{
			idx = UtilFunc::Transformation::Random(0, segmentSize);
			
			// �I�o�����C���f�b�N�X�̃��x��
			nowLevel = BlockInfoList.GetData(idx)->GetLevel();

			// �O��ƈႤ�ԍ��ɂ���
			if ((i != 0 && segmentSize > 1))
			{
				if (idx == randIdx[i - 1]) continue;
			}

			// �Y�����x���ƈႤ�ꍇ
			if (DECIDE_LEVEL[i] != nowLevel)
			{
				continue;
			}

			break;
		}

		// �I�o���ꂽ�ԍ���ǉ�
		randIdx.push_back(idx);
	}


	//=============================
	// ��{�̃R�[�X�ɂ���
	//=============================
	std::vector<MyLib::Vector3> segmentpos;	// ��_�̈ʒu
	std::vector<float> segmentlength;
	int size = 0;
	MyLib::Vector3 start;
	std::vector<MyLib::Vector3> vecstart;	// ��_�̈ʒu
	vecstart.push_back(MyLib::Vector3(2000.0f, 0.0f, 0.0f));
	start = vecstart.back() + MyLib::Vector3(DISTANCE_TO_CHUNCK, 0.0f, 0.0f);
	segmentpos.push_back(MyLib::Vector3(-3010.0f, 0.0f, 0.0f));
	segmentpos.push_back(MyLib::Vector3(-3000.0f, 0.0f, 0.0f));
	segmentpos.push_back(MyLib::Vector3(2000.0f, 0.0f, 0.0f));
	segmentlength.push_back(2000.0f);

	int i = 0;
	const float lastDistance = 20000.0f;
	for (const auto& idx : randIdx)
	{
		for (const auto& pos : m_vecAllSegmentPos[idx])
		{
			segmentpos.push_back(start + pos);
		}

		// �Ԋu�ǉ�
		if (i == randIdx.size() - 1)
		{// ���X�g
			segmentpos.push_back(segmentpos.back() + MyLib::Vector3(lastDistance, 0.0f, 0.0f));
			start = segmentpos.back() + MyLib::Vector3(lastDistance, 0.0f, 0.0f);
		}
		else
		{
			float distance = DISTANCE_TO_CHUNCK / 8.0f;
			
			for (int push = 0; push < 8; push++)
			{
				segmentpos.push_back(segmentpos.back() + MyLib::Vector3(distance, 0.0f, 0.0f));
			}
			start = segmentpos.back() + MyLib::Vector3(DISTANCE_TO_CHUNCK, 0.0f, 0.0f);
		}
		vecstart.push_back(start);
		i++;

		// �����܂ł̒����擾
		size += segmentpos.size();
		std::vector<float> calLength;
		calLength.resize(size);

		segmentlength.push_back(MySpline::CalSegmentLength_NonLoop(segmentpos, &calLength, 10.0f));
	}
	segmentpos.push_back(segmentpos.back() + MyLib::Vector3(10.0f, 0.0f, 0.0f));


	//=============================
	// �R�[�X�쐬
	//=============================
	CCourse* pCourse = CCourse::Create("data\\TEXT\\map\\course.bin");
	pCourse->SetVecPosition(segmentpos);
	pCourse->ReCreateVtx();
	CGame::GetInstance()->SetCource(pCourse);

	// �����ɂ��킹���z�u���s��
	for (int i = 0; i < NUM_CHUNK; i++)
	{
		CMapBlock* pBlock = DEBUG_NEW CMapBlock;

		float f = 3000.0f;

		if (i != 0)
		{
			f = 1450.0f;
		}

		if (pBlock != nullptr)
		{
			pBlock->Set(randIdx[i], vecstart[i], segmentlength[i] + f);
		}
	}


	//���̒��ŏ�Q���A�`�F�b�N�|�C���g

	//=============================
	// �Ί_(��)
	//=============================
	m_pStoneWall = CStoneWall::Create();

	// ��_�n�_�ݒ�
	m_pStoneWall->SetVecPosition(pCourse->GetVecPosition());
	m_pStoneWall->Reset();

	std::vector<CCourse::VtxInfo> vtxInfo = pCourse->GetVecVtxinfo();
	std::vector<MyLib::Vector3> vecpos;

	MyLib::Vector3 setpos;
	for (const auto& info : vtxInfo)
	{
		setpos.x = info.pos.x + sinf(D3DX_PI + info.rot.y) * -600.0f;
		setpos.y = info.pos.y;
		setpos.z = info.pos.z + cosf(D3DX_PI + info.rot.y) * -600.0f;
		vecpos.push_back(setpos);
	}

	// �e���_���W
	m_pStoneWall->SetVecVtxPosition(vecpos);
	m_pStoneWall->BindVtxPosition();


	//=============================
	// �Ί_(��O)
	//=============================
	m_pStoneWall_Front = CStoneWall_Front::Create();

	// ��_�n�_�ݒ�
	m_pStoneWall_Front->SetVecPosition(pCourse->GetVecPosition());
	m_pStoneWall_Front->Reset();

	vtxInfo = pCourse->GetVecVtxinfo();
	vecpos.clear();

	for (const auto& info : vtxInfo)
	{
		setpos.x = info.pos.x + sinf(D3DX_PI + info.rot.y) * 800.0f;
		setpos.y = info.pos.y;
		setpos.z = info.pos.z + cosf(D3DX_PI + info.rot.y) * 800.0f;
		vecpos.push_back(setpos);
	}

	// �e���_���W
	m_pStoneWall_Front->SetVecVtxPosition(vecpos);
	m_pStoneWall_Front->BindVtxPosition();


#if 1
	//=============================
	// ���˂�̊X�t�B�[���h
	//=============================
	CMapMesh* pTownField = CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_SINUOUS);
	pTownField->SetVecPosition(pCourse->GetVecPosition());
	pTownField->Reset();

	// �Ί_�̒���ɒ��_�����낦��
	pTownField->SetVecVtxPosition(m_pStoneWall->GetVecTopPosition());
	pTownField->BindVtxPosition();


	//=============================
	// ���˂�̊X�t�B�[���h(��O)
	//=============================
	CMapMesh* pTownFieldFront = CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_SINUOUS_FRONT);
	pTownFieldFront->SetVecPosition(pCourse->GetVecPosition());
	pTownFieldFront->Reset();

	// �Ί_�̒���ɒ��_�����낦��
	pTownFieldFront->SetVecVtxPosition(m_pStoneWall_Front->GetVecTopPosition());
	pTownFieldFront->BindVtxPosition();

	//=============================
	// �Œ蕽�ʊX�t�B�[���h
	//=============================
	CMapMesh* pTownFieldFixed = CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_FIXEDPLANE);
#endif
}

//==========================================================================
// ��_�ǉ�
//==========================================================================
void CCourseManager::AddSegmentPos()
{
	m_vecAllSegmentPos.emplace_back();
	m_vecAllSegmentPos.back() = DEFAULT_SEGMENTPOS;
}

//==========================================================================
// ��_�폜
//==========================================================================
void CCourseManager::SubSegmentPos()
{
	if (static_cast<int>(m_vecAllSegmentPos.size()) < 1) return;
	m_vecAllSegmentPos.pop_back();
}

//==========================================================================
// ��_�폜
//==========================================================================
void CCourseManager::SubSegmentPos(int idx)
{
	if (static_cast<int>(m_vecAllSegmentPos.size()) < 1) return;

	if (static_cast<int>(m_vecAllSegmentPos.size()) <= idx) return;

	std::vector<std::vector<MyLib::Vector3>>::iterator itr = m_vecAllSegmentPos.begin();
	std::advance(itr, idx);
	m_vecAllSegmentPos.erase(itr);

}
