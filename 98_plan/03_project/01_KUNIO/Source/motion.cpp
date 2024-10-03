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
// �ÓI�����o�ϐ��錾
//==========================================================================
std::vector<std::string> CMotion::m_sTextFile = {};		// �e�L�X�g�t�@�C����
std::vector<CMotion::Info> CMotion::m_aLoadData[MAX_MOTION] = {};	// ���[�V�����̏��
std::vector<CMotion::AttackInfo> CMotion::m_aLoadAttackData[MAX_MOTION][MAX_PARTS] = {};	// �U���̓ǂݍ��ݏ��
int CMotion::m_nNumLoad = 0;								// �ǂݍ��񂾐�
std::vector<int> CMotion::m_nNumLoadData = {};				// ���[�V�������̃f�[�^��

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMotion::CMotion()
{
	// �l�̃N���A
	m_nNumAll = 0;			// ���[�V�����̑���
	m_nType = 0;			// ���݂̃��[�V�����̎��
	m_nOldType = 0;			// �O��̃��[�V�����̎��
	m_bLoop = false;		// ���[�v���邩�ǂ���
	m_nPatternKey = 0;		// ���ڂ̃L�[��
	m_fCntFrame = 0.0f;		// �t���[���̃J�E���g
	m_fCntAllFrame = 0.0f;	// �S�ẴJ�E���g
	m_fMaxAllFrame = 0.0f;	// �S�ẴJ�E���g�̍ő�l
	m_fSlowFactor = 0.0f;	// �x���W��
	m_bFinish = false;		// �I���������ǂ���
	m_bCancelable = false;	// �L�����Z���\��
	m_bCombiable = false;	// �R���{�\��
	m_bBeforeInAction = false;		// �U���O�t���O
	m_bAttaking = false;			// �U�����蒆�t���O
	m_bAllAfterAttack = false;		// �S�Ă̍U����t���O

	m_pObjChara = nullptr;		// �I�u�W�F�N�g�̃|�C���^
	m_ppModel = nullptr;		// ���f���̃|�C���^
	m_nNumModel = 0;		// ���f���̑���
	m_nNumMotion = 0;		// ���[�V�����̑���
	m_pInfo = nullptr;		// ���[�V�������
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
		pMotion->Init();
		pMotion->ReadText(file);
	}

	return pMotion;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMotion::Init()
{
	m_nNumAll = 0;			// ���[�V�����̑���
	m_nType = 0;			// ���݂̃��[�V�����̎��
	m_bLoop = false;		// ���[�v���邩�ǂ���
	m_nPatternKey = 0;		// ���ڂ̃L�[��
	m_fCntFrame = 0.0f;		// �t���[���̃J�E���g
	m_fCntAllFrame = 0.0f;	// �S�ẴJ�E���g
	m_fMaxAllFrame = 0.0f;	// �S�ẴJ�E���g�̍ő�l
	m_bFinish = true;		// �I���������ǂ���
	m_ppModel = nullptr;		// ���f���̃|�C���^
	m_nNumModel = 0;		// ���f���̑���
	m_fSlowFactor = 1.0f;	// �x���W��

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMotion::Uninit()
{
	if (m_pInfo != nullptr)
	{
		// �U�����̔j��
		for (int nCntMotion = 0; nCntMotion < m_nNumMotion; nCntMotion++)
		{
			for (int nCntAttack = 0; nCntAttack < MAX_PARTS; nCntAttack++)
			{
				if (m_pInfo[nCntMotion].AttackInfo[nCntAttack] != nullptr)
				{
					delete m_pInfo[nCntMotion].AttackInfo[nCntAttack];
					m_pInfo[nCntMotion].AttackInfo[nCntAttack] = nullptr;
				}
			}
		}

		delete[] m_pInfo;
		m_pInfo = nullptr;
	}

	if (m_pPartsOld != nullptr)
	{
		delete[] m_pPartsOld;
		m_pPartsOld = nullptr;
	}
}

//==========================================================================
// ���[�V�������̓o�^
//==========================================================================
void CMotion::SetInfo(Info info)
{
	if (m_nNumMotion <= m_nNumAll)
	{
		return;
	}

	// �R�s�[
	memcpy(m_pInfo[m_nNumAll].aKey, info.aKey, sizeof(info.aKey));

	m_pInfo[m_nNumAll].nLoop = info.nLoop;
	m_pInfo[m_nNumAll].nMove = info.nMove;
	m_pInfo[m_nNumAll].nNumKey = info.nNumKey;
	m_pInfo[m_nNumAll].nCancelableFrame = info.nCancelableFrame;
	m_pInfo[m_nNumAll].nCombolableFrame = info.nCombolableFrame;

	for (int nCntKey = 0; nCntKey < m_pInfo[m_nNumAll].nNumKey; nCntKey++)
	{
		int nBeforeCnt = nCntKey - 1;
		if (nBeforeCnt <= 0)
		{
			nBeforeCnt = 0;
		}

		float PosX = m_pInfo[m_nNumAll].aKey[nBeforeCnt].aParts[0].pos.x;
		float PosZ = m_pInfo[m_nNumAll].aKey[nBeforeCnt].aParts[0].pos.z;

		// ���������߂�
		m_pInfo[m_nNumAll].aKey[nCntKey].fRotMove = atan2f(
			(PosX - m_pInfo[m_nNumAll].aKey[nCntKey].aParts[0].pos.x),
			(PosZ - m_pInfo[m_nNumAll].aKey[nCntKey].aParts[0].pos.z));
	}

	// ���[�V�����̑������Z
	m_nNumAll++;
}

//==========================================================================
// �U�����̓o�^
//==========================================================================
void CMotion::SetAttackInfo(AttackInfo info)
{
	int nIdx = m_pInfo[m_nNumAll].nNumAttackInfo;

	// �������m��
	m_pInfo[m_nNumAll].AttackInfo[nIdx] = DEBUG_NEW AttackInfo;

	if (m_pInfo[m_nNumAll].AttackInfo[nIdx] != nullptr)
	{// nullptr����Ȃ����

		// ���n��
		*m_pInfo[m_nNumAll].AttackInfo[nIdx] = info;

		// �U�����̑������Z
		m_pInfo[m_nNumAll].nNumAttackInfo++;
	}
}

//==========================================================================
// ���[�V���������郂�f���̓o�^
//==========================================================================
void CMotion::SetModel(CModel **pModel, int nNumModel)
{
	// ���f���̃|�C���^��n��
	m_ppModel = pModel;

	// ���f���̑���
	m_nNumModel = nNumModel;

	m_pPartsOld = DEBUG_NEW Parts[m_nNumModel];
	if (m_pPartsOld == nullptr)
	{
		return;
	}
	memset(m_pPartsOld, 0, sizeof(Parts) * m_nNumModel);
}

//==========================================================================
// �|�[�Y�̃��Z�b�g
//==========================================================================
void CMotion::ResetPose(int nType)
{
	

	int nStartIdx = m_pObjChara->GetMotionStartIdx();
	for (int nCntParts = nStartIdx; nCntParts < m_nNumModel + nStartIdx + 1; nCntParts++)
	{// �S�p�[�c���J��Ԃ�

		int nCntModel = nCntParts;
		if (nStartIdx != 0)
		{
			nCntModel = nCntParts - nStartIdx;
		}

		if (nCntModel >= m_nNumModel)
		{
			break;
		}

		if (m_ppModel[nCntModel] == nullptr)
		{// nullptr��������
			continue;
		}

		// �����ݒ�
		m_ppModel[nCntModel]->SetRotation(m_pInfo[nType].aKey[0].aParts[nCntParts].rot);
		m_pPartsOld[nCntParts].rot = m_pInfo[nType].aKey[0].aParts[nCntParts].rot;
		m_pPartsOld[nCntParts].scale = m_pInfo[nType].aKey[0].aParts[nCntParts].scale;

		// ���̈ʒu�擾
		MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

		// �ʒu�ݒ�
		if (nCntParts == 0)
		{
			m_ppModel[nCntModel]->SetPosition(m_pInfo[nType].aKey[0].aParts[nCntParts].pos + posOrigin);
		}
		else
		{
			m_ppModel[nCntModel]->SetPosition(m_pInfo[nType].aKey[0].aParts[nCntParts].pos + m_ppModel[nCntModel]->GetOriginPosition());
		}
	}

	m_pPartsOld[0].pos = m_pInfo[nType].aKey[0].aParts[0].pos;

	Set(nType);
}

//==========================================================================
// �X�V����
//==========================================================================
void CMotion::Update(float fBuff)
{
#if 0
	// �e�L�X�g�̕`��
	CManager::GetInstance()->GetDebugProc()->Print(
		"�{���F%f\n", fBuff);
#endif

	// �U�����蒆�t���O���Z�b�g
	m_bAttaking = false;

	// �U�����̑����擾
	int nNumAttackInfo = m_pInfo[m_nType].nNumAttackInfo;

	for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
	{
		if (m_pInfo[m_nType].AttackInfo[nCntAttack] == nullptr)
		{
			continue;
		}

		if (m_bBeforeInAction)
		{
			if (m_pInfo[m_nType].AttackInfo[nCntAttack]->nMaxCnt > 0 &&
				m_pInfo[m_nType].AttackInfo[nCntAttack]->nMinCnt <= m_fCntAllFrame)
			{
				m_bBeforeInAction = false;
			}
		}

		if (m_pInfo[m_nType].AttackInfo[nCntAttack]->nInpactCnt < 0)
		{
			continue;
		}

		// �܂��Ռ��J�E���g�̍s�������ĂȂ���Ԃɂ���
		m_pInfo[m_nType].AttackInfo[nCntAttack]->bInpactAct = false;

		if (m_fCntAllFrame >= m_pInfo[m_nType].AttackInfo[nCntAttack]->nInpactCnt &&
			m_pInfo[m_nType].AttackInfo[nCntAttack]->bInpactActSet == false)
		{// �Ռ��̃J�E���g�𒴂����� && �܂��s�����ĂȂ�������
			
			// �܂��Ռ��J�E���g�̍s�������ĂȂ���Ԃɂ���
			m_pInfo[m_nType].AttackInfo[nCntAttack]->bInpactAct = true;
			m_pInfo[m_nType].AttackInfo[nCntAttack]->bInpactActSet = true;
		}
	}


	if (m_bFinish && m_pInfo[m_nType].nLoop == LOOP_OFF)
	{// �I�����Ă� && ���[�vOFF��������
		return;
	}

	// �Đ��t���[��
	int nFrame;

	// �Đ��t���[���ۑ�
	nFrame = m_pInfo[m_nType].aKey[m_nPatternKey].nFrame;

	if (m_nType == 0 && m_nOldType != 0 && m_nPatternKey == 0)
	{// �j���[�g������0�̎�
		nFrame = 10;
	}

	// ���̃L�[
	int nNextKey = (m_nPatternKey + 1) % m_pInfo[m_nType].nNumKey;

	if (nNextKey == 0 && m_pInfo[m_nType].nLoop == LOOP_OFF)
	{// ���[�v���Ȃ��Ƃ�

		// �Ō�ŌŒ�
		nNextKey = m_pInfo[m_nType].nNumKey - 1;
	}

	int nStartIdx = m_pObjChara->GetMotionStartIdx();
	for (int nCntParts = nStartIdx; nCntParts < m_nNumModel + nStartIdx + 1; nCntParts++)
	{// �S�p�[�c���J��Ԃ�

		int nCntModel = nCntParts;
		if (nStartIdx != 0)
		{
			nCntModel = nCntParts - nStartIdx;
		}

		if (nCntModel >= m_nNumModel)
		{
			break;
		}

		if (m_ppModel[nCntModel] == nullptr)
		{// nullptr��������
			continue;
		}

		// ���ƍ��̌����̍����擾
		float rotDiffX = m_pInfo[m_nType].aKey[nNextKey].aParts[nCntParts].rot.x -
			m_pPartsOld[nCntParts].rot.x;

		float rotDiffY = m_pInfo[m_nType].aKey[nNextKey].aParts[nCntParts].rot.y -
			m_pPartsOld[nCntParts].rot.y;

		float rotDiffZ = m_pInfo[m_nType].aKey[nNextKey].aParts[nCntParts].rot.z -
			m_pPartsOld[nCntParts].rot.z;

		// �p�x�̐��K��
		UtilFunc::Transformation::RotNormalize(rotDiffX);
		UtilFunc::Transformation::RotNormalize(rotDiffY);
		UtilFunc::Transformation::RotNormalize(rotDiffZ);

		// �p�[�c�̌�����ݒ�
		MyLib::Vector3 rot = mylib_const::DEFAULT_VECTOR3;

		// �p�[�c�̌�����ݒ�
		rot.x =
			m_pPartsOld[nCntParts].rot.x +
			rotDiffX *
			(
			m_fCntFrame /
			(float)nFrame
			);

		// �p�[�c�̌�����ݒ�
		rot.y =
			m_pPartsOld[nCntParts].rot.y +
			rotDiffY *
			(
			m_fCntFrame /
			(float)nFrame
			);

		// �p�[�c�̌�����ݒ�
		rot.z =
			m_pPartsOld[nCntParts].rot.z +
			rotDiffZ *
			(
			m_fCntFrame /
			(float)nFrame
			);

		// �p�x�̐��K��
		UtilFunc::Transformation::RotNormalize(rot.x);
		UtilFunc::Transformation::RotNormalize(rot.y);
		UtilFunc::Transformation::RotNormalize(rot.z);

		// �����ݒ�
		m_ppModel[nCntModel]->SetRotation(rot);

		{
			// ���ƍ��̌����̍����擾
			float scaleDiffX = m_pInfo[m_nType].aKey[nNextKey].aParts[nCntParts].scale.x -
				m_pPartsOld[nCntParts].scale.x;

			float scaleDiffY = m_pInfo[m_nType].aKey[nNextKey].aParts[nCntParts].scale.y -
				m_pPartsOld[nCntParts].scale.y;

			float scaleDiffZ = m_pInfo[m_nType].aKey[nNextKey].aParts[nCntParts].scale.z -
				m_pPartsOld[nCntParts].scale.z;

			// �p�[�c�̌�����ݒ�
			MyLib::Vector3 scale = MyLib::Vector3(0.0f, 0.0f, 0.0f);

			// �p�[�c�̌�����ݒ�
			scale.x =
				m_pPartsOld[nCntParts].scale.x +
				scaleDiffX *
				(
				m_fCntFrame /
				(float)nFrame
				);

			// �p�[�c�̌�����ݒ�
			scale.y =
				m_pPartsOld[nCntParts].scale.y +
				scaleDiffY *
				(
				m_fCntFrame /
				(float)nFrame
				);

			// �p�[�c�̌�����ݒ�
			scale.z =
				m_pPartsOld[nCntParts].scale.z +
				scaleDiffZ *
				(
				m_fCntFrame /
				(float)nFrame
				);

			// �����ݒ�
			m_ppModel[nCntModel]->SetScale(scale);
		}

		if (nCntParts == 0)
		{
			// �{�̂̈ʒu�擾
			MyLib::Vector3 pos = m_pObjChara->GetPosition();

			// �{�̂̌����擾
			MyLib::Vector3 rot = m_pObjChara->GetRotation();

			// ���̈ʒu�擾
			MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

			// �p�[�c�̈ʒu�擾
			MyLib::Vector3 posParts = m_ppModel[nCntModel]->GetPosition();
			MyLib::Vector3 posPartsOld = m_ppModel[nCntModel]->GetPosition();

			// �ڕW�̈ʒu�Ƃ̍��������߂�
			float posDiffX = m_pInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.x -
				m_pPartsOld[nCntParts].pos.x;

			float posDiffY = m_pInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.y -
				m_pPartsOld[nCntParts].pos.y;

			float posDiffZ = m_pInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.z -
				m_pPartsOld[nCntParts].pos.z;

			// �e��Y��␳
			posParts.y =
				m_pPartsOld[nCntParts].pos.y +
				posDiffY *
				(
				m_fCntFrame /
				(float)nFrame);

			// �ʒu�ݒ�
			m_ppModel[nCntModel]->SetPosition(posParts + posOrigin);

			int nNextMoveKey = m_nPatternKey + 1;
			if (nNextMoveKey >= m_pInfo[m_nType].nNumKey)
			{
				nNextMoveKey = m_nPatternKey;
			}

			// ���������������߂�
			float fMoveDiff =
				sqrtf((m_pInfo[m_nType].aKey[nNextMoveKey].aParts[nCntParts].pos.x - m_pPartsOld[nCntParts].pos.x) * (m_pInfo[m_nType].aKey[nNextMoveKey].aParts[nCntParts].pos.x - m_pPartsOld[nCntParts].pos.x)
					+ (m_pInfo[m_nType].aKey[nNextMoveKey].aParts[nCntParts].pos.z - m_pPartsOld[nCntParts].pos.z) * (m_pInfo[m_nType].aKey[nNextMoveKey].aParts[nCntParts].pos.z - m_pPartsOld[nCntParts].pos.z));
			fMoveDiff /= (static_cast<float>(nFrame) / static_cast<float>(fBuff));

			// �����̌������ꎞ���
			float fRot = m_pInfo[m_nType].aKey[nNextMoveKey].fRotMove;

			// �����̌��������ֈړ�
			pos.x += sinf(D3DX_PI + fRot + rot.y) * fMoveDiff;
			pos.z += cosf(D3DX_PI + fRot + rot.y) * fMoveDiff;

			m_pObjChara->SetPosition(pos);
		}
		else
		{
			// �p�[�c�̈ʒu�擾
			MyLib::Vector3 posParts = m_ppModel[nCntModel]->GetPosition();

			// ���̈ʒu�擾
			MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

			// �ڕW�̈ʒu�Ƃ̍��������߂�
			float posDiffX = m_pInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.x -
				m_pPartsOld[nCntParts].pos.x;

			float posDiffY = m_pInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.y -
				m_pPartsOld[nCntParts].pos.y;

			float posDiffZ = m_pInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.z -
				m_pPartsOld[nCntParts].pos.z;

			// �p�[�c�̈ʒu��ݒ�
			posParts.y =
				m_pPartsOld[nCntParts].pos.y +
				posDiffY *
				(m_fCntFrame /
				(float)nFrame);

			posParts.x =
				m_pPartsOld[nCntParts].pos.x +
				posDiffX *
				(m_fCntFrame /
				(float)nFrame);

			posParts.z =
				m_pPartsOld[nCntParts].pos.z +
				posDiffZ *
				(m_fCntFrame /
				(float)nFrame);

			// �ʒu�ݒ�
			m_ppModel[nCntModel]->SetPosition(posParts + m_ppModel[nCntModel]->GetOriginPosition());
		}
	}

	// �t���[���̃J�E���g�����Z
	m_fCntFrame += 1.0f * fBuff;
	m_fCntAllFrame += 1.0f * fBuff;
	m_bAllAfterAttack = false;		// �S�Ă̍U����t���O

	// �U���̍ő�t���[��
	float maxFrame = -1.0f;

	// �U�����蒆�t���O�ݒ�
	for (int nCntAttack = 0; nCntAttack < m_pInfo[m_nType].nNumAttackInfo; nCntAttack++)
	{
		AttackInfo* pAttack = m_pInfo[m_nType].AttackInfo[nCntAttack];
		if (pAttack == nullptr)
		{
			continue;
		}

		if (pAttack->nMaxCnt == 0)
		{
			continue;
		}

		// �U���̍ő�t���[��
		if (maxFrame < static_cast<float>(pAttack->nMaxCnt))
		{
			maxFrame = static_cast<float>(pAttack->nMaxCnt);
		}

		if (maxFrame < m_fCntAllFrame)
		{
			m_bAllAfterAttack = true;
		}
		else
		{
			m_bAllAfterAttack = false;
		}

		// �U������
		if (m_fCntAllFrame >= static_cast<float>(pAttack->nMinCnt) && m_fCntAllFrame <= maxFrame)
		{
			m_bAttaking = true;
		}
	}


	if (m_pInfo[m_nType].nCombolableFrame != -1 &&
		m_pInfo[m_nType].nCombolableFrame <= m_fCntAllFrame)
	{// -1����Ȃ��A�t���[���𒴂�����
		m_bCombiable = true;	// �R���{�\
	}

	if (m_pInfo[m_nType].nCancelableFrame != -1 &&
		m_pInfo[m_nType].nCancelableFrame <= m_fCntAllFrame)
	{// -1����Ȃ��A�t���[���𒴂�����
		m_bCancelable = true;	// �L�����Z���\
	}

	if (m_pInfo[m_nType].nLoop == LOOP_ON)
	{// ���[�v���[�V�����͂��ł��I����Ă�

		// �I������ON
		m_bFinish = true;
	}

	if (m_fCntFrame >= (float)nFrame)
	{// �t���[���̃J�E���g���t���[�����ɒB������

		// �t���[���̃J�E���g���[���ɖ߂�
		m_fCntFrame = 0;

		m_nOldType = m_nType;	// �O��̃��[�V�����̎��

		// �p�^�[��NO.�X�V
		m_nPatternKey = (m_nPatternKey + 1) % m_pInfo[m_nType].nNumKey;

		int nStartIdx = m_pObjChara->GetMotionStartIdx();
		for (int nCntParts = nStartIdx; nCntParts < m_nNumModel + nStartIdx + 1; nCntParts++)
		{// �S�p�[�c���J��Ԃ�

			int nCntModel = nCntParts;
			if (nStartIdx != 0)
			{
				nCntModel = nCntParts - nStartIdx;
			}

			if (nCntModel >= m_nNumModel)
			{
				break;
			}

			m_pPartsOld[nCntParts].rot = m_pInfo[m_nType].aKey[m_nPatternKey].aParts[nCntParts].rot;
			m_pPartsOld[nCntParts].pos = m_pInfo[m_nType].aKey[m_nPatternKey].aParts[nCntParts].pos;
			m_pPartsOld[nCntParts].scale = m_pInfo[m_nType].aKey[m_nPatternKey].aParts[nCntParts].scale;
		}

		if (m_nPatternKey == 0)
		{// �L�[���[���̎�

			// ���t���[�������Z�b�g
			m_fCntAllFrame = 0.0f;

			//���[�V�����̐ݒ�
			if (m_pInfo[m_nType].nLoop == LOOP_OFF)
			{// ���[�v���[�V��������Ȃ���

				// �I������ON
				m_bFinish = true;
			}
			else
			{
				for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
				{
					if (m_pInfo[m_nType].AttackInfo[nCntAttack] == nullptr)
					{// nullptr��������
						continue;
					}

					if (m_pInfo[m_nType].AttackInfo[nCntAttack]->nInpactCnt < 0)
					{
						continue;
					}

					// �܂��Ռ��J�E���g�̍s�������ĂȂ���Ԃɂ���
					m_pInfo[m_nType].AttackInfo[nCntAttack]->bInpactAct = false;
					m_pInfo[m_nType].AttackInfo[nCntAttack]->bInpactActSet = false;
					m_pInfo[m_nType].AttackInfo[nCntAttack]->bEndAtk = false;
				}
			}
		}
	}
}

//==========================================================================
// ���[�V�����̐ݒ菈��
//==========================================================================
void CMotion::Set(int nType, bool bBlend)
{
	// �f�o�b�O�p����
	if (nType >= m_nNumAll)
	{// �����𒴂��Ă�����
		return;
	}

	if (m_nType == nType)
	{// ���݂̎�ނƓ�����������
		return;
	}

	m_nOldType = m_nType;	// �O��̃��[�V�����̎��
	m_nType = nType;		// ��ސݒ�
	m_nPatternKey = 0;		// ���ڂ̃L�[��
	m_fCntFrame = 0.0f;		// �t���[���̃J�E���g
	m_fCntAllFrame = 0.0f;	// �S�ẴJ�E���g
	m_fMaxAllFrame = 0.0f;	// �S�ẴJ�E���g�̍ő�l
	m_bFinish = false;		// �I���������ǂ���
	m_bCancelable = false;	// �L�����Z���\��
	m_bCombiable = false;	// �R���{�\��
	m_bBeforeInAction = true;		// �U���O�t���O
	m_bAttaking = false;			// �U�����蒆�t���O
	m_bAllAfterAttack = false;		// �S�Ă̍U����t���O

	for (int nCntKey = 0; nCntKey < m_pInfo[m_nPatternKey].nNumKey; nCntKey++)
	{
		m_fMaxAllFrame += m_pInfo[m_nPatternKey].aKey[nCntKey].nFrame;	// �S�ẴJ�E���g�̍ő�l
	}

	// �U�����̑����擾
	int nNumAttackInfo = m_pInfo[m_nType].nNumAttackInfo;
	for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
	{
		if (m_pInfo[m_nType].AttackInfo[nCntAttack] == nullptr)
		{// nullptr��������
			continue;
		}

		// �܂��Ռ��J�E���g�̍s�������ĂȂ���Ԃɂ���
		m_pInfo[m_nType].AttackInfo[nCntAttack]->bInpactAct = false;
		m_pInfo[m_nType].AttackInfo[nCntAttack]->bInpactActSet = false;

		// �U���̃t���O���Z�b�g
		m_pInfo[m_nType].AttackInfo[nCntAttack]->bEndAtk = false;
	}

	int nStartIdx = m_pObjChara->GetMotionStartIdx();
	for (int nCntParts = nStartIdx; nCntParts < m_nNumModel + nStartIdx + 1; nCntParts++)
	{// �S�p�[�c���J��Ԃ�

		int nCntModel = nCntParts;
		if (nStartIdx != 0)
		{
			nCntModel = nCntParts - nStartIdx;
		}

		if (nCntModel >= m_nNumModel)
		{
			break;
		}

		if (m_ppModel[nCntModel] == nullptr)
		{// nullptr��������
			continue;
		}

		// �ߋ��̈ʒu�E������ۑ�
		if (bBlend)
		{
			m_pPartsOld[nCntParts].rot = m_ppModel[nCntModel]->GetRotation();
			m_pPartsOld[nCntParts].scale = m_ppModel[nCntModel]->GetScale();

			if (nStartIdx == nCntParts)
			{
				m_pPartsOld[nCntParts].pos = m_ppModel[nCntModel]->GetPosition() - m_pObjChara->GetOriginPosition();
			}
			else
			{
				m_pPartsOld[nCntParts].pos = m_ppModel[nCntModel]->GetPosition() - m_ppModel[nCntModel]->GetOriginPosition();
			}
		}
		else
		{
			m_pPartsOld[nCntParts].rot = m_pInfo[m_nType].aKey[0].aParts[nCntParts].rot;

			if (nStartIdx == nCntParts)
			{
				m_pPartsOld[nCntParts].pos = m_pInfo[m_nType].aKey[0].aParts[nCntParts].pos + m_pObjChara->GetOriginPosition() - m_ppModel[nCntModel]->GetOriginPosition();
			}
			else
			{
				m_pPartsOld[nCntParts].pos = m_pInfo[m_nType].aKey[0].aParts[nCntParts].pos;
			}
			//m_pPartsOld[nCntParts].pos = m_pInfo[m_nType].aKey[0].aParts[nCntParts].pos + m_pObjChara->GetOriginPosition();

			m_pPartsOld[nCntParts].scale = m_pInfo[m_nType].aKey[0].aParts[nCntParts].scale;
		}
	}
}

//==========================================================================
// ���݂̃��[�V�����^�C�v�擾
//==========================================================================
int CMotion::GetType()
{
	return m_nType;
}

//==========================================================================
// �O��̃��[�V�����^�C�v�擾
//==========================================================================
int CMotion::GetOldType()
{
	return m_nOldType;
}

//==========================================================================
// �I�����Ă��邩�̔���
//==========================================================================
bool CMotion::IsFinish()
{
	return m_bFinish;
}

//==========================================================================
// �I�����Ă��邩�̔���؂�ւ�
//==========================================================================
void CMotion::ToggleFinish(bool bFinish)
{
	m_bFinish = bFinish;
}

//==========================================================================
// �x���W���̐ݒ�
//==========================================================================
void CMotion::SetSlowFactor(float fFactor)
{
	m_fSlowFactor = fFactor;
}

//==========================================================================
// �ړ��̔���擾
//==========================================================================
int CMotion::IsGetMove(int nType)
{
	return m_pInfo[nType].nMove;
}

//==========================================================================
// ���[�V�������̐ݒ�
//==========================================================================
void CMotion::SetInfoData(int nType, Info info)
{
	m_pInfo[nType] = info;
}

//==========================================================================
// ���[�V�������̎擾
//==========================================================================
CMotion::Info CMotion::GetInfo(int nType)
{
	return m_pInfo[nType];
}

//==========================================================================
// ���[�V�������̎擾
//==========================================================================
CMotion::Info *CMotion::GetInfoPtr(int nType)
{
	return &m_pInfo[nType];
}

//==========================================================================
// ���[�V�������̓o�^
//==========================================================================
void CMotion::SetInfoSave(int nType, int nKey, int nParts, MyLib::Vector3 rot)
{
	m_pInfo[nType].aKey[nKey].aParts[nParts].rot = rot;
}

//==========================================================================
// �L�[�����ύX
//==========================================================================
void CMotion::ChangeKeyNum(int nType, int nNum)
{
	m_pInfo[nType].nNumKey = nNum;
}

//==========================================================================
// �L�[�t���[���ύX
//==========================================================================
void CMotion::ChangeKeyFrame(int nType, int nKey, int nNum)
{
	m_pInfo[nType].aKey[nKey].nFrame = nNum;
}

//==========================================================================
// �U�����ύX
//==========================================================================
void CMotion::ChangeAttackInfo(int nType, int nIdx, AttackInfo info)
{
	*m_pInfo[nType].AttackInfo[nIdx] = info;
}

//==========================================================================
// �U�����̑������Z
//==========================================================================
void CMotion::AddNumAttackInfo(int nType)
{
	if (m_pInfo[nType].nNumAttackInfo >= MAX_PARTS)
	{
		return;
	}

	// �������m��
	m_pInfo[nType].AttackInfo[m_pInfo[nType].nNumAttackInfo] = DEBUG_NEW AttackInfo;

	if (m_pInfo[nType].AttackInfo[m_pInfo[nType].nNumAttackInfo] != nullptr)
	{// nullptr����Ȃ����

		// ������
		memset(m_pInfo[nType].AttackInfo[m_pInfo[nType].nNumAttackInfo], 0, sizeof(AttackInfo));

		// �U�����̑������Z
		m_pInfo[nType].nNumAttackInfo++;
	}
}

//==========================================================================
// �U�����̑������Z
//==========================================================================
void CMotion::SubNumAttackInfo(int nType)
{
	if (m_pInfo[nType].nNumAttackInfo <= 1)
	{
		return;
	}
	int nIdx = m_pInfo[nType].nNumAttackInfo - 1;

	// ���������
	//if (m_pInfo[nType].AttackInfo[nIdx] != nullptr)
	{
		delete m_pInfo[nType].AttackInfo[nIdx];
		m_pInfo[nType].AttackInfo[nIdx] = nullptr;

		// �U�����̑������Z
		m_pInfo[nType].nNumAttackInfo--;
	}
}

//==========================================================================
// �U�����̑������Z
//==========================================================================
CMotion::Parts CMotion::GetPartsOld(int nParts)
{ 
	return m_pPartsOld[nParts];
}

//==========================================================================
// �U�����̑������Z
//==========================================================================
void CMotion::SetPartsOld(int nParts, Parts parts)
{ 
	m_pPartsOld[nParts] = parts;
}

//==========================================================================
// �Ռ��̃t���[�����ǂ����擾
//==========================================================================
bool CMotion::IsImpactFrame(AttackInfo attackInfo)
{
	return attackInfo.bInpactAct;
}

//==========================================================================
// �U���̈ʒu�擾
//==========================================================================
MyLib::Vector3 CMotion::GetAttackPosition(CModel **ppModel, AttackInfo attackInfo)
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
MyLib::Vector3 CMotion::GetAttackPosition(CModel *pModel, AttackInfo attackInfo)
{
	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	if (pModel == nullptr)
	{// nullptr��������
		return mylib_const::DEFAULT_VECTOR3;
	}

	// ���肷��p�[�c�̃}�g���b�N�X�擾
	D3DXMATRIX mtxWepon = pModel->GetWorldMtx().ConvertD3DXMATRIX();

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, attackInfo.Offset.x, attackInfo.Offset.y, attackInfo.Offset.z);
	D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

	return MyLib::Vector3(mtxWepon._41, mtxWepon._42, mtxWepon._43);
}

//==========================================================================
// �t���[���J�E���g�ݒ�
//==========================================================================
void CMotion::SetFrameCount(float fCnt)
{
	m_fCntFrame = fCnt;
}

//==========================================================================
// �J�E���g�擾
//==========================================================================
float CMotion::GetAllCount()
{
	return m_fCntAllFrame;
}

//==========================================================================
// �S�ẴJ�E���g�擾
//==========================================================================
int CMotion::GetMaxAllCount(int nType)
{
	int nAllFrame = 0;
	for (int nCntKey = 0; nCntKey < m_pInfo[nType].nNumKey; nCntKey++)
	{
		nAllFrame += m_pInfo[nType].aKey[nCntKey].nFrame;	// �S�ẴJ�E���g�̍ő�l
	}
	return nAllFrame;
}

//==========================================================================
// �O���t�@�C���ǂݍ���
//==========================================================================
void CMotion::ReadText(const std::string& file)
{
	// �ǂݍ��݊m�F
	std::vector<std::string>::iterator itr = std::find(m_sTextFile.begin(), m_sTextFile.end(), file);
	if (itr != m_sTextFile.end())
	{// �t�@�C��������v

		// �C���f�b�N�X�Z�o
		int nIdx = itr - m_sTextFile.begin();

		// ���[�V������
		m_nNumMotion = m_nNumLoadData[nIdx];

		// ���[�V�����̏�񐶐�
		m_pInfo = DEBUG_NEW Info[m_nNumMotion];
		memset(m_pInfo, 0, sizeof(Info) * m_nNumMotion);

		for (int nCntInfo = 0; nCntInfo < m_nNumMotion; nCntInfo++)
		{
			for (int nCntATK = 0; nCntATK < m_aLoadData[nIdx][nCntInfo].nNumAttackInfo; nCntATK++)
			{// �U����񕪌J��Ԃ�

				// �U�����o�^
				SetAttackInfo(m_aLoadAttackData[nIdx][nCntInfo][nCntATK]);
			}

			// ���o�^
			SetInfo(m_aLoadData[nIdx][nCntInfo]);
		}
		return;
	}

	// �t�@�C�����ۑ�
	m_sTextFile.push_back(file);


	// �t�@�C�����J��
	FILE* pFile = fopen(file.c_str(), "r");
	if (pFile == nullptr)
	{//�t�@�C�����J���Ȃ������ꍇ
		return;
	}

	// �ǂݍ��݂̃f�[�^�����Z
	m_nNumLoadData.push_back(0);


	// �ǂݍ��ݗp�ϐ�
	char aComment[MAX_COMMENT];	// �R�����g�p
	int nCntMotion = 0;
	int nCntFile = 0;	// �t�@�C���̓ǂݍ��݉�
	std::vector<std::string> motionFile;	// ���[�V�����t�@�C��

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// ���[�V�������̐ݒ�
		if (strcmp(aComment, "NUM_MOTION") == 0)
		{// NUM_MOTION��������

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�
			fscanf(pFile, "%d", &m_nNumMotion);	// ���[�V�������ǂݍ���

			// ���[�V�����̏�񐶐�
			m_pInfo = DEBUG_NEW Info[m_nNumMotion];
			if (m_pInfo == nullptr)
			{
				fclose(pFile);
				return;
			}
			memset(m_pInfo, 0, sizeof(Info) * m_nNumMotion);

			while (nCntFile != m_nNumMotion)
			{// ���[�V���������ǂݍ��ނ܂ŌJ��Ԃ�

				// ������̓ǂݍ���
				fscanf(pFile, "%s", &aComment[0]);

				// ���[�V�����t�@�C���̓ǂݍ���
				if (strcmp(aComment, "MOTION_FILENAME") == 0)
				{// MOTION_FILENAME��������

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%s", &aComment[0]);	// �t�@�C����

					// �t�@�C�����ۑ�
					motionFile.push_back(&aComment[0]);

					nCntFile++;	// �t�@�C���̓ǂݍ��݉񐔉��Z
				}
			}

			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);

	// �v�f���J��Ԃ�
	for (int i = 0; i < static_cast<int>(motionFile.size()); i++)
	{
		// ���[�V�����ǂݍ���
		LoadMotion(motionFile[i], i);
	}

	// �ǂݍ��񂾐����Z
	m_nNumLoad++;
}

//==========================================================================
// ���[�V�����ǂݍ���
//==========================================================================
void CMotion::LoadMotion(const std::string& file, int nIdxMotion)
{

	// �t�@�C�����J��
	FILE* pFile = fopen(file.c_str(), "r");
	if (pFile == nullptr)
	{//�t�@�C�����J���Ȃ������ꍇ
		return;
	}

	// �ǂݍ��ݗp�ϐ�
	char aComment[MAX_COMMENT];
	int nCntKey = 0, nCntParts = 0;
	Info InitInfo = {};
	AttackInfo InitAttackInfo = {};

	// �L�����Z���\�t���[�����Z�b�g
	InitInfo.nCancelableFrame = -1;
	InitInfo.nCombolableFrame = -1;

	// �X�P�[���l���Z�b�g
	for (int key = 0; key < MAX_KEY; key++)
	{
		for (int parts = 0; parts < MAX_PARTS; parts++)
		{
			InitInfo.aKey[key].aParts[parts].scale = mylib_const::DEFAULT_SCALE;
		}
	}

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		//==================================================
		// �e���[�V�����̐ݒ�
		//==================================================
		if (strcmp(aComment, "MOTIONSET") == 0)
		{// ���[�V�������̓ǂݍ��݂��J�n

			// ������
			m_aLoadData[m_nNumLoad].push_back(InitInfo);

			while (strcmp(aComment, "END_MOTIONSET") != 0)
			{// END_MOTIONSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	// �m�F����

				if (strcmp(aComment, "LOOP") == 0)
				{// LOOP�������烋�[�vON/OFF�ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad][nIdxMotion].nLoop);	// ���[�v0��1
				}

				if (strcmp(aComment, "NUM_KEY") == 0)
				{// NUM_KEY��������L�[���ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad][nIdxMotion].nNumKey);	// �L�[��
				}

				if (strcmp(aComment, "MOVE") == 0)
				{// MOVE��������ړ�ON/OFF�ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad][nIdxMotion].nMove);	// �ړ�0��1��
				}

				if (strcmp(aComment, "CANCELABLE") == 0)
				{// CANCELABLE�ŃL�����Z���\�t���[���ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad][nIdxMotion].nCancelableFrame);	// �L�����Z���\�t���[��
				}

				if (strcmp(aComment, "COMBOLABLE") == 0)
				{// COMBOLABLE�ŃR���{�\�t���[���ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad][nIdxMotion].nCombolableFrame);	// �R���{�\�t���[��
				}

				if (strcmp(aComment, "ATTACKINFO") == 0)
				{// ATTACKINFO��������

					// ������
					m_aLoadAttackData[m_nNumLoad][nIdxMotion].push_back(InitAttackInfo);

					int nNowAttackNum = m_aLoadData[m_nNumLoad][nIdxMotion].nNumAttackInfo;

					while (strcmp(aComment, "END_ATTACKINFO") != 0)
					{// END_ATTACKINFO������܂ŌJ��Ԃ�

						fscanf(pFile, "%s", &aComment[0]);	// �m�F����

						if (strcmp(aComment, "COLLISIONPARTS") == 0)
						{// COLLISIONPARTS��������p�[�c�ԍ��ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%d", &m_aLoadAttackData[m_nNumLoad][nIdxMotion][nNowAttackNum].nCollisionNum);	// ��������p�[�c�ԍ�
						}

						if (strcmp(aComment, "ATTACKRANGE") == 0)
						{// ATTACKRANGE��������U���T�C�Y�ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%f", &m_aLoadAttackData[m_nNumLoad][nIdxMotion][nNowAttackNum].fRangeSize);	// ����̃T�C�Y
						}

						if (strcmp(aComment, "OFFSET") == 0)
						{// OFFSET��������I�t�Z�b�g�ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%f", &m_aLoadAttackData[m_nNumLoad][nIdxMotion][nNowAttackNum].Offset.x);	// X���W
							fscanf(pFile, "%f", &m_aLoadAttackData[m_nNumLoad][nIdxMotion][nNowAttackNum].Offset.y);	// Y���W
							fscanf(pFile, "%f", &m_aLoadAttackData[m_nNumLoad][nIdxMotion][nNowAttackNum].Offset.z);	// Z���W
						}

						if (strcmp(aComment, "ATTACKCOUNT") == 0)
						{// ATTACKCOUNT��������U���J�E���g�ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%d", &m_aLoadAttackData[m_nNumLoad][nIdxMotion][nNowAttackNum].nMinCnt);	// ����̍ŏ��J�E���g
							fscanf(pFile, "%d", &m_aLoadAttackData[m_nNumLoad][nIdxMotion][nNowAttackNum].nMaxCnt);	// ����̍ő�J�E���g
						}

						if (strcmp(aComment, "INPACTCOUNT") == 0)
						{// INPACTCOUNT��������Ռ��J�E���g�ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%d", &m_aLoadAttackData[m_nNumLoad][nIdxMotion][nNowAttackNum].nInpactCnt);	// �Ռ��̃J�E���g
						}

						if (strcmp(aComment, "DAMAGE") == 0)
						{// DAMAGE��������U���͓ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%d", &m_aLoadAttackData[m_nNumLoad][nIdxMotion][nNowAttackNum].nDamage);	// �U����
						}

						if (strcmp(aComment, "ONLYONETIME") == 0)
						{// ONLYONETIME����������݂̂��ǂݍ���
							int nFrag = 0;
							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%d", &nFrag);	// 1�x�������̔���

							if (nFrag == 0) { m_aLoadAttackData[m_nNumLoad][nIdxMotion][nNowAttackNum].bOnlyOneTime = false; }
							else { m_aLoadAttackData[m_nNumLoad][nIdxMotion][nNowAttackNum].bOnlyOneTime = true; }
						}

						if (strcmp(aComment, "ATTACKTYPE") == 0)
						{// ATTACKTYPE�ōU���̎�ޓǂݍ���
							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%d", &m_aLoadAttackData[m_nNumLoad][nIdxMotion][nNowAttackNum].AtkType);	// �U���̎��
						}
					}

					// �U�����̓o�^
					SetAttackInfo(m_aLoadAttackData[m_nNumLoad][nIdxMotion][nNowAttackNum]);

					// �U�����̐����Z
					m_aLoadData[m_nNumLoad][nIdxMotion].nNumAttackInfo++;
				}

				//==================================================
				//�e�L�[�̐ݒ�
				//==================================================
				if (strcmp(aComment, "KEYSET") == 0)
				{// KEYSET�ŃL�[���̓ǂݍ��݊J�n

					while (strcmp(aComment, "END_KEYSET") != 0)
					{// END_KEYSET������܂ŌJ��Ԃ�

						fscanf(pFile, "%s", &aComment[0]);	// �m�F����

						if (strcmp(aComment, "FRAME") == 0)
						{// FRAME��������Đ��t���[�����ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		// =�̕�
							fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad][nIdxMotion].aKey[nCntKey].nFrame);	// �Đ��t���[��
						}

						//==================================================
						// �p�[�c�̐ݒ�
						//==================================================
						if (strcmp(aComment, "PARTS") == 0)
						{// PARTS�Ńp�[�c���̓ǂݍ��݊J�n

							while (strcmp(aComment, "END_PARTS") != 0)
							{// END_PARTS������܂ŌJ��Ԃ�

								fscanf(pFile, "%s", &aComment[0]);	// �m�F����

								if (strcmp(aComment, "POS") == 0)
								{// POS��������ʒu�ǂݍ���

									fscanf(pFile, "%s", &aComment[0]);		// =�̕�
									fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad][nIdxMotion].aKey[nCntKey].aParts[nCntParts].pos.x);	// X���W
									fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad][nIdxMotion].aKey[nCntKey].aParts[nCntParts].pos.y);	// Y���W
									fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad][nIdxMotion].aKey[nCntKey].aParts[nCntParts].pos.z);	// Z���W
								}

								if (strcmp(aComment, "ROT") == 0)
								{// ROT��������p�x�ǂݍ���

									fscanf(pFile, "%s", &aComment[0]);		// =�̕�
									fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad][nIdxMotion].aKey[nCntKey].aParts[nCntParts].rot.x);	// X�p�x
									fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad][nIdxMotion].aKey[nCntKey].aParts[nCntParts].rot.y);	// Y�p�x
									fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad][nIdxMotion].aKey[nCntKey].aParts[nCntParts].rot.z);	// Z�p�x
								}

								if (strcmp(aComment, "SCALE") == 0)
								{// SCALE��������X�P�[���ǂݍ���

									fscanf(pFile, "%s", &aComment[0]);		// =�̕�
									fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad][nIdxMotion].aKey[nCntKey].aParts[nCntParts].scale.x);	// X�X�P�[��
									fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad][nIdxMotion].aKey[nCntKey].aParts[nCntParts].scale.y);	// Y�X�P�[��
									fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad][nIdxMotion].aKey[nCntKey].aParts[nCntParts].scale.z);	// Z�X�P�[��
								}
							}// END_PARTS�̂�����

							nCntParts++;	//�p�[�c�̃J�E���g���Z
						}
					}// END_KEYSET�̂�����

					nCntParts = 0;	// �p�[�c�̃J�E���g���Z�b�g
					nCntKey++;	// �L�[�̃J�E���g���Z
				}

			}// END_MOTIONSET�̂�����

			// ���[�V�������̓o�^
			SetInfo(m_aLoadData[m_nNumLoad][nIdxMotion]);

			if (nIdxMotion == 0)
			{
				m_fMaxAllFrame = 0;
				for (int nCntKey = 0; nCntKey < m_pInfo[m_nPatternKey].nNumKey; nCntKey++)
				{
					m_fMaxAllFrame += m_pInfo[m_nPatternKey].aKey[nCntKey].nFrame;	// �S�ẴJ�E���g�̍ő�l
				}
			}

			nCntKey = 0;	// �L�[�̃J�E���g���Z�b�g
			nIdxMotion++;	// ���[�V�����̃J�E���g���Z

			// ���[�V�������̃f�[�^��
			m_nNumLoadData[m_nNumLoad]++;
		}

		if (strcmp(aComment, "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���

			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);

}
