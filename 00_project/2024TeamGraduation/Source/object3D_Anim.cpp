//==========================================================================
// 
//  オブジェクト3Dアニメ処理 [object3D_Anim.cpp]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//==========================================================================
#include "object3D_Anim.h"
#include "calculation.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CObject3DAnim::CObject3DAnim(int nPriority) : CObject3D(nPriority),
	m_funcPattern	(nullptr),				// パターン変更関数ポインタ
	m_ptrn			(MyLib::PosGrid2(1)),	// テクスチャ分割数
	m_split			(MyLib::Vector2()),		// スプライト量
	m_pNextTime		(nullptr),				// パターン変更時間
	m_fCurTime		(0.0f),					// 現在の待機時間
	m_fCurWholeTime	(0.0f),					// 現在の全体時間
	m_fMaxWholeTime	(0.0f),					// 総全体時間
	m_nCurPtrn		(0),					// 現在のパターン
	m_nMaxPtrn		(0),					// パターンの総数
	m_nNumLoop		(0),					// パターン繰り返し数
	m_bStop			(false),				// 停止フラグ
	m_bPlayBack		(false),				// 逆再生フラグ
	m_bLoop			(true),					// ループフラグ
	m_bFinish		(false),				// 終了フラグ
	m_bAutoDeath	(false)					// 自動破棄フラグ
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CObject3DAnim::~CObject3DAnim()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObject3DAnim *CObject3DAnim::Create
(
	const MyLib::PosGrid2& rPtrn,	// テクスチャ分割数
	const MyLib::Vector3& rPos,		// 位置
	const float fNextTime,			// パターン変更時間
	const bool bAutoDeath,			// 自動破棄フラグ
	int nPriority
)
{
	// メモリの確保
	CObject3DAnim* pObject3D = DEBUG_NEW CObject3DAnim(nPriority);
	if (pObject3D != nullptr)
	{// メモリの確保が出来ていたら

		// クラスの初期化
		if (FAILED(pObject3D->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pObject3D);
			return nullptr;
		}

		// 引数情報
		pObject3D->SetPosition(rPos);			// 位置設定
		pObject3D->SetOriginPosition(rPos);		// 原点位置設定
		pObject3D->SetTexPtrn(rPtrn);			// テクスチャ分割数設定
		pObject3D->SetNextTime(fNextTime);		// パターン変更時間設定
		pObject3D->m_bAutoDeath = bAutoDeath;	// 自動削除フラグ設定
	}

	return pObject3D;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObject3DAnim::Init()
{
	// 親クラスの初期化
	HRESULT hr = CObject3D::Init();
	if (FAILED(hr)) { return E_FAIL; }

	// 通常再生を初期化
	SetEnablePlayBack(false);

	// 頂点情報設定
	SetVtx();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObject3DAnim::Uninit()
{
	// パターン変更時間の破棄
	SAFE_DEL_ARRAY(m_pNextTime);

	// 終了処理
	CObject3D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObject3DAnim::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 停止中の場合抜ける
	if (m_bStop) { return; }

	// パターンの更新
	assert(m_funcPattern != nullptr);
	if (m_funcPattern(fDeltaTime, fDeltaRate, fSlowRate))
	{ // 関数内部で破棄された場合

		return;
	}

	// 更新処理
	CObject3D::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CObject3DAnim::SetVtx()
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 境界線のベクトル
	MyLib::Vector3 vecLine0 = pVtx[1].pos - pVtx[0].pos;
	MyLib::Vector3 vecLine1 = pVtx[2].pos - pVtx[0].pos;

	// 外積求める
	MyLib::Vector3 Nor[4];

	// 外積
	D3DXVec3Cross(&Nor[0], &vecLine0, &vecLine1);
	D3DXVec3Normalize(&Nor[0], &Nor[0]);

	Nor[0] = MyLib::Vector3(
		((vecLine0.y * vecLine1.z) - (vecLine0.z * vecLine1.y)),
		((vecLine0.z * vecLine1.x) - (vecLine0.x * vecLine1.z)),
		((vecLine0.x * vecLine1.y) - (vecLine0.y * vecLine1.x)));
	D3DXVec3Normalize(&Nor[0], &Nor[0]);

	// 境界線のベクトル
	vecLine0 = pVtx[2].pos - pVtx[3].pos;
	vecLine1 = pVtx[1].pos - pVtx[3].pos;

	// 外積求める
	Nor[3] = MyLib::Vector3(
		((vecLine0.y * vecLine1.z) - (vecLine0.z * vecLine1.y)),
		((vecLine0.z * vecLine1.x) - (vecLine0.x * vecLine1.z)),
		((vecLine0.x * vecLine1.y) - (vecLine0.y * vecLine1.x)));

	// 外積
	D3DXVec3Cross(&Nor[3], &vecLine0, &vecLine1);
	D3DXVec3Normalize(&Nor[3], &Nor[3]);

	Nor[1] = (Nor[0] + Nor[3]) / 2.0f;
	Nor[2] = (Nor[0] + Nor[3]) / 2.0f;

	// 法線ベクトルの設定
	pVtx[0].nor = Nor[0];
	pVtx[1].nor = Nor[1];
	pVtx[2].nor = Nor[2];
	pVtx[3].nor = Nor[3];

	MyLib::Vector3 size = GetSize();
	D3DXCOLOR col = GetColor();

	// 位置を更新
	pVtx[0].pos = MyLib::Vector3(-size.x, +size.y, +size.z);
	pVtx[1].pos = MyLib::Vector3(+size.x, +size.y, +size.z);
	pVtx[2].pos = MyLib::Vector3(-size.x, -size.y, -size.z);
	pVtx[3].pos = MyLib::Vector3(+size.x, -size.y, -size.z);

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// テクスチャ座標の設定
	int nWidthCurrent = m_nCurPtrn % m_ptrn.x;					// 現在の横のパターン
	int nHeightCurrent = (m_nCurPtrn / m_ptrn.x) % m_ptrn.y;	// 現在の縦のパターン
	pVtx[0].tex = D3DXVECTOR2(m_split.x *  nWidthCurrent,		m_split.y *  nHeightCurrent);
	pVtx[1].tex = D3DXVECTOR2(m_split.x * (nWidthCurrent + 1),	m_split.y *  nHeightCurrent);
	pVtx[2].tex = D3DXVECTOR2(m_split.x *  nWidthCurrent,		m_split.y * (nHeightCurrent + 1));
	pVtx[3].tex = D3DXVECTOR2(m_split.x * (nWidthCurrent + 1),	m_split.y * (nHeightCurrent + 1));

	// 頂点バッファをアンロックロック
	m_pVtxBuff->Unlock();
}

//============================================================
//	現在パターンの設定処理
//============================================================
void CObject3DAnim::SetCurPtrn(const int nPtrn)
{
	// 引数のパターン数を設定
	m_nCurPtrn = nPtrn;

	// 終了フラグをOFFにする
	m_bFinish = false;

	// 全体時間の初期化
	m_fCurWholeTime = 0.0f;

	// 全体時間をパターンの開始時間に設定
	int nAddTime = (!m_bPlayBack) ? 0 : 1;	// 逆再生の場合は現在パターンの待機時間も含む
	int nLoop = m_nCurPtrn + nAddTime;		// 繰り返し数を求める
	for (int i = 0; i < nLoop; i++)
	{ // 開始パターンまで繰り返す

		// 待機時間を加算
		m_fCurWholeTime += m_pNextTime[i];
	}

	// 頂点の設定
	SetVtx();
}

//============================================================
//	テクスチャ分割数の設定処理
//============================================================
void CObject3DAnim::SetTexPtrn(const MyLib::PosGrid2 &rPtrn)
{
	// 引数のテクスチャ分割数を設定
	m_ptrn = rPtrn;

	// スプライト量を設定
	m_split.x = 1.0f / static_cast<float>(m_ptrn.x);	// Uのスプライト量設定
	m_split.y = 1.0f / static_cast<float>(m_ptrn.y);	// Vのスプライト量設定

	// パターン総数を設定
	SetMaxPtrn(m_ptrn.x * m_ptrn.y);
}

//============================================================
//	テクスチャ横分割数の設定処理
//============================================================
void CObject3DAnim::SetTexPtrnWidth(const int nTexPtrnW)
{
	// 引数のテクスチャ横分割数を設定
	m_ptrn.x = nTexPtrnW;

	// Uスプライト量を設定
	m_split.x = 1.0f / static_cast<float>(m_ptrn.x);

	// パターン総数を設定
	SetMaxPtrn(m_ptrn.x * m_ptrn.y);
}

//============================================================
//	テクスチャ縦分割数の設定処理
//============================================================
void CObject3DAnim::SetTexPtrnHeight(const int nTexPtrnH)
{
	// 引数のテクスチャ縦分割数を設定
	m_ptrn.y = nTexPtrnH;

	// Vスプライト量を設定
	m_split.y = 1.0f / static_cast<float>(m_ptrn.y);

	// パターン総数を設定
	SetMaxPtrn(m_ptrn.x * m_ptrn.y);
}

//============================================================
//	再生フラグの設定処理
//============================================================
void CObject3DAnim::SetEnableStop(const bool bStop)
{
	// 停止した場合に繰り返し数を初期化
	if (!bStop) { m_nNumLoop = 0; }

	// 引数の停止状況を設定
	m_bStop = bStop;
}

//============================================================
//	逆再生フラグの設定処理
//============================================================
void CObject3DAnim::SetEnablePlayBack(const bool bPlayBack)
{
	// 再生/逆再生が反転した場合に繰り返し数と終了フラグを初期化
	if (m_bPlayBack != bPlayBack) { m_nNumLoop = 0; m_bFinish = false; }

	// 引数の逆再生状況を設定
	m_bPlayBack = bPlayBack;

	if (!m_bPlayBack)
	{ // 通常再生の場合

		// パターン加算関数を設定
		m_funcPattern = std::bind(&CObject3DAnim::NextPtrn, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}
	else
	{ // 逆再生の場合

		// パターン減算関数を設定
		m_funcPattern = std::bind(&CObject3DAnim::BackPtrn, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}
}

//============================================================
//	ループフラグの設定処理
//============================================================
void CObject3DAnim::SetEnableLoop(const bool bLoop)
{
	// ループする場合終了フラグを初期化
	if (bLoop) { m_bFinish = false; }

	// 引数のループ状況を設定
	m_bLoop = bLoop;
}

//============================================================
//	現在パターンの初期化処理
//============================================================
void CObject3DAnim::ResetCurPtrn()
{
	// 終了フラグをOFFにする
	m_bFinish = false;

	if (!m_bPlayBack)
	{ // 通常再生の場合

		// 開始パターンの初期化
		m_nCurPtrn = 0;

		// 待機時間の初期化
		m_fCurTime = 0.0f;
		m_fCurWholeTime = 0.0f;
	}
	else
	{ // 逆再生の場合

		// 開始パターンの初期化
		m_nCurPtrn = m_nMaxPtrn - 1;

		// 待機時間の初期化
		m_fCurTime = m_pNextTime[m_nCurPtrn];
		m_fCurWholeTime = m_fMaxWholeTime;
	}
}

//============================================================
//	パターン変更時間の設定処理 (パターン指定)
//============================================================
void CObject3DAnim::SetNextTime(const int nPtrnIdx, const float fNextTime)
{
	// パターンインデックスが範囲外の場合抜ける
	if (nPtrnIdx <= -1 || nPtrnIdx >= m_nMaxPtrn) { assert(false); return; }

	// 変更時間がプラスではない場合抜ける
	if (fNextTime <= 0.0f) { assert(false); return; }

	// 変更前のパターン変更時間を保存
	float fOldNextTime = m_pNextTime[nPtrnIdx];

	// 引数のパターン変更時間を設定
	m_pNextTime[nPtrnIdx] = fNextTime;

	// 総全体時間を変更
	m_fMaxWholeTime -= fOldNextTime;
	m_fMaxWholeTime += fNextTime;
}

//============================================================
//	パターン変更時間の設定処理 (全パターン)
//============================================================
void CObject3DAnim::SetNextTime(const float fNextTime)
{
	// 変更時間がプラスではない場合抜ける
	if (fNextTime <= 0.0f) { assert(false); return; }

	for (int i = 0; i < m_nMaxPtrn; i++)
	{ // パターンの総数分繰り返す

		// 引数のパターン変更時間を設定
		m_pNextTime[i] = fNextTime;
	}

	// 総全体時間を変更
	m_fMaxWholeTime = fNextTime * (float)m_nMaxPtrn;
}

//============================================================
//	パターン総数の設定処理
//============================================================
HRESULT CObject3DAnim::SetMaxPtrn(const int nMaxPtrn)
{
	// 引数のパターンの総数を設定
	m_nMaxPtrn = nMaxPtrn;

	// パターン変更時間の破棄
	SAFE_DEL_ARRAY(m_pNextTime);

	// パターン変更時間の再生成
	m_pNextTime = new float[m_nMaxPtrn];
	if (m_pNextTime == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// パターン変更時間を初期化
	SetNextTime(DEF_NEXT);

	// 頂点の設定
	SetVtx();

	// 成功を返す
	return S_OK;
}

//============================================================
//	パターン加算処理
//============================================================
bool CObject3DAnim::NextPtrn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// アニメーションが終了している場合抜ける
	if (m_bFinish) { return false; }

	// 現在の待機時間を加算
	m_fCurTime += fDeltaTime * fDeltaRate * fSlowRate;
	m_fCurWholeTime += fDeltaTime * fDeltaRate * fSlowRate;

	if (m_fCurTime >= m_pNextTime[m_nCurPtrn])
	{ // 待機し終わった場合

		// 現在の待機時間から今回の待機時間を減算
		m_fCurTime -= m_pNextTime[m_nCurPtrn];

		// パターンを加算
		m_nCurPtrn = (m_nCurPtrn + 1) % m_nMaxPtrn;
		if (m_nCurPtrn == 0)
		{ // 先頭パターンの場合

			if (m_bLoop)
			{ // ループする場合

				// 繰り返し数を加算
				m_nNumLoop++;

				// 全体時間を初期化
				m_fCurWholeTime = 0.0f;
			}
			else
			{ // ループしない場合

				// パターンを補正
				m_nCurPtrn = m_nMaxPtrn - 1;

				// 全体時間を初期化
				m_fCurWholeTime = m_fMaxWholeTime;

				// アニメーション終了を保存
				m_bFinish = true;
			}

			if (m_bAutoDeath)
			{ // 自動破棄がONの場合

				// オブジェクト破棄
				Uninit();
				return true;
			}
		}
	}

	return false;
}

//============================================================
//	パターン減算処理
//============================================================
bool CObject3DAnim::BackPtrn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// アニメーションが終了している場合抜ける
	if (m_bFinish) { return false; }

	// 現在の待機時間を加算
	m_fCurTime -= fDeltaTime * fDeltaRate * fSlowRate;
	m_fCurWholeTime -= fDeltaTime * fDeltaRate * fSlowRate;

	if (m_fCurTime <= 0.0f)
	{ // 待機し終わった場合

		// 現在の待機時間から今回の待機時間を減算
		m_fCurTime += m_pNextTime[m_nCurPtrn];

		// パターンを減算
		m_nCurPtrn = (m_nCurPtrn + (m_nMaxPtrn - 1)) % m_nMaxPtrn;

		if (m_nCurPtrn == m_nMaxPtrn - 1)
		{ // 最終パターンの場合

			if (m_bLoop)
			{ // ループする場合

				// 繰り返し数を加算
				m_nNumLoop++;

				// 全体時間を初期化
				m_fCurWholeTime = m_fMaxWholeTime;
			}
			else
			{ // ループしない場合

				// パターンを補正
				m_nCurPtrn = 0;

				// 全体時間を初期化
				m_fCurWholeTime = 0.0f;

				// アニメーション終了を保存
				m_bFinish = true;
			}

			if (m_bAutoDeath)
			{ // 自動破棄がONの場合

				// オブジェクト破棄
				Uninit();
				return true;
			}
		}
	}

	return false;
}
