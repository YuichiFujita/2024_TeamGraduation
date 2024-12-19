//=============================================================================
// 
//  �I�v�V�����̌f������ [option_BBS.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "option_BBS.h"
#include "renderTexture.h"
#include "balloonFrame.h"
#include "camera.h"
#include "objectX.h"
#include "optionMenu.h"

#if 0
#define EDIT()	// 2D�ŃG�f�B�b�g
#endif

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string MODEL = "data\\MODEL\\title\\BBS.x";			// �f���̃��f��
	const MyLib::Vector3 DEFAULT_POSITION = MyLib::Vector3(0.0f, 80.0f, 0.0f);	// �����ʒu
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
COption_BBS::COption_BBS(int nPriority) : CObjectX(nPriority),
	m_pRenderScene	(nullptr),	// �V�[�������_�[�e�N�X�`��
	m_pOptionMenu	(nullptr)	// �I�v�V�������j���[
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
COption_BBS::~COption_BBS()
{

}

//==========================================================================
// ��������
//==========================================================================
COption_BBS* COption_BBS::Create()
{
	// �������̊m��
	COption_BBS* pObj = DEBUG_NEW COption_BBS;
	if (pObj != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pObj->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT COption_BBS::Init()
{
	// �I�u�W�F�N�gX�̏�����
	if (FAILED(CObjectX::Init(MODEL)))
	{ // �������Ɏ��s�����ꍇ
		return E_FAIL;
	}

	// �ʒu�ƌ����ݒ�
	SetPosition(DEFAULT_POSITION);
	SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));

	// �����_�[�e�N�X�`���̐���
	if (FAILED(CreateRenderTexture()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �e�N�X�`���p�I�u�W�F�N�g�̐���
	if (FAILED(CreateTextureObject()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �e�N�X�`���C���f�b�N�X�̐ݒ�
	int* pIdxTexture = GetIdxTexture();
	pIdxTexture[0] = m_pRenderScene->GetTextureIndex();
	BindTexture(pIdxTexture);

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECTX);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void COption_BBS::Uninit()
{
	// �����_�[�e�N�X�`���̔j��
	SAFE_REF_RELEASE(m_pRenderScene);


	// �I�u�W�F�N�gX�̏I��
	CObjectX::Uninit();
}

//==========================================================================
// �폜����
//==========================================================================
void COption_BBS::Kill()
{
	
	// �I�u�W�F�N�gX�̏I��
	CObjectX::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void COption_BBS::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �f���̍X�V
	assert(m_pOptionMenu != nullptr);
#ifndef EDIT
	m_pOptionMenu->Update(fDeltaTime, fDeltaRate, fSlowRate);
#endif

	// �I�u�W�F�N�gX�̍X�V
	CObjectX::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void COption_BBS::Draw()
{
	// �I�u�W�F�N�gX�̕`��
	CObjectX::Draw();
}

//==========================================================================
// �����_�[�e�N�X�`����������
//==========================================================================
HRESULT COption_BBS::CreateRenderTexture()
{
	// �V�[�������_�[�e�N�X�`���̐���
	m_pRenderScene = CRenderTexture::Create
	( // ����
		CRenderTextureManager::LAYER_BBS,						// �`�揇���C���[
		std::bind(&COption_BBS::CreateTexture, this),			// �e�N�X�`���쐬�֐��|�C���^
		std::bind(&CCamera::SetCamera, GET_MANAGER->GetCamera())	// �J�����ݒ�֐��|�C���^
	);
	if (m_pRenderScene == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �e�N�X�`���p�I�u�W�F�N�g��������
//==========================================================================
HRESULT COption_BBS::CreateTextureObject()
{
	// �I�v�V�������j���[����
	m_pOptionMenu = COptionMenu::Create();
#ifdef EDIT
	m_pOptionMenu->SetType(CObject::TYPE::TYPE_OBJECT2D);
#else
	m_pOptionMenu->SetType(CObject::TYPE::TYPE_NONE);
#endif // EDIT

	return S_OK;
}

//==========================================================================
// �e�N�X�`���쐬����
//==========================================================================
void COption_BBS::CreateTexture()
{
	// �I�v�V�������j���[�̕`��
	assert(m_pOptionMenu != nullptr);
	m_pOptionMenu->Draw();

}
