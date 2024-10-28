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
CLightManager::CLightManager()
{
	// メンバ変数をクリア
	memset(&m_apLight[0], 0, sizeof(m_apLight));	// ライト情報
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
	for (int i = 0; i < MAX_NUM; i++)
	{ // 使用するライト数分繰り返す

		// ライトの更新
		m_apLight[i]->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//============================================================
//	明るさの設定処理
//============================================================
void CLightManager::SetEnableBright(const bool bBright)
{
	int nBright = (bBright) ? 0 : 1;	// ライト配列インデックス
	for (int i = 0; i < MAX_NUM; i++)
	{ // 使用するライト数分繰り返す

		// ライトの拡散光を設定
		m_apLight[i]->SetDiffuse(DIFFUSE[nBright][i]);
	}

	// カメラのライトフラグを設定
	GET_MANAGER->GetCamera()->SetEnableLight(bBright);
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
