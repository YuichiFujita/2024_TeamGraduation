//=============================================================================
// 
// �����ւ�(��)���� [dressup_hair_Mii.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "dressup_hair_Mii.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"
#include "dressup_hair_MiiLow.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::wstring FOLDERNAME = L"data\\MODEL\\audience\\hair";	// �ǂݍ��ރt�H���_��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CDressup_Hair_Mii::CDressup_Hair_Mii() : CDressup_Hair()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CDressup_Hair_Mii::~CDressup_Hair_Mii()
{

}

//==========================================================================
// ��������
//==========================================================================
CDressup_Hair_Mii* CDressup_Hair_Mii::Create(CDressup::EType type)
{
	// �C���X�^���X����
	CDressup_Hair_Mii* pAudience = nullptr;

	switch (type)
	{
	case CDressup::EType::TYPE_HAIR_MII:
		pAudience = DEBUG_NEW CDressup_Hair_Mii;
		break;

	case CDressup::EType::TYPE_HAIR_MIILOW:
		pAudience = DEBUG_NEW CDressup_Hair_MiiLow;
		break;

	default:
		break;
	}

	return pAudience;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CDressup_Hair_Mii::Init()
{
	// �S�ēǂݍ���
	LoadAllModel(FOLDERNAME);

	// �e�̏�����
	CDressup::Init();
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CDressup_Hair_Mii::Uninit()
{
	// �e�̏I��
	CDressup::Uninit();
	delete this;
}
