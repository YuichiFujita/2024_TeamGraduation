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
	m_size = D3DXVECTOR2(0.0f, 0.0f);			// �T�C�Y
	m_sizeOrigin = D3DXVECTOR2(0.0f, 0.0f);		// ���̃T�C�Y
	m_fLength = 0.0f;							// �Ίp���̒���
	m_fAngle = 0.0f;							// �Ίp���̌���
	m_nTexIdx = 0;	// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_pVtxBuff = nullptr;		// ���_�o�b�t�@
	m_vecUV.clear();			// �e�N�X�`�����W
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObjectBillboard::~CObjectBillboard()
{

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

	// �e�N�X�`�����W
	m_vecUV.resize(POLYGON_TOP);
	m_vecUV[0] = D3DXVECTOR2(0.0f, 0.0f);
	m_vecUV[1] = D3DXVECTOR2(1.0f, 0.0f);
	m_vecUV[2] = D3DXVECTOR2(0.0f, 1.0f);
	m_vecUV[3] = D3DXVECTOR2(1.0f, 1.0f);

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
void CObjectBillboard::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
	pVtx[0].tex = m_vecUV[0];
	pVtx[1].tex = m_vecUV[1];
	pVtx[2].tex = m_vecUV[2];
	pVtx[3].tex = m_vecUV[3];

	// ���_�o�b�t�@���A�����b�N���b�N
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CObjectBillboard::SetSize(const D3DXVECTOR2& size)
{
	m_size = size;		// �T�C�Y
	SetLength(sqrtf(m_size.x * m_size.x + m_size.y * m_size.y));	// �Ίp���̒���
	SetAngle(atan2f(m_size.x, m_size.y));							// �Ίp���̌���
}