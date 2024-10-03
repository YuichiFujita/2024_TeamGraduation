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
// �萔��`
//==========================================================================
namespace
{
	const std::wstring MAINFOLODER = L"data\\MODEL";
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CXLoad* CXLoad::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CXLoad::CXLoad()
{
	m_XFileInfo.clear();		// X�t�@�C���̏��
	m_ImageNames.clear();		// �ǂݍ��ݗp������
	m_FolderFilePath.clear();	// �t�H���_�[�i�[�p������
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CXLoad::~CXLoad()
{

}

//==========================================================================
// ��������
//==========================================================================
CXLoad* CXLoad::Create()
{
	if (m_pThisPtr == nullptr)
	{// �܂��������Ă��Ȃ�������

		// �C���X�^���X����
		m_pThisPtr = DEBUG_NEW CXLoad;
		m_pThisPtr->Init();
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CXLoad::Init()
{
	return S_OK;
}

//==========================================================================
// �S�Ẵ��f���ǂݍ���
//==========================================================================
HRESULT CXLoad::LoadAll()
{
#ifndef _DEBUG

	// �S����
	SearchAllXModel(MAINFOLODER);

	// �ǂݍ��񂾃t�@�C�����R�s�[
	for (const auto& name : m_FolderFilePath)
	{
		if (FAILED(Load(name)))
		{
			return E_FAIL;
		}
	}

#endif
	return S_OK;
}

//==========================================================================
// �S�Ẵ��f������
//==========================================================================
void CXLoad::SearchAllXModel(const std::wstring& folderPath)
{
	std::stack<std::wstring> folderStack;
	folderStack.push(folderPath);

	while (!folderStack.empty())
	{
		std::wstring currentFolder = folderStack.top();
		folderStack.pop();

		WIN32_FIND_DATAW findFileData;
		HANDLE hFind = FindFirstFileW((currentFolder + L"\\*").c_str(), &findFileData);

		if (hFind == INVALID_HANDLE_VALUE)
		{
			continue;
		}

		do {

			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{// �p�X
				std::string fileName = UtilFunc::Transformation::WideToMultiByte((currentFolder + L"\\" + findFileData.cFileName).c_str());
				if (fileName.find(".x") != std::string::npos)
				{
					m_FolderFilePath.push_back(fileName);
				}
			}
			else if (lstrcmpW(findFileData.cFileName, L".") != 0 && lstrcmpW(findFileData.cFileName, L"..") != 0)
			{// �t�H���_
				std::wstring subFolderPath = currentFolder + L"\\" + findFileData.cFileName;
				folderStack.push(subFolderPath);
			}

		} while (FindNextFileW(hFind, &findFileData) != 0);

		FindClose(hFind);
	}
}

//==========================================================================
// �I������
//==========================================================================
void CXLoad::Uninit()
{
	for (auto& XInfo : m_XFileInfo)
	{
		if (XInfo.nIdxTexture != nullptr)
		{
			delete[] XInfo.nIdxTexture;
			XInfo.nIdxTexture = nullptr;
		}

		// ���b�V���̔j��
		if (XInfo.pMesh != nullptr)
		{
			XInfo.pMesh->Release();
			XInfo.pMesh = nullptr;
		}

		// �}�e���A���̔j��
		if (XInfo.pBuffMat != nullptr)
		{
			XInfo.pBuffMat->Release();
			XInfo.pBuffMat = nullptr;
		}

		// ���_���W�̔j��
		if (XInfo.pVtxPos != nullptr)
		{
			delete[] XInfo.pVtxPos;
			XInfo.pVtxPos = nullptr;
		}
	}

	m_XFileInfo.clear();
	m_ImageNames.clear();	// �ǂݍ��ݗp������

	if (m_pThisPtr != nullptr)
	{
		delete m_pThisPtr;
		m_pThisPtr = nullptr;
	}
}

//==========================================================================
// X�t�@�C���̔j��
//==========================================================================
void CXLoad::Unload()
{

}

//==========================================================================
// X�t�@�C���̓ǂݍ���
//==========================================================================
int CXLoad::XLoad(std::string file)
{
#if 0
	// �ő吔�擾
	int nIdx = 0;
	int nNumAll = GetNumAll();

	if (file == "")
	{
		return -1;
	}
	int nNowLen = file.length();


	for (int nCntData = 0; nCntData < nNumAll; nCntData++)
	{
		if (m_XFileInfo[nCntData].nFileNameLen != nNowLen)
		{// �t�@�C�����̒������Ⴄ
			continue;
		}

		// ���Ƀe�N�X�`�����ǂݍ��܂�ĂȂ����̍ŏI�m�F
		if (m_XFileInfo[nCntData].filename == file)
		{// �t�@�C��������v���Ă���
			return nCntData;
		}
	}

	// �ǂݍ���
	if (FAILED(Load(file)))
	{
		return E_FAIL;
	}

	// �C���f�b�N�X�ԍ��ۑ�
	nIdx = nNumAll;
	return nIdx;

#else


	if (file == "")
	{
		return 0;
	}

	// \\�ϊ�
	file = UtilFunc::Transformation::ReplaceBackslash(file);
	file = UtilFunc::Transformation::ReplaceForwardSlashes(file);

	auto itr = std::find(m_ImageNames.begin(), m_ImageNames.end(), file);
	if (itr != m_ImageNames.end())
	{
		return itr - m_ImageNames.begin();
	}

	// �����ۑ�
	int nNumAll = GetNumAll();

	// �ǂݍ���
	HRESULT hr = Load(file);
	if (FAILED(hr))
	{
		return 0;
	}

	return nNumAll;
#endif
}

//==========================================================================
// ���[�h����
//==========================================================================
HRESULT CXLoad::Load(std::string file)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	int nIdx = GetNumAll();

	// �v�f�ǉ�
	SXFile initinfo = {};
	m_XFileInfo.push_back(initinfo);

	//X�t�@�C���̓ǂݍ���
	HRESULT hr = D3DXLoadMeshFromX(
		file.c_str(),
		D3DXMESH_SYSTEMMEM,
		pDevice,
		nullptr,
		&m_XFileInfo[nIdx].pBuffMat,
		nullptr,
		&m_XFileInfo[nIdx].dwNumMat,
		&m_XFileInfo[nIdx].pMesh);

	if (FAILED(hr))
	{
		// �v�f�폜
		m_XFileInfo.erase(m_XFileInfo.end() - 1);

		std::string error = "���f���ǂݍ��ݎ��s�I" + file;
		MyAssert::CustomAssert(false, error);
		return E_FAIL;
	}

	// �t�@�C�����ƒ����ۑ�
	m_XFileInfo[nIdx].filename = file;
	m_XFileInfo[nIdx].nFileNameLen = file.length();

	// �ǂݍ��ݗp�ɂ��ۑ�
	m_ImageNames.emplace_back();
	m_ImageNames.back() = file;

	// �e�N�X�`���̃C���f�b�N�X�ԍ�
	if (m_XFileInfo[nIdx].nIdxTexture == nullptr)
	{
		m_XFileInfo[nIdx].nIdxTexture = DEBUG_NEW int[(int)m_XFileInfo[nIdx].dwNumMat];

		if (m_XFileInfo[nIdx].nIdxTexture == nullptr)
		{// �m�ۂł��Ă��Ȃ��ꍇ
			return E_FAIL;
		}
	}

	// ���_���擾
	m_XFileInfo[nIdx].nVtxNum = m_XFileInfo[nIdx].pMesh->GetNumVertices();

	// ���_�����Ń������m��
	m_XFileInfo[nIdx].pVtxPos = DEBUG_NEW MyLib::Vector3[m_XFileInfo[nIdx].nVtxNum];

	// �ʂ̐��擾
	m_XFileInfo[nIdx].nFaceNum = m_XFileInfo[nIdx].pMesh->GetNumFaces();

	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_XFileInfo[nIdx].pBuffMat->GetBufferPointer();

	// ���_�����J��Ԃ�
	for (int nCntMat = 0; nCntMat < (int)m_XFileInfo[nIdx].dwNumMat; nCntMat++)
	{
		m_XFileInfo[nIdx].nIdxTexture[nCntMat] = 0;

		if (pMat[nCntMat].pTextureFilename != nullptr)
		{// �t�@�C�������݂���

			// �e�N�X�`���̓ǂݍ���
			m_XFileInfo[nIdx].nIdxTexture[nCntMat] = CTexture::GetInstance()->Regist(pMat[nCntMat].pTextureFilename);

			if (FAILED(hr))
			{// ���s���Ă�����
				return E_FAIL;
			}
		}
	}

	BYTE* pVtxBuff;

	// ���_�o�b�t�@�����b�N
	m_XFileInfo[nIdx].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	DWORD dwSizeFVF = D3DXGetFVFVertexSize(m_XFileInfo[nIdx].pMesh->GetFVF());

	// ���_���W
	for (int nCntVtx = 0; nCntVtx < m_XFileInfo[nIdx].nVtxNum; nCntVtx++)
	{
		// ���_���W���
		m_XFileInfo[nIdx].pVtxPos[nCntVtx] = *(MyLib::Vector3*)pVtxBuff;

		// �T�C�Y���|�C���^�ړ�
		pVtxBuff += dwSizeFVF;
	}

	// ���_�o�b�t�@���A�����b�N
	m_XFileInfo[nIdx].pMesh->UnlockVertexBuffer();

	// �S���_�`�F�b�N
	UtilFunc::Calculation::CalModelVtx(0.0f, &m_XFileInfo[nIdx].vtxMax, &m_XFileInfo[nIdx].vtxMin, m_XFileInfo[nIdx].pMesh, m_XFileInfo[nIdx].pVtxBuff);

	// �C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIndexBuff;
	m_XFileInfo[nIdx].pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndexBuff);

	for (int nCntIdx = 0; nCntIdx < m_XFileInfo[nIdx].nFaceNum; nCntIdx++)
	{
		// �O�p�`���`������C���f�b�N�X, �ʖ�������3������
		int nIdx1 = (int)pIndexBuff[nCntIdx * 3];
		int nIdx2 = (int)pIndexBuff[nCntIdx * 3 + 1];
		int nIdx3 = (int)pIndexBuff[nCntIdx * 3 + 2];

		// �ꎞ���
		MyLib::Vector3 pos1 = m_XFileInfo[nIdx].pVtxPos[nIdx1];
		MyLib::Vector3 pos2 = m_XFileInfo[nIdx].pVtxPos[nIdx2];
		MyLib::Vector3 pos3 = m_XFileInfo[nIdx].pVtxPos[nIdx3];

		// ���_�Ԃ̍ő勗��
		float fVtxDistance = 0.0f;

		// 2�_�̋���
		fVtxDistance = sqrtf(
			((pos1.x - pos2.x) * (pos1.x - pos2.x)) +
			((pos1.z - pos2.z) * (pos1.z - pos2.z)));

		if (fVtxDistance > m_XFileInfo[nIdx].fMaxVtxDistance)
		{
			// �ő勗���ۑ�
			m_XFileInfo[nIdx].fMaxVtxDistance = fVtxDistance;
		}

		// 2�_�̋���
		fVtxDistance = sqrtf(
			((pos2.x - pos3.x) * (pos2.x - pos3.x)) +
			((pos2.z - pos3.z) * (pos2.z - pos3.z)));
		if (fVtxDistance > m_XFileInfo[nIdx].fMaxVtxDistance)
		{
			// �ő勗���ۑ�
			m_XFileInfo[nIdx].fMaxVtxDistance = fVtxDistance;
		}

		// 2�_�̋���
		fVtxDistance = sqrtf(
			((pos3.x - pos1.x) * (pos3.x - pos1.x)) +
			((pos3.z - pos1.z) * (pos3.z - pos1.z)));
		if (fVtxDistance > m_XFileInfo[nIdx].fMaxVtxDistance)
		{
			// �ő勗���ۑ�
			m_XFileInfo[nIdx].fMaxVtxDistance = fVtxDistance;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_XFileInfo[nIdx].pMesh->UnlockIndexBuffer();

	return S_OK;
}

//==========================================================================
// X�t�@�C���̃f�[�^�擾
//==========================================================================
CXLoad::SXFile *CXLoad::GetMyObject(int nIdx)
{
	return &m_XFileInfo[nIdx];
}

//==========================================================================
// X�t�@�C���̑����擾
//==========================================================================
int CXLoad::GetNumAll()
{
	return static_cast<int>(m_XFileInfo.size());
}