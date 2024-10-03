//=============================================================================
// 
//  HP�Q�[�W�̐�[���� [hp_gauge_tip.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "hp_gauge_tip.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "hp_gauge_tip_billboard.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\hpgauge\\tip.png";	// �e�N�X�`���t�@�C��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CHPGaugeTip::CHPGaugeTip(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	memset(m_pObj2D, 0, sizeof(m_pObj2D));
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CHPGaugeTip::~CHPGaugeTip()
{

}

//==========================================================================
// ��������
//==========================================================================
CHPGaugeTip* CHPGaugeTip::Create(const MyLib::Vector3& leftpos, const MyLib::Vector3& rightpos, TYPE type)
{
	// �������̊m��
	CHPGaugeTip* pMarker = nullptr;

	switch (type)
	{
	case CHPGaugeTip::TYPE_2D:
		pMarker = DEBUG_NEW CHPGaugeTip;
		break;

	case CHPGaugeTip::TYPE_BILLBOARD:
		pMarker = DEBUG_NEW CHPGaugeTip_Billboard;
		break;

	default:
		return pMarker;
		break;
	}

	if (pMarker != nullptr)
	{
		// ����������
		pMarker->Init(leftpos, rightpos);
	}

	return pMarker;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CHPGaugeTip::Init(const MyLib::Vector3& leftpos, const MyLib::Vector3& rightpos)
{
	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);

	CreateTip(nTexIdx);

	return S_OK;
}

//==========================================================================
// ��[����
//==========================================================================
void CHPGaugeTip::CreateTip(int nTexIdx)
{
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nTexIdx);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 15.0f);

	for (int i = 0; i < VTXTYPE_MAX; i++)
	{
		// ��������
		m_pObj2D[i] = CObject2D::Create(GetPriority());
		if (m_pObj2D[i] == nullptr)
		{
			return;
		}
		CObject2D* pObj = m_pObj2D[i];

		// �e�N�X�`�����蓖��
		pObj->BindTexture(nTexIdx);

		// ���ݒ�
		pObj->SetSize(size);
		pObj->SetSizeOrigin(size);

		float angle = 0.0f;

		//MyLib::Vector3 pos = leftpos;
		switch (i)
		{
		case VTXTYPE::VTXTYPE_LEFT:
			break;

		case VTXTYPE::VTXTYPE_RIGHT:
			angle = D3DX_PI;
			//pos = rightpos;
			break;
		}

		// �����ݒ�
		pObj->SetRotation(MyLib::Vector3(0.0f, 0.0f, angle));
		pObj->SetOriginRotation(pObj->GetRotation());

		//pObj->SetPosition(pos);
		pObj->SetOriginPosition(pObj->GetPosition());
	}
}

//==========================================================================
// �I������
//==========================================================================
void CHPGaugeTip::Uninit()
{
	for (int i = 0; i < VTXTYPE_MAX; i++)
	{
		m_pObj2D[i] = nullptr;
	}
	Release();
}

//==========================================================================
// �I������
//==========================================================================
void CHPGaugeTip::Kill()
{
	for (int i = 0; i < VTXTYPE_MAX; i++)
	{
		if (m_pObj2D[i] != nullptr)
		{
			// �I������
			m_pObj2D[i]->Uninit();
			m_pObj2D[i] = nullptr;
		}
	}
	Release();

}

//==========================================================================
// �X�V����
//==========================================================================
void CHPGaugeTip::Update()
{
	
}

void CHPGaugeTip::SetLeftPosition(const MyLib::Vector3& pos)
{
	m_pObj2D[VTXTYPE::VTXTYPE_LEFT]->SetPosition(pos);
}

void CHPGaugeTip::SetRightPosition(const MyLib::Vector3& pos)
{
	m_pObj2D[VTXTYPE::VTXTYPE_RIGHT]->SetPosition(pos);
}
