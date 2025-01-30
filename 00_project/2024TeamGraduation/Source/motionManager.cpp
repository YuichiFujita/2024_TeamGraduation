//=============================================================================
// 
//  ���[�V�����}�l�[�W���[���� [motionManager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "motionManager.h"
#include "calculation.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CMotionManager* CMotionManager::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMotionManager::CMotionManager()
{
	// �������N���A
	m_LoadInfo.clear();
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMotionManager::~CMotionManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CMotionManager* CMotionManager::Create()
{
	// ���݂���ꍇ�����̃C���X�^���X�Ԃ�
	if (m_pThisPtr != nullptr) return m_pThisPtr;

	// �C���X�^���X����
	m_pThisPtr = DEBUG_NEW CMotionManager;

	if (m_pThisPtr != nullptr)
	{
		// ����������
		m_pThisPtr->Init();
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMotionManager::Init()
{
	// �������N���A
	m_LoadInfo.clear();
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMotionManager::Uninit()
{
	// �������N���A
	m_LoadInfo.clear();

	// ���g�̃|�C���^�폜
	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CMotionManager::Reset()
{
	// �I��
	Uninit();

	// ����
	Create();
}

//==========================================================================
// �ǂݍ��ݏ���
//==========================================================================
CMotionManager::SLoadInfo CMotionManager::Load(const std::string& setupfile)
{
	//--------------------------
	// �v�f�`�F�b�N
	//--------------------------
	// �\���̂̒��̕������T��
	auto CheckFile = [&setupfile](const CMotionManager::SLoadInfo& info)
	{
		return info.sTextFile == setupfile;
	};
	std::vector<CMotionManager::SLoadInfo>::iterator itr = std::find_if(m_LoadInfo.begin(), m_LoadInfo.end(), CheckFile);

	if (itr != m_LoadInfo.end())
	{// �t�@�C��������v

		return (*itr);
	}

	//--------------------------
	// �t�@�C���ǂݍ���
	//--------------------------
	// �t�@�C�����J��
	std::ifstream File(setupfile);
	if (!File.is_open())
	{//�t�@�C�����J���Ȃ������ꍇ
		MyAssert::CustomAssert(false, "�t�@�C���J���ĂȂ���I");
		return SLoadInfo();
	}

	// �ǂݍ��ݗp�ϐ�
	std::vector<std::string> motionFile;	// ���[�V�����t�@�C��
	SLoadInfo loadInfo;

	// �t�@�C�����ۑ�
	loadInfo.sTextFile = setupfile;

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

		if (line.find("DEFAULT_ID") != std::string::npos)
		{// �f�t�H���g���[�V�����ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			lineStream >>	// �薼
				hoge >>		// �S�~
				hoge;		// ��

			// ���n��
			int value;
			while (lineStream >> value)
			{
				loadInfo.vecDefaultIdx.push_back(value);
			}
		}

		if (line.find("MOTION_FILENAME") != std::string::npos)
		{// ���[�V�����t�@�C�����ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>		// �S�~
				hoge >>		// ��
				hoge;		// ���[�V�����t�@�C����

			// �t�@�C�����ۑ�
			motionFile.push_back(hoge);
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// �t�@�C�������
	File.close();


	//--------------------------
	// ���[�V�����ǂݍ���
	//--------------------------
	// �v�f���J��Ԃ�
	for (int i = 0; i < static_cast<int>(motionFile.size()); i++)
	{
		// ���[�V�����ǂݍ���
		loadInfo.vecLoadData.push_back(LoadMotion(motionFile[i], i));
	}

	// �v�f�ǉ�
	m_LoadInfo.push_back(loadInfo);

	return loadInfo;
}

//==========================================================================
// ���[�V�����ǂݍ���
//==========================================================================
CMotionManager::Info CMotionManager::LoadMotion(const std::string& file, int nIdxMotion)
{
	// �t�@�C�����J��
	std::ifstream File(file);
	if (!File.is_open())
	{//�t�@�C�����J���Ȃ������ꍇ
		MyAssert::CustomAssert(false, "�t�@�C���J���ĂȂ���I");
		return Info();
	}

	// �ǂݍ��ݗp�f�[�^
	Info loadInfo;

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

		if (line.find("MOTIONSET") != std::string::npos)
		{// ���[�V�������̓ǂݍ��݂��J�n

			while (line.find("END_MOTIONSET") == std::string::npos)
			{// END_MOTIONSET������܂ŌJ��Ԃ�

				// �m�F����
				std::getline(File, line);

				if (line.find("LOOP") != std::string::npos)
				{// LOOP�������烋�[�vON/OFF�ǂݍ���

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// ���n��
					lineStream >>
						hoge >>			// �S�~
						hoge >>			// ��
						loadInfo.nLoop;	// ���[�v0��1
				}
				else if (line.find("NUM_KEY") != std::string::npos)
				{// NUM_KEY��������L�[���ǂݍ���

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// ���n��
					lineStream >>
						hoge >>				// �S�~
						hoge >>				// ��
						loadInfo.nNumKey;	// �L�[��
				}
				else if (line.find("MOVE") != std::string::npos)
				{// MOVE��������ړ�ON/OFF�ǂݍ���

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// ���n��
					lineStream >>
						hoge >>				// �S�~
						hoge >>				// ��
						loadInfo.nMove;		// �ړ�0��1��
				}
				else if (line.find("SPECIAL") != std::string::npos)
				{// SPECIAL��������ON/OFF�ǂݍ���

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// �X�y�V����
					int nSpecial;

					// ���n��
					lineStream >>
						hoge >>		// �S�~
						hoge >>		// ��
						nSpecial;	// �X�y�V��������

					// �X�y�V��������
					loadInfo.bSpecial = (nSpecial == 1);
				}
				else if (line.find("CANCELABLE") != std::string::npos)
				{// CANCELABLE�ŃL�����Z���\�t���[���ǂݍ���

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// ���n��
					lineStream >>
						hoge >>		// �S�~
						hoge >>		// ��
						loadInfo.nCancelableFrame;	// �L�����Z���\�t���[��
				}
				else if (line.find("COMBOLABLE") != std::string::npos)
				{// COMBOLABLE�ŃR���{�\�t���[���ǂݍ���

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// ���n��
					lineStream >>
						hoge >>		// �S�~
						hoge >>		// ��
						loadInfo.nCombolableFrame;	// �R���{�\�t���[��
				}


				//--------------------------
				// �U�����
				//--------------------------
				if (line.find("ATTACKINFO") != std::string::npos)
				{// ATTACKINFO��������

					// �ǂݍ��ݗp�ϐ�
					AttackInfo loadAttackInfo;

					while (line.find("END_ATTACKINFO") == std::string::npos)
					{// END_ATTACKINFO������܂ŌJ��Ԃ�

						// �m�F����
						std::getline(File, line);


						if (line.find("COLLISIONPARTS") != std::string::npos)
						{// COLLISIONPARTS��������p�[�c�ԍ��ǂݍ���

							// �X�g���[���쐬
							std::istringstream lineStream(line);

							// ���n��
							lineStream >>
								hoge >>			// �S�~
								hoge >>			// ��
								loadAttackInfo.nCollisionNum;	// ��������p�[�c�ԍ�
						}
						else if (line.find("ATTACKRANGE") != std::string::npos)
						{// ATTACKRANGE��������U���T�C�Y�ǂݍ���

							// �X�g���[���쐬
							std::istringstream lineStream(line);

							// ���n��
							lineStream >>
								hoge >>			// �S�~
								hoge >>			// ��
								loadAttackInfo.fRangeSize;	// ����̃T�C�Y
						}
						else if (line.find("OFFSET") != std::string::npos)
						{// OFFSET��������I�t�Z�b�g�ǂݍ���

							// �X�g���[���쐬
							std::istringstream lineStream(line);

							// ���n��
							lineStream >>
								hoge >>			// �S�~
								hoge >>			// ��
								loadAttackInfo.Offset.x >> loadAttackInfo.Offset.y >> loadAttackInfo.Offset.z;	// �I�t�Z�b�g
						}
						else if (line.find("ATTACKCOUNT") != std::string::npos)
						{// ATTACKCOUNT��������U���J�E���g�ǂݍ���

							// �X�g���[���쐬
							std::istringstream lineStream(line);

							// ���n��
							lineStream >>
								hoge >>			// �S�~
								hoge >>			// ��
								loadAttackInfo.nMinCnt >>
								loadAttackInfo.nMaxCnt;	// �Ռ��̃J�E���g
						}
						else if (line.find("INPACTCOUNT") != std::string::npos)
						{// INPACTCOUNT��������Ռ��J�E���g�ǂݍ���

							// �X�g���[���쐬
							std::istringstream lineStream(line);

							// ���n��
							lineStream >>
								hoge >>			// �S�~
								hoge >>			// ��
								loadAttackInfo.nInpactCnt;	// �Ռ��̃J�E���g
						}
						else if (line.find("DAMAGE") != std::string::npos)
						{// DAMAGE��������U���͓ǂݍ���

							// �X�g���[���쐬
							std::istringstream lineStream(line);

							// ���n��
							lineStream >>
								hoge >>			// �S�~
								hoge >>			// ��
								loadAttackInfo.nDamage;	// �U����
						}
						else if (line.find("ALIGNFRAME") != std::string::npos)
						{// ALIGNFRAME�������瑫����

							// �X�g���[���쐬
							std::istringstream lineStream(line);

							// ���n��
							lineStream >>
								hoge >>			// �S�~
								hoge >>			// ��
								loadAttackInfo.AlignInfo.nFrame >>			// �Ռ��̃t���[��
								loadAttackInfo.AlignInfo.nExtensionFrame;	// �P�\�t���[��
						}
					}

					// �U���̏��ǉ�
					loadInfo.AttackInfo.push_back(loadAttackInfo);
				}


				//--------------------------
				// �e�L�[�̐ݒ�
				//--------------------------
				if (line.find("KEYSET") != std::string::npos)
				{// KEYSET�ŃL�[���̓ǂݍ��݊J�n

					// �ǂݍ��ݗp�f�[�^
					Key loadKey;

					while (line.find("END_KEYSET") == std::string::npos)
					{// END_KEYSET������܂ŌJ��Ԃ�

						// �m�F����
						std::getline(File, line);


						if (line.find("FRAME") != std::string::npos)
						{// FRAME��������Đ��t���[�����ǂݍ���

							// �X�g���[���쐬
							std::istringstream lineStream(line);

							// ���n��
							lineStream >>
								hoge >>			// �S�~
								hoge >>			// ��
								loadKey.nFrame;	// �Đ��t���[��
						}


						//--------------------------
						// �p�[�c�̐ݒ�
						//--------------------------
						if (line.find("PARTS") != std::string::npos)
						{// PARTS�Ńp�[�c���̓ǂݍ��݊J�n

							// �ǂݍ��ݗp�f�[�^
							Parts loadParts;

							while (line.find("END_PARTS") == std::string::npos)
							{// END_PARTS������܂ŌJ��Ԃ�

								// �m�F����
								std::getline(File, line);

								if (line.find("POS") != std::string::npos)
								{// POS

									// �X�g���[���쐬
									std::istringstream lineStream(line);

									// ���n��
									lineStream >>
										hoge >>			// �S�~
										hoge >>			// ��
										loadParts.pos.x >> loadParts.pos.y >> loadParts.pos.z;	// �ʒu
								}
								else if (line.find("ROT") != std::string::npos)
								{// ROT

									// �X�g���[���쐬
									std::istringstream lineStream(line);

									// ���n��
									lineStream >>
										hoge >>			// �S�~
										hoge >>			// ��
										loadParts.rot.x >> loadParts.rot.y >> loadParts.rot.z;	// ����
								}
								else if (line.find("SCALE") != std::string::npos)
								{// SCALE

									// �X�g���[���쐬
									std::istringstream lineStream(line);

									// ���n��
									lineStream >>
										hoge >>			// �S�~
										hoge >>			// ��
										loadParts.scale.x >> loadParts.scale.y >> loadParts.scale.z;	// �X�P�[��
								}

							}// END_PARTS�̂�����

							// �p�[�c�f�[�^�ǉ�
							loadKey.aParts.push_back(loadParts);
						}

					}// END_KEYSET�̂�����

					// �L�[�f�[�^�ǉ�
					loadInfo.aKey.push_back(loadKey);
				}

			}// END_MOTIONSET�̂�����
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// �t�@�C�������
	File.close();


	//-----------------------------
	// �ǂݍ��݌�̌v�Z
	//-----------------------------
	// �L�[�̐�
	loadInfo.nNumKey = static_cast<int>(loadInfo.aKey.size());

	// �U�����̐�
	loadInfo.nNumAttackInfo = static_cast<int>(loadInfo.AttackInfo.size());

	for (int i = 0; i < loadInfo.nNumKey; i++)
	{
		// 1�O�̃L�[�C���f�b�N�X
		int nBeforeCnt = UtilFunc::Transformation::Clamp(i - 1, 0, loadInfo.nNumKey);

		// 1�O�̈ʒu
		float PosX = loadInfo.aKey[nBeforeCnt].aParts[0].pos.x;
		float PosZ = loadInfo.aKey[nBeforeCnt].aParts[0].pos.z;

		// ���������߂�
		loadInfo.aKey[i].fRotMove = atan2f(
			(PosX - loadInfo.aKey[i].aParts[0].pos.x),
			(PosZ - loadInfo.aKey[i].aParts[0].pos.z));
	}

	// �ǂݍ��݃f�[�^�n��
	return loadInfo;
}
