//=============================================================================
// 
//  �I�u�W�F�N�g�L�����N�^�[���� [objectChara.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "objectChara.h"
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "3D_effect.h"
#include "characterStatus.h"

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
std::vector<CObjectChara::ColliderData> CObjectChara::m_LoadColliderData = {};	// �R���C�_�[���

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObjectChara::CObjectChara(int nPriority) : CObjectHierarchy(nPriority)
{
	// �l�̃N���A
	m_fRotDest = 0.0f;		// �ڕW�̌���
	m_nLife = 0;			// �̗�
	m_nLifeOrigin = 0;		// ���̗̑�
	m_pStatus = nullptr;	// �X�e�[�^�X
	m_bInDicision = false;	// �U�����蒆�t���O
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObjectChara::~CObjectChara()
{

}

//==========================================================================
// ��������
//==========================================================================
CObjectChara* CObjectChara::Create(const std::string pTextFile)
{
	// �������̊m��
	CObjectChara* pObjChara = DEBUG_NEW CObjectChara;

	if (pObjChara != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		HRESULT hr = pObjChara->SetCharacter(pTextFile);
		if (FAILED(hr))
		{// ���s���Ă�����
			return nullptr;
		}
	}

	return pObjChara;
}

//==========================================================================
// �L�����쐬
//==========================================================================
HRESULT CObjectChara::SetCharacter(const std::string& file)
{
	// �L�����쐬
	CObjectHierarchy::SetCharacter(file);

	// ���[�V�����̐�������
	m_pMotion = CMotion::Create(file, this);

	// ���[�V�����̐ݒ�
	if (m_pMotion != nullptr)
	{
		m_pMotion->SetModel(GetModel(), GetNumModel());

		// �|�[�Y�̃��Z�b�g
		m_pMotion->ResetPose(0);
	}

	// �X�t�B�A�R���C�_�[�f�[�^�ǂݍ���
	LoadSphereColliders(file);

	return S_OK;
}

//==========================================================================
// �I�u�W�F�N�g���̃f�[�^���蓖��
//==========================================================================
void CObjectChara::BindObjectData(int nCntData)
{
	// �I�u�W�F�N�g���̃f�[�^���蓖��
	CObjectHierarchy::BindObjectData(nCntData);

	// �X�e�[�^�X����
	CreateStatus(m_aLoadData[m_nIdxFile].parameter);

	// �̗�
	m_nLife = m_aLoadData[m_nIdxFile].parameter.nLife;
	m_nLifeOrigin = m_nLife;
}

//==========================================================================
// �X�e�[�^�X����
//==========================================================================
void CObjectChara::CreateStatus(const CCharacterStatus::CharParameter& parameter)
{
	if (m_pStatus != nullptr)
	{
		delete m_pStatus;
		m_pStatus = nullptr;
	}

	// ����
	m_pStatus = DEBUG_NEW CCharacterStatus(parameter);
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectChara::Init()
{
	// ����������
	CObjectHierarchy::Init();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObjectChara::Uninit()
{
	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// �I������
	CObjectHierarchy::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObjectChara::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �I�u�W�F�N�g�q�G�����L�[�̍X�V
	CObjectHierarchy::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ���[�V�����X�V
	if (m_pMotion != nullptr)
	{
		m_pMotion->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// �R���C�_�[�̈ʒu����
	for (auto& collider : m_SphereColliders)
	{
		// ��������
		if (collider.nParentPartsIdx >= GetNumModel()) continue;

		// ���肷��p�[�c�擾
		CModel* pModel = GetModel()[collider.nParentPartsIdx];

		// ���肷��p�[�c�̃}�g���b�N�X�擾
		MyLib::Matrix mtxTrans;
		MyLib::Matrix mtxWepon = pModel->GetWorldMtx();

		// �ʒu�𔽉f����
		mtxTrans.Translation(collider.offset);
		mtxWepon.Multiply(mtxTrans, mtxWepon);
		collider.center = mtxWepon.GetWorldPosition();
	}

	// ���[�V�������̍s������
	MotionInProgressAction();
}

//==========================================================================
// ���[�V�������̍s������
//==========================================================================
void CObjectChara::MotionInProgressAction()
{
	// �U������t���O���Z�b�g
	m_bInDicision = false;

	if (m_pMotion == nullptr)
	{
		return;
	}

	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	CMotion::Info aInfo = m_pMotion->GetInfo(m_pMotion->GetType());
	int nType = m_pMotion->GetType();

	// �U�����̑����擾
	int nNumAttackInfo = aInfo.nNumAttackInfo;

	for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
	{
		if (aInfo.AttackInfo[nCntAttack] == nullptr)
		{
			continue;
		}

		// �U�����擾
		CMotion::AttackInfo* AttackInfo = aInfo.AttackInfo[nCntAttack];

		if (m_pMotion->IsImpactFrame(*AttackInfo))
		{// �Ռ��̃J�E���g�Ɠ����Ƃ�

			// �U��������
			AttackAction(*AttackInfo, nCntAttack);
		}

		// ���[�V�����J�E���^�[�擾
		float fAllCount = m_pMotion->GetAllCount();
		if (fAllCount > AttackInfo->nMinCnt && fAllCount <= AttackInfo->nMaxCnt)
		{// �U�����蒆

			// �U������t���O
			m_bInDicision = true;

			// �U�����蒆����
			AttackInDicision(AttackInfo, nCntAttack);
		}
	}
}

//==========================================================================
// �U��������
//==========================================================================
void CObjectChara::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// ����̈ʒu
	MyLib::Vector3 weponpos = m_pMotion->GetAttackPosition(GetModel(), ATKInfo);
}

//==========================================================================
// �U�����蒆����
//==========================================================================
void CObjectChara::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
{
	// ����̈ʒu
	MyLib::Vector3 weponpos = m_pMotion->GetAttackPosition(GetModel(), *pATKInfo);
}

//==========================================================================
// ���[�V�����t�@�C���ύX
//==========================================================================
void CObjectChara::ChangeMotion(const char* pMotionFile)
{
	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// ���[�V�����̐�������
	m_pMotion = CMotion::Create(pMotionFile, this);

	// ���[�V�����̐ݒ�
	m_pMotion->SetModel(GetModel(), GetNumModel());

	// �|�[�Y�̃��Z�b�g
	m_pMotion->ResetPose(0);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectChara::Draw()
{
	// �`�揈��
	CObjectHierarchy::Draw();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectChara::Draw(D3DXCOLOR col)
{
	// �`�揈��
	CObjectHierarchy::Draw(col);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectChara::Draw(float fAlpha)
{
	// �`�揈��
	CObjectHierarchy::Draw(fAlpha);
}

//==========================================================================
// �I�u�W�F�N�g���̃f�[�^�ǂݍ���
//==========================================================================
void CObjectChara::LoadObjectData(FILE* pFile, const std::string& file)
{
	// �I�u�W�F�N�g���̃f�[�^�ǂݍ���
	CObjectHierarchy::LoadObjectData(pFile, file);

	char hoge[MAX_COMMENT];	// �R�����g

	if (file.find("VELOCITY_NORMAL") != std::string::npos)
	{// �ʏ�ړ���

		fscanf(pFile, "%s", &hoge[0]);	// =�̕�
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].parameter.fVelocityNormal);	// �ʏ�ړ���
	}

	if (file.find("VELOCITY_RUN") != std::string::npos)
	{// �_�b�V���ړ���

		fscanf(pFile, "%s", &hoge[0]);	// =�̕�
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].parameter.fVelocityDash);	// �_�b�V���ړ���
	}

	if (file.find("VELOCITY_BLINK") != std::string::npos)
	{// �u�����N�ړ���

		fscanf(pFile, "%s", &hoge[0]);	// =�̕�
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].parameter.fVelocityBlink);	// �u�����N�ړ���
	}

	if (file.find("JUMP") != std::string::npos)
	{// �W�����v��

		fscanf(pFile, "%s", &hoge[0]);	// =�̕�
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].parameter.fVelocityJump);	// �W�����v��
	}

	if (file.find("HEIGHT") != std::string::npos)
	{// �g��

		fscanf(pFile, "%s", &hoge[0]);	// =�̕�
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].parameter.fHeight);	// �g��
	}

	if (file.find("LIFE") != std::string::npos)
	{// LIFE�ő̗�

		fscanf(pFile, "%s", &hoge[0]);	// =�̕�
		fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].parameter.nLife);	// �̗�
	}
}

//==========================================================================
// �p�[�c���̃f�[�^�ǂݍ���
//==========================================================================
void CObjectChara::LoadPartsData(FILE* pFile, const std::string& file, int *pCntParts)
{
	// �p�[�c���̃f�[�^�ǂݍ���
	CObjectHierarchy::LoadPartsData(pFile, file, pCntParts);
}

//==========================================================================
// �X�t�B�A�R���C�_�[�ǂݍ���
//==========================================================================
void CObjectChara::LoadSphereColliders(const std::string& file)
{
	// �\���̂̒��̕������T��
	auto itr = std::find_if(m_LoadColliderData.begin(), m_LoadColliderData.end(),
		[&file](const CObjectChara::ColliderData& data) {return data.filename == file; });

	// ���������ꍇ�̏���
	if (itr != m_LoadColliderData.end())
	{
		m_SphereColliders = (*itr).colliders;
		return;
	}


	// �t�@�C�����J��
	FILE* pFile = fopen(file.c_str(), "r");
	if (pFile == nullptr)
	{//�t�@�C�����J���Ȃ������ꍇ
		return;
	}

	std::string filename;

	// �ǂݍ��ݗp�ϐ�
	char aComment[MAX_COMMENT];	// �R�����g�p


	// �v�f�ǉ�
	m_LoadColliderData.emplace_back();

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
			filename = aComment;

			m_LoadColliderData.back().filename = filename;
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
	std::ifstream filejson(filename);
	if (!filejson.is_open())
	{
		return;
	}

	nlohmann::json jsonData;
	filejson >> jsonData;	// json�f�[�^��^����

	// json�f�[�^����ǂݍ���
	from_json(jsonData);
}

//==========================================================================
// �X�t�B�A�R���C�_�[��������
//==========================================================================
void CObjectChara::SaveSphereColliders()
{
	// �t�@�C���ɃL�����N�^�[�̃f�[�^����������
	std::ofstream outFile("character_data.json");
	if (!outFile.is_open())
	{
		return;
	}

	nlohmann::json outputData;
	to_json(outputData);
}

//==========================================================================
// �R���C�_�[�擾
//==========================================================================
CObjectChara::SphereCollider CObjectChara::GetNowSphereCollider(int nIdx)
{
	if (nIdx >= 0 && nIdx < static_cast<int>(m_SphereColliders.size()))
	{// �͈͓�
		return m_SphereColliders[nIdx];
	}
	return SphereCollider();
}

