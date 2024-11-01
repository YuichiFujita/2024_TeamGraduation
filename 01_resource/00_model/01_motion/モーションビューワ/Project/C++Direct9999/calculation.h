//=============================================================================
// 
// �֗��֐� [calculation.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _CALCULATION_H_
#define _CALCULATION_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define MAX_COMMENT	(512)	// �R�����g�̍ő吔
#define MOVE_LOG	(64)	// ���S�̍ő吔

// UI�g�k�̃��[�h
typedef enum
{
	MOVELOG_TYPE_ADD = 0,	// �g��
	MOVELOG_TYPE_REMOVE,	// �k��
	MOVELOG_TYPE_SAME,		// ������
	MOVELOG_TYPE_MAX
}MOVELOG_TYPE;

// ���S�g�k�̍\����
typedef struct
{
	int nCnt;		// ���S�g�k�̃J�E���g
	int nPattern;	// ���S�g�k�̃p�^�[��
	float Length;	// ���S�g�k�̒���
	bool bUse;		// �g�p���Ă��邩
}MOVELOG;

//==========================================================================
// �v���g�^�C�v�錾
//==========================================================================
void InitCalculastion(void);
void RotNormalize(float &fRot);												// �p�x�̐��K��(-3.14 || 3.14��������␳)
template<class T> void ValueNormalize(T &Value, T MaxValue, T MinValue);	// �l�̐��K������
void InertiaCorrection(float &fMain, float fTarget, float fCntCorrection);	// �����␳
int Random(int nMinNum, int nMaxNum);										// �����_������
bool CircleRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2);						// 1��2�̓����蔻��(�~)
bool SphereRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2);						// 1��2�̓����蔻��(��)
bool CylinderRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2, float fHeight1, float fHeight2);	// 1��2�̓����蔻��(��)
void CheckVtx(float RotY, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin, LPD3DXMESH pMesh, BYTE *pVtxBuff);	// �S���_�`�F�b�N
void CollisionCharacter(D3DXVECTOR3 &MainPos, D3DXVECTOR3 &MainPosOld, D3DXVECTOR3 MainVtxMax, D3DXVECTOR3 MainVtxMin, D3DXVECTOR3 &TargetPos, D3DXVECTOR3 TargetVtxMax, D3DXVECTOR3 TargetVtxMin);		// �Ԃ�������␳
bool bHitCharacter(D3DXVECTOR3 &MainPos, D3DXVECTOR3 &MainPosOld, D3DXVECTOR3 MainVtxMax, D3DXVECTOR3 MainVtxMin, D3DXVECTOR3 &TargetPos, D3DXVECTOR3 TargetVtxMax, D3DXVECTOR3 TargetVtxMin);			// �Ԃ��������`�F�b�N
void CollisionLimitLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 &pPos, D3DXVECTOR3 posOld);								// �����̓����蔻��
bool CollisionTriangle(D3DXVECTOR3 posCenter, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld);	// �O�p�̓����蔻��
bool CollisionFan(D3DXVECTOR3 posChar, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld);	// ��̓����蔻��
bool CollisionLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld);							// �����̉E�ɂ��邩�ǂ���
int SetMoveLog(void);																											// ���S�g�k���鎞�̃C���f�b�N�X�擾
float MoveLog(int nIdxMoveLog, float pos, int nCntElapsed);																		// pVtx[?].pos�̒����ɂȂ���
void ResetMoveLog(int nIdxMoveLog);																								// �������g�k�������Ƃ�
template<class T> void CuadricCurveComp(T& Value, const int nRange, T RangeOrigin, T Max, int& nAlphaCnt);	// �񎟋Ȑ��␳

std::string extractFileName(const std::string& filePath);	// �p�X�̍Ō�̕��������擾


//==========================================================================
// �l�̐��K������
//==========================================================================
template<class T> void ValueNormalize(T &Value, T MaxValue, T MinValue)
{
	if (Value >= MaxValue)
	{// �ő�l�𒴂�����

		// �ő�l�Œ�
		Value = MaxValue;
	}
	else if (Value <= MinValue)
	{// �ŏ��l�����������

		// �ŏ��l�Œ�
		Value = MinValue;
	}
}

//==========================================================================
// �񎟋Ȑ��␳
//==========================================================================
template<class T> void CuadricCurveComp(T& Value, const int nRange, T RangeOrigin, T Max, int& nAlphaCnt)
{
	// �⊮�̒l����o��
	T Comp = ((T)Max / (T)(nRange * nRange));

	if (nAlphaCnt > nRange)
	{// �͈͒�������߂�
		nAlphaCnt = -nRange;
	}
	else if (nAlphaCnt < -nRange)
	{// �͈͒�������߂�
		nAlphaCnt = nRange;
	}

	// �s�����̃J�E���g���Z
	nAlphaCnt++;

	// �Ȑ��␳
	Value = (T)((Comp * (nAlphaCnt * nAlphaCnt)) - (2 * (Comp * nAlphaCnt * RangeOrigin)) + (Comp * (RangeOrigin * RangeOrigin)));
}


#endif