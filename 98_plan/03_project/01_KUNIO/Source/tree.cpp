//=============================================================================
// 
//  tree���� [tree.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "tree.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_TREE_000 = "data\\TEXTURE\\tree\\trees_001.png";	// �e�N�X�`���̃t�@�C��
	const std::string TEXTURE_TREE_001 = "data\\TEXTURE\\tree\\trees_002.png";	// �e�N�X�`���̃t�@�C��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTree::CTree(int nPriority, const LAYER layer) : CObject3D(nPriority, layer)
{
	// �l�̃N���A
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTree::~CTree()
{

}

//==========================================================================
// ��������
//==========================================================================
CTree* CTree::Create(MyLib::Vector3 pos)
{
	// �������̊m��
	CTree* pObj = DEBUG_NEW CTree;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();
		pObj->SetPosition(pos);
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTree::Init()
{
	int choice = rand() % 2;
	int texID;

	// �I�u�W�F�N�g3D�̏�����
	CObject3D::Init();

	if (choice == 0)
	{
		// �e�N�X�`���ݒ�
		texID = CTexture::GetInstance()->Regist(TEXTURE_TREE_000);
		BindTexture(texID);

	}
	else
	{
		// �e�N�X�`���ݒ�
		texID = CTexture::GetInstance()->Regist(TEXTURE_TREE_001);
		BindTexture(texID);

	}

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 1000.0f);
#endif

	MyLib::Vector3 setsize = MyLib::Vector3(size.x, size.y, 0.0f);
	SetSize(setsize);
	//SetPosition(MyLib::Vector3(-2223.0f,900.0f,10128.0f));
	SetSizeOrigin(setsize);


	// �ʒu�A�����ݒ�͕K�v������Βǉ�

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTree::Uninit()
{
	// �I������
	CObject3D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTree::Update()
{
	// �X�V����
	CObject3D::Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTree::Draw()
{
	// �`�揈��
	CObject3D::Draw();
}
