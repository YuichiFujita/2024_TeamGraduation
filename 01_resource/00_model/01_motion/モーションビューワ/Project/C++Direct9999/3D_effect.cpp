//=============================================================================
// 
// �G�t�F�N�g���� [3D_effect.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "3D_effect.h"

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
const char *CEffect3D::m_apTextureFile[] =	// �t�@�C���ǂݍ���
{
	"data\\TEXTURE\\effect\\effect000.jpg",	// �ʏ�G�t�F�N�g
	"data\\TEXTURE\\effect\\effect002.jpg",	// ���G�t�F�N�g
	"data\\TEXTURE\\effect\\smoke_05.tga",	// ����
	"data\\TEXTURE\\effect\\effect000.png",	// ���G�t�F�N�g
	"data\\TEXTURE\\effect\\effect001.png",	// �\���G�t�F�N�g
	"data\\TEXTURE\\effect\\cancelacle.png",// �L�����Z���G�t�F�N�g
};
int CEffect3D::m_nNumAll = 0;	// ����

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEffect3D::CEffect3D(int nPriority) : CObjectBillBoard(nPriority)
{
	// �l�̃N���A
	m_colOrigin = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F�̌�
	m_fRadius = 0.0f;							// ���a
	m_fMaxRadius = 0.0f;						// �ő唼�a
	m_nLife = 0;								// ����
	m_nMaxLife = 0;								// �ő����(�Œ�)
	m_moveType = MOVEEFFECT_NONE;				// �ړ��̎��
	m_nType = TYPE_NORMAL;						// ���
	m_bAddAlpha = true;							// ���Z�����̔���

	// �e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		m_nTexIdx[nCntTex] = 0;		// �e�N�X�`���̃C���f�b�N�X�ԍ�
	}

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
// ��������
//==========================================================================
CEffect3D *CEffect3D::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CEffect3D *pEffect = NULL;

	if (pEffect == NULL)
	{// NULL��������

		// �������̊m��
		pEffect = DEBUG_NEW CEffect3D;

		if (pEffect->GetID() < 0)
		{// �������m�ۂɎ��s���Ă�����

			delete pEffect;
			return NULL;
		}

		if (pEffect != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pEffect->Init();
		}

		return pEffect;
	}

	return NULL;
}

//==========================================================================
// ��������
//==========================================================================
CEffect3D *CEffect3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXCOLOR col, const float fRadius, const int nLife, const int moveType, const TYPE type)
{
	// �����p�̃I�u�W�F�N�g
	CEffect3D *pEffect = NULL;

	if (pEffect == NULL)
	{// NULL��������

		// �������̊m��
		pEffect = DEBUG_NEW CEffect3D;

		if (pEffect->GetID() < 0)
		{// �������m�ۂɎ��s���Ă�����

			delete pEffect;
			return NULL;
		}

		if (pEffect != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pEffect->Init(pos, move, col, fRadius, nLife, moveType, type);
		}

		return pEffect;
	}

	return NULL;
}

//==================================================================================
// �G�t�F�N�g�̏���������
//==================================================================================
HRESULT CEffect3D::Init(void)
{
	HRESULT hr;

	// �e��ϐ��̏�����
	m_colOrigin = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F�̌�
	m_fRadius = DEF_RADIUS;						// ���a
	m_fMaxRadius = m_fRadius;					// �ő唼�a
	m_nLife = EFFECT_3D_LIFE;					// ����
	m_nMaxLife = m_nLife;						// �ő����(�Œ�)
	m_moveType = MOVEEFFECT_NONE;				// �ړ��̎��
	m_nType = TYPE_NORMAL;						// ���
	m_bAddAlpha = true;							// ���Z�����̔���

	// ��ނ̐ݒ�
	SetType(TYPE_EFFECT3D);

	// �e�N�X�`���̊��蓖��
	m_nTexIdx[m_nType] = CManager::GetTexture()->Regist(m_apTextureFile[m_nType]);

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx[m_nType]);

	// ����������
	hr = CObjectBillBoard::Init();

	if (FAILED(hr))
	{// ���s�����Ƃ�

		return E_FAIL;
	}

	return S_OK;
}

//==================================================================================
// �G�t�F�N�g�̏���������
//==================================================================================
HRESULT CEffect3D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXCOLOR col, const float fRadius, const int nLife, const int moveType, const TYPE type)
{
	HRESULT hr;

	// �e��ϐ��̏�����
	SetPosition(pos);
	SetMove(move);
	m_colOrigin = col;							// �F�̌�
	SetColor(col);								// �F
	m_fRadius = fRadius;						// ���a
	m_fMaxRadius = m_fRadius;					// �ő唼�a
	SetSize(D3DXVECTOR2(m_fRadius, m_fRadius));	// �T�C�Y�ݒ�
	m_nLife = nLife;							// ����
	m_nMaxLife = m_nLife;						// �ő����(�Œ�)
	m_moveType = moveType;						// �ړ��̎��
	m_nType = type;								// ���

	// ���Z�����̔���
	switch (m_nType)
	{
	case TYPE_NORMAL:
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

	case TYPE_CANCEL:
		m_bAddAlpha = false;
		break;

	default:
		m_bAddAlpha = true;
		break;
	}

	// ��ނ̐ݒ�
	SetType(TYPE_EFFECT3D);

	// �e�N�X�`���̊��蓖��
	m_nTexIdx[m_nType] = CManager::GetTexture()->Regist(m_apTextureFile[m_nType]);

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx[m_nType]);

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx[m_nType]);

	// ����������
	hr = CObjectBillBoard::Init();

	if (FAILED(hr))
	{// ���s�����Ƃ�

		return E_FAIL;
	}

	return S_OK;
}

//==================================================================================
// �G�t�F�N�g�̏I������
//==================================================================================
void CEffect3D::Uninit(void)
{
	// �������Z
	m_nNumAll--;

	// �I������
	CObjectBillBoard::Uninit();
}

//==================================================================================
// �G�t�F�N�g�̍X�V����
//==================================================================================
void CEffect3D::Update(void)
{

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �F�擾
	D3DXCOLOR col = GetColor();

	// �ʒu�X�V
	pos += move;

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// ���������Ă��������̏ꍇ
	if (m_moveType == MOVEEFFECT_SUB)
	{// �G�t�F�N�g�����������Ă���

		// �k������
		SubSize();
	}
	else if (m_moveType == MOVEEFFECT_SUPERSUB)
	{// �G�t�F�N�g�����������Ă���

		// ���k������
		SuperSubSize();
	}
	else if (m_moveType == MOVEEFFECT_ADD)
	{// �G�t�F�N�g��傫�����Ă���

		// �g�又��
		AddSize();
	}

	// �����̍X�V
	m_nLife--;

	// �s�����x�̍X�V
	col.a = m_colOrigin.a * ((float)m_nLife / (float)m_nMaxLife);

	// �T�C�Y�ݒ�
	SetSize(D3DXVECTOR2(m_fRadius, m_fRadius));

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

//==================================================================================
// �G�t�F�N�g�̏k������
//==================================================================================
void CEffect3D::SubSize(void)
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

//==================================================================================
// �G�t�F�N�g�̏k������
//==================================================================================
void CEffect3D::SuperSubSize(void)
{
	m_fRadius = m_fMaxRadius * (float)m_nLife / (float)m_nMaxLife;
}

//==================================================================================
// �G�t�F�N�g�̊g�又��
//==================================================================================
void CEffect3D::AddSize(void)
{
	int nEffect_3DType = rand() % 3;

	if (nEffect_3DType == 0)
	{
		m_fRadius += 1.8f;
	}
	else if (nEffect_3DType == 1)
	{
		m_fRadius += 0.8f;
	}
	else if (nEffect_3DType == 2)
	{
		m_fRadius += 0.0f;
	}
}

//==================================================================================
// �G�t�F�N�g�̕`�揈��
//==================================================================================
void CEffect3D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���u�����f�B���O�����Z�����ɐݒ�
	if (m_bAddAlpha == true)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// �`�揈��
	CObjectBillBoard::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CEffect3D::SetVtx(void)
{
	// ���_�ݒ�
	CObjectBillBoard::SetVtx();
}

//==========================================================================
// �����擾
//==========================================================================
int CEffect3D::GetNumAll(void)
{
	return m_nNumAll;
}
