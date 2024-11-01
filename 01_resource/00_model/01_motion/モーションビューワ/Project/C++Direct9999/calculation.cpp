//==================================================================================
// 
// �֗��֐�[calculation.cpp]
// Author:���n�Ή�
// 
//==================================================================================
#include "calculation.h"

// �O���[�o���ϐ��錾
MOVELOG g_aMoveLog[MOVE_LOG];		// ���S�g�k�̍\����

//==================================================================================
// ����������
//==================================================================================
void InitCalculastion(void)
{
	// ���S�g�k
	for (int nCntLog = 0; nCntLog < MOVE_LOG; nCntLog++)
	{
		g_aMoveLog[nCntLog].nCnt = 0;
		g_aMoveLog[nCntLog].nPattern = 0;
		g_aMoveLog[nCntLog].Length = 0.0f;
		g_aMoveLog[nCntLog].bUse = false;
	}
}

//==================================================================================
// ���S�̐ݒ�
//==================================================================================
int SetMoveLog(void)
{
	int nIdxMoveLog = -1;

	for (int nCntLog = 0; nCntLog < MOVE_LOG; nCntLog++)
	{
		if (g_aMoveLog[nCntLog].bUse == false)
		{// �g�p����Ă��Ȃ�������

		 // �C���f�b�N�X�ێ����ď�������
			g_aMoveLog[nCntLog].bUse = true;
			nIdxMoveLog = nCntLog;
			break;
		}
	}

	return nIdxMoveLog;
}

//==================================================================================
// ���S�̑��΍��W�Z�o
//==================================================================================
float MoveLog(int nIdxMoveLog, float pos, int nCntElapsed)
{

	// �J�E���g���Z
	g_aMoveLog[nIdxMoveLog].nCnt = (g_aMoveLog[nIdxMoveLog].nCnt + 1) % nCntElapsed;

	if (g_aMoveLog[nIdxMoveLog].nPattern == MOVELOG_TYPE_ADD)
	{// �g��

		if ((g_aMoveLog[nIdxMoveLog].nCnt % nCntElapsed) == 0)
		{
			g_aMoveLog[nIdxMoveLog].nPattern = (g_aMoveLog[nIdxMoveLog].nPattern + 1) % MOVELOG_TYPE_MAX;	// �p�^�[��No.���X�V
		}

		g_aMoveLog[nIdxMoveLog].Length = pos + g_aMoveLog[nIdxMoveLog].nCnt * 6.0f;
	}
	else if (g_aMoveLog[nIdxMoveLog].nPattern == MOVELOG_TYPE_REMOVE)
	{// �k��

		if ((g_aMoveLog[nIdxMoveLog].nCnt % nCntElapsed) == 0)
		{
			g_aMoveLog[nIdxMoveLog].nPattern = (g_aMoveLog[nIdxMoveLog].nPattern + 1) % MOVELOG_TYPE_MAX;	// �p�^�[��No.���X�V
		}

		g_aMoveLog[nIdxMoveLog].Length = pos - g_aMoveLog[nIdxMoveLog].nCnt * 6.0f;
	}
	else if (g_aMoveLog[nIdxMoveLog].nPattern == MOVELOG_TYPE_SAME)
	{// �߂�

		// �g�傷��
		g_aMoveLog[nIdxMoveLog].Length = pos + g_aMoveLog[nIdxMoveLog].nCnt * 6.0f;

		if (g_aMoveLog[nIdxMoveLog].Length >= pos)
		{// �K��l�ȏ�

		 // �K��l�ɖ߂�
			g_aMoveLog[nIdxMoveLog].Length = pos;
		}
	}

	return g_aMoveLog[nIdxMoveLog].Length;
}

//==================================================================================
// ���S�̃��Z�b�g
//==================================================================================
void ResetMoveLog(int nIdxMoveLog)
{
	g_aMoveLog[nIdxMoveLog].nCnt = 0;
	g_aMoveLog[nIdxMoveLog].Length = 0.0f;
	g_aMoveLog[nIdxMoveLog].nPattern = MOVELOG_TYPE_ADD;
}

//==================================================================================
// �p�x�̐��K������
//==================================================================================
void RotNormalize(float &fRot)
{
	// 1�����z������␳����
	if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2.0f;
	}
	else if (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2.0f;
	}
}

//==================================================================================
// �����␳
//==================================================================================
void InertiaCorrection(float &fMain, float fTarget, float fCntCorrection)
{
	fMain += (fTarget - fMain) * fCntCorrection;
}

//==================================================================================
// �����_��
//==================================================================================
int Random(int nMinNum, int nMaxNum)
{
	// �����_����
	int nRand = 0;

	// �͈�
	int nRange = nMaxNum - nMinNum;

	if (nMinNum == 0)
	{// �ŏ��l��0�̎�

	 // �͈͂�1���Z
		nRange += 1;
	}

	// �����_������
	nRand = rand() % nRange + nMinNum;

	return nRand;
}

//==================================================================================
// �~�̓����蔻��
//==================================================================================
bool CircleRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2)
{
	float fLength = (pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.z - pos2.z) * (pos1.z - pos2.z);

	if (fLength <= (fRadius1 * fRadius2) + (fRadius1 * fRadius2))
	{// �~�̒��ɓ�������

		// ������������Ԃ�
		return true;
	}

	return false;
}

//==================================================================================
// ���̓����蔻��
//==================================================================================
bool SphereRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2)
{
	float fLength = (pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.y - pos2.y) * (pos1.y - pos2.y) + (pos1.z - pos2.z) * (pos1.z - pos2.z);

	if (fLength <= (fRadius1 + fRadius2) + (fRadius1 * fRadius2))
	{// �~�̒��ɓ�������

		// ������������Ԃ�
		return true;
	}

	return false;
}

//==================================================================================
// 1��2�̓����蔻��(��)
//==================================================================================
bool CylinderRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2, float fHeight1, float fHeight2)
{
	float fLength = (pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.z - pos2.z) * (pos1.z - pos2.z);

	if (fLength <= (fRadius1 * fRadius2) + (fRadius1 * fRadius2))
	{// �~�̒��ɓ�������

		if (pos1.y - fHeight1 <= pos2.y + fHeight2)
		{// �������Ԃ����Ă�����

			// ������������Ԃ�
			return true;
		}
	}

	return false;
}

//==================================================================================
// �S���_�`�F�b�N
//==================================================================================
void CheckVtx(float RotY, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin, LPD3DXMESH pMesh, BYTE *pVtxBuff)
{
	//���Z�b�g
	*pVtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	*pVtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

	//���f���̒��_�����擾
	int nNumVtx = pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	DWORD dwSizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

	//���_�o�b�t�@�����b�N
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//�S�Ă̒��_���`�F�b�N����
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//����̃f�[�^
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//���_���W�̑��

		//X
		if (pVtxMin->x > vtx.x)
		{//���̍ŏ��l��������̒l����������������

			pVtxMin->x = vtx.x;
		}
		if (pVtxMax->x < vtx.x)
		{//���̍ő�l��������̒l���傫��������

			pVtxMax->x = vtx.x;
		}

		//Y
		if (pVtxMin->y > vtx.y)
		{//���̍ŏ��l��������̒l����������������

			pVtxMin->y = vtx.y;
		}
		if (pVtxMax->y < vtx.y)
		{//���̍ő�l��������̒l���傫��������

			pVtxMax->y = vtx.y;
		}

		//Z
		if (pVtxMin->z > vtx.z)
		{//���̍ŏ��l��������̒l����������������

			pVtxMin->z = vtx.z;
		}
		if (pVtxMax->z < vtx.z)
		{//���̍ő�l��������̒l���傫��������

			pVtxMax->z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;	//�T�C�Y���|�C���^�ړ�
	}

	//���_�o�b�t�@���A�����b�N
	pMesh->UnlockVertexBuffer();

	D3DXVECTOR3 VtxMax = *pVtxMax, VtxMin = *pVtxMin;

	if (RotY <= D3DX_PI * 0.5f + 0.05f && RotY >= D3DX_PI * 0.5f - 0.05f)
	{//1.57f�̂Ƃ�

		pVtxMax->x = VtxMax.z;
		pVtxMin->x = VtxMin.z;

		pVtxMax->z = -VtxMin.x;
		pVtxMin->z = -VtxMax.x;
	}
	else if (RotY <= -D3DX_PI * 0.5f + 0.05f && RotY >= -D3DX_PI * 0.5f - 0.05f)
	{//-1.57f�̂Ƃ�
		
		////////////////////////////////

		pVtxMax->x = -VtxMin.z;
		pVtxMin->x = -VtxMax.z;

		pVtxMax->z = VtxMax.x;
		pVtxMin->z = VtxMin.x;
	}
	else if (RotY <= D3DX_PI + 0.05f && RotY >= D3DX_PI - 0.05f)
	{//D3DX_PI�̂Ƃ�

	 ////////////////////////////////

		pVtxMax->x = -VtxMin.x;
		pVtxMin->x = -VtxMax.x;

		pVtxMax->z = -VtxMin.z;
		pVtxMin->z = -VtxMax.z;
	}
	else if (RotY <= -D3DX_PI + 0.05f && RotY >= -D3DX_PI - 0.05f)
	{//-D3DX_PI�̂Ƃ�

		pVtxMax->x = -VtxMin.x;
		pVtxMin->x = -VtxMax.x;

		pVtxMax->z = -VtxMin.z;
		pVtxMin->z = -VtxMax.z;
	}
}

//==================================================================================
// �L�������m�̓����蔻��(�␳)
//==================================================================================
void CollisionCharacter(D3DXVECTOR3 &MainPos, D3DXVECTOR3 &MainPosOld, D3DXVECTOR3 MainVtxMax, D3DXVECTOR3 MainVtxMin, D3DXVECTOR3 &TargetPos, D3DXVECTOR3 TargetVtxMax, D3DXVECTOR3 TargetVtxMin)
{

	// ���̔���
	if (MainPos.z + MainVtxMax.z > TargetPos.z + TargetVtxMin.z &&	// �G�̍ŏ��l�ɓ����Ă�
		MainPos.z + MainVtxMin.z < TargetPos.z + TargetVtxMax.z)		// �G�̍ő�l�ɓ����Ă�
	{// �G��Z���̒��ɂ���Ƃ�

		if (MainPos.x + MainVtxMin.x <= TargetPos.x + TargetVtxMax.x &&	// ���݂̍ŏ��l���G�̍ő�l����������
			MainPosOld.x + MainVtxMin.x >= TargetPos.x + TargetVtxMax.x)	// �O��̍ŏ��l���G�̍ő�l�����傫��
		{// �E����

			MainPos.x = (TargetPos.x + TargetVtxMax.x) - MainVtxMin.x + 0.1f;
		}
		if (MainPos.x + MainVtxMax.x >= TargetPos.x + TargetVtxMin.x &&	// ���݂̍ő�l���G�̍ŏ��l�����傫��
			MainPosOld.x + MainVtxMax.x <= TargetPos.x + TargetVtxMin.x)	// �O��̍ő�l���G�̍ŏ��l����������
		{// ������

			MainPos.x = (TargetPos.x + TargetVtxMin.x) - MainVtxMax.x - 0.1f;
		}
	}

	// �c�̔���
	if (MainPos.x + MainVtxMax.x > TargetPos.x + TargetVtxMin.x &&	// �G�̍ŏ��l�ɓ����Ă�
		MainPos.x + MainVtxMin.x < TargetPos.x + TargetVtxMax.x)		// �G�̍ő�l�ɓ����Ă�
	{// �G��X���̒��ɂ���Ƃ�

		if (MainPos.z + MainVtxMin.z <= TargetPos.z + TargetVtxMax.z &&	// ���݂̍ŏ��l���G�̍ő�l����������
			MainPosOld.z + MainVtxMin.z >= TargetPos.z + TargetVtxMax.z)	// �O��̍ŏ��l���G�̍ő�l�����傫��
		{// ������

			MainPos.z = (TargetPos.z + TargetVtxMax.z) - MainVtxMin.z + 0.1f;
		}
		if (MainPos.z + MainVtxMax.z >= TargetPos.z + TargetVtxMin.z &&	// ���݂̍ő�l���G�̍ŏ��l�����傫��
			MainPosOld.z + MainVtxMax.z <= TargetPos.z + TargetVtxMin.z)	// �O��̍ő�l���G�̍ŏ��l����������
		{// ��O����

			MainPos.z = (TargetPos.z + TargetVtxMin.z) - MainVtxMax.z - 0.1f;
		}
	}
}

//==================================================================================
// �L�������m�̓����蔻��(�����������̔���Ԃ�)
//==================================================================================
bool bHitCharacter(D3DXVECTOR3 &MainPos, D3DXVECTOR3 &MainPosOld, D3DXVECTOR3 MainVtxMax, D3DXVECTOR3 MainVtxMin, D3DXVECTOR3 &TargetPos, D3DXVECTOR3 TargetVtxMax, D3DXVECTOR3 TargetVtxMin)
{
	bool bHit = false;

	// ���̔���
	if (MainPos.z + MainVtxMax.z > TargetPos.z + TargetVtxMin.z &&	// �G�̍ŏ��l�ɓ����Ă�
		MainPos.z + MainVtxMin.z < TargetPos.z + TargetVtxMax.z)		// �G�̍ő�l�ɓ����Ă�
	{// �G��Z���̒��ɂ���Ƃ�

		if (MainPos.x + MainVtxMin.x <= TargetPos.x + TargetVtxMax.x &&	// ���݂̍ŏ��l���G�̍ő�l����������
			MainPosOld.x + MainVtxMin.x >= TargetPos.x + TargetVtxMax.x)	// �O��̍ŏ��l���G�̍ő�l�����傫��
		{// �E����

			bHit = true;
		}
		if (MainPos.x + MainVtxMax.x >= TargetPos.x + TargetVtxMin.x &&	// ���݂̍ő�l���G�̍ŏ��l�����傫��
			MainPosOld.x + MainVtxMax.x <= TargetPos.x + TargetVtxMin.x)	// �O��̍ő�l���G�̍ŏ��l����������
		{// ������

			bHit = true;
		}
	}

	// �c�̔���
	if (MainPos.x + MainVtxMax.x > TargetPos.x + TargetVtxMin.x &&	// �G�̍ŏ��l�ɓ����Ă�
		MainPos.x + MainVtxMin.x < TargetPos.x + TargetVtxMax.x)		// �G�̍ő�l�ɓ����Ă�
	{// �G��X���̒��ɂ���Ƃ�

		if (MainPos.z + MainVtxMin.z <= TargetPos.z + TargetVtxMax.z &&	// ���݂̍ŏ��l���G�̍ő�l����������
			MainPosOld.z + MainVtxMin.z >= TargetPos.z + TargetVtxMax.z)	// �O��̍ŏ��l���G�̍ő�l�����傫��
		{// ������

			bHit = true;
		}
		if (MainPos.z + MainVtxMax.z >= TargetPos.z + TargetVtxMin.z &&	// ���݂̍ő�l���G�̍ŏ��l�����傫��
			MainPosOld.z + MainVtxMax.z <= TargetPos.z + TargetVtxMin.z)	// �O��̍ő�l���G�̍ŏ��l����������
		{// ��O����

			bHit = true;
		}
	}

	return bHit;
}

//==================================================================================
// �����̓����蔻��
//==================================================================================
void CollisionLimitLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 &pPos, D3DXVECTOR3 posOld)
{
	// ���E���̃x�N�g��
	D3DXVECTOR3 vecLine;
	vecLine.x = pos1.x - pos0.x;
	vecLine.z = pos1.z - pos0.z;

	// �v���C���[�̋��E���̃x�N�g��
	D3DXVECTOR3 vecLinePlayer;
	vecLinePlayer.x = pPos.x - posOld.x;
	vecLinePlayer.z = pPos.z - posOld.z;

	// �v���C���[�ƕǂ̃x�N�g��
	D3DXVECTOR3 vecToPosPlayer;
	vecToPosPlayer.x = pos1.x - posOld.x;
	vecToPosPlayer.z = pos1.z - posOld.z;

	// �ʐς̍ő�l
	float fMaxAreaPlayer = (vecLinePlayer.z * vecLine.x) - (vecLinePlayer.x * vecLine.z);

	// ����̖ʐ�
	float fNowAreaPlayer = (vecToPosPlayer.z * vecLine.x) - (vecToPosPlayer.x * vecLine.z);

	// ����
	float fRatePlayer = fNowAreaPlayer / fMaxAreaPlayer;

	if (fRatePlayer >= 0.0f && fRatePlayer <= 1.0f)
	{// �ʐς͈͓̔��ɂ����画��

		if ((vecLinePlayer.z * vecToPosPlayer.x) - (vecLinePlayer.x * vecToPosPlayer.z) > 0)
		{// �ǂɓ���������

			// ��_����͂ݏo����
			D3DXVECTOR3 CollisionPointPlayer = D3DXVECTOR3(0.0f, pPos.y, 0.0f);
			CollisionPointPlayer.x = (pPos.x + (vecLinePlayer.x * (fRatePlayer - 1.0f)));
			CollisionPointPlayer.z = (pPos.z + (vecLinePlayer.z * (fRatePlayer - 1.0f)));

			// �@���x�N�g��(���E���x�N�g����X��Z���])
			D3DXVECTOR3 vecNor = D3DXVECTOR3(vecLine.z, 0.0f, -vecLine.x);

			// �x�N�g���̐��K��
			D3DXVec3Normalize(&vecNor, &vecNor);

			// �v���C���[�̋t�ړ���
			D3DXVECTOR3 PlayerInverceMove;
			PlayerInverceMove.z = vecLinePlayer.z * (fRatePlayer - 1.0f);
			PlayerInverceMove.x = vecLinePlayer.x * (fRatePlayer - 1.0f);

			// ����(�ǂ̖@���ƃv���C���[�̋t�ړ���)
			float fDot = (PlayerInverceMove.x * vecNor.x) + (PlayerInverceMove.z * vecNor.z);

			// �ǂ���ړ���
			D3DXVECTOR3 MoveWall = vecNor * fDot;

			D3DXVECTOR3 ppp = (CollisionPointPlayer - pPos);
			D3DXVECTOR3 pppp = ppp + MoveWall;

			// �Ԃ������_�ɕ␳
			pPos += MoveWall + (vecNor * 0.1f);

		}
	}
}

//==================================================================================
//  ��̓����蔻��
//==================================================================================
bool CollisionFan(D3DXVECTOR3 posChar, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld)
{
	// �����������̔���
	bool bHit = false;
	bool bLine1 = false, bLine2 = false, bCircle = false;

	// ���̓_�Ƃ̐���
	bLine1 = CollisionLine(posChar, posLeft, MainPos, MainPosOld);

	// �E�̓_�Ƃ̐���
	bLine2 = CollisionLine(posRight, posChar, MainPos, MainPosOld);

	// ��̔��a
	float fLen = 
		sqrtf((posLeft.x - posChar.x) * (posLeft.x - posChar.x)
		+ (posLeft.z - posChar.z) * (posLeft.z - posChar.z));

	// �~�̓����蔻��
	if (CircleRange(posChar, MainPos, fLen, fLen) == true)
	{// �������Ă�����

		bCircle = true;
	}

	// �S�Ă͈̔͂ɓ����Ă�����
	if (bLine1 == true && bLine2 == true && bCircle == true)
	{
		bHit = true;
	}

	return bHit;
}

//==================================================================================
// �O�p�̓����ɂ��邩����
//==================================================================================
bool CollisionTriangle(D3DXVECTOR3 posCenter, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld)
{
	// �����������̔���
	bool bHit = false;
	bool bLine1 = false, bLine2 = false, bLine3 = false;

	// �^�񒆂ƍ��̐�
	bLine1 = CollisionLine(posCenter, posLeft, MainPos, MainPosOld);

	// ���ƉE�̐�
	bLine2 = CollisionLine(posLeft, posRight, MainPos, MainPosOld);

	// �E�Ɛ^�񒆂̐�
	bLine3 = CollisionLine(posRight, posCenter, MainPos, MainPosOld);

	// �S�Ă͈̔͂ɓ����Ă�����
	if (bLine1 == true && bLine2 == true && bLine3 == true)
	{
		bHit = true;
	}

	return bHit;
}

//==================================================================================
//  ���̓����蔻��  
//==================================================================================
bool CollisionLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld)
{
	// �����������̔���
	bool bHit = false;

	// ���E���̃x�N�g��
	D3DXVECTOR3 vecLine;
	vecLine.x = pos1.x - pos0.x;
	vecLine.z = pos1.z - pos0.z;

	// �e�ƕǂ̃x�N�g��
	D3DXVECTOR3 vecToPos;
	vecToPos.x = MainPos.x - pos0.x;
	vecToPos.z = MainPos.z - pos0.z;

	// �O��̒e�ƕǂ̃x�N�g��
	D3DXVECTOR3 vecToPosOld;
	vecToPosOld.x = MainPosOld.x - pos0.x;
	vecToPosOld.z = MainPosOld.z - pos0.z;

	// �Ώۂ̈ړ��x�N�g��
	D3DXVECTOR3 vecMove;
	vecMove.x = MainPos.x - MainPosOld.x;
	vecMove.z = MainPos.z - MainPosOld.z;

	if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) <= 0)
	{// ���𒴂���

		// ��������
		bHit = true;
	}

	return bHit;
}

// �p�X�̍Ō�̕��������擾
std::string extractFileName(const std::string& filePath) 
{
	size_t lastSeparator = filePath.find_last_of("/\\");

	if (lastSeparator != std::string::npos) 
	{
		return filePath.substr(lastSeparator + 1);
	}

	return filePath;
}