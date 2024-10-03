//=============================================================================
// 
//  �I�u�W�F�N�g3D���� [object3D.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "object3D.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int POLYGON_TOP = 4;	// �|���S���̒��_��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObject3D::CObject3D(int nPriority, const LAYER layer) : CObject(nPriority, layer)
{
	m_mtxWorld.Identity();				// ���[���h�}�g���b�N�X
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
	m_fSize = MyLib::Vector3(0.0f, 0.0f, 0.0f);		// �T�C�Y
	m_pVtxBuff = nullptr;							// ���_�o�b�t�@
	m_nTexIdx = 0;									// �e�N�X�`���̃C���f�b�N�X�ԍ�

	m_fTex[0] = D3DXVECTOR2(0.0f, 0.0f);			// �e�N�X�`�����W
	m_fTex[1] = D3DXVECTOR2(1.0f, 0.0f);			// �e�N�X�`�����W
	m_fTex[2] = D3DXVECTOR2(0.0f, 1.0f);			// �e�N�X�`�����W
	m_fTex[3] = D3DXVECTOR2(1.0f, 1.0f);			// �e�N�X�`�����W

	for (int nCntVtx = 4; nCntVtx < 32; nCntVtx++)
	{
		m_fTex[0] = D3DXVECTOR2(0.0f, 0.0f);			// �e�N�X�`�����W
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObject3D::~CObject3D()
{

}

//==========================================================================
// �e�N�X�`���̊��蓖��
//==========================================================================
void CObject3D::BindTexture(int nIdx)
{
	// ���蓖�Ă�
	m_nTexIdx = nIdx;
}

//==========================================================================
// ��������
//==========================================================================
CObject3D* CObject3D::Create(int nPriority)
{
	// �������̊m��
	CObject3D* pObject3D = DEBUG_NEW CObject3D(nPriority);

	if (pObject3D != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		pObject3D->Init();
	}

	return pObject3D;
}

//==========================================================================
// ��������
//==========================================================================
CObject3D* CObject3D::Create(MyLib::Vector3 pos, MyLib::Vector3 rot)
{

	// �������̊m��
	CObject3D* pObject3D = DEBUG_NEW CObject3D;

	if (pObject3D != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		pObject3D->Init();

		// �ʒu�E����
		pObject3D->SetPosition(pos);
		pObject3D->SetOriginPosition(pos);
		pObject3D->SetRotation(rot);
	}

	return pObject3D;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObject3D::Init()
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	if (m_pVtxBuff != nullptr)
	{// ���ɏ�񂪓����Ă�ꍇ

		return E_FAIL;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObject3D::Uninit()
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
void CObject3D::Update()
{
	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObject3D::Draw()
{
	// �}�l�[�W���̃C���X�^���X�擾
	CManager* pMgr = CManager::GetInstance();
	if (GetType() == CObject::TYPE::TYPE_UI && !pMgr->IsDisp_UI())
	{
		return;
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pMgr->GetRenderer()->GetDevice();

	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 rotOrigin = GetOriginRotation();

	// �v�Z�p�}�g���b�N�X�錾
	MyLib::Matrix mtxRot, mtxTrans, mtxRotOrigin;
	mtxRotOrigin.Identity();

	// ���[���h�}�g���b�N�X�̏�����
	m_mtxWorld.Identity();

	// ���̌����𔽉f����
	mtxRotOrigin.RotationYawPitchRoll(rotOrigin.y, rotOrigin.x, rotOrigin.z);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRotOrigin);

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

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nTexIdx));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObject3D::SetVtx()
{

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	MyLib::Vector3 size = GetSize();
	D3DXCOLOR col = GetColor();

	// �ʒu���X�V
	pVtx[0].pos = MyLib::Vector3(-size.x, +size.y, +size.z);
	pVtx[1].pos = MyLib::Vector3(+size.x, +size.y, +size.z);
	pVtx[2].pos = MyLib::Vector3(-size.x, -size.y, -size.z);
	pVtx[3].pos = MyLib::Vector3(+size.x, -size.y, -size.z);

	// ���E���̃x�N�g��
	MyLib::Vector3 vecLine0 = pVtx[1].pos - pVtx[0].pos;
	MyLib::Vector3 vecLine1 = pVtx[2].pos - pVtx[0].pos;

	// �O�ϋ��߂�
	MyLib::Vector3 Nor[4];

	// �O��
	D3DXVec3Cross(&Nor[0], &vecLine0, &vecLine1);

	// ���K��
	D3DXVec3Normalize(&Nor[0], &Nor[0]);

	Nor[0] = MyLib::Vector3(
		((vecLine0.y * vecLine1.z) - (vecLine0.z * vecLine1.y)),
		((vecLine0.z * vecLine1.x) - (vecLine0.x * vecLine1.z)),
		((vecLine0.x * vecLine1.y) - (vecLine0.y * vecLine1.x)));

	// ���K��
	D3DXVec3Normalize(&Nor[0], &Nor[0]);


	// ���E���̃x�N�g��
	vecLine0 = pVtx[2].pos - pVtx[3].pos;
	vecLine1 = pVtx[1].pos - pVtx[3].pos;

	// �O�ϋ��߂�
	Nor[3] = MyLib::Vector3(
		((vecLine0.y * vecLine1.z) - (vecLine0.z * vecLine1.y)),
		((vecLine0.z * vecLine1.x) - (vecLine0.x * vecLine1.z)),
		((vecLine0.x * vecLine1.y) - (vecLine0.y * vecLine1.x)));

	// �O��
	D3DXVec3Cross(&Nor[3], &vecLine0, &vecLine1);

	// ���K��
	D3DXVec3Normalize(&Nor[3], &Nor[3]);

	Nor[1] = (Nor[0] + Nor[3]) / 2.0f;
	Nor[2] = (Nor[0] + Nor[3]) / 2.0f;

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = Nor[0];
	pVtx[1].nor = Nor[1];
	pVtx[2].nor = Nor[2];
	pVtx[3].nor = Nor[3];

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = m_fTex[0];
	pVtx[1].tex = m_fTex[1];
	pVtx[2].tex = m_fTex[2];
	pVtx[3].tex = m_fTex[3];

	// ���_�o�b�t�@���A�����b�N���b�N
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �}�g���b�N�X�ݒ�
//==========================================================================
void CObject3D::SetWorldMtx(const MyLib::Matrix mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// �}�g���b�N�X�擾
//==========================================================================
MyLib::Matrix CObject3D::GetWorldMtx() const
{
	return m_mtxWorld;
}

//==========================================================================
// �F�ݒ�
//==========================================================================
void CObject3D::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}

//==========================================================================
// �F�擾
//==========================================================================
D3DXCOLOR CObject3D::GetColor() const
{
	return m_col;
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CObject3D::SetSize(const MyLib::Vector3& size)
{
	m_fSize = size;
}

//==========================================================================
// �T�C�Y�擾
//==========================================================================
MyLib::Vector3 CObject3D::GetSize() const
{
	return m_fSize;
}

//==========================================================================
// ���̃T�C�Y�ݒ�
//==========================================================================
void CObject3D::SetSizeOrigin(const MyLib::Vector3& size)
{
	m_fSize = size;
}

//==========================================================================
// ���̃T�C�Y�擾
//==========================================================================
MyLib::Vector3 CObject3D::GetSizeOrigin() const
{
	return m_fSize;
}

//==========================================================================
// �e�N�X�`�����W�ݒ�
//==========================================================================
void CObject3D::SetTex(D3DXVECTOR2 *tex)
{
	memcpy(&m_fTex[0], tex, sizeof(m_fTex));
}

//==========================================================================
// �e�N�X�`�����W�擾
//==========================================================================
D3DXVECTOR2 *CObject3D::GetTex()
{
	return &m_fTex[0];
}

//==========================================================================
// �I�u�W�F�N�g3D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject3D *CObject3D::GetObject3D()
{
	return this;
}

//==========================================================================
// �I�u�W�F�N�g3D���b�V���I�u�W�F�N�g�̎擾
//==========================================================================
CObject3DMesh *CObject3D::GetObject3DMesh()
{
	return nullptr;
}