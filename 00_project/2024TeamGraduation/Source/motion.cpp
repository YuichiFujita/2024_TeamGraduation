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
	m_bAttaking = false;			// 攻撃判定中フラグ

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
	// パーツ情報
	const Info& info = m_vecInfo[nType];
	const Key& keyInfo = info.aKey[0];

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

	// 再生中の情報
	Info& nowInfo = m_vecInfo[m_nType];

	// 攻撃判定中フラグリセット
	m_bAttaking = false;

	// 攻撃情報の総数取得
	int nNumAttackInfo = nowInfo.nNumAttackInfo;

	for (auto& attackInfo : nowInfo.AttackInfo)
	{
		// まだ衝撃カウントの行動をしてない状態にする
		attackInfo.bInpactAct = false;

		if (attackInfo.nInpactCnt > 0&&
			m_fAllFrame >= attackInfo.nInpactCnt &&
			!attackInfo.bInpactActSet)
		{// 衝撃のカウントを超えた時 && まだ行動してなかったら

			// 行動してる状態にして、設定完了
			attackInfo.bInpactAct = true;
			attackInfo.bInpactActSet = true;
		}
	}

	if (m_bFinish && nowInfo.nLoop == LOOP_OFF)
	{// 終了してた && ループOFFだったら
		return;
	}


	// 再生中のキー
	const Key& nowKey = nowInfo.aKey[m_nPatternKey];

	// 再生フレーム保存
	int nMaxFrame = nowKey.nFrame;

	if (m_nType == 0 && m_nOldType != 0 && m_nPatternKey == 0)
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
		// 現在のパーツ情報
		const Parts& nextParts = nowInfo.aKey[nNextKey].aParts[i];
		const Parts& nowParts = nowKey.aParts[i];

		// モデルがなかったら戻る
		if (m_ppModel[i] == nullptr) continue;

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


		//--------------------------
		// スケール
		//--------------------------
		// 次と今の向きの差分取得
		float scaleDiffX = nowInfo.aKey[nNextKey].aParts[i].scale.x -
			m_pPartsOld[i].scale.x;

		float scaleDiffY = nowInfo.aKey[nNextKey].aParts[i].scale.y -
			m_pPartsOld[i].scale.y;

		float scaleDiffZ = nowInfo.aKey[nNextKey].aParts[i].scale.z -
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


		//--------------------------
		// 位置反映
		//--------------------------
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
			float posDiffX = nowInfo.aKey[nNextKey].aParts[i].pos.x -
				m_pPartsOld[i].pos.x;

			float posDiffY = nowInfo.aKey[nNextKey].aParts[i].pos.y -
				m_pPartsOld[i].pos.y;

			float posDiffZ = nowInfo.aKey[nNextKey].aParts[i].pos.z -
				m_pPartsOld[i].pos.z;

			// 親のYを補正
			posParts.y =
				m_pPartsOld[i].pos.y + (posDiffY * ratio);

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
			fMoveDiff /= (static_cast<float>(nMaxFrame) / static_cast<float>(fDeltaRate));

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

	// フレームのカウントを加算
	m_fCntFrame += 1.0f * fDeltaRate * fSlowRate;
	m_fAllFrame += 1.0f * fDeltaRate * fSlowRate;

	// 攻撃判定中フラグ設定
	for (auto& attackInfo : nowInfo.AttackInfo)
	{
		if (attackInfo.nMaxCnt == 0)
		{// 最大値が0のはなにもしない
			continue;
		}

		// 攻撃判定
		if (m_fAllFrame >= static_cast<float>(attackInfo.nMinCnt) && 
			m_fAllFrame <= static_cast<float>(attackInfo.nMaxCnt))
		{// 最小値と最大値の中
			m_bAttaking = true;
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
		{// キーがゼロの時

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
// モーションの設定処理
//==========================================================================
void CMotion::Set(int nType, bool bBlend)
{
	// デバッグ用処理
	if (nType >= m_nNumMotion)
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
	m_fAllFrame = 0.0f;	// 全てのカウント
	m_fMaxAllFrame = 0.0f;	// 全てのカウントの最大値
	m_bFinish = false;		// 終了したかどうか
	m_bCancelable = false;	// キャンセル可能か
	m_bCombiable = false;	// コンボ可能か
	m_bAttaking = false;			// 攻撃判定中フラグ

	// 現在の情報
	Info& nowInfo = m_vecInfo[m_nType];

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
				m_pPartsOld[nCntParts].rot = nowInfo.aKey[0].aParts[nCntParts].rot;
				m_pPartsOld[nCntParts].scale = nowInfo.aKey[0].aParts[nCntParts].scale;

				if (nCntParts == 0)
				{// 親はキャラクターの位置にする
					m_pPartsOld[nCntParts].pos = nowInfo.aKey[0].aParts[nCntParts].pos + m_pObjChara->GetOriginPosition() - m_ppModel[nCntParts]->GetOriginPosition();
				}
				else
				{
					m_pPartsOld[nCntParts].pos = nowInfo.aKey[0].aParts[nCntParts].pos;
				}
			}

		}
	}
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
MyLib::Vector3 CMotion::GetAttackPosition(CModel** ppModel, AttackInfo attackInfo)
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
MyLib::Vector3 CMotion::GetAttackPosition(CModel* pModel, AttackInfo attackInfo)
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
		m_nNumMotion = static_cast<int>(m_vecLoadData[nIdx].size());

		// モーション情報渡す
		m_vecInfo = m_vecLoadData[nIdx];
		return;
	}

	// ファイル名保存
	m_sTextFile.push_back(file);


	//--------------------------
	// ファイル読み込み
	//--------------------------
	// ファイルを開く
	std::ifstream File(file);
	if (!File.is_open())
	{//ファイルが開けなかった場合
		return;
	}

	// 読み込み用変数
	int nCntFile = 0;	// ファイルの読み込み回数
	std::vector<std::string> motionFile;	// モーションファイル

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

		if (line.find("MOTION_FILENAME") != std::string::npos)
		{// モーションファイル名読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>		// ゴミ
				hoge >>		// ＝
				hoge;		// モーションファイル名

			// ファイル名保存
			motionFile.push_back(hoge);
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// ファイルを閉じる
	File.close();


	// 読み込みデータ渡す
	m_vecLoadData.emplace_back();

	// 要素分繰り返し
	for (int i = 0; i < static_cast<int>(motionFile.size()); i++)
	{
		// モーション読み込み
		LoadMotion(motionFile[i], i);
	}


	// モーション情報渡す
	m_vecInfo = m_vecLoadData[m_nNumLoad];

	// モーション数
	m_nNumMotion = static_cast<int>(m_vecInfo.size());

	// 読み込んだ数加算
	m_nNumLoad++;
}

//==========================================================================
// モーション読み込み
//==========================================================================
void CMotion::LoadMotion(const std::string& file, int nIdxMotion)
{
	// ファイルを開く
	std::ifstream File(file);
	if (!File.is_open())
	{//ファイルが開けなかった場合
		return;
	}


	// 読み込み用変数
	Info InitInfo = {};

	// キャンセル可能フレームリセット
	InitInfo.nCancelableFrame = -1;
	InitInfo.nCombolableFrame = -1;

	// 読み込み用データ
	Info loadInfo = InitInfo;

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

		if (line.find("MOTIONSET") != std::string::npos)
		{// モーション情報の読み込みを開始

			while (line.find("END_MOTIONSET") == std::string::npos)
			{// END_MOTIONSETが来るまで繰り返し

				// 確認する
				std::getline(File, line);

				if (line.find("LOOP") != std::string::npos)
				{// LOOPが来たらループON/OFF読み込み

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>			// ゴミ
						hoge >>			// ＝
						loadInfo.nLoop;	// ループ0か1
				}
				else if (line.find("NUM_KEY") != std::string::npos)
				{// NUM_KEYが来たらキー数読み込み

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>				// ゴミ
						hoge >>				// ＝
						loadInfo.nNumKey;	// キー数
				}
				else if (line.find("MOVE") != std::string::npos)
				{// MOVEが来たら移動ON/OFF読み込み

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>				// ゴミ
						hoge >>				// ＝
						loadInfo.nMove;		// 移動0か1か
				}
				else if (line.find("CANCELABLE") != std::string::npos)
				{// CANCELABLEでキャンセル可能フレーム読み込み

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>		// ゴミ
						hoge >>		// ＝
						loadInfo.nCancelableFrame;	// キャンセル可能フレーム
				}
				else if (line.find("COMBOLABLE") != std::string::npos)
				{// COMBOLABLEでコンボ可能フレーム読み込み

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>		// ゴミ
						hoge >>		// ＝
						loadInfo.nCombolableFrame;	// コンボ可能フレーム
				}


				//--------------------------
				// 攻撃情報
				//--------------------------
				if (line.find("ATTACKINFO") != std::string::npos)
				{// ATTACKINFOが来たら

					// 読み込み用変数
					AttackInfo loadAttackInfo;

					while (line.find("END_ATTACKINFO") == std::string::npos)
					{// END_ATTACKINFOが来るまで繰り返し

						// 確認する
						std::getline(File, line);


						if (line.find("COLLISIONPARTS") != std::string::npos)
						{// COLLISIONPARTSが来たらパーツ番号読み込み

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>			// ゴミ
								hoge >>			// ＝
								loadAttackInfo.nCollisionNum;	// 判定を取るパーツ番号
						}
						else if (line.find("ATTACKRANGE") != std::string::npos)
						{// ATTACKRANGEが来たら攻撃サイズ読み込み

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>			// ゴミ
								hoge >>			// ＝
								loadAttackInfo.fRangeSize;	// 判定のサイズ
						}
						else if (line.find("OFFSET") != std::string::npos)
						{// OFFSETが来たらオフセット読み込み

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>			// ゴミ
								hoge >>			// ＝
								loadAttackInfo.Offset.x >> loadAttackInfo.Offset.y >> loadAttackInfo.Offset.z;	// オフセット
						}
						else if (line.find("ATTACKCOUNT") != std::string::npos)
						{// ATTACKCOUNTが来たら攻撃カウント読み込み

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>			// ゴミ
								hoge >>			// ＝
								loadAttackInfo.nMinCnt >>
								loadAttackInfo.nMaxCnt;	// 衝撃のカウント
						}
						else if (line.find("INPACTCOUNT") != std::string::npos)
						{// INPACTCOUNTが来たら衝撃カウント読み込み

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>			// ゴミ
								hoge >>			// ＝
								loadAttackInfo.nInpactCnt;	// 衝撃のカウント
						}
						else if (line.find("DAMAGE") != std::string::npos)
						{// DAMAGEが来たら攻撃力読み込み

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>			// ゴミ
								hoge >>			// ＝
								loadAttackInfo.nDamage;	// 攻撃力
						}
					}

					// 攻撃の情報追加
					loadInfo.AttackInfo.push_back(loadAttackInfo);
				}


				//--------------------------
				// 各キーの設定
				//--------------------------
				if (line.find("KEYSET") != std::string::npos)
				{// KEYSETでキー情報の読み込み開始

					// 読み込み用データ
					Key loadKey;

					while (line.find("END_KEYSET") == std::string::npos)
					{// END_KEYSETが来るまで繰り返し

						// 確認する
						std::getline(File, line);


						if (line.find("FRAME") != std::string::npos)
						{// FRAMEが来たら再生フレーム数読み込み

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>			// ゴミ
								hoge >>			// ＝
								loadKey.nFrame;	// 再生フレーム
						}


						//--------------------------
						// パーツの設定
						//--------------------------
						if (line.find("PARTS") != std::string::npos)
						{// PARTSでパーツ情報の読み込み開始

							// 読み込み用データ
							Parts loadParts;

							while (line.find("END_PARTS") == std::string::npos)
							{// END_PARTSが来るまで繰り返し

								// 確認する
								std::getline(File, line);

								if (line.find("POS") != std::string::npos)
								{// POS

									// ストリーム作成
									std::istringstream lineStream(line);

									// 情報渡す
									lineStream >>
										hoge >>			// ゴミ
										hoge >>			// ＝
										loadParts.pos.x >> loadParts.pos.y >> loadParts.pos.z;	// 位置
								}
								else if (line.find("ROT") != std::string::npos)
								{// ROT

									// ストリーム作成
									std::istringstream lineStream(line);

									// 情報渡す
									lineStream >>
										hoge >>			// ゴミ
										hoge >>			// ＝
										loadParts.rot.x >> loadParts.rot.y >> loadParts.rot.z;	// 向き
								}
								else if (line.find("SCALE") != std::string::npos)
								{// SCALE

									// ストリーム作成
									std::istringstream lineStream(line);

									// 情報渡す
									lineStream >>
										hoge >>			// ゴミ
										hoge >>			// ＝
										loadParts.scale.x >> loadParts.scale.y >> loadParts.scale.z;	// スケール
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
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// ファイルを閉じる
	File.close();


	//-----------------------------
	// 読み込み後の計算
	//-----------------------------
	// キーの数
	loadInfo.nNumKey = static_cast<int>(loadInfo.aKey.size());

	// 攻撃情報の数
	loadInfo.nNumAttackInfo = static_cast<int>(loadInfo.AttackInfo.size());

	for (int i = 0; i < loadInfo.nNumKey; i++)
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

}
