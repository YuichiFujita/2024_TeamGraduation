//=============================================================================
// 
//  ����(2D)���� [number_2D.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "number_3D.h"
#include "manager.h"
#include "renderer.h"
#include "object3D.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CNumber3D::CNumber3D(int nPriority) : CNumber(nPriority)
{
	// �l�̃N���A
	m_aObject3D = nullptr;			// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	m_bAddAlpha = true;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CNumber3D::~CNumber3D()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CNumber3D::Init()
{
	// ��������
	m_aObject3D = CObject3D::Create(GetPriority());

	// ��ސݒ�
	SetType(CObject::TYPE_NUMBER);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CNumber3D::Uninit()
{
	// �I������
	if (m_aObject3D != nullptr)
	{
		m_aObject3D = nullptr;
	}

	CNumber::Uninit();
}

//==========================================================================
// �폜����
//==========================================================================
void CNumber3D::Kill()
{
	if (m_aObject3D != nullptr)
	{
		m_aObject3D->Uninit();
		m_aObject3D = nullptr;
	}

	CNumber::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CNumber3D::Update()
{
	// �X�V����
	if (m_aObject3D != nullptr)
	{
		m_aObject3D->SetEnableDisp(IsDisp());
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CNumber3D::Draw()
{
	//// �f�o�C�X�̎擾
	//LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//// �`�揈��
	//if (m_aObject3D != nullptr)
	//{
	//	// �A���t�@�e�X�g��L���ɂ���
	//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//	// ���u�����f�B���O�����Z�����ɐݒ�
	//	if (m_bAddAlpha)
	//	{
	//		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//	}

	//	m_aObject3D->Draw();

	//	// ���u�����f�B���O�����ɖ߂�
	//	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//	// �A���t�@�e�X�g�𖳌��ɂ���
	//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	//}
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CNumber3D::SetVtx()
{
	m_aObject3D->SetVtx();
}

//==========================================================================
// �e�N�X�`���̊��蓖��
//==========================================================================
void CNumber3D::BindTexture(int nIdx)
{
	m_aObject3D->BindTexture(nIdx);
}

//==========================================================================
// ��ސݒ�
//==========================================================================
void CNumber3D::SetType(const CObject::TYPE type)
{
	m_aObject3D->SetType(type);
	CNumber::SetType(type);
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CNumber3D::SetPosition(const MyLib::Vector3& pos)
{
	m_aObject3D->SetPosition(pos);
}

//==========================================================================
// �ʒu�擾
//==========================================================================
MyLib::Vector3 CNumber3D::GetPosition() const
{
	return m_aObject3D->GetPosition();
}

//==========================================================================
// �ړ��ʐݒ�
//==========================================================================
void CNumber3D::SetMove(const MyLib::Vector3& move)
{
	m_aObject3D->SetMove(move);
}

//==========================================================================
// �ړ��ʎ擾
//==========================================================================
MyLib::Vector3 CNumber3D::GetMove() const
{
	return m_aObject3D->GetMove();
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CNumber3D::SetRotation(const MyLib::Vector3& rot)
{
	m_aObject3D->SetRotation(rot);
}

//==========================================================================
// �����擾
//==========================================================================
MyLib::Vector3 CNumber3D::GetRotation() const
{
	return m_aObject3D->GetRotation();
}

//==========================================================================
// �F�ݒ�
//==========================================================================
void CNumber3D::SetColor(const D3DXCOLOR col)
{
	m_aObject3D->SetColor(col);
}

//==========================================================================
// �F�擾
//==========================================================================
D3DXCOLOR CNumber3D::GetColor() const
{
	return m_aObject3D->GetColor();
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CNumber3D::SetSize(const D3DXVECTOR2 size)
{
	m_aObject3D->SetSize(MyLib::Vector3(size.x, 0.0f, size.y));
}

//==========================================================================
// �T�C�Y�擾
//==========================================================================
D3DXVECTOR2 CNumber3D::GetSize() const
{
	MyLib::Vector3 size = m_aObject3D->GetSize();
	return D3DXVECTOR2(size.x, size.z);
}

//==========================================================================
// ���̃T�C�Y�̐ݒ�
//==========================================================================
void CNumber3D::SetSizeOrigin(const D3DXVECTOR2 size)
{
	m_aObject3D->SetSizeOrigin(MyLib::Vector3(size.x, 0.0f, size.y));
}

//==========================================================================
// ���̃T�C�Y�̎擾
//==========================================================================
D3DXVECTOR2 CNumber3D::GetSizeOrigin() const
{
	MyLib::Vector3 size = m_aObject3D->GetSizeOrigin();
	return D3DXVECTOR2(size.x, size.z);
}

//==========================================================================
// �e�N�X�`�����W�ݒ�
//==========================================================================
void CNumber3D::SetTex(D3DXVECTOR2 *tex)
{
	m_aObject3D->SetTex(tex);
}

//==========================================================================
// �e�N�X�`�����W�擾
//==========================================================================
D3DXVECTOR2 *CNumber3D::GetTex()
{
	return m_aObject3D->GetTex();
}

//==========================================================================
// �I�u�W�F�N�g3D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject3D *CNumber3D::GetObject3D()
{
	return m_aObject3D;
}
