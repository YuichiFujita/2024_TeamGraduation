//=============================================================================
// 
//  �퓬�J�n���� [battlestart_effect.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "battlestart_effect.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "particle.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\battlestart\\don3.png";	// �e�N�X�`���̃t�@�C��
	const float LIFE = 0.7f;	// �W�܂鎞��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBattleStart_Effect::CBattleStart_Effect(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_fLife = 0.0f;	// ����
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBattleStart_Effect::~CBattleStart_Effect()
{

}

//==========================================================================
// ��������
//==========================================================================
CBattleStart_Effect* CBattleStart_Effect::Create(const MyLib::Vector3& pos)
{
	// �������̊m��
	CBattleStart_Effect* pMarker = DEBUG_NEW CBattleStart_Effect;

	if (pMarker != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �ʒu�ݒ�
		pMarker->SetPosition(pos);
		pMarker->SetOriginPosition(pos);

		// ����������
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CBattleStart_Effect::Init()
{

	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nTexIdx);

	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nTexIdx);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
	SetSize(size);
	SetSizeOrigin(size);

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// ����
	m_fLife = LIFE;
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CBattleStart_Effect::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CBattleStart_Effect::Update()
{
	// �������Z
	m_fLife -= CManager::GetInstance()->GetDeltaTime();

	if (m_fLife <= 0.0f)
	{
		Uninit();
		return;
	}

	// �g��
	D3DXVECTOR2 size = GetSize();
	size += GetSizeOrigin() * 0.1f;
	UtilFunc::Correction::InertiaCorrection(size.x, GetSizeOrigin().x * 3.0f, 0.08f);
	UtilFunc::Correction::InertiaCorrection(size.y, GetSizeOrigin().y * 3.0f, 0.08f);
	SetSize(size);

	float alpha = (m_fLife / LIFE) * 0.4f;
	SetAlpha(alpha);

	// �X�V����
	CObject2D::Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CBattleStart_Effect::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}
