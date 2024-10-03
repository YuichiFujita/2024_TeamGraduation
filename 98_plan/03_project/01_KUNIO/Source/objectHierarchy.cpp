//=============================================================================
// 
//  �K�w�I�u�W�F�N�g���� [objectHierarchy.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "objectHierarchy.h"
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "3D_effect.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
std::vector<CObjectHierarchy::Load> CObjectHierarchy::m_aLoadData = {};	// �ǂݍ��݃f�[�^
int CObjectHierarchy::m_nNumLoad = 0;	// �ǂݍ��񂾐�

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObjectHierarchy::CObjectHierarchy(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	//D3DXMatrixIdentity(&m_mtxWorld);			// ���[���h�}�g���b�N�X
	m_posOrigin = 0.0f;			// �ŏ��̈ʒu
	m_posCenter = 0.0f;			// ���S�ʒu
	m_fRadius = 0.0f;			// ���a
	m_nNumModel = 0;			// ���f���̐�
	m_nIdxFile = 0;				// �t�@�C���̃C���f�b�N�X�ԍ�
	m_nCenterPartsIdx = 0;		// ���S�ɂ���p�[�c�̃C���f�b�N�X
	m_CenterOffset = 0.0f;		// ���S�̃I�t�Z�b�g

	m_apModel.clear();			// ���f��(�p�[�c)�̃|�C���^
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObjectHierarchy::~CObjectHierarchy()
{

}

//==========================================================================
// ��������
//==========================================================================
CObjectHierarchy* CObjectHierarchy::Create(const std::string& pTextFile)
{
	// �������̊m��
	CObjectHierarchy* pObjChara = DEBUG_NEW CObjectHierarchy;

	if (pObjChara != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		HRESULT hr = pObjChara->SetCharacter(pTextFile);
		if (FAILED(hr)){
			return nullptr;
		}
	}

	return pObjChara;
}

//==========================================================================
// �L�����쐬
//==========================================================================
HRESULT CObjectHierarchy::SetCharacter(const std::string& file)
{
	// ����������
	if (FAILED(CObjectHierarchy::Init()))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// �\���̂̒��̕������T��
	auto itr = std::find_if(m_aLoadData.begin(), m_aLoadData.end(), [&file](const CObjectHierarchy::Load& string) {return string.sTextFile == file;});
	
	// ���������ꍇ�̏���
	if (itr != m_aLoadData.end())
	{
		// �I�u�W�F�N�g���̃f�[�^���蓖��
		BindObjectData(itr - m_aLoadData.begin());
		return S_OK;
	}

	// �Ȃ��ꍇ�̏���
	if (FAILED(CObjectHierarchy::ReadText(file)))
	{// ���s���Ă�����
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I�u�W�F�N�g���̃f�[�^���蓖��
//==========================================================================
void CObjectHierarchy::BindObjectData(int nCntData)
{
	Load loadData = m_aLoadData[nCntData];

	// ���S�ɂ���p�[�c�̃C���f�b�N�X
	m_nCenterPartsIdx = loadData.nCenterIdx;

	// ���S�̃I�t�Z�b�g
	m_CenterOffset = loadData.centerOffSet;

	// ���f�����ݒ�
	m_nNumModel = loadData.nNumModel;

	// �ŏ��̈ʒu�ݒ�
	m_posOrigin = loadData.posOrigin;

	// ���a
	m_fRadius = loadData.fRadius;

	// �t�@�C���̃C���f�b�N�X�ԍ�
	m_nIdxFile = nCntData;

	for (int nCntParts = 0; nCntParts < loadData.nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		LoadData modelInfo = loadData.LoadData[nCntParts];

		// ���f���̐���
		m_apModel.emplace_back();

		m_apModel[nCntParts] = CModel::Create(
			loadData.LoadData[modelInfo.nType].pModelFile.c_str(),
			modelInfo.pos,
			modelInfo.rot);

		// �e���f���̐ݒ�
		if (modelInfo.nParent >= 0)
		{
			// �e�̃��f���I�u�W�F�N�g�ݒ�
			m_apModel[nCntParts]->SetParent(m_apModel[modelInfo.nParent]);

			int idx = modelInfo.nParent;
			m_apModel[nCntParts]->SetMtxParent(m_apModel[idx]->GetPtrWorldMtx());
		}
		else
		{// �������e�̎�
			m_apModel[nCntParts]->SetParent(nullptr);
			m_apModel[nCntParts]->SetMtxParent(&m_mtxWorld);
		}

		// �����z�u�̔���
		if (modelInfo.nStart != 1)
		{
			ChangeObject(nCntParts, -1);
		}
	}
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectHierarchy::Init()
{
	
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObjectHierarchy::Uninit()
{
	// ���f���̏I������
	for (auto& model : m_apModel)
	{
		model->Uninit();
		delete model;
		model = nullptr;
	}
	m_apModel.clear();

	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObjectHierarchy::Update()
{
	// ���肷��p�[�c�擾
	CModel* pModel = m_apModel[m_nCenterPartsIdx];
	if (pModel == nullptr)
	{
		return;
	}

	// ���肷��p�[�c�̃}�g���b�N�X�擾
	MyLib::Matrix mtxTrans;
	MyLib::Matrix mtxWepon = pModel->GetWorldMtx();

	// �ʒu�𔽉f����
	mtxTrans.Translation(m_CenterOffset);
	mtxWepon.Multiply(mtxTrans, mtxWepon);
	m_posCenter = mtxWepon.GetWorldPosition();
}

//==========================================================================
// ���f���ύX
//==========================================================================
void CObjectHierarchy::ChangeObject(int nDeleteParts, int nNewParts)
{
	// �t�@�C�����擾
	Load LoadData = GetLoadData(m_nIdxFile);

	// �I�u�W�F�N�g�L�����N�^�[�̏��擾
	CObjectHierarchy *pObjChar = GetObjectHierarchy();

	int nNumAll = pObjChar->GetNumModel();

	if (m_apModel[nDeleteParts] != nullptr)
	{
		// ���f���̏I������
		m_apModel[nDeleteParts]->Uninit();
		delete m_apModel[nDeleteParts];
		m_apModel[nDeleteParts] = nullptr;
	}

	// �V�����p�[�c��ǂݍ���
	if (nNewParts >= 0 && m_apModel[nNewParts] == nullptr)
	{
		if (m_apModel[nNewParts] == nullptr)
		{
			// ���f���쐬
			m_apModel[nNewParts] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nNewParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nNewParts].pos,
				LoadData.LoadData[nNewParts].rot);
		}
	}

	// �e���f���̐ݒ�
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		if (m_apModel[nCntParts] == nullptr)
		{// nullptr��������
			continue;
		}

		// �e���f���̐ݒ�
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// �e�̃��f���I�u�W�F�N�g�ݒ�
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// �������e�̎�
			m_apModel[nCntParts]->SetParent(nullptr);
		}
	}
}

//==========================================================================
// �؂�ւ��̎��
//==========================================================================
void CObjectHierarchy::ChangeObject(int nSwitchType)
{
	// �t�@�C�����擾
	Load LoadData = GetLoadData(m_nIdxFile);

#if _DEBUG
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		if (m_apModel[nCntParts] != nullptr)
		{
			// ���f���̏I������
			m_apModel[nCntParts]->Uninit();
			delete m_apModel[nCntParts];
			m_apModel[nCntParts] = nullptr;
		}

		// ���f���쐬
		if (m_apModel[nCntParts] == nullptr)
		{
			m_apModel[nCntParts] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nCntParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nCntParts].pos,
				LoadData.LoadData[nCntParts].rot);
		}

		// �e���f���̐ݒ�
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// �e�̃��f���I�u�W�F�N�g�ݒ�
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// �������e�̎�
			m_apModel[nCntParts]->SetParent(nullptr);
		}

		if (LoadData.LoadData[nCntParts].nStart != 1)
		{
			ChangeObject(nCntParts, -1);
		}
	}
#endif

	// ���f���̐؂�ւ�
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		if (LoadData.LoadData[nCntParts].nSwitchType != nSwitchType)
		{// �؂�ւ��̎�ނ��Ⴄ�Ƃ�
			continue;
		}

		// �폜����C���f�b�N�X�ԍ�
		int nDeleteIdx = LoadData.LoadData[nCntParts].nIDSwitchModel;

		if (nDeleteIdx >= 0 && m_apModel[nDeleteIdx] != nullptr)
		{
			// ���f���̏I������
			m_apModel[nDeleteIdx]->Uninit();
			delete m_apModel[nDeleteIdx];
			m_apModel[nDeleteIdx] = nullptr;
		}

		// ��������C���f�b�N�X�ԍ�
		int nNewIdx = nDeleteIdx;

		if (nNewIdx < 0)
		{
			nNewIdx = nCntParts;
		}

		// ���f���쐬
		if (m_apModel[nNewIdx] == nullptr)
		{
			m_apModel[nNewIdx] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nCntParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nCntParts].pos,
				LoadData.LoadData[nCntParts].rot);
		}

		// �e���f���̐ݒ�
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// �e�̃��f���I�u�W�F�N�g�ݒ�
			m_apModel[nNewIdx]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// �������e�̎�
			m_apModel[nNewIdx]->SetParent(nullptr);
		}
	}

}

//==========================================================================
// ���f���ݒ�
//==========================================================================
void CObjectHierarchy::SetObject(int nNewParts)
{
	// �t�@�C�����擾
	Load LoadData = GetLoadData(m_nIdxFile);

	// �V�����p�[�c��ǂݍ���
	if (nNewParts >= 0 && m_apModel[nNewParts] == nullptr)
	{
		if (m_apModel[nNewParts] == nullptr)
		{
			// ���f���쐬
			m_apModel[nNewParts] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nNewParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nNewParts].pos,
				LoadData.LoadData[nNewParts].rot);
		}

		if (m_apModel[nNewParts] == nullptr)
		{
			// ���f���̏I������
			m_apModel[nNewParts]->Uninit();
			delete m_apModel[nNewParts];
			m_apModel[nNewParts] = nullptr;
		}
	}

	// �e���f���̐ݒ�
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		if (m_apModel[nCntParts] == nullptr)
		{
			continue;
		}

		// �e���f���̐ݒ�
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// �e�̃��f���I�u�W�F�N�g�ݒ�
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// �������e�̎�
			m_apModel[nCntParts]->SetParent(nullptr);
		}
	}
}

//==========================================================================
// ���f���폜
//==========================================================================
void CObjectHierarchy::DeleteObject(int nSwitchType)
{
	// �t�@�C�����擾
	Load LoadData = GetLoadData(m_nIdxFile);

	// ���f���̐؂�ւ�
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// �p�[�c���J��Ԃ�

		if (LoadData.LoadData[nCntParts].nSwitchType != nSwitchType)
		{// �؂�ւ��̎�ނ��Ⴄ�Ƃ�
			continue;
		}

		// �폜����C���f�b�N�X�ԍ�
		int nDeleteIdx = LoadData.LoadData[nCntParts].nIDSwitchModel;

		if (nDeleteIdx >= 0 && m_apModel[nDeleteIdx] != nullptr)
		{
			// ���f���̏I������
			m_apModel[nDeleteIdx]->Uninit();
			delete m_apModel[nDeleteIdx];
			m_apModel[nDeleteIdx] = nullptr;
		}
	}
}

//==========================================================================
// ���[���h�}�g���b�N�X�̌v�Z����
//==========================================================================
void CObjectHierarchy::CalWorldMtx()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	MyLib::Matrix mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	// ���[���h�}�g���b�N�X�̏�����
	m_mtxWorld.Identity();

	// �����𔽉f����
	mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRot);

	// �ʒu�𔽉f����
	mtxTrans.Translation(pos);
	m_mtxWorld.Multiply(m_mtxWorld, mtxTrans);

}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectHierarchy::Draw()
{
	// ���[���h�}�g���b�N�X�̌v�Z����
	CalWorldMtx();

	// ���f���̕`��
	for (const auto& model : m_apModel)
	{
		model->Draw();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectHierarchy::Draw(D3DXCOLOR col)
{
	// ���[���h�}�g���b�N�X�̌v�Z����
	CalWorldMtx();

	// ���f���̕`��
	for (const auto& model : m_apModel)
	{
		model->Draw(col);
	}
}


//==========================================================================
// �`�揈��
//==========================================================================
void CObjectHierarchy::Draw(float fAlpha)
{
	// ���[���h�}�g���b�N�X�̌v�Z����
	CalWorldMtx();

	// ���f���̕`��
	for (const auto& model : m_apModel)
	{
		model->Draw(fAlpha);
	}
}

//==========================================================================
// �e�L�X�g�ǂݍ��ݏ���
//==========================================================================
HRESULT CObjectHierarchy::ReadText(const std::string& file)
{
	FILE *pFile = nullptr;	// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen(file.c_str(), "r");

	if (pFile == nullptr)
	{// �t�@�C�����J�����ꍇ
		return E_FAIL;
	}

	// �v�f�ǉ�
	m_aLoadData.emplace_back();

	// �t�@�C�����ۑ�
	m_aLoadData[m_nNumLoad].sTextFile = file;

	// �t�@�C���̃C���f�b�N�X�ԍ�
	m_nIdxFile = m_nNumLoad;

	char aComment[MAX_COMMENT];	// �R�����g
	int nCntFileName = 0;
	int nCntSetParts = 0;

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

				// �v�f�ǉ�
				m_aLoadData[m_nNumLoad].LoadData.emplace_back();

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

				// �I�u�W�F�N�g���̃f�[�^�ǂݍ���
				LoadObjectData(pFile, aComment);

				// �p�[�c���̃f�[�^�ǂݍ���
				LoadPartsData(pFile, aComment, &nCntSetParts);
			}
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
// �I�u�W�F�N�g���̃f�[�^�ǂݍ���
//==========================================================================
void CObjectHierarchy::LoadObjectData(FILE *pFile, const std::string& file)
{
	char hoge[MAX_COMMENT] = {};	// �R�����g
	if (file.find("RADIUS") != std::string::npos)
	{// RADIUS�Ŕ��a

		fscanf(pFile, "%s", &hoge[0]);	// =�̕�
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].fRadius);	// ���a
		m_fRadius = m_aLoadData[m_nNumLoad].fRadius;
	}

	if (file.find("CENTERSET") != std::string::npos)
	{// CENTERSET�Œ��S�ʒu�ǂݍ���

		while (strcmp(hoge, "END_CENTERSET") != 0)
		{// END_CENTERSET������܂ŌJ��Ԃ�

			fscanf(pFile, "%s", &hoge[0]);	//�m�F����

			if (strcmp(hoge, "PARTS") == 0)
			{// PARTS��������p�[�c�ԍ��ǂݍ���

				fscanf(pFile, "%s", &hoge[0]);	// =�̕�
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].nCenterIdx);		// ���f����ނ̗�

				m_nCenterPartsIdx = m_aLoadData[m_nNumLoad].nCenterIdx;		// ���S�ɂ���p�[�c�̃C���f�b�N�X
			}

			if (strcmp(hoge, "OFFSET") == 0)
			{// OFFSET��������ʒu�ǂݍ���

				fscanf(pFile, "%s", &hoge[0]);		// =�̕�
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].centerOffSet.x);	// X���W
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].centerOffSet.y);	// Y���W
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].centerOffSet.z);	// Z���W
				m_CenterOffset = m_aLoadData[m_nNumLoad].centerOffSet;			// ���S�̃I�t�Z�b�g
			}
		}
	}
}

//==========================================================================
// �p�[�c���̃f�[�^�ǂݍ���
//==========================================================================
void CObjectHierarchy::LoadPartsData(FILE* pFile, const std::string& file, int *pCntParts)
{
	char CheckText[MAX_COMMENT] = {};	// �`�F�b�N�p
	char hoge[MAX_COMMENT] = {};	// �R�����g

	int nCntSetParts = *pCntParts;

	if (file.find("PARTSSET") != std::string::npos)
	{// PARTSSET�Ńp�[�c���̓ǂݍ��݊J�n

		while (strcmp(CheckText, "END_PARTSSET") != 0)
		{// END_PARTSSET������܂ŌJ��Ԃ�

			fscanf(pFile, "%s", &CheckText[0]);	// �m�F����

			if (strcmp(CheckText, "INDEX") == 0)
			{// INDEX�������烂�f���ԍ��ǂݍ���

				fscanf(pFile, "%s", &hoge[0]);	// =�̕�
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nType);		// ���f����ނ̗�
			}

			if (strcmp(CheckText, "PARENT") == 0)
			{// PARENT��������e�̔ԍ��ǂݍ���

				fscanf(pFile, "%s", &hoge[0]);	// =�̕�
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent);		// �e�̔ԍ�
			}

			if (strcmp(CheckText, "POS") == 0)
			{// POS��������ʒu�ǂݍ���

				fscanf(pFile, "%s", &hoge[0]);		// =�̕�
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.x);	// X���W
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.y);	// Y���W
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.z);	// Z���W

				if (nCntSetParts == 0)
				{
					m_posOrigin = m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos;	// �ŏ��̈ʒu
					m_aLoadData[m_nNumLoad].posOrigin = m_posOrigin;	// �ŏ��̈ʒu
				}
			}

			if (strcmp(CheckText, "ROT") == 0)
			{// ROT������������ǂݍ���

				fscanf(pFile, "%s", &hoge[0]);		//=�̕�
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.x);	// X�̊p�x
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.y);	// Y�̊p�x
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.z);	// Z�̊p�x
			}

			if (strcmp(CheckText, "START") == 0)
			{// START�������珉�����

				fscanf(pFile, "%s", &hoge[0]);		//=�̕�
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nStart);	// �ŏ����炠�邩
			}

			if (strcmp(CheckText, "SWITCHTYPE") == 0)
			{// SWITCHTYPE��������؂�ւ��̎��

				fscanf(pFile, "%s", &hoge[0]);		//=�̕�
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nSwitchType);	// �؂�ւ��̎��
			}

			if (strcmp(CheckText, "ID_SWITCHMODEL") == 0)
			{// ID_SWITCHMODEL��������؂�ւ��郂�f��ID

				fscanf(pFile, "%s", &hoge[0]);		//=�̕�
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nIDSwitchModel);	// �؂�ւ��郂�f��ID
			}

		}// END_PARTSSET�̂�����

		// ���f���̐���
		m_apModel.emplace_back();

		if (m_apModel[nCntSetParts] == nullptr)
		{
			m_apModel[nCntSetParts] = CModel::Create(
				m_aLoadData[m_nNumLoad].LoadData[m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nType].pModelFile.c_str(),
				m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos,
				m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot);
		}

		// �e���f���̐ݒ�
		if (m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent >= 0)
		{
			m_apModel[nCntSetParts]->SetParent(m_apModel[m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent]);

			int idx = m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent;
			m_apModel[nCntSetParts]->SetMtxParent(m_apModel[idx]->GetPtrWorldMtx());
		}
		else
		{
			m_apModel[nCntSetParts]->SetParent(nullptr);
			m_apModel[nCntSetParts]->SetMtxParent(&m_mtxWorld);
		}

		if (m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nStart != 1)
		{
			ChangeObject(nCntSetParts, -1);
		}

		(*pCntParts)++;	// �p�[�c�̃J�E���g�����Z
	}
}

//==========================================================================
// �}�g���b�N�X�ݒ�
//==========================================================================
void CObjectHierarchy::SetmtxWorld(const MyLib::Matrix mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// �}�g���b�N�X�擾
//==========================================================================
MyLib::Matrix CObjectHierarchy::GetmtxWorld() const
{
	return m_mtxWorld;
}

//==========================================================================
// ���S�̈ʒu�擾
//==========================================================================
MyLib::Vector3 CObjectHierarchy::GetCenterPosition() const
{
	return m_posCenter;
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CObjectHierarchy::SetOriginPosition(const MyLib::Vector3& pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// �ʒu�擾
//==========================================================================
MyLib::Vector3 CObjectHierarchy::GetOriginPosition() const
{
	return m_posOrigin;
}

//==========================================================================
// ���a�ݒ�
//==========================================================================
void CObjectHierarchy::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//==========================================================================
// ���a�擾
//==========================================================================
float CObjectHierarchy::GetRadius() const
{
	return m_fRadius;
}

//==========================================================================
// �I�u�W�F�N�g�L�����N�^�[�I�u�W�F�N�g�̎擾
//==========================================================================
CObjectHierarchy *CObjectHierarchy::GetObjectHierarchy()
{
	return this;
}

//==========================================================================
// ���f�����擾
//==========================================================================
int CObjectHierarchy::GetNumModel() 
{
	return m_nNumModel;
}

//==========================================================================
// ���f�����ݒ�
//==========================================================================
void CObjectHierarchy::SetNumModel(int nNumModel)
{
	m_nNumModel = nNumModel;
}

//==========================================================================
// ���f�����̎擾
//==========================================================================
CModel **CObjectHierarchy::GetModel()
{
	return &m_apModel[0];
}

//==========================================================================
// ���f���擾
//==========================================================================
CModel* CObjectHierarchy::GetModel(int idx)
{
	if (m_nNumModel <= idx) return nullptr;

	return m_apModel[idx];
}

//==========================================================================
// �ǂݍ��ݏ��擾
//==========================================================================
CObjectHierarchy::Load CObjectHierarchy::GetLoadData(int nIdx)
{
	return m_aLoadData[nIdx];
}

//==========================================================================
// �t�@�C���̃C���f�b�N�X�ԍ��擾
//==========================================================================
int CObjectHierarchy::GetIdxFile()
{
	return m_nIdxFile;
}
