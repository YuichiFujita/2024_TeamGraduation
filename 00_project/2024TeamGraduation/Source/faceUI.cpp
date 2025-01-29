//============================================================
//
//	�v���C���[��UI���� [faceUI.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "faceUI.h"
#include "camera.h"
#include "renderTexture.h"
#include "object2D.h"
#include "object2D_Anim.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// �v���C���[��UI�̗D�揇��

	namespace player
	{
		namespace idx
		{
			const std::string	  TEXTURE = "data\\TEXTURE\\playerIdx000.png";					// �e�N�X�`��
			const MyLib::PosGrid2 PTRN	  = MyLib::PosGrid2(mylib_const::MAX_PLAYER + 1, 1);	// �e�N�X�`��������
			const MyLib::Vector3  OFFSET  = MyLib::Vector3(33.5f, -52.0f, 0.0f);	// �I�t�Z�b�g
			const float WIDTH = 25.0f;	// ����
		}
		namespace face
		{
			const MyLib::Vector3 OFFSET = MyLib::Vector3(30.0f, 0.0f, 0.0f);	// �I�t�Z�b�g
			const float WIDTH = 140.0f;	// ����
		}
	}
}

//************************************************************
//	�q�N���X [CFaceUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFaceUI::CFaceUI(const CPlayer* pPlayer) : CObject(PRIORITY, LAYER::LAYER_2D),
	m_pPlayerParent	(pPlayer),	// �e�v���C���[
	m_pRenderScene	(nullptr),	// �V�[�������_�[�e�N�X�`��
	m_pPlayerIdxUI	(nullptr),	// �v���C���[�C���f�b�N�XUI���
	m_pPlayerUI		(nullptr),	// �v���C���[UI���
	m_pPlayer		(nullptr)	// �v���C���[
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CFaceUI::~CFaceUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CFaceUI::Init()
{
	// �v���C���[�̐���
	if (FAILED(CreatePlayer()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �����_�[�e�N�X�`���̐���
	if (FAILED(CreateRenderTexture()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// UI�̐���
	if (FAILED(CreateUI()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ��I�u�W�F�N�g2D�ɂ���
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CFaceUI::Uninit()
{
	// �����_�[�e�N�X�`���̔j��
	SAFE_REF_RELEASE(m_pRenderScene);

	// �v���C���[�C���f�b�N�XUI�̏I��
	SAFE_UNINIT(m_pPlayerIdxUI);

	// �v���C���[UI�̏I��
	SAFE_UNINIT(m_pPlayerUI);

	// �v���C���[�̏I��
	SAFE_UNINIT(m_pPlayer);

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�폜����
//============================================================
void CFaceUI::Kill()
{
	// �v���C���[�̍폜
	SAFE_KILL(m_pPlayer);

	// ���g�̏I��
	CFaceUI::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CFaceUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���Έʒu�̐ݒ�
	SetPositionRelative();

	// �v���C���[UI�̍X�V
	m_pPlayerUI->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �v���C���[�C���f�b�N�XUI�̍X�V
	m_pPlayerIdxUI->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �v���C���[�̍X�V
	m_pPlayer->Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//============================================================
//	�`�揈��
//============================================================
void CFaceUI::Draw()
{
	// �v���C���[UI�̕`��
	m_pPlayerUI->Draw();

	// �v���C���[�C���f�b�N�XUI�̕`��
	m_pPlayerIdxUI->Draw();
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CFaceUI::SetEnableDisp(const bool bDraw)
{
	// ���g�̕`��󋵂�ݒ�
	CObject::SetEnableDisp(bDraw);

	// �v���C���[UI�̕`��󋵂�ݒ�
	m_pPlayerUI->SetEnableDisp(bDraw);

	// �v���C���[�C���f�b�N�XUI�̕`��󋵂�ݒ�
	m_pPlayerIdxUI->SetEnableDisp(bDraw);
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CFaceUI::SetPosition(const MyLib::Vector3& pos)
{
	// �ݒ肷��ʒu��ۑ�
	CObject::SetPosition(pos);

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�����ւ�����
//============================================================
void CFaceUI::BindDressUp(int nHair, int nAccessory, int nFace)
{
	// �����ւ�����
	m_pPlayer->BindDressUp(nHair, nAccessory, nFace);
}

//============================================================
//	�����̃C���f�b�N�X�ݒ�
//============================================================
void CFaceUI::SetMyPlayerIdx(int nIdx)
{
	m_pPlayer->SetMyPlayerIdx(nIdx);
	m_pPlayerIdxUI->SetPatternAnim(nIdx);
}

//============================================================
//	��������
//============================================================
CFaceUI *CFaceUI::Create(const CPlayer* pPlayer, const MyLib::Vector3& rPos)
{
	// �v���C���[��UI�̐���
	CFaceUI* pFaceUI = DEBUG_NEW CFaceUI(pPlayer);
	if (pFaceUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �v���C���[��UI�̏�����
		if (FAILED(pFaceUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �v���C���[��UI�̔j��
			SAFE_DELETE(pFaceUI);
			return nullptr;
		}

		// �ʒu��ݒ�
		pFaceUI->SetPosition(rPos);

		// �m�ۂ����A�h���X��Ԃ�
		return pFaceUI;
	}
}

//============================================================
// UI�̐�������
//============================================================
HRESULT CFaceUI::CreateUI()
{
	// �v���C���[UI�̐���
	if (FAILED(CreatePlayerUI()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �v���C���[�C���f�b�N�XUI�̐���
	if (FAILED(CreatePlayerIdxUI()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();

	return S_OK;
}

//============================================================
// �v���C���[UI�̐�������
//============================================================
HRESULT CFaceUI::CreatePlayerUI()
{
	// �v���C���[UI�̐���
	m_pPlayerUI = CObject2D::Create(PRIORITY);
	if (m_pPlayerUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ������j��/�X�V/�`�悵�Ȃ����̂ɂ���
	m_pPlayerUI->SetType(CObject::TYPE::TYPE_NONE);

	// �e�N�X�`���̊���
	const int nTexID = m_pRenderScene->GetTextureIndex();
	m_pPlayerUI->BindTexture(nTexID);

	// ���������ɃT�C�Y��ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, player::face::WIDTH);
	m_pPlayerUI->SetSize(size);
	m_pPlayerUI->SetSizeOrigin(m_pPlayerUI->GetSize());

	return S_OK;
}

//============================================================
// �v���C���[�C���f�b�N�XUI�̐�������
//============================================================
HRESULT CFaceUI::CreatePlayerIdxUI()
{
	// �v���C���[�C���f�b�N�XUI�̐���
	m_pPlayerIdxUI = CObject2D_Anim::Create
	( // ����
		VEC3_ZERO,				// �ʒu
		player::idx::PTRN.x,	// �e�N�X�`����������
		player::idx::PTRN.y,	// �e�N�X�`���c������
		0.0f,		// �Đ�����
		false,		// �����j��
		PRIORITY	// �D�揇��
	);
	if (m_pPlayerIdxUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ������j��/�X�V/�`�悵�Ȃ����̂ɂ���
	m_pPlayerIdxUI->SetType(CObject::TYPE::TYPE_NONE);

	// �����Đ���OFF�ɂ���
	m_pPlayerIdxUI->SetEnableAutoPlay(false);

	// �e�N�X�`���p�^�[���̐ݒ�
	m_pPlayerIdxUI->SetPatternAnim(m_pPlayerParent->GetMyPlayerIdx());

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(player::idx::TEXTURE);
	m_pPlayerIdxUI->BindTexture(nTexID);

	// ���������ɃT�C�Y��ݒ�
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, player::idx::WIDTH);
	size.y *= (float)player::idx::PTRN.x;
	m_pPlayerIdxUI->SetSize(size);
	m_pPlayerIdxUI->SetSizeOrigin(m_pPlayerIdxUI->GetSize());

	return S_OK;
}

//============================================================
// �v���C���[�̐�������
//============================================================
HRESULT CFaceUI::CreatePlayer()
{
	// �v���C���[����
	m_pPlayer = CPlayer::Create
	( // ����
		VEC3_ZERO,	// �ʒu
		m_pPlayerParent->GetTeam(),			// �`�[���T�C�h
		CPlayer::EHuman::HUMAN_FACEICON,	// �|�W�V����
		m_pPlayerParent->GetBodyType(),		// �̌n
		m_pPlayerParent->GetHandedness()	// ������
	);
	if (m_pPlayer == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ������j��/�X�V/�`�悵�Ȃ����̂ɂ���
	m_pPlayer->SetType(CObject::TYPE::TYPE_NONE);

	// �C���f�b�N�X�̏㏑��
	m_pPlayer->SetMyPlayerIdx(m_pPlayerParent->GetMyPlayerIdx());

	return S_OK;
}

//============================================================
// �����_�[�e�N�X�`����������
//============================================================
HRESULT CFaceUI::CreateRenderTexture()
{
	// �V�[�������_�[�e�N�X�`���̐���
	m_pRenderScene = CRenderTexture::Create
	( // ����
		CRenderTextureManager::LAYER_PLAYER,		// �`�揇���C���[
		std::bind(&CFaceUI::CreateTexture, this),	// �e�N�X�`���쐬�֐��|�C���^
		std::bind(&CCamera::SetCameraFace, GET_MANAGER->GetCamera())	// �J�����ݒ�֐��|�C���^
	);
	if (m_pRenderScene == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
// �e�N�X�`���쐬����
//============================================================
void CFaceUI::CreateTexture()
{
	// �v���C���[�̕`��
	assert(m_pPlayer != nullptr);
	m_pPlayer->Draw();
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CFaceUI::SetPositionRelative()
{
	// ���g�̈ʒu���擾
	MyLib::Vector3 posThis = GetPosition();

	// �v���C���[�C���f�b�N�XUI�̈ʒu�ݒ�
	m_pPlayerIdxUI->SetPosition(posThis + player::idx::OFFSET);

	// �v���C���[UI�̈ʒu�ݒ�
	m_pPlayerUI->SetPosition(posThis + player::face::OFFSET);
}
