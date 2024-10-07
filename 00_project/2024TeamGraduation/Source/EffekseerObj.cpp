//=============================================================================
// 
// �G�t�F�N�V�A�I�u�W�F�N�g���� [EffekseerObj.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "EffekseerObj.h"
#include "manager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	
}
CListManager<CEffekseerObj> CEffekseerObj::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEffekseerObj::CEffekseerObj()
{
	m_Label = CMyEffekseer::EFKLABEL::EFKLABEL_SAMPLE_LASER;	// �G�t�F�N�g�̃��x��
	m_bDeath = false;		// ���S�t���O
	m_bDeleteLater = false;	// ��Ŏ��S�t���O
	m_fDeleteTimer = 0.0f;	// ���S�^�C�}�[
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEffekseerObj::~CEffekseerObj()
{

}

//==========================================================================
// ��������
//==========================================================================
CEffekseerObj* CEffekseerObj::Create(const CMyEffekseer::EFKLABEL& label, const MyLib::Vector3& pos, const MyLib::Vector3& rot, const MyLib::Vector3 move, float scale, bool bAutoDeath)
{
	// �������̊m��
	CEffekseerObj* pObj = DEBUG_NEW CEffekseerObj;

	if (pObj != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		pObj->m_EffekseerInfo = CMyEffekseer::MyEffekseerInfo(pos, rot, move, scale, nullptr, 0, bAutoDeath);	// �G�t�F�N�g���
		pObj->m_Label = label;

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEffekseerObj::Init()
{
	// �}�l�[�W���擾
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return E_FAIL;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// �G�t�F�N�g�ǂݍ���
	m_EffekseerInfo.efcRef = pMyEffekseer->LoadEffect(m_Label);

	// �G�t�F�N�g�̍Đ�
	m_EffekseerInfo.handle = efkManager->Play(m_EffekseerInfo.efcRef, 0.0f, 0.0f, 0.0f);

	// ���ݒ�
	efkManager->SetLocation(m_EffekseerInfo.handle, m_EffekseerInfo.pos.x, m_EffekseerInfo.pos.y, m_EffekseerInfo.pos.z);
	efkManager->SetRotation(m_EffekseerInfo.handle, m_EffekseerInfo.rot.x, m_EffekseerInfo.rot.y, m_EffekseerInfo.rot.z);
	efkManager->SetScale(m_EffekseerInfo.handle, m_EffekseerInfo.scale, m_EffekseerInfo.scale, m_EffekseerInfo.scale);

	// ���X�g�ǉ�
	m_List.Regist(this);
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEffekseerObj::Uninit()
{
	// �}�l�[�W���擾
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// �G�t�F�N�g���~����
	efkManager->StopEffect(m_EffekseerInfo.handle);

	// ���X�g�폜
	m_List.Delete(this);

	delete this;
}

//==========================================================================
// ��~
//==========================================================================
void CEffekseerObj::Stop()
{

}

//==========================================================================
// �X�V����
//==========================================================================
void CEffekseerObj::Update()
{
	// �}�l�[�W���擾
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// ���S
	if (m_bDeath)
	{
		return;
	}

	// ��ŏ���
	if (m_bDeleteLater)
	{
		m_fDeleteTimer -= CManager::GetInstance()->GetDeltaTime();
		if (m_fDeleteTimer <= 0.0f)
		{// �폜
			m_bDeath = true;
			Uninit();
			return;
		}
	}

	if (!efkManager->Exists(m_EffekseerInfo.handle))
	{// �Đ��I��

		// �G�t�F�N�g���~����
		efkManager->StopEffect(m_EffekseerInfo.handle);

		// �����폜
		if (m_EffekseerInfo.bAutoDeath)
		{
			// ���S�t���OON
			m_bDeath = true;

			Uninit();
			return;
		}
		else
		{// �����폜���Ȃ�(���[�v)

			// �V���ɃG�t�F�N�g���Đ��������B
			// �ʒu���]�A�g��k�����ݒ�
			m_EffekseerInfo.handle = efkManager->Play(m_EffekseerInfo.efcRef, 0.0f, 0.0f, 0.0f);
			efkManager->SetLocation(m_EffekseerInfo.handle, m_EffekseerInfo.pos.x, m_EffekseerInfo.pos.y, m_EffekseerInfo.pos.z);
			efkManager->SetRotation(m_EffekseerInfo.handle, m_EffekseerInfo.rot.x, m_EffekseerInfo.rot.y, m_EffekseerInfo.rot.z);
			efkManager->SetScale(m_EffekseerInfo.handle, m_EffekseerInfo.scale, m_EffekseerInfo.scale, m_EffekseerInfo.scale);
		}
	}
	else
	{// �Đ���

		if (!m_EffekseerInfo.move.IsNearlyZero(0.01f))
		{
			// �G�t�F�N�g�̈ړ�
			efkManager->AddLocation(
				m_EffekseerInfo.handle,
				Effekseer::Vector3D(m_EffekseerInfo.move.x, m_EffekseerInfo.move.y, m_EffekseerInfo.move.z));
		}

		// �g�嗦
		Effekseer::Vector3D scale;
		efkManager->GetMatrix(m_EffekseerInfo.handle).GetScale(scale);
		efkManager->SetScale(m_EffekseerInfo.handle, m_EffekseerInfo.scale, m_EffekseerInfo.scale, m_EffekseerInfo.scale);
	}
}

//==========================================================================
// �g���K�[���M
//==========================================================================
void CEffekseerObj::SetTrigger(int trigger)
{
	// �}�l�[�W���擾
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// �g���K�[���M
	efkManager->SendTrigger(m_EffekseerInfo.handle, trigger);
}

//==========================================================================
// ��ŏ���
//==========================================================================
void CEffekseerObj::DeleteLater(float time)
{
	m_bDeleteLater = true;	// ��Ŏ��S�t���O
	m_fDeleteTimer = time;	// ���S�^�C�}�[
}

//==========================================================================
// �`�揈��
//==========================================================================
void CEffekseerObj::Draw()
{
	
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CEffekseerObj::SetPosition(const MyLib::Vector3& pos)
{
	// �}�l�[�W���擾
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// �ʒu�ݒ�
	m_EffekseerInfo.pos = pos;
	efkManager->SetLocation(m_EffekseerInfo.handle, Effekseer::Vector3D(pos.x, pos.y, pos.z));
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CEffekseerObj::SetRotation(const MyLib::Vector3& rot)
{
	// �}�l�[�W���擾
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// �����ݒ�
	m_EffekseerInfo.rot = rot;
	efkManager->SetRotation(m_EffekseerInfo.handle, rot.x, rot.y, rot.z);
}

//==========================================================================
// �}�g���b�N�X�ݒ�
//==========================================================================
void CEffekseerObj::SetMatrix(const MyLib::Matrix& mtx)
{
	// �}�l�[�W���擾
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// �O�Ղ̃}�g���b�N�X�擾
	Effekseer::Matrix43 efcmtx = GetMatrix();

	// 4x3�s��Ɍ�����ݒ�
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			efcmtx.Value[i][j] = mtx.m[i][j];
		}
	}

	// �G�t�F�N�g�̃C���X�^���X�ɕϊ��s���ݒ�
	efkManager->SetMatrix(m_EffekseerInfo.handle, efcmtx);

	// �X�P�[�����ݒ�
	efkManager->SetScale(m_EffekseerInfo.handle, m_EffekseerInfo.scale, m_EffekseerInfo.scale, m_EffekseerInfo.scale);
}

//==========================================================================
// �}�g���b�N�X�擾
//==========================================================================
Effekseer::Matrix43 CEffekseerObj::GetMatrix()
{
	// �}�l�[�W���擾
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return Effekseer::Matrix43();
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	return efkManager->GetMatrix(m_EffekseerInfo.handle);
}

//==========================================================================
// �X�P�[���ݒ�
//==========================================================================
void CEffekseerObj::SetScale(const float scale)
{
	// �}�l�[�W���擾
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// �X�P�[���ݒ�
	m_EffekseerInfo.scale = scale;
	efkManager->SetScale(m_EffekseerInfo.handle, scale, scale, scale);
}