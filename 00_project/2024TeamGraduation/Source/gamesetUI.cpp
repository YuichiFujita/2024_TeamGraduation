//============================================================
//
//	�Q�[���Z�b�gUI���� [gamesetUI.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gamesetUI.h"
#include "manager.h"
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// UI�̗D�揇��
	namespace gameset
	{
		const char* TEXTURE = "data\\TEXTURE\\gameset000.png";	// ����\�L�̔w�i
		const MyLib::Vector3 INIT_POS = MyLib::Vector3(-VEC3_SCREEN_SIZE.x, 360.0f, 0.0f);		// �����ʒu
		const MyLib::Vector3 DEST_POS = MyLib::Vector3(VEC3_SCREEN_CENT.x, INIT_POS.y, 0.0f);	// �ڕW�ʒu
		const float MOVE_TIME = 0.75f;	// �ړ�����
		const float WIDTH = 580.0f;		// ����
	}
}

//************************************************************
//	�q�N���X [CGameSetUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGameSetUI::CGameSetUI() : CObject(PRIORITY, LAYER::LAYER_2D),
	m_pGameSet	(nullptr),	// �Q�[���Z�b�g���
	m_fCurTime	(0.0f)		// �o�ߎ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGameSetUI::~CGameSetUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGameSetUI::Init()
{
	// UI�̐���
	if (FAILED(CreateUI()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ��I�u�W�F�N�g2D�ɂ���
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// ���E��~���ɓ�����悤�ɂ���
	SetEnablePosibleMove_WorldPause(true);

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGameSetUI::Uninit()
{
	// ���ɔj���ς݂̏ꍇ������
	if (IsDeath()) { return; }

	// �Q�[���Z�b�g�̏I��
	SAFE_UNINIT(m_pGameSet);

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�폜����
//============================================================
void CGameSetUI::Kill()
{
	// ���g�̏I��
	CGameSetUI::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGameSetUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �o�ߎ��Ԃ����Z
	m_fCurTime += fDeltaTime * fSlowRate;

	// �w�i�̈ʒu���ړ�
	MyLib::Vector3 posGameSet = UtilFunc::Correction::EaseOutBack(gameset::INIT_POS, gameset::DEST_POS, 0.0f, gameset::MOVE_TIME, m_fCurTime, 1.0f);
	m_pGameSet->SetPosition(posGameSet);
}

//============================================================
//	�`�揈��
//============================================================
void CGameSetUI::Draw()
{

}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CGameSetUI::SetEnableDisp(const bool bDisp)
{
	// ���g�̕`��󋵂�ݒ�
	CObject::SetEnableDisp(bDisp);

	// �Q�[���Z�b�g�̕`��󋵂�ݒ�
	m_pGameSet->SetEnableDisp(bDisp);
}

//============================================================
//	��������
//============================================================
CGameSetUI *CGameSetUI::Create()
{
	// �C���X�^���X�̐���
	CGameSetUI* pTrans = DEBUG_NEW CGameSetUI;
	if (pTrans != nullptr)
	{ // �����ɐ��������ꍇ

		// �N���X�̏�����
		if (FAILED(pTrans->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pTrans);
			return nullptr;
		}
	}

	// �C���X�^���X��Ԃ�
	return pTrans;
}

//============================================================
//	���o�I���̊m�F����
//============================================================
bool CGameSetUI::IsEnd()
{
	// ���o�I���t���O��Ԃ�
	return (m_fCurTime >= gameset::MOVE_TIME);
}

//============================================================
//	UI�̐�������
//============================================================
HRESULT CGameSetUI::CreateUI()
{
	CTexture* pTexture = CTexture::GetInstance();	// �e�N�X�`�����

	// �w�i�̐���
	m_pGameSet = CObject2D::Create(PRIORITY);
	if (m_pGameSet == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �e�N�X�`���̊���
	int texID = pTexture->Regist(gameset::TEXTURE);
	m_pGameSet->BindTexture(texID);

	// �ʒu��ݒ�
	m_pGameSet->SetPosition(gameset::INIT_POS);

	// ���������ɃT�C�Y�v�Z
	MyLib::Vector2 size = pTexture->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, gameset::WIDTH);
	m_pGameSet->SetSize(size);

	// ���E��~���ɓ�����悤�ɂ���
	m_pGameSet->SetEnablePosibleMove_WorldPause(true);

	return S_OK;
}
