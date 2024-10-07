//=============================================================================
// 
//  �I�u�W�F�N�g���C������ [objectLine.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "objectLine.h"
#include "manager.h"
#include "calculation.h"
#include "3D_Effect.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObjectLine::CObjectLine(int nPriority, const LAYER layer) : CObject(nPriority, layer)
{
	m_Info = SLineInfo();
	m_bZSort = false;				// Z�\�[�g�̃t���O
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObjectLine::~CObjectLine()
{

}

//==========================================================================
// ��������
//==========================================================================
CObjectLine *CObjectLine::Create(MyLib::Vector3 left, MyLib::Vector3 right, D3DXCOLOR col)
{
	// �����p�̃I�u�W�F�N�g
	CObjectLine *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{// nullptr��������

		// �������̊m��
		pObject3D = DEBUG_NEW CObjectLine;

		if (pObject3D != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// �������
			pObject3D->m_Info.left = left;
			pObject3D->m_Info.right = right;
			pObject3D->m_Info.col = col;

			// ����������
			pObject3D->Init();
		}

		return pObject3D;
	}

	return nullptr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectLine::Init()
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// ���_���ݒ�
	SetVtx();

	SetType(CObject::TYPE::TYPE_OBJECT3D);

	SetPosition(0.0f);
	SetOriginPosition((m_Info.left + m_Info.right) * 0.5f);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObjectLine::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObjectLine::Update()
{

	// ���_���ݒ�
	SetVtx();
}


//==========================================================================
// �`�揈��
//==========================================================================
void CObjectLine::Draw()
{
	// �{�b�N�X�R���C�_�[�̕`��
	if (!CManager::IsDisp_BoxColliders()) return;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (m_bZSort)
	{
		// Z�e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	// ��ɕ`�悷��
	}



	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	// �v�Z�p�}�g���b�N�X�錾
	MyLib::Matrix mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����
	m_mtxWorld.Identity();

	// �����𔽉f����
	mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRot);

	// �ʒu�𔽉f����
	mtxTrans.Translation(pos);
	m_mtxWorld.Multiply(m_mtxWorld, mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3DXMATRIX mtx = m_mtxWorld.ConvertD3DXMATRIX();
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


}

//==========================================================================
// ���_���ݒ�
//==========================================================================
void CObjectLine::SetVtx()
{
	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �ʒu���X�V
	pVtx[0].pos = m_Info.left;
	pVtx[1].pos = m_Info.right;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_Info.col;
	pVtx[1].col = m_Info.col;

	//// �@���̐ݒ�
	//pVtx[0].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	//pVtx[1].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N���b�N
	m_pVtxBuff->Unlock();
}
