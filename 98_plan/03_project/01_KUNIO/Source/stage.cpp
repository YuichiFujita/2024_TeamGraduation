//=============================================================================
// 
//  �X�e�[�W���� [stage.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "stage.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "objectX.h"
#include "shadow.h"
#include "input.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CStage::CStage()
{
	// �N���A
	m_nNumAll = 0;
	ModelFile.clear();		// ���f���t�@�C����
	memset(&m_pObjX[0], 0, sizeof(m_pObjX));	// �I�u�W�F�N�gX�̃|�C���^
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CStage::~CStage()
{

}

//==========================================================================
// ��������
//==========================================================================
CStage *CStage::Create(const char *pTextFile)
{
	// �����p�̃I�u�W�F�N�g
	CStage *pObjectX = nullptr;

	if (pObjectX == nullptr)
	{// nullptr��������

		// �������̊m��
		pObjectX = DEBUG_NEW CStage;

		if (pObjectX != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			HRESULT hr = pObjectX->Init();
			if (FAILED(hr))
			{// ���s���Ă�����
				return nullptr;
			}

			// �O���e�L�X�g�ǂݍ��ݏ���
			hr = pObjectX->ReadText(pTextFile);
			if (FAILED(hr))
			{// ���s���Ă�����
				return nullptr;
			}
		}

		return pObjectX;
	}

	return nullptr;
}


//==========================================================================
// ����������
//==========================================================================
HRESULT CStage::Init()
{
	// ����
	m_nNumAll = 0;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CStage::Uninit()
{
	// �I������
	for (int nCntObj = 0; nCntObj < mylib_const::MAX_STAGE; nCntObj++)
	{
		if (m_pObjX[nCntObj] != nullptr)
		{// nullptr����Ȃ�������
			m_pObjX[nCntObj] = nullptr;
		}
	}

	// �������Z�b�g
	m_nNumAll = 0;
}

//==========================================================================
// �������
//==========================================================================
void CStage::Release()
{
	// �I������
	for (int nCntObj = 0; nCntObj < mylib_const::MAX_STAGE; nCntObj++)
	{
		if (m_pObjX[nCntObj] != nullptr)
		{// nullptr����Ȃ�������
			m_pObjX[nCntObj]->Uninit();
			m_pObjX[nCntObj] = nullptr;
		}
	}

	// �������Z�b�g
	m_nNumAll = 0;
}

//==========================================================================
// �X�V����
//==========================================================================
void CStage::Update()
{

}

//==========================================================================
// �`�揈��
//==========================================================================
void CStage::Draw()
{

}

//==========================================================================
// �X�e�[�W�؂�ւ�
//==========================================================================
void CStage::ChangeStage(const char* pTextFile)
{
	// �폜
	Release();

	// �O���e�L�X�g�ǂݍ��ݏ���
	ReadText(pTextFile);
}

//==========================================================================
// �O���t�@�C���������ݏ���
//==========================================================================
HRESULT CStage::SaveText()
{
	return S_OK;
}

//==========================================================================
// ���f���ǂݍ��ݏ���
//==========================================================================
HRESULT CStage::ReadXFile(const char *pTextFile)
{
	// ���Z�b�g
	int nNumFileAll = 0;

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

		while (nNumFileAll != nFileNum)
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
				pXLoad->XLoad(&ModelFile[m_nNumAll][0]);

				// �t�@�C����
				nNumFileAll++;
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
HRESULT CStage::ReadText(const char *pTextFile)
{

	// �t�@�C������ǂݍ���
	if (FAILED(ReadXFile(pTextFile)))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	char aComment[MAX_COMMENT] = {};	//�R�����g�p

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

		// ���f���̐ݒ�
		if (strcmp(&aComment[0], "MODELSET") == 0)
		{// ���f���̓ǂݍ��݂��J�n

			int nType = 0;
			int nShadow = 0;
			MyLib::Vector3 pos = mylib_const::DEFAULT_VECTOR3;
			MyLib::Vector3 rot = mylib_const::DEFAULT_VECTOR3;


			while (strcmp(&aComment[0], "END_MODELSET"))
			{// END_MODELSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	// �m�F����

				if (strcmp(&aComment[0], "TYPE") == 0)
				{// TYPE���������ޓǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &nType);		// ���f����ނ̗�
				}

				if (strcmp(&aComment[0], "POS") == 0)
				{// POS��������ʒu�ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &pos.x);		// X���W
					fscanf(pFile, "%f", &pos.y);		// Y���W
					fscanf(pFile, "%f", &pos.z);		// Z���W
				}

				if (strcmp(&aComment[0], "ROT") == 0)
				{// ROT������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &rot.x);		// X�̌���
					fscanf(pFile, "%f", &rot.y);		// Y�̌���
					fscanf(pFile, "%f", &rot.z);		// Z�̌���
				}

				if (strcmp(&aComment[0], "SHADOW") == 0)
				{// SHADOW������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &nShadow);		// �e���g�����ǂ���
				}

			}// END_MODELSET�̂�����

			bool bShadow = false;
			if (nShadow == 1)
			{// �e���g�p����ꍇ
				bShadow = true;

			}
			// �^�C�v�̕��𐶐�
			m_pObjX[m_nNumAll] = CObjectX::Create(ModelFile[nType].c_str(), pos, rot, bShadow);
			if (m_pObjX[m_nNumAll] == nullptr)
			{// ���s���Ă�����
				return E_FAIL;
			}

			// ��ސݒ�
			m_pObjX[m_nNumAll]->SetType(CObject::TYPE_STAGE);
			m_nNumAll++;	// ���f�������Z
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
// �I�u�W�F�N�g�擾
//==========================================================================
CObjectX *CStage::GetObj(int nIdx)
{
	return m_pObjX[nIdx];
}

//==========================================================================
// �����擾
//==========================================================================
int CStage::GetNumAll()
{
	return m_nNumAll;
}