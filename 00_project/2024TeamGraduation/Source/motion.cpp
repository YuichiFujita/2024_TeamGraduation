//=============================================================================
// 
//  ���f������ [motion.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "motion.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "model.h"
#include "objectChara.h"
#include "debugproc.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int IDX_VISUALMOVE = 2;	// �����ڂ݈̂ړ�
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMotion::CMotion()
{
	// �l�̃N���A
	m_nType = 0;			// ���݂̃��[�V�����̎��
	m_nOldType = 0;			// �O��̃��[�V�����̎��
	m_bLoop = false;		// ���[�v���邩�ǂ���
	m_nPatternKey = 0;		// ���ڂ̃L�[��
	m_fCntFrame = 0.0f;		// �t���[���̃J�E���g
	m_fAllFrame = 0.0f;	// �S�ẴJ�E���g
	m_fMaxAllFrame = 0.0f;	// �S�ẴJ�E���g�̍ő�l
	m_fSlowFactor = 0.0f;	// �x���W��
	m_bFinish = false;		// �I���������ǂ���
	m_bCancelable = false;	// �L�����Z���\��
	m_bCombiable = false;	// �R���{�\��
	m_bAttaking = false;	// �U�����蒆�t���O
	m_bLoopMomnet = false;	// ���[�v�����u��
	m_bMotion = true;		// ���[�V�����Đ��t���O

	m_pObjChara = nullptr;		// �I�u�W�F�N�g�̃|�C���^
	m_ppModel = nullptr;		// ���f���̃|�C���^
	m_nNumModel = 0;			// ���f���̑���
	m_nNumMotion = 0;			// ���[�V�����̑���
	m_vecDefaultIdx.clear();	// �f�t�H���g�̃C���f�b�N�X
	m_vecInfo.clear();			// ���[�V�������
	//m_pPartsOld = nullptr;	// �ߋ��̏��

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMotion::~CMotion()
{

}

//==========================================================================
// ��������
//==========================================================================
CMotion* CMotion::Create(const std::string& file, CObjectChara* pObjChara)
{
	// �����p�̃I�u�W�F�N�g
	CMotion* pMotion = DEBUG_NEW CMotion;

	if (pMotion != nullptr)
	{
		// �I�u�W�F�N�g�̃|�C���^��n��
		pMotion->m_pObjChara = pObjChara;

		// ����������
		pMotion->Init(file);
	}

	return pMotion;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMotion::Init(const std::string& file)
{
	m_nType = 0;			// ���݂̃��[�V�����̎��
	m_bLoop = false;		// ���[�v���邩�ǂ���
	m_nPatternKey = 0;		// ���ڂ̃L�[��
	m_fCntFrame = 0.0f;		// �t���[���̃J�E���g
	m_fAllFrame = 0.0f;	// �S�ẴJ�E���g
	m_fMaxAllFrame = 0.0f;	// �S�ẴJ�E���g�̍ő�l
	m_bFinish = true;		// �I���������ǂ���
	m_ppModel = nullptr;		// ���f���̃|�C���^
	m_nNumModel = 0;		// ���f���̑���
	m_fSlowFactor = 1.0f;	// �x���W��
	m_pPartsOld = nullptr;
	m_vecDefaultIdx.push_back(0);	// �f�t�H���g�̃C���f�b�N�X

	// ���[�V�����ǂݍ���
	CMotionManager* pMotionManager = CMotionManager::GetInstance();
	if (pMotionManager == nullptr)
	{// �v�f�Ȃ��ꍇ����
		CMotionManager::Create();
	}
	const CMotionManager::SLoadInfo& loadInfo = pMotionManager->Load(file);

	// �ǂݍ��ݏ��擾
	m_vecDefaultIdx = loadInfo.vecDefaultIdx;						// �f�t�H���g�̃C���f�b�N�X
	m_vecInfo = loadInfo.vecLoadData;								// ���[�V�������
	m_nNumMotion = static_cast<int>(loadInfo.vecLoadData.size());	// ���[�V������

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMotion::Uninit()
{
	m_vecInfo.clear();

	if (m_pPartsOld != nullptr)
	{
		delete[] m_pPartsOld;
		m_pPartsOld = nullptr;
	}
}

//==========================================================================
// ���[�V���������郂�f���̓o�^
//==========================================================================
void CMotion::SetModel(CModel** pModel, int nNumModel)
{
	// ���f���̃|�C���^��n��
	m_ppModel = pModel;

	// ���f���̑���
	m_nNumModel = nNumModel;

	m_pPartsOld = DEBUG_NEW CMotionManager::Parts[m_nNumModel];
	if (m_pPartsOld == nullptr)
	{
		return;
	}
	memset(m_pPartsOld, 0, sizeof(CMotionManager::Parts) * m_nNumModel);
}

//==========================================================================
// �|�[�Y�̃��Z�b�g
//==========================================================================
void CMotion::ResetPose(int nType)
{
	// �p�[�c���
	const CMotionManager::Info& info = m_vecInfo[nType];
	const CMotionManager::Key& keyInfo = info.aKey[0];

	int partsSize = static_cast<int>(info.aKey[0].aParts.size());
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{// �S�p�[�c���J��Ԃ�

		// ���f�����Ȃ��Ɩ߂�
		if (m_ppModel[nCntParts] == nullptr) continue;

		// �p�[�c��񂪂Ȃ��Ɩ߂�
		if (partsSize <= nCntParts) continue;

		// �����ݒ�
		m_ppModel[nCntParts]->SetRotation(keyInfo.aParts[nCntParts].rot);
		m_pPartsOld[nCntParts].rot = keyInfo.aParts[nCntParts].rot;
		m_pPartsOld[nCntParts].scale = keyInfo.aParts[nCntParts].scale;

		// ���̈ʒu�擾
		MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

		// �ʒu�ݒ�
		if (nCntParts == 0)
		{// �e�̏ꍇ�̓L�����N�^�[�̈ʒu
			m_ppModel[nCntParts]->SetPosition(keyInfo.aParts[nCntParts].pos + posOrigin);
		}
		else
		{
			m_ppModel[nCntParts]->SetPosition(keyInfo.aParts[nCntParts].pos + m_ppModel[nCntParts]->GetOriginPosition());
		}
	}

	// �ߋ��̈ʒu���ۑ�
 	m_pPartsOld[0].pos = m_vecInfo[nType].aKey[0].aParts[0].pos;

	// ���[�V�����ݒ�
	Set(nType);
}

//==========================================================================
// �X�V����
//==========================================================================
void CMotion::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_bMotion) { return; }

	if (m_vecInfo[0].aKey.size() == 0)
	{// ���[�V������񃁃������[�N�H
		MyAssert::CustomAssert(false,"�ϋq����\������");
	}

	// �Đ����̏��
	CMotionManager::Info& nowInfo = m_vecInfo[m_nType];

	// �U�����蒆�t���O���Z�b�g
	m_bAttaking = false;

	// ���[�v�����u��
	m_bLoopMomnet = false;

	// �U�����̑����擾
	int nNumAttackInfo = nowInfo.nNumAttackInfo;

	for (auto& attackInfo : nowInfo.AttackInfo)
	{
		//--------------------------
		// �U�����
		//--------------------------
		// �܂��Ռ��J�E���g�̍s�������ĂȂ���Ԃɂ���
		if (attackInfo.nInpactCnt != 0 || m_fAllFrame > 0.0f)
		{
			attackInfo.bInpactAct = false;
		}

		if (attackInfo.nInpactCnt >= 0 &&
			!attackInfo.bInpactActSet &&
			m_fAllFrame >= static_cast<float>(attackInfo.nInpactCnt))
		{// �Ռ��̃J�E���g�𒴂����� && �܂��s�����ĂȂ�������

			// �s�����Ă��Ԃɂ��āA�ݒ芮��
			attackInfo.bInpactAct = true;
			attackInfo.bInpactActSet = true;
		}


		//--------------------------
		// �������
		//--------------------------
		attackInfo.AlignInfo.bSet = false;

		if (attackInfo.AlignInfo.nFrame >= 0 &&
			m_fAllFrame >= static_cast<float>(attackInfo.AlignInfo.nFrame) &&
			m_fAllFrame <= static_cast<float>(attackInfo.AlignInfo.nFrame + attackInfo.AlignInfo.nExtensionFrame))
		{// �t���[����
			attackInfo.AlignInfo.bSet = true;
		}
	}

	if (m_bFinish && nowInfo.nLoop == LOOP_OFF)
	{// �I�����Ă� && ���[�vOFF��������
		return;
	}


	// �Đ����̃L�[
	const CMotionManager::Key& nowKey = nowInfo.aKey[m_nPatternKey];

	// �Đ��t���[���ۑ�
	int nMaxFrame = nowKey.nFrame;

	// �f�t�H���g���[�V��������
	bool bDefault = std::find(m_vecDefaultIdx.begin(), m_vecDefaultIdx.end(), m_nType) != m_vecDefaultIdx.end();

	if (bDefault && m_nOldType != 0 && m_nPatternKey == 0)
	{// �j���[�g������0�̎�
		nMaxFrame = 10;
	}

	// ���̃L�[
	int nNextKey = (m_nPatternKey + 1) % nowInfo.nNumKey;

	if (nNextKey == 0 && nowInfo.nLoop == LOOP_OFF)
	{// ���[�v���Ȃ��Ƃ�

		// �Ō�ŌŒ�
		nNextKey = nowInfo.nNumKey - 1;
	}

	//--------------------------
	// �Đ��t���[���Ƃ̊���
	//--------------------------
	float ratio = m_fCntFrame / static_cast<float>(nMaxFrame);
	ratio = UtilFunc::Transformation::Clamp(ratio, 0.0f, 1.0f);

	for (int i = 0; i < static_cast<int>(nowKey.aParts.size()); i++)
	{
		// ��������������
		if (i >= m_nNumModel) continue;

		// ���݂̃p�[�c���
		const CMotionManager::Parts& nowParts = nowKey.aParts[i];
		const CMotionManager::Parts& nextParts = nowInfo.aKey[nNextKey].aParts[i];

		// ���f�����Ȃ�������߂�
		if (m_ppModel[i] == nullptr) continue;

		//--------------------------
		// �����X�V
		//--------------------------
		UpdateRotation(i, nowParts, nextParts, ratio);

		//--------------------------
		// �X�P�[���X�V
		//--------------------------
		UpdateScale(i, nowParts, nextParts, ratio);

		//--------------------------
		// �ʒu���f
		//--------------------------
		UpdatePosition(i, nowInfo, nowParts, nextParts, ratio, nMaxFrame, fDeltaTime, fDeltaRate, fSlowRate);

	}

	// �t���[���̃J�E���g�����Z
	m_fCntFrame += 1.0f * fDeltaRate * fSlowRate;
	m_fAllFrame += 1.0f * fDeltaRate * fSlowRate;

	// �U�����蒆�t���O�ݒ�
	float fMinCnt, fMaxCnt;
	for (auto& attackInfo : nowInfo.AttackInfo)
	{
		// �U�������Z�b�g
		attackInfo.bAtkking = false;

		if (attackInfo.nMaxCnt == 0)
		{// �ő�l��0�̂͂Ȃɂ����Ȃ�
			continue;
		}

		// �ŏ��l�ƍő�l
		fMinCnt = static_cast<float>(attackInfo.nMinCnt);
		fMaxCnt = static_cast<float>(attackInfo.nMaxCnt);

		// ����̊���
		attackInfo.fCntRatio = UtilFunc::Transformation::Clamp(
			(m_fAllFrame - fMinCnt) / (fMaxCnt - fMinCnt),
			0.0f, 1.0f);

		// �U������
		if (m_fAllFrame >= fMinCnt &&
			m_fAllFrame <= fMaxCnt)
		{// �ŏ��l�ƍő�l�̒�
			m_bAttaking = true;
			attackInfo.bAtkking = true;	// �U����
		}
	}

	//--------------------------
	// �R���{�t���[��
	//--------------------------
	if (nowInfo.nCombolableFrame != -1 &&
		nowInfo.nCombolableFrame <= m_fAllFrame)
	{// -1����Ȃ��A�t���[���𒴂�����
		m_bCombiable = true;	// �R���{�\
	}

	//--------------------------
	// �L�����Z���t���[��
	//--------------------------
	if (nowInfo.nCancelableFrame != -1 &&
		nowInfo.nCancelableFrame <= m_fAllFrame)
	{// -1����Ȃ��A�t���[���𒴂�����
		m_bCancelable = true;	// �L�����Z���\
	}

	if (nowInfo.nLoop == LOOP_ON)
	{// ���[�v���[�V�����͂��ł��I����Ă�

		// �I������ON
		m_bFinish = true;
	}

	//--------------------------
	// �L�[�X�V
	//--------------------------
	if (m_fCntFrame >= static_cast<float>(nMaxFrame))
	{// �t���[���̃J�E���g���t���[�����ɒB������

		// �t���[���̃J�E���g���[���ɖ߂�
		m_fCntFrame = 0;

		m_nOldType = m_nType;	// �O��̃��[�V�����̎��

		// �p�^�[��NO.�X�V
		m_nPatternKey = (m_nPatternKey + 1) % nowInfo.nNumKey;

		// �p�[�c�T�C�Y
		int partsSize = static_cast<int>(nowInfo.aKey[m_nPatternKey].aParts.size());
		for (int i = 0; i < m_nNumModel; i++)
		{// �S�p�[�c���J��Ԃ�

			if (partsSize <= i)
			{
				m_pPartsOld[i].rot = MyLib::Vector3();
				m_pPartsOld[i].pos = MyLib::Vector3();
				m_pPartsOld[i].scale = MyLib::Vector3(1.0f);
			}
			else
			{
				m_pPartsOld[i].rot = nowInfo.aKey[m_nPatternKey].aParts[i].rot;
				m_pPartsOld[i].pos = nowInfo.aKey[m_nPatternKey].aParts[i].pos;
				m_pPartsOld[i].scale = nowInfo.aKey[m_nPatternKey].aParts[i].scale;
			}
		}

		if (m_nPatternKey == 0)
		{// 1�T����

			// ���[�v�����u��
			m_bLoopMomnet = true;	

			// ���t���[�������Z�b�g
			m_fAllFrame = 0.0f;

			//���[�V�����̐ݒ�
			if (nowInfo.nLoop == LOOP_OFF)
			{// ���[�v���[�V��������Ȃ���

				// �I������ON
				m_bFinish = true;
			}
			else
			{// ���[�v���[�V����

				// �U�����̃t���O�I�t
				for (auto& attackInfo : nowInfo.AttackInfo)
				{
					attackInfo.bInpactAct = false;
					attackInfo.bInpactActSet = false;
					attackInfo.bEndAtk = false;
				}
			}
		}
	}
}

//==========================================================================
// �����̍X�V
//==========================================================================
void CMotion::UpdateRotation(int i, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, float ratio)
{
	
	//--------------------------
	// ���ƍ��̌����̍����擾
	//--------------------------
	float rotDiffX = nextParts.rot.x - m_pPartsOld[i].rot.x;
	float rotDiffY = nextParts.rot.y - m_pPartsOld[i].rot.y;
	float rotDiffZ = nextParts.rot.z - m_pPartsOld[i].rot.z;

	// �p�x�̐��K��
	UtilFunc::Transformation::RotNormalize(rotDiffX);
	UtilFunc::Transformation::RotNormalize(rotDiffY);
	UtilFunc::Transformation::RotNormalize(rotDiffZ);

	//--------------------------
	// �p�[�c�̌�����ݒ�
	//--------------------------
	MyLib::Vector3 rot;
	rot.x = m_pPartsOld[i].rot.x + (rotDiffX * ratio);
	rot.y = m_pPartsOld[i].rot.y + (rotDiffY * ratio);
	rot.z = m_pPartsOld[i].rot.z + (rotDiffZ * ratio);

	// �p�x�̐��K��
	UtilFunc::Transformation::RotNormalize(rot);

	// �����ݒ�
	m_ppModel[i]->SetRotation(rot);
}

//==========================================================================
// �X�P�[���̍X�V
//==========================================================================
void CMotion::UpdateScale(int i, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, float ratio)
{
	//--------------------------
	// �X�P�[��
	//--------------------------
	// ���ƍ��̌����̍����擾
	float scaleDiffX = nextParts.scale.x -
		m_pPartsOld[i].scale.x;

	float scaleDiffY = nextParts.scale.y -
		m_pPartsOld[i].scale.y;

	float scaleDiffZ = nextParts.scale.z -
		m_pPartsOld[i].scale.z;


	//--------------------------
	// �X�P�[�����f
	//--------------------------
	MyLib::Vector3 scale = MyLib::Vector3(0.0f, 0.0f, 0.0f);
	scale.x = m_pPartsOld[i].scale.x + (scaleDiffX * ratio);
	scale.y = m_pPartsOld[i].scale.y + (scaleDiffY * ratio);
	scale.z = m_pPartsOld[i].scale.z + (scaleDiffZ * ratio);

	// �X�P�[���ݒ�
	m_ppModel[i]->SetScale(scale);
}

//==========================================================================
// �ʒu�X�V
//==========================================================================
void CMotion::UpdatePosition(
	int i, 
	const CMotionManager::Info& nowInfo, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, 
	float ratio, int nMaxFrame,
	const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (nowInfo.nMove == IDX_VISUALMOVE)
	{// �����ڂ݈̂ʒu�X�V
		UpdateVisualPosition(i, nowInfo, nowParts, nextParts, ratio, nMaxFrame, fDeltaTime, fDeltaRate, fSlowRate);
	}
	else
	{
		UpdateEntityPosition(i, nowInfo, nowParts, nextParts, ratio, nMaxFrame, fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �{�̂��ƈʒu�X�V
//==========================================================================
void CMotion::UpdateEntityPosition(
	int i, 
	const CMotionManager::Info& nowInfo, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, 
	float ratio, int nMaxFrame,
	const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���̃L�[
	int nNextKey = (m_nPatternKey + 1) % nowInfo.nNumKey;
	if (nNextKey == 0 && nowInfo.nLoop == LOOP_OFF)
	{// ���[�v���Ȃ��Ƃ�

		// �Ō�ŌŒ�
		nNextKey = nowInfo.nNumKey - 1;
	}

	if (i == 0)
	{
		// �{�̂̈ʒu�擾
		MyLib::Vector3 pos = m_pObjChara->GetPosition();

		// �{�̂̌����擾
		MyLib::Vector3 rot = m_pObjChara->GetRotation();

		// ���̈ʒu�擾
		MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

		// �p�[�c�̈ʒu�擾
		MyLib::Vector3 posParts = m_ppModel[i]->GetPosition();
		MyLib::Vector3 posPartsOld = m_ppModel[i]->GetPosition();

		// �ڕW�̈ʒu�Ƃ̍��������߂�
		MyLib::Vector3 destpos = nowInfo.aKey[nNextKey].aParts[i].pos;
		float posDiffX = destpos.x -
			m_pPartsOld[i].pos.x;

		float posDiffY = destpos.y -
			m_pPartsOld[i].pos.y;

		float posDiffZ = destpos.z -
			m_pPartsOld[i].pos.z;

		// �e��Y��␳
		posParts.y =
			m_pPartsOld[i].pos.y + (posDiffY * ratio);
		
		// �X�P�[�����ړ�
		//posParts.y *= m_pObjChara->GetScale();

		// �ʒu�ݒ�
		m_ppModel[i]->SetPosition(posParts + posOrigin);

		int nNextMoveKey = m_nPatternKey + 1;
		if (nNextMoveKey >= nowInfo.nNumKey)
		{
			nNextMoveKey = m_nPatternKey;
		}

		// ���������������߂�
		float fMoveDiff =
			sqrtf((nowInfo.aKey[nNextMoveKey].aParts[i].pos.x - m_pPartsOld[i].pos.x) * (nowInfo.aKey[nNextMoveKey].aParts[i].pos.x - m_pPartsOld[i].pos.x)
				+ (nowInfo.aKey[nNextMoveKey].aParts[i].pos.z - m_pPartsOld[i].pos.z) * (nowInfo.aKey[nNextMoveKey].aParts[i].pos.z - m_pPartsOld[i].pos.z));
		fMoveDiff /= (static_cast<float>(nMaxFrame) / (fDeltaRate * fSlowRate));

		// �����̌������ꎞ���
		float fRot = nowInfo.aKey[nNextMoveKey].fRotMove;

		// �����̌��������ֈړ�
		pos.x += sinf(D3DX_PI + fRot + rot.y) * fMoveDiff;
		pos.z += cosf(D3DX_PI + fRot + rot.y) * fMoveDiff;

		// �ʒu�ݒ�
		m_pObjChara->SetPosition(pos);
	}
	else
	{
		// �p�[�c�̈ʒu�擾
		MyLib::Vector3 posParts = m_ppModel[i]->GetPosition();

		// ���̈ʒu�擾
		MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

		// �ڕW�̈ʒu�Ƃ̍��������߂�
		float posDiffX = nextParts.pos.x - m_pPartsOld[i].pos.x;
		float posDiffY = nextParts.pos.y - m_pPartsOld[i].pos.y;
		float posDiffZ = nextParts.pos.z - m_pPartsOld[i].pos.z;

		// �p�[�c�̈ʒu��ݒ�
		posParts.y = m_pPartsOld[i].pos.y + (posDiffY * ratio);
		posParts.x = m_pPartsOld[i].pos.x + (posDiffX * ratio);
		posParts.z = m_pPartsOld[i].pos.z + (posDiffZ * ratio);

		// �ʒu�ݒ�
		m_ppModel[i]->SetPosition(posParts + m_ppModel[i]->GetOriginPosition());
	}
}

//==========================================================================
// �����ڂ݈̂ʒu�X�V
//==========================================================================
void CMotion::UpdateVisualPosition(
	int i, 
	const CMotionManager::Info& nowInfo, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, 
	float ratio, int nMaxFrame,
	const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���̃L�[
	int nNextKey = (m_nPatternKey + 1) % nowInfo.nNumKey;
	if (nNextKey == 0 && nowInfo.nLoop == LOOP_OFF)
	{// ���[�v���Ȃ��Ƃ�

		// �Ō�ŌŒ�
		nNextKey = nowInfo.nNumKey - 1;
	}

	if (i == 0)
	{
		// �p�[�c�̈ʒu�擾
		MyLib::Vector3 posParts = m_ppModel[i]->GetPosition();

		// ���̈ʒu�擾
		MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

		// �ڕW�̈ʒu�Ƃ̍��������߂�
		float posDiffX = nextParts.pos.x - m_pPartsOld[i].pos.x;
		float posDiffY = nextParts.pos.y - m_pPartsOld[i].pos.y;
		float posDiffZ = nextParts.pos.z - m_pPartsOld[i].pos.z;

		// �p�[�c�̈ʒu��ݒ�
		posParts.y = m_pPartsOld[i].pos.y + (posDiffY * ratio);
		posParts.x = m_pPartsOld[i].pos.x + (posDiffX * ratio);
		posParts.z = m_pPartsOld[i].pos.z + (posDiffZ * ratio);

		// �ʒu�ݒ�
		m_ppModel[i]->SetPosition((posParts + m_ppModel[i]->GetOriginPosition()) * m_pObjChara->GetScale());
	}
	else
	{
		// �p�[�c�̈ʒu�擾
		MyLib::Vector3 posParts = m_ppModel[i]->GetPosition();

		// ���̈ʒu�擾
		MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

		// �ڕW�̈ʒu�Ƃ̍��������߂�
		float posDiffX = nextParts.pos.x - m_pPartsOld[i].pos.x;
		float posDiffY = nextParts.pos.y - m_pPartsOld[i].pos.y;
		float posDiffZ = nextParts.pos.z - m_pPartsOld[i].pos.z;

		// �p�[�c�̈ʒu��ݒ�
		posParts.y = m_pPartsOld[i].pos.y + (posDiffY * ratio);
		posParts.x = m_pPartsOld[i].pos.x + (posDiffX * ratio);
		posParts.z = m_pPartsOld[i].pos.z + (posDiffZ * ratio);

		// �ʒu�ݒ�
		m_ppModel[i]->SetPosition(posParts + m_ppModel[i]->GetOriginPosition());
	}
}

//==========================================================================
// ���[�V�����̐ݒ菈��
//==========================================================================
void CMotion::Set(int nType, int nStartKey, bool bBlend, float fCntFrame)
{
	// �f�o�b�O�p����
	if (nType >= m_nNumMotion)
	{// �����𒴂��Ă�����
		MyAssert::CustomAssert(false, "�͈͊O�̃��[�V����");
		return;
	}

	if (m_nType == nType)
	{// ���݂̎�ނƓ�����������
		return;
	}

	m_nOldType = m_nType;		// �O��̃��[�V�����̎��
	m_nType = nType;			// ��ސݒ�
	m_nPatternKey = nStartKey;	// ���ڂ̃L�[��
	m_fCntFrame = fCntFrame;	// �t���[���̃J�E���g
	m_fAllFrame = 0.0f;			// �S�ẴJ�E���g
	m_fMaxAllFrame = 0.0f;		// �S�ẴJ�E���g�̍ő�l
	m_bFinish = false;			// �I���������ǂ���
	m_bCancelable = false;		// �L�����Z���\��
	m_bCombiable = false;		// �R���{�\��
	m_bAttaking = false;		// �U�����蒆�t���O

	// ���݂̏��
	CMotionManager::Info& nowInfo = m_vecInfo[m_nType];

	for (int nCntKey = 0; nCntKey < nStartKey; nCntKey++)
	{
		m_fAllFrame += nowInfo.aKey[nCntKey].nFrame;	// �S�ẴJ�E���g
	}

	for (int nCntKey = 0; nCntKey < nowInfo.nNumKey; nCntKey++)
	{
		m_fMaxAllFrame += nowInfo.aKey[nCntKey].nFrame;	// �S�ẴJ�E���g�̍ő�l
	}

	// �U�����̃t���O�I�t
	for (auto& attackInfo : nowInfo.AttackInfo)
	{
		attackInfo.bInpactAct = false;
		attackInfo.bInpactActSet = false;
		attackInfo.bEndAtk = false;

		// �����t���[�����O�̕��͐ݒ�ς�
#if 0
		if (m_fAllFrame >= static_cast<float>(attackInfo.nInpactCnt))
		{
			attackInfo.bInpactAct = true;
			attackInfo.bInpactActSet = true;
		}
#endif
	}

	// �p�[�c�T�C�Y
	int partsSize = static_cast<int>(nowInfo.aKey[m_nPatternKey].aParts.size());
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{// �S�p�[�c���J��Ԃ�

		if (m_ppModel[nCntParts] == nullptr)
		{
			continue;
		}

		// �ߋ��̈ʒu�E������ۑ�
		if (bBlend)
		{
			// ���݂̏����ߋ��̏���
			m_pPartsOld[nCntParts].rot = m_ppModel[nCntParts]->GetRotation();
			m_pPartsOld[nCntParts].scale = m_ppModel[nCntParts]->GetScale();

			if (nCntParts == 0)
			{// �e�̓L�����N�^�[�̈ʒu�ɂ���
				m_pPartsOld[nCntParts].pos = m_ppModel[nCntParts]->GetPosition() - m_pObjChara->GetOriginPosition();

				if (m_vecInfo[m_nOldType].nMove == IDX_VISUALMOVE)
				{
					m_pPartsOld[nCntParts].pos = nowInfo.aKey[m_nPatternKey].aParts[nCntParts].pos;
					m_ppModel[nCntParts]->SetPosition(m_ppModel[nCntParts]->GetOriginPosition() + nowInfo.aKey[m_nPatternKey].aParts[nCntParts].pos);
				}
			}
			else
			{
				m_pPartsOld[nCntParts].pos = m_ppModel[nCntParts]->GetPosition() - m_ppModel[nCntParts]->GetOriginPosition();
			}
		}
		else
		{// �u�����h����

			if (partsSize <= nCntParts)
			{
				// ���݂̏����ߋ��̏���
				m_pPartsOld[nCntParts].rot = MyLib::Vector3();
				m_pPartsOld[nCntParts].scale = MyLib::Vector3(1.0f);

				if (nCntParts == 0)
				{// �e�̓L�����N�^�[�̈ʒu�ɂ���
					m_pPartsOld[nCntParts].pos = m_pObjChara->GetOriginPosition() - m_ppModel[nCntParts]->GetOriginPosition();
				}
				else
				{
					m_pPartsOld[nCntParts].pos = MyLib::Vector3();
				}
			}
			else
			{
				// ���݂̏����ߋ��̏���
				m_pPartsOld[nCntParts].rot = nowInfo.aKey[m_nPatternKey].aParts[nCntParts].rot;
				m_pPartsOld[nCntParts].scale = nowInfo.aKey[m_nPatternKey].aParts[nCntParts].scale;

				if (nCntParts == 0)
				{// �e�̓L�����N�^�[�̈ʒu�ɂ���
					m_pPartsOld[nCntParts].pos = nowInfo.aKey[m_nPatternKey].aParts[nCntParts].pos + m_pObjChara->GetOriginPosition() - m_ppModel[nCntParts]->GetOriginPosition();
				}
				else
				{
					m_pPartsOld[nCntParts].pos = nowInfo.aKey[m_nPatternKey].aParts[nCntParts].pos;
				}
			}

		}
	}







	// �Đ����̃L�[
	const CMotionManager::Key& nowKey = nowInfo.aKey[m_nPatternKey];

	// �Đ��t���[���ۑ�
	int nMaxFrame = nowKey.nFrame;

	// �f�t�H���g���[�V��������
	bool bDefault = std::find(m_vecDefaultIdx.begin(), m_vecDefaultIdx.end(), m_nType) != m_vecDefaultIdx.end();

	if (bDefault && m_nOldType != 0 && m_nPatternKey == 0)
	{// �j���[�g������0�̎�
		nMaxFrame = 10;
	}

	// ���̃L�[
	int nNextKey = (m_nPatternKey + 1) % nowInfo.nNumKey;

	if (nNextKey == 0 && nowInfo.nLoop == LOOP_OFF)
	{// ���[�v���Ȃ��Ƃ�

		// �Ō�ŌŒ�
		nNextKey = nowInfo.nNumKey - 1;
	}

	//--------------------------
	// �Đ��t���[���Ƃ̊���
	//--------------------------
	float ratio = m_fCntFrame / static_cast<float>(nMaxFrame);
	ratio = UtilFunc::Transformation::Clamp(ratio, 0.0f, 1.0f);

	for (int i = 0; i < static_cast<int>(nowKey.aParts.size()); i++)
	{
		// ��������������
		if (i >= m_nNumModel) continue;

		// ���݂̃p�[�c���
		const CMotionManager::Parts& nowParts = nowKey.aParts[i];
		const CMotionManager::Parts& nextParts = nowInfo.aKey[nNextKey].aParts[i];

		// ���f�����Ȃ�������߂�
		if (m_ppModel[i] == nullptr) continue;

		//--------------------------
		// �����X�V
		//--------------------------
		UpdateRotation(i, nowParts, nextParts, ratio);

		//--------------------------
		// �X�P�[���X�V
		//--------------------------
		UpdateScale(i, nowParts, nextParts, ratio);

		//--------------------------
		// �ʒu���f
		//--------------------------
		//UpdatePosition(i, nowInfo, nowParts, nextParts, ratio, nMaxFrame, 1.0f, 1.0f, 1.0f);
	}
}

//==========================================================================
// �w�肵����񂪏Ռ��J�E���g��
//==========================================================================
bool CMotion::IsImpactFrame(const CMotionManager::Info& info)
{
	// �U�����̃R���e�i
	const std::vector<CMotionManager::AttackInfo>& vecAtkInfo = info.AttackInfo;

	for (const auto& atkInfo : vecAtkInfo)
	{
		// �Ռ��t���[������Ȃ��Ɣ�����
		if (!atkInfo.bInpactAct) continue;

		return true;
	}

	return false;
}

//==========================================================================
// �w�肵���C���f�b�N�X�̏�񂪏Ռ��J�E���g��
//==========================================================================
bool CMotion::IsImpactFrame(int nCntAtk)
{
	// �U�����̃R���e�i
	const std::vector<CMotionManager::AttackInfo>& vecAtkInfo = m_vecInfo[m_nType].AttackInfo;

	// �͈͊O
	if (static_cast<int>(vecAtkInfo.size()) <= nCntAtk) return false;

	return vecAtkInfo[nCntAtk].bInpactAct;
}

//==========================================================================
// �w�肵����񂪃t���[����
//==========================================================================
bool CMotion::IsAlignFrame(const CMotionManager::Info& info)
{
	// �U�����̃R���e�i
	const std::vector<CMotionManager::AttackInfo>& vecAtkInfo = m_vecInfo[m_nType].AttackInfo;

	for (const auto& atkInfo : vecAtkInfo)
	{
		// �������
		const CMotionManager::AlignInfo& align = atkInfo.AlignInfo;

		// �t���[���O�Ŗ߂�
		if (!align.bSet) continue;

		return true;
	}

	return false;
}

//==========================================================================
// �U���̈ʒu�擾
//==========================================================================
MyLib::Vector3 CMotion::GetAttackPosition(CModel** ppModel, CMotionManager::AttackInfo attackInfo)
{
	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	// ��������
	if (attackInfo.nCollisionNum >= m_nNumModel) return MyLib::Vector3();

	if (ppModel[attackInfo.nCollisionNum] == nullptr) return MyLib::Vector3();

	// ���肷��p�[�c�̃}�g���b�N�X�擾
	D3DXMATRIX mtxWepon = ppModel[attackInfo.nCollisionNum]->GetWorldMtx().ConvertD3DXMATRIX();

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, attackInfo.Offset.x, attackInfo.Offset.y, attackInfo.Offset.z);
	D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

	return MyLib::Vector3(mtxWepon._41, mtxWepon._42, mtxWepon._43);
}

//==========================================================================
// �U���̈ʒu�擾
//==========================================================================
MyLib::Vector3 CMotion::GetAttackPosition(CModel* pModel, CMotionManager::AttackInfo attackInfo)
{
	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	if (pModel == nullptr)
	{// nullptr��������
		return MyLib::Vector3();
	}

	// ���肷��p�[�c�̃}�g���b�N�X�擾
	D3DXMATRIX mtxWepon = pModel->GetWorldMtx().ConvertD3DXMATRIX();

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, attackInfo.Offset.x, attackInfo.Offset.y, attackInfo.Offset.z);
	D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

	return MyLib::Vector3(mtxWepon._41, mtxWepon._42, mtxWepon._43);
}

//==========================================================================
// �S�ẴJ�E���g�擾
//==========================================================================
int CMotion::GetMaxAllCount(int nType)
{
	int nAllFrame = 0;
	for (int nCntKey = 0; nCntKey < m_vecInfo[nType].nNumKey; nCntKey++)
	{
		nAllFrame += m_vecInfo[nType].aKey[nCntKey].nFrame;	// �S�ẴJ�E���g�̍ő�l
	}
	return nAllFrame;
}
