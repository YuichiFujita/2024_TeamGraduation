//=============================================================================
// 
//  �e�N�X�`������ [texture.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"

// �ǂݍ��ރe�N�X�`���̃w�b�_�[
#include "player.h"
#include "3D_effect.h"
#include "particle.h"
#include "meshfield.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CTexture::m_nNumAll = 0;	// ����

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTexture::CTexture()
{
	for (int nCntData = 0; nCntData < mylib_const::MAX_OBJ; nCntData++)
	{
		m_pXFile[nCntData].nFileNameLen = 0;	// �t�@�C�����̕�����
		m_pXFile[nCntData].pTexture = NULL;		// �e�N�X�`���̃|�C���^
		
		for (int nCntFile = 0; nCntFile < mylib_const::MAX_STRING; nCntFile++)
		{
			m_pXFile[nCntData].acFilename[nCntFile] = NULL;	// �t�@�C����
		}
	}
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTexture::~CTexture()
{

}

//==========================================================================
// �e�N�X�`���̓ǂݍ���
//==========================================================================
HRESULT CTexture::LoadAll(void)
{
	////**********************
	//// 2D�I�u�W�F�N�g
	////**********************
	//// �w�i
	//if (FAILED(CBG::Load()))
	//{// ���s���Ă�����
	//	return E_FAIL;
	//}

	//// �G
	//if (FAILED(CEnemy::Load()))
	//{// ���s���Ă�����
	//	return E_FAIL;
	//}

	//// �e
	//if (FAILED(CBullet::Load()))
	//{// ���s���Ă�����
	//	return E_FAIL;
	//}

	//// ����
	//if (FAILED(CExplosion::Load()))
	//{// ���s���Ă�����
	//	return E_FAIL;
	//}

	//// 2D�G�t�F�N�g
	//if (FAILED(CEffect2D::Load()))
	//{// ���s���Ă�����
	//	return E_FAIL;
	//}

	//// 3D�G�t�F�N�g
	//if (FAILED(CEffect3D::Load()))
	//{// ���s���Ă�����
	//	return E_FAIL;
	//}

	//// �X�R�A
	//if (FAILED(CScore::Load()))
	//{// ���s���Ă�����
	//	return E_FAIL;
	//}

	//// �^�C�}�[
	//if (FAILED(CTimer::Load()))
	//{// ���s���Ă�����
	//	return E_FAIL;
	//}

	//// �u���b�N
	//if (FAILED(CBlock::Load()))
	//{// ���s���Ă�����
	//	return E_FAIL;
	//}

	//// �A�C�e��
	//if (FAILED(CItem::Load()))
	//{// ���s���Ă�����
	//	return E_FAIL;
	//}

	//// �e�N�X�`���ǂݍ���
	//if (FAILED(CPlayer::Load()))
	//{// ���s���Ă�����
	//	return E_FAIL;
	//}


	////**********************
	//// 3D�I�u�W�F�N�g
	////**********************
	//// �t�B�[���h
	//if (FAILED(CField::Load()))
	//{// ���s���Ă�����
	//	return E_FAIL;
	//}

	//// ���b�V���t�B�[���h
	//if (FAILED(CMeshField::Load()))
	//{// ���s���Ă�����
	//	return E_FAIL;
	//}


	////**********************
	//// �r���{�[�h�I�u�W�F�N�g
	////**********************
	//// �̗̓Q�[�W
	//if (FAILED(CHP_Gauge::Load()))
	//{// ���s���Ă�����
	//	return E_FAIL;
	//}

	return S_OK;
}

//==========================================================================
// ����e�N�X�`���̓ǂݍ���
//==========================================================================
HRESULT CTexture::Load(void)
{
	return S_OK;
}

//==========================================================================
// �S�Ẵe�N�X�`���̔j��
//==========================================================================
void CTexture::Unload(void)
{
	for (int nCntData = 0; nCntData < mylib_const::MAX_OBJ; nCntData++)
	{
		// �e�N�X�`���̔j��
		if (m_pXFile[nCntData].pTexture != NULL)
		{
			m_pXFile[nCntData].pTexture->Release();
			m_pXFile[nCntData].pTexture = NULL;
		}
	}
}

//==========================================================================
// �e�N�X�`���̊��蓖�ď���
//==========================================================================
int CTexture::Regist(const char *pFileName)
{
	// �ő吔�擾
	int nNumAll = GetNumAll() + 1;
	int nIdx = -1;	// ����̔ԍ��ۑ�

	for (int nCntData = 0; nCntData < nNumAll; nCntData++)
	{
		if (pFileName != NULL)
		{// NULL����Ȃ�������

			if (m_pXFile[nCntData].nFileNameLen == strlen(pFileName))
			{// �t�@�C�����̒�����������������

				// ���Ƀe�N�X�`�����ǂݍ��܂�ĂȂ����̍ŏI�m�F
				if (strcmp(m_pXFile[nCntData].acFilename, pFileName) == 0)
				{// �t�@�C��������v���Ă���

					// �ԍ����蓖��
					nIdx = nCntData;

					return nIdx;
				}
			}
		}
	}

	if (pFileName != NULL)
	{// NULL����Ȃ�������

		// �e�N�X�`���ǂݍ���
		HRESULT hr = LoadTex(pFileName);

		if (FAILED(hr))
		{// ���s���Ă�����
			return 0;
		}

		// �ԍ����蓖��
		nIdx = nNumAll - 1;
		return nIdx;
	}

	return 0;
}

//==========================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//==========================================================================
HRESULT CTexture::LoadTex(const char *pFileName)
{
	HRESULT hr;
	int nIdx = m_nNumAll;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevive = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	hr = D3DXCreateTextureFromFile(pDevive,
		pFileName,
		&m_pXFile[nIdx].pTexture);

	if (hr == D3DXERR_INVALIDDATA)
	{// ���s�����Ƃ�

		return E_FAIL;
	}

	// �t�@�C�����ƒ����ۑ�
	strcpy(m_pXFile[nIdx].acFilename, pFileName);
	m_pXFile[nIdx].nFileNameLen = strlen(&m_pXFile[nIdx].acFilename[0]);

	// �������Z
	m_nNumAll++;

	return S_OK;
}

//==========================================================================
// �e�N�X�`���̃A�h���X�擾
//==========================================================================
LPDIRECT3DTEXTURE9 CTexture::GetAdress(int nIdx)
{
	return m_pXFile[nIdx].pTexture;
}

//==========================================================================
// �e�N�X�`���̑����擾
//==========================================================================
int CTexture::GetNumAll(void)
{
	return m_nNumAll;
}

//==========================================================================
// �e�N�X�`�����擾
//==========================================================================
CTexture::STexture CTexture::GetTextureInfo(const char *pFileName)
{
	// �ő吔�擾
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

					return m_pXFile[nCntData];
				}
			}
		}
	}

	return m_pXFile[nNumAll + 1];
}

//==========================================================================
// �e�N�X�`�����擾
//==========================================================================
CTexture::STexture CTexture::GetTextureInfo(int nIdxTex)
{
	return m_pXFile[nIdxTex];
}
