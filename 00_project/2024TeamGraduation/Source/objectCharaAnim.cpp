//============================================================
//
//	オブジェクトキャラクターアニメーション処理 [objectCharaAnim.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectCharaAnim.h"
#include "manager.h"
#include "renderer.h"
#include "characterAnim.h"

//************************************************************
//	子クラス [CObjectCharaAnim] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectCharaAnim::CObjectCharaAnim(int nPriority) : CObject3DAnim(nPriority)
{

}

//============================================================
//	デストラクタ
//============================================================
CObjectCharaAnim::~CObjectCharaAnim()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectCharaAnim::Init()
{
	// アニメーション3Dの初期化
	if (FAILED(CObject3DAnim::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectCharaAnim::Uninit()
{
	// アニメーション3Dの終了
	CObject3DAnim::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CObjectCharaAnim::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モーションがない場合抜ける
	if (m_info.vecMotion.empty()) { assert(false); return; }

	// アニメーション3Dの更新
	CObject3DAnim::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//============================================================
//	描画処理
//============================================================
void CObjectCharaAnim::Draw()
{
	// アニメーション3Dの描画
	CObject3DAnim::Draw();
}

//============================================================
//	生成処理
//============================================================
CObjectCharaAnim* CObjectCharaAnim::Create(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot)
{
	// オブジェクトキャラクターアニメーションの生成
	CObjectCharaAnim* pObjectCharaAnim = DEBUG_NEW CObjectCharaAnim;
	if (pObjectCharaAnim == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// オブジェクトキャラクターアニメーションの初期化
		if (FAILED(pObjectCharaAnim->Init()))
		{ // 初期化に失敗した場合

			// オブジェクトキャラクターアニメーションの破棄
			SAFE_DELETE(pObjectCharaAnim);
			return nullptr;
		}

		// 位置を設定
		pObjectCharaAnim->SetPosition(rPos);

		// 向きを設定
		pObjectCharaAnim->SetRotation(rRot);

		// 確保したアドレスを返す
		return pObjectCharaAnim;
	}
}

//============================================================
//	キャラクター情報割当
//============================================================
void CObjectCharaAnim::BindCharaData(const char* pCharaPath)
{
	// 割り当てるモーションパスが存在しない場合抜ける
	if (pCharaPath == nullptr) { assert(false); return; }

	// モーション情報の全設定
	SetAllInfo(GET_MANAGER->GetCharacterAnim()->Regist(pCharaPath));
}

//============================================================
//	モーションの設定処理
//============================================================
void CObjectCharaAnim::SetMotion(const int nType)
{
	// 指定されたモーションが存在しない場合抜ける
	if (nType <= -1 || nType >= m_info.GetNumMotion()) { assert(false); return; }

	// 引数のモーションの種類を設定
	m_info.nType = nType;

	// 変数をポインタ化し簡略化
	SChara* pInfoChara = &m_info.vecMotion[nType].infoChara;	// キャラクター情報

	// 指定テクスチャの割当
	BindTexture(CTexture::GetInstance()->Regist(pInfoChara->sPathTexture.c_str()));

	// キャラクター情報を設定
	SetTexPtrn(pInfoChara->ptrnTexture);	// テクスチャ分割数
	SetEnableLoop(pInfoChara->bLoop);		// ループON/OFF
	SetSize(pInfoChara->size);				// 大きさ

	assert((int)pInfoChara->vecNextTime.size() == pInfoChara->nMaxPtrn);
	for (int i = 0; i < pInfoChara->nMaxPtrn; i++)
	{
		// パターン変更時間を設定
		SetNextTime(pInfoChara->vecNextTime[i]);
	}

	// キャラクター情報を初期化
	ResetCurPtrn();	// 開始パターン
	ResetNumLoop();	// 繰り返し数
}

//============================================================
//	モーション情報の追加処理
//============================================================
void CObjectCharaAnim::AddInfo(const SMotion& rMotion)
{
	int nSetMotionIdx = m_info.GetNumMotion();	// モーションを設定する配列番号

	// 空の要素を最後尾に追加
	m_info.vecMotion.emplace_back();

	// 引数のモーション情報を設定
	m_info.vecMotion[nSetMotionIdx] = rMotion;
}

//============================================================
//	モーション情報全設定処理
//============================================================
void CObjectCharaAnim::SetAllInfo(const SInfo& rInfo)
{
	// モーション情報をクリア
	m_info.vecMotion.clear();

	for (auto& rVec : rInfo.vecMotion)
	{ // 読み込んだモーション数分繰り返す

		// モーション情報の追加
		AddInfo(rVec);
	}
}

//============================================================
//	キャンセル取得処理
//============================================================
bool CObjectCharaAnim::IsCancel() const
{
	float fCancelTime = m_info.vecMotion[m_info.nType].fCancelTime;	// キャンセル可能時間
	if (fCancelTime >= 0.0f)
	{ // キャンセル可能時間が設定されている場合

		// モーションのキャンセル状況を返す
		return (GetCurWholeTime() >= fCancelTime);
	}

	// キャンセル不可を返す
	return false;
}

//============================================================
//	コンボ取得処理
//============================================================
bool CObjectCharaAnim::IsCombo() const
{
	float fComboTime = m_info.vecMotion[m_info.nType].fComboTime;	// コンボ可能時間
	if (fComboTime >= 0.0f)
	{ // コンボ可能時間が設定されている場合

		// モーションのコンボ状況を返す
		return (GetCurWholeTime() >= fComboTime);
	}

	// コンボ不可を返す
	return false;
}

//============================================================
//	原点オフセット反映位置の計算処理
//============================================================
MyLib::Vector3 CObjectCharaAnim::CalcOriginOffsetPosition(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot) const
{
	// オフセット反映位置の計算結果を返す
	const MyLib::Vector3& rOffset = m_info.vecMotion[m_info.nType].infoChara.offset;	// オフセット
	return CalcOffsetPosition(rPos, rRot, rOffset);
}

//============================================================
//	判定原点オフセット反映位置の計算処理
//============================================================
MyLib::Vector3 CObjectCharaAnim::CalcCollOffsetPosition(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot) const
{
	// オフセット反映位置の計算結果を返す
	const MyLib::Vector3& rOffset = m_info.vecMotion[m_info.nType].infoColl.offset;	// オフセット
	return CalcOffsetPosition(rPos, rRot, rOffset);
}

//============================================================
//	原点オフセット反映位置の取得処理
//============================================================
MyLib::Vector3 CObjectCharaAnim::GetOriginOffsetPosition() const
{
	// 原点オフセット反映位置の計算結果を返す
	const MyLib::Vector3& rPos = GetPosition();	// 位置
	const MyLib::Vector3& rRot = GetRotation();	// 向き
	return CalcOriginOffsetPosition(rPos, rRot);
}

//============================================================
//	判定原点オフセット反映位置の取得処理
//============================================================
MyLib::Vector3 CObjectCharaAnim::GetCollOffsetPosition() const
{
	// 判定原点オフセット反映位置の計算結果を返す
	const MyLib::Vector3& rPos = GetPosition();	// 位置
	const MyLib::Vector3& rRot = GetRotation();	// 向き
	return CalcCollOffsetPosition(rPos, rRot);
}

//============================================================
//	描画マトリックスの計算処理
//============================================================
void CObjectCharaAnim::CalWorldMtx()
{
	MyLib::Vector3 offset = m_info.vecMotion[m_info.nType].infoChara.offset;	// オフセット
	MyLib::Matrix mtxWorld = GetWorldMtx();		// ワールドマトリックス
	MyLib::Matrix mtxRot, mtxTrans, mtxOffset;	// 計算用マトリックス
	MyLib::Vector3 pos = GetPosition();			// 位置
	MyLib::Vector3 rot = GetRotation();			// 向き

	// ワールドマトリックスの初期化
	mtxWorld.Identity();

	// 位置を反映する
	mtxOffset.Translation(offset);
	mtxWorld.Multiply(mtxWorld, mtxOffset);

	// 向きを反映する
	mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
	mtxWorld.Multiply(mtxWorld, mtxRot);

	// 位置を反映する
	mtxTrans.Translation(pos);
	mtxWorld.Multiply(mtxWorld, mtxTrans);

	// ワールドマトリックスを反映
	SetWorldMtx(mtxWorld);
}

//============================================================
//	オフセット反映位置の計算処理
//============================================================
MyLib::Vector3 CObjectCharaAnim::CalcOffsetPosition
(
	const MyLib::Vector3& rPos,		// 位置
	const MyLib::Vector3& rRot,		// 向き
	const MyLib::Vector3& rOffset	// オフセット
) const
{
	MyLib::Matrix mtxWorld, mtxRot, mtxTrans, mtxOffset;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	mtxWorld.Identity();

	// 位置を反映する
	mtxOffset.Translation(rOffset);
	mtxWorld.Multiply(mtxWorld, mtxOffset);

	// 向きを反映する
	mtxRot.RotationYawPitchRoll(rRot.y, rRot.x, rRot.z);
	mtxWorld.Multiply(mtxWorld, mtxRot);

	// 位置を反映する
	mtxTrans.Translation(rPos);
	mtxWorld.Multiply(mtxWorld, mtxTrans);

	// 算出したマトリックスの位置を返す
	return mtxWorld.GetWorldPosition();
}
