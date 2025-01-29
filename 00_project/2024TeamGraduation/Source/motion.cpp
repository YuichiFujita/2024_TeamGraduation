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
// 定数定義
//==========================================================================
namespace
{
	const int IDX_VISUALMOVE = 2;	// 見た目のみ移動
}

//==========================================================================
// コンストラクタ
//==========================================================================
CMotion::CMotion()
{
	// 値のクリア
	m_nType = 0;			// 現在のモーションの種類
	m_nOldType = 0;			// 前回のモーションの種類
	m_bLoop = false;		// ループするかどうか
	m_nPatternKey = 0;		// 何個目のキーか
	m_fCntFrame = 0.0f;		// フレームのカウント
	m_fAllFrame = 0.0f;	// 全てのカウント
	m_fMaxAllFrame = 0.0f;	// 全てのカウントの最大値
	m_fSlowFactor = 0.0f;	// 遅延係数
	m_bFinish = false;		// 終了したかどうか
	m_bCancelable = false;	// キャンセル可能か
	m_bCombiable = false;	// コンボ可能か
	m_bAttaking = false;	// 攻撃判定中フラグ
	m_bLoopMomnet = false;	// ループした瞬間
	m_bMotion = true;		// モーション再生フラグ

	m_pObjChara = nullptr;		// オブジェクトのポインタ
	m_ppModel = nullptr;		// モデルのポインタ
	m_nNumModel = 0;			// モデルの総数
	m_nNumMotion = 0;			// モーションの総数
	m_vecDefaultIdx.clear();	// デフォルトのインデックス
	m_vecInfo.clear();			// モーション情報
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
		pMotion->Init(file);
	}

	return pMotion;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMotion::Init(const std::string& file)
{
	m_nType = 0;			// 現在のモーションの種類
	m_bLoop = false;		// ループするかどうか
	m_nPatternKey = 0;		// 何個目のキーか
	m_fCntFrame = 0.0f;		// フレームのカウント
	m_fAllFrame = 0.0f;	// 全てのカウント
	m_fMaxAllFrame = 0.0f;	// 全てのカウントの最大値
	m_bFinish = true;		// 終了したかどうか
	m_ppModel = nullptr;		// モデルのポインタ
	m_nNumModel = 0;		// モデルの総数
	m_fSlowFactor = 1.0f;	// 遅延係数
	m_pPartsOld = nullptr;
	m_vecDefaultIdx.push_back(0);	// デフォルトのインデックス

	// モーション読み込み
	CMotionManager* pMotionManager = CMotionManager::GetInstance();
	if (pMotionManager == nullptr)
	{// 要素ない場合生成
		CMotionManager::Create();
	}
	const CMotionManager::SLoadInfo& loadInfo = pMotionManager->Load(file);

	// 読み込み情報取得
	m_vecDefaultIdx = loadInfo.vecDefaultIdx;						// デフォルトのインデックス
	m_vecInfo = loadInfo.vecLoadData;								// モーション情報
	m_nNumMotion = static_cast<int>(loadInfo.vecLoadData.size());	// モーション数

	return S_OK;
}

//==========================================================================
// 終了処理
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
// モーションをするモデルの登録
//==========================================================================
void CMotion::SetModel(CModel** pModel, int nNumModel)
{
	// モデルのポインタを渡す
	m_ppModel = pModel;

	// モデルの総数
	m_nNumModel = nNumModel;

	m_pPartsOld = DEBUG_NEW CMotionManager::Parts[m_nNumModel];
	if (m_pPartsOld == nullptr)
	{
		return;
	}
	memset(m_pPartsOld, 0, sizeof(CMotionManager::Parts) * m_nNumModel);
}

//==========================================================================
// ポーズのリセット
//==========================================================================
void CMotion::ResetPose(int nType)
{
	// パーツ情報
	const CMotionManager::Info& info = m_vecInfo[nType];
	const CMotionManager::Key& keyInfo = info.aKey[0];

	int partsSize = static_cast<int>(info.aKey[0].aParts.size());
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{// 全パーツ分繰り返す

		// モデルがないと戻る
		if (m_ppModel[nCntParts] == nullptr) continue;

		// パーツ情報がないと戻る
		if (partsSize <= nCntParts) continue;

		// 向き設定
		m_ppModel[nCntParts]->SetRotation(keyInfo.aParts[nCntParts].rot);
		m_pPartsOld[nCntParts].rot = keyInfo.aParts[nCntParts].rot;
		m_pPartsOld[nCntParts].scale = keyInfo.aParts[nCntParts].scale;

		// 元の位置取得
		MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

		// 位置設定
		if (nCntParts == 0)
		{// 親の場合はキャラクターの位置
			m_ppModel[nCntParts]->SetPosition(keyInfo.aParts[nCntParts].pos + posOrigin);
		}
		else
		{
			m_ppModel[nCntParts]->SetPosition(keyInfo.aParts[nCntParts].pos + m_ppModel[nCntParts]->GetOriginPosition());
		}
	}

	// 過去の位置も保存
 	m_pPartsOld[0].pos = m_vecInfo[nType].aKey[0].aParts[0].pos;

	// モーション設定
	Set(nType);
}

//==========================================================================
// 更新処理
//==========================================================================
void CMotion::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_bMotion) { return; }

	if (m_vecInfo[0].aKey.size() == 0)
	{// モーション情報メモリリーク？
		MyAssert::CustomAssert(false,"観客壊れる可能性あり");
	}

	// 再生中の情報
	CMotionManager::Info& nowInfo = m_vecInfo[m_nType];

	// 攻撃判定中フラグリセット
	m_bAttaking = false;

	// ループした瞬間
	m_bLoopMomnet = false;

	// 攻撃情報の総数取得
	int nNumAttackInfo = nowInfo.nNumAttackInfo;

	for (auto& attackInfo : nowInfo.AttackInfo)
	{
		//--------------------------
		// 攻撃情報
		//--------------------------
		// まだ衝撃カウントの行動をしてない状態にする
		if (attackInfo.nInpactCnt != 0 || m_fAllFrame > 0.0f)
		{
			attackInfo.bInpactAct = false;
		}

		if (attackInfo.nInpactCnt >= 0 &&
			!attackInfo.bInpactActSet &&
			m_fAllFrame >= static_cast<float>(attackInfo.nInpactCnt))
		{// 衝撃のカウントを超えた時 && まだ行動してなかったら

			// 行動してる状態にして、設定完了
			attackInfo.bInpactAct = true;
			attackInfo.bInpactActSet = true;
		}


		//--------------------------
		// 揃え情報
		//--------------------------
		attackInfo.AlignInfo.bSet = false;

		if (attackInfo.AlignInfo.nFrame >= 0 &&
			m_fAllFrame >= static_cast<float>(attackInfo.AlignInfo.nFrame) &&
			m_fAllFrame <= static_cast<float>(attackInfo.AlignInfo.nFrame + attackInfo.AlignInfo.nExtensionFrame))
		{// フレーム内
			attackInfo.AlignInfo.bSet = true;
		}
	}

	if (m_bFinish && nowInfo.nLoop == LOOP_OFF)
	{// 終了してた && ループOFFだったら
		return;
	}


	// 再生中のキー
	const CMotionManager::Key& nowKey = nowInfo.aKey[m_nPatternKey];

	// 再生フレーム保存
	int nMaxFrame = nowKey.nFrame;

	// デフォルトモーション判定
	bool bDefault = std::find(m_vecDefaultIdx.begin(), m_vecDefaultIdx.end(), m_nType) != m_vecDefaultIdx.end();

	if (bDefault && m_nOldType != 0 && m_nPatternKey == 0)
	{// ニュートラルで0個の時
		nMaxFrame = 10;
	}

	// 次のキー
	int nNextKey = (m_nPatternKey + 1) % nowInfo.nNumKey;

	if (nNextKey == 0 && nowInfo.nLoop == LOOP_OFF)
	{// ループしないとき

		// 最後で固定
		nNextKey = nowInfo.nNumKey - 1;
	}

	//--------------------------
	// 再生フレームとの割合
	//--------------------------
	float ratio = m_fCntFrame / static_cast<float>(nMaxFrame);
	ratio = UtilFunc::Transformation::Clamp(ratio, 0.0f, 1.0f);

	for (int i = 0; i < static_cast<int>(nowKey.aParts.size()); i++)
	{
		// 総数を上回ったら
		if (i >= m_nNumModel) continue;

		// 現在のパーツ情報
		const CMotionManager::Parts& nowParts = nowKey.aParts[i];
		const CMotionManager::Parts& nextParts = nowInfo.aKey[nNextKey].aParts[i];

		// モデルがなかったら戻る
		if (m_ppModel[i] == nullptr) continue;

		//--------------------------
		// 向き更新
		//--------------------------
		UpdateRotation(i, nowParts, nextParts, ratio);

		//--------------------------
		// スケール更新
		//--------------------------
		UpdateScale(i, nowParts, nextParts, ratio);

		//--------------------------
		// 位置反映
		//--------------------------
		UpdatePosition(i, nowInfo, nowParts, nextParts, ratio, nMaxFrame, fDeltaTime, fDeltaRate, fSlowRate);

	}

	// フレームのカウントを加算
	m_fCntFrame += 1.0f * fDeltaRate * fSlowRate;
	m_fAllFrame += 1.0f * fDeltaRate * fSlowRate;

	// 攻撃判定中フラグ設定
	float fMinCnt, fMaxCnt;
	for (auto& attackInfo : nowInfo.AttackInfo)
	{
		// 攻撃中リセット
		attackInfo.bAtkking = false;

		if (attackInfo.nMaxCnt == 0)
		{// 最大値が0のはなにもしない
			continue;
		}

		// 最小値と最大値
		fMinCnt = static_cast<float>(attackInfo.nMinCnt);
		fMaxCnt = static_cast<float>(attackInfo.nMaxCnt);

		// 判定の割合
		attackInfo.fCntRatio = UtilFunc::Transformation::Clamp(
			(m_fAllFrame - fMinCnt) / (fMaxCnt - fMinCnt),
			0.0f, 1.0f);

		// 攻撃判定
		if (m_fAllFrame >= fMinCnt &&
			m_fAllFrame <= fMaxCnt)
		{// 最小値と最大値の中
			m_bAttaking = true;
			attackInfo.bAtkking = true;	// 攻撃中
		}
	}

	//--------------------------
	// コンボフレーム
	//--------------------------
	if (nowInfo.nCombolableFrame != -1 &&
		nowInfo.nCombolableFrame <= m_fAllFrame)
	{// -1じゃなく、フレームを超えた時
		m_bCombiable = true;	// コンボ可能
	}

	//--------------------------
	// キャンセルフレーム
	//--------------------------
	if (nowInfo.nCancelableFrame != -1 &&
		nowInfo.nCancelableFrame <= m_fAllFrame)
	{// -1じゃなく、フレームを超えた時
		m_bCancelable = true;	// キャンセル可能
	}

	if (nowInfo.nLoop == LOOP_ON)
	{// ループモーションはいつでも終わってる

		// 終了判定ON
		m_bFinish = true;
	}

	//--------------------------
	// キー更新
	//--------------------------
	if (m_fCntFrame >= static_cast<float>(nMaxFrame))
	{// フレームのカウントがフレーム数に達したら

		// フレームのカウントをゼロに戻す
		m_fCntFrame = 0;

		m_nOldType = m_nType;	// 前回のモーションの種類

		// パターンNO.更新
		m_nPatternKey = (m_nPatternKey + 1) % nowInfo.nNumKey;

		// パーツサイズ
		int partsSize = static_cast<int>(nowInfo.aKey[m_nPatternKey].aParts.size());
		for (int i = 0; i < m_nNumModel; i++)
		{// 全パーツ分繰り返す

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
		{// 1週した

			// ループした瞬間
			m_bLoopMomnet = true;	

			// 総フレーム数リセット
			m_fAllFrame = 0.0f;

			//モーションの設定
			if (nowInfo.nLoop == LOOP_OFF)
			{// ループモーションじゃない時

				// 終了判定ON
				m_bFinish = true;
			}
			else
			{// ループモーション

				// 攻撃情報のフラグオフ
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
// 向きの更新
//==========================================================================
void CMotion::UpdateRotation(int i, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, float ratio)
{
	
	//--------------------------
	// 次と今の向きの差分取得
	//--------------------------
	float rotDiffX = nextParts.rot.x - m_pPartsOld[i].rot.x;
	float rotDiffY = nextParts.rot.y - m_pPartsOld[i].rot.y;
	float rotDiffZ = nextParts.rot.z - m_pPartsOld[i].rot.z;

	// 角度の正規化
	UtilFunc::Transformation::RotNormalize(rotDiffX);
	UtilFunc::Transformation::RotNormalize(rotDiffY);
	UtilFunc::Transformation::RotNormalize(rotDiffZ);

	//--------------------------
	// パーツの向きを設定
	//--------------------------
	MyLib::Vector3 rot;
	rot.x = m_pPartsOld[i].rot.x + (rotDiffX * ratio);
	rot.y = m_pPartsOld[i].rot.y + (rotDiffY * ratio);
	rot.z = m_pPartsOld[i].rot.z + (rotDiffZ * ratio);

	// 角度の正規化
	UtilFunc::Transformation::RotNormalize(rot);

	// 向き設定
	m_ppModel[i]->SetRotation(rot);
}

//==========================================================================
// スケールの更新
//==========================================================================
void CMotion::UpdateScale(int i, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, float ratio)
{
	//--------------------------
	// スケール
	//--------------------------
	// 次と今の向きの差分取得
	float scaleDiffX = nextParts.scale.x -
		m_pPartsOld[i].scale.x;

	float scaleDiffY = nextParts.scale.y -
		m_pPartsOld[i].scale.y;

	float scaleDiffZ = nextParts.scale.z -
		m_pPartsOld[i].scale.z;


	//--------------------------
	// スケール反映
	//--------------------------
	MyLib::Vector3 scale = MyLib::Vector3(0.0f, 0.0f, 0.0f);
	scale.x = m_pPartsOld[i].scale.x + (scaleDiffX * ratio);
	scale.y = m_pPartsOld[i].scale.y + (scaleDiffY * ratio);
	scale.z = m_pPartsOld[i].scale.z + (scaleDiffZ * ratio);

	// スケール設定
	m_ppModel[i]->SetScale(scale);
}

//==========================================================================
// 位置更新
//==========================================================================
void CMotion::UpdatePosition(
	int i, 
	const CMotionManager::Info& nowInfo, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, 
	float ratio, int nMaxFrame,
	const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (nowInfo.nMove == IDX_VISUALMOVE)
	{// 見た目のみ位置更新
		UpdateVisualPosition(i, nowInfo, nowParts, nextParts, ratio, nMaxFrame, fDeltaTime, fDeltaRate, fSlowRate);
	}
	else
	{
		UpdateEntityPosition(i, nowInfo, nowParts, nextParts, ratio, nMaxFrame, fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// 本体ごと位置更新
//==========================================================================
void CMotion::UpdateEntityPosition(
	int i, 
	const CMotionManager::Info& nowInfo, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, 
	float ratio, int nMaxFrame,
	const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 次のキー
	int nNextKey = (m_nPatternKey + 1) % nowInfo.nNumKey;
	if (nNextKey == 0 && nowInfo.nLoop == LOOP_OFF)
	{// ループしないとき

		// 最後で固定
		nNextKey = nowInfo.nNumKey - 1;
	}

	if (i == 0)
	{
		// 本体の位置取得
		MyLib::Vector3 pos = m_pObjChara->GetPosition();

		// 本体の向き取得
		MyLib::Vector3 rot = m_pObjChara->GetRotation();

		// 元の位置取得
		MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

		// パーツの位置取得
		MyLib::Vector3 posParts = m_ppModel[i]->GetPosition();
		MyLib::Vector3 posPartsOld = m_ppModel[i]->GetPosition();

		// 目標の位置との差分を求める
		MyLib::Vector3 destpos = nowInfo.aKey[nNextKey].aParts[i].pos;
		float posDiffX = destpos.x -
			m_pPartsOld[i].pos.x;

		float posDiffY = destpos.y -
			m_pPartsOld[i].pos.y;

		float posDiffZ = destpos.z -
			m_pPartsOld[i].pos.z;

		// 親のYを補正
		posParts.y =
			m_pPartsOld[i].pos.y + (posDiffY * ratio);
		
		// スケール分移動
		//posParts.y *= m_pObjChara->GetScale();

		// 位置設定
		m_ppModel[i]->SetPosition(posParts + posOrigin);

		int nNextMoveKey = m_nPatternKey + 1;
		if (nNextMoveKey >= nowInfo.nNumKey)
		{
			nNextMoveKey = m_nPatternKey;
		}

		// 動いた長さを求める
		float fMoveDiff =
			sqrtf((nowInfo.aKey[nNextMoveKey].aParts[i].pos.x - m_pPartsOld[i].pos.x) * (nowInfo.aKey[nNextMoveKey].aParts[i].pos.x - m_pPartsOld[i].pos.x)
				+ (nowInfo.aKey[nNextMoveKey].aParts[i].pos.z - m_pPartsOld[i].pos.z) * (nowInfo.aKey[nNextMoveKey].aParts[i].pos.z - m_pPartsOld[i].pos.z));
		fMoveDiff /= (static_cast<float>(nMaxFrame) / (fDeltaRate * fSlowRate));

		// 動きの向きを一時代入
		float fRot = nowInfo.aKey[nNextMoveKey].fRotMove;

		// 動きの向き方向へ移動
		pos.x += sinf(D3DX_PI + fRot + rot.y) * fMoveDiff;
		pos.z += cosf(D3DX_PI + fRot + rot.y) * fMoveDiff;

		// 位置設定
		m_pObjChara->SetPosition(pos);
	}
	else
	{
		// パーツの位置取得
		MyLib::Vector3 posParts = m_ppModel[i]->GetPosition();

		// 元の位置取得
		MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

		// 目標の位置との差分を求める
		float posDiffX = nextParts.pos.x - m_pPartsOld[i].pos.x;
		float posDiffY = nextParts.pos.y - m_pPartsOld[i].pos.y;
		float posDiffZ = nextParts.pos.z - m_pPartsOld[i].pos.z;

		// パーツの位置を設定
		posParts.y = m_pPartsOld[i].pos.y + (posDiffY * ratio);
		posParts.x = m_pPartsOld[i].pos.x + (posDiffX * ratio);
		posParts.z = m_pPartsOld[i].pos.z + (posDiffZ * ratio);

		// 位置設定
		m_ppModel[i]->SetPosition(posParts + m_ppModel[i]->GetOriginPosition());
	}
}

//==========================================================================
// 見た目のみ位置更新
//==========================================================================
void CMotion::UpdateVisualPosition(
	int i, 
	const CMotionManager::Info& nowInfo, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, 
	float ratio, int nMaxFrame,
	const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 次のキー
	int nNextKey = (m_nPatternKey + 1) % nowInfo.nNumKey;
	if (nNextKey == 0 && nowInfo.nLoop == LOOP_OFF)
	{// ループしないとき

		// 最後で固定
		nNextKey = nowInfo.nNumKey - 1;
	}

	if (i == 0)
	{
		// パーツの位置取得
		MyLib::Vector3 posParts = m_ppModel[i]->GetPosition();

		// 元の位置取得
		MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

		// 目標の位置との差分を求める
		float posDiffX = nextParts.pos.x - m_pPartsOld[i].pos.x;
		float posDiffY = nextParts.pos.y - m_pPartsOld[i].pos.y;
		float posDiffZ = nextParts.pos.z - m_pPartsOld[i].pos.z;

		// パーツの位置を設定
		posParts.y = m_pPartsOld[i].pos.y + (posDiffY * ratio);
		posParts.x = m_pPartsOld[i].pos.x + (posDiffX * ratio);
		posParts.z = m_pPartsOld[i].pos.z + (posDiffZ * ratio);

		// 位置設定
		m_ppModel[i]->SetPosition((posParts + m_ppModel[i]->GetOriginPosition()) * m_pObjChara->GetScale());
	}
	else
	{
		// パーツの位置取得
		MyLib::Vector3 posParts = m_ppModel[i]->GetPosition();

		// 元の位置取得
		MyLib::Vector3 posOrigin = m_pObjChara->GetOriginPosition();

		// 目標の位置との差分を求める
		float posDiffX = nextParts.pos.x - m_pPartsOld[i].pos.x;
		float posDiffY = nextParts.pos.y - m_pPartsOld[i].pos.y;
		float posDiffZ = nextParts.pos.z - m_pPartsOld[i].pos.z;

		// パーツの位置を設定
		posParts.y = m_pPartsOld[i].pos.y + (posDiffY * ratio);
		posParts.x = m_pPartsOld[i].pos.x + (posDiffX * ratio);
		posParts.z = m_pPartsOld[i].pos.z + (posDiffZ * ratio);

		// 位置設定
		m_ppModel[i]->SetPosition(posParts + m_ppModel[i]->GetOriginPosition());
	}
}

//==========================================================================
// モーションの設定処理
//==========================================================================
void CMotion::Set(int nType, int nStartKey, bool bBlend, float fCntFrame)
{
	// デバッグ用処理
	if (nType >= m_nNumMotion)
	{// 総数を超えていたら
		MyAssert::CustomAssert(false, "範囲外のモーション");
		return;
	}

	if (m_nType == nType)
	{// 現在の種類と同じだったら
		return;
	}

	m_nOldType = m_nType;		// 前回のモーションの種類
	m_nType = nType;			// 種類設定
	m_nPatternKey = nStartKey;	// 何個目のキーか
	m_fCntFrame = fCntFrame;	// フレームのカウント
	m_fAllFrame = 0.0f;			// 全てのカウント
	m_fMaxAllFrame = 0.0f;		// 全てのカウントの最大値
	m_bFinish = false;			// 終了したかどうか
	m_bCancelable = false;		// キャンセル可能か
	m_bCombiable = false;		// コンボ可能か
	m_bAttaking = false;		// 攻撃判定中フラグ

	// 現在の情報
	CMotionManager::Info& nowInfo = m_vecInfo[m_nType];

	for (int nCntKey = 0; nCntKey < nStartKey; nCntKey++)
	{
		m_fAllFrame += nowInfo.aKey[nCntKey].nFrame;	// 全てのカウント
	}

	for (int nCntKey = 0; nCntKey < nowInfo.nNumKey; nCntKey++)
	{
		m_fMaxAllFrame += nowInfo.aKey[nCntKey].nFrame;	// 全てのカウントの最大値
	}

	// 攻撃情報のフラグオフ
	for (auto& attackInfo : nowInfo.AttackInfo)
	{
		attackInfo.bInpactAct = false;
		attackInfo.bInpactActSet = false;
		attackInfo.bEndAtk = false;

		// 初期フレームより前の物は設定済み
#if 0
		if (m_fAllFrame >= static_cast<float>(attackInfo.nInpactCnt))
		{
			attackInfo.bInpactAct = true;
			attackInfo.bInpactActSet = true;
		}
#endif
	}

	// パーツサイズ
	int partsSize = static_cast<int>(nowInfo.aKey[m_nPatternKey].aParts.size());
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{// 全パーツ分繰り返す

		if (m_ppModel[nCntParts] == nullptr)
		{
			continue;
		}

		// 過去の位置・向きを保存
		if (bBlend)
		{
			// 現在の情報を過去の情報に
			m_pPartsOld[nCntParts].rot = m_ppModel[nCntParts]->GetRotation();
			m_pPartsOld[nCntParts].scale = m_ppModel[nCntParts]->GetScale();

			if (nCntParts == 0)
			{// 親はキャラクターの位置にする
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
		{// ブレンド無し

			if (partsSize <= nCntParts)
			{
				// 現在の情報を過去の情報に
				m_pPartsOld[nCntParts].rot = MyLib::Vector3();
				m_pPartsOld[nCntParts].scale = MyLib::Vector3(1.0f);

				if (nCntParts == 0)
				{// 親はキャラクターの位置にする
					m_pPartsOld[nCntParts].pos = m_pObjChara->GetOriginPosition() - m_ppModel[nCntParts]->GetOriginPosition();
				}
				else
				{
					m_pPartsOld[nCntParts].pos = MyLib::Vector3();
				}
			}
			else
			{
				// 現在の情報を過去の情報に
				m_pPartsOld[nCntParts].rot = nowInfo.aKey[m_nPatternKey].aParts[nCntParts].rot;
				m_pPartsOld[nCntParts].scale = nowInfo.aKey[m_nPatternKey].aParts[nCntParts].scale;

				if (nCntParts == 0)
				{// 親はキャラクターの位置にする
					m_pPartsOld[nCntParts].pos = nowInfo.aKey[m_nPatternKey].aParts[nCntParts].pos + m_pObjChara->GetOriginPosition() - m_ppModel[nCntParts]->GetOriginPosition();
				}
				else
				{
					m_pPartsOld[nCntParts].pos = nowInfo.aKey[m_nPatternKey].aParts[nCntParts].pos;
				}
			}

		}
	}







	// 再生中のキー
	const CMotionManager::Key& nowKey = nowInfo.aKey[m_nPatternKey];

	// 再生フレーム保存
	int nMaxFrame = nowKey.nFrame;

	// デフォルトモーション判定
	bool bDefault = std::find(m_vecDefaultIdx.begin(), m_vecDefaultIdx.end(), m_nType) != m_vecDefaultIdx.end();

	if (bDefault && m_nOldType != 0 && m_nPatternKey == 0)
	{// ニュートラルで0個の時
		nMaxFrame = 10;
	}

	// 次のキー
	int nNextKey = (m_nPatternKey + 1) % nowInfo.nNumKey;

	if (nNextKey == 0 && nowInfo.nLoop == LOOP_OFF)
	{// ループしないとき

		// 最後で固定
		nNextKey = nowInfo.nNumKey - 1;
	}

	//--------------------------
	// 再生フレームとの割合
	//--------------------------
	float ratio = m_fCntFrame / static_cast<float>(nMaxFrame);
	ratio = UtilFunc::Transformation::Clamp(ratio, 0.0f, 1.0f);

	for (int i = 0; i < static_cast<int>(nowKey.aParts.size()); i++)
	{
		// 総数を上回ったら
		if (i >= m_nNumModel) continue;

		// 現在のパーツ情報
		const CMotionManager::Parts& nowParts = nowKey.aParts[i];
		const CMotionManager::Parts& nextParts = nowInfo.aKey[nNextKey].aParts[i];

		// モデルがなかったら戻る
		if (m_ppModel[i] == nullptr) continue;

		//--------------------------
		// 向き更新
		//--------------------------
		UpdateRotation(i, nowParts, nextParts, ratio);

		//--------------------------
		// スケール更新
		//--------------------------
		UpdateScale(i, nowParts, nextParts, ratio);

		//--------------------------
		// 位置反映
		//--------------------------
		//UpdatePosition(i, nowInfo, nowParts, nextParts, ratio, nMaxFrame, 1.0f, 1.0f, 1.0f);
	}
}

//==========================================================================
// 指定した情報が衝撃カウントか
//==========================================================================
bool CMotion::IsImpactFrame(const CMotionManager::Info& info)
{
	// 攻撃情報のコンテナ
	const std::vector<CMotionManager::AttackInfo>& vecAtkInfo = info.AttackInfo;

	for (const auto& atkInfo : vecAtkInfo)
	{
		// 衝撃フレームじゃないと抜ける
		if (!atkInfo.bInpactAct) continue;

		return true;
	}

	return false;
}

//==========================================================================
// 指定したインデックスの情報が衝撃カウントか
//==========================================================================
bool CMotion::IsImpactFrame(int nCntAtk)
{
	// 攻撃情報のコンテナ
	const std::vector<CMotionManager::AttackInfo>& vecAtkInfo = m_vecInfo[m_nType].AttackInfo;

	// 範囲外
	if (static_cast<int>(vecAtkInfo.size()) <= nCntAtk) return false;

	return vecAtkInfo[nCntAtk].bInpactAct;
}

//==========================================================================
// 指定した情報がフレーム内
//==========================================================================
bool CMotion::IsAlignFrame(const CMotionManager::Info& info)
{
	// 攻撃情報のコンテナ
	const std::vector<CMotionManager::AttackInfo>& vecAtkInfo = m_vecInfo[m_nType].AttackInfo;

	for (const auto& atkInfo : vecAtkInfo)
	{
		// 揃え情報
		const CMotionManager::AlignInfo& align = atkInfo.AlignInfo;

		// フレーム外で戻る
		if (!align.bSet) continue;

		return true;
	}

	return false;
}

//==========================================================================
// 攻撃の位置取得
//==========================================================================
MyLib::Vector3 CMotion::GetAttackPosition(CModel** ppModel, CMotionManager::AttackInfo attackInfo)
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
MyLib::Vector3 CMotion::GetAttackPosition(CModel* pModel, CMotionManager::AttackInfo attackInfo)
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
