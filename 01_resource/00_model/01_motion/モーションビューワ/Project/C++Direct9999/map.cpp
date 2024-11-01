//=============================================================================
// 
//  �}�b�v���� [MyMap.cpp]
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

namespace MyMapdate
{
	// �\���̒�`
	struct SMap
	{
		int nType;						// ���
		D3DXVECTOR3 pos;				// �ʒu
		D3DXVECTOR3 rot;				// ����
		int nWidth, nHeight;			// ������
		float fWidthLen, fHeightLen;	// �T�C�Y
		float fMove;					// �ړ���
		int nShadow;					// �e�g�����ǂ���
	};

	int nNumModelAll = 0;	// ���f���̑���
	int nNumTexAll = 0;		// �e�N�X�`���̑���
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
vector<std::string> ModelFile;		// ���f���t�@�C����
vector<std::string> TextureFile;	// �e�N�X�`���t�@�C����
MyMapdate::SMap g_Map;

//==========================================================================
// ��������
//==========================================================================
HRESULT MyMap::Create(void)
{
	// ����
	MyMapdate::nNumModelAll = 0;

	// �O���e�L�X�g�ǂݍ��ݏ���
	HRESULT hr = ReadText();

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �O���t�@�C���������ݏ���
//==========================================================================
HRESULT MyMap::SaveText(void)
{
	FILE *pFile = NULL;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen("data\\TEXT\\edit_info.txt", "w");

	if (pFile == NULL)
	{// �t�@�C�����J���Ȃ������ꍇ
		return E_FAIL;
	}

	// �t�@�C���ɏ����o��
	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# �e�N�X�`����\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_TEXTURE = %d\n\n", MyMapdate::nNumTexAll);

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# �e�N�X�`���t�@�C����\n"
		"#------------------------------------------------------------------------------\n");

	// �e�N�X�`���t�@�C������
	for (int nCntFileNum = 0; nCntFileNum < MyMapdate::nNumTexAll; nCntFileNum++)
	{
		fprintf(pFile, "TEXTURE_FILENAME = %s\t\t# [%d]\n", &TextureFile[nCntFileNum][0], nCntFileNum);
	}

	// �t�@�C���ɏ����o��
	fprintf(pFile,
		"\n"
		"#------------------------------------------------------------------------------\n"
		"# ���f����\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_MODEL = %d\n\n", MyMapdate::nNumModelAll);

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# ���f���t�@�C����\n"
		"#------------------------------------------------------------------------------\n");

	// ���f���t�@�C������
	for (int nCntFileNum = 0; nCntFileNum < MyMapdate::nNumModelAll; nCntFileNum++)
	{
		fprintf(pFile, "MODEL_FILENAME = %s\t\t# [%d]\n", &ModelFile[nCntFileNum][0], nCntFileNum);
	}


	//**********************************
	// ��z�u
	//**********************************
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# ����\n"
		"#==============================================================================\n");

	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
		{
			// �I�u�W�F�N�g�̏��擾
			CObject *pObj = CObject::GetObject(nCntPriority, nCntObj);

			if (pObj != NULL)
			{// ��񂪓����Ă�����

				// ��ނ̎擾
				CObject::TYPE TargetType = pObj->GetType();

				if (TargetType == CObject::TYPE_MESHDOME)
				{// ���b�V���h�[����������

					// X�t�@�C���̏��擾
					CObject3DMesh *pObjMesh = pObj->GetObject3DMesh();

					// �e�N�X�`���̃C���f�b�N�X�ԍ�
					int nType = pObjMesh->GetIdxTex();	// ���
					nType--;	// NULL�����炷

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

	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
		{
			// �I�u�W�F�N�g�̏��擾
			CObject *pObj = CObject::GetObject(nCntPriority, nCntObj);

			if (pObj != NULL)
			{// ��񂪓����Ă�����

				// ��ނ̎擾
				CObject::TYPE TargetType = pObj->GetType();

				if (TargetType == CObject::TYPE_MESHCYLINDER)
				{// ���b�V���V�����_�[��������

					// ���b�V���̏��擾
					CObject3DMesh *pObjMesh = pObj->GetObject3DMesh();

					// �e�N�X�`���̃C���f�b�N�X�ԍ�
					int nType = pObjMesh->GetIdxTex();	// ���
					nType--;	// NULL�����炷

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

	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
		{
			// �I�u�W�F�N�g�̏��擾
			CObject *pObj = CObject::GetObject(nCntPriority, nCntObj);

			if (pObj != NULL)
			{// ��񂪓����Ă�����

				// ��ނ̎擾
				CObject::TYPE TargetType = pObj->GetType();

				if (TargetType == CObject::TYPE_MESHFIELD)
				{// ���b�V���t�B�[���h��������

					// ���b�V���̏��擾
					CObject3DMesh *pObjMesh = pObj->GetObject3DMesh();

					// �e�N�X�`���̃C���f�b�N�X�ԍ�
					int nType = pObjMesh->GetIdxTex();	// ���
					nType--;	// NULL�����炷
					D3DXVECTOR3 pos = pObjMesh->GetPosition();		// �ʒu
					D3DXVECTOR3 rot = pObjMesh->GetRotation();		// ����
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

	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
		{
			// �I�u�W�F�N�g�̏��擾
			CObject *pObj = CObject::GetObject(nCntPriority, nCntObj);

			if (pObj != NULL)
			{// ��񂪓����Ă�����

				// ��ނ̎擾
				CObject::TYPE TargetType = pObj->GetType();

				if (TargetType == CObject::TYPE_XFILE)
				{// X�t�@�C���̃��f����������

					// X�t�@�C���̏��擾
					CObjectX *pObjX = pObj->GetObjectX();

					int nType = pObjX->GetIdxXFile();		// ���
					D3DXVECTOR3 pos = pObjX->GetPosition();	// �ʒu
					D3DXVECTOR3 rot = pObjX->GetRotation();	// ����
					int nShadow = 0;						// �e�g�����ǂ���

					if (pObjX->GetUseShadow() == true)
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
			}
		}
	}

	fprintf(pFile, "END_SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI");

	//�t�@�C�������
	fclose(pFile);

	return S_OK;
}

//==========================================================================
// �O���e�L�X�g�ǂݍ��ݏ���
//==========================================================================
HRESULT MyMap::ReadText(void)
{
	char aComment[MAX_COMMENT] = {};	//�R�����g�p
	int nFileNum = 0;					// �t�@�C���̐�
	int nCntTexture = 0;				// �e�N�X�`���ǂݍ��݃J�E���g

	// �I�u�W�F�N�gX�̃I�u�W�F�N�g
	CObjectX *pObjectX = NULL;

	// ���b�V���I�u�W�F�N�g�̃I�u�W�F�N�g
	CObject3DMesh *pObjMesh = NULL;

	//�t�@�C�����J��
	FILE *pFile = fopen("data\\TEXT\\map.txt", "r");

	if (pFile == NULL)
	{//�t�@�C�����J�����ꍇ
		return E_FAIL;
	}

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// �e�N�X�`�����̐ݒ�
		if (strcmp(&aComment[0], "NUM_TEXTURE") == 0)
		{// NUM_MODEL��������

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�
			fscanf(pFile, "%d", &MyMapdate::nNumTexAll);	// �e�N�X�`����
		}

		while (nCntTexture != MyMapdate::nNumTexAll)
		{// �e�N�X�`���̐����ǂݍ��ނ܂ŌJ��Ԃ�

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�

			if (strcmp(&aComment[0], "TEXTURE_FILENAME") == 0)
			{// TEXTURE_FILENAME��������

				fscanf(pFile, "%s", &aComment[0]);	// =�̕�
				fscanf(pFile, "%s", &aComment[0]);	// �t�@�C����

				// �Ō���ɒǉ�
				TextureFile.push_back(&aComment[0]);

				// �e�N�X�`���̊��蓖��
				CManager::GetTexture()->Regist(&TextureFile[nCntTexture][0]);

				nCntTexture++;	// �e�N�X�`�������Z
			}
		}
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
			pObjMesh = CMeshField::Create(
				g_Map.pos, g_Map.rot,
				g_Map.fWidthLen, g_Map.fHeightLen,
				g_Map.nWidth, g_Map.nHeight,
				(CMeshField::TYPE)g_Map.nType, &TextureFile[g_Map.nType][0]);

			if (pObjMesh == NULL)
			{// NULL��������
				return E_FAIL;
			}
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
			pObjMesh = CMeshCylinder::Create(&TextureFile[g_Map.nType][0]);

			if (pObjMesh == NULL)
			{// NULL��������
				return E_FAIL;
			}
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
			pObjMesh = CMeshDome::Create(g_Map.fMove, &TextureFile[g_Map.nType][0]);

			if (pObjMesh == NULL)
			{// NULL��������
				return E_FAIL;
			}
		}


		// ���f�����̐ݒ�
		if (strcmp(&aComment[0], "NUM_MODEL") == 0)
		{// NUM_MODEL��������

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�
			fscanf(pFile, "%d", &nFileNum);	// ���f����
		}

		while (MyMapdate::nNumModelAll != nFileNum)
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
				CXLoad *pXLoad = CManager::GetXLoad();

				// �t�@�C���ǂݍ���
				pXLoad->XLoad(&ModelFile[MyMapdate::nNumModelAll][0]);

				MyMapdate::nNumModelAll++;	// ���f�������Z
			}
		}

#if 0
		// ���f���̐ݒ�
		if (strcmp(&aComment[0], "MODELSET") == 0)
		{// ���f���̓ǂݍ��݂��J�n

			while (strcmp(&aComment[0], "END_MODELSET"))
			{// END_MODELSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	// �m�F����

				if (strcmp(&aComment[0], "TYPE") == 0)
				{// TYPE���������ޓǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &g_Map.nType);	// ���f����ނ̗�
				}

				if (strcmp(&aComment[0], "POS") == 0)
				{// POS��������ʒu�ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &g_Map.pos.x);	// X���W
					fscanf(pFile, "%f", &g_Map.pos.y);	// Y���W
					fscanf(pFile, "%f", &g_Map.pos.z);	// Z���W
				}

				if (strcmp(&aComment[0], "ROT") == 0)
				{// ROT������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &g_Map.rot.x);	// X�̌���
					fscanf(pFile, "%f", &g_Map.rot.y);	// Y�̌���
					fscanf(pFile, "%f", &g_Map.rot.z);	// Z�̌���
				}

				if (strcmp(&aComment[0], "SHADOW") == 0)
				{// SHADOW������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);		// =�̕�
					fscanf(pFile, "%d", &g_Map.nShadow);	// �e���g�����ǂ���
				}

			}// END_MODELSET�̂�����

			if (g_Map.nShadow == 1)
			{
				// �^�C�v�̕��𐶐�
				pObjectX = pObjectX->Create(&ModelFile[g_Map.nType][0], g_Map.pos, g_Map.rot, true);
			}
			else
			{
				// �^�C�v�̕��𐶐�
				pObjectX = pObjectX->Create(&ModelFile[g_Map.nType][0], g_Map.pos, g_Map.rot, false);
			}

			if (pObjectX == NULL)
			{// ���s���Ă�����
				return E_FAIL;
			}

			// ��ސݒ�
			pObjectX->SetType(CObject::TYPE_XFILE);
		}
#endif
		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���

			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);

	return S_OK;;
}

//==========================================================================
// �����擾
//==========================================================================
int MyMap::GetNumModelAll(void)
{
	return MyMapdate::nNumModelAll;
}