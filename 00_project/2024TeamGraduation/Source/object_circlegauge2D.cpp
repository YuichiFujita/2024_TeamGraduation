//=============================================================================
// 
//  �I�u�W�F�N�g�~�Q�[�W(2D)���� [object_circlegauge2D.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "object_circlegauge2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "calculation.h"
#include "debugproc.h"

namespace
{
	int MIN_POLYGON = 4;	// �ŏ����p�`
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObjectCircleGauge2D::CObjectCircleGauge2D(int nPriority, const LAYER layer) : CObject(nPriority, layer)
{
	m_col = mylib_const::DEFAULT_COLOR;	// �F
	m_fRate = 0.0f;		// ����
	m_fRateDest = 0.0f;	// �ڕW�̊���
	m_fSize = 0.0f;		// �T�C�Y
	m_nNumVertex = 0;	// ���_��
	m_nTexIdx = 0;						// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_pVtxBuff = nullptr;					// ���_�o�b�t�@

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObjectCircleGauge2D::~CObjectCircleGauge2D()
{

}

//==========================================================================
// �e�N�X�`���̊��蓖��
//==========================================================================
void CObjectCircleGauge2D::BindTexture(int nIdx)
{
	// ���蓖�Ă�
	m_nTexIdx = nIdx;
}

//==========================================================================
// ��������
//==========================================================================
CObjectCircleGauge2D *CObjectCircleGauge2D::Create(int nPolygon, float fSize)
{
	// �����p�̃I�u�W�F�N�g
	CObjectCircleGauge2D *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{// nullptr��������

		if (nPolygon < MIN_POLYGON)
		{// �l�p�`�����͉~�ɂȂ�Ȃ����甲����
			return nullptr;
		}

		// �������̊m��
		pObject2D = DEBUG_NEW CObjectCircleGauge2D;

		if (pObject2D != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// ���_��
			pObject2D->m_nNumVertex = nPolygon + 2;

			// �T�C�Y
			pObject2D->m_fSize = fSize;

			// ����������
			pObject2D->Init();
		}

		return pObject2D;
	}

	return nullptr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectCircleGauge2D::Init()
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	if (m_pVtxBuff != nullptr)
	{// ���ɏ�񂪓����Ă�ꍇ
		return E_FAIL;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
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
void CObjectCircleGauge2D::Uninit()
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
void CObjectCircleGauge2D::Update()
{
	// �����X�V
	m_fRate += (m_fRateDest - m_fRate) * 0.15f;

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectCircleGauge2D::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nTexIdx));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_nNumVertex - 2);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObjectCircleGauge2D::SetVtx()
{

	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	// ���_���W�̐ݒ�
	pVtx[0].pos = pos;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = mylib_const::DEFAULT_VECTOR2;
	pVtx += 1;

	// �~�Q�[�W�v�Z�p�̕ϐ�
	bool bEnd = false;	// �I�[����
	float fRotDivision = (D3DX_PI * 2.0f) / (m_nNumVertex - 2);	// �������̌���
	float fOldRot = m_fRate * (D3DX_PI * 2.0f);	// �O��̌���
	float fEndRot = 0.0f;	// �Ō�̌���
	MyLib::Vector3 EndPos = mylib_const::DEFAULT_VECTOR3;

	// ���_���W�̐ݒ�
	for (int i = 0; i < m_nNumVertex - 1; i++)
	{
		// ����̌���
		float fRot = fRotDivision * i;
		
		if (bEnd == false)
		{
			// �O��̃C���f�b�N�X�ԍ�
			int nIdx = i - 1;
			UtilFunc::Transformation::ValueNormalize(nIdx, m_nNumVertex, 0);

			// ���񕪂̌���
			float fRotDiff = (fRotDivision * i) - (fRotDivision * nIdx);

			// ���܂ł̕����獡�񕪈���
			fOldRot -= fRotDiff;

			if (fOldRot < 0.0f)
			{
				// �Ō�̌�������o��
				fEndRot = (fRotDivision * i) + fOldRot;

				// ����
				float fRate = (fEndRot - (fRotDivision * nIdx)) / fRotDivision;

				// �O��̒��_�ƍ���̒��_
				MyLib::Vector3 p1 = MyLib::Vector3(
					pos.x + cosf(D3DX_PI * -0.5f + (fRotDivision * nIdx) + rot.z) * m_fSize,
					pos.y + sinf(D3DX_PI * -0.5f + (fRotDivision * nIdx) + rot.z) * m_fSize,
					0.0f);

				MyLib::Vector3 p2 = MyLib::Vector3(
					pos.x + cosf(D3DX_PI * -0.5f + (fRotDivision * i) + rot.z) * m_fSize,
					pos.y + sinf(D3DX_PI * -0.5f + (fRotDivision * i) + rot.z) * m_fSize,
					0.0f);

				// �ӏ�̈ʒu�擾(2D)
				EndPos = UtilFunc::Calculation::GetPointOnEdge2D(p1, p2, fRate);

				// �I�[���
				bEnd = true;
			}
		}

		if (bEnd == false)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = MyLib::Vector3(
				pos.x + cosf(D3DX_PI * -0.5f + fRot + rot.z) * m_fSize,
				pos.y + sinf(D3DX_PI * -0.5f + fRot + rot.z) * m_fSize,
				0.0f);
		}
		else
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = EndPos;
		}

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = m_col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = mylib_const::DEFAULT_VECTOR2;

		pVtx += 1;
	}

	// ���_�o�b�t�@���A�����b�N���b�N
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �F�ݒ�
//==========================================================================
void CObjectCircleGauge2D::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}

//==========================================================================
// �F�擾
//==========================================================================
D3DXCOLOR CObjectCircleGauge2D::GetColor() const
{
	return m_col;
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CObjectCircleGauge2D::SetRate(const float fRate)
{
	m_fRate = fRate;
}

//==========================================================================
// �����擾
//==========================================================================
float CObjectCircleGauge2D::GetRate() const
{
	return m_fRate;
}

//==========================================================================
// �ڕW�̊����ݒ�
//==========================================================================
void CObjectCircleGauge2D::SetRateDest(const float fRate)
{
	m_fRateDest = fRate;
}

//==========================================================================
// �ڕW�̊����擾
//==========================================================================
float CObjectCircleGauge2D::GetRateDest() const
{
	return m_fRateDest;
}
