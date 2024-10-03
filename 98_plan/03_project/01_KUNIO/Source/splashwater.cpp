//=============================================================================
// 
//  �T���v��_�I�u�W�F�N�g2D���� [sample_obj2D.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "splashwater.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_SPLASH[] =
	{
		"data\\TEXTURE\\splash\\drop_pattern_000.png",
		"data\\TEXTURE\\splash\\splash_01.png",

	};	// �e�N�X�`���̃t�@�C��
	const float TIME_FADEOUT_RATIO = 0.3f;
	const float DEFAULT_LIFE = 1.0f;
}

namespace StateTime
{
	const float APPEARANCE = 0.3f;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CSplashwater::CSplashwater(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_fLife = 0.0f;			// ����
	m_fOriginLife = 0.0f;	// ����
	m_state = State::STATE_NONE;				// ���
	m_fStateTime = 0.0f;	// ��ԃ^�C�}�[
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CSplashwater::~CSplashwater()
{

}

//==========================================================================
// ��������
//==========================================================================
CSplashwater* CSplashwater::Create()
{
	// �������̊m��
	CSplashwater* pObj = DEBUG_NEW CSplashwater;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CSplashwater::Init()
{
	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SPLASH[UtilFunc::Transformation::Random(0, 1)]);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 25.0f + UtilFunc::Transformation::Random(-200, 100) * 0.1f);
#endif

	// �ڕW�T�C�Y
	m_sizeDest = size;

	size *= 0.5f;
	SetSize(size);
	SetSizeOrigin(size);

	// �ʒu�ݒ�
	m_posDest = MyLib::Vector3(
		UtilFunc::Transformation::Random(-64, 64) * 10.0f + 640.0f,
		UtilFunc::Transformation::Random(-4, 66) * 10.0f + UtilFunc::Transformation::Random(-8, 8) * 10.0f,
		//UtilFunc::Transformation::Random(-40, 100),
		0.0f);

	SetPosition(MyLib::Vector3(
		UtilFunc::Transformation::Random(-50, 50) + 640.0f,
		UtilFunc::Transformation::Random(-20, 20) + 600.0f,
		0.0f));
	SetOriginPosition(GetPosition());

	// �F�ݒ�
	float rg = UtilFunc::Transformation::Random(7, 10) * 0.1f;
	SetColor(D3DXCOLOR(rg, rg, 1.0f, UtilFunc::Transformation::Random(2, 9) * 0.1f));
	SetOriginColor(GetColor());

	// ����
	m_fLife = DEFAULT_LIFE + UtilFunc::Transformation::Random(-2, 10) * 0.1f;
	m_fOriginLife = m_fLife;

	m_state = State::STATE_APPEARANCE;	// ���
	m_fStateTime = 0.0f;	// ��ԃ^�C�}�[

	// �ړ���
	SetMove(CalSetmove());

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CSplashwater::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CSplashwater::Update()
{
	// �I������
	CObject2D::Update();

	if (m_state == State::STATE_APPEARANCE)
	{
		StateAppearance();
		return;
	}

	m_fLife -= CManager::GetInstance()->GetDeltaTime();

	MyLib::Vector3 move = GetMove();

	if (UtilFunc::Transformation::Random(0, 8) == 0)
	{
		move.y = CalSetmove().y;
	}

	if (UtilFunc::Transformation::Random(0, 4) == 0)
	{
		move.x = CalSetmove().x;
	}

	AddPosition(move);
	SetMove(move);

	float lifeRatio = m_fLife / m_fOriginLife;
	if (lifeRatio <= TIME_FADEOUT_RATIO)
	{
		SetAlpha(GetOriginColor().a * (lifeRatio / TIME_FADEOUT_RATIO));
	}

	if (m_fLife <= 0.0f)
	{
		Uninit();
	}
}

//==========================================================================
// �o�����
//==========================================================================
void CSplashwater::StateAppearance()
{
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// �ʒu�X�V
	MyLib::Vector3 pos = UtilFunc::Correction::EasingEaseInOut(GetOriginPosition(), m_posDest, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetPosition(pos);

	// �T�C�Y�X�V
	D3DXVECTOR2 size;
	size.x = UtilFunc::Correction::EasingEaseIn(GetSizeOrigin().x, m_sizeDest.x, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	size.y = UtilFunc::Correction::EasingEaseIn(GetSizeOrigin().y, m_sizeDest.y, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetSize(size);

	if (m_fStateTime >= StateTime::APPEARANCE)
	{
		SetPosition(m_posDest);
		SetSize(m_sizeDest);
		m_state = State::STATE_NONE;
	}
}

//==========================================================================
// �ړ��ʌv�Z
//==========================================================================
MyLib::Vector3 CSplashwater::CalSetmove()
{
	MyLib::Vector3 move;
	move.x = UtilFunc::Transformation::Random(-8, 8) * 0.1f;
	//move.x = UtilFunc::Transformation::Random(-40, 40) * 0.1f;
	move.y = UtilFunc::Transformation::Random(0, 400) * 0.01f;
	//move.y = UtilFunc::Transformation::Random(40, 1500) * 0.01f;
	return move;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CSplashwater::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	/*pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/

	// �`�揈��
	CObject2D::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
