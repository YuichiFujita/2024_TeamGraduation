//==================================================================================
// 
// 便利関数[calculation.cpp]
// Author:相馬靜雅
// 
//==================================================================================
#include "calculation.h"

// グローバル変数宣言
MOVELOG g_aMoveLog[MOVE_LOG];		// ロゴ拡縮の構造体

//==================================================================================
// 初期化処理
//==================================================================================
void InitCalculastion(void)
{
	// ロゴ拡縮
	for (int nCntLog = 0; nCntLog < MOVE_LOG; nCntLog++)
	{
		g_aMoveLog[nCntLog].nCnt = 0;
		g_aMoveLog[nCntLog].nPattern = 0;
		g_aMoveLog[nCntLog].Length = 0.0f;
		g_aMoveLog[nCntLog].bUse = false;
	}
}

//==================================================================================
// ロゴの設定
//==================================================================================
int SetMoveLog(void)
{
	int nIdxMoveLog = -1;

	for (int nCntLog = 0; nCntLog < MOVE_LOG; nCntLog++)
	{
		if (g_aMoveLog[nCntLog].bUse == false)
		{// 使用されていなかったら

		 // インデックス保持して処理抜け
			g_aMoveLog[nCntLog].bUse = true;
			nIdxMoveLog = nCntLog;
			break;
		}
	}

	return nIdxMoveLog;
}

//==================================================================================
// ロゴの相対座標算出
//==================================================================================
float MoveLog(int nIdxMoveLog, float pos, int nCntElapsed)
{

	// カウント加算
	g_aMoveLog[nIdxMoveLog].nCnt = (g_aMoveLog[nIdxMoveLog].nCnt + 1) % nCntElapsed;

	if (g_aMoveLog[nIdxMoveLog].nPattern == MOVELOG_TYPE_ADD)
	{// 拡大

		if ((g_aMoveLog[nIdxMoveLog].nCnt % nCntElapsed) == 0)
		{
			g_aMoveLog[nIdxMoveLog].nPattern = (g_aMoveLog[nIdxMoveLog].nPattern + 1) % MOVELOG_TYPE_MAX;	// パターンNo.を更新
		}

		g_aMoveLog[nIdxMoveLog].Length = pos + g_aMoveLog[nIdxMoveLog].nCnt * 6.0f;
	}
	else if (g_aMoveLog[nIdxMoveLog].nPattern == MOVELOG_TYPE_REMOVE)
	{// 縮小

		if ((g_aMoveLog[nIdxMoveLog].nCnt % nCntElapsed) == 0)
		{
			g_aMoveLog[nIdxMoveLog].nPattern = (g_aMoveLog[nIdxMoveLog].nPattern + 1) % MOVELOG_TYPE_MAX;	// パターンNo.を更新
		}

		g_aMoveLog[nIdxMoveLog].Length = pos - g_aMoveLog[nIdxMoveLog].nCnt * 6.0f;
	}
	else if (g_aMoveLog[nIdxMoveLog].nPattern == MOVELOG_TYPE_SAME)
	{// 戻す

		// 拡大する
		g_aMoveLog[nIdxMoveLog].Length = pos + g_aMoveLog[nIdxMoveLog].nCnt * 6.0f;

		if (g_aMoveLog[nIdxMoveLog].Length >= pos)
		{// 規定値以上

		 // 規定値に戻す
			g_aMoveLog[nIdxMoveLog].Length = pos;
		}
	}

	return g_aMoveLog[nIdxMoveLog].Length;
}

//==================================================================================
// ロゴのリセット
//==================================================================================
void ResetMoveLog(int nIdxMoveLog)
{
	g_aMoveLog[nIdxMoveLog].nCnt = 0;
	g_aMoveLog[nIdxMoveLog].Length = 0.0f;
	g_aMoveLog[nIdxMoveLog].nPattern = MOVELOG_TYPE_ADD;
}

//==================================================================================
// 角度の正規化処理
//==================================================================================
void RotNormalize(float &fRot)
{
	// 1周分越えたら補正する
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
// 慣性補正
//==================================================================================
void InertiaCorrection(float &fMain, float fTarget, float fCntCorrection)
{
	fMain += (fTarget - fMain) * fCntCorrection;
}

//==================================================================================
// ランダム
//==================================================================================
int Random(int nMinNum, int nMaxNum)
{
	// ランダム数
	int nRand = 0;

	// 範囲
	int nRange = nMaxNum - nMinNum;

	if (nMinNum == 0)
	{// 最小値が0の時

	 // 範囲に1加算
		nRange += 1;
	}

	// ランダム生成
	nRand = rand() % nRange + nMinNum;

	return nRand;
}

//==================================================================================
// 円の当たり判定
//==================================================================================
bool CircleRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2)
{
	float fLength = (pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.z - pos2.z) * (pos1.z - pos2.z);

	if (fLength <= (fRadius1 * fRadius2) + (fRadius1 * fRadius2))
	{// 円の中に入ったら

		// 当たった判定返す
		return true;
	}

	return false;
}

//==================================================================================
// 球の当たり判定
//==================================================================================
bool SphereRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2)
{
	float fLength = (pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.y - pos2.y) * (pos1.y - pos2.y) + (pos1.z - pos2.z) * (pos1.z - pos2.z);

	if (fLength <= (fRadius1 + fRadius2) + (fRadius1 * fRadius2))
	{// 円の中に入ったら

		// 当たった判定返す
		return true;
	}

	return false;
}

//==================================================================================
// 1と2の当たり判定(筒)
//==================================================================================
bool CylinderRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2, float fHeight1, float fHeight2)
{
	float fLength = (pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.z - pos2.z) * (pos1.z - pos2.z);

	if (fLength <= (fRadius1 * fRadius2) + (fRadius1 * fRadius2))
	{// 円の中に入ったら

		if (pos1.y - fHeight1 <= pos2.y + fHeight2)
		{// 高さもぶつかっていたら

			// 当たった判定返す
			return true;
		}
	}

	return false;
}

//==================================================================================
// 全頂点チェック
//==================================================================================
void CheckVtx(float RotY, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin, LPD3DXMESH pMesh, BYTE *pVtxBuff)
{
	//リセット
	*pVtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	*pVtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

	//モデルの頂点数を取得
	int nNumVtx = pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	DWORD dwSizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

	//頂点バッファをロック
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//全ての頂点をチェックする
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//今回のデータ
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標の代入

		//X
		if (pVtxMin->x > vtx.x)
		{//今の最小値よりも今回の値がちいさかったら

			pVtxMin->x = vtx.x;
		}
		if (pVtxMax->x < vtx.x)
		{//今の最大値よりも今回の値が大きかったら

			pVtxMax->x = vtx.x;
		}

		//Y
		if (pVtxMin->y > vtx.y)
		{//今の最小値よりも今回の値がちいさかったら

			pVtxMin->y = vtx.y;
		}
		if (pVtxMax->y < vtx.y)
		{//今の最大値よりも今回の値が大きかったら

			pVtxMax->y = vtx.y;
		}

		//Z
		if (pVtxMin->z > vtx.z)
		{//今の最小値よりも今回の値がちいさかったら

			pVtxMin->z = vtx.z;
		}
		if (pVtxMax->z < vtx.z)
		{//今の最大値よりも今回の値が大きかったら

			pVtxMax->z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;	//サイズ分ポインタ移動
	}

	//頂点バッファをアンロック
	pMesh->UnlockVertexBuffer();

	D3DXVECTOR3 VtxMax = *pVtxMax, VtxMin = *pVtxMin;

	if (RotY <= D3DX_PI * 0.5f + 0.05f && RotY >= D3DX_PI * 0.5f - 0.05f)
	{//1.57fのとき

		pVtxMax->x = VtxMax.z;
		pVtxMin->x = VtxMin.z;

		pVtxMax->z = -VtxMin.x;
		pVtxMin->z = -VtxMax.x;
	}
	else if (RotY <= -D3DX_PI * 0.5f + 0.05f && RotY >= -D3DX_PI * 0.5f - 0.05f)
	{//-1.57fのとき
		
		////////////////////////////////

		pVtxMax->x = -VtxMin.z;
		pVtxMin->x = -VtxMax.z;

		pVtxMax->z = VtxMax.x;
		pVtxMin->z = VtxMin.x;
	}
	else if (RotY <= D3DX_PI + 0.05f && RotY >= D3DX_PI - 0.05f)
	{//D3DX_PIのとき

	 ////////////////////////////////

		pVtxMax->x = -VtxMin.x;
		pVtxMin->x = -VtxMax.x;

		pVtxMax->z = -VtxMin.z;
		pVtxMin->z = -VtxMax.z;
	}
	else if (RotY <= -D3DX_PI + 0.05f && RotY >= -D3DX_PI - 0.05f)
	{//-D3DX_PIのとき

		pVtxMax->x = -VtxMin.x;
		pVtxMin->x = -VtxMax.x;

		pVtxMax->z = -VtxMin.z;
		pVtxMin->z = -VtxMax.z;
	}
}

//==================================================================================
// キャラ同士の当たり判定(補正)
//==================================================================================
void CollisionCharacter(D3DXVECTOR3 &MainPos, D3DXVECTOR3 &MainPosOld, D3DXVECTOR3 MainVtxMax, D3DXVECTOR3 MainVtxMin, D3DXVECTOR3 &TargetPos, D3DXVECTOR3 TargetVtxMax, D3DXVECTOR3 TargetVtxMin)
{

	// 横の判定
	if (MainPos.z + MainVtxMax.z > TargetPos.z + TargetVtxMin.z &&	// 敵の最小値に入ってる
		MainPos.z + MainVtxMin.z < TargetPos.z + TargetVtxMax.z)		// 敵の最大値に入ってる
	{// 敵のZ幅の中にいるとき

		if (MainPos.x + MainVtxMin.x <= TargetPos.x + TargetVtxMax.x &&	// 現在の最小値が敵の最大値よりも小さい
			MainPosOld.x + MainVtxMin.x >= TargetPos.x + TargetVtxMax.x)	// 前回の最小値が敵の最大値よりも大きい
		{// 右から

			MainPos.x = (TargetPos.x + TargetVtxMax.x) - MainVtxMin.x + 0.1f;
		}
		if (MainPos.x + MainVtxMax.x >= TargetPos.x + TargetVtxMin.x &&	// 現在の最大値が敵の最小値よりも大きい
			MainPosOld.x + MainVtxMax.x <= TargetPos.x + TargetVtxMin.x)	// 前回の最大値が敵の最小値よりも小さい
		{// 左から

			MainPos.x = (TargetPos.x + TargetVtxMin.x) - MainVtxMax.x - 0.1f;
		}
	}

	// 縦の判定
	if (MainPos.x + MainVtxMax.x > TargetPos.x + TargetVtxMin.x &&	// 敵の最小値に入ってる
		MainPos.x + MainVtxMin.x < TargetPos.x + TargetVtxMax.x)		// 敵の最大値に入ってる
	{// 敵のX幅の中にいるとき

		if (MainPos.z + MainVtxMin.z <= TargetPos.z + TargetVtxMax.z &&	// 現在の最小値が敵の最大値よりも小さい
			MainPosOld.z + MainVtxMin.z >= TargetPos.z + TargetVtxMax.z)	// 前回の最小値が敵の最大値よりも大きい
		{// 奥から

			MainPos.z = (TargetPos.z + TargetVtxMax.z) - MainVtxMin.z + 0.1f;
		}
		if (MainPos.z + MainVtxMax.z >= TargetPos.z + TargetVtxMin.z &&	// 現在の最大値が敵の最小値よりも大きい
			MainPosOld.z + MainVtxMax.z <= TargetPos.z + TargetVtxMin.z)	// 前回の最大値が敵の最小値よりも小さい
		{// 手前から

			MainPos.z = (TargetPos.z + TargetVtxMin.z) - MainVtxMax.z - 0.1f;
		}
	}
}

//==================================================================================
// キャラ同士の当たり判定(当たったかの判定返す)
//==================================================================================
bool bHitCharacter(D3DXVECTOR3 &MainPos, D3DXVECTOR3 &MainPosOld, D3DXVECTOR3 MainVtxMax, D3DXVECTOR3 MainVtxMin, D3DXVECTOR3 &TargetPos, D3DXVECTOR3 TargetVtxMax, D3DXVECTOR3 TargetVtxMin)
{
	bool bHit = false;

	// 横の判定
	if (MainPos.z + MainVtxMax.z > TargetPos.z + TargetVtxMin.z &&	// 敵の最小値に入ってる
		MainPos.z + MainVtxMin.z < TargetPos.z + TargetVtxMax.z)		// 敵の最大値に入ってる
	{// 敵のZ幅の中にいるとき

		if (MainPos.x + MainVtxMin.x <= TargetPos.x + TargetVtxMax.x &&	// 現在の最小値が敵の最大値よりも小さい
			MainPosOld.x + MainVtxMin.x >= TargetPos.x + TargetVtxMax.x)	// 前回の最小値が敵の最大値よりも大きい
		{// 右から

			bHit = true;
		}
		if (MainPos.x + MainVtxMax.x >= TargetPos.x + TargetVtxMin.x &&	// 現在の最大値が敵の最小値よりも大きい
			MainPosOld.x + MainVtxMax.x <= TargetPos.x + TargetVtxMin.x)	// 前回の最大値が敵の最小値よりも小さい
		{// 左から

			bHit = true;
		}
	}

	// 縦の判定
	if (MainPos.x + MainVtxMax.x > TargetPos.x + TargetVtxMin.x &&	// 敵の最小値に入ってる
		MainPos.x + MainVtxMin.x < TargetPos.x + TargetVtxMax.x)		// 敵の最大値に入ってる
	{// 敵のX幅の中にいるとき

		if (MainPos.z + MainVtxMin.z <= TargetPos.z + TargetVtxMax.z &&	// 現在の最小値が敵の最大値よりも小さい
			MainPosOld.z + MainVtxMin.z >= TargetPos.z + TargetVtxMax.z)	// 前回の最小値が敵の最大値よりも大きい
		{// 奥から

			bHit = true;
		}
		if (MainPos.z + MainVtxMax.z >= TargetPos.z + TargetVtxMin.z &&	// 現在の最大値が敵の最小値よりも大きい
			MainPosOld.z + MainVtxMax.z <= TargetPos.z + TargetVtxMin.z)	// 前回の最大値が敵の最小値よりも小さい
		{// 手前から

			bHit = true;
		}
	}

	return bHit;
}

//==================================================================================
// 線分の当たり判定
//==================================================================================
void CollisionLimitLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 &pPos, D3DXVECTOR3 posOld)
{
	// 境界線のベクトル
	D3DXVECTOR3 vecLine;
	vecLine.x = pos1.x - pos0.x;
	vecLine.z = pos1.z - pos0.z;

	// プレイヤーの境界線のベクトル
	D3DXVECTOR3 vecLinePlayer;
	vecLinePlayer.x = pPos.x - posOld.x;
	vecLinePlayer.z = pPos.z - posOld.z;

	// プレイヤーと壁のベクトル
	D3DXVECTOR3 vecToPosPlayer;
	vecToPosPlayer.x = pos1.x - posOld.x;
	vecToPosPlayer.z = pos1.z - posOld.z;

	// 面積の最大値
	float fMaxAreaPlayer = (vecLinePlayer.z * vecLine.x) - (vecLinePlayer.x * vecLine.z);

	// 今回の面積
	float fNowAreaPlayer = (vecToPosPlayer.z * vecLine.x) - (vecToPosPlayer.x * vecLine.z);

	// 割合
	float fRatePlayer = fNowAreaPlayer / fMaxAreaPlayer;

	if (fRatePlayer >= 0.0f && fRatePlayer <= 1.0f)
	{// 面積の範囲内にいたら判定

		if ((vecLinePlayer.z * vecToPosPlayer.x) - (vecLinePlayer.x * vecToPosPlayer.z) > 0)
		{// 壁に当たったら

			// 交点からはみ出た分
			D3DXVECTOR3 CollisionPointPlayer = D3DXVECTOR3(0.0f, pPos.y, 0.0f);
			CollisionPointPlayer.x = (pPos.x + (vecLinePlayer.x * (fRatePlayer - 1.0f)));
			CollisionPointPlayer.z = (pPos.z + (vecLinePlayer.z * (fRatePlayer - 1.0f)));

			// 法線ベクトル(境界線ベクトルのXとZ反転)
			D3DXVECTOR3 vecNor = D3DXVECTOR3(vecLine.z, 0.0f, -vecLine.x);

			// ベクトルの正規化
			D3DXVec3Normalize(&vecNor, &vecNor);

			// プレイヤーの逆移動量
			D3DXVECTOR3 PlayerInverceMove;
			PlayerInverceMove.z = vecLinePlayer.z * (fRatePlayer - 1.0f);
			PlayerInverceMove.x = vecLinePlayer.x * (fRatePlayer - 1.0f);

			// 内積(壁の法線とプレイヤーの逆移動量)
			float fDot = (PlayerInverceMove.x * vecNor.x) + (PlayerInverceMove.z * vecNor.z);

			// 壁ずり移動量
			D3DXVECTOR3 MoveWall = vecNor * fDot;

			D3DXVECTOR3 ppp = (CollisionPointPlayer - pPos);
			D3DXVECTOR3 pppp = ppp + MoveWall;

			// ぶつかった点に補正
			pPos += MoveWall + (vecNor * 0.1f);

		}
	}
}

//==================================================================================
//  扇の当たり判定
//==================================================================================
bool CollisionFan(D3DXVECTOR3 posChar, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld)
{
	// 当たったかの判定
	bool bHit = false;
	bool bLine1 = false, bLine2 = false, bCircle = false;

	// 左の点との線分
	bLine1 = CollisionLine(posChar, posLeft, MainPos, MainPosOld);

	// 右の点との線分
	bLine2 = CollisionLine(posRight, posChar, MainPos, MainPosOld);

	// 扇の半径
	float fLen = 
		sqrtf((posLeft.x - posChar.x) * (posLeft.x - posChar.x)
		+ (posLeft.z - posChar.z) * (posLeft.z - posChar.z));

	// 円の当たり判定
	if (CircleRange(posChar, MainPos, fLen, fLen) == true)
	{// 当たっていたら

		bCircle = true;
	}

	// 全ての範囲に入っていたら
	if (bLine1 == true && bLine2 == true && bCircle == true)
	{
		bHit = true;
	}

	return bHit;
}

//==================================================================================
// 三角の内側にいるか判定
//==================================================================================
bool CollisionTriangle(D3DXVECTOR3 posCenter, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld)
{
	// 当たったかの判定
	bool bHit = false;
	bool bLine1 = false, bLine2 = false, bLine3 = false;

	// 真ん中と左の線
	bLine1 = CollisionLine(posCenter, posLeft, MainPos, MainPosOld);

	// 左と右の線
	bLine2 = CollisionLine(posLeft, posRight, MainPos, MainPosOld);

	// 右と真ん中の線
	bLine3 = CollisionLine(posRight, posCenter, MainPos, MainPosOld);

	// 全ての範囲に入っていたら
	if (bLine1 == true && bLine2 == true && bLine3 == true)
	{
		bHit = true;
	}

	return bHit;
}

//==================================================================================
//  線の当たり判定  
//==================================================================================
bool CollisionLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld)
{
	// 当たったかの判定
	bool bHit = false;

	// 境界線のベクトル
	D3DXVECTOR3 vecLine;
	vecLine.x = pos1.x - pos0.x;
	vecLine.z = pos1.z - pos0.z;

	// 弾と壁のベクトル
	D3DXVECTOR3 vecToPos;
	vecToPos.x = MainPos.x - pos0.x;
	vecToPos.z = MainPos.z - pos0.z;

	// 前回の弾と壁のベクトル
	D3DXVECTOR3 vecToPosOld;
	vecToPosOld.x = MainPosOld.x - pos0.x;
	vecToPosOld.z = MainPosOld.z - pos0.z;

	// 対象の移動ベクトル
	D3DXVECTOR3 vecMove;
	vecMove.x = MainPos.x - MainPosOld.x;
	vecMove.z = MainPos.z - MainPosOld.z;

	if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) <= 0)
	{// 線を超えた

		// 当たった
		bHit = true;
	}

	return bHit;
}

// パスの最後の部分だけ取得
std::string extractFileName(const std::string& filePath) 
{
	size_t lastSeparator = filePath.find_last_of("/\\");

	if (lastSeparator != std::string::npos) 
	{
		return filePath.substr(lastSeparator + 1);
	}

	return filePath;
}