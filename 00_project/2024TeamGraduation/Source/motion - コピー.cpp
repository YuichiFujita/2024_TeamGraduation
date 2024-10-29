//=============================================================================
// 
//  モデル処理 [motion.cpp]
//  Author : 相馬靜雅
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
// 静的メンバ変数宣言
//==========================================================================
std::vector<std::string> CMotion::m_sTextFile = {};		// テキストファイル名
std::vector<std::vector<CMotion::Info>> CMotion::m_vecLoadData = {};	// モーションの読み込み情報
int CMotion::m_nNumLoad = 0;								// 読み込んだ数
std::vector<int> CMotion::m_nNumLoadData = {};				// モーション毎のデータ数

//==========================================================================
// コンストラクタ
//==========================================================================
CMotion::CMotion()
{
	// 値のクリア
	m_nNumAll = 0;			// モーションの総数
	m_nType = 0;			// 現在のモーションの種類
	m_nOldType = 0;			// 前回のモーションの種類
	m_bLoop = false;		// ループするかどうか
	m_nPatternKey = 0;		// 何個目のキーか
	m_fCntFrame = 0.0f;		// フレームのカウント
	m_fCntAllFrame = 0.0f;	// 全てのカウント
	m_fMaxAllFrame = 0.0f;	// 全てのカウントの最大値
	m_fSlowFactor = 0.0f;	// 遅延係数
	m_bFinish = false;		// 終了したかどうか
	m_bCancelable = false;	// キャンセル可能か
	m_bCombiable = false;	// コンボ可能か
	m_bBeforeInAction = false;		// 攻撃前フラグ
	m_bAttaking = false;			// 攻撃判定中フラグ
	m_bAllAfterAttack = false;		// 全ての攻撃後フラグ

	m_pObjChara = nullptr;		// オブジェクトのポインタ
	m_ppModel = nullptr;		// モデルのポインタ
	m_nNumModel = 0;		// モデルの総数
	m_nNumMotion = 0;		// モーションの総数
	m_vecInfo.clear();		// モーション情報
	//m_pPartsOld = nullptr;	// 過去の情報

}

//==========================================================================
// デストラクタ
//==========================================================================
CMotion::~CMotion()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CMotion* CMotion::Create(const std::string& file, CObjectChara* pObjChara)
{
	// 生成用のオブジェクト
	CMotion* pMotion = DEBUG_NEW CMotion;

	if (pMotion != nullptr)
	{
		// オブジェクトのポインタを渡す
		pMotion->m_pObjChara = pObjChara;

		// 初期化処理
		pMotion->Init();
		pMotion->ReadText(file);
	}

	return pMotion;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMotion::Init()
{
	m_nNumAll = 0;			// モーションの総数
	m_nType = 0;			// 現在のモーションの種類
	m_bLoop = false;		// ループするかどうか
	m_nPatternKey = 0;		// 何個目のキーか
	m_fCntFrame = 0.0f;		// フレームのカウント
	m_fCntAllFrame = 0.0f;	// 全てのカウント
	m_fMaxAllFrame = 0.0f;	// 全てのカウントの最大値
	m_bFinish = true;		// 終了したかどうか
	m_ppModel = nullptr;		// モデルのポインタ
	m_nNumModel = 0;		// モデルの総数
	m_fSlowFactor = 1.0f;	// 遅延係数

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMotion::Uninit()
{
	if (m_pPartsOld != nullptr)
	{
		delete[] m_pPartsOld;
		m_pPartsOld = nullptr;
	}
}

//==========================================================================
// モーション情報の登録
//==========================================================================
void CMotion::SetInfo(Info info)
{
	if (m_nNumMotion <= m_nNumAll)
	{
		return;
	}

	// コピー
	memcpy(m_vecInfo[m_nNumAll].aKey, info.aKey, sizeof(info.aKey));

	m_vecInfo[m_nNumAll].nLoop = info.nLoop;
	m_vecInfo[m_nNumAll].nMove = info.nMove;
	m_vecInfo[m_nNumAll].nNumKey = info.nNumKey;
	m_vecInfo[m_nNumAll].nCancelableFrame = info.nCancelableFrame;
	m_vecInfo[m_nNumAll].nCombolableFrame = info.nCombolableFrame;

	for (int nCntKey = 0; nCntKey < m_vecInfo[m_nNumAll].nNumKey; nCntKey++)
	{
		int nBeforeCnt = nCntKey - 1;
		if (nBeforeCnt <= 0)
		{
			nBeforeCnt = 0;
		}

		float PosX = m_vecInfo[m_nNumAll].aKey[nBeforeCnt].aParts[0].pos.x;
		float PosZ = m_vecInfo[m_nNumAll].aKey[nBeforeCnt].aParts[0].pos.z;

		// 向きを求める
		m_vecInfo[m_nNumAll].aKey[nCntKey].fRotMove = atan2f(
			(PosX - m_vecInfo[m_nNumAll].aKey[nCntKey].aParts[0].pos.x),
			(PosZ - m_vecInfo[m_nNumAll].aKey[nCntKey].aParts[0].pos.z));
	}

	// モーションの総数加算
	m_nNumAll++;
}

//==========================================================================
// 攻撃情報の登録
//==========================================================================
void CMotion::SetAttackInfo(AttackInfo info)
{
	// 今読み込んでいる
	int nIdx = m_vecInfo[m_nNumAll].nNumAttackInfo;

	// モーション情報の終端に
	m_vecInfo.back().AttackInfo.back().push_back(info);

	// メモリ確保
	m_vecInfo[m_nNumAll].AttackInfo[nIdx] = DEBUG_NEW AttackInfo;

	if (m_vecInfo[m_nNumAll].AttackInfo[nIdx] != nullptr)
	{// nullptrじゃなければ

		// 情報渡す
		*m_vecInfo[m_nNumAll].AttackInfo[nIdx] = info;

		// 攻撃情報の総数加算
		m_vecInfo[m_nNumAll].nNumAttackInfo++;
	}
}

//==========================================================================
// モーションをするモデルの登録
//==========================================================================
void CMotion::SetModel(CModel **pModel, int nNumModel)
{
	// モデルのポインタを渡す
	m_ppModel = pModel;

	// モデルの総数
	m_nNumModel = nNumModel;

	m_pPartsOld = DEBUG_NEW Parts[m_nNumModel];
	if (m_pPartsOld == nullptr)
	{
		return;
	}
	memset(m_pPartsOld, 0, sizeof(Parts) * m_nNumModel);
}

//==========================================================================
// ポーズのリセット
//==========================================================================
void CMotion::ResetPose(int nType)
{
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{// 全パーツ分繰り返す

		int nCntModel = nCntParts;
		if (nCntModel >= m_nNumModel)
		{
			break;
		}

		if (m_ppModel[nCntModel] == nullptr)
		{// nullptrだったら
			continue;
		}

		// 向き設定
		m_ppModel[nCntModel]->SetRotation(m_vecInfo[nType].aKey[0].aParts[nCntParts].rot);
		m_pPartsOld[nCntParts].rot = m_vecInfo[nType].aKey[0].aParts[nCntParts].rot;
		m_pPartsOld[nCntParts].scale = m_vecInfo[nType].aKey[0].aParts[nCntParts].scale;

		// 元の位置取得
		MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

		// 位置設定
		if (nCntParts == 0)
		{
			m_ppModel[nCntModel]->SetPosition(m_vecInfo[nType].aKey[0].aParts[nCntParts].pos + posOrigin);
		}
		else
		{
			m_ppModel[nCntModel]->SetPosition(m_vecInfo[nType].aKey[0].aParts[nCntParts].pos + m_ppModel[nCntModel]->GetOriginPosition());
		}
	}

	m_pPartsOld[0].pos = m_vecInfo[nType].aKey[0].aParts[0].pos;

	Set(nType);
}

//==========================================================================
// 更新処理
//==========================================================================
void CMotion::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#if 0
	// テキストの描画
	CManager::GetInstance()->GetDebugProc()->Print(
		"倍率：%f\n", fBuff);
#endif

	// 攻撃判定中フラグリセット
	m_bAttaking = false;

	// 攻撃情報の総数取得
	int nNumAttackInfo = m_vecInfo[m_nType].nNumAttackInfo;

	for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
	{
		if (m_vecInfo[m_nType].AttackInfo[nCntAttack] == nullptr)
		{
			continue;
		}

		if (m_bBeforeInAction)
		{
			if (m_vecInfo[m_nType].AttackInfo[nCntAttack]->nMaxCnt > 0 &&
				m_vecInfo[m_nType].AttackInfo[nCntAttack]->nMinCnt <= m_fCntAllFrame)
			{
				m_bBeforeInAction = false;
			}
		}

		if (m_vecInfo[m_nType].AttackInfo[nCntAttack]->nInpactCnt < 0)
		{
			continue;
		}

		// まだ衝撃カウントの行動をしてない状態にする
		m_vecInfo[m_nType].AttackInfo[nCntAttack]->bInpactAct = false;

		if (m_fCntAllFrame >= m_vecInfo[m_nType].AttackInfo[nCntAttack]->nInpactCnt &&
			m_vecInfo[m_nType].AttackInfo[nCntAttack]->bInpactActSet == false)
		{// 衝撃のカウントを超えた時 && まだ行動してなかったら
			
			// まだ衝撃カウントの行動をしてない状態にする
			m_vecInfo[m_nType].AttackInfo[nCntAttack]->bInpactAct = true;
			m_vecInfo[m_nType].AttackInfo[nCntAttack]->bInpactActSet = true;
		}
	}


	if (m_bFinish && m_vecInfo[m_nType].nLoop == LOOP_OFF)
	{// 終了してた && ループOFFだったら
		return;
	}

	// 再生フレーム
	int nFrame;

	// 再生フレーム保存
	nFrame = m_vecInfo[m_nType].aKey[m_nPatternKey].nFrame;

	if (m_nType == 0 && m_nOldType != 0 && m_nPatternKey == 0)
	{// ニュートラルで0個の時
		nFrame = 10;
	}

	// 次のキー
	int nNextKey = (m_nPatternKey + 1) % m_vecInfo[m_nType].nNumKey;

	if (nNextKey == 0 && m_vecInfo[m_nType].nLoop == LOOP_OFF)
	{// ループしないとき

		// 最後で固定
		nNextKey = m_vecInfo[m_nType].nNumKey - 1;
	}

	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{// 全パーツ分繰り返す

		int nCntModel = nCntParts;
		if (nCntModel >= m_nNumModel)
		{
			break;
		}

		if (m_ppModel[nCntModel] == nullptr)
		{// nullptrだったら
			continue;
		}

		// 次と今の向きの差分取得
		float rotDiffX = m_vecInfo[m_nType].aKey[nNextKey].aParts[nCntParts].rot.x -
			m_pPartsOld[nCntParts].rot.x;

		float rotDiffY = m_vecInfo[m_nType].aKey[nNextKey].aParts[nCntParts].rot.y -
			m_pPartsOld[nCntParts].rot.y;

		float rotDiffZ = m_vecInfo[m_nType].aKey[nNextKey].aParts[nCntParts].rot.z -
			m_pPartsOld[nCntParts].rot.z;

		// 角度の正規化
		UtilFunc::Transformation::RotNormalize(rotDiffX);
		UtilFunc::Transformation::RotNormalize(rotDiffY);
		UtilFunc::Transformation::RotNormalize(rotDiffZ);

		// パーツの向きを設定
		MyLib::Vector3 rot = MyLib::Vector3();

		// パーツの向きを設定
		rot.x =
			m_pPartsOld[nCntParts].rot.x +
			rotDiffX *
			(
			m_fCntFrame /
			(float)nFrame
			);

		// パーツの向きを設定
		rot.y =
			m_pPartsOld[nCntParts].rot.y +
			rotDiffY *
			(
			m_fCntFrame /
			(float)nFrame
			);

		// パーツの向きを設定
		rot.z =
			m_pPartsOld[nCntParts].rot.z +
			rotDiffZ *
			(
			m_fCntFrame /
			(float)nFrame
			);

		// 角度の正規化
		UtilFunc::Transformation::RotNormalize(rot.x);
		UtilFunc::Transformation::RotNormalize(rot.y);
		UtilFunc::Transformation::RotNormalize(rot.z);

		// 向き設定
		m_ppModel[nCntModel]->SetRotation(rot);

		{
			// 次と今の向きの差分取得
			float scaleDiffX = m_vecInfo[m_nType].aKey[nNextKey].aParts[nCntParts].scale.x -
				m_pPartsOld[nCntParts].scale.x;

			float scaleDiffY = m_vecInfo[m_nType].aKey[nNextKey].aParts[nCntParts].scale.y -
				m_pPartsOld[nCntParts].scale.y;

			float scaleDiffZ = m_vecInfo[m_nType].aKey[nNextKey].aParts[nCntParts].scale.z -
				m_pPartsOld[nCntParts].scale.z;

			// パーツの向きを設定
			MyLib::Vector3 scale = MyLib::Vector3(0.0f, 0.0f, 0.0f);

			// パーツの向きを設定
			scale.x =
				m_pPartsOld[nCntParts].scale.x +
				scaleDiffX *
				(
				m_fCntFrame /
				(float)nFrame
				);

			// パーツの向きを設定
			scale.y =
				m_pPartsOld[nCntParts].scale.y +
				scaleDiffY *
				(
				m_fCntFrame /
				(float)nFrame
				);

			// パーツの向きを設定
			scale.z =
				m_pPartsOld[nCntParts].scale.z +
				scaleDiffZ *
				(
				m_fCntFrame /
				(float)nFrame
				);

			// 向き設定
			m_ppModel[nCntModel]->SetScale(scale);
		}

		if (nCntParts == 0)
		{
			// 本体の位置取得
			MyLib::Vector3 pos = m_pObjChara->GetPosition();

			// 本体の向き取得
			MyLib::Vector3 rot = m_pObjChara->GetRotation();

			// 元の位置取得
			MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

			// パーツの位置取得
			MyLib::Vector3 posParts = m_ppModel[nCntModel]->GetPosition();
			MyLib::Vector3 posPartsOld = m_ppModel[nCntModel]->GetPosition();

			// 目標の位置との差分を求める
			float posDiffX = m_vecInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.x -
				m_pPartsOld[nCntParts].pos.x;

			float posDiffY = m_vecInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.y -
				m_pPartsOld[nCntParts].pos.y;

			float posDiffZ = m_vecInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.z -
				m_pPartsOld[nCntParts].pos.z;

			// 親のYを補正
			posParts.y =
				m_pPartsOld[nCntParts].pos.y +
				posDiffY *
				(
				m_fCntFrame /
				(float)nFrame);

			// 位置設定
			m_ppModel[nCntModel]->SetPosition(posParts + posOrigin);

			int nNextMoveKey = m_nPatternKey + 1;
			if (nNextMoveKey >= m_vecInfo[m_nType].nNumKey)
			{
				nNextMoveKey = m_nPatternKey;
			}

			// 動いた長さを求める
			float fMoveDiff =
				sqrtf((m_vecInfo[m_nType].aKey[nNextMoveKey].aParts[nCntParts].pos.x - m_pPartsOld[nCntParts].pos.x) * (m_vecInfo[m_nType].aKey[nNextMoveKey].aParts[nCntParts].pos.x - m_pPartsOld[nCntParts].pos.x)
					+ (m_vecInfo[m_nType].aKey[nNextMoveKey].aParts[nCntParts].pos.z - m_pPartsOld[nCntParts].pos.z) * (m_vecInfo[m_nType].aKey[nNextMoveKey].aParts[nCntParts].pos.z - m_pPartsOld[nCntParts].pos.z));
			fMoveDiff /= (static_cast<float>(nFrame) / static_cast<float>(fDeltaRate));

			// 動きの向きを一時代入
			float fRot = m_vecInfo[m_nType].aKey[nNextMoveKey].fRotMove;

			// 動きの向き方向へ移動
			pos.x += sinf(D3DX_PI + fRot + rot.y) * fMoveDiff;
			pos.z += cosf(D3DX_PI + fRot + rot.y) * fMoveDiff;

			m_pObjChara->SetPosition(pos);
		}
		else
		{
			// パーツの位置取得
			MyLib::Vector3 posParts = m_ppModel[nCntModel]->GetPosition();

			// 元の位置取得
			MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

			// 目標の位置との差分を求める
			float posDiffX = m_vecInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.x -
				m_pPartsOld[nCntParts].pos.x;

			float posDiffY = m_vecInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.y -
				m_pPartsOld[nCntParts].pos.y;

			float posDiffZ = m_vecInfo[m_nType].aKey[nNextKey].aParts[nCntParts].pos.z -
				m_pPartsOld[nCntParts].pos.z;

			// パーツの位置を設定
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

			// 位置設定
			m_ppModel[nCntModel]->SetPosition(posParts + m_ppModel[nCntModel]->GetOriginPosition());
		}
	}

	// フレームのカウントを加算
	m_fCntFrame += 1.0f * fDeltaRate * fSlowRate;
	m_fCntAllFrame += 1.0f * fDeltaRate * fSlowRate;
	m_bAllAfterAttack = false;		// 全ての攻撃後フラグ

	// 攻撃の最大フレーム
	float maxFrame = -1.0f;

	// 攻撃判定中フラグ設定
	for (int nCntAttack = 0; nCntAttack < m_vecInfo[m_nType].nNumAttackInfo; nCntAttack++)
	{
		AttackInfo* pAttack = m_vecInfo[m_nType].AttackInfo[nCntAttack];
		if (pAttack == nullptr)
		{
			continue;
		}

		if (pAttack->nMaxCnt == 0)
		{
			continue;
		}

		// 攻撃の最大フレーム
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

		// 攻撃判定
		if (m_fCntAllFrame >= static_cast<float>(pAttack->nMinCnt) && m_fCntAllFrame <= maxFrame)
		{
			m_bAttaking = true;
		}
	}


	if (m_vecInfo[m_nType].nCombolableFrame != -1 &&
		m_vecInfo[m_nType].nCombolableFrame <= m_fCntAllFrame)
	{// -1じゃなく、フレームを超えた時
		m_bCombiable = true;	// コンボ可能
	}

	if (m_vecInfo[m_nType].nCancelableFrame != -1 &&
		m_vecInfo[m_nType].nCancelableFrame <= m_fCntAllFrame)
	{// -1じゃなく、フレームを超えた時
		m_bCancelable = true;	// キャンセル可能
	}

	if (m_vecInfo[m_nType].nLoop == LOOP_ON)
	{// ループモーションはいつでも終わってる

		// 終了判定ON
		m_bFinish = true;
	}

	if (m_fCntFrame >= (float)nFrame)
	{// フレームのカウントがフレーム数に達したら

		// フレームのカウントをゼロに戻す
		m_fCntFrame = 0;

		m_nOldType = m_nType;	// 前回のモーションの種類

		// パターンNO.更新
		m_nPatternKey = (m_nPatternKey + 1) % m_vecInfo[m_nType].nNumKey;

		for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
		{// 全パーツ分繰り返す

			if (nCntParts >= m_nNumModel)
			{
				break;
			}

			m_pPartsOld[nCntParts].rot = m_vecInfo[m_nType].aKey[m_nPatternKey].aParts[nCntParts].rot;
			m_pPartsOld[nCntParts].pos = m_vecInfo[m_nType].aKey[m_nPatternKey].aParts[nCntParts].pos;
			m_pPartsOld[nCntParts].scale = m_vecInfo[m_nType].aKey[m_nPatternKey].aParts[nCntParts].scale;
		}

		if (m_nPatternKey == 0)
		{// キーがゼロの時

			// 総フレーム数リセット
			m_fCntAllFrame = 0.0f;

			//モーションの設定
			if (m_vecInfo[m_nType].nLoop == LOOP_OFF)
			{// ループモーションじゃない時

				// 終了判定ON
				m_bFinish = true;
			}
			else
			{
				for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
				{
					if (m_vecInfo[m_nType].AttackInfo[nCntAttack] == nullptr)
					{// nullptrだったら
						continue;
					}

					if (m_vecInfo[m_nType].AttackInfo[nCntAttack]->nInpactCnt < 0)
					{
						continue;
					}

					// まだ衝撃カウントの行動をしてない状態にする
					m_vecInfo[m_nType].AttackInfo[nCntAttack]->bInpactAct = false;
					m_vecInfo[m_nType].AttackInfo[nCntAttack]->bInpactActSet = false;
					m_vecInfo[m_nType].AttackInfo[nCntAttack]->bEndAtk = false;
				}
			}
		}
	}
}

//==========================================================================
// モーションの設定処理
//==========================================================================
void CMotion::Set(int nType, bool bBlend)
{
	// デバッグ用処理
	if (nType >= m_nNumAll)
	{// 総数を超えていたら
		return;
	}

	if (m_nType == nType)
	{// 現在の種類と同じだったら
		return;
	}

	m_nOldType = m_nType;	// 前回のモーションの種類
	m_nType = nType;		// 種類設定
	m_nPatternKey = 0;		// 何個目のキーか
	m_fCntFrame = 0.0f;		// フレームのカウント
	m_fCntAllFrame = 0.0f;	// 全てのカウント
	m_fMaxAllFrame = 0.0f;	// 全てのカウントの最大値
	m_bFinish = false;		// 終了したかどうか
	m_bCancelable = false;	// キャンセル可能か
	m_bCombiable = false;	// コンボ可能か
	m_bBeforeInAction = true;		// 攻撃前フラグ
	m_bAttaking = false;			// 攻撃判定中フラグ
	m_bAllAfterAttack = false;		// 全ての攻撃後フラグ

	for (int nCntKey = 0; nCntKey < m_vecInfo[m_nPatternKey].nNumKey; nCntKey++)
	{
		m_fMaxAllFrame += m_vecInfo[m_nPatternKey].aKey[nCntKey].nFrame;	// 全てのカウントの最大値
	}

	// 攻撃情報の総数取得
	int nNumAttackInfo = m_vecInfo[m_nType].nNumAttackInfo;
	for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
	{
		if (m_vecInfo[m_nType].AttackInfo[nCntAttack] == nullptr)
		{// nullptrだったら
			continue;
		}

		// まだ衝撃カウントの行動をしてない状態にする
		m_vecInfo[m_nType].AttackInfo[nCntAttack]->bInpactAct = false;
		m_vecInfo[m_nType].AttackInfo[nCntAttack]->bInpactActSet = false;

		// 攻撃のフラグリセット
		m_vecInfo[m_nType].AttackInfo[nCntAttack]->bEndAtk = false;
	}

	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{// 全パーツ分繰り返す

		if (m_ppModel[nCntParts] == nullptr)
		{
			continue;
		}

		// 過去の位置・向きを保存
		if (bBlend)
		{
			m_pPartsOld[nCntParts].rot = m_ppModel[nCntParts]->GetRotation();
			m_pPartsOld[nCntParts].scale = m_ppModel[nCntParts]->GetScale();

			if (nCntParts == 0)
			{
				m_pPartsOld[nCntParts].pos = m_ppModel[nCntParts]->GetPosition() - m_pObjChara->GetOriginPosition();
			}
			else
			{
				m_pPartsOld[nCntParts].pos = m_ppModel[nCntParts]->GetPosition() - m_ppModel[nCntParts]->GetOriginPosition();
			}
		}
		else
		{
			m_pPartsOld[nCntParts].rot = m_vecInfo[m_nType].aKey[0].aParts[nCntParts].rot;

			if (nCntParts == 0)
			{
				m_pPartsOld[nCntParts].pos = m_vecInfo[m_nType].aKey[0].aParts[nCntParts].pos + m_pObjChara->GetOriginPosition() - m_ppModel[nCntParts]->GetOriginPosition();
			}
			else
			{
				m_pPartsOld[nCntParts].pos = m_vecInfo[m_nType].aKey[0].aParts[nCntParts].pos;
			}

			m_pPartsOld[nCntParts].scale = m_vecInfo[m_nType].aKey[0].aParts[nCntParts].scale;
		}
	}
}

//==========================================================================
// 現在のモーションタイプ取得
//==========================================================================
int CMotion::GetType()
{
	return m_nType;
}

//==========================================================================
// 前回のモーションタイプ取得
//==========================================================================
int CMotion::GetOldType()
{
	return m_nOldType;
}

//==========================================================================
// 終了しているかの判定
//==========================================================================
bool CMotion::IsFinish()
{
	return m_bFinish;
}

//==========================================================================
// 終了しているかの判定切り替え
//==========================================================================
void CMotion::ToggleFinish(bool bFinish)
{
	m_bFinish = bFinish;
}

//==========================================================================
// 遅延係数の設定
//==========================================================================
void CMotion::SetSlowFactor(float fFactor)
{
	m_fSlowFactor = fFactor;
}

//==========================================================================
// 移動の判定取得
//==========================================================================
int CMotion::IsGetMove(int nType)
{
	return m_vecInfo[nType].nMove;
}

//==========================================================================
// モーション情報の設定
//==========================================================================
void CMotion::SetInfoData(int nType, Info info)
{
	m_vecInfo[nType] = info;
}

//==========================================================================
// モーション情報の取得
//==========================================================================
CMotion::Info CMotion::GetInfo(int nType)
{
	return m_vecInfo[nType];
}

//==========================================================================
// 現在のモーション情報の取得
//==========================================================================
CMotion::Info CMotion::GetNowInfo()
{
	return m_vecInfo[m_nType];
}

//==========================================================================
// モーション情報の取得
//==========================================================================
CMotion::Info *CMotion::GetInfoPtr(int nType)
{
	return &m_vecInfo[nType];
}

//==========================================================================
// モーション情報の登録
//==========================================================================
void CMotion::SetInfoSave(int nType, int nKey, int nParts, MyLib::Vector3 rot)
{
	m_vecInfo[nType].aKey[nKey].aParts[nParts].rot = rot;
}

//==========================================================================
// キー総数変更
//==========================================================================
void CMotion::ChangeKeyNum(int nType, int nNum)
{
	m_vecInfo[nType].nNumKey = nNum;
}

//==========================================================================
// キーフレーム変更
//==========================================================================
void CMotion::ChangeKeyFrame(int nType, int nKey, int nNum)
{
	m_vecInfo[nType].aKey[nKey].nFrame = nNum;
}

//==========================================================================
// 攻撃情報変更
//==========================================================================
void CMotion::ChangeAttackInfo(int nType, int nIdx, AttackInfo info)
{
	*m_vecInfo[nType].AttackInfo[nIdx] = info;
}

//==========================================================================
// 攻撃情報の総数加算
//==========================================================================
void CMotion::AddNumAttackInfo(int nType)
{
	if (m_vecInfo[nType].nNumAttackInfo >= MAX_PARTS)
	{
		return;
	}

	// メモリ確保
	m_vecInfo[nType].AttackInfo[m_vecInfo[nType].nNumAttackInfo] = DEBUG_NEW AttackInfo;

	if (m_vecInfo[nType].AttackInfo[m_vecInfo[nType].nNumAttackInfo] != nullptr)
	{// nullptrじゃなければ

		// 初期化
		memset(m_vecInfo[nType].AttackInfo[m_vecInfo[nType].nNumAttackInfo], 0, sizeof(AttackInfo));

		// 攻撃情報の総数加算
		m_vecInfo[nType].nNumAttackInfo++;
	}
}

//==========================================================================
// 攻撃情報の総数減算
//==========================================================================
void CMotion::SubNumAttackInfo(int nType)
{
	if (m_vecInfo[nType].nNumAttackInfo <= 1)
	{
		return;
	}
	int nIdx = m_vecInfo[nType].nNumAttackInfo - 1;

	// メモリ解放
	//if (m_vecInfo[nType].AttackInfo[nIdx] != nullptr)
	{
		delete m_vecInfo[nType].AttackInfo[nIdx];
		m_vecInfo[nType].AttackInfo[nIdx] = nullptr;

		// 攻撃情報の総数減算
		m_vecInfo[nType].nNumAttackInfo--;
	}
}

//==========================================================================
// 攻撃情報の総数減算
//==========================================================================
CMotion::Parts CMotion::GetPartsOld(int nParts)
{ 
	return m_pPartsOld[nParts];
}

//==========================================================================
// 攻撃情報の総数減算
//==========================================================================
void CMotion::SetPartsOld(int nParts, Parts parts)
{ 
	m_pPartsOld[nParts] = parts;
}

//==========================================================================
// 衝撃のフレームかどうか取得
//==========================================================================
bool CMotion::IsImpactFrame(AttackInfo attackInfo)
{
	return attackInfo.bInpactAct;
}

//==========================================================================
// 攻撃の位置取得
//==========================================================================
MyLib::Vector3 CMotion::GetAttackPosition(CModel **ppModel, AttackInfo attackInfo)
{
	D3DXMATRIX mtxTrans;	// 計算用マトリックス宣言

	// 総数制御
	if (attackInfo.nCollisionNum >= m_nNumModel) return MyLib::Vector3();

	if (ppModel[attackInfo.nCollisionNum] == nullptr) return MyLib::Vector3();

	// 判定するパーツのマトリックス取得
	D3DXMATRIX mtxWepon = ppModel[attackInfo.nCollisionNum]->GetWorldMtx().ConvertD3DXMATRIX();

	// 位置を反映する
	D3DXMatrixTranslation(&mtxTrans, attackInfo.Offset.x, attackInfo.Offset.y, attackInfo.Offset.z);
	D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

	return MyLib::Vector3(mtxWepon._41, mtxWepon._42, mtxWepon._43);
}

//==========================================================================
// 攻撃の位置取得
//==========================================================================
MyLib::Vector3 CMotion::GetAttackPosition(CModel *pModel, AttackInfo attackInfo)
{
	D3DXMATRIX mtxTrans;	// 計算用マトリックス宣言

	if (pModel == nullptr)
	{// nullptrだったら
		return MyLib::Vector3();
	}

	// 判定するパーツのマトリックス取得
	D3DXMATRIX mtxWepon = pModel->GetWorldMtx().ConvertD3DXMATRIX();

	// 位置を反映する
	D3DXMatrixTranslation(&mtxTrans, attackInfo.Offset.x, attackInfo.Offset.y, attackInfo.Offset.z);
	D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

	return MyLib::Vector3(mtxWepon._41, mtxWepon._42, mtxWepon._43);
}

//==========================================================================
// フレームカウント設定
//==========================================================================
void CMotion::SetFrameCount(float fCnt)
{
	m_fCntFrame = fCnt;
}

//==========================================================================
// カウント取得
//==========================================================================
float CMotion::GetAllCount()
{
	return m_fCntAllFrame;
}

//==========================================================================
// 全てのカウント取得
//==========================================================================
int CMotion::GetMaxAllCount(int nType)
{
	int nAllFrame = 0;
	for (int nCntKey = 0; nCntKey < m_vecInfo[nType].nNumKey; nCntKey++)
	{
		nAllFrame += m_vecInfo[nType].aKey[nCntKey].nFrame;	// 全てのカウントの最大値
	}
	return nAllFrame;
}

//==========================================================================
// 外部ファイル読み込み
//==========================================================================
void CMotion::ReadText(const std::string& file)
{
	// 読み込み確認
	std::vector<std::string>::iterator itr = std::find(m_sTextFile.begin(), m_sTextFile.end(), file);
	if (itr != m_sTextFile.end())
	{// ファイル名が一致

		// インデックス算出
		int nIdx = itr - m_sTextFile.begin();

		// モーション数
		m_nNumMotion = m_nNumLoadData[nIdx];

		// モーション情報渡す
		m_vecInfo = m_vecLoadData[nIdx];

		// 攻撃情報渡す

		for (int nCntInfo = 0; nCntInfo < m_nNumMotion; nCntInfo++)
		{
			// モーション情報
			Info motionInfo = m_vecLoadData[nIdx][nCntInfo];

			for (int nCntATK = 0; nCntATK < motionInfo.nNumAttackInfo; nCntATK++)
			{// 攻撃情報分繰り返す

				// 攻撃情報登録
				SetAttackInfo(m_aLoadAttackData[nIdx][nCntInfo][nCntATK]);
			}

			// 情報登録
			SetInfo(motionInfo);
		}
		return;
	}

	// ファイル名保存
	m_sTextFile.push_back(file);


	// ファイルを開く
	FILE* pFile = fopen(file.c_str(), "r");
	if (pFile == nullptr)
	{//ファイルが開けなかった場合
		return;
	}

	// 読み込みのデータ数加算
	m_nNumLoadData.push_back(0);


	// 読み込み用変数
	char aComment[MAX_COMMENT];	// コメント用
	int nCntMotion = 0;
	int nCntFile = 0;	// ファイルの読み込み回数
	std::vector<std::string> motionFile;	// モーションファイル

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// モーション数の設定
		if (strcmp(aComment, "NUM_MOTION") == 0)
		{// NUM_MOTIONがきたら

			fscanf(pFile, "%s", &aComment[0]);	// =の分
			fscanf(pFile, "%d", &m_nNumMotion);	// モーション数読み込み

			// モーションの数分確保
			m_vecLoadData.back();

			while (nCntFile != m_nNumMotion)
			{// モーション数分読み込むまで繰り返し

				// 文字列の読み込み
				fscanf(pFile, "%s", &aComment[0]);

				// モーションファイルの読み込み
				if (strcmp(aComment, "MOTION_FILENAME") == 0)
				{// MOTION_FILENAMEがきたら

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%s", &aComment[0]);	// ファイル名

					// ファイル名保存
					motionFile.push_back(&aComment[0]);

					nCntFile++;	// ファイルの読み込み回数加算
				}
			}

			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	// 要素分繰り返し
	for (int i = 0; i < static_cast<int>(motionFile.size()); i++)
	{
		// モーション読み込み
		LoadMotion(motionFile[i], i);
	}

	// 読み込んだ数加算
	m_nNumLoad++;
}

//==========================================================================
// モーション読み込み
//==========================================================================
void CMotion::LoadMotion(const std::string& file, int nIdxMotion)
{

	// ファイルを開く
	FILE* pFile = fopen(file.c_str(), "r");
	if (pFile == nullptr)
	{//ファイルが開けなかった場合
		return;
	}

	// 読み込み用変数
	char aComment[MAX_COMMENT];
	int nCntKey = 0, nCntParts = 0;
	Info InitInfo = {};
	AttackInfo InitAttackInfo = {};

	// キャンセル可能フレームリセット
	InitInfo.nCancelableFrame = -1;
	InitInfo.nCombolableFrame = -1;

	// 読み込み用データ
	Info loadInfo = InitInfo;

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		//==================================================
		// 各モーションの設定
		//==================================================
		if (strcmp(aComment, "MOTIONSET") == 0)
		{// モーション情報の読み込みを開始

			while (strcmp(aComment, "END_MOTIONSET") != 0)
			{// END_MOTIONSETが来るまで繰り返し

				fscanf(pFile, "%s", &aComment[0]);	// 確認する

				if (strcmp(aComment, "LOOP") == 0)
				{// LOOPが来たらループON/OFF読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &loadInfo.nLoop);	// ループ0か1
				}

				if (strcmp(aComment, "NUM_KEY") == 0)
				{// NUM_KEYが来たらキー数読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &loadInfo.nNumKey);	// キー数
				}

				if (strcmp(aComment, "MOVE") == 0)
				{// MOVEが来たら移動ON/OFF読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &loadInfo.nMove);	// 移動0か1か
				}

				if (strcmp(aComment, "CANCELABLE") == 0)
				{// CANCELABLEでキャンセル可能フレーム読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &loadInfo.nCancelableFrame);	// キャンセル可能フレーム
				}

				if (strcmp(aComment, "COMBOLABLE") == 0)
				{// COMBOLABLEでコンボ可能フレーム読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &loadInfo.nCombolableFrame);	// コンボ可能フレーム
				}

				if (strcmp(aComment, "ATTACKINFO") == 0)
				{// ATTACKINFOが来たら

					// 読み込み用変数
					AttackInfo loadAttackInfo;

					while (strcmp(aComment, "END_ATTACKINFO") != 0)
					{// END_ATTACKINFOが来るまで繰り返し

						fscanf(pFile, "%s", &aComment[0]);	// 確認する

						if (strcmp(aComment, "COLLISIONPARTS") == 0)
						{// COLLISIONPARTSが来たらパーツ番号読み込み

							fscanf(pFile, "%s", &aComment[0]);		// =の分
							fscanf(pFile, "%d", &loadAttackInfo.nCollisionNum);	// 判定を取るパーツ番号
						}

						if (strcmp(aComment, "ATTACKRANGE") == 0)
						{// ATTACKRANGEが来たら攻撃サイズ読み込み

							fscanf(pFile, "%s", &aComment[0]);		// =の分
							fscanf(pFile, "%f", &loadAttackInfo.fRangeSize);	// 判定のサイズ
						}

						if (strcmp(aComment, "OFFSET") == 0)
						{// OFFSETが来たらオフセット読み込み

							fscanf(pFile, "%s", &aComment[0]);		// =の分
							fscanf(pFile, "%f", &loadAttackInfo.Offset.x);	// X座標
							fscanf(pFile, "%f", &loadAttackInfo.Offset.y);	// Y座標
							fscanf(pFile, "%f", &loadAttackInfo.Offset.z);	// Z座標
						}

						if (strcmp(aComment, "ATTACKCOUNT") == 0)
						{// ATTACKCOUNTが来たら攻撃カウント読み込み

							fscanf(pFile, "%s", &aComment[0]);		// =の分
							fscanf(pFile, "%d", &loadAttackInfo.nMinCnt);	// 判定の最小カウント
							fscanf(pFile, "%d", &loadAttackInfo.nMaxCnt);	// 判定の最大カウント
						}

						if (strcmp(aComment, "INPACTCOUNT") == 0)
						{// INPACTCOUNTが来たら衝撃カウント読み込み

							fscanf(pFile, "%s", &aComment[0]);		// =の分
							fscanf(pFile, "%d", &loadAttackInfo.nInpactCnt);	// 衝撃のカウント
						}

						if (strcmp(aComment, "DAMAGE") == 0)
						{// DAMAGEが来たら攻撃力読み込み

							fscanf(pFile, "%s", &aComment[0]);		// =の分
							fscanf(pFile, "%d", &loadAttackInfo.nDamage);	// 攻撃力
						}
					}

					// 攻撃の情報追加
					loadInfo.AttackInfo.push_back(loadAttackInfo);
				}

				//==================================================
				// 各キーの設定
				//==================================================
				if (strcmp(aComment, "KEYSET") == 0)
				{// KEYSETでキー情報の読み込み開始

					// 読み込み用データ
					Key loadKey;

					while (strcmp(aComment, "END_KEYSET") != 0)
					{// END_KEYSETが来るまで繰り返し

						fscanf(pFile, "%s", &aComment[0]);	// 確認する

						if (strcmp(aComment, "FRAME") == 0)
						{// FRAMEが来たら再生フレーム数読み込み

							fscanf(pFile, "%s", &aComment[0]);		// =の分
							fscanf(pFile, "%d", &loadKey.nFrame);	// 再生フレーム
						}

						//==================================================
						// パーツの設定
						//==================================================
						if (strcmp(aComment, "PARTS") == 0)
						{// PARTSでパーツ情報の読み込み開始

							// 読み込み用データ
							Parts loadParts;

							while (strcmp(aComment, "END_PARTS") != 0)
							{// END_PARTSが来るまで繰り返し

								fscanf(pFile, "%s", &aComment[0]);	// 確認する

								if (strcmp(aComment, "POS") == 0)
								{// POSが来たら位置読み込み

									fscanf(pFile, "%s", &aComment[0]);		// =の分
									fscanf(pFile, "%f", &loadParts.pos.x);	// X座標
									fscanf(pFile, "%f", &loadParts.pos.y);	// Y座標
									fscanf(pFile, "%f", &loadParts.pos.z);	// Z座標
								}

								if (strcmp(aComment, "ROT") == 0)
								{// ROTが来たら角度読み込み

									fscanf(pFile, "%s", &aComment[0]);		// =の分
									fscanf(pFile, "%f", &loadParts.rot.x);	// X角度
									fscanf(pFile, "%f", &loadParts.rot.y);	// Y角度
									fscanf(pFile, "%f", &loadParts.rot.z);	// Z角度
								}

								if (strcmp(aComment, "SCALE") == 0)
								{// SCALEが来たらスケール読み込み

									fscanf(pFile, "%s", &aComment[0]);		// =の分
									fscanf(pFile, "%f", &loadParts.scale.x);	// Xスケール
									fscanf(pFile, "%f", &loadParts.scale.y);	// Yスケール
									fscanf(pFile, "%f", &loadParts.scale.z);	// Zスケール
								}
							}// END_PARTSのかっこ

							// パーツデータ追加
							loadKey.aParts.push_back(loadParts);
						}
					}// END_KEYSETのかっこ

					// キーデータ追加
					loadInfo.aKey.push_back(loadKey);
				}

			}// END_MOTIONSETのかっこ

			// モーションの情報追加
			vecLoadInfo.push_back(loadInfo);


			// モーション情報の登録
			SetInfo(m_vecLoadData[m_nNumLoad][nIdxMotion]);

			if (nIdxMotion == 0)
			{
				m_fMaxAllFrame = 0;
				for (int nCntKey = 0; nCntKey < m_vecInfo[m_nPatternKey].nNumKey; nCntKey++)
				{
					m_fMaxAllFrame += m_vecInfo[m_nPatternKey].aKey[nCntKey].nFrame;	// 全てのカウントの最大値
				}
			}

			nCntKey = 0;	// キーのカウントリセット
			nIdxMotion++;	// モーションのカウント加算

			// モーション毎のデータ数
			m_nNumLoadData[m_nNumLoad]++;
		}

		if (strcmp(aComment, "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける

			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);


	//-----------------------------
	// 読み込み後の計算
	//-----------------------------
	// キーの数
	loadInfo.nNumKey = static_cast<int>(loadInfo.aKey.size());

	// 攻撃情報の数
	loadInfo.nNumAttackInfo = static_cast<int>(loadInfo.AttackInfo.size());

	for (int i = 0; loadInfo.nNumKey; i++)
	{
		// 1つ前のキーインデックス
		int nBeforeCnt = UtilFunc::Transformation::Clamp(i - 1, 0, loadInfo.nNumKey);

		// 1つ前の位置
		float PosX = loadInfo.aKey[nBeforeCnt].aParts[0].pos.x;
		float PosZ = loadInfo.aKey[nBeforeCnt].aParts[0].pos.z;

		// 向きを求める
		loadInfo.aKey[i].fRotMove = atan2f(
			(PosX - loadInfo.aKey[i].aParts[0].pos.x),
			(PosZ - loadInfo.aKey[i].aParts[0].pos.z));
	}

	// 読み込みデータ渡す
	m_vecLoadData.back().push_back(loadInfo);





















	// ファイルを開く
	std::ifstream File(file);
	if (!File.is_open())
	{//ファイルが開けなかった場合
		return;
	}

	// コメント用
	std::string hoge;

	// データ読み込み
	std::string line;
	while (std::getline(File, line))
	{
		// コメントはスキップ
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("BAGGAGESET") != std::string::npos)
		{// MODELSETで配置情報読み込み

			// 読み込み情報
			CBaggage::SBaggageInfo info;

			while (line.find("END_BAGGAGESET") == std::string::npos)
			{
				std::getline(File, line);
				if (line.find("MODEL") != std::string::npos)
				{// TYPEで配置物の種類

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>
						hoge >>		// ＝
						info.path;	// パス
					info.path = UtilFunc::Transformation::ReplaceBackslash(info.path);
				}
				else if (line.find("WEIGHT") != std::string::npos)
				{// POSで位置

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>
						hoge >>			// ＝
						info.weight;	// 重さ
				}
				else if (line.find("WIDTH") != std::string::npos)
				{// ROTで向き

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>
						hoge >>		// ＝
						info.width;	// ぶれ幅
				}
				else if (line.find("CYCLE") != std::string::npos)
				{// ROTで向き

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>
						hoge >>		// ＝
						info.cycle;	// 周期
				}
				else if (line.find("LIFE") != std::string::npos)
				{// LIFEで体力

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>
						hoge >>		// ＝
						info.life;	// 周期
				}
			}

			// 取り込み
			m_BaggageInfo.push_back(info);
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// ファイルを閉じる
	File.close();


}
