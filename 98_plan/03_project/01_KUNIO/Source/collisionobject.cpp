//=============================================================================
// 
// �����蔻��I�u�W�F�N�g���� [collisionobject.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "collisionobject.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "game.h"
#include "player.h"
#include "calculation.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CCollisionObject::m_nIdxTex = 0;	// �e�N�X�`���̃C���f�b�N�X�ԍ�

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCollisionObject::CCollisionObject(int nPriority) : CObjectBillboard(nPriority)
{
	// �l�̃N���A
	m_fRadius = 0.0f;	// ���a
	m_nDamage = 0;		// �_���[�W
	m_nLife = 0;		// ����
	m_nMaxLife = 0;		// �ő����(�Œ�)
	m_MyTag = TAG_NONE;			// �����̃^�O
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCollisionObject::~CCollisionObject()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CCollisionObject *CCollisionObject::Create()
{
	// �����p�̃I�u�W�F�N�g
	CCollisionObject *pEffect = nullptr;

	if (pEffect == nullptr)
	{// nullptr��������

		// �������̊m��
		pEffect = DEBUG_NEW CCollisionObject;

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
CCollisionObject *CCollisionObject::Create(const MyLib::Vector3 pos, const MyLib::Vector3 move, const float fRadius, const int nLife, const int nDamage, eMyTag tag)
{
	// �����p�̃I�u�W�F�N�g
	CCollisionObject *pEffect = nullptr;

	if (pEffect == nullptr)
	{// nullptr��������

		// �������̊m��
		pEffect = DEBUG_NEW CCollisionObject;

		if (pEffect != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			pEffect->SetPosition(pos);
			pEffect->SetMove(move);
			pEffect->m_nDamage = nDamage;	// �_���[�W
			pEffect->m_MyTag = tag;			// ���
			pEffect->m_fRadius = fRadius;
			pEffect->SetSize(D3DXVECTOR2(fRadius, fRadius));
			pEffect->m_nLife = nLife;

			// ����������
			HRESULT hr = pEffect->Init();
			if (FAILED(hr))
			{
				return nullptr;
			}
		}

		return pEffect;
	}

	return nullptr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCollisionObject::Init()
{
	HRESULT hr;

	// �e��ϐ��̏�����
	m_nMaxLife = m_nLife;						// �ő����(�Œ�)

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECTX);

#if 0
	if (m_nIdxTex == 0)
	{
		m_nIdxTex = CTexture::GetInstance()->Regist("data\\TEXTURE\\effect\\effect000.jpg");
	}

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nIdxTex);
#endif

	// ����������
	hr = CObjectBillboard::Init();
	if (FAILED(hr))
	{// ���s�����Ƃ�
		Uninit();
		return E_FAIL;
	}

	SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCollisionObject::Uninit()
{
	// �I������
	CObjectBillboard::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CCollisionObject::Update()
{

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �ʒu�X�V
	pos += move;

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����̍X�V
	m_nLife--;

	// �T�C�Y�ݒ�
	SetSize(D3DXVECTOR2(m_fRadius, m_fRadius));

	if (m_nLife <= 0)
	{// �������s������
		Uninit();
		return;
	}

	switch (m_MyTag)
	{
	case CCollisionObject::TAG_PLAYER:
		CollisionEnemy();
		break;

	case CCollisionObject::TAG_ENEMY:
		CollisionPlayer();
		break;
	}

	// ���_���W�̐ݒ�
	SetVtx();

}

//==========================================================================
// �G�Ƃ̓����蔻��
//==========================================================================
void CCollisionObject::CollisionEnemy()
{
	
	
}

//==========================================================================
// �v���C���[�Ƃ̓����蔻��
//==========================================================================
void CCollisionObject::CollisionPlayer()
{

	// �����̏��擾
	MyLib::Vector3 pos = GetPosition();


	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;

	// ���X�g���[�v
	while (playerList.ListLoop(&pPlayer))
	{
		// �v���C���[���擾
		MyLib::Vector3 PlayerPos = pPlayer->GetPosition();
		float PlayerRadius = pPlayer->GetRadius();
		CPlayer::STATE PlayerState = pPlayer->GetState();

		// ���̔���
		if (UtilFunc::Collision::SphereRange(pos, PlayerPos, m_fRadius, PlayerRadius).ishit)
		{
			// �q�b�g����
			pPlayer->Hit(m_nDamage);
		}
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CCollisionObject::Draw()
{
	return;
#ifdef _DEBUG
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

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
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �`�揈��
	CObjectBillboard::Draw();

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
#endif
}
