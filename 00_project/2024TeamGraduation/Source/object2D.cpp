//=============================================================================
// 
//  �I�u�W�F�N�g2D���� [object2D.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "object2D.h"
#include "manager.h"
#include "pause.h"
#include "2D_effect.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int DEF_VTXNUM = 4;	// �f�t�H���g�̒��_��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObject2D::CObject2D(int nPriority, const LAYER layer) : CObject(nPriority, layer)
{

	m_col = MyLib::color::White();			// �F
	m_colOrigin = MyLib::color::White();	// ���̐F
	m_size = D3DXVECTOR2(0.0f, 0.0f);		// �T�C�Y
	m_sizeOrigin = D3DXVECTOR2(0.0f, 0.0f);	// ���̃T�C�Y
	m_AnchorType = AnchorPoint::CENTER;		// �A���J�[�|�C���g�̎��
	m_fLength = 0.0f;						// �Ίp���̒���
	m_fAngle = 0.0f;						// �Ίp���̌���
	m_nTexIdx = 0;							// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nNumVertex = DEF_VTXNUM;				// ���_��
	m_pVtxBuff = nullptr;					// ���_�o�b�t�@
	m_vecUV.clear();						// �e�N�X�`�����W
	m_vecVtxPos.clear();					// ���_���W

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObject2D::~CObject2D()
{

}

//==========================================================================
// ��������
//==========================================================================
CObject2D* CObject2D::Create(int nPriority, int nNumVtx)
{
	// �������m��
	CObject2D* pObject2D = DEBUG_NEW CObject2D(nPriority);

	if (pObject2D != nullptr)
	{
		// �������ݒ�
		pObject2D->m_vecVtxPos.resize(nNumVtx);
		pObject2D->m_vecUV.resize(nNumVtx);

		// ����������
		pObject2D->Init();
	}

	return pObject2D;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObject2D::Init()
{
	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (m_vecVtxPos.empty())	{ m_vecVtxPos.resize(DEF_VTXNUM); }
	if (m_vecUV.empty())		{ m_vecUV.resize(DEF_VTXNUM); }

	if ((int)m_vecUV.size() >= DEF_VTXNUM)
	{// ���T�C�Y�ς�

		// �e�N�X�`�����W
		m_vecUV[0] = D3DXVECTOR2(0.0f, 0.0f);
		m_vecUV[1] = D3DXVECTOR2(1.0f, 0.0f);
		m_vecUV[2] = D3DXVECTOR2(0.0f, 1.0f);
		m_vecUV[3] = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���_�o�b�t�@�̐���
	if (m_pVtxBuff != nullptr) return E_FAIL;

	HRESULT hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * DEF_VTXNUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (FAILED(hr)) return E_FAIL;	// ���s

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObject2D::Init(int nNumVertex)
{
	// ���_��
	m_nNumVertex = nNumVertex;

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (m_vecVtxPos.empty()) { m_vecVtxPos.resize(nNumVertex); }
	if (m_vecUV.empty()) { m_vecUV.resize(nNumVertex); }

	if ((int)m_vecUV.size() >= nNumVertex)
	{// ���T�C�Y�ς�

		// �e�N�X�`�����W
		m_vecUV[0] = D3DXVECTOR2(0.0f, 0.0f);
		m_vecUV[1] = D3DXVECTOR2(1.0f, 0.0f);
		m_vecUV[2] = D3DXVECTOR2(0.0f, 1.0f);
		m_vecUV[3] = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���_�o�b�t�@�̐���
	if (m_pVtxBuff != nullptr) return E_FAIL;

	HRESULT hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (FAILED(hr)) return E_FAIL;	// ���s

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObject2D::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//==========================================================================
// ���I�폜����
//==========================================================================
void CObject2D::Kill()
{
	// �I������
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObject2D::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObject2D::Draw()
{

	// �}�l�[�W���̃C���X�^���X�擾
	CManager* pMgr = CManager::GetInstance();

#if 0
#if _DEBUG
	
	if ((pMgr->GetPause()->IsPause() || pMgr->Is2DDisp()) &&
		(!GetType() == CObject::TYPE::TYPE_UI || !GetType() == CObject::TYPE::TYPE_NONE))
	{
		return;
	}
#endif
#endif

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pMgr->GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nTexIdx));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVertex - 2);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObject2D::SetVtx()
{
	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	D3DXCOLOR col = GetColor();

	// �A���J�[�|�C���g�̐ݒ�
	float anchorX = pos.x;
	float anchorY = pos.y;

	D3DXVECTOR2 size = GetSize();

	// �Z���^�[�ȊO�͌v�Z
	if (m_AnchorType != AnchorPoint::CENTER)
	{
		float distanceX = size.x, distanceY = size.y;
		switch (m_AnchorType)
		{
		case CObject2D::LEFT:
			anchorX = pos.x + distanceX;
			anchorY = pos.y;
			break;

		case CObject2D::TOP_LEFT:
			anchorX = pos.x + distanceX;
			anchorY = pos.y + distanceY;
			break;

		case CObject2D::TOP_CENTER:
			anchorX = pos.x;
			anchorY = pos.y + distanceY;
			break;

		case CObject2D::TOP_RIGHT:
			anchorX = pos.x - distanceX;
			anchorY = pos.y + distanceY;
			break;

		case CObject2D::RIGHT:
			anchorX = pos.x - distanceX;
			anchorY = pos.y;
			break;

		case CObject2D::UNDER_RIGHT:
			anchorX = pos.x - distanceX;
			anchorY = pos.y - distanceY;
			break;

		case CObject2D::UNDER_CENTER:
			anchorX = pos.x;
			anchorY = pos.y - distanceY;
			break;

		case CObject2D::UNDER_LEFT:
			anchorX = pos.x + distanceX;
			anchorY = pos.y - distanceY;
			break;

		default:
			anchorX = pos.x;
			anchorY = pos.y;
			break;
		}
	}

	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	if (m_AnchorType == AnchorPoint::CENTER)
	{
		float fAngle = GetAngle();
		float fLength = GetLength();

		pVtx[0].pos.x = anchorX + sinf(rot.z - D3DX_PI + fAngle) * fLength;
		pVtx[0].pos.y = anchorY + cosf(rot.z - D3DX_PI + fAngle) * fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = anchorX + sinf(rot.z + D3DX_PI - fAngle) * fLength;
		pVtx[1].pos.y = anchorY + cosf(rot.z + D3DX_PI - fAngle) * fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = anchorX + sinf(rot.z - fAngle) * fLength;
		pVtx[2].pos.y = anchorY + cosf(rot.z - fAngle) * fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = anchorX + sinf(rot.z + fAngle) * fLength;
		pVtx[3].pos.y = anchorY + cosf(rot.z + fAngle) * fLength;
		pVtx[3].pos.z = 0.0f;
	}
	else
	{
		pVtx[0].pos = RotateVtx(MyLib::Vector3(anchorX - size.x, anchorY - size.y, 0.0f), pos);
		pVtx[1].pos = RotateVtx(MyLib::Vector3(anchorX + size.x, anchorY - size.y, 0.0f), pos);
		pVtx[2].pos = RotateVtx(MyLib::Vector3(anchorX - size.x, anchorY + size.y, 0.0f), pos);
		pVtx[3].pos = RotateVtx(MyLib::Vector3(anchorX + size.x, anchorY + size.y, 0.0f), pos);

		pVtx[0].pos.z = pVtx[1].pos.z = pVtx[2].pos.z = pVtx[3].pos.z = 0.0f;
	}

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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
// ���_��]����
//==========================================================================
MyLib::Vector3 CObject2D::RotateVtx(const MyLib::Vector3& vtx, const MyLib::Vector3& center)
{
	// �v�Z�p�}�g���b�N�X
	MyLib::Matrix mtxRot;
	MyLib::Matrix mtxWepon;
	mtxWepon.Identity();

	// ���擾
	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 pos = GetPosition();

	// �����𔽉f����
	mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
	mtxWepon.Multiply(mtxWepon, mtxRot);

	// �I�t�Z�b�g�𔽉f����
	MyLib::Vector3 offset(vtx.x - pos.x, vtx.y - pos.y, 0.0f);

	// �I�t�Z�b�g�ɉ�]�s���K�p
	offset = mtxRot.Coord(offset);

	return (offset + center);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void  CObject2D::SetVtx(int nNumVertex)
{
	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	D3DXCOLOR col = GetColor();

	// ���_���W�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
	{
		pVtx[nCntVtx].pos.x = pos.x + m_vecVtxPos[nCntVtx].x;
		pVtx[nCntVtx].pos.y = pos.y + m_vecVtxPos[nCntVtx].y;
		pVtx[nCntVtx].pos.z = 0.0f;

		// rhw�̐ݒ�
		pVtx[nCntVtx].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[nCntVtx].col = col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[nCntVtx].tex = m_vecUV[nCntVtx];
	}

	// ���_�o�b�t�@���A�����b�N���b�N
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CObject2D::SetSize(const D3DXVECTOR2& size)
{
	m_size = size;		// �T�C�Y
	SetLength(sqrtf(m_size.x * m_size.x + m_size.y * m_size.y));	// �Ίp���̒���
	SetAngle(atan2f(m_size.x, m_size.y));							// �Ίp���̌���
}

//==========================================================================
// �e�N�X�`�����W�̐ݒ�
//==========================================================================
void CObject2D::SetTexUV(const std::vector<D3DXVECTOR2>& uv)
{
	m_vecUV = uv;

	// ���_���ݒ�
	SetVtx();
}
