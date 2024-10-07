//=============================================================================
// 
//  HP�Q�[�W�̐�[���� [hp_gauge_tip_billboard.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "hp_gauge_tip_billboard.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CHPGaugeTip_Billboard::CHPGaugeTip_Billboard(int nPriority) : CHPGaugeTip(nPriority)
{
	// �l�̃N���A
	memset(m_pObjBillboard, 0, sizeof(m_pObjBillboard));
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CHPGaugeTip_Billboard::~CHPGaugeTip_Billboard()
{

}

//==========================================================================
// ��[����
//==========================================================================
void CHPGaugeTip_Billboard::CreateTip(int nTexIdx)
{
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nTexIdx);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 15.0f);

	for (int i = 0; i < VTXTYPE_MAX; i++)
	{
		// ��������
		m_pObjBillboard[i] = CObjectBillboard::Create(9);
		if (m_pObjBillboard[i] == nullptr)
		{
			return;
		}
		CObjectBillboard* pObj = m_pObjBillboard[i];

		pObj->SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);

		// �e�N�X�`�����蓖��
		pObj->BindTexture(nTexIdx);

		// ���ݒ�
		pObj->SetSize(size);
		pObj->SetSizeOrigin(size);

		float angle = 0.0f;

		switch (i)
		{
		case VTXTYPE::VTXTYPE_LEFT:
			break;

		case VTXTYPE::VTXTYPE_RIGHT:
			angle = D3DX_PI;
			break;
		}

		// �����ݒ�
		pObj->SetRotation(MyLib::Vector3(0.0f, 0.0f, angle));
		pObj->SetOriginRotation(pObj->GetRotation());
	}
}

//==========================================================================
// �I������
//==========================================================================
void CHPGaugeTip_Billboard::Uninit()
{
	for (int i = 0; i < VTXTYPE_MAX; i++)
	{
		m_pObjBillboard[i] = nullptr;
	}
	Release();
}

//==========================================================================
// �I������
//==========================================================================
void CHPGaugeTip_Billboard::Kill()
{
	for (int i = 0; i < VTXTYPE_MAX; i++)
	{
		if (m_pObjBillboard[i] != nullptr)
		{
			// �I������
			m_pObjBillboard[i]->Uninit();
			m_pObjBillboard[i] = nullptr;
		}
	}

	Release();
}

void CHPGaugeTip_Billboard::SetLeftPosition(const MyLib::Vector3& pos)
{
	m_pObjBillboard[VTXTYPE::VTXTYPE_LEFT]->SetPosition(pos);
}

void CHPGaugeTip_Billboard::SetRightPosition(const MyLib::Vector3& pos)
{
	m_pObjBillboard[VTXTYPE::VTXTYPE_RIGHT]->SetPosition(pos);
}
