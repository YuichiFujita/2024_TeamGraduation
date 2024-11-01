//=============================================================================
// 
//  �I�u�W�F�N�g���� [object.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "object.h"
#include "manager.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CObject::m_nNumAll = 0;	// ����
int CObject::m_nNumPriorityAll[mylib_const::PRIORITY_NUM] = {};
CObject *CObject::m_apObject[mylib_const::PRIORITY_NUM][mylib_const::MAX_OBJ] = {};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObject::CObject(int nPriority)
{
	if (nPriority < 0 || nPriority >= mylib_const::PRIORITY_NUM)
	{// �͈͊O��������

		return;
	}

	// �l�̃N���A
	m_nID = -1;

	for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
	{
		if (m_apObject[nPriority][nCntObj] == NULL)
		{// NULL��������

			m_apObject[nPriority][nCntObj] = this;	// �������g�̃|�C���^���󂯎��
			m_nPriority = nPriority;	// �D�揇��
			m_nID = nCntObj;			// �������g��ID�ݒ�
			m_type = TYPE_NONE;			// ���
			m_nNumAll++;				// �������Z
			m_nNumPriorityAll[nPriority]++;

			return;
		}
	}

	if (m_nID == -1)
	{
		int n = 0;
	}

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObject::~CObject()
{

}

//==========================================================================
// �S�ẴI�u�W�F�N�g�̔j��
//==========================================================================
void CObject::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
		{
			if (m_apObject[nCntPriority][nCntObj] != NULL)
			{// �������̊m�ۂ�����Ă�����

				if (m_apObject[nCntPriority][nCntObj]->m_type != TYPE_NONE)
				{// NONE����Ȃ�������

					// �I������
					m_apObject[nCntPriority][nCntObj]->Uninit();
				}
			}
		}
	}
}

//==========================================================================
// �S�ẴI�u�W�F�N�g�̍X�V����
//==========================================================================
void CObject::UpdateAll(void)
{
#if _DEBUG
	// �G�f�B�b�g�̏��擾
	CEdit *pEdit = CManager::GetEdit();
#endif

	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
		{
			if (m_apObject[nCntPriority][nCntObj] != NULL)
			{// �������̊m�ۂ�����Ă�����

#if _DEBUG
				if (pEdit != NULL && (m_apObject[nCntPriority][nCntObj]->m_type == TYPE_EDIT || m_apObject[nCntPriority][nCntObj]->m_type == TYPE_XFILE || m_apObject[nCntPriority][nCntObj]->m_type == TYPE_ELEVATION))
				{// �G�f�B�b�g��Ԃ�������G�f�B�b�g�̂ݍX�V

					// �X�V����
					m_apObject[nCntPriority][nCntObj]->Update();
				}
				else if (pEdit == NULL && m_apObject[nCntPriority][nCntObj]->m_type != TYPE_NONE)
				{// �G�f�B�b�g��Ԃ���Ȃ� && �^�C�v��NONE�ȊO

					// �X�V����
					m_apObject[nCntPriority][nCntObj]->Update();
				}
#else
				if (m_apObject[nCntPriority][nCntObj]->m_type != TYPE_NONE)
				{// �^�C�v��NONE�ȊO

					// �X�V����
					m_apObject[nCntPriority][nCntObj]->Update();
				}
#endif
			}
		}
	}
}

//==========================================================================
// �S�ẴI�u�W�F�N�g�̕`�揈��
//==========================================================================
void CObject::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
		{
			if (m_apObject[nCntPriority][nCntObj] != NULL)
			{// �������̊m�ۂ�����Ă�����

				if (m_apObject[nCntPriority][nCntObj]->m_type != TYPE_NONE)
				{// NONE����Ȃ�������

					// �`�揈��
					m_apObject[nCntPriority][nCntObj]->Draw();
				}
			}
		}
	}
}

//==========================================================================
// �I�u�W�F�N�g�̔j��
//==========================================================================
void CObject::Release(void)
{
	// ����̔ԍ��ۑ�
	int nIdx = m_nID;
	int nPriority = m_nPriority;

	if (m_apObject[nPriority][nIdx] != NULL)
	{// �������̊m�ۂ�����Ă�����

		// �������̊J��
		delete m_apObject[nPriority][nIdx];
		m_apObject[nPriority][nIdx] = NULL;

		// ���������炷
		m_nNumAll--;
		m_nNumPriorityAll[nPriority]--;
	}
	else
	{
		int nn = 0;
	}
}

//==========================================================================
// �Q���̃o�[�`�����q�b�g����
//==========================================================================
bool CObject::Hit(const int nValue)
{
	// ����łȂ�
	return false;
}


//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObject::SetVtx(void)
{

}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CObject::SetPosition(const D3DXVECTOR3 pos)
{
	
}

//==========================================================================
// �ʒu�擾
//==========================================================================
D3DXVECTOR3 CObject::GetPosition(void) const
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

// �ړ��ʐݒ�
void CObject::SetMove(const D3DXVECTOR3 rot)
{

}

// �ړ��ʎ擾
D3DXVECTOR3 CObject::GetMove(void) const 
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CObject::SetRotation(const D3DXVECTOR3 rot)
{

}

//==========================================================================
// �����擾
//==========================================================================
D3DXVECTOR3 CObject::GetRotation(void) const
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


//==========================================================================
// ��ނ̐ݒ�
//==========================================================================
void CObject::SetType(const TYPE type)
{
	m_type = type;
}

//==========================================================================
// ��ނ̎擾
//==========================================================================
CObject::TYPE CObject::GetType(void) const
{
	return m_type;
}

//==========================================================================
// �I�u�W�F�N�g�̎擾
//==========================================================================
CObject *CObject::GetObject(int nPriority, int nIdx)
{
	return m_apObject[nPriority][nIdx];
}

//==========================================================================
// �I�u�W�F�N�g�̎擾
//==========================================================================
CObject *CObject::GetObject(void)
{
	return this;
}

//==========================================================================
// �I�u�W�F�N�g2D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject2D *CObject::GetObject2D(void)
{
	return NULL;
}

//==========================================================================
// �I�u�W�F�N�g3D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject3D *CObject::GetObject3D(void)
{
	return NULL;
}

//==========================================================================
// �I�u�W�F�N�g3D���b�V���I�u�W�F�N�g�̎擾
//==========================================================================
CObject3DMesh *CObject::GetObject3DMesh(void)
{
	return NULL;
}

//==========================================================================
// �I�u�W�F�N�gX�I�u�W�F�N�g�̎擾
//==========================================================================
CObjectX *CObject::GetObjectX(void)
{
	return NULL;
}

//==========================================================================
// �I�u�W�F�N�g�L�����N�^�[�I�u�W�F�N�g�̎擾
//==========================================================================
CObjectChara *CObject::GetObjectChara(void)
{
	return NULL;
}