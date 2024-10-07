//=============================================================================
// 
//  �}�b�v���� [map.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "map.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "calculation.h"
#include "objectX.h"
#include "object3DMesh.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "shadow.h"

using namespace std;	// ���O��Ԃ��w��

namespace mapdate
{
	// �\���̒�`
	struct SMap
	{
		int nType;						// ���
		MyLib::Vector3 pos;				// �ʒu
		MyLib::Vector3 rot;				// ����
		int nWidth, nHeight;			// ������
		float fWidthLen, fHeightLen;	// �T�C�Y
		float fMove;					// �ړ���
		int nShadow;					// �e�g�����ǂ���
	};

	int nNumModelAll = 0;	// ���f���̑���
	int nNumTexAll = 0;		// �e�N�X�`���̑���
	int nNumObjXAll = 0;		// �I�u�W�F�N�gX�̑���
	int nNumObj3DMeshAll = 0;		// �I�u�W�F�N�g3D���b�V���̑���
	CObjectX *pObjX[mylib_const::MAX_OBJ];		// �I�u�W�F�N�gX
	CObject3DMesh *pObj3DMesh[mylib_const::MAX_OBJ];		// �I�u�W�F�N�g3D���b�V��
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
vector<std::string> ModelFile;		// ���f���t�@�C����
vector<std::string> TextureFile;	// �e�N�X�`���t�@�C����
vector<int> ModelIdx;	// ���f���C���f�b�N�X
vector<int> TextureIdx;	// �e�N�X�`���C���f�b�N�X
mapdate::SMap g_Map;

//==========================================================================
// ��������
//==========================================================================
HRESULT MyMap::Create(const char *pTextFile)
{
	// ����
	mapdate::nNumModelAll = 0;
	mapdate::nNumObjXAll = 0;
	mapdate::nNumObj3DMeshAll = 0;

	memset(mapdate::pObjX, 0, sizeof(mapdate::pObjX));
	memset(mapdate::pObj3DMesh, 0, sizeof(mapdate::pObj3DMesh));	// �I�u�W�F�N�g3D���b�V��
	ModelIdx.clear();
	ModelFile.clear();		// ���f���t�@�C����

	// �O���e�L�X�g�ǂݍ��ݏ���
	HRESULT hr = ReadText(pTextFile);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �������
//==========================================================================
void MyMap::Release()
{
	for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
	{
		if (mapdate::pObjX[nCntObj] != nullptr)
		{
			mapdate::pObjX[nCntObj]->Kill();
			mapdate::pObjX[nCntObj] = nullptr;
		}

		if (mapdate::pObj3DMesh[nCntObj] != nullptr)
		{
			mapdate::pObj3DMesh[nCntObj]->Uninit();
			mapdate::pObj3DMesh[nCntObj] = nullptr;
		}
	}

	ModelFile.clear();		// ���f���t�@�C����
	ModelIdx.clear();
	mapdate::nNumObjXAll = 0;
}

//==========================================================================
// �}�b�v�؂�ւ�
//==========================================================================
void MyMap::ChangeMap(const char* pTextFile)
{
	// �J������
	Release();

	// �O���e�L�X�g�ǂݍ��ݏ���
	ReadText(pTextFile);
}

//==========================================================================
// �O���t�@�C���������ݏ���
//==========================================================================
HRESULT MyMap::SaveText()
{
#if TOPCUR
	FILE *pFile = nullptr;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen("data\\TEXT\\map\\save_info.txt", "w");

	if (pFile == nullptr)
	{// �t�@�C�����J���Ȃ������ꍇ
		return E_FAIL;
	}

	// �t�@�C���ɏ����o��
	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# �e�N�X�`����\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_TEXTURE = %d\n\n", mapdate::nNumTexAll);

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# �e�N�X�`���t�@�C����\n"
		"#------------------------------------------------------------------------------\n");

	// �e�N�X�`���t�@�C������
	for (int nCntFileNum = 0; nCntFileNum < mapdate::nNumTexAll; nCntFileNum++)
	{
		fprintf(pFile, "TEXTURE_FILENAME = %s\t\t# [%d]\n", &TextureFile[nCntFileNum][0], nCntFileNum);
	}

	// �t�@�C���ɏ����o��
	fprintf(pFile,
		"\n"
		"#------------------------------------------------------------------------------\n"
		"# ���f����\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_MODEL = %d\n\n", mapdate::nNumModelAll);

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# ���f���t�@�C����\n"
		"#------------------------------------------------------------------------------\n");

	// ���f���t�@�C������
	for (int nCntFileNum = 0; nCntFileNum < mapdate::nNumModelAll; nCntFileNum++)
	{
		fprintf(pFile, "MODEL_FILENAME = %s\t\t# [%d]\n", &ModelFile[nCntFileNum][0], nCntFileNum);
	}

	// �e�N�X�`���擾
	CTexture* pTexture = CTexture::GetInstance();

	//**********************************
	// ��z�u
	//**********************************
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# ����\n"
		"#==============================================================================\n");


	std::map<CObject::LAYER, std::map<int, std::vector<CObject*>>> objects = CObject::GetTop();
	for (const auto& layer : objects)
	{
		for (const auto& priority : layer.second)
		{
			for (const auto& obj : priority.second)
			{
				// ��ނ̎擾
				CObject::TYPE TargetType = obj->GetType();

				if (TargetType == CObject::TYPE_MESHDOME)
				{// ���b�V���h�[����������

					// X�t�@�C���̏��擾
					CObject3DMesh* pObjMesh = obj->GetObject3DMesh();

					// �e�N�X�`���̃C���f�b�N�X�ԍ�
					int nType = pObjMesh->GetIdxTex();	// ���

					// �e�N�X�`����
					vector<string>::iterator itr = find(TextureFile.begin(), TextureFile.end(), pTexture->GetTextureInfo(nType).filename);
					nType = static_cast<int>(std::distance(TextureFile.begin(), itr));

					// �o��
					fprintf(pFile,
						"SKYSET\n"
						"\tTEXTYPE = %d\n"
						"\tMOVE = %f\n"
						"END_SKYSET\n\n",
						nType, g_Map.fMove);
				}
			}
		}
	}

	
	//**********************************
	// �R�z�u
	//**********************************
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# �R���\n"
		"#==============================================================================\n");

	for (const auto& layer : objects)
	{
		for (const auto& priority : layer.second)
		{
			for (const auto& obj : priority.second)
			{
				// ��ނ̎擾
				CObject::TYPE TargetType = obj->GetType();

				if (TargetType == CObject::TYPE_MESHCYLINDER)
				{// ���b�V���V�����_�[��������

					// X�t�@�C���̏��擾
					CObject3DMesh* pObjMesh = obj->GetObject3DMesh();

					// �e�N�X�`���̃C���f�b�N�X�ԍ�
					int nType = pObjMesh->GetIdxTex();	// ���

					// �e�N�X�`����
					vector<string>::iterator itr = find(TextureFile.begin(), TextureFile.end(), pTexture->GetTextureInfo(nType).filename);
					nType = static_cast<int>(std::distance(TextureFile.begin(), itr));

					// �o��
					fprintf(pFile,
						"MOUNTAINSET\n"
						"\tTEXTYPE = %d\n"
						"END_MOUNTAINSET\n\n", nType);
				}
			}
		}
	}


	//**********************************
	// �n�ʔz�u
	//**********************************
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# �n�ʏ��\n"
		"#==============================================================================\n");


	for (const auto& layer : objects)
	{
		for (const auto& priority : layer.second)
		{
			for (const auto& obj : priority.second)
			{
				// ��ނ̎擾
				CObject::TYPE TargetType = obj->GetType();

				if (TargetType == CObject::TYPE_MESHFIELD)
				{// ���b�V���t�B�[���h��������

					// X�t�@�C���̏��擾
					CObject3DMesh* pObjMesh = obj->GetObject3DMesh();

					// �e�N�X�`���̃C���f�b�N�X�ԍ�
					int nType = pObjMesh->GetIdxTex();	// ���

					// �e�N�X�`����
					vector<string>::iterator itr = find(TextureFile.begin(), TextureFile.end(), pTexture->GetTextureInfo(nType).filename);
					nType = static_cast<int>(std::distance(TextureFile.begin(), itr));

					MyLib::Vector3 pos = pObjMesh->GetPosition();		// �ʒu
					MyLib::Vector3 rot = pObjMesh->GetRotation();		// ����
					int nWidth = pObjMesh->GetWidthBlock();			// ��������
					int nHeight = pObjMesh->GetHeightBlock();		// �c������
					float fWidthLen = pObjMesh->GetWidthLen();		// ������
					float fHeightLen = pObjMesh->GetHeightLen();	// �c����

					// �o��
					fprintf(pFile,
						"FIELDSET\n"
						"\tTEXTYPE = %d\n"
						"\tPOS = %.0f %.0f %.0f\n"
						"\tROT = %.0f %.0f %.0f\n"
						"\tBLOCK = %d %d\n"
						"\tSIZE = %.0f %.0f\n"
						"END_FIELDSET\n\n",
						nType, pos.x, pos.y, pos.z,
						rot.x, rot.y, rot.z,
						nWidth, nHeight,
						fWidthLen, fHeightLen);
				}
			}
		}
	}
	

	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# ���f���̔z�u\n"
		"#==============================================================================\n");

	int nnnn = 0;
	for (int i = 0; i < mylib_const::MAX_OBJ; i++)
	{
		// X�t�@�C���̏��擾
		CObjectX* pObjX = mapdate::pObjX[i];

		if (pObjX == nullptr) {
			continue;
		}

		int nType = ModelIdx[nnnn];		// ���
		nnnn++;
		MyLib::Vector3 pos = pObjX->GetPosition();	// �ʒu
		MyLib::Vector3 rot = pObjX->GetRotation();	// ����
		int nShadow = 0;						// �e�g�����ǂ���

		if (pObjX->GetUseShadow())
		{// �g���Ă���ꍇ
			nShadow = 1;
		}

		// �o��
		fprintf(pFile,
			"MODELSET\n"
			"\tTYPE = %d\n"
			"\tPOS = %.2f %.2f %.2f\n"
			"\tROT = %.2f %.2f %.2f\n"
			"\tSHADOW = %d\n"
			"END_MODELSET\n\n",
			nType, pos.x, pos.y, pos.z,
			rot.x, rot.y, rot.z, nShadow);
	}

	

	fprintf(pFile, "\nEND_SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI");

	//�t�@�C�������
	fclose(pFile);
#endif
	return S_OK;
}

//==========================================================================
// �e�N�X�`���ǂݍ��ݏ���
//==========================================================================
HRESULT MyMap::ReadTexture()
{
	char aComment[MAX_COMMENT] = {};	// �R�����g�p
	int nFileNum = 0;					// �t�@�C���̐�
	int nCntTexture = 0;				// �e�N�X�`���ǂݍ��݃J�E���g

	// �t�@�C���|�C���^
	FILE *pFile = nullptr;

	//�t�@�C�����J��
	pFile = fopen("data\\TEXT\\map\\info.txt", "r");

	// ��ł���
	/*switch (CManager::GetInstance()->GetMode())
	{
	case CScene::MODE_TITLE:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_TUTORIAL:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_GAME:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_RESULT:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_RANKING:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	default:
		return E_FAIL;
		break;
	}*/

	if (pFile == nullptr)
	{//�t�@�C�����J�����ꍇ
		return E_FAIL;
	}

	TextureIdx.clear();

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// �e�N�X�`�����̐ݒ�
		if (strcmp(&aComment[0], "NUM_TEXTURE") == 0)
		{// NUM_MODEL��������

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�
			fscanf(pFile, "%d", &mapdate::nNumTexAll);	// �e�N�X�`����
		}

		while (nCntTexture != mapdate::nNumTexAll)
		{// �e�N�X�`���̐����ǂݍ��ނ܂ŌJ��Ԃ�

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�

			if (strcmp(&aComment[0], "TEXTURE_FILENAME") == 0)
			{// TEXTURE_FILENAME��������

				fscanf(pFile, "%s", &aComment[0]);	// =�̕�
				fscanf(pFile, "%s", &aComment[0]);	// �t�@�C����

				// �Ō���ɒǉ�
				TextureFile.emplace_back();
				TextureFile.back() = &aComment[0];

				TextureFile.back() = UtilFunc::Transformation::ReplaceBackslash(TextureFile.back());
				TextureFile.back() = UtilFunc::Transformation::ReplaceForwardSlashes(TextureFile.back());

				// �e�N�X�`���̊��蓖��
				CTexture::GetInstance()->Regist(&TextureFile[nCntTexture][0]);

				// �e�N�X�`�����ǉ�
				TextureIdx.emplace_back();
				TextureIdx.back()= nCntTexture;

				nCntTexture++;	// �e�N�X�`�������Z
			}
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���

			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);

	return S_OK;
}

//==========================================================================
// ���f���ǂݍ��ݏ���
//==========================================================================
HRESULT MyMap::ReadXFile(const char *pTextFile)
{
	// ���Z�b�g
	mapdate::nNumModelAll = 0;


	char aComment[MAX_COMMENT] = {};	// �R�����g�p
	int nFileNum = 0;					// �t�@�C���̐�
	int nCntTexture = 0;				// �e�N�X�`���ǂݍ��݃J�E���g

	// �t�@�C���|�C���^
	FILE *pFile = nullptr;

	//�t�@�C�����J��
	pFile = fopen(pTextFile, "r");

	if (pFile == nullptr)
	{//�t�@�C�����J�����ꍇ
		return E_FAIL;
	}

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// ���f�����̐ݒ�
		if (strcmp(&aComment[0], "NUM_MODEL") == 0)
		{// NUM_MODEL��������

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�
			fscanf(pFile, "%d", &nFileNum);	// ���f����
		}

		while (mapdate::nNumModelAll != nFileNum)
		{// ���f���̐����ǂݍ��ނ܂ŌJ��Ԃ�

			// ������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			// ���f�����̐ݒ�
			if (strcmp(&aComment[0], "MODEL_FILENAME") == 0)
			{// NUM_MODEL��������

				fscanf(pFile, "%s", &aComment[0]);	// =�̕�
				fscanf(pFile, "%s", &aComment[0]);	// �t�@�C����

				// �Ō���ɒǉ�
				ModelFile.push_back(&aComment[0]);

				// X�t�@�C���̃f�[�^�擾
				CXLoad *pXLoad = CXLoad::GetInstance();

				// �t�@�C���ǂݍ���
				pXLoad->XLoad(&ModelFile[mapdate::nNumModelAll][0]);

				mapdate::nNumModelAll++;	// ���f�������Z
			}
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���

			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);

	return S_OK;
}

//==========================================================================
// �O���e�L�X�g�ǂݍ��ݏ���
//==========================================================================
HRESULT MyMap::ReadText(const char *pTextFile)
{
	std::string FileName;	// �t�@�C����
	char aComment[MAX_COMMENT] = {};	//�R�����g�p
	int nFileNum = 0;					// �t�@�C���̐�
	int nCntTexture = 0;				// �e�N�X�`���ǂݍ��݃J�E���g

	// �t�@�C���|�C���^
	FILE *pFile = nullptr;

	//�t�@�C�����J��
	switch (CManager::GetInstance()->GetMode())
	{
	case CScene::MODE_TITLE:
		FileName = "data\\TEXT\\map\\info_title.txt";
		break;

	case CScene::MODE_TUTORIAL:
		FileName = "data\\TEXT\\map\\info_title.txt";
		break;

	case CScene::MODE_GAME:
	case CScene::MODE::MODE_GAMETUTORIAL:
		FileName = pTextFile;
		break;

	case CScene::MODE_RESULT:
		FileName = "data\\TEXT\\map\\info_title.txt";
		break;

		//�����F���C���A�{����info_ranking.txt���Ɨ�����
	case CScene::MODE_RANKING:
		FileName = "data\\TEXT\\map\\info_title.txt";
		break;

	default:
		FileName = "data\\TEXT\\map\\info_title.txt";
		break;
	}

	// �}�b�v�t�@�C�����J��
	pFile = fopen(FileName.c_str(), "r");

	if (pFile == nullptr)
	{//�t�@�C�����J�����ꍇ
		return E_FAIL;
	}

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

#if 1
		// ���b�V���t�B�[���h�̐ݒ�
		if (strcmp(&aComment[0], "FIELDSET") == 0)
		{// ���f���̓ǂݍ��݂��J�n

			while (strcmp(&aComment[0], "END_FIELDSET"))
			{// END_FIELDSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	// �m�F����

				if (strcmp(&aComment[0], "TEXTYPE") == 0)
				{// TEXTYPE���������ޓǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &g_Map.nType);		// ���f����ނ̗�
				}

				if (strcmp(&aComment[0], "POS") == 0)
				{// POS��������ʒu�ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &g_Map.pos.x);		// X���W
					fscanf(pFile, "%f", &g_Map.pos.y);		// Y���W
					fscanf(pFile, "%f", &g_Map.pos.z);		// Z���W
				}

				if (strcmp(&aComment[0], "ROT") == 0)
				{// ROT������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);		 // =�̕�
					fscanf(pFile, "%f", &g_Map.rot.x);		 // X�̌���
					fscanf(pFile, "%f", &g_Map.rot.y);		 // Y�̌���
					fscanf(pFile, "%f", &g_Map.rot.z);		 // Z�̌���
					g_Map.rot.x = D3DXToRadian(g_Map.rot.x); // 360�x�`������ϊ�
					g_Map.rot.y = D3DXToRadian(g_Map.rot.y); // 360�x�`������ϊ�
					g_Map.rot.z = D3DXToRadian(g_Map.rot.z); // 360�x�`������ϊ�
				}

				if (strcmp(&aComment[0], "BLOCK") == 0)
				{//BLOCK������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);		// =�̕�
					fscanf(pFile, "%d", &g_Map.nWidth);		// ���̕�����
					fscanf(pFile, "%d", &g_Map.nHeight);	// �c�̕�����
				}

				if (strcmp(&aComment[0], "SIZE") == 0)
				{//SIZE������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);		// =�̕�
					fscanf(pFile, "%f", &g_Map.fWidthLen);	// ���̒���
					fscanf(pFile, "%f", &g_Map.fHeightLen);	// �c�̒���
				}

			}// END_FIELDSET�̂�����

			 //**********************************
			 // ��������
			 //**********************************
			mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] = CMeshField::Create(
				g_Map.pos, g_Map.rot,
				g_Map.fWidthLen, g_Map.fHeightLen,
				g_Map.nWidth, g_Map.nHeight,
				(CMeshField::TYPE)g_Map.nType, &TextureFile[g_Map.nType][0]);

			if (mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] == nullptr)
			{// nullptr��������
				return E_FAIL;
			}

			// �I�u�W�F�N�g���b�V���̑������Z
			mapdate::nNumObj3DMeshAll++;
		}

#endif
		// ���b�V���V�����_�[�̐ݒ�
		if (strcmp(&aComment[0], "MOUNTAINSET") == 0)
		{// ���f���̓ǂݍ��݂��J�n

			while (strcmp(&aComment[0], "END_MOUNTAINSET"))
			{// END_MOUNTAINSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	// �m�F����

				if (strcmp(&aComment[0], "TEXTYPE") == 0)
				{// TEXTYPE���������ޓǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &g_Map.nType);	// ���f����ނ̗�
				}

			}// END_MOUNTAINSET�̂�����

			 //**********************************
			 // ��������
			 //**********************************
			mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] = CMeshCylinder::Create(&TextureFile[g_Map.nType][0]);

			if (mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] == nullptr)
			{// nullptr��������
				return E_FAIL;
			}

			// �I�u�W�F�N�g���b�V���̑������Z
			mapdate::nNumObj3DMeshAll++;
		}

		// ���b�V���h�[���̐ݒ�
		if (strcmp(&aComment[0], "SKYSET") == 0)
		{// ���f���̓ǂݍ��݂��J�n

			while (strcmp(&aComment[0], "END_SKYSET"))
			{// END_MOUNTAINSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	// �m�F����

				if (strcmp(&aComment[0], "TEXTYPE") == 0)
				{// TEXTYPE���������ޓǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &g_Map.nType);		// ���f����ނ̗�
				}

				if (strcmp(&aComment[0], "MOVE") == 0)
				{//MOVE��������ړ��ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &g_Map.fMove);	// �ړ���
				}

			}// END_SKYSET�̂�����

			//**********************************
			// ��������
			//**********************************
			mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] = CMeshDome::Create(g_Map.fMove, &TextureFile[g_Map.nType][0]);

			if (mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] == nullptr)
			{// nullptr��������
				return E_FAIL;
			}

			// �I�u�W�F�N�g���b�V���̑������Z
			mapdate::nNumObj3DMeshAll++;
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���

			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);

	return S_OK;
}

void MyMap::Regist(int nType, MyLib::Vector3 pos, MyLib::Vector3 rot, bool bShadow)
{
	ModelIdx.push_back(nType);

	// �^�C�v�̕��𐶐�
	mapdate::pObjX[mapdate::nNumObjXAll] = CObjectX::Create(ModelFile[nType].c_str(), pos, rot, bShadow);
	mapdate::pObjX[mapdate::nNumObjXAll]->SetType(CObject::TYPE_XFILE);
	mapdate::nNumObjXAll++;
}

void MyMap::Delete(CObjectX* obj)
{
	for (int i = 0; i < mylib_const::MAX_OBJ; i++)
	{
		if (mapdate::pObjX[i] == obj)
		{
			mapdate::pObjX[i]->Kill();
			mapdate::pObjX[i] = nullptr;

			mapdate::nNumObjXAll--;
			break;
		}
		
	}
}

std::string MyMap::GetModelFileName(int nIdx)
{
	return ModelFile[nIdx];
}

std::vector<std::string> MyMap::GetModelFileNameAll()
{
	return ModelFile;
}

std::vector<std::string> MyMap::GetTextureFileNameAll()
{
	return TextureFile;
}

//==========================================================================
// �����擾
//==========================================================================
int MyMap::GetNumModelAll()
{
	return mapdate::nNumModelAll;
}
