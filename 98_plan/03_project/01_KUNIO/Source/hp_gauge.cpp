//=============================================================================
// 
//  �̗̓Q�[�W���� [hp_gauge.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "hp_gauge.h"
#include "manager.h"
#include "hp_gauge_tip.h"
#include "camera.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define WIDTH			(35.0f)	// ����
#define HEIGHT			(5.0f)	// �c��

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CHP_Gauge::m_apTextureFile[] =					// �e�N�X�`���̃t�@�C��
{
	"data\\TEXTURE\\hpgauge\\black.png",
	"data\\TEXTURE\\hpgauge\\hpgauge.png",
};
int CHP_Gauge::m_nNumAll = 0;	// ����

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CHP_Gauge::CHP_Gauge(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		m_HPGauge[nCntGauge].pObjBillboard = nullptr;
		m_HPGauge[nCntGauge].fMaxWidth = 0.0f;		// ���̍ő�l
		m_HPGauge[nCntGauge].fMaxHeight = 0.0f;		// �����̍ő�l
		m_HPGauge[nCntGauge].fWidthDest = 0.0f;		// ���̍���
		m_nTexIdx[nCntGauge] = 0;					// �e�N�X�`���̃C���f�b�N�X�ԍ�
	}

	m_nLife = 0;	// �̗�
	m_nMaxLife = 0;	// �ő�̗�

	// �������Z
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CHP_Gauge::~CHP_Gauge()
{

}

//==========================================================================
// ��������
//==========================================================================
CHP_Gauge* CHP_Gauge::Create(float fPosLen, int nMaxLife, float fSizeBuff)
{
	// �����p�̃I�u�W�F�N�g
	CHP_Gauge* pHPGauge = nullptr;

	// �������̊m��
	pHPGauge = DEBUG_NEW CHP_Gauge;

	if (pHPGauge != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ���_����̒�����n��
		pHPGauge->m_fPosLength = fPosLen;

		// �ő�̗�
		pHPGauge->m_nMaxLife = nMaxLife;

		for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
		{
			// ��������
			pHPGauge->m_HPGauge[nCntGauge].pObjBillboard =
				pHPGauge->m_HPGauge[nCntGauge].pObjBillboard->Create();

			if (pHPGauge->m_HPGauge[nCntGauge].pObjBillboard == nullptr)
			{// nullptr��������
				pHPGauge->Uninit();
				return nullptr;
			}

			pHPGauge->m_HPGauge[nCntGauge].pObjBillboard->SetSize(
				D3DXVECTOR2(WIDTH * fSizeBuff, HEIGHT * fSizeBuff));	// �T�C�Y
		}

		// ����������
		pHPGauge->Init();

		// ��ނ̐ݒ�
		pHPGauge->SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);
	}

	return pHPGauge;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CHP_Gauge::Init()
{
	// �ő�̗�
	m_nLife = m_nMaxLife;

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{

		// �e��ϐ��̏�����
		m_HPGauge[nCntGauge].fMaxWidth = m_HPGauge[nCntGauge].pObjBillboard->GetSize().x;		// ���̍ő�l
		m_HPGauge[nCntGauge].fMaxHeight = m_HPGauge[nCntGauge].pObjBillboard->GetSize().y;		// �����̍ő�l

		// �e�N�X�`���̊��蓖��
		m_nTexIdx[nCntGauge] = CTexture::GetInstance()->Regist(m_apTextureFile[nCntGauge]);

		// �e�N�X�`���̊��蓖��
		m_HPGauge[nCntGauge].pObjBillboard->BindTexture(m_nTexIdx[nCntGauge]);
	}

	// ��[����
	m_pTip = CHPGaugeTip::Create(GetPosition() - MyLib::Vector3(WIDTH, 0.0f, 0.0f), GetPosition() + MyLib::Vector3(WIDTH, 0.0f, 0.0f), CHPGaugeTip::TYPE::TYPE_BILLBOARD);
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CHP_Gauge::Uninit()
{
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != nullptr)
		{// nullptr����Ȃ�������

			// �I������
			m_HPGauge[nCntGauge].pObjBillboard->Uninit();
			m_HPGauge[nCntGauge].pObjBillboard = nullptr;
		}
	}

	if (m_pTip != nullptr)
	{
		m_pTip->Uninit();
		m_pTip = nullptr;
	}

	// ���폜
	Release();

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != nullptr)
		{// nullptr����Ȃ�������

			// �I������
			m_HPGauge[nCntGauge].pObjBillboard = nullptr;
		}
	}

	m_nNumAll--;
}


//==========================================================================
// �I������
//==========================================================================
void CHP_Gauge::Kill()
{
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != nullptr)
		{// nullptr����Ȃ�������

			// �I������
			m_HPGauge[nCntGauge].pObjBillboard->Uninit();
			m_HPGauge[nCntGauge].pObjBillboard = nullptr;
		}
	}

	if (m_pTip != nullptr)
	{
		m_pTip->Kill();
		m_pTip = nullptr;
	}

	// ���폜
	Release();

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != nullptr)
		{// nullptr����Ȃ�������

			// �I������
			m_HPGauge[nCntGauge].pObjBillboard = nullptr;
		}
	}

	m_nNumAll--;
}

//==========================================================================
// �X�V����
//==========================================================================
void CHP_Gauge::Update()
{

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != nullptr)
		{// nullptr����Ȃ�������

			if (nCntGauge == VTXTYPE_PINK)
			{// �Q�[�W���������@

				// ��������
				GaugeDecrement(nCntGauge);
			}

			// ���_���W�ݒ�
			SetVtx(nCntGauge);
		}
	}

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	MyLib::Vector3 cameraRot = CManager::GetInstance()->GetCamera()->GetRotation();

	// ��[
	if (m_pTip != nullptr) {
		MyLib::Vector3 left = pos, right = pos;
		float maxlen = m_HPGauge[0].fMaxWidth;

		left.x = pos.x - sinf(D3DX_PI * 0.5f + cameraRot.y) * maxlen;
		left.z = pos.z - cosf(D3DX_PI * 0.5f + cameraRot.y) * maxlen;
		left.x -= sinf(cameraRot.y) * 3.0f;
		left.z -= cosf(cameraRot.y) * 3.0f;

		right.x = pos.x + sinf(D3DX_PI * 0.5f + cameraRot.y) * maxlen;
		right.z = pos.z + cosf(D3DX_PI * 0.5f + cameraRot.y) * maxlen;
		right.x -= sinf(cameraRot.y) * 3.0f;
		right.z -= cosf(cameraRot.y) * 3.0f;

		m_pTip->SetLeftPosition(left);
		m_pTip->SetRightPosition(right);
	}
}

//==========================================================================
// �ʒu�X�V
//==========================================================================
void CHP_Gauge::UpdatePosition(MyLib::Vector3 pos, int nLife)
{
	// ��������ɂ�����
	pos.y += m_fPosLength;

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != nullptr)
		{// nullptr����Ȃ�������

			// �ʒu�ݒ�
			m_HPGauge[nCntGauge].pObjBillboard->SetPosition(pos);

			// ���݂̗̑͐ݒ�
			m_nLife = nLife;

			//�ڕW�̕��ݒ�
			m_HPGauge[nCntGauge].fWidthDest =
				m_HPGauge[nCntGauge].fMaxWidth * ((float)m_nLife / (float)m_nMaxLife);
		}
	}
	SetPosition(pos);
}

//==========================================================================
// ��������
//==========================================================================
void CHP_Gauge::GaugeDecrement(int nCntGauge)
{
	// �T�C�Y�擾
	D3DXVECTOR2 size = m_HPGauge[nCntGauge].pObjBillboard->GetSize();

	// �����ŏ��X�Ɍ��炵�Ă���
	size.x +=
		(m_HPGauge[nCntGauge].fWidthDest - size.x) * 0.15f;

	// �T�C�Y�ݒ�
	m_HPGauge[nCntGauge].pObjBillboard->SetSize(size);

	// ���_���W�ݒ�
	SetVtx(nCntGauge);

}

//==========================================================================
// �F�X�V
//==========================================================================
void CHP_Gauge::ChangeColor(int nCntGauge)
{
	// �T�C�Y�擾
	D3DXVECTOR2 size = m_HPGauge[nCntGauge].pObjBillboard->GetSize();

	// �F�擾
	D3DXCOLOR col = m_HPGauge[nCntGauge].pObjBillboard->GetColor();

	if (
		size.x / m_HPGauge[nCntGauge].fMaxWidth <= 0.95f &&
		size.x / m_HPGauge[nCntGauge].fMaxWidth > 0.6f)
	{//HP�Q�[�W8��

		col = D3DXCOLOR(0.2f, 0.6f, 0.2f, 1.0f);
	}
	else if (
		size.x / m_HPGauge[nCntGauge].fMaxWidth <= 0.6f &&
		size.x / m_HPGauge[nCntGauge].fMaxWidth > 0.4f)
	{//HP�Q�[�W6��

		col = D3DXCOLOR(0.8f, 0.7f, 0.2f, 1.0f);
	}
	else if (
		size.x / m_HPGauge[nCntGauge].fMaxWidth <= 0.4f &&
		size.x / m_HPGauge[nCntGauge].fMaxWidth > 0.2f)
	{//HP�Q�[�W4��

		col = D3DXCOLOR(0.8f, 0.5f, 0.2f, 1.0f);
	}
	else if (size.x / m_HPGauge[nCntGauge].fMaxWidth <= 0.2f)
	{//�댯�G���A

		col = D3DXCOLOR(0.8f, 0.2f, 0.2f, 1.0f);
	}
	else
	{//���^��

		col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	}

	// �F�ݒ�
	m_HPGauge[nCntGauge].pObjBillboard->SetColor(col);

	// �T�C�Y�擾
	m_HPGauge[nCntGauge].pObjBillboard->SetSize(size);

}

//==========================================================================
// �`�揈��
//==========================================================================
void CHP_Gauge::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		// ���C�e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// �`�揈��
		m_HPGauge[nCntGauge].pObjBillboard->Draw();

		// ���C�e�B���O��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CHP_Gauge::SetVtx()
{

}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CHP_Gauge::SetVtx(int nCntGauge)
{
	// ���_�ݒ�
	m_HPGauge[nCntGauge].pObjBillboard->SetVtx();

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_HPGauge[nCntGauge].pObjBillboard->GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	// �T�C�Y�擾
	D3DXVECTOR2 size = m_HPGauge[nCntGauge].pObjBillboard->GetSize();

	//���_���W�̐ݒ�
	pVtx[0].pos = MyLib::Vector3(-size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x), size.y, 0.0f);
	pVtx[1].pos = MyLib::Vector3(size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x), size.y, 0.0f);
	pVtx[2].pos = MyLib::Vector3(-size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x), -size.y, 0.0f);
	pVtx[3].pos = MyLib::Vector3(size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x), -size.y, 0.0f);

	// ���_�o�b�t�@���A�����b�N���b�N
	m_HPGauge[nCntGauge].pObjBillboard->GetVtxBuff()->Unlock();

}
