//=============================================================================
// 
//  ����(�r���{�[�h)���� [number_Billboard.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "number_Billboard.h"
#include "manager.h"
#include "renderer.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CNumberBillboard::CNumberBillboard(int nPriority) : CNumber(nPriority)
{
	// �l�̃N���A
	m_pObjBillboard = nullptr;		// �I�u�W�F�N�g�r���{�[�h�̃I�u�W�F�N�g
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CNumberBillboard::~CNumberBillboard()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CNumberBillboard::Init()
{
	// ��������
	m_pObjBillboard = CObjectBillboard::Create(GetPriority());

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CNumberBillboard::Uninit()
{
	// �I������
	if (m_pObjBillboard != nullptr)
	{
		m_pObjBillboard = nullptr;
	}

	CNumber::Uninit();
}

//==========================================================================
// �폜����
//==========================================================================
void CNumberBillboard::Kill()
{
	if (m_pObjBillboard != nullptr)
	{
		m_pObjBillboard->Uninit();
		m_pObjBillboard = nullptr;
	}

	CNumber::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CNumberBillboard::Update()
{
	// �X�V����
	if (m_pObjBillboard != nullptr)
	{
		m_pObjBillboard->SetEnableDisp(IsDisp());
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CNumberBillboard::Draw()
{
	//// �f�o�C�X�̎擾
	//LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//// ���C�e�B���O�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//// �A���t�@�e�X�g��L���ɂ���
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//// �`�揈��
	//if (m_pObjBillboard != nullptr)
	//{// nullptr����Ȃ�������
	//	m_pObjBillboard->Draw();
	//}

	//// �A���t�@�e�X�g�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//// ���C�e�B���O��L���ɂ���
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CNumberBillboard::SetVtx()
{
	m_pObjBillboard->SetVtx();
}

//==========================================================================
// �e�N�X�`���̊��蓖��
//==========================================================================
void CNumberBillboard::BindTexture(int nIdx)
{
	// ���蓖�Ă�
	m_pObjBillboard->BindTexture(nIdx);
}

//==========================================================================
// ��ސݒ�
//==========================================================================
void CNumberBillboard::SetType(const CObject::TYPE type)
{
	m_pObjBillboard->SetType(type);
	CNumber::SetType(type);
}


//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CNumberBillboard::SetPosition(const MyLib::Vector3& pos)
{
	m_pObjBillboard->SetPosition(pos);
}

//==========================================================================
// �ʒu�擾
//==========================================================================
MyLib::Vector3 CNumberBillboard::GetPosition() const
{
	return m_pObjBillboard->GetPosition();
}

//==========================================================================
// �ړ��ʐݒ�
//==========================================================================
void CNumberBillboard::SetMove(const MyLib::Vector3& move)
{
	m_pObjBillboard->SetMove(move);
}

//==========================================================================
// �ړ��ʎ擾
//==========================================================================
MyLib::Vector3 CNumberBillboard::GetMove() const
{
	return m_pObjBillboard->GetMove();
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CNumberBillboard::SetRotation(const MyLib::Vector3& rot)
{
	m_pObjBillboard->SetRotation(rot);
}

//==========================================================================
// �����擾
//==========================================================================
MyLib::Vector3 CNumberBillboard::GetRotation() const
{
	return m_pObjBillboard->GetRotation();
}

//==========================================================================
// �F�ݒ�
//==========================================================================
void CNumberBillboard::SetColor(const D3DXCOLOR col)
{
	m_pObjBillboard->SetColor(col);
}

//==========================================================================
// �F�擾
//==========================================================================
D3DXCOLOR CNumberBillboard::GetColor() const
{
	return m_pObjBillboard->GetColor();
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CNumberBillboard::SetSize(const D3DXVECTOR2 size)
{
	m_pObjBillboard->SetSize(size);		// �T�C�Y
}

//==========================================================================
// �T�C�Y�擾
//==========================================================================
D3DXVECTOR2 CNumberBillboard::GetSize() const
{
	return m_pObjBillboard->GetSize();
}

//==========================================================================
// ���̃T�C�Y�̐ݒ�
//==========================================================================
void CNumberBillboard::SetSizeOrigin(const D3DXVECTOR2 size)
{
	m_pObjBillboard->SetSizeOrigin(size);
}

//==========================================================================
// ���̃T�C�Y�̎擾
//==========================================================================
D3DXVECTOR2 CNumberBillboard::GetSizeOrigin() const
{
	return m_pObjBillboard->GetSizeOrigin();
}

//==========================================================================
// �e�N�X�`�����W�ݒ�
//==========================================================================
void CNumberBillboard::SetTex(D3DXVECTOR2 *tex)
{
	m_pObjBillboard->SetTex(tex);
}

//==========================================================================
// �e�N�X�`�����W�擾
//==========================================================================
D3DXVECTOR2 *CNumberBillboard::GetTex()
{
	return m_pObjBillboard->GetTex();
}
