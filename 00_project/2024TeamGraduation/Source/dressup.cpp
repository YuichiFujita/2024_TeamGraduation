//=============================================================================
// 
// �����ւ�(��)���� [dressup.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "dressup.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"

// �h����
#include "dressup_hair.h"		// ��
#include "dressup_accessory.h"	// �A�N�Z
#include "dressup_face.h"		// ��

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CDressup::CDressup()
{
	m_nSwitchIdx = 0;		// �؂�ւ���C���f�b�N�X
	m_nNowIdx = 0;			// ���݂̃C���f�b�N�X
	m_nControllIdx = 0;		// ���삷��C���f�b�N�X
	m_pObjChara = nullptr;	// �L�����N�^�[�̃|�C���^
	m_vecModelName.clear();	// ���f����
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CDressup::~CDressup()
{

}

//==========================================================================
// ��������
//==========================================================================
CDressup* CDressup::Create(EType type, CObjectChara* pObjChara, int nSwitchType)
{
	// �������̊m��
	CDressup* pObj = nullptr;

	switch (type)
	{
	case CDressup::TYPE_HAIR:
		pObj = DEBUG_NEW CDressup_Hair;
		break;

	case CDressup::TYPE_ACCESSORY:
		pObj = DEBUG_NEW CDressup_Accessory;
		break;

	case CDressup::TYPE_FACE:
		pObj = DEBUG_NEW CDressup_Face;
		break;

	default:
		return nullptr;
		break;
	}

	if (pObj != nullptr)
	{
		// �������
		pObj->m_nSwitchIdx = nSwitchType;
		pObj->m_pObjChara = pObjChara;

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CDressup::Init()
{
	return S_OK;
}

//==========================================================================
// ���f���S�ēǂݍ���
//==========================================================================
void CDressup::LoadAllModel(const std::wstring& folder)
{
	// �K�w�t�H���_�ǉ�
	std::stack<std::wstring> folderStack;
	folderStack.push(folder);

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

		do 
		{// �I�[�̃t�H���_�܂Ŋm�F

			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{// �t�@�C���p�X�i�[
				std::string fileName = UtilFunc::Transformation::WideToMultiByte((currentFolder + L"\\" + findFileData.cFileName).c_str());
				if (fileName.find(".x") != std::string::npos)
				{// .x�̂ݒǉ�
					m_vecModelName.push_back(fileName);
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
}

//==========================================================================
// �e�N�X�`���S�ǂݍ���
//==========================================================================
void CDressup::LoadAllTexture(const std::wstring& folder)
{
	// �K�w�t�H���_�ǉ�
	std::stack<std::wstring> folderStack;
	folderStack.push(folder);

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

		do
		{// �I�[�̃t�H���_�܂Ŋm�F

			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{// �t�@�C���p�X�i�[
				std::string fileName = UtilFunc::Transformation::WideToMultiByte((currentFolder + L"\\" + findFileData.cFileName).c_str());
				if (fileName.find(".png") != std::string::npos || fileName.find(".jpg") != std::string::npos || fileName.find(".tga") != std::string::npos)
				{
					m_vecModelName.push_back(fileName);
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
}

//==========================================================================
// �I������
//==========================================================================
void CDressup::Uninit()
{
}

//==========================================================================
// �X�V����
//==========================================================================
void CDressup::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �Ċ��蓖��
//==========================================================================
void CDressup::ReRegist(int idx)
{
	m_nNowIdx = idx;

	// �Ċ��蓖��
	ReRegist();
}

//==========================================================================
// �����_���ݒ�
//==========================================================================
void CDressup::RandSet()
{
	m_nNowIdx = UtilFunc::Transformation::Random(0, static_cast<int>(m_vecModelName.size()) - 1);
	ReRegist();
}

//==========================================================================
// �f�o�b�O����
//==========================================================================
void CDressup::Debug()
{
	
}
