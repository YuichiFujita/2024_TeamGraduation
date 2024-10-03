//=============================================================================
// 
//  �e�N�X�`������ [texture.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"

// �ǂݍ��ރe�N�X�`���̃w�b�_�[
#include "map.h"
#include "keyconfig.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::wstring MAINFOLODER = L"data\\TEXTURE";
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CTexture* CTexture::m_pTexture = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTexture::CTexture()
{
	m_TexInfo.clear();
	m_ImageNames.clear();	// �ǂݍ��ݗp������
	m_FolderFilePath.clear();	// �t�H���_�[�i�[�̃t�@�C���p�X
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTexture::~CTexture()
{

}

//==========================================================================
// ��������
//==========================================================================
CTexture* CTexture::Create()
{
	if (m_pTexture == nullptr)
	{// �܂��������Ă��Ȃ�������

		// �C���X�^���X����
		m_pTexture = DEBUG_NEW CTexture;
		m_pTexture->Init();
	}

	return m_pTexture;
}

//==========================================================================
// ����������
//==========================================================================
void CTexture::Init()
{
	STexture init = {};
	m_TexInfo.emplace_back();		// �ŏ���null��ǉ�
	m_ImageNames.emplace_back();	// �ŏ���null��ǉ�
}

//==========================================================================
// �S�Ẵe�N�X�`���ǂݍ���
//==========================================================================
HRESULT CTexture::LoadAll()
{
#ifndef _DEBUG

	// �S����
	SearchAllImages(MAINFOLODER);

	// �ǂݍ��񂾃t�@�C�����R�s�[
	std::vector<std::string> filenames = m_FolderFilePath;
	for (const auto& name : filenames)
	{
		LoadTex(name);
	}

#endif

	// �}�b�v�p�̓ǂݍ���
	if (FAILED(MyMap::ReadTexture()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	// �L�[�R���t�B�O�p�̓ǂݍ���
	CKeyConfigManager::ConfigTextureLoad();

	return S_OK;
}

//==========================================================================
// �S�Ẳ摜����
//==========================================================================
void CTexture::SearchAllImages(const std::wstring& folderPath) 
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
			{// �t�@�C���p�X�i�[
				std::string fileName = UtilFunc::Transformation::WideToMultiByte((currentFolder + L"\\" + findFileData.cFileName).c_str());
				if (fileName.find(".png") != std::string::npos || fileName.find(".jpg") != std::string::npos || fileName.find(".tga") != std::string::npos)
				{
					m_FolderFilePath.push_back(fileName);
				}
			}
			else if (lstrcmpW(findFileData.cFileName, L".") != 0 && lstrcmpW(findFileData.cFileName, L"..") != 0) 
			{// �K�w�m�F
				std::wstring subFolderPath = currentFolder + L"\\" + findFileData.cFileName;
				folderStack.push(subFolderPath);
			}

		} while (FindNextFileW(hFind, &findFileData) != 0);	// �I�[�̃t�H���_�܂Ŋm�F

		FindClose(hFind);
	}

	int n = 0;
}

//==========================================================================
// �S�Ẵe�N�X�`���̔j��
//==========================================================================
void CTexture::Unload()
{

	for (const auto& texture : m_TexInfo)
	{
		if (texture.pTexture == nullptr)
		{
			continue;
		}
		texture.pTexture->Release();
	}

	m_TexInfo.clear();
	m_TexInfo.shrink_to_fit();
}

//==========================================================================
// �e�N�X�`���̊��蓖�ď���
//==========================================================================
int CTexture::Regist(const std::string& file)
{
	if (file == "")
	{
		return 0;
	}

	// �ϊ���̃p�X
	std::string transformFile(file);

	// \\�ϊ�
	transformFile = UtilFunc::Transformation::ReplaceBackslash(transformFile);
	transformFile = UtilFunc::Transformation::ReplaceForwardSlashes(transformFile);

	auto itr = std::find(m_ImageNames.begin(), m_ImageNames.end(), transformFile);
	if (itr != m_ImageNames.end())
	{
		return  static_cast<int>(std::distance(m_ImageNames.begin(), itr));
	}

	// �����ۑ�
	int nNumAll = GetNumAll();

	// �e�N�X�`���ǂݍ���
	HRESULT hr = LoadTex(transformFile);
	if (FAILED(hr))
	{
		return 0;
	}

	return nNumAll;
}

//==========================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//==========================================================================
HRESULT CTexture::LoadTex(const std::string& file)
{
	HRESULT hr;
	int nIdx = static_cast<int>(m_TexInfo.size());

	// ���蓖��
	m_TexInfo.emplace_back();
	m_ImageNames.emplace_back();	// �ǂݍ��ݗp������

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevive = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	hr = D3DXCreateTextureFromFile(pDevive,
		file.c_str(),
		&m_TexInfo[nIdx].pTexture);

	if (FAILED(hr))
	{
		// �v�f�폜
		m_TexInfo.erase(m_TexInfo.end() - 1);

		std::string error = "�e�N�X�`���ǂݍ��ݎ��s�I" + file;
		MyAssert::CustomAssert(false, error);
		return E_FAIL;
	}

	// �e�N�X�`���f�ޏ��
	D3DXGetImageInfoFromFile(file.c_str(), &m_TexInfo[nIdx].imageInfo);

	// �t�@�C�����ƒ����ۑ�
	m_TexInfo[nIdx].filename = file;
	m_TexInfo[nIdx].nFileNameLen = m_TexInfo[nIdx].filename.length();

	m_ImageNames.back() = file;	// �ǂݍ��ݗp������

	return S_OK;
}

//==========================================================================
// �e�N�X�`���̃A�h���X�擾
//==========================================================================
LPDIRECT3DTEXTURE9 CTexture::GetAdress(int nIdx)
{
	if (static_cast<int>(m_TexInfo.size()) <= nIdx)
	{
		return nullptr;
	}
	return m_TexInfo[nIdx].pTexture;
}

//==========================================================================
// �e�N�X�`���̑����擾
//==========================================================================
int CTexture::GetNumAll()
{
	return static_cast<int>(m_TexInfo.size());
}

//==========================================================================
// �e�N�X�`�����擾
//==========================================================================
CTexture::STexture CTexture::GetTextureInfo(const std::string& file)
{
	// �ő吔�擾
	int nNumAll = GetNumAll() + 1;

	if (file == "")
	{
		return m_TexInfo[nNumAll];
	}

	int nNowLen = file.length();	// ����̃t�@�C��������

	for (int nCntData = 0; nCntData < nNumAll; nCntData++)
	{
		if (m_TexInfo[nCntData].nFileNameLen != nNowLen)
		{// �t�@�C�����̒������������
			continue;
		}

		// ���Ƀe�N�X�`�����ǂݍ��܂�ĂȂ����̍ŏI�m�F
		if (m_TexInfo[nCntData].filename == file)
		{// �t�@�C��������v���Ă���

			return m_TexInfo[nCntData];
		}
	}

	int nIdx = nNumAll + 1;
	return m_TexInfo[nIdx];
}

//==========================================================================
// �e�N�X�`�����擾
//==========================================================================
CTexture::STexture CTexture::GetTextureInfo(int nIdxTex)
{
	return m_TexInfo[nIdxTex];
}

//==========================================================================
// �e�N�X�`���f�ނ̃T�C�Y�擾
//==========================================================================
D3DXVECTOR2 CTexture::GetImageSize(int nIdx)
{
	if (static_cast<int>(m_TexInfo.size()) <= nIdx)
	{
		return mylib_const::DEFAULT_VECTOR2;
	}
	return D3DXVECTOR2((float)m_TexInfo[nIdx].imageInfo.Width, (float)m_TexInfo[nIdx].imageInfo.Height);
}
