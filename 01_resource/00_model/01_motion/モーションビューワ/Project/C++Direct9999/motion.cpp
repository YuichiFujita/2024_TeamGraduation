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
#include "particle.h"

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
	memset(&m_aInfo[0], NULL, sizeof(m_aInfo));
	memset(&m_aInfo[0].AttackInfo[0], 0, sizeof(AttackInfo) * MAX_PARTS);

	memset(&aPartsOld[0], NULL, sizeof(aPartsOld));
	m_nNumAll = 0;			// ���[�V�����̑���
	m_nType = 0;			// ���݂̃��[�V�����̎��
	m_nOldType = 0;			// �O��̃��[�V�����̎��
	m_bLoop = false;		// ���[�v���邩�ǂ���
	m_nPatternKey = 0;		// ���ڂ̃L�[��
	m_nCntFrame = 0;		// �t���[���̃J�E���g
	m_nCntAllFrame = 0;		// �S�ẴJ�E���g
	m_bFinish = false;		// �I���������ǂ���
	m_bCancelable = false;	// �L�����Z���\��
	m_pObjChara = NULL;		// �I�u�W�F�N�g�̃|�C���^
	m_ppModel = NULL;		// ���f���̃|�C���^
	m_nNumModel = 0;		// ���f���̑���
	m_nNumMotion = 0;		// ���[�V�����̑���

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
CMotion *CMotion::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CMotion *pMotion = NULL;

	if (pMotion == NULL)
	{// NULL��������

		// �������̊m��
		pMotion = DEBUG_NEW CMotion;

		if (pMotion != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pMotion->Init();
		}

		return pMotion;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMotion::Init(void)
{
	//for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	//{
	//	m_aInfo[nCntMotion] = {};	// ���[�V�����̏��
	//}
	m_nNumAll = 0;				// ���[�V�����̑���
	m_nType = 0;				// ���݂̃��[�V�����̎��
	m_bLoop = false;			// ���[�v���邩�ǂ���
	m_nPatternKey = 0;			// ���ڂ̃L�[��
	m_nCntFrame = 0;			// �t���[���̃J�E���g
	m_nCntAllFrame = 0;			// �S�ẴJ�E���g
	m_bFinish = false;			// �I���������ǂ���
	m_ppModel = NULL;			// ���f���̃|�C���^
	m_nNumModel = 0;			// ���f���̑���

	for (int i = 0; i < MAX_MOTION; i++)
	{
		for (int key = 0; key < MAX_KEY; key++)
		{
			for (int parts = 0; parts < MAX_PARTS; parts++)
			{
				m_aInfo[i].aKey[key].aParts[parts].scale = mylib_const::DEFAULT_SCALE;
			}
		}
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMotion::Uninit(void)
{
	m_sMotionFile.clear();

	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{
		for (int nCntAttack = 0; nCntAttack < MAX_PARTS; nCntAttack++)
		{
			if (m_aInfo[nCntMotion].AttackInfo[nCntAttack] != NULL)
			{
				delete m_aInfo[nCntMotion].AttackInfo[nCntAttack];
				m_aInfo[nCntMotion].AttackInfo[nCntAttack] = NULL;
			}
		}
	}
}

//==========================================================================
// ���[�V�������̓o�^
//==========================================================================
void CMotion::SetInfo(Info info)
{
	// ����n��
	//m_aInfo[m_nNumAll] = info;\
	// �R�s�[
	memcpy(m_aInfo[m_nNumAll].aKey, info.aKey, sizeof(info.aKey));

	m_aInfo[m_nNumAll].nLoop = info.nLoop;
	m_aInfo[m_nNumAll].nMove = info.nMove;
	m_aInfo[m_nNumAll].nNumKey = info.nNumKey;
	m_aInfo[m_nNumAll].nMaxAllFrame = info.nMaxAllFrame;
	m_aInfo[m_nNumAll].nCancelableFrame = info.nCancelableFrame;
	m_aInfo[m_nNumAll].nCombolableFrame = info.nCombolableFrame;
	m_aInfo[m_nNumAll].filename = info.filename;

	for (int nCntKey = 0; nCntKey < m_aInfo[m_nNumAll].nNumKey; nCntKey++)
	{
		int nBeforeCnt = nCntKey - 1;
		if (nBeforeCnt <= 0)
		{
			nBeforeCnt = 0;
		}

		float PosX = m_aInfo[m_nNumAll].aKey[nBeforeCnt].aParts[0].pos.x;
		float PosZ = m_aInfo[m_nNumAll].aKey[nBeforeCnt].aParts[0].pos.z;

		// ���������߂�
		m_aInfo[m_nNumAll].aKey[nCntKey].fRotMove = atan2f(
			(PosX - m_aInfo[m_nNumAll].aKey[nCntKey].aParts[0].pos.x),
			(PosZ - m_aInfo[m_nNumAll].aKey[nCntKey].aParts[0].pos.z));
		int n = 0;
	}

	// ���[�V�����̑������Z
	m_nNumAll++;
}

//==========================================================================
// �U�����̓o�^
//==========================================================================
void CMotion::SetAttackInfo(AttackInfo info)
{
	// �������m��
	m_aInfo[m_nNumAll].AttackInfo[m_aInfo[m_nNumAll].nNumAttackInfo] = DEBUG_NEW AttackInfo;

	if (m_aInfo[m_nNumAll].AttackInfo[m_aInfo[m_nNumAll].nNumAttackInfo] != NULL)
	{// NULL����Ȃ����

		// ���n��
		*m_aInfo[m_nNumAll].AttackInfo[m_aInfo[m_nNumAll].nNumAttackInfo] = info;

		// �U�����̑������Z
		m_aInfo[m_nNumAll].nNumAttackInfo++;
	}
}

//==========================================================================
// ���[�V���������郂�f���̓o�^
//==========================================================================
void CMotion::SetModel(CModel **pModel, int nNumModel, CObjectChara *pObjChara)
{
	// ���f���̃|�C���^��n��
	m_ppModel = pModel;

	// ���f���̑���
	m_nNumModel = nNumModel;

	// �I�u�W�F�N�g�̃|�C���^��n��
	m_pObjChara = pObjChara;
}

//==========================================================================
// �X�V����
//==========================================================================
void CMotion::Update(void)
{
	// �Đ��t���[��
	int nFrame;

	// �Đ��t���[���ۑ�
	nFrame = m_aInfo[m_nType].aKey[m_nPatternKey].nFrame;

	if (m_nType == 0 && m_nOldType != 0 && m_nPatternKey == 0)
	{// �j���[�g������0�̎�
		nFrame = 10;
	}

	// ���̃L�[
	int nNextKey = (m_nPatternKey + 1) % m_aInfo[m_nType].nNumKey;

	if (nNextKey == 0 && m_aInfo[m_nType].nLoop == LOOP_OFF)
	{// ���[�v���Ȃ��Ƃ�

		// �Ō�ŌŒ�
		nNextKey = m_aInfo[m_nType].nNumKey - 1;
	}

	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{// �S�p�[�c���J��Ԃ�

		if (m_ppModel[nCntParts] == NULL)
		{// NULL��������

			continue;
		}

		// ���ƍ��̌����̍����擾
		float rotDiffX = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].rot.x -
			aPartsOld[nCntParts].rot.x;

		float rotDiffY = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].rot.y -
			aPartsOld[nCntParts].rot.y;

		float rotDiffZ = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].rot.z -
			aPartsOld[nCntParts].rot.z;

		// �p�x�̐��K��
		RotNormalize(rotDiffX);
		RotNormalize(rotDiffY);
		RotNormalize(rotDiffZ);

		// �p�[�c�̌�����ݒ�
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �p�[�c�̌�����ݒ�
		rot.x =
			aPartsOld[nCntParts].rot.x +
			rotDiffX *
			(
			(float)m_nCntFrame /
			(float)nFrame
			);

		// �p�[�c�̌�����ݒ�
		rot.y =
			aPartsOld[nCntParts].rot.y +
			rotDiffY *
			(
			(float)m_nCntFrame /
			(float)nFrame
			);

		// �p�[�c�̌�����ݒ�
		rot.z =
			aPartsOld[nCntParts].rot.z +
			rotDiffZ *
			(
			(float)m_nCntFrame /
			(float)nFrame
			);

		// �p�x�̐��K��
		RotNormalize(rot.x);
		RotNormalize(rot.y);
		RotNormalize(rot.z);

		// �����ݒ�
		m_ppModel[nCntParts]->SetRotation(rot);



		{
			// ���ƍ��̌����̍����擾
			float scaleDiffX = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].scale.x -
				aPartsOld[nCntParts].scale.x;

			float scaleDiffY = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].scale.y -
				aPartsOld[nCntParts].scale.y;

			float scaleDiffZ = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].scale.z -
				aPartsOld[nCntParts].scale.z;

			// �p�[�c�̌�����ݒ�
			D3DXVECTOR3 scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// �p�[�c�̌�����ݒ�
			scale.x =
				aPartsOld[nCntParts].scale.x +
				scaleDiffX *
				(
				(float)m_nCntFrame /
				(float)nFrame
				);

			// �p�[�c�̌�����ݒ�
			scale.y =
				aPartsOld[nCntParts].scale.y +
				scaleDiffY *
				(
				(float)m_nCntFrame /
				(float)nFrame
				);

			// �p�[�c�̌�����ݒ�
			scale.z =
				aPartsOld[nCntParts].scale.z +
				scaleDiffZ *
				(
				(float)m_nCntFrame /
				(float)nFrame
				);

			// �����ݒ�
			m_ppModel[nCntParts]->SetScale(scale);
		}


#if 1
		// �p�[�c�̈ʒu��ݒ�
		if (nCntParts == 0)
		{
			// �{�̂̈ʒu�擾
			D3DXVECTOR3 pos = m_pObjChara->GetPosition();

			// �{�̂̌����擾
			D3DXVECTOR3 rot = m_pObjChara->GetRotation();

			// ���̈ʒu�擾
			D3DXVECTOR3 posOrigin = m_pObjChara->GetOriginPosition();

			// �p�[�c�̈ʒu�擾
			D3DXVECTOR3 posParts = m_ppModel[nCntParts]->GetPosition();
			D3DXVECTOR3 posPartsOld = m_ppModel[nCntParts]->GetPosition();

			// �ڕW�̈ʒu�Ƃ̍��������߂�
			float posDiffX = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.x -
				aPartsOld[nCntParts].pos.x;

			float posDiffY = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.y -
				aPartsOld[nCntParts].pos.y;

			float posDiffZ = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.z -
				aPartsOld[nCntParts].pos.z;

			// �e��Y��␳
			posParts.y =
				aPartsOld[nCntParts].pos.y +
				posDiffY *
				(
				(float)m_nCntFrame /
					(float)nFrame
					);

			// �ʒu�ݒ�
			m_ppModel[nCntParts]->SetPosition(posParts + posOrigin);

			int nNextMoveKey = m_nPatternKey + 1;
			if (nNextMoveKey >= m_aInfo[m_nType].nNumKey)
			{
				nNextMoveKey = m_nPatternKey;
			}

			// ���������������߂�
			float fMoveDiff =
				sqrtf((m_aInfo[m_nType].aKey[nNextMoveKey].aParts[nCntParts].pos.x - aPartsOld[nCntParts].pos.x) * (m_aInfo[m_nType].aKey[nNextMoveKey].aParts[nCntParts].pos.x - aPartsOld[nCntParts].pos.x)
					+ (m_aInfo[m_nType].aKey[nNextMoveKey].aParts[nCntParts].pos.z - aPartsOld[nCntParts].pos.z) * (m_aInfo[m_nType].aKey[nNextMoveKey].aParts[nCntParts].pos.z - aPartsOld[nCntParts].pos.z));
			fMoveDiff /= (float)nFrame;

			// �����̌������ꎞ���
			float fRot = m_aInfo[m_nType].aKey[nNextMoveKey].fRotMove;

			// �����̌��������ֈړ�
			pos.x += sinf(D3DX_PI + fRot + rot.y) * fMoveDiff;
			pos.z += cosf(D3DX_PI + fRot + rot.y) * fMoveDiff;

			m_pObjChara->SetPosition(pos);
		}
		else
		{
			// �p�[�c�̈ʒu�擾
			D3DXVECTOR3 posParts = m_ppModel[nCntParts]->GetPosition();

			// ���̈ʒu�擾
			D3DXVECTOR3 posOrigin = m_pObjChara->GetOriginPosition();

			// �ڕW�̈ʒu�Ƃ̍��������߂�
			float posDiffX = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.x -
				aPartsOld[nCntParts].pos.x;

			float posDiffY = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.y -
				aPartsOld[nCntParts].pos.y;

			float posDiffZ = m_aInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.z -
				aPartsOld[nCntParts].pos.z;

			// �p�[�c�̈ʒu��ݒ�
			posParts.y =
				aPartsOld[nCntParts].pos.y +
				posDiffY *
				((float)m_nCntFrame /
				(float)nFrame
					);

			posParts.x =
				aPartsOld[nCntParts].pos.x +
				posDiffX *
				((float)m_nCntFrame /
				(float)nFrame
					);

			posParts.z =
				aPartsOld[nCntParts].pos.z +
				posDiffZ *
				((float)m_nCntFrame /
				(float)nFrame
					);

			// �ʒu�ݒ�
			m_ppModel[nCntParts]->SetPosition(posParts + m_ppModel[nCntParts]->GetOriginPosition());
		}
	}
#endif

	// �t���[���̃J�E���g�����Z
	m_nCntFrame++;
	m_nCntAllFrame++;

	if (m_aInfo[m_nType].nCancelableFrame != -1 &&
		m_aInfo[m_nType].nCancelableFrame <= m_nCntAllFrame)
	{// -1����Ȃ��A�t���[���𒴂�����
		m_bCancelable = true;	// �L�����Z���\

		// �p�[�e�B�N������
		CManager::GetParticle()->Create(m_pObjChara->GetPosition(), CParticle::TYPE_CANCELABLE);
	}

	if (m_aInfo[m_nType].nLoop == LOOP_ON)
	{// ���[�v���[�V�����͂��ł��I����Ă�

		// �I������ON
		m_bFinish = true;
	}

	if (m_nCntFrame >= nFrame)
	{// �t���[���̃J�E���g���t���[�����ɒB������

		// �t���[���̃J�E���g���[���ɖ߂�
		m_nCntFrame = 0;

		m_nOldType = m_nType;	// �O��̃��[�V�����̎��

		// �p�^�[��NO.�X�V
		m_nPatternKey = (m_nPatternKey + 1) % m_aInfo[m_nType].nNumKey;

		for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
		{// �S�p�[�c���J��Ԃ�
			aPartsOld[nCntParts].rot = m_aInfo[m_nType].aKey[m_nPatternKey].aParts[nCntParts].rot;
			aPartsOld[nCntParts].pos = m_aInfo[m_nType].aKey[m_nPatternKey].aParts[nCntParts].pos;
			aPartsOld[nCntParts].scale = m_aInfo[m_nType].aKey[m_nPatternKey].aParts[nCntParts].scale;
		}

		if (m_nPatternKey == 0)
		{// �L�[���[���̎�

			// ���t���[�������Z�b�g
			m_nCntAllFrame = 0;

			//���[�V�����̐ݒ�
			if (m_aInfo[m_nType].nLoop == LOOP_OFF)
			{// ���[�v���[�V��������Ȃ���

				// �I������ON
				m_bFinish = true;
			}
			else
			{// ���[�v���[�V�����̎�

				// ���݂Ɠ������[�V������ݒ�
				Set(m_nType);
			}
		}
	}
}

//==========================================================================
// ���[�V�����̐ݒ菈��
//==========================================================================
void CMotion::Set(int nType)
{
	if (nType > m_nNumAll)
	{// �����𒴂��Ă�����
		return;
	}

	if (m_nType != nType)
	{// ���݂̎�ނƓ�������Ȃ�������

		m_nOldType = m_nType;	// �O��̃��[�V�����̎��
		m_nType = nType;		// ��ސݒ�
		m_nPatternKey = 0;		// ���ڂ̃L�[��
		m_nCntFrame = 0;		// �t���[���̃J�E���g
		m_nCntAllFrame = 0;		// �S�ẴJ�E���g
		m_bFinish = false;		// �I���������ǂ���
		m_bCancelable = false;	// �L�����Z���\�ɂȂ�����

		for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
		{// �S�p�[�c���J��Ԃ�

			if (m_ppModel[nCntParts] == NULL)
			{// NULL��������
				continue;
			}

			// �ߋ��̈ʒu�E������ۑ�
			aPartsOld[nCntParts].rot = m_ppModel[nCntParts]->GetRotation();
			//aPartsOld[nCntParts].pos = m_ppModel[nCntParts]->GetPosition();
			aPartsOld[nCntParts].scale = m_ppModel[nCntParts]->GetScale();

			if (nCntParts == 0)
			{
				aPartsOld[nCntParts].pos = m_ppModel[nCntParts]->GetPosition() - m_pObjChara->GetOriginPosition();
			}
			else
			{
				aPartsOld[nCntParts].pos = m_ppModel[nCntParts]->GetPosition() - m_ppModel[nCntParts]->GetOriginPosition();
			}
		}
	}
}

//==========================================================================
// ���݂̃��[�V�����^�C�v�擾
//==========================================================================
int CMotion::GetType(void)
{
	return m_nType;
}

//==========================================================================
// �I�����Ă��邩�̔���
//==========================================================================
bool CMotion::IsFinish(void)
{
	return m_bFinish;
}

//==========================================================================
// �L�����Z���̔���擾
//==========================================================================
bool CMotion::IsGetCancelable(void)
{
	return m_bCancelable;
}

//==========================================================================
// �ړ��̔���擾
//==========================================================================
int CMotion::IsGetMove(int nType)
{
	return m_aInfo[nType].nMove;
}

//==========================================================================
// ���[�V�������̎擾
//==========================================================================
CMotion::Info CMotion::GetInfo(int nType)
{
	return m_aInfo[nType];
}

//==========================================================================
// ���[�V�������̓o�^
//==========================================================================
void CMotion::SetInfoSave(int nType, int nKey, int nParts, D3DXVECTOR3 rot)
{
	m_aInfo[nType].aKey[nKey].aParts[nParts].rot = rot;
}

//==========================================================================
// ���[�V�������̓o�^
//==========================================================================
void CMotion::SetInfoSavePos(int nType, int nKey, int nParts, D3DXVECTOR3 pos)
{
	m_aInfo[nType].aKey[nKey].aParts[nParts].pos = pos;
}

//==========================================================================
// ���[�V�������̓o�^
//==========================================================================
void CMotion::SetInfoSaveScale(int nType, int nKey, int nParts, D3DXVECTOR3 scale)
{
	m_aInfo[nType].aKey[nKey].aParts[nParts].scale = scale;
}

//==========================================================================
// �ړ����̃��Z�b�g
//==========================================================================
void CMotion::ResetMoveData(int nType, int nKey)
{
	int nBeforeCnt = nKey - 1;
	if (nBeforeCnt <= 0)
	{
		nBeforeCnt = 0;
	}

	float PosX = m_aInfo[nType].aKey[nBeforeCnt].aParts[0].pos.x;
	float PosZ = m_aInfo[nType].aKey[nBeforeCnt].aParts[0].pos.z;

	// ���������߂�
	m_aInfo[nType].aKey[nKey].fRotMove = atan2f(
		(PosX - m_aInfo[nType].aKey[nKey].aParts[0].pos.x),
		(PosZ - m_aInfo[nType].aKey[nKey].aParts[0].pos.z));
}

//==========================================================================
// �L�[�����ύX
//==========================================================================
void CMotion::ChangeKeyNum(int nType, int nNum)
{
	m_aInfo[nType].nNumKey = nNum;
}

//==========================================================================
// �L�[�t���[���ύX
//==========================================================================
void CMotion::ChangeKeyFrame(int nType, int nKey, int nNum)
{
	m_aInfo[nType].aKey[nKey].nFrame = nNum;

	m_aInfo[nType].nMaxAllFrame = 0;
	for (int nCntKey = 0; nCntKey < m_aInfo[nType].nNumKey; nCntKey++)
	{
		m_aInfo[nType].nMaxAllFrame += m_aInfo[nType].aKey[nCntKey].nFrame;	// �S�ẴJ�E���g�̍ő�l
	}
}

//==========================================================================
// �L�����Z���\�t���[���ύX
//==========================================================================
void CMotion::ChangeCancelableFrame(int nType, int nFrame)
{
	m_aInfo[nType].nCancelableFrame = nFrame;
}

//==========================================================================
// �R���{�\�t���[���ύX
//==========================================================================
void CMotion::ChangeCombolableFrame(int nType, int nFrame)
{
	m_aInfo[nType].nCombolableFrame = nFrame;
}

//==========================================================================
// �U�����ύX
//==========================================================================
void CMotion::ChangeAttackInfo(int nType, int nIdx, AttackInfo info)
{
	*m_aInfo[nType].AttackInfo[nIdx] = info;
}

//==========================================================================
// �U�����̑������Z
//==========================================================================
void CMotion::AddNumAttackInfo(int nType)
{
	if (m_aInfo[nType].nNumAttackInfo >= MAX_PARTS)
	{
		return;
	}

	// �������m��
	m_aInfo[nType].AttackInfo[m_aInfo[nType].nNumAttackInfo] = new AttackInfo;

	if (m_aInfo[nType].AttackInfo[m_aInfo[nType].nNumAttackInfo] != NULL)
	{// NULL����Ȃ����

		// ������
		memset(m_aInfo[nType].AttackInfo[m_aInfo[nType].nNumAttackInfo], 0, sizeof(AttackInfo));

		// �U�����̑������Z
		m_aInfo[nType].nNumAttackInfo++;
	}
}

//==========================================================================
// �U�����̑������Z
//==========================================================================
void CMotion::SubNumAttackInfo(int nType)
{
	if (m_aInfo[nType].nNumAttackInfo <= 1)
	{
		return;
	}
	int nIdx = m_aInfo[nType].nNumAttackInfo - 1;

	// ���������
	//if (m_aInfo[nType].AttackInfo[nIdx] != NULL)
	{
		delete m_aInfo[nType].AttackInfo[nIdx];
		m_aInfo[nType].AttackInfo[nIdx] = NULL;

		// �U�����̑������Z
		m_aInfo[nType].nNumAttackInfo--;
	}
}

//==========================================================================
// �J�E���g�擾
//==========================================================================
int CMotion::GetAllCount(void)
{
	return m_nCntAllFrame;
}

//==========================================================================
// �O���t�@�C���ǂݍ���
//==========================================================================
void CMotion::ReadText(const char *pTextFile)
{
	// �ǂݍ��݊m�F
	for (int nCntData = 0; nCntData < m_nNumLoad; nCntData++)
	{
		if (m_sTextFile[nCntData] != pTextFile)
		{// �t�@�C��������v���ĂȂ�
			continue;
		}

		for (int nCntInfo = 0; nCntInfo < m_nNumLoadData[nCntData]; nCntInfo++)
		{
			for (int nCntATK = 0; nCntATK < m_aLoadData[nCntData][nCntInfo].nNumAttackInfo; nCntATK++)
			{// �U����񕪌J��Ԃ�

				// �U�����o�^
				SetAttackInfo(m_aLoadAttackData[nCntData][nCntInfo][nCntATK]);
			}

			// ���o�^
			SetInfo(m_aLoadData[nCntData][nCntInfo]);
		}
		return;
	}

	// �t�@�C�����ۑ�
	m_sTextFile.push_back(pTextFile);


	// �t�@�C�����J��
	FILE* pFile = fopen(pTextFile, "r");
	if (pFile == NULL)
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


					m_sMotionFile.emplace_back();
					m_sMotionFile[nCntFile] = &aComment[0];

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
void CMotion::LoadMotion(const std::string text, int nIdxMotion)
{

	// �t�@�C�����J��
	FILE* pFile = fopen(text.c_str(), "r");
	if (pFile == NULL)
	{//�t�@�C�����J���Ȃ������ꍇ
		return;
	}

	// �ǂݍ��ݗp�ϐ�
	char aComment[MAX_COMMENT];
	int nCntKey = 0, nCntParts = 0;
	Info InitInfo = {};
	AttackInfo InitAttackInfo = {};

	// �t�@�C�����ۑ�
	InitInfo.filename = text;

	// �L�����Z���\�t���[�����Z�b�g
	InitInfo.nCancelableFrame = 0;

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

			for (int nCntKey = 0; nCntKey < m_aLoadData[m_nNumLoad][nIdxMotion].nNumKey; nCntKey++)
			{
				m_aLoadData[m_nNumLoad][nIdxMotion].nMaxAllFrame += m_aLoadData[m_nNumLoad][nIdxMotion].aKey[nCntKey].nFrame;	// �S�ẴJ�E���g�̍ő�l
			}

			// ���[�V�������̓o�^
			SetInfo(m_aLoadData[m_nNumLoad][nIdxMotion]);

			if (m_nNumMotion == 0)
			{
				m_nCntAllFrame = 0;
				for (int nCntKey = 0; nCntKey < m_aInfo[m_nPatternKey].nNumKey; nCntKey++)
				{
					m_nCntAllFrame += m_aInfo[m_nPatternKey].aKey[nCntKey].nFrame;	// �S�ẴJ�E���g�̍ő�l
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
