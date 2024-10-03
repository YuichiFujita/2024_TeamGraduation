//=============================================================================
// 
//  �I�u�W�F�N�g3D�A�j������ [object3D_Anim.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "object3D_Anim.h"
#include "calculation.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObject3DAnim::CObject3DAnim(int nPriority) : CObject3D(nPriority)
{
	m_nCntAnim = 0;			// �A�j���[�V�����̃J�E���^�[
	m_nPatternAnim = 0;		// �A�j���[�V�����̃p�^�[��
	m_nDivisionU = 0;		// U�̕�����
	m_nDivisionV = 0;		// V�̕�����
	m_nIntervalAnim = 0;	// �A�j���[�V�����̃C���^�[�o��
	m_fSplitValueU = 0.0f;	// U�̃X�v���C�g��
	m_fSplitValueV = 0.0f;	// V�̃X�v���C�g��
	m_bAutoDeath = false;	// �����폜�̃t���O
	m_bFinish = false;		// �A�j���[�V�������I���������
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObject3DAnim::~CObject3DAnim()
{

}

//==========================================================================
// ��������
//==========================================================================
CObject3DAnim *CObject3DAnim::Create(MyLib::Vector3 pos, const int nDivisionU, const int nDivisionV, const int nInterval, bool bAutoDeath)
{
	// �����p�̃I�u�W�F�N�g
	CObject3DAnim *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{// nullptr��������

		// �������̊m��
		pObject3D = DEBUG_NEW CObject3DAnim;

		if (pObject3D != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// �������
			pObject3D->SetPosition(pos);
			pObject3D->SetOriginPosition(pos);
			pObject3D->m_nDivisionU = nDivisionU;
			pObject3D->m_nDivisionV = nDivisionV;
			pObject3D->m_nIntervalAnim = nInterval;
			pObject3D->m_bAutoDeath = bAutoDeath;

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
HRESULT CObject3DAnim::Init()
{
	// ����������
	HRESULT hr = CObject3D::Init();
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// �X�v���C�g��
	m_fSplitValueU = 1.0f / static_cast<float>(m_nDivisionU);
	m_fSplitValueV = 1.0f / static_cast<float>(m_nDivisionV);

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObject3DAnim::Init(const int nDivisionU, const int nDivisionV, const int nInterval)
{
	// �������
	m_nDivisionU = nDivisionU;
	m_nDivisionV = nDivisionV;
	m_nIntervalAnim = nInterval;

	// ����������
	HRESULT hr = CObject3D::Init();
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// �X�v���C�g��
	m_fSplitValueU = 1.0f / static_cast<float>(m_nDivisionU);
	m_fSplitValueV = 1.0f / static_cast<float>(m_nDivisionV);

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// �X�V����
//==========================================================================
void CObject3DAnim::Update()
{

	// �J�E���g���X�V
	m_nCntAnim = (m_nCntAnim + 1) % m_nIntervalAnim;

	// �p�^�[���X�V
	if (m_nCntAnim == 0)
	{
		// �p�^�[��No.���X�V
		m_nPatternAnim = (m_nPatternAnim + 1) % (m_nDivisionU * m_nDivisionV);

		if (m_nPatternAnim == 0)
		{// �p�^�[�������������

			// �I�����
			m_bFinish = true;

			if (m_bAutoDeath)
			{
				// �I�u�W�F�N�g�j��
				Uninit();
				return;
			}
		}
	}

	// �X�V����
	CObject3D::Update();
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObject3DAnim::SetVtx()
{
	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���E���̃x�N�g��
	MyLib::Vector3 vecLine0 = pVtx[1].pos - pVtx[0].pos;
	MyLib::Vector3 vecLine1 = pVtx[2].pos - pVtx[0].pos;

	// �O�ϋ��߂�
	MyLib::Vector3 Nor[4];

	// �O��
	D3DXVec3Cross(&Nor[0], &vecLine0, &vecLine1);
	D3DXVec3Normalize(&Nor[0], &Nor[0]);

	Nor[0] = MyLib::Vector3(
		((vecLine0.y * vecLine1.z) - (vecLine0.z * vecLine1.y)),
		((vecLine0.z * vecLine1.x) - (vecLine0.x * vecLine1.z)),
		((vecLine0.x * vecLine1.y) - (vecLine0.y * vecLine1.x)));
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
	D3DXVec3Normalize(&Nor[3], &Nor[3]);

	Nor[1] = (Nor[0] + Nor[3]) / 2.0f;
	Nor[2] = (Nor[0] + Nor[3]) / 2.0f;

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = Nor[0];
	pVtx[1].nor = Nor[1];
	pVtx[2].nor = Nor[2];
	pVtx[3].nor = Nor[3];

	MyLib::Vector3 size = GetSize();
	D3DXCOLOR col = GetColor();

	// �ʒu���X�V
	pVtx[0].pos = MyLib::Vector3(-size.x, +size.y, +size.z);
	pVtx[1].pos = MyLib::Vector3(+size.x, +size.y, +size.z);
	pVtx[2].pos = MyLib::Vector3(-size.x, -size.y, -size.z);
	pVtx[3].pos = MyLib::Vector3(+size.x, -size.y, -size.z);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * m_fSplitValueU,			(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV);
	pVtx[1].tex = D3DXVECTOR2((m_nPatternAnim + 1) * m_fSplitValueU,	(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV);
	pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * m_fSplitValueU,			(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV + m_fSplitValueV);
	pVtx[3].tex = D3DXVECTOR2((m_nPatternAnim + 1) * m_fSplitValueU,	(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV + m_fSplitValueV);

	// ���_�o�b�t�@���A�����b�N���b�N
	m_pVtxBuff->Unlock();
}
