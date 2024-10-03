//=============================================================================
// 
// �^�C�g�����S���� [titlelogo.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "titlelogo.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
#include "fade.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\title\\titlelogo4.png";
}

namespace TexturePath	// �e�N�X�`���p�X
{
	const std::string WATER = "data\\TEXTURE\\title\\water.png";
	const std::string PLAYER = "data\\TEXTURE\\title\\player.png";
	const std::string BRESS = "data\\TEXTURE\\title\\bress.png";
	const std::string LOVE[] =
	{
		"data\\TEXTURE\\title\\L.png",
		"data\\TEXTURE\\title\\O.png",
		"data\\TEXTURE\\title\\V.png",
		"data\\TEXTURE\\title\\E.png",
	};
	const std::string SUITON = "data\\TEXTURE\\title\\suiton.png";
}

namespace Size	// �T�C�Y
{
	const float WATER = 250.0f;
	const float PLAYER = 100.0f;
	const float BRESS = 80.0f;
	const float LOVE = 60.0f;
	const float SUITON = 100.0f;
}

namespace BasePoint	// ��_�̈ʒu
{
	const MyLib::Vector3 WATER = MyLib::Vector3(640.0f, 270.0f, 0.0f);
	const MyLib::Vector3 PLAYER = MyLib::Vector3(740.0f, 415.0f, 0.0f);
	const MyLib::Vector3 BRESS = MyLib::Vector3(495.0f, 265.0f, 0.0f);
	const MyLib::Vector3 LOVE = MyLib::Vector3(313.0f, 175.0f, 0.0f);
	const MyLib::Vector3 SUITON = MyLib::Vector3(830.0f, 170.0f, 0.0f);
}

namespace DestPoint	// �ڕW�̈ʒu
{
	const MyLib::Vector3 PLAYER = MyLib::Vector3(640.0f, 395.0f, 0.0f);
	const MyLib::Vector3 BRESS = MyLib::Vector3(510.0f, 347.0f, 0.0f);
	const MyLib::Vector3 LOVE[] =	// LOVE�̏ꍇ�͍���
	{
		MyLib::Vector3(-10.0f, -20.0f, 0.0f),
		MyLib::Vector3(-5.0f, -60.0f, 0.0f),
		MyLib::Vector3(0.0f, -50.0f, 0.0f),
		MyLib::Vector3(5.0f, -20.0f, 0.0f),
	};
}


namespace DestRotation	// �ڕW�̌���
{
	const MyLib::Vector3 LOVE[] =
	{
		MyLib::Vector3(0.0f, 0.0f, D3DX_PI * 0.15f),
		MyLib::Vector3(0.0f, 0.0f, D3DX_PI * 0.08f),
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		MyLib::Vector3(0.0f, 0.0f, D3DX_PI * -0.1f),
	};
}

namespace StateTime	// ��Ԏ���
{
	const float WAIT = 1.0f;
	const float WATER = 1.0f;
	const float PLAYER = 1.2f;
	const float BRESS = 1.5f;
	const float AFTERMOVEMENT = 2.0f;
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CTitleLogo::STATE_FUNC CTitleLogo::m_StateFunc[] =
{
	&CTitleLogo::StateNone,					// �Ȃ�
	&CTitleLogo::StateWait,					// �ҋ@
	&CTitleLogo::StateFadeIn_Water,			// ���t�F�[�h�C��
	&CTitleLogo::StateFadeIn_Player,	// �v���C���[�Ɩ��O�t�F�[�h�C��
	&CTitleLogo::StateBress,				// ��
	&CTitleLogo::StateWait_After,			// ��̑ҋ@
	&CTitleLogo::StateAfterMovement,		// ��̓���
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitleLogo::CTitleLogo(float fadetime, int nPriority) : m_fFadeOutTime(fadetime), CObject(nPriority)
{
	// �l�̃N���A
	m_state = STATE_NONE;	// ���
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTitleLogo::~CTitleLogo()
{

}

//==========================================================================
// ��������
//==========================================================================
CTitleLogo* CTitleLogo::Create(float fadetime)
{
	// �������̊m��
	CTitleLogo* pScreen = DEBUG_NEW CTitleLogo(fadetime);

	if (pScreen != nullptr)
	{
		// ����������
		pScreen->Init();
	}

	return pScreen;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTitleLogo::Init()
{
	// ��ނ̐ݒ�
	SetType(TYPE_OBJECT2D);

	// ������
	CreateWater();

	// �v���C���[����
	CreatePlayer();

	// ������
	CreateBress();

	// LOVE����
	CreateLOVE();

	// SUITON����
	CreateSUITON();

	// LOVE�̊Ԋu�����_���I��
	DrawingLOVEDistance();

	// ��ԑJ��
	SetState(State::STATE_WAIT);

	return S_OK;
}

//==========================================================================
// ������
//==========================================================================
void CTitleLogo::CreateWater()
{
	// ����
	m_pWater = CObject2D::Create(GetPriority() + 1);
	CObject2D* pObj2D = m_pWater;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �ʒu�ݒ�
	pObj2D->SetPosition(BasePoint::WATER);
	pObj2D->SetOriginPosition(BasePoint::WATER);


	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TexturePath::WATER);
	pObj2D->BindTexture(texID);

	// �c�������ɃT�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::WATER);

	// �T�C�Y�ݒ�
	pObj2D->SetSize(size);
	pObj2D->SetSizeOrigin(size);

	// �����x�ݒ�
	pObj2D->SetAlpha(0.0f);
}

//==========================================================================
// �v���C���[����
//==========================================================================
void CTitleLogo::CreatePlayer()
{
	// ����
	m_pPlayer = CObject2D::Create(GetPriority());
	CObject2D* pObj2D = m_pPlayer;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �ʒu�ݒ�
	pObj2D->SetPosition(BasePoint::PLAYER);
	pObj2D->SetOriginPosition(BasePoint::PLAYER);


	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TexturePath::PLAYER);
	pObj2D->BindTexture(texID);

	// �c�������ɃT�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::PLAYER);

	// �T�C�Y�ݒ�
	pObj2D->SetSize(size);
	pObj2D->SetSizeOrigin(size);

	// �����x�ݒ�
	pObj2D->SetAlpha(0.0f);
}

//==========================================================================
// ������
//==========================================================================
void CTitleLogo::CreateBress()
{
	// ����
	m_pBress = CObject2D::Create(GetPriority());
	CObject2D* pObj2D = m_pBress;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);
	pObj2D->SetAnchorType(CObject2D::AnchorPoint::UNDER_CENTER);

	// �ʒu�ݒ�
	pObj2D->SetPosition(BasePoint::BRESS);
	pObj2D->SetOriginPosition(BasePoint::BRESS);

	// ����
	pObj2D->SetRotation(MyLib::Vector3(0.0f, 0.0f, D3DX_PI * -0.15f));


	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TexturePath::BRESS);
	pObj2D->BindTexture(texID);

	// �c�������ɃT�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::BRESS);

	// �T�C�Y�ݒ�
	pObj2D->SetSize(D3DXVECTOR2(size.x, 0.0f));
	pObj2D->SetSizeOrigin(size);

	// �����x�ݒ�
	pObj2D->SetAlpha(0.0f);
}

//==========================================================================
// LOVE����
//==========================================================================
void CTitleLogo::CreateLOVE()
{
	for (int i = 0; i < LOGO_LOVE::MAX; i++)
	{
		// ����
		m_pLOVE[i] = CObject2D::Create(GetPriority());
		CObject2D* pObj2D = m_pLOVE[i];
		pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

		// �e�N�X�`���ݒ�
		int texID = CTexture::GetInstance()->Regist(TexturePath::LOVE[i]);
		pObj2D->BindTexture(texID);

		// �c�������ɃT�C�Y�ݒ�
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::LOVE);

		// �T�C�Y�ݒ�
		pObj2D->SetSize(size);
		pObj2D->SetSizeOrigin(size);

		// �ʒu�ݒ�
		pObj2D->SetPosition(BasePoint::LOVE + (MyLib::Vector3(size.x * 1.45f, 0.0f, 0.0f) * i));
		pObj2D->SetOriginPosition(pObj2D->GetPosition());

		// �����x�ݒ�
		pObj2D->SetAlpha(0.0f);
	}
}

//==========================================================================
// SUITON����
//==========================================================================
void CTitleLogo::CreateSUITON()
{
	// ����
	m_pSUITON = CObject2D::Create(GetPriority());
	CObject2D* pObj2D = m_pSUITON;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �ʒu�ݒ�
	pObj2D->SetPosition(BasePoint::SUITON);
	pObj2D->SetOriginPosition(BasePoint::SUITON);


	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TexturePath::SUITON);
	pObj2D->BindTexture(texID);

	// �c�������ɃT�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::SUITON);

	// �T�C�Y�ݒ�
	pObj2D->SetSize(size);
	pObj2D->SetSizeOrigin(size);

	// �����x�ݒ�
	pObj2D->SetAlpha(0.0f);
}

//==========================================================================
// �I������
//==========================================================================
void CTitleLogo::Uninit()
{
	// �I������
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTitleLogo::Update()
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// �t�F�[�h���͔�����
		return;
	}

	// ��ԕʍX�V
	UpdateState();
	if (IsDeath())
	{
		return;
	}

#if 0
	MyLib::Vector3 posWater = m_pWater->GetPosition();
	ImGui::DragFloat3("posWater", (float*)&posWater, 0.5f, 0.0f, 0.0f, "%.2f");
	m_pWater->SetPosition(posWater);

	MyLib::Vector3 posPlayer = m_pPlayer->GetPosition();
	ImGui::DragFloat3("posPlayer", (float*)&posPlayer, 0.5f, 0.0f, 0.0f, "%.2f");
	m_pPlayer->SetPosition(posPlayer);

	MyLib::Vector3 posBress = m_pBress->GetPosition();
	ImGui::DragFloat3("posBress", (float*)&posBress, 0.5f, 0.0f, 0.0f, "%.2f");
	m_pBress->SetPosition(posBress);

	for (int i = 0; i < LOGO_LOVE::MAX; i++)
	{
		CObject2D* pObj2D = m_pLOVE[i];

		// �ʒu�ݒ�
		MyLib::Vector3 pos = pObj2D->GetPosition();
		ImGui::Text("[%d]", i);
		ImGui::SameLine();
		ImGui::DragFloat3("posLove", (float*)&pos, 0.5f, 0.0f, 0.0f, "%.2f");
		pObj2D->SetPosition(pos);
	}

	MyLib::Vector3 posSUITON = m_pSUITON->GetPosition();
	ImGui::DragFloat3("posSUITON", (float*)&posSUITON, 0.5f, 0.0f, 0.0f, "%.2f");
	m_pSUITON->SetPosition(posSUITON);
#endif
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CTitleLogo::UpdateState()
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// �Ȃɂ��Ȃ�
//==========================================================================
void CTitleLogo::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// �ҋ@
//==========================================================================
void CTitleLogo::StateWait()
{
	if (m_fStateTime >= StateTime::WAIT)
	{// ���Ԍo��
		SetState(State::STATE_FADEIN_WATER);
	}
}

//==========================================================================
// ���t�F�[�h�C��
//==========================================================================
void CTitleLogo::StateFadeIn_Water()
{
	//=============================
	// ���O
	//=============================
	{
		// �s�����x�ݒ�
		float alpha = UtilFunc::Transformation::Clamp(m_fStateTime / StateTime::WATER, 0.0f, 1.0f);
		for (const auto& love : m_pLOVE)
		{
			love->SetAlpha(alpha);
		}
		m_pSUITON->SetAlpha(alpha);

		// �T�C�Y�X�V
		D3DXVECTOR2 size, sizeOrigin = m_pSUITON->GetSizeOrigin();
		size.x = UtilFunc::Correction::EaseOutBack(0.0f, sizeOrigin.x, 0.0f, StateTime::WATER, m_fStateTime, 4.5f);
		size.y = UtilFunc::Correction::EaseOutBack(0.0f, sizeOrigin.y, 0.0f, StateTime::WATER, m_fStateTime, 4.5f);
		m_pSUITON->SetSize(size);

		for (const auto& love : m_pLOVE)
		{
			D3DXVECTOR2 size, sizeOrigin = love->GetSizeOrigin();
			size.x = UtilFunc::Correction::EaseOutBack(0.0f, sizeOrigin.x, 0.0f, StateTime::WATER, m_fStateTime, 4.5f);
			size.y = UtilFunc::Correction::EaseOutBack(0.0f, sizeOrigin.y, 0.0f, StateTime::WATER, m_fStateTime, 4.5f);
			love->SetSize(size);
		}

	}

	// �s�����x�ݒ�
	float alpha = UtilFunc::Transformation::Clamp(m_fStateTime / StateTime::WATER, 0.0f, 1.0f);
	m_pWater->SetAlpha(alpha);

	if (m_fStateTime >= StateTime::WATER)
	{// ���Ԍo��
		SetState(State::STATE_FADEIN_PLAYER);
	}
}

//==========================================================================
// �v���C���[�Ɩ��O�t�F�[�h�C��
//==========================================================================
void CTitleLogo::StateFadeIn_Player()
{
	//=============================
	// �v���C���[
	//=============================
	{
		// �s�����x�ݒ�
		float alpha = UtilFunc::Transformation::Clamp(m_fStateTime / StateTime::PLAYER, 0.0f, 1.0f);
		m_pPlayer->SetAlpha(alpha);

		// �v���C���[�ړ�
		MyLib::Vector3 playerpos = UtilFunc::Correction::EasingEaseOut(BasePoint::PLAYER, DestPoint::PLAYER, 0.0f, StateTime::PLAYER, m_fStateTime);
		m_pPlayer->SetPosition(playerpos);
	}

	if (m_fStateTime >= StateTime::PLAYER)
	{// ���Ԍo��
		SetState(State::STATE_BRESS);
	}
}

//==========================================================================
// ��
//==========================================================================
void CTitleLogo::StateBress()
{
	//=============================
	// ��
	//=============================
	{
		// �s�����x�ݒ�
		float alpha = UtilFunc::Correction::EaseOutExpo(0.0f, 1.0f, 0.0f, StateTime::BRESS, m_fStateTime);
		m_pBress->SetAlpha(alpha);

		// �ړ�
		MyLib::Vector3 pos = UtilFunc::Correction::EaseOutExpo(BasePoint::BRESS, DestPoint::BRESS, 0.0f, StateTime::BRESS, m_fStateTime);
		m_pBress->SetPosition(pos);

		// �T�C�Y
		float sizeX = m_pBress->GetSizeOrigin().x;
		float sizeY = UtilFunc::Correction::EaseOutExpo(0.0f, m_pBress->GetSizeOrigin().y, 0.0f, StateTime::BRESS, m_fStateTime);
		m_pBress->SetSize(D3DXVECTOR2(sizeX, sizeY));

		D3DXVECTOR2* pTex = m_pBress->GetTex();
		pTex[2].y = pTex[3].y = (sizeY / m_pBress->GetSizeOrigin().y);
	}

	//=============================
	// LOVE
	//=============================
	{
		// �ړ�
		for (int i = 0; i < LOGO_LOVE::MAX; i++)
		{
			// �ړ�
			MyLib::Vector3 pos = UtilFunc::Correction::EaseOutExpo(
				m_pLOVE[i]->GetOriginPosition(),
				m_pLOVE[i]->GetOriginPosition() + DestPoint::LOVE[i],
				0.0f, StateTime::BRESS, m_fStateTime);
			m_pLOVE[i]->SetPosition(pos);

			// ����
			MyLib::Vector3 rot = UtilFunc::Correction::EaseOutExpo(
				0.0f,
				DestRotation::LOVE[i],
				0.0f, StateTime::BRESS, m_fStateTime);
			m_pLOVE[i]->SetRotation(rot);
		}
	}


	if (m_fStateTime >= StateTime::BRESS)
	{// ���Ԍo��
		SetState(State::STATE_WAIT_AFTER);
	}
}

//==========================================================================
// ��̑ҋ@
//==========================================================================
void CTitleLogo::StateWait_After()
{
	if (m_fStateTime >= StateTime::WAIT)
	{// ���Ԍo��
		SetState(State::STATE_AFTERMOVEMENT);
	}
}

//==========================================================================
// ��̓���
//==========================================================================
void CTitleLogo::StateAfterMovement()
{
	//=============================
	// ��
	//=============================
	{
		// �X�N���[��
		D3DXVECTOR2* pTex = m_pWater->GetTex();
		pTex[0].x -= 0.001f;
		pTex[1].x = pTex[0].x + 1.0f;
		pTex[2].x -= 0.001f;
		pTex[3].x = pTex[2].x + 1.0f;
	}

	//=============================
	// ��
	//=============================
	{
		// �ړ�
		MyLib::Vector3 pos = m_pBress->GetPosition();
		pos = DestPoint::BRESS + sinf(D3DX_PI * (m_fStateTime / StateTime::AFTERMOVEMENT)) * MyLib::Vector3(-10.0f, -10.0f, 0.0f);
		m_pBress->SetPosition(pos);
	}

	//=============================
	// �v���C���[
	//=============================
	{
		// �ړ�
		MyLib::Vector3 pos = m_pPlayer->GetPosition();
		pos = DestPoint::PLAYER + sinf(D3DX_PI * (m_fStateTime / StateTime::AFTERMOVEMENT)) * MyLib::Vector3(-10.0f, -10.0f, 0.0f);
		m_pPlayer->SetPosition(pos);
	}

	//=============================
	// LOVE
	//=============================
	{
		for (int i = 0; i < LOGO_LOVE::MAX; i++)
		{
			// ����
			float ratio = D3DX_PI * (m_fStateTime / StateTime::AFTERMOVEMENT);
			MyLib::Vector3 rot = DestRotation::LOVE[i] + sinf(ratio) * (DestRotation::LOVE[i] * 0.1f);
			m_pLOVE[i]->SetRotation(rot);

			// �ړ�
			MyLib::Vector3 pos, posOrigin = m_pLOVE[i]->GetPosition();
			pos.x = sinf(ratio) * (sinf(rot.z) * m_fDistance_LOVE[i]);
			pos.y = sinf(ratio) * (cosf(rot.z) * m_fDistance_LOVE[i]);
			pos += m_pLOVE[i]->GetOriginPosition() + DestPoint::LOVE[i];
			m_pLOVE[i]->SetPosition(pos);
		}
	}

	if (m_fStateTime >= StateTime::AFTERMOVEMENT * 2.0f)
	{
		// LOVE�̊Ԋu�����_���I��
		DrawingLOVEDistance();

		// �^�C�}�[���Z�b�g
		m_fStateTime = 0.0f;
	}
}

//==========================================================================
// LOVE�̊Ԋu�����_���I��
//==========================================================================
void CTitleLogo::DrawingLOVEDistance()
{
	for (int i = 0; i < LOGO_LOVE::MAX; i++)
	{
		m_fDistance_LOVE[i] = 8.0f + UtilFunc::Transformation::Random(-30, 30) * 0.1f;
		m_fDistance_LOVE[i] *= -1;
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTitleLogo::Draw()
{
	
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CTitleLogo::SetState(State state)
{ 
	m_state = state; 
	m_fStateTime = 0.0f;
}
