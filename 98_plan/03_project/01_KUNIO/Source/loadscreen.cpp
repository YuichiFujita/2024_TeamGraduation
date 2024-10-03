//=============================================================================
// 
//  �t�F�[�h���� [loadscreen.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "loadscreen.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int NUM_STRING = 10;	// NOWLOADING�̕�����
	const std::string TEXPATH[NUM_STRING] =
	{
		"data\\TEXTURE\\load\\n.png",
		"data\\TEXTURE\\load\\o.png",
		"data\\TEXTURE\\load\\w.png",
		"data\\TEXTURE\\load\\l.png",
		"data\\TEXTURE\\load\\o.png",
		"data\\TEXTURE\\load\\a.png",
		"data\\TEXTURE\\load\\d.png",
		"data\\TEXTURE\\load\\i.png",
		"data\\TEXTURE\\load\\mini_n.png",
		"data\\TEXTURE\\load\\g.png",
	};
	const std::string TEXTURE_CYLINDER = "data\\TEXTURE\\load\\cylinder.png";
	const MyLib::Vector3 STR_DEFPOS = MyLib::Vector3(150.0f, 300.0f, 0.0f);	// ��_�̈ʒu
	const MyLib::Vector3 DEFAULTPOS_CYLINDER = MyLib::Vector3(-300.0f, 600.0f, 0.0f);	// ��_�̈ʒu
	const float STR_HEIGHT = 100.0f;		// ������̍���
	const float CYLINDER_HEIGHT = 100.0f;	// ���̍���
}


//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CLoadScreen::CLoadScreen()
{
	// �l�̃N���A
	for (int i = 0; i < NUM_STRING; i++)
	{
		m_apObj2D[i] = nullptr;
	}

	// ��
	m_pCylinder = nullptr;
	m_fBobbingTime = 0.0f;		// �Ղ��Ղ��^�C�}�[
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CLoadScreen::~CLoadScreen()
{

}

//==========================================================================
// ��������
//==========================================================================
CLoadScreen* CLoadScreen::Create()
{
	// �������̊m��
	CLoadScreen* pFade = DEBUG_NEW CLoadScreen;

	if (pFade != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		if (FAILED(pFade->Init()))
		{// ���s���Ă�����
			return nullptr;
		}
	}

	return pFade;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CLoadScreen::Init()
{
	
	MyLib::Vector3 pos = STR_DEFPOS;
	CTexture* pTex = CTexture::GetInstance();

	// ��������
	for (int i = 0; i < NUM_STRING; i++)
	{
		m_apObj2D[i] = CObject2D::Create();
		m_apObj2D[i]->SetType(CObject::TYPE::TYPE_NONE);
		m_apObj2D[i]->SetPosition(pos);
		m_apObj2D[i]->SetOriginPosition(pos);

		// �e�N�X�`�����蓖��
		int nIdx = pTex->Regist(TEXPATH[i]);
		m_apObj2D[i]->BindTexture(nIdx);

		// �T�C�Y�ύX
		D3DXVECTOR2 size = UtilFunc::Transformation::AdjustSizeByHeight(pTex->GetImageSize(nIdx), STR_HEIGHT);
		m_apObj2D[i]->SetSize(size);

		// �����ʒu���炷
		pos.x += size.x * 2.0f;

		if (i == 2)
		{// NOW��Loading�̊�
			pos.x += size.x * 2;
		}
	}

	// ������
	CreateCylinder();

	return S_OK;
}

//==========================================================================
// ������
//==========================================================================
void CLoadScreen::CreateCylinder()
{
	// ������
	m_pCylinder = CObject2D::Create();
	m_pCylinder->SetType(CObject::TYPE::TYPE_NONE);
	m_pCylinder->SetPosition(DEFAULTPOS_CYLINDER);
	m_pCylinder->SetOriginPosition(DEFAULTPOS_CYLINDER);

	// �e�N�X�`�����蓖��
	int nIdx = CTexture::GetInstance()->Regist(TEXTURE_CYLINDER);
	m_pCylinder->BindTexture(nIdx);

	// �T�C�Y�ύX
	D3DXVECTOR2 size = UtilFunc::Transformation::AdjustSizeByHeight(CTexture::GetInstance()->GetImageSize(nIdx), CYLINDER_HEIGHT);
	m_pCylinder->SetSize(size);

}

//==========================================================================
// �I������
//==========================================================================
void CLoadScreen::Uninit()
{

	for (int i = 0; i < NUM_STRING; i++)
	{
		if (m_apObj2D[i] == nullptr)
		{
			m_apObj2D[i]->Uninit();
			m_apObj2D[i] = nullptr;
		}
	}

	if (m_pCylinder != nullptr)
	{
		m_pCylinder->Uninit();
		m_pCylinder = nullptr;
	}
}

//==========================================================================
// �폜����
//==========================================================================
void CLoadScreen::Kill()
{
	for (int i = 0; i < NUM_STRING; i++)
	{
		if (m_apObj2D[i] != nullptr)
		{
			m_apObj2D[i]->Uninit();
			delete m_apObj2D[i];
			m_apObj2D[i] = nullptr;
		}
	}

	if (m_pCylinder != nullptr)
	{
		m_pCylinder->Uninit();
		delete m_pCylinder;
		m_pCylinder = nullptr;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CLoadScreen::Update()
{
	// ��������
	for (int i = 0; i < NUM_STRING; i++)
	{
		if (m_apObj2D[i] == nullptr) continue;

		// �A�h���X�n��
		CObject2D* pObj2D = m_apObj2D[i];

		// ���擾
		MyLib::Vector3 pos = pObj2D->GetPosition(), move = pObj2D->GetMove(), rot = pObj2D->GetRotation();
		MyLib::Vector3 posOrigin = pObj2D->GetOriginPosition();
		pos.y += move.y;
		move.y += 0.15f;

		// ��]
		rot.z = UtilFunc::Correction::EasingEaseIn(0.0f, D3DX_PI * -0.5f, posOrigin.y, posOrigin.y - 200.0f, pos.y);

		if (posOrigin.y <= pos.y)
		{// ���n
			move.y = 0.0f;
			pos.y = posOrigin.y;
		}

		// ���ݒ�
		pObj2D->SetPosition(pos);
		pObj2D->SetMove(move);
		pObj2D->SetRotation(rot);

		// �X�V����
		pObj2D->Update();
	}

	// ���̓���
	MoveCylinder();
	if (m_pCylinder != nullptr)
	{
		m_pCylinder->Update();
	}

	// �����Ƃ̔���
	CollisionText();
}

//==========================================================================
// �����Ƃ̔���
//==========================================================================
void CLoadScreen::CollisionText()
{
	if (m_pCylinder == nullptr) return;

	// �ʒu�擾
	MyLib::Vector3 pos = m_pCylinder->GetPosition();

	MyLib::Vector3 textpos;
	D3DXVECTOR2 textsize;
	for (const auto& text : m_apObj2D)
	{
		// ���擾
		textpos = text->GetPosition();
		textsize = text->GetSize();

		if (pos.x >= textpos.x - textsize.x &&
			pos.x <= textpos.x + textsize.x &&
			textpos.IsNearlyTargetY(text->GetOriginPosition().y, 0.1f))
		{// ���� && ���n

			// ���グ
			text->SetMove(MyLib::Vector3(0.0f, -5.0f, 0.0f));

		}
	}

}

//==========================================================================
// ���̓���
//==========================================================================
void CLoadScreen::MoveCylinder()
{
	if (m_pCylinder == nullptr) return;

	// �ʒu�擾
	MyLib::Vector3 pos = m_pCylinder->GetPosition(), posOrigin = m_pCylinder->GetOriginPosition();

	// ���ړ�
	pos.x += 5.0f;

	// �Ղ��Ղ�
	m_fBobbingTime += CManager::GetInstance()->GetDeltaTime();
	pos.y = posOrigin.y + sinf(D3DX_PI * (m_fBobbingTime / 0.8f)) * 10.0f;

	// �܂�Ԃ�
	if (pos.x >= 1280.0f + m_pCylinder->GetSize().x)
	{
		pos.x = -100.0f;
	}

	// �ʒu�ݒ�
	m_pCylinder->SetPosition(pos);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CLoadScreen::Draw()
{
	// �����`��
	for (int i = 0; i < NUM_STRING; i++)
	{
		if (m_apObj2D[i] != nullptr)
		{
			m_apObj2D[i]->Draw();
		}
	}

	if (m_pCylinder != nullptr)
	{
		m_pCylinder->Draw();
	}
}
