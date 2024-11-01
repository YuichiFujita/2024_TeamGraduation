//=============================================================================
// 
//  X�t�@�C������ [Xload.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "Xload.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "calculation.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CXLoad::SXFile CXLoad::m_pXFile[mylib_const::MAX_OBJ] = {};	// X�t�@�C���̏��
int CXLoad::m_nNumAll = 0;				// �ǂݍ��񂾑���

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CXLoad::CXLoad()
{
	for (int nCntData = 0; nCntData < mylib_const::MAX_OBJ; nCntData++)
	{
		m_pXFile[nCntData].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���f���̍ŏ��l
		m_pXFile[nCntData].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���f���̍ő�l
		m_pXFile[nCntData].pVtxBuff = NULL;								// ���_�o�b�t�@�̃|�C���^
		m_pXFile[nCntData].pMesh = NULL;								// ���b�V��(���_���)�ւ̃|�C���^
		m_pXFile[nCntData].pBuffMat = NULL;								// �}�e���A���ւ̃|�C���^
		m_pXFile[nCntData].dwNumMat = 0;								// �}�e���A���̐�
		m_pXFile[nCntData].nFileNameLen = 0;							// �t�@�C�����̕�����
		m_pXFile[nCntData].nIdxTexture = 0;								// �e�N�X�`���̃C���f�b�N�X�ԍ�
		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			// �}�e���A���̃f�[�^
			ZeroMemory(&m_pXFile[nCntData].pMatData[nCntMat], sizeof(D3DXMATERIAL));
		}

		for (int nCntFile = 0; nCntFile < mylib_const::MAX_STRING; nCntFile++)
		{
			m_pXFile[nCntData].acFilename[nCntFile] = NULL;	// �t�@�C����
		}
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CXLoad::~CXLoad()
{

}

//==========================================================================
// �I������
//==========================================================================
void CXLoad::Uninit(void)
{
	for (int nCntData = 0; nCntData < mylib_const::MAX_OBJ; nCntData++)
	{
		if (m_pXFile[nCntData].nIdxTexture != NULL)
		{// NULL����Ȃ�������

			// �J������
			delete[] m_pXFile[nCntData].nIdxTexture;
			m_pXFile[nCntData].nIdxTexture = NULL;
		}

		// ���b�V���̔j��
		if (m_pXFile[nCntData].pMesh != NULL)
		{
			m_pXFile[nCntData].pMesh->Release();
			m_pXFile[nCntData].pMesh = NULL;
		}

		// �}�e���A���̔j��
		if (m_pXFile[nCntData].pBuffMat != NULL)
		{
			m_pXFile[nCntData].pBuffMat->Release();
			m_pXFile[nCntData].pBuffMat = NULL;
		}
	}
}

//==========================================================================
// X�t�@�C���̔j��
//==========================================================================
void CXLoad::Unload(void)
{

}


//==========================================================================
// X�t�@�C���̓ǂݍ���
//==========================================================================
int CXLoad::XLoad(const char *pFileName)
{
	// �ő吔�擾
	int nIdx = 0;
	int nNumAll = GetNumAll() + 1;

	for (int nCntData = 0; nCntData < nNumAll; nCntData++)
	{
		if (pFileName != NULL)
		{// NULL����Ȃ�������

			if (m_pXFile[nCntData].nFileNameLen == strlen(pFileName))
			{// �t�@�C�����̒�����������������

				// ���Ƀe�N�X�`�����ǂݍ��܂�ĂȂ����̍ŏI�m�F
				if (strcmp(m_pXFile[nCntData].acFilename, pFileName) == 0)
				{// �t�@�C��������v���Ă���

					// �C���f�b�N�X�ԍ��ۑ�
					nIdx = nCntData;
					return nIdx;
				}
			}
		}
	}

	if (pFileName != NULL)
	{// NULL����Ȃ�������

		// �ǂݍ���
		HRESULT hr = Load(pFileName);

		if (FAILED(hr))
		{// ���s���Ă�����
			return E_FAIL;
		}

		// �C���f�b�N�X�ԍ��ۑ�
		nIdx = nNumAll - 1;
		return nIdx;
	}

	return 0;
}

//==========================================================================
// ���[�h����
//==========================================================================
HRESULT CXLoad::Load(const char *pFileName)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	int nIdx = m_nNumAll;

	//X�t�@�C���̓ǂݍ���
	HRESULT hr = D3DXLoadMeshFromX(pFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pXFile[nIdx].pBuffMat,
		NULL,
		&m_pXFile[nIdx].dwNumMat,
		&m_pXFile[nIdx].pMesh);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// �t�@�C�����ƒ����ۑ�
	strcpy(m_pXFile[nIdx].acFilename, pFileName);
	m_pXFile[nIdx].nFileNameLen = strlen(&m_pXFile[nIdx].acFilename[0]);

	if (m_pXFile[nIdx].nIdxTexture == NULL)
	{
		// �e�N�X�`���̃C���f�b�N�X�ԍ�
		m_pXFile[nIdx].nIdxTexture = DEBUG_NEW int[(int)m_pXFile[nIdx].dwNumMat];
	}

	if (m_pXFile[nIdx].nIdxTexture == NULL)
	{// �m�ۂł��Ă��Ȃ��ꍇ
		return E_FAIL;
	}

	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pXFile[nIdx].pBuffMat->GetBufferPointer();

	// ���_�����J��Ԃ�
	for (int nCntMat = 0; nCntMat < (int)m_pXFile[nIdx].dwNumMat; nCntMat++)
	{
		m_pXFile[nIdx].nIdxTexture[nCntMat] = 0;

		if (pMat[nCntMat].pTextureFilename != NULL)
		{// �t�@�C�������݂���

			// �e�N�X�`���̓ǂݍ���
			m_pXFile[nIdx].nIdxTexture[nCntMat] = CManager::GetTexture()->Regist(pMat[nCntMat].pTextureFilename);

			if (FAILED(hr))
			{// ���s���Ă�����
				return E_FAIL;
			}
		}
	}

	// �e�N�X�`�����蓖��
	//pCObjX->BindTexture(m_pXFile[nIdx].nIdxTexture);

	// �������Z
	m_nNumAll++;

	return S_OK;
}

//==========================================================================
// X�t�@�C���̃f�[�^�擾
//==========================================================================
CXLoad::SXFile *CXLoad::GetObjectX(int nIdx)
{
	return &m_pXFile[nIdx];
}

//==========================================================================
// X�t�@�C���̑����擾
//==========================================================================
int CXLoad::GetNumAll(void)
{
	return m_nNumAll;
}