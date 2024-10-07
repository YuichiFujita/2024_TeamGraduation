//======================================================
//
//�f�o�C�X���X�g�ȂǂŔj�����K�v�Ȃ��̃}�l�[�W���[[lostrssmanager.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "lostrssmanager.h"
#include "Imguimanager.h"
#include "MyEffekseer.h"
#include "manager.h"
#include "light.h"

CLostResourceManager* CLostResourceManager::m_pInstance = nullptr;
//=================================
//�R���X�g���N�^
//=================================
CLostResourceManager::CLostResourceManager()
{
	m_lostResources.clear();
}

//=================================
//�f�X�g���N�^
//=================================
CLostResourceManager::~CLostResourceManager()
{
	
}

//=================================
//�C���X�^���X�擾�i�Ȃ���ΐ����j
//=================================
CLostResourceManager* CLostResourceManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CLostResourceManager;
	}
	return m_pInstance;
}

//=================================
//�C���X�^���X�j��
//=================================
void CLostResourceManager::Release()
{
	if (m_pInstance != nullptr)
	{
		m_pInstance->m_lostResources.clear();	//���X�g��ɂ���
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

//=================================
//���X�g������̓o�^
//�w�b�_�ōς񂶂�����B
//=================================

//=================================
//���X�g������̍폜
//=================================
void CLostResourceManager::Remove(ILostResource* lostrss)
{
	m_lostResources.erase(std::remove(m_lostResources.begin(), m_lostResources.end(), lostrss));
}

//=================================
//���X�g������̂�������o�b�N�A�b�v
//=================================
void CLostResourceManager::Backup()
{
	// ����n
	for (auto itr = m_lostResources.begin(); itr != m_lostResources.end(); itr++)
	{
		(*itr)->Backup();
	}

	// �O�����C�u����
	ImguiMgr::Backup();

	auto efkRenderer = CMyEffekseer::GetInstance()->GetEffekseerRenderer();
	efkRenderer->OnLostDevice();
}

//=================================
//���X�g������̕���
//=================================
void CLostResourceManager::Restore()
{
	// �O�����C�u����
	auto efkRenderer = CMyEffekseer::GetInstance()->GetEffekseerRenderer();
	efkRenderer->OnResetDevice();

	ImguiMgr::Restore();

	// ����n
	for (auto itr = m_lostResources.begin(); itr != m_lostResources.end(); itr++)
	{
		(*itr)->Restore();
	}

	// ���C�g���Đݒ�
	CManager::GetInstance()->GetLight()->Init();
}
