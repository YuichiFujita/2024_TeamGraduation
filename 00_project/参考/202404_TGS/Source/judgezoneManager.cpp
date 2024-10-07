//=============================================================================
// 
//  ����]�[���}�l�[�W�� [judgezoneManager.cpp]
//  Author : �Ό��D�n
// 
//============================================================================
#include "judgezoneManager.h"
#include "judgezone.h"
#include "judge.h"
#include "judgeobj.h"
#include "game.h"
#include "course.h"
#include "texture.h"
#include "object2D.h"
#include "spline.h"
#include "objectBillboard.h"
#include <map>
#include <direct.h>

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	std::map<CJudge::JUDGE, std::string> TEXTURE =
	{
		{CJudge::JUDGE::JUDGE_AAA,"data\\TEXTURE\\judge_test_01.png"},
		{CJudge::JUDGE::JUDGE_BBB,"data\\TEXTURE\\judge_test_02.png"},
		{CJudge::JUDGE::JUDGE_CCC,"data\\TEXTURE\\judge_test_03.png"},
		{CJudge::JUDGE::JUDGE_DDD,"data\\TEXTURE\\judge_test_04.png"},
	};
	const std::string TEXT_LINE = "#------------------------------------------------------------------------------";	// �e�L�X�g�̃��C��
	const int ADDPOINT[] =
	{
		15,	// AAA
		5,	// BBB
		-5,	// CCC
		-10,// DDD
	};
}
CJudgeZoneManager* CJudgeZoneManager::m_ThisPtr = nullptr;

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CJudgeZoneManager::CJudgeZoneManager()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CJudgeZoneManager::~CJudgeZoneManager()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CJudgeZoneManager *CJudgeZoneManager::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// �������̊m��
		m_ThisPtr = DEBUG_NEW CJudgeZoneManager;

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
HRESULT CJudgeZoneManager::Init()
{
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CJudgeZoneManager::Uninit()
{
	ReleaseAll();
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// ����m�F����
//==========================================================================
void CJudgeZoneManager::Check(float progress)
{
	CListManager<CJudgeZone> list = CJudgeZone::GetListObj();
	CListManager<CJudgeZone>::Iterator itr = list.GetEnd();
	while (list.ListLoop(itr))
	{
		if ((*itr)->IsEnable())
		{
			CJudgeZone::SJudgeZone zone = (*itr)->GetZone();
			if (progress >= zone.start && progress <= zone.end)
			{//�͈͓�
				(*itr)->Check();
			}
			else if (progress > zone.end)
			{//�I���i����j
				CJudge::JUDGE judge = (*itr)->Judge();	//�����ɔ��肪�����Ă�

				// �|�C���g���Z
				CGame::GetInstance()->GetGameManager()->AddEvaluationPoint(ADDPOINT[judge]);

				// �]�����Ƃ�UI����
				CJudgeObj* pObj	= CJudgeObj::Create(MyLib::Vector3(400.0f, 100.0f, 0.0f), judge);
				pObj->SetSize(D3DXVECTOR2(256.0f, 72.0f));
				pObj->SetSizeOrigin(D3DXVECTOR2(192.0f, 54.0f));

				(*itr)->Uninit();
			}

#ifdef _DEBUG
			CCourse* pCource = CGame::GetInstance()->GetCourse();
			MyLib::Vector3 pos;
			float length = pCource->GetCourceLength();

			//�X�^�[�g
			pos = MySpline::GetSplinePosition_NonLoop(pCource->GetVecPosition(), length * (*itr)->GetZone().start);
			pos.y = (*itr)->GetZone().borderHeight;
			CEffect3D::Create(
				pos,
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
				40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

			//�I��
			pos = MySpline::GetSplinePosition_NonLoop(pCource->GetVecPosition(), length * (*itr)->GetZone().end);
			pos.y = (*itr)->GetZone().borderHeight;
			CEffect3D::Create(
				pos,
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
				40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif // _DEBUG
		}
	}

#ifdef _DEBUG
	// �\���p�{�^��
	if (ImGui::Button("DispObj_AAA"))
	{
		CJudgeObj* pObj = CJudgeObj::Create(MyLib::Vector3(400.0f, 100.0f, 0.0f), CJudge::JUDGE::JUDGE_AAA);
		pObj->SetSize(D3DXVECTOR2(256.0f, 72.0f));
		pObj->SetSizeOrigin(D3DXVECTOR2(192.0f, 54.0f));

		// �|�C���g���Z
		CGame::GetInstance()->GetGameManager()->AddEvaluationPoint(ADDPOINT[CJudge::JUDGE::JUDGE_AAA]);
	}
	if (ImGui::Button("DispObj_BBB"))
	{
		CJudgeObj* pObj = CJudgeObj::Create(MyLib::Vector3(400.0f, 100.0f, 0.0f), CJudge::JUDGE::JUDGE_BBB);
		pObj->SetSize(D3DXVECTOR2(256.0f, 72.0f));
		pObj->SetSizeOrigin(D3DXVECTOR2(192.0f, 54.0f));

		// �|�C���g���Z
		CGame::GetInstance()->GetGameManager()->AddEvaluationPoint(ADDPOINT[CJudge::JUDGE::JUDGE_BBB]);
	}
	if (ImGui::Button("DispObj_CCC"))
	{
		CJudgeObj* pObj = CJudgeObj::Create(MyLib::Vector3(400.0f, 100.0f, 0.0f), CJudge::JUDGE::JUDGE_CCC);
		pObj->SetSize(D3DXVECTOR2(256.0f, 72.0f));
		pObj->SetSizeOrigin(D3DXVECTOR2(192.0f, 54.0f));

		// �|�C���g���Z
		CGame::GetInstance()->GetGameManager()->AddEvaluationPoint(ADDPOINT[CJudge::JUDGE::JUDGE_CCC]);
	}
	if (ImGui::Button("DispObj_DDD"))
	{
		CJudgeObj* pObj = CJudgeObj::Create(MyLib::Vector3(400.0f, 100.0f, 0.0f), CJudge::JUDGE::JUDGE_DDD);
		pObj->SetSize(D3DXVECTOR2(256.0f, 72.0f));
		pObj->SetSizeOrigin(D3DXVECTOR2(192.0f, 54.0f));

		// �|�C���g���Z
		CGame::GetInstance()->GetGameManager()->AddEvaluationPoint(ADDPOINT[CJudge::JUDGE::JUDGE_DDD]);
	}
#endif
	
	// �����������]�[���j��
	Release();
}

//==========================================================================
// �����]�[���j������
//==========================================================================
void CJudgeZoneManager::Release()
{
	CListManager<CJudgeZone> list = CJudgeZone::GetListObj();
	std::list<CJudgeZone*> removeList;
	std::list<CJudgeZone*>::iterator itr;
	itr = list.GetEnd();
	CJudgeZone* pObj = nullptr;
	while (list.ListLoop(itr))
	{
		if (!(*itr)->IsEnable())
		{
			removeList.push_back((*itr));
		}
	}
	
	for (itr = removeList.begin(); itr != removeList.end(); itr++)
	{
		(*itr)->Uninit();
		list.Delete((*itr));
	}
}

//==========================================================================
// �S�]�[���j������
//==========================================================================
void CJudgeZoneManager::ReleaseAll()
{
	CListManager<CJudgeZone> list = CJudgeZone::GetListObj();
	CJudgeZone* pZone = nullptr;
	while (list.ListLoop(&pZone))
	{
		pZone->Uninit();			// �I��
	}
}

//==========================================================================
// ����]�[���ޓǂݍ��ݏ���
//==========================================================================
void CJudgeZoneManager::Load(std::string path)
{
	CJudgeZone* pJudgeZone = nullptr;
	std::string aPath[2];

	// �t�@�C�����J��
	std::ifstream File(path);
	if (File.is_open()) {
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

			if (line.find("USEZONE") != std::string::npos)
			{// TYPE�Ŕz�u���̎��

				// �X�g���[���쐬
				std::istringstream lineStream(line);

				// ���n��
				std::string path;
				lineStream >>
					hoge >>
					hoge >>		// ��
					path;		// ���l
				path = UtilFunc::Transformation::ReplaceBackslash(path);

				//����]�[���ǂݍ���
				LoadZone(path);
			}

			if (line.find("END_SCRIPT") != std::string::npos)
			{
				break;
			}
		}

		// �t�@�C�������
		File.close();
	}
}

//==========================================================================
// ����]�[���ǂݍ��ݏ���
//==========================================================================
void CJudgeZoneManager::LoadZone(std::string path)
{
	CJudgeZone* pJudgeZone = nullptr;
	std::string aPath[2];

	// �t�@�C�����J��
	std::ifstream File(path);
	if (File.is_open()) {
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

			if (line.find("ZONESET") != std::string::npos)
			{// MODELSET�Ŕz�u���ǂݍ���

				// �ǂݍ��ݏ��
				CJudgeZone::SJudgeZone zone = CJudgeZone::SJudgeZone(0.0f, 1.0f, 0.0f);
				float border = 0.0f;

				while (line.find("END_ZONESET") == std::string::npos)
				{
					std::getline(File, line);
					if (line.find("START") != std::string::npos)
					{// TYPE�Ŕz�u���̎��

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							zone.start;	// ���l
					}
					if (line.find("END") != std::string::npos)
					{// TYPE�Ŕz�u���̎��

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							zone.end;	// ���l
					}
					if (line.find("BORDER") != std::string::npos)
					{// TYPE�Ŕz�u���̎��

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							border;		// ���l
					}
					else if (line.find("CONDITION_TOP") != std::string::npos)
					{// POS�ňʒu

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							aPath[0];	// �p�X
						aPath[0] = UtilFunc::Transformation::ReplaceBackslash(aPath[0]);
					}
					else if (line.find("CONDITION_UNDER") != std::string::npos)
					{// ROT�Ō���

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							aPath[1];	// �p�X
						aPath[1] = UtilFunc::Transformation::ReplaceBackslash(aPath[1]);
					}
				}

				// ��荞��
				if (pJudgeZone == nullptr)
				{
					pJudgeZone = CJudgeZone::Create(zone.start, zone.end, border);
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

	// �����ǂݍ���
	if (pJudgeZone != nullptr)
	{
		CListManager<CJudgeZone> list = CJudgeZone::GetListObj();

		pJudgeZone->SetPath(path);
		pJudgeZone->SetInfo(CJudge::BORDER::TOP, LoadCondition(aPath[CJudge::BORDER::TOP]));
		pJudgeZone->SetInfo(CJudge::BORDER::UNDER, LoadCondition(aPath[CJudge::BORDER::UNDER]));
		list.Regist(pJudgeZone);
	}
}

//==========================================================================
// �����ǂݍ��ݏ���
//==========================================================================
CJudge::SJudgeCondition CJudgeZoneManager::LoadCondition(std::string path)
{
	CJudge::SJudgeCondition info;
	info.type = CJudge::CONDITIONTYPE::TYPE_NONE;
	info.judgeParam = 
	{
		{CJudge::JUDGE::JUDGE_AAA,-1},
		{CJudge::JUDGE::JUDGE_BBB,-1},
		{CJudge::JUDGE::JUDGE_CCC,-1},
		{CJudge::JUDGE::JUDGE_DDD,-1},
	};

	// �t�@�C�����J��
	std::ifstream File(path);
	if (File.is_open()) {
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

			if (line.find("CONDITIONSET") != std::string::npos)
			{// MODELSET�Ŕz�u���ǂݍ���
				while (line.find("END_CONDITIONSET") == std::string::npos)
				{
					std::getline(File, line);
					if (line.find("TYPE") != std::string::npos)
					{// TYPE�Ŕz�u���̎��
						int num = 0;
						
						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							num;		// ���l

						info.type = static_cast<CJudge::CONDITIONTYPE>(num);
					}
					if (line.find("JUDGE_AAA") != std::string::npos)
					{// TYPE�Ŕz�u���̎��

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							info.judgeParam[CJudge::JUDGE::JUDGE_AAA];	// ���l
					}
					if (line.find("JUDGE_BBB") != std::string::npos)
					{// TYPE�Ŕz�u���̎��

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							info.judgeParam[CJudge::JUDGE::JUDGE_BBB];	// ���l
					}
					if (line.find("JUDGE_CCC") != std::string::npos)
					{// TYPE�Ŕz�u���̎��

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							info.judgeParam[CJudge::JUDGE::JUDGE_CCC];	// ���l
					}
					if (line.find("JUDGE_DDD") != std::string::npos)
					{// TYPE�Ŕz�u���̎��

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							info.judgeParam[CJudge::JUDGE::JUDGE_DDD];	// ���l
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
	return info;
}

//==========================================================================
// ����]�[�����X�g�������ݏ���
//==========================================================================
void CJudgeZoneManager::Save(std::string path)
{
	// �t�@�C�����J��
	std::ofstream File(path, std::ios_base::out);
	if (!File.is_open()) {
		return;
	}

	int path_Length = path.find_last_of("\\");
	int name_Length = path.size() - path.find_last_of("\\");
	std::string fileName_ext = path.substr(path_Length + 1, name_Length + 1);
	std::string filename = fileName_ext.substr(0, fileName_ext.find_last_of("."));
	File << "#====================================================================================================" << std::endl;
	File << "#" << std::endl;
	File << "# ����]�[�����X�g�X�N���v�g�t�@�C�� [" << fileName_ext << "]" << std::endl;
	File << "# Author : �G�f�B�^�����o��" << std::endl;
	File << "#" << std::endl;
	File << "#====================================================================================================" << std::endl;
	File << "SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI\n" << std::endl;
	File << std::endl;
	File << TEXT_LINE << std::endl;
	File << "# ����]�[���p�X���" << std::endl;
	File << "# USEZONE	: �g�p���锻��]�[���̃t�@�C���p�X" << std::endl;
	File << TEXT_LINE << std::endl;

	CListManager<CJudgeZone> list = CJudgeZone::GetListObj();
	auto itr = list.GetEnd();
	while(list.ListLoop(itr))
	{
		// �]�[���P�̂̃p�X�����o��
		File << "USEZONE = " << (*itr)->GetPath() << std::endl;
	}

	// �t�@�C�������
	File << "\nEND_SCRIPT\t\t# ���̍s�͐�Ώ����Ȃ����ƁI" << std::endl;
	File.close();
}

//==========================================================================
// ����]�[���������ݏ���
//==========================================================================
void CJudgeZoneManager::SaveZone(std::string path, CJudgeZone::SJudgeZone zoneinfo, CJudge::SJudgeCondition conditionUp, CJudge::SJudgeCondition conditionUnder)
{
	// �]�[���t�@�C���p�X����
	int path_Length = path.find_last_of("\\");
	int name_Length = path.size() - path.find_last_of("\\");
	std::string dirName = path.substr(0, path_Length + 1);
	std::string fileName_ext = path.substr(path_Length + 1, name_Length + 1);
	std::string filename = fileName_ext.substr(0, fileName_ext.find_last_of("."));

	// �����t�@�C���p�X�쐬
	std::string conditionPath_base = "data\\TEXT\\judgecondition\\" + filename;
	std::string pathConditionUp, pathConditionUnder;
	pathConditionUp = conditionPath_base + "_condition_Top.txt";
	pathConditionUnder = conditionPath_base + "_condition_Under.txt";

	// �����t�@�C�������o��
	_mkdir("data\\TEXT\\judgecondition");
	if (!SaveCondition(pathConditionUp, conditionUp))
	{
		pathConditionUp = "<<< FAILED >>>";
	}
	if (!SaveCondition(pathConditionUnder, conditionUnder))
	{
		pathConditionUnder = "<<< FAILED >>>";
	}

	// �t�@�C�����J��
	std::ofstream File(path, std::ios_base::out);
	if (!File.is_open()) {
		return;
	}

	File << "#====================================================================================================" << std::endl;
	File << "#" << std::endl;
	File << "# ����]�[���X�N���v�g�t�@�C�� [" << fileName_ext << "]" << std::endl;
	File << "# Author : �G�f�B�^�����o��" << std::endl;
	File << "#" << std::endl;
	File << "#====================================================================================================" << std::endl;
	File << "SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI\n" << std::endl;
	File << std::endl;
	File << TEXT_LINE << std::endl;
	File << "# ����]�[�����" << std::endl;
	File << "# START				: ����J�n�n�_�i�i�s�x�����\�L, �����łǂ����̏����ɂ��邩����j" << std::endl;
	File << "# END : ����I���n�_�i�i�s�x�����\�L, �����Ŕ��肪�o��j" << std::endl;
	File << "# BORDER : ��Ɖ��̋��E���̍���" << std::endl;
	File << "# CONDITION_TOP : ��̏����t�@�C���p�X" << std::endl;
	File << "# CONDITION_UNDER : ���̏����t�@�C���p�X" << std::endl;
	File << TEXT_LINE << std::endl;

	{
		// �]�[�����ݒ�
		File << "ZONESET" << std::endl;

		// �ϐ���
		File << "START = " << zoneinfo.start << std::endl;
		File << "END = " << zoneinfo.end << std::endl;
		File << "BORDER = " << zoneinfo.borderHeight << std::endl;

		// �����t�@�C���p�X
		File << "CONDITION_TOP = " << pathConditionUp << std::endl;
		File << "CONDITION_UNDER = " << pathConditionUnder << std::endl;

		// �I���
		File << "END_ZONESET" << std::endl;
	}

	// �t�@�C�������
	File << "\nEND_SCRIPT\t\t# ���̍s�͐�Ώ����Ȃ����ƁI" << std::endl;
	File.close();
}

//==========================================================================
// ��������������ݏ���
//==========================================================================
bool CJudgeZoneManager::SaveCondition(std::string path, CJudge::SJudgeCondition condition)
{
	// �t�@�C�����J��
	std::ofstream File(path, std::ios_base::out);
	if (!File.is_open()) {
		return false;
	}

	int path_Length = path.find_last_of("\\");
	int name_Length = path.size() - path.find_last_of("\\");
	std::string fileName_ext = path.substr(path_Length + 1, name_Length + 1);
	std::string filename = fileName_ext.substr(0, fileName_ext.find_last_of("."));
	File << "#====================================================================================================" << std::endl;
	File << "#" << std::endl;
	File << "# ��������X�N���v�g�t�@�C�� [" << fileName_ext << "]" << std::endl;
	File << "# Author : �G�f�B�^�����o��" << std::endl;
	File << "#" << std::endl;
	File << "#====================================================================================================" << std::endl;
	File << "SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI\n" << std::endl;
	File << std::endl;
	File << TEXT_LINE << std::endl;
	File << "# ����]�[�����" << std::endl;
	File << "# TYPE		: ��ށi0: ������, 1 : �Ԃ������񐔁j" << std::endl;
	File << "# JUDGE_xxx(AAA/BBB/CCC/DDD) : �����𖞂������l�i -1�ȉ��Ŕ�����g�p���Ȃ��j" << std::endl;
	File << "#	TYPE = 0 : ��������̐��l�������Ă��锻�肪�K�p�B��������ꍇ���l���傫�������K�p�j" << std::endl;
	File << "#	TYPE = 1 : �Ԃ������񐔁i�w�肵�����l�ȉ��ł��̔���ɂȂ�j" << std::endl;
	File << TEXT_LINE << std::endl;

	{
		// �]�[�����ݒ�
		File << "CONDITIONSET" << std::endl;

		// ���
		File << "TYPE = " << static_cast<int>(condition.type) << std::endl;

		// ������
		File << "JUDGE_AAA = " << condition.judgeParam[CJudge::JUDGE::JUDGE_AAA] << std::endl;
		File << "JUDGE_BBB = " << condition.judgeParam[CJudge::JUDGE::JUDGE_BBB] << std::endl;
		File << "JUDGE_CCC = " << condition.judgeParam[CJudge::JUDGE::JUDGE_CCC] << std::endl;
		File << "JUDGE_DDD = " << condition.judgeParam[CJudge::JUDGE::JUDGE_DDD] << std::endl;

		// �I���
		File << "END_CONDITIONSET" << std::endl;
	}

	// �t�@�C�������
	File << "\nEND_SCRIPT\t\t# ���̍s�͐�Ώ����Ȃ����ƁI" << std::endl;
	File.close();

	return true;
}

