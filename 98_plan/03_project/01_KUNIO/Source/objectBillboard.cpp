//=============================================================================
// 
//  �r���{�[�h���� [objectBillboard.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "objectBillboard.h"
#include "manager.h"

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
CObjectBillboard::CObjectBillboard(int nPriority) : CObject(nPriority)
{
	m_mtxWorld.Identity();			// ���[���h�}�g���b�N�X
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	m_fSize = D3DXVECTOR2(0.0f, 0.0f);			// �T�C�Y
	m_sizeOrigin = D3DXVECTOR2(0.0f, 0.0f);		// ���̃T�C�Y
	m_fLength = 0.0f;							// �Ίp���̒���
	m_fAngle = 0.0f;							// �Ίp���̌���
	m_nTexIdx = 0;	// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_pVtxBuff = nullptr;		// ���_�o�b�t�@

	for (int nCntVtx = 0; nCntVtx < 32; nCntVtx++)
	{
		m_fTex[nCntVtx] = D3DXVECTOR2(0.0f, 0.0f);			// �e�N�X�`�����W
	}

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObjectBillboard::~CObjectBillboard()
{

}

//==========================================================================
// �e�N�X�`���̊��蓖��
//==========================================================================
void CObjectBillboard::BindTexture(int nIdx)
{
	// ���蓖�Ă�
	m_nTexIdx = nIdx;
}


//==========================================================================
// ��������
//==========================================================================
CObjectBillboard* CObjectBillboard::Create(int nPriority)
{
	// �������̊m��
	CObjectBillboard* pObjBillBoard = DEBUG_NEW CObjectBillboard(nPriority);

	if (pObjBillBoard != nullptr)
	{
		// ����������
		pObjBillBoard->Init();
	}

	return pObjBillBoard;
}

//==========================================================================
// ��������
//==========================================================================
CObjectBillboard* CObjectBillboard::Create(const MyLib::Vector3& pos, const MyLib::Vector3& rot)
{
	// �������̊m��
	CObjectBillboard* pObjBillBoard = DEBUG_NEW CObjectBillboard;

	if (pObjBillBoard != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �ʒu�E����
		pObjBillBoard->SetPosition(pos);
		pObjBillBoard->SetRotation(rot);

		// ����������
		pObjBillBoard->Init();
	}

	return pObjBillBoard;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectBillboard::Init()
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

	m_fTex[0] = D3DXVECTOR2(0.0f, 0.0f);	// �e�N�X�`�����W
	m_fTex[1] = D3DXVECTOR2(1.0f, 0.0f);	// �e�N�X�`�����W
	m_fTex[2] = D3DXVECTOR2(0.0f, 1.0f);	// �e�N�X�`�����W
	m_fTex[3] = D3DXVECTOR2(1.0f, 1.0f);	// �e�N�X�`�����W

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObjectBillboard::Uninit()
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
void CObjectBillboard::Update()
{
	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectBillboard::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �v�Z�p�}�g���b�N�X�錾
	MyLib::Matrix mtxRot, mtxTrans;

	// ���擾
	MyLib::Vector3 pos = GetPosition();

	// ���[���h�}�g���b�N�X�̏�����
	m_mtxWorld.Identity();

	// �r���[�}�g���b�N�X���擾
	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMATRIX mtxInvert;
	D3DXMatrixInverse(&mtxInvert, nullptr, &mtxView);	// �t�z������߂�

	m_mtxWorld = mtxInvert;

#if 0

	// X���ɉ����Ă�
	m_mtxWorld._12 = 0.0f;
	m_mtxWorld._13 = 0.0f;
	m_mtxWorld._14 = 0.0f;

#else

	// ��ŏ�����
	/*m_mtxWorld._21 = 0.0f;
	m_mtxWorld._23 = 0.0f;
	m_mtxWorld._24 = 0.0f;*/

#endif

	// �ʏ�
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

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

	// �r���{�[�h�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObjectBillboard::SetVtx()
{

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR2 size = GetSize();
	D3DXCOLOR col = GetColor();
	MyLib::Vector3 rot = GetRotation();

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = sinf(rot.z - m_fAngle) * m_fLength;
	pVtx[0].pos.y = cosf(rot.z - m_fAngle) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = sinf(rot.z + m_fAngle) * m_fLength;
	pVtx[1].pos.y = cosf(rot.z + m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = sinf(rot.z - D3DX_PI + m_fAngle) * m_fLength;
	pVtx[2].pos.y = cosf(rot.z - D3DX_PI + m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = sinf(rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[3].pos.y = cosf(rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;


	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);

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
// �����ݒ�
//==========================================================================
void CObjectBillboard::SetRotation(const MyLib::Vector3& rot)
{
	// �����ݒ�
	CObject::SetRotation(rot);
	m_fLength = sqrtf(m_fSize.x * m_fSize.x + m_fSize.y * m_fSize.y);	// �Ίp���̒���
	m_fAngle = atan2f(m_fSize.x, m_fSize.y);							// �Ίp���̌���
}

//==========================================================================
// �}�g���b�N�X�ݒ�
//==========================================================================
void CObjectBillboard::SetWorldMtx(const MyLib::Matrix mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// �}�g���b�N�X�擾
//==========================================================================
MyLib::Matrix CObjectBillboard::GetWorldMtx() const
{
	return m_mtxWorld;
}

//==========================================================================
// �F�ݒ�
//==========================================================================
void CObjectBillboard::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}

//==========================================================================
// �F�擾
//==========================================================================
D3DXCOLOR CObjectBillboard::GetColor() const
{
	return m_col;
}

//==========================================================================
// �s�����x�ݒ�
//==========================================================================
void CObjectBillboard::SetAlpha(const float alpha)
{
	m_col.a = alpha;
}

//==========================================================================
// �s�����x�擾
//==========================================================================
float CObjectBillboard::GetAlpha() const
{
	return m_col.a;
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CObjectBillboard::SetSize(const D3DXVECTOR2 size)
{
	m_fSize = size;
	m_fLength = sqrtf(m_fSize.x * m_fSize.x + m_fSize.y * m_fSize.y);	// �Ίp���̒���
	m_fAngle = atan2f(m_fSize.x, m_fSize.y);							// �Ίp���̌���
}

//==========================================================================
// �T�C�Y�擾
//==========================================================================
D3DXVECTOR2 CObjectBillboard::GetSize() const
{
	return m_fSize;
}

//==========================================================================
// ���̃T�C�Y�̐ݒ�
//==========================================================================
void CObjectBillboard::SetSizeOrigin(const D3DXVECTOR2 size)
{
	m_sizeOrigin = size;
}

//==========================================================================
// ���̃T�C�Y�̎擾
//==========================================================================
D3DXVECTOR2 CObjectBillboard::GetSizeOrigin() const
{
	return m_sizeOrigin;
}

//==========================================================================
// �e�N�X�`�����W�ݒ�
//==========================================================================
void CObjectBillboard::SetTex(D3DXVECTOR2 *tex)
{
	memcpy(&m_fTex[0], tex, sizeof(m_fTex));
}

//==========================================================================
// �e�N�X�`�����W�擾
//==========================================================================
D3DXVECTOR2 *CObjectBillboard::GetTex()
{
	return &m_fTex[0];
}

//==========================================================================
// ���_�o�b�t�@�̎擾
//==========================================================================
LPDIRECT3DVERTEXBUFFER9 CObjectBillboard::GetVtxBuff() const
{
	return m_pVtxBuff;
}

//==========================================================================
// �r���{�[�h�I�u�W�F�N�g�̎擾
//==========================================================================
CObjectBillboard *CObjectBillboard::GetObjectBillBoard()
{
	return this;
}
