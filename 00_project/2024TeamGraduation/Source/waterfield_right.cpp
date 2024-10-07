//=============================================================================
// 
//  ���t�B�[���h���� [waterfield_right.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "waterfield_right.h"
#include "manager.h"
#include "input.h"
#include "calculation.h"
#include "debugproc.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\FIELD\\water-bg-pattern-02.jpg";
	const float SCROLL_V = -0.0015f;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CWaterField_Right::CWaterField_Right() : CWaterField()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CWaterField_Right::~CWaterField_Right()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CWaterField_Right::Init()
{
	// ���t�B�[���h�̏�����
	CWaterField::Init();

	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �e��ϐ�������
	SetPosition(MyLib::Vector3(0.0f, -51.0f, 0.0f));			// �ʒu


	// �S�Ă̗v�f����������
	D3DXCOLOR* pVtxCol = GetVtxCol();
	std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));

	return E_FAIL;
}

//==========================================================================
// �I������
//==========================================================================
void CWaterField_Right::Uninit()
{
	// �I������
	CWaterField::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CWaterField_Right::Update()
{
	m_fTexV -= SCROLL_V;		// V�X�N���[���p
	m_fTexU -= SCROLL_V;		// U�X�N���[���p

	CWaterField::Update();

#if _DEBUG
	// �J���[�G�f�B�b�g
	static ImVec4 myColor = ImVec4(1.0f, 1.0f, 1.0f, 0.21f); // RGBA

	if (ImGui::ColorEdit4("Right Color", &myColor.x))
	{
		D3DXCOLOR* pVtxCol = GetVtxCol();

		// �S�Ă̗v�f����������
		std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(myColor.x, myColor.y, myColor.z, myColor.w));
	}
#endif
}
