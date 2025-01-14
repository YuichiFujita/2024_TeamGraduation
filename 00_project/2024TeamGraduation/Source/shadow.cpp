//==========================================================================
// 
//  �e���� [shadow.cpp]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
// 
//==========================================================================
#include "shadow.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//==========================================================================
//	�萔�錾
//==========================================================================
namespace
{
	const float	MAX_DIS_HEIGHT	= 260.0f;	// �e�Ɛe�̏c�̋����̍ő�l
	const float	MAX_PLUS_RADIUS	= 50.0f;	// �e�̑傫�����Z�ʂ̍ő�l
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority),
	m_pObject	(nullptr),	// �I�u�W�F�N�g�̃|�C���^
	m_fLandY	(0.0f),		// �\��Y���W
	m_nTexIdx	(0)			// �e�N�X�`���̃C���f�b�N�X�ԍ�
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CShadow::~CShadow()
{
	
}

//==========================================================================
// ��������(�I�[�o�[���[�h)
//==========================================================================
CShadow *CShadow::Create
(
	CObject* pObject,	// �e�I�u�W�F�N�g
	float fRadius,		// ���̔��a
	float fMinAlpha,	// �ŏ������x
	float fMaxAlpha,	// �ő哧���x
	float fLandY		// �\��Y���W
)
{
	// �����p�̃I�u�W�F�N�g
	CShadow *pShadow = nullptr;
	if (pShadow == nullptr)
	{// nullptr��������

		// �������̊m��
		pShadow = DEBUG_NEW CShadow;
		if (pShadow != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// �Ǐ]�I�u�W�F�N�g�̊���
			pShadow->BindObject(pObject);

			// ���Έʒu�̐ݒ�
			pShadow->SetPositionRelative();

			// �傫���̐ݒ�
			pShadow->SetSize(MyLib::Vector3(fRadius, 0.0f, fRadius));

			// ���̔��a�̐ݒ�
			pShadow->m_fOriginRadius = fRadius;

			// �\��Y���W�̐ݒ�
			pShadow->m_fLandY = fLandY;

			// �����x�̍ŏ��l
			pShadow->m_fMinAlpha = fMinAlpha;

			// �����x�̍ő�l
			pShadow->m_fMaxAlpha = fMaxAlpha;

			// ����������
			pShadow->Init();
		}
		return pShadow;
	}

	return nullptr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CShadow::Init()
{
	HRESULT hr;

	// �e��ϐ��̏�����
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));	// �F

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// ����������
	hr = CObject3D::Init();

	if (FAILED(hr))
	{// ���s�����Ƃ�

		return E_FAIL;
	}

	// �e�N�X�`���̊��蓖��
	m_nTexIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\effect\\effect000.jpg");

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx);

	// �����ݒ�
	SetRotation(MyLib::Vector3(D3DX_PI * 0.5f, 0.0f, 0.0f));

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CShadow::Uninit()
{
	// �I������
	CObject3D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CShadow::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���S�̔���
	if (IsDeath()) { return; }

	// ���Έʒu�̐ݒ�
	SetPositionRelative();

	// ���_���ݒ�
	CObject3D::SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CShadow::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

	// �`�揈��
	CObject3D::Draw();

	// ���Z���������Ƃɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//==========================================================================
// ���Έʒu�̐ݒ菈��
//==========================================================================
void CShadow::SetPositionRelative()
{
	MyLib::Vector3 posParent = m_pObject->GetPosition();	// �e�I�u�W�F�N�g�ʒu
	MyLib::Vector3 posShadow = posParent;	// �e�ʒu
	MyLib::Vector2 sizeShadow;	// �e�傫��
	float fDis = 0.0f;		// �e�Ɛe�̋���
	float fAlpha = 0.0f;	// �����x

	// Y���W��n�ʂɐݒ�
	posShadow.y = m_fLandY + 1.0f;

	// �e�Ɛe�̏c���W�̋��������߂�
	fDis = fabsf(posParent.y - posShadow.y);								// �c�̋��������߂�
	UtilFunc::Transformation::ValueNormalize(fDis, MAX_DIS_HEIGHT, 0.0f);	// �c�̋����𐧌�
	fDis *= 1.0f / MAX_DIS_HEIGHT;											// ������������

	// �e�̑傫�������߂�
	float fRadius = m_fOriginRadius + (MAX_PLUS_RADIUS * fDis);	// ���a
	sizeShadow = fRadius;

	// ���l�����߂�
	fAlpha = fabsf(fDis - 1.0f);	// ���l��ݒ�
	UtilFunc::Transformation::ValueNormalize(fAlpha, m_fMaxAlpha, m_fMinAlpha);	// ���l�𐧌�

	// �e�̕`�����ݒ�
	SetPosition(posShadow);	// �ʒu
	SetSize(sizeShadow);	// �傫��
	SetAlpha(fAlpha);		// �����x
}
