//=============================================================================
// 
//  �ו��}�l�[�W�� [baggageManager.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "baggageManager.h"
#include "baggage.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXT = "data\\TEXT\\baggage\\baggage_info.txt";	// �ݒu���X�N���v�g�t�@�C��
	const std::string TEXT_LINE = "#------------------------------------------------------------------------------";	// �e�L�X�g�̃��C��
}
CBaggageManager* CBaggageManager::m_ThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBaggageManager::CBaggageManager()
{
	m_BaggageInfo.clear();	// ��Q�����
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBaggageManager::~CBaggageManager()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CBaggageManager *CBaggageManager::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// �������̊m��
		m_ThisPtr = DEBUG_NEW CBaggageManager;

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
HRESULT CBaggageManager::Init()
{
	// �ǂݍ���
	Load();
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CBaggageManager::Uninit()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// �폜
//==========================================================================
void CBaggageManager::Kill()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CBaggageManager::Update()
{
	

}

//==========================================================================
// ��ގw�肵�Đ���
//==========================================================================
CBaggage* CBaggageManager::CreateBaggage(CBaggage::TYPE idx)
{
	return CBaggage::Create(idx, m_BaggageInfo[idx]);
}

//==========================================================================
// �Z�[�u
//==========================================================================
void CBaggageManager::Save()
{
	// �t�@�C�����J��
	std::ofstream File(TEXT);
	if (File.is_open()) 
	{
		File << "#====================================================================================================" << std::endl;
		File << "#" << std::endl;
		File << "# �ו����X�N���v�g�t�@�C�� [baggage_info.txt]" << std::endl;
		File << "#" << std::endl;
		File << "#====================================================================================================" << std::endl;
		File << "SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI\n" << std::endl;

		// �ו����ڎ�
		File << TEXT_LINE << std::endl;
		File << "# �ו����" << std::endl;
		File << "# MODEL  : ���f���̃t�@�C���p�X" << std::endl;
		File << "# WEIGHT : �d��" << std::endl;
		File << "# WIDTH : �U�ꕝ" << std::endl;
		File << "# CYCLE : �����i���W�A���P�ʁj" << std::endl;
		File << "# LIFE : �̗�" << std::endl;
		File << TEXT_LINE << std::endl;

		// ���ݒ�
		for (const auto& info : m_BaggageInfo)
		{
			File << "BAGGAGESET" << std::endl;
			File << "MODEL = " << info.path << std::endl;
			File << "WEIGHT = " << info.weight << std::endl;
			File << "WIDTH = " << info.width << std::endl;
			File << "CYCLE = " << info.cycle << std::endl;
			File << "LIFE = " << info.life << std::endl;
			File << "END_BAGGAGESET" << std::endl;
			File << "\n" << std::endl;
		}

		// �t�@�C�������
		File << "END_SCRIPT\t\t# ���̍s�͐�Ώ����Ȃ����ƁI" << std::endl;
		File.close();
	}
}

//==========================================================================
// ���[�h
//==========================================================================
void CBaggageManager::Load()
{
	// �t�@�C�����J��
	std::ifstream File(TEXT);
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

			if (line.find("BAGGAGESET") != std::string::npos)
			{// MODELSET�Ŕz�u���ǂݍ���

				// �ǂݍ��ݏ��
				CBaggage::SBaggageInfo info;

				while (line.find("END_BAGGAGESET") == std::string::npos)
				{
					std::getline(File, line);
					if (line.find("MODEL") != std::string::npos)
					{// TYPE�Ŕz�u���̎��

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							info.path;	// �p�X
						info.path = UtilFunc::Transformation::ReplaceBackslash(info.path);
					}
					else if (line.find("WEIGHT") != std::string::npos)
					{// POS�ňʒu

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>			// ��
							info.weight;	// �d��
					}
					else if (line.find("WIDTH") != std::string::npos)
					{// ROT�Ō���

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							info.width;	// �Ԃꕝ
					}
					else if (line.find("CYCLE") != std::string::npos)
					{// ROT�Ō���

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							info.cycle;	// ����
					}
					else if (line.find("LIFE") != std::string::npos)
					{// LIFE�ő̗�

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							info.life;	// ����
					}
				}

				// ��荞��
				m_BaggageInfo.push_back(info);
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
