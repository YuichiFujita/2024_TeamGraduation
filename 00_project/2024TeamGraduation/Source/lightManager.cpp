//============================================================
//
//	ライトマネージャー処理 [lightManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "lightManager.h"
#include "manager.h"
#include "camera.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const MyLib::Color DIFFUSE[2][CLightManager::MAX_NUM] =	// 設定用拡散光カラー
	{
		{ // 明るいときのライト
			MyLib::Color(1.0f, 1.0f, 1.0f, 1.0f),
			MyLib::Color(0.65f, 0.65f, 0.65f, 1.0f),
			MyLib::Color(0.15f, 0.15f, 0.15f, 1.0f),
		},

		{ // 暗いときのライト
			MyLib::Color(0.25f, 0.25f, 0.25f, 1.0f),
			MyLib::Color(0.25f, 0.25f, 0.25f, 1.0f),
			MyLib::Color(0.25f, 0.25f, 0.25f, 1.0f),
		}
	};

	const MyLib::Vector3 DIRECTION[] =	// 設定用方向ベクトル
	{
		MyLib::Vector3(0.22f, -0.87f, 0.44f),
		MyLib::Vector3(-0.18f, 0.88f, -0.44f),
		MyLib::Vector3(0.89f, -0.11f, 0.44f),
	};
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(DIRECTION) == CLightManager::MAX_NUM, "ERROR : Light Count Mismatch");

//************************************************************
//	親クラス [CLightManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CLightManager::CLightManager() :
	m_colCamStart	(MyLib::color::White()),	// カメラライト開始色
	m_colCamEnd		(MyLib::color::White()),	// カメラライト終了色
	m_fStartTime	(0.0f),	// 開始時間
	m_fCurTime		(0.0f),	// 経過時間
	m_bChange		(false)	// 変動フラグ
{
	// メンバ変数をクリア
	memset(&m_apLight[0], 0, sizeof(m_apLight));		// ライト情報
	memset(&m_aColStart[0], 0, sizeof(m_aColStart));	// ライト開始色
	memset(&m_aColEnd[0], 0, sizeof(m_aColEnd));		// ライト終了色

	D3DXCOLOR m_colCamStart;		// カメラライト開始色
	D3DXCOLOR m_colCamEnd;			// カメラライト終了色
}

//============================================================
//	デストラクタ
//============================================================
CLightManager::~CLightManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CLightManager::Init()
{
	for (int i = 0; i < MAX_NUM; i++)
	{ // 使用するライト数分繰り返す

		// ライトの生成
		m_apLight[i] = CLightDir::Create();
		if (m_apLight[i] == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}

		// ライトのオブジェクト種類を変更
		m_apLight[i]->SetType(CObject::TYPE::TYPE_NONE);	// 自動破棄/自動更新を停止

		// ライトの拡散光を設定
		m_apLight[i]->SetDiffuse(DIFFUSE[0][i]);

		// ライトの方向を設定
		m_apLight[i]->SetDirection(DIRECTION[i]);
	}

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CLightManager::Uninit()
{
	for (int i = 0; i < MAX_NUM; i++)
	{ // 使用するライト数分繰り返す

		// ライトの終了
		SAFE_UNINIT(m_apLight[i]);
	}
}

//============================================================
//	更新処理
//============================================================
void CLightManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_bChange)
	{
		CCamera* pCamera = GET_MANAGER->GetCamera();	// カメラ情報

		// 変動時間を減算
		m_fCurTime -= fDeltaTime * fSlowRate;

		// 経過終了した場合終了
		if (m_fCurTime <= 0.0f) { m_bChange = false; }

		// カメラライトの拡散光を保存
		MyLib::Vector3 vecCamStart	= ConvertColorToVector3(m_colCamStart);
		MyLib::Vector3 vecCamEnd	= ConvertColorToVector3(m_colCamEnd);
		MyLib::Vector3 vecCamCur	= UtilFunc::Correction::EasingQuintIn(vecCamStart, vecCamEnd, m_fStartTime, 0.0f, m_fCurTime);
		D3DXCOLOR	   colCamCur	= ConvertVector3ToColor(vecCamCur, m_colCamEnd.a);
		pCamera->SetLightDiffuse(colCamCur);

		for (int i = 0; i < MAX_NUM; i++)
		{ // 使用するライト数分繰り返す

			// ライトの拡散光を保存
			MyLib::Vector3 vecStart	= ConvertColorToVector3(m_aColStart[i]);
			MyLib::Vector3 vecEnd	= ConvertColorToVector3(m_aColEnd[i]);
			MyLib::Vector3 vecCur	= UtilFunc::Correction::EasingQuintIn(vecStart, vecEnd, m_fStartTime, 0.0f, m_fCurTime);
			D3DXCOLOR	   colCur	= ConvertVector3ToColor(vecCur, m_colCamEnd.a);
			m_apLight[i]->SetDiffuse(colCur);
		}
	}

	for (int i = 0; i < MAX_NUM; i++)
	{ // 使用するライト数分繰り返す

		// ライトの更新
		m_apLight[i]->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//============================================================
//	明るさの設定処理
//============================================================
void CLightManager::SetEnableBright(const bool bBright, const float fChangeTime)
{
	CCamera* pCamera = GET_MANAGER->GetCamera();	// カメラ情報
	int nBright = (bBright) ? 0 : 1;		// ライト配列インデックス
	const D3DXCOLOR aEndCol[] =		// カメラライト配列
	{
		pCamera->GetDestLightDiffuse(),		// 明るいときの色
		MyLib::color::Black(aEndCol[0].a),	// 暗いときの色
	};

	if (fChangeTime > 0.0f)
	{ // 変動時間が設定されている場合

		// 変動時間を保存
		m_fStartTime = m_fCurTime = fChangeTime;

		for (int i = 0; i < MAX_NUM; i++)
		{ // 使用するライト数分繰り返す

			// ライトの拡散光を保存
			m_aColStart[i] = m_apLight[i]->GetLight().Diffuse;
			m_aColEnd[i] = DIFFUSE[nBright][i];
		}

		// カメラライトの拡散光を保存
		m_colCamStart = pCamera->GetLightDiffuse();
		m_colCamEnd = aEndCol[nBright];

		// 変動中にする
		m_bChange = true;
	}
	else
	{ // 変動時間が未設定の場合

		// 時間の初期化
		m_fStartTime = m_fCurTime = 0.0f;

		for (int i = 0; i < MAX_NUM; i++)
		{ // 使用するライト数分繰り返す

			// ライトの拡散光を設定
			m_apLight[i]->SetDiffuse(DIFFUSE[nBright][i]);
		}

		// カメラライトの拡散光を設定
		pCamera->SetLightDiffuse(aEndCol[nBright]);

		// 変動終了
		m_bChange = false;
	}
}

//============================================================
//	生成処理
//============================================================
CLightManager* CLightManager::Create()
{
	// ライトマネージャーの生成
	CLightManager* pLightManager = DEBUG_NEW CLightManager;
	if (pLightManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ライトマネージャーの初期化
		if (FAILED(pLightManager->Init()))
		{ // 初期化に失敗した場合

			// ライトマネージャーの破棄
			SAFE_DELETE(pLightManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pLightManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CLightManager::Release(CLightManager*& prLightManager)
{
	// ライトマネージャーの終了
	assert(prLightManager != nullptr);
	prLightManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prLightManager);
}
