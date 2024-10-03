//=============================================================================
// 
//  �L�[�R���t�B�O�ɉ������{�^���\�� [controlkeydisp.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "controlkeydisp.h"
#include "keyconfig.h"

std::map<CInputGamepad::BUTTON, int> CControlKeyDisp::m_buttonTextures;
//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CControlKeyDisp::CControlKeyDisp(int nPriority) : CObject2D(nPriority)
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CControlKeyDisp::~CControlKeyDisp()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CControlKeyDisp* CControlKeyDisp::Create(MyLib::Vector3 pos, MyLib::Vector3 rot, float width, float height, int type)
{
	// �������̊m��
	CControlKeyDisp* pObject = DEBUG_NEW CControlKeyDisp;

	if (pObject != nullptr)
	{
		pObject->SetPosition(pos);
		pObject->SetOriginPosition(pos);
		pObject->SetRotation(rot);
		pObject->SetSize(D3DXVECTOR2(width, height));
		pObject->SetSizeOrigin(pObject->GetSize());

		// ����������
		pObject->Init();

		// �L�[�R���t�B�O�ɉ����ăe�N�X�`���ݒ�
		pObject->CControlKeyDisp::SetType(type);
	}

	return pObject;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CControlKeyDisp::Init()
{
	CObject2D::Init();

	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CControlKeyDisp::Uninit()
{
	CObject2D::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CControlKeyDisp::Kill()
{
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CControlKeyDisp::Update()
{
	CObject2D::Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CControlKeyDisp::Draw()
{
	CObject2D::Draw();
}

//==========================================================================
// ��ށE�e�N�X�`���ݒ�
//==========================================================================
void CControlKeyDisp::SetType(int type)
{
	auto itr = m_buttonTextures.find(static_cast<CInputGamepad::BUTTON>(type));
	if (itr != m_buttonTextures.end())
	{
		BindTexture((*itr).second);
		SetVtx();
	}
}