//=============================================================================
// 
// �f�o�b�O���͈͏��� [debug_bressrange.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "debug_bressrange.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CDebugBressRange::CDebugBressRange(int nPriority) : CObjectBillboard(nPriority)
{
	// �l�̃N���A
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CDebugBressRange::~CDebugBressRange()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CDebugBressRange* CDebugBressRange::Create()
{
	// �����p�̃I�u�W�F�N�g
	CDebugBressRange* pEffect = nullptr;

	// �������̊m��
	pEffect = DEBUG_NEW CDebugBressRange;

	if (pEffect != nullptr)
	{
		// ����������
		pEffect->Init();
	}

	return pEffect;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CDebugBressRange::Init()
{
	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// ����������
	HRESULT hr = CObjectBillboard::Init();

	if (FAILED(hr))
	{
		Uninit();
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CDebugBressRange::Uninit()
{
	
	// �I������
	CObjectBillboard::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CDebugBressRange::Update()
{
	// �ߋ��̈ʒu�ݒ�
	SetOldPosition(GetPosition());

	CObjectBillboard::Update();

	// ���_���W�̐ݒ�
	SetVtx();

}

//==========================================================================
// �`�揈��
//==========================================================================
void CDebugBressRange::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �`�揈��
	CObjectBillboard::Draw();

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CDebugBressRange::SetVtx()
{
	// ���_�ݒ�
	CObjectBillboard::SetVtx();

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = m_LeftUP;
	pVtx[1].pos = m_RightUP;
	pVtx[2].pos = m_LeftDW;
	pVtx[3].pos = m_RightDW;

	// ���_�o�b�t�@���A�����b�N���b�N
	m_pVtxBuff->Unlock();
}

void CDebugBressRange::SetRange(const MyLib::Vector3& leftup, const MyLib::Vector3& rightup, const MyLib::Vector3& leftdw, const MyLib::Vector3& rightdw)
{
	m_LeftUP = leftup;
	m_RightUP = rightup;
	m_LeftDW = leftdw;
	m_RightDW = rightdw;
}
