//=============================================================================
// 
//  �I�u�W�F�N�g3D���� [objectChara.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "objectChara.h"
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "3D_effect.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CObjectChara::Load CObjectChara::m_aLoadData[mylib_const::MAX_MODEL] = {};	// �ǂݍ��݃f�[�^
int CObjectChara::m_nNumLoad = 0;	// �ǂݍ��񂾐�

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObjectChara::CObjectChara(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	D3DXMatrixIdentity(&m_mtxWorld);			// ���[���h�}�g���b�N�X
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��̈ʒu
	m_posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ŏ��̈ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_fVelocity = 0.0f;			// �ړ����x
	m_fRadius = 0.0f;			// ���a
	m_fRotDest = 0.0f;							// �ڕW�̌���
	m_nNumModel = 0;		// ���f���̐�
	m_nIdxFile = 0;			// �t�@�C���̃C���f�b�N�X�ԍ�

	m_nCenterPartsIdx = 0;		// ���S�ɂ���p�[�c�̃C���f�b�N�X
	m_CenterOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���S�̃I�t�Z�b�g


	memset(&m_apModel[0], NULL, sizeof(m_apModel));	// ���f��(�p�[�c)�̃|�C���^
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObjectChara::~CObjectChara()
{

}

//==========================================================================
// �L�����쐬
//==========================================================================
HRESULT CObjectChara::SetCharacter(const std::string pTextFile)
{
	// ����������
	if (FAILED(CObjectChara::Init()))
	{// ���s���Ă�����
		return E_FAIL;
	}


	// �X�t�B�A�R���C�_�[�f�[�^�ǂݍ���
	LoadSphereColliders(pTextFile);

	for (const auto& colliderData : m_SphereColliders)
	{
		m_MeshSphere.push_back(nullptr);

		// ����
		m_MeshSphere[m_MeshSphere.size() - 1] = CMeshSphere::Create(mylib_const::DEFAULT_VECTOR3, 10.0f, 0, 4, 8);
	}

	// �ǂݍ��݊m�F
	for (int nCntData = 0; nCntData < m_nNumLoad; nCntData++)
	{
		if (m_aLoadData[nCntData].sTextFile == pTextFile)
		{// �t�@�C��������v���Ă���

			for (int nCntParts = 0; nCntParts < m_aLoadData[nCntData].nNumModel; nCntParts++)
			{// �p�[�c���J��Ԃ�

				// ���f�����ݒ�
				m_nNumModel = m_aLoadData[nCntData].nNumModel;

				// �ŏ��̈ʒu�ݒ�
				m_posOrigin = m_aLoadData[nCntData].posOrigin;

				// �ړ����x
				m_fVelocity = m_aLoadData[nCntData].fVelocity;

				// ���a
				m_fRadius = m_aLoadData[nCntData].fRadius;

				// �t�@�C���̃C���f�b�N�X�ԍ�
				m_nIdxFile = nCntData;

				// ���f���̐���
				m_apModel[nCntParts] = CModel::Create(
					m_aLoadData[nCntData].LoadData[m_aLoadData[nCntData].LoadData[nCntParts].nType].pModelFile.c_str(),
					m_aLoadData[nCntData].LoadData[nCntParts].pos,
					m_aLoadData[nCntData].LoadData[nCntParts].rot);

				// �e���f���̐ݒ�
				if (m_aLoadData[nCntData].LoadData[nCntParts].nParent >= 0)
				{
					// �e�̃��f���I�u�W�F�N�g�ݒ�
					m_apModel[nCntParts]->SetParent(m_apModel[m_aLoadData[nCntData].LoadData[nCntParts].nParent], m_aLoadData[nCntData].LoadData[nCntParts].nParent);
				}
				else
				{// �������e�̎�
					m_apModel[nCntParts]->SetParent(NULL, -1);
				}
			}

			return S_OK;
		}
	}

	if (FAILED(CObjectChara::ReadText(pTextFile)))
	{// ���s���Ă�����
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectChara::Init(void)
{
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		for (int nCntParts = 0; nCntParts < mylib_const::MAX_MODEL; nCntParts++)
		{
			m_aLoadData[nCntModel].LoadData[nCntParts].nStart = 1;	// �ǂݍ��݂̃f�[�^
		}
	}
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObjectChara::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULL����Ȃ�������

			// ���f���̏I������
			m_apModel[nCntModel]->Uninit();
			m_apModel[nCntModel] = NULL;
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

// �R���C�_�[�ǉ�
void CObjectChara::AddCollider(const SphereCollider& collider)
{
	m_SphereColliders.push_back(collider);
	m_MeshSphere.push_back(nullptr);

	// ����
	m_MeshSphere[GetNumCollider() - 1] = CMeshSphere::Create(mylib_const::DEFAULT_VECTOR3, 10.0f, 0, 4, 8);
}

void CObjectChara::SubCollider(void)	// �R���C�_�[�폜
{
	m_MeshSphere[GetNumCollider() - 1]->Uninit();

	m_MeshSphere.pop_back();			// ���b�V���X�t�B�A
	m_SphereColliders.pop_back();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObjectChara::Update(void)
{
	// ���f�����擾
	CModel** pModel = GetModel();
	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	// ����̃}�g���b�N�X�擾
	D3DXMATRIX mtxWepon;
	D3DXMatrixIdentity(&mtxWepon);

	if (pModel[m_nCenterPartsIdx] == NULL)
	{
		return;
	}

	// ���肷��p�[�c�̃}�g���b�N�X�擾
	mtxWepon = pModel[m_nCenterPartsIdx]->GetWorldMtx();

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, m_CenterOffset.x, m_CenterOffset.y, m_CenterOffset.z);
	D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

	// ����̈ʒu
	m_posCenter = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

	for (int i = 0; i < 5; i++)
	{
		CEffect3D::Create(m_posCenter,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.4f, 1.0f, 0.6f, 1.0f), 5.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}
}

//==========================================================================
// ���f���ύX
//==========================================================================
void CObjectChara::ChangeObject(int nDeleteParts, int nNewParts)
{
	int nIdx = GetIdxFile();
	Load LoadData = GetLoadData(nIdx);

	// �I�u�W�F�N�g�L�����N�^�[�̏��擾
	CObjectChara *pObjChar = GetObjectChara();

	int nNumAll = pObjChar->GetNumModel();

	if (m_apModel[nDeleteParts] != NULL)
	{// NULL����Ȃ�������

		// ���f���̏I������
		m_apModel[nDeleteParts]->Uninit();
		m_apModel[nDeleteParts] = NULL;
	}

	// NULL�ɂ���
	//m_apModel[nDeleteParts] = NULL;

	// �V�����p�[�c��ǂݍ���
	if (nNewParts >= 0)
	{
		m_apModel[nNewParts] = CModel::Create(
			LoadData.LoadData[LoadData.LoadData[nNewParts].nType].pModelFile.c_str(),
			LoadData.LoadData[nNewParts].pos,
			LoadData.LoadData[nNewParts].rot);
	}

	// �e���f���̐ݒ�
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		if (m_apModel[nCntParts] == NULL)
		{// NULL��������
			continue;
		}

		// �e���f���̐ݒ�
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// �e�̃��f���I�u�W�F�N�g�ݒ�
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent], LoadData.LoadData[nCntParts].nParent);
		}
		else
		{// �������e�̎�
			m_apModel[nCntParts]->SetParent(NULL, -1);
		}
	}
}

//==========================================================================
// ���f���ݒ�
//==========================================================================
void CObjectChara::SetObject(int nNewParts)
{
	int nIdx = GetIdxFile();
	Load LoadData = GetLoadData(nIdx);

	// �V�����p�[�c��ǂݍ���
	if (nNewParts >= 0)
	{
		m_apModel[nNewParts] = CModel::Create(
			LoadData.LoadData[LoadData.LoadData[nNewParts].nType].pModelFile.c_str(),
			LoadData.LoadData[nNewParts].pos,
			LoadData.LoadData[nNewParts].rot);
	}

	// �e���f���̐ݒ�
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		if (m_apModel[nCntParts] == NULL)
		{// NULL��������
			continue;
		}

		// �e���f���̐ݒ�
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// �e�̃��f���I�u�W�F�N�g�ݒ�
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent], LoadData.LoadData[nCntParts].nParent);
		}
		else
		{// �������e�̎�
			m_apModel[nCntParts]->SetParent(NULL, -1);
		}
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectChara::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULL����Ȃ�������

			// �p�[�c���Ƃ̕`��
			m_apModel[nCntModel]->Draw();
		}
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectChara::Draw(D3DXCOLOR col)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULL����Ȃ�������

			// �p�[�c���Ƃ̕`��
			m_apModel[nCntModel]->Draw(col);
		}
	}
}


//==========================================================================
// �`�揈��
//==========================================================================
void CObjectChara::Draw(int nIdx, float fAlpha)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

									// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULL����Ȃ�������

			if (nIdx == nCntModel)
			{
				// �p�[�c���Ƃ̕`��
				m_apModel[nCntModel]->Draw(fAlpha);
			}
			else
			{
				// �p�[�c���Ƃ̕`��
				m_apModel[nCntModel]->Draw();
			}
		}
	}
}

//==========================================================================
// �e�L�X�g�ǂݍ��ݏ���
//==========================================================================
HRESULT CObjectChara::ReadText(const std::string pTextFile)
{
	FILE *pFile = NULL;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen(pTextFile.c_str(), "r");

	if (pFile == NULL)
	{//�t�@�C�����J�����ꍇ
		return E_FAIL;
	}

	// �t�@�C�����ۑ�
	m_aLoadData[m_nNumLoad].sTextFile = pTextFile;

	// �t�@�C���̃C���f�b�N�X�ԍ�
	m_nIdxFile = m_nNumLoad;

	char aComment[MAX_COMMENT];	// �R�����g
	int nCntSetParts = 0;
	int nCntFileName = 0;

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// ���f�����̐ݒ�
		if (strcmp(aComment, "NUM_MODEL") == 0)
		{// NUM_MODEL��������

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�
			fscanf(pFile, "%d", &m_nNumModel);	// ���f����
			m_aLoadData[m_nNumLoad].nNumModel = m_nNumModel;
		}

		while (nCntFileName != m_nNumModel)
		{// ���f���̐����ǂݍ��ނ܂ŌJ��Ԃ�

			// ������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			// ���f�����̐ݒ�
			if (strcmp(aComment, "MODEL_FILENAME") == 0)
			{// NUM_MODEL��������

				fscanf(pFile, "%s", &aComment[0]);	// =�̕�
				fscanf(pFile, "%s", &aComment[0]);	// �t�@�C����

				// �t�@�C�����ۑ�
				m_aLoadData[m_nNumLoad].LoadData[nCntFileName].pModelFile = aComment;

				nCntFileName++;	// �t�@�C�������Z
			}
		}

		// �e�p�[�c�̐ݒ�
		if (strcmp(aComment, "CHARACTERSET") == 0)
		{// �L�����N�^�[���̓ǂݍ��݂��J�n

			while (strcmp(aComment, "END_CHARACTERSET") != 0)
			{// END_CHARACTERSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	//�m�F����


				if (strcmp(aComment, "CENTERSET") == 0)
				{// CENTERSET�Œ��S�ʒu�ǂݍ���

					while (strcmp(aComment, "END_CENTERSET") != 0)
					{// END_CENTERSET������܂ŌJ��Ԃ�

						fscanf(pFile, "%s", &aComment[0]);	//�m�F����

						if (strcmp(aComment, "PARTS") == 0)
						{// PARTS��������p�[�c�ԍ��ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);	// =�̕�
							fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].nCenterIdx);		// ���f����ނ̗�

							m_nCenterPartsIdx = m_aLoadData[m_nNumLoad].nCenterIdx;		// ���S�ɂ���p�[�c�̃C���f�b�N�X
						}

						if (strcmp(aComment, "OFFSET") == 0)
						{// OFFSET��������ʒu�ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].centerOffSet.x);	// X���W
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].centerOffSet.y);	// Y���W
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].centerOffSet.z);	// Z���W
							m_CenterOffset = m_aLoadData[m_nNumLoad].centerOffSet;			// ���S�̃I�t�Z�b�g
						}
					}
				}

				if (strcmp(aComment, "VELOCITY") == 0)
				{// VELOCITY�ňړ����x

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].fVelocity);	// �ړ����x
					m_fVelocity = m_aLoadData[m_nNumLoad].fVelocity;
				}

				if (strcmp(aComment, "RADIUS") == 0)
				{// RADIUS�Ŕ��a

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].fRadius);	// ���a
					m_fRadius = m_aLoadData[m_nNumLoad].fRadius;
				}

				if (strcmp(aComment, "PARTSSET") == 0)
				{// PARTSSET�Ńp�[�c���̓ǂݍ��݊J�n

					while (strcmp(aComment, "END_PARTSSET") != 0)
					{// END_PARTSSET������܂ŌJ��Ԃ�

						fscanf(pFile, "%s", &aComment[0]);	// �m�F����

						if (strcmp(aComment, "INDEX") == 0)
						{// INDEX�������烂�f���ԍ��ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);	// =�̕�
							fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nType);		// ���f����ނ̗�
						}

						if (strcmp(aComment, "PARENT") == 0)
						{// PARENT��������e�̔ԍ��ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);	// =�̕�
							fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent);		// �e�̔ԍ�
						}

						if (strcmp(aComment, "POS") == 0)
						{// POS��������ʒu�ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.x);	// X���W
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.y);	// Y���W
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.z);	// Z���W

							if (nCntSetParts == 0)
							{
								m_posOrigin = m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos;	// �ŏ��̈ʒu
								m_aLoadData[m_nNumLoad].posOrigin = m_posOrigin;	// �ŏ��̈ʒu
							}
						}

						if (strcmp(aComment, "ROT") == 0)
						{// ROT������������ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		//=�̕�
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.x);	// X�̊p�x
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.y);	// Y�̊p�x
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.z);	// Z�̊p�x
						}

						if (strcmp(aComment, "START") == 0)
						{// START�������珉�����

							fscanf(pFile, "%s", &aComment[0]);		//=�̕�
							fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nStart);	// �ŏ����炠�邩
						}

					}// END_PARTSSET�̂�����

					// ���f���̐���
					m_apModel[nCntSetParts] = CModel::Create(
						m_aLoadData[m_nNumLoad].LoadData[m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nType].pModelFile.c_str(),
						m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos,
						m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot);

					// �e���f���̐ݒ�
					if (m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent >= 0)
					{
						m_apModel[nCntSetParts]->SetParent(m_apModel[m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent], m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent);
					}
					else
					{
						m_apModel[nCntSetParts]->SetParent(NULL, -1);
					}

					if (m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nStart != 1)
					{
						ChangeObject(nCntSetParts, -1);
					}

					nCntSetParts++;	// �p�[�c�̃J�E���g�����Z
				}
			}// END_CHARACTERSET�̂�����
		}

		if (strcmp(aComment, "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���
			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);

	// �ǂݍ��񂾐����Z
	m_nNumLoad++;

	return S_OK;
}


//==========================================================================
// �X�t�B�A�R���C�_�[�ǂݍ���
//==========================================================================
void CObjectChara::LoadSphereColliders(const std::string textfile)
{
	m_SetUpFilename = "";

	// �t�@�C�����J��
	FILE* pFile = fopen(textfile.c_str(), "r");
	if (pFile == NULL)
	{//�t�@�C�����J���Ȃ������ꍇ
		return;
	}

	// �ǂݍ��ݗp�ϐ�
	char aComment[MAX_COMMENT];	// �R�����g�p

	while (1)
	{
		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// �R���C�_�[�t�@�C���̓ǂݍ���
		if (strcmp(aComment, "COLLIDER_FILENAME") == 0)
		{// COLLIDER_FILENAME��������

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�
			fscanf(pFile, "%s", &aComment[0]);	// �t�@�C����

			// �t�@�C�����ۑ�
			m_SetUpFilename = aComment;
			break;
		}

		if (strcmp(aComment, "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���
			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);


	// �t�@�C������JSON��ǂݍ���
	std::ifstream file(m_SetUpFilename);
	if (!file.is_open())
	{
		return;
	}

	nlohmann::json jsonData;
	file >> jsonData;	// json�f�[�^��^����

	// json�f�[�^����ǂݍ���
	from_json(jsonData);
}

//==========================================================================
// �X�t�B�A�R���C�_�[��������
//==========================================================================
void CObjectChara::SaveSphereColliders()
{
	if (m_SetUpFilename == "")
	{
		return;
	}

	// �t�@�C���ɃL�����N�^�[�̃f�[�^����������
	std::ofstream outFile(m_SetUpFilename);
	if (!outFile.is_open())
	{
		return;
	}

	nlohmann::json outputData;
	to_json(outputData);

	// �t�@�C���Ƀf�[�^����������
	outFile << std::setw(4) << outputData << std::endl;
}

//==========================================================================
// �}�g���b�N�X�ݒ�
//==========================================================================
void CObjectChara::SetmtxWorld(const D3DXMATRIX mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// �}�g���b�N�X�擾
//==========================================================================
D3DXMATRIX CObjectChara::GetmtxWorld(void) const
{
	return m_mtxWorld;
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CObjectChara::SetPosition(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==========================================================================
// �ʒu�擾
//==========================================================================
D3DXVECTOR3 CObjectChara::GetPosition(void) const
{
	return m_pos;
}


//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CObjectChara::SetOriginPosition(const D3DXVECTOR3 pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// �ʒu�擾
//==========================================================================
D3DXVECTOR3 CObjectChara::GetOriginPosition(void) const
{
	return m_posOrigin;
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CObjectChara::SetOldPosition(const D3DXVECTOR3 posOld)
{
	m_posOld = posOld;
}

//==========================================================================
// �ʒu�擾
//==========================================================================
D3DXVECTOR3 CObjectChara::GetOldPosition(void) const
{
	return m_posOld;
}


//==========================================================================
// �ړ��ʐݒ�
//==========================================================================
void CObjectChara::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//==========================================================================
// �ړ��ʎ擾
//==========================================================================
D3DXVECTOR3 CObjectChara::GetMove(void) const
{
	return m_move;
}

//==========================================================================
// �ړ����x�ݒ�
//==========================================================================
void CObjectChara::SetVelocity(const float fVelocity)
{
	m_fVelocity = fVelocity;
}

//==========================================================================
// �ړ����x�ݒ�
//==========================================================================
float CObjectChara::GetVelocity(void) const
{
	return m_fVelocity;
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CObjectChara::SetRotation(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//==========================================================================
// �����擾
//==========================================================================
D3DXVECTOR3 CObjectChara::GetRotation(void) const
{
	return m_rot;
}

//==========================================================================
// �ڕW�̌����ݒ�
//==========================================================================
void CObjectChara::SetRotDest(const float fRotDest)
{
	m_fRotDest = fRotDest;
}

//==========================================================================
// �ڕW�̌����ݒ�
//==========================================================================
float CObjectChara::GetRotDest(void) const
{
	return m_fRotDest;
}

//==========================================================================
// ���a�ݒ�
//==========================================================================
void CObjectChara::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//==========================================================================
// ���a�擾
//==========================================================================
float CObjectChara::GetRadius(void) const
{
	return m_fRadius;
}

//==========================================================================
// �I�u�W�F�N�g�L�����N�^�[�I�u�W�F�N�g�̎擾
//==========================================================================
CObjectChara *CObjectChara::GetObjectChara(void)
{
	return this;
}

//==========================================================================
// ���f�����擾
//==========================================================================
int CObjectChara::GetNumModel(void) 
{
	return m_nNumModel;
}

//==========================================================================
// ���f�����ݒ�
//==========================================================================
void CObjectChara::SetNumModel(int nNumModel)
{
	m_nNumModel = nNumModel;
}

//==========================================================================
// ���f�����̎擾
//==========================================================================
CModel **CObjectChara::GetModel(void)
{
	return &m_apModel[0];
}

//==========================================================================
// �ǂݍ��ݏ��擾
//==========================================================================
CObjectChara::Load CObjectChara::GetLoadData(int nIdx)
{
	return m_aLoadData[nIdx];
}

//==========================================================================
// �t�@�C���̃C���f�b�N�X�ԍ��擾
//==========================================================================
int CObjectChara::GetIdxFile(void)
{
	return m_nIdxFile;
}
