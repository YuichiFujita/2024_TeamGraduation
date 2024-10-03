//=============================================================================
// 
//  �퓬�������� [beforebattle.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "beforebattle.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "input.h"
#include "battlestart.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\battlestart\\yoi.png";	// �e�N�X�`���̃t�@�C��
	const std::string PRESS_TEXTURE = "data\\TEXTURE\\battlestart\\beforebattle_button.png";	// �e�N�X�`���̃t�@�C��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBeforeBattle::CBeforeBattle(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_pAnyButton= nullptr;	// �u�L�[�����āv�̃e�L�X�g
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBeforeBattle::~CBeforeBattle()
{

}

//==========================================================================
// ��������
//==========================================================================
CBeforeBattle* CBeforeBattle::Create(const MyLib::Vector3& pos)
{
	// �������̊m��
	CBeforeBattle* pMarker = DEBUG_NEW CBeforeBattle;

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
HRESULT CBeforeBattle::Init()
{

	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// �e�N�X�`���ݒ�
	int pointTex = CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	BindTexture(pointTex);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(pointTex);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
	SetSize(size);
	SetSizeOrigin(size);

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);



	// �e�L�X�g����
	m_pAnyButton = CObject2D::Create(GetPriority());
	m_pAnyButton->SetPosition(GetPosition() + MyLib::Vector3(0.0f, 70.0f, 0.0f));

	// �e�N�X�`���ݒ�
	int textTex = CTexture::GetInstance()->Regist(PRESS_TEXTURE);
	m_pAnyButton->BindTexture(textTex);

	// �T�C�Y�ݒ�
	size = CTexture::GetInstance()->GetImageSize(textTex);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, 30.0f);
	m_pAnyButton->SetSize(size);
	m_pAnyButton->SetSizeOrigin(size);
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CBeforeBattle::Uninit()
{
	if (m_pAnyButton != nullptr)
	{
		m_pAnyButton->Uninit();
		m_pAnyButton = nullptr;
	}

	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CBeforeBattle::Update()
{
	// ���͏��擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_Y, 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_START, 0) ||
		pInputKeyboard->GetTrigger(DIK_RETURN) ||
		pInputKeyboard->GetTrigger(DIK_SPACE) ||
		pInputKeyboard->GetTrigger(DIK_BACKSPACE) 
		)
	{
		// �퓬�J�n�ɑJ��
		CBattleStart::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f));
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_BATTLESTART);

		// �I������
		Uninit();
		return;
	}

	// �X�V����
	CObject2D::Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CBeforeBattle::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw();

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}
