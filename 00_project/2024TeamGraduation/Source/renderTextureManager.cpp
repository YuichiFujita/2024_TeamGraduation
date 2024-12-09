//==========================================================================
// 
//  �����_�[�e�N�X�`���}�l�[�W���[���� [renderTextureManager.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "renderTextureManager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CRenderTextureManager* CRenderTextureManager::m_pInstance = nullptr;	// ���g�̃C���X�^���X

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CRenderTextureManager::CRenderTextureManager()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CRenderTextureManager::~CRenderTextureManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CRenderTextureManager* CRenderTextureManager::Create()
{
	// ���ɐ����ς݂̏ꍇ������
	if (m_pInstance != nullptr) { return m_pInstance; }

	// �C���X�^���X�̐���
	m_pInstance = DEBUG_NEW CRenderTextureManager;
	if (m_pInstance != nullptr)
	{ // �������̊m�ۂ��o�����ꍇ

		// �C���X�^���X�̏�����
		if (FAILED(m_pInstance->Init()))
		{ // �������Ɏ��s�����ꍇ

			return nullptr;
		}
	}

	// �C���X�^���X��Ԃ�
	return m_pInstance;
}

//==========================================================================
// �C���X�^���X�擾����
//==========================================================================
CRenderTextureManager* CRenderTextureManager::GetInstance()
{
	// �C���X�^���X��Ԃ�
	return m_pInstance;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CRenderTextureManager::Init()
{
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CRenderTextureManager::Uninit()
{
	// ���g�̃C���X�^���X�̔j��
	SAFE_DELETE(m_pInstance);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CRenderTextureManager::Draw()
{
	for (int i = 0; i < ELayer::LAYER_MAX; i++)
	{ // ���C���[�̑������J��Ԃ�

		int nIdxLoop = 0;	// ���[�v��
		for (auto& rItrRenTex : m_vecRenderTexture[i])
		{ // �z��̗v�f�����J��Ԃ�

			// �����_�[�e�N�X�`���̕`��
			rItrRenTex->Draw();
		}
	}
}

//==========================================================================
// �����_�[�e�N�X�`���o�^����
//==========================================================================
void CRenderTextureManager::RegistRenderTexture(CRenderTexture* pRenderTexture, const ELayer layer)
{
	// �������C���[�̍Ō���Ƀ����_�[�e�N�X�`����ǉ�
	m_vecRenderTexture[layer].push_back(pRenderTexture);
}

//==========================================================================
// �����_�[�e�N�X�`���폜����
//==========================================================================
HRESULT CRenderTextureManager::DeleteRenderTexture(CRenderTexture* pRenderTexture)
{
	for (int i = 0; i < ELayer::LAYER_MAX; i++)
	{ // ���C���[�̑������J��Ԃ�

		int nIdxLoop = 0;	// ���[�v��
		for (auto& rItrRenTex : m_vecRenderTexture[i])
		{ // �z��̗v�f�����J��Ԃ�

			// ���������_�[�e�N�X�`���ƈႤ�ꍇ����
			if (rItrRenTex != pRenderTexture) { nIdxLoop++; continue; }

			// �����_�[�e�N�X�`���𓮓I�z�񂩂�폜
			m_vecRenderTexture[i].erase(m_vecRenderTexture[i].begin() + nIdxLoop);

			return S_OK;
		}
	}

	// �����ɗ����疳�������_�[�e�N�X�`�����폜���悤�Ƃ��Ă��
	assert(false);
	return E_FAIL;
}
