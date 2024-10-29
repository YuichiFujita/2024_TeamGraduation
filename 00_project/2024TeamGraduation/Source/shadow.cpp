//=============================================================================
// 
//  �e���� [shadow.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "shadow.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define POS_SHADOW	(50.0f)

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority)
{
	m_pObject = nullptr;	// �I�u�W�F�N�g�̃|�C���^
	m_nTexIdx = 0;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
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
CShadow *CShadow::Create(CObject* pObject, float fRadius)
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
}

//==========================================================================
// ���Έʒu�̐ݒ菈��
//==========================================================================
void CShadow::SetPositionRelative()
{
	// �ʒu�擾
	MyLib::Vector3 pos = m_pObject->GetPosition();

	// Y���W��n�ʂɐݒ�
	pos.y = CGameManager::FIELD_LIMIT + 1.0f;

	// �ʒu�ݒ�
	SetPosition(pos);
}
