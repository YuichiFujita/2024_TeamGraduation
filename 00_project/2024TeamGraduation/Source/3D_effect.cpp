//=============================================================================
// 
// �G�t�F�N�g���� [3D_effect.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "3D_effect.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define DEF_RADIUS			(20.0f)
#define EFFECT_3D_LIFE		(30)
#define EFFECT_3DSIZE1		(0.97f)
#define EFFECT_3DSIZE2		(0.98f)
#define EFFECT_3DSIZE3		(0.99f)

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CEffect3D::m_apTextureFile[] =					// �t�@�C���ǂݍ���
{
	"data\\TEXTURE\\effect\\effect000.jpg",	   // �ʏ�G�t�F�N�g
	"data\\TEXTURE\\effect\\effect_point01.tga",	   // �_�G�t�F�N�g
	"data\\TEXTURE\\effect\\smoke_05.tga",	   // ���G�t�F�N�g
	"data\\TEXTURE\\effect\\smoke_05.tga",	   // ����
	"data\\TEXTURE\\effect\\effect000.png",	   // ���G�t�F�N�g
	"data\\TEXTURE\\effect\\effect001.png",		// �\���G�t�F�N�g
	"data\\TEXTURE\\effect\\Star01.png",		// �\���G�t�F�N�g
	"data\\TEXTURE\\effect\\thunder_02.tga",	// ���G�t�F�N�g
	"data\\TEXTURE\\effect\\water.png",			// ���G�t�F�N�g
	"",											// nullptr�G�t�F�N�g
};
int CEffect3D::m_nNumAll = 0;	// ����
int CEffect3D::m_nTexIdx[TYPE_MAX] = {};	// �e�N�X�`���̃C���f�b�N�X�ԍ�

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEffect3D::CEffect3D(int nPriority) : CObjectBillboard(nPriority)
{
	// �l�̃N���A
	m_posOrigin = mylib_const::DEFAULT_VECTOR3;			// ���_
	m_updatePosition = mylib_const::DEFAULT_VECTOR3;	// �X�V��̈ʒu
	m_setupPosition = mylib_const::DEFAULT_VECTOR3;		// �Z�b�g�A�b�v�ʒu
	m_posDest = mylib_const::DEFAULT_VECTOR3;			// �ڕW�̈ʒu
	m_colOrigin = mylib_const::DEFAULT_COLOR;	// �F�̌�
	m_pMtxParent = nullptr;						// �e�}�g���b�N�X�̃|�C���^
	m_fRadius = 0.0f;							// ���a
	m_fMaxRadius = 0.0f;						// �ő唼�a
	m_fAddSizeValue = 0.0f;						// �T�C�Y�ύX��
	m_fGravity = 0.0f;							// �d��
	m_fMoveFactor = 0.0f;						// �ړ��␳�W��
	m_nLife = 0;								// ����
	m_nMaxLife = 0;								// �ő����(�Œ�)
	m_moveType = MOVEEFFECT_NONE;				// �ړ��̎��
	m_nType = TYPE_NORMAL;						// ���
	m_pParent = nullptr;							// �e�̃|�C���^
	m_bAddAlpha = true;							// ���Z�����̔���
	m_bZSort = false;							// Z�\�[�g�̃t���O
	m_bGravity = false;							// �d�͂̃t���O
	m_bChaseDest = false;						// �ڕW�̈ʒu�֌������t���O

	// �������Z
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEffect3D::~CEffect3D()
{
	
}

//==========================================================================
// �e�N�X�`���ǂݍ���
//==========================================================================
void CEffect3D::LoadTexture()
{
	// �e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < sizeof(m_apTextureFile) / sizeof(*m_apTextureFile); nCntTex++)
	{// �e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
		m_nTexIdx[nCntTex] = CTexture::GetInstance()->Regist(m_apTextureFile[nCntTex]);
	}
}

//==========================================================================
// ��������
//==========================================================================
CEffect3D *CEffect3D::Create()
{
	// �����p�̃I�u�W�F�N�g
	CEffect3D *pEffect = nullptr;

	if (pEffect == nullptr)
	{// nullptr��������

		// �������̊m��
		pEffect = DEBUG_NEW CEffect3D;

		if (pEffect != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pEffect->Init();
		}

		return pEffect;
	}

	return nullptr;
}

//==========================================================================
// ��������
//==========================================================================
CEffect3D* CEffect3D::Create(const MyLib::Vector3& pos, const MyLib::Vector3& move, const D3DXCOLOR& col, const float fRadius, const int nLife, const int moveType, const TYPE type, const float fAddSizeValue)
{
	// �������̊m��
	CEffect3D* pEffect = DEBUG_NEW CEffect3D;

	if (pEffect != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		pEffect->m_fAddSizeValue = fAddSizeValue;	// �T�C�Y�ύX��
		HRESULT hr = pEffect->Init(pos, move, col, fRadius, nLife, moveType, type);
		if (FAILED(hr))
		{
			return nullptr;
		}
	}

	return pEffect;
}

//==========================================================================
// �G�t�F�N�g�̏���������
//==========================================================================
HRESULT CEffect3D::Init()
{
	HRESULT hr;

	// �e��ϐ��̏�����
	m_posOrigin = MyLib::Vector3(0.0f, 0.0f, 0.0f);		// ���_
	m_updatePosition = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// �X�V��̈ʒu
	m_setupPosition = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// �Z�b�g�A�b�v�ʒu
	m_colOrigin = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F�̌�
	m_fRadius = DEF_RADIUS;						// ���a
	m_fMaxRadius = m_fRadius;					// �ő唼�a
	m_nLife = EFFECT_3D_LIFE;					// ����
	m_nMaxLife = m_nLife;						// �ő����(�Œ�)
	m_moveType = MOVEEFFECT_NONE;				// �ړ��̎��
	m_nType = TYPE_NORMAL;						// ���
	m_bAddAlpha = true;							// ���Z�����̔���
	m_fGravity = mylib_const::GRAVITY;

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// ����������
	hr = CObjectBillboard::Init();

	if (FAILED(hr))
	{// ���s�����Ƃ�

		Uninit();
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �G�t�F�N�g�̏���������
//==========================================================================
HRESULT CEffect3D::Init(const MyLib::Vector3& pos, const MyLib::Vector3& move, const D3DXCOLOR& col, const float fRadius, const int nLife, const int moveType, const TYPE type)
{
	HRESULT hr;

	// �e��ϐ��̏�����
	m_posOrigin = pos;							// ���_
	m_updatePosition = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// �X�V��̈ʒu
	m_setupPosition = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// �Z�b�g�A�b�v�ʒu
	SetPosition(pos);							// �ʒu
	SetMove(move);								// �ړ���
	m_colOrigin = col;							// �F�̌�
	SetColor(col);								// �F
	m_fRadius = fRadius;						// ���a
	m_fMaxRadius = m_fRadius;					// �ő唼�a
	SetSize(D3DXVECTOR2(m_fRadius, m_fRadius));	// �T�C�Y�ݒ�
	m_nLife = nLife;							// ����
	m_nMaxLife = m_nLife;						// �ő����(�Œ�)
	m_moveType = moveType;						// �ړ��̎��
	m_nType = type;								// ���

	if (m_nType >= TYPE_MAX)
	{
		int n = 0;
	}

	// ���Z�����̔���
	switch (m_nType)
	{
	case TYPE_NORMAL:
		m_bAddAlpha = true;
		break;

	case TYPE_POINT:
		m_bAddAlpha = true;
		break;

	case TYPE_SMOKE:
		m_bAddAlpha = true;
		break;

	case TYPE_SMOKEBLACK:
		m_bAddAlpha = false;
		break;

	case TYPE_BLACK:
		m_bAddAlpha = false;
		break;

	case TYPE_JUJI:
		m_bAddAlpha = true;
		break;

	case TYPE_JUJI2:
		m_bAddAlpha = true;
		break;

	case TYPE_THUNDER:
		m_bAddAlpha = true;
		break;

	case TYPE_WATER:
		m_bAddAlpha = true;
		break;

	default:
		m_bAddAlpha = true;
		break;
	}

	// Z�\�[�g�̃t���O
	m_bZSort = true;

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �e�N�X�`���̊��蓖��
	if (m_nTexIdx[m_nType] == 0)
	{
		m_nTexIdx[m_nType] = CTexture::GetInstance()->Regist(m_apTextureFile[m_nType]);
	}

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx[m_nType]);

	// ����������
	hr = CObjectBillboard::Init();

	if (FAILED(hr))
	{// ���s�����Ƃ�
		Uninit();
		return E_FAIL;
	}

	// �ړ��␳�W��
	m_fMoveFactor = 0.15f;

	return S_OK;
}

//==========================================================================
// �G�t�F�N�g�̏I������
//==========================================================================
void CEffect3D::Uninit()
{
	if (m_pParent != nullptr)
	{
		// �G�t�F�N�g�̊J��
		m_pParent->ReleaseEffect(m_nParentIdx);
		m_pParent = nullptr;
	}

	// �������Z
	m_nNumAll--;

	// �I������
	CObjectBillboard::Uninit();
}

//==========================================================================
// �e�̔j��
//==========================================================================
void CEffect3D::UninitParent()
{
	// �e��nullptr�ɂ���
	m_pParent = nullptr;
}

//==========================================================================
// �G�t�F�N�g�̍X�V����
//==========================================================================
void CEffect3D::Update()
{
	// �ߋ��̈ʒu�ݒ�
	SetOldPosition(GetPosition());

	// �ړ�����
	UpdateMove();

	switch (m_moveType)
	{
	case MOVEEFFECT_ADD:
		AddSize();
		break;

	case MOVEEFFECT_SUB:
		SubSize();
		break;

	case MOVEEFFECT_SUPERSUB:
		SuperSubSize();
		break;

	case MOVEEFFECT_GENSUI:
		Gensui();
		break;
	}

	// �T�C�Y�ݒ�
	SetSize(D3DXVECTOR2(m_fRadius, m_fRadius));

	// �����̍X�V
	m_nLife--;

	// �F�擾
	D3DXCOLOR col = GetColor();

	// �s�����x�̍X�V
	col.a = m_colOrigin.a * ((float)m_nLife / (float)m_nMaxLife);

	// �F�ݒ�
	SetColor(col);

	if (m_nLife <= 0)
	{// �������s������

		// �G�t�F�N�g�̍폜
		Uninit();
		return;
	}

	// ���_���W�̐ݒ�
	SetVtx();

}

//==========================================================================
// �ړ�����
//==========================================================================
void CEffect3D::UpdateMove()
{
	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 posOld = GetOldPosition();

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �ʒu�X�V
	if (m_bGravity)
	{
		move.y -= m_fGravity;
	}

	// �␳�ʏ���
	if (m_bChaseDest == false)
	{
		m_updatePosition += move;
		pos = m_posOrigin + m_updatePosition;
	}
	else
	{
		// �������`���
		float fRatio = 1.0f - ((float)m_nLife / (float)m_nMaxLife);
		pos.x = UtilFunc::Correction::EasingLinear(m_posOrigin.x, m_posDest.x, fRatio);
		pos.y = UtilFunc::Correction::EasingLinear(m_posOrigin.y, m_posDest.y, fRatio);
		pos.z = UtilFunc::Correction::EasingLinear(m_posOrigin.z, m_posDest.z, fRatio);
	}

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);
}


//==========================================================================
// �Z�b�g�A�b�v
//==========================================================================
void CEffect3D::SetUp(MyLib::Vector3 setup, D3DXMATRIX *pMtxParent, CObject *pObj, int nParentIdx)
{
	// �e�̃|�C���^�n��
	if (m_pParent == nullptr)
	{
		m_pParent = pObj;
	}

	// �e�}�g���b�N�X�̃|�C���^
	m_pMtxParent = pMtxParent;

	// �e�̃C���f�b�N�X�ԍ�
	m_nParentIdx = nParentIdx;

	// �Z�b�g�A�b�v�ʒu
	m_setupPosition = setup;
}

//==========================================================================
// �ʒu�X�V
//==========================================================================
void CEffect3D::UpdatePosition(MyLib::Vector3 rot)
{

	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;	// �v�Z�p�}�g���b�N�X�錾

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, m_setupPosition.x, m_setupPosition.y, m_setupPosition.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// �����ɐe�̃��[���h�}�g���b�N�X���|����
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, m_pMtxParent);

	m_posOrigin = UtilFunc::Transformation::WorldMtxChangeToPosition(mtxWorld);

}

//==========================================================================
// �G�t�F�N�g�̏k������
//==========================================================================
void CEffect3D::SubSize()
{
	int nEffect_3DType = rand() % 3;

	if (nEffect_3DType == 0)
	{
		m_fRadius *= EFFECT_3DSIZE1;
	}
	else if (nEffect_3DType == 1)
	{
		m_fRadius *= EFFECT_3DSIZE2;
	}
	else if (nEffect_3DType == 2)
	{
		m_fRadius *= EFFECT_3DSIZE3;
	}
}

//==========================================================================
// �G�t�F�N�g�̏k������
//==========================================================================
void CEffect3D::SuperSubSize()
{
	m_fRadius = m_fMaxRadius * (float)m_nLife / (float)m_nMaxLife;
}

//==========================================================================
// �G�t�F�N�g�̊g�又��
//==========================================================================
void CEffect3D::AddSize()
{
	// �g��
	m_fRadius += m_fAddSizeValue;
}

//==========================================================================
// �G�t�F�N�g�̌�������
//==========================================================================
void CEffect3D::Gensui()
{
	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	move.x += (0.0f - move.x) * m_fMoveFactor;
	move.y += (0.0f - move.y) * m_fMoveFactor;
	move.z += (0.0f - move.z) * m_fMoveFactor;

	// �ړ��ʐݒ�
	SetMove(move);

	m_fRadius = m_fMaxRadius * (float)m_nLife / (float)m_nMaxLife;

}

//==========================================================================
// �G�t�F�N�g�̕`�揈��
//==========================================================================
void CEffect3D::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (!m_bZSort)
	{
		// Z�e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	// ��ɕ`�悷��
	}

	// ���u�����f�B���O�����Z�����ɐݒ�
	if (m_bAddAlpha)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �`�揈��
	CObjectBillboard::Draw();

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

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
void CEffect3D::SetVtx()
{
	// ���_�ݒ�
	CObjectBillboard::SetVtx();
}

//==========================================================================
// �ڕW�̈ʒu�ݒ�
//==========================================================================
void CEffect3D::SetPositionDest(MyLib::Vector3 pos)
{
	m_posDest = pos;
	m_bChaseDest = true;	// �L��
}

//==========================================================================
// �d�͂̒l�ݒ�
//==========================================================================
void CEffect3D::SetGravityValue(float fValue)
{ 
	m_fGravity = fValue; 
	m_bGravity = true;	// �L��
}

//==========================================================================
// �����擾
//==========================================================================
int CEffect3D::GetNumAll()
{
	return m_nNumAll;
}
