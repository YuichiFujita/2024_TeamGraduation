//=============================================================================
// 
//  �����Ԃ��w�i���� [splashwater_bg.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "splashwater_bg.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_SPLASH = "data\\TEXTURE\\splash\\BG.png";
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
CSplashwater_BG::CSplashwater_BG(int nPriority) : CObject2D(nPriority)
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
CSplashwater_BG::~CSplashwater_BG()
{

}

//==========================================================================
// ��������
//==========================================================================
CSplashwater_BG* CSplashwater_BG::Create()
{
	// �������̊m��
	CSplashwater_BG* pObj = DEBUG_NEW CSplashwater_BG;

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
HRESULT CSplashwater_BG::Init()
{
	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SPLASH);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 640.0f);
	SetSize(size);
	SetSizeOrigin(size);

	// �ʒu
	SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));

	// �F�ݒ�
	SetAlpha(0.0f);
	float rg = UtilFunc::Transformation::Random(7, 10) * 0.1f;
	SetOriginColor(D3DXCOLOR(rg, rg, 1.0f, 0.5f));

	// ����
	m_fLife = DEFAULT_LIFE + UtilFunc::Transformation::Random(-2, 1) * 0.1f;
	m_fOriginLife = m_fLife;

	m_state = State::STATE_APPEARANCE;	// ���
	m_fStateTime = 0.0f;	// ��ԃ^�C�}�[

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CSplashwater_BG::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CSplashwater_BG::Update()
{
	// �I������
	CObject2D::Update();

	if (m_state == State::STATE_APPEARANCE)
	{
		StateAppearance();
		return;
	}

	m_fLife -= CManager::GetInstance()->GetDeltaTime();

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
void CSplashwater_BG::StateAppearance()
{
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	float alpha = UtilFunc::Correction::EasingLinear(0.0f, GetOriginColor().a, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::APPEARANCE)
	{
		SetAlpha(GetOriginColor().a);
		m_state = State::STATE_NONE;
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CSplashwater_BG::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}
