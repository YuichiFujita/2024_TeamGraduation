//=============================================================================
// 
//  エフェクシア処理 [MyEffekseer.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "MyEffekseer.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "pause.h"
#include <Effekseer.h>
#include "EffekseerObj.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
std::string CMyEffekseer::m_EffectName[CMyEffekseer::EFKLABEL_MAX] =	// エフェクトのファイル名
{
	"data/Effekseer/Laser01.efkefc",		// サンプルのレーザー
	"data/Effekseer/bress.efkefc",			// 息
	"data/Effekseer/sample_river.efkefc",	// 川サンプル
	"data/Effekseer/impact.efkefc",			// 衝撃
	"data/Effekseer/spraywater.efkefc",		// 衝撃
	"data/Effekseer/spraywater_mini.efkefc",// 衝撃
	"data/Effekseer/waterjump.efkefc",		// 魚入水
	"data/Effekseer/playermoveLine.efkefc",	// プレイヤーの移動線
	"data/Effekseer/photon.efkefc",			// 死亡
	"data/Effekseer/ItemGet.efkefc",		// 荷物獲得
	"data/Effekseer/air.efkefc",			// 空気
	"data/Effekseer/crowd.efkefc",			// 群衆
	"data/Effekseer/teleport.efkefc",		// テレポート
	"data/Effekseer/hit.efkefc",			// ヒット
	"data/Effekseer/wing.efkefc",			// 羽ばたき
	"data/Effekseer/smash.efkefc",			// 潰れる
	"data/Effekseer/aura.efkefc",			// オール
};
CMyEffekseer* CMyEffekseer::m_pMyEffekseer = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CMyEffekseer::CMyEffekseer()
{
	// 変数のクリア
	time = 0;
	efkHandle = 0;
}

//==========================================================================
// デストラクタ
//==========================================================================
CMyEffekseer::~CMyEffekseer()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CMyEffekseer* CMyEffekseer::Create()
{
	if (m_pMyEffekseer == nullptr)
	{// まだ生成していなかったら

		// インスタンス生成
		m_pMyEffekseer = DEBUG_NEW CMyEffekseer;

		// 初期化処理
		m_pMyEffekseer->Init();
	}
	else
	{
		// インスタンス取得
		m_pMyEffekseer->GetInstance();
	}

	return m_pMyEffekseer;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMyEffekseer::Init()
{
	time = 0;
	efkHandle = 0;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// Effekseerのオブジェクトはスマートポインタで管理される。変数がなくなると自動的に削除される。

	// エフェクトのマネージャーの作成
	m_efkManager = ::Effekseer::Manager::Create(12800);

	// Effekseerのモジュールをセットアップする
	SetupEffekseerModules(m_efkManager);
	auto efkRenderer = GetEffekseerRenderer();

	// 座標系を設定する。アプリケーションと一致させる必要がある。
	m_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// 視点位置を確定
	viewerPosition = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	// 投影行列を設定
	projectionMatrix.PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 500.0f);

	// カメラ行列を設定
	cameraMatrix.LookAtRH(viewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));

	return S_OK;
}

//==========================================================================
// 読み込み処理
//==========================================================================
Effekseer::EffectRef CMyEffekseer::LoadEffect(std::string efkpath)
{
	// char16_tに変換
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	std::u16string string16t = converter.from_bytes(efkpath);

	return LoadProcess(string16t);
}

//==========================================================================
// エフェクト読み込み
//==========================================================================
Effekseer::EffectRef CMyEffekseer::LoadEffect(EFKLABEL label)
{
	// char16_tに変換
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	std::u16string string16t = converter.from_bytes(m_EffectName[label]);

	return LoadProcess(string16t);
}

//==========================================================================
// 読み込み処理
//==========================================================================
Effekseer::EffectRef CMyEffekseer::LoadProcess(const std::u16string& efkpath)
{
	// エフェクトの読込
	auto effect = Effekseer::Effect::Create(m_efkManager, efkpath.c_str());

	onLostDevice = [effect]() -> void
	{
		// 読み込んだエフェクトのリソースは全て破棄する。
		if (effect != nullptr)
		{
			effect->UnloadResources();
		}
	};

	onResetDevice = [effect]() -> void
	{
		// エフェクトのリソースを再読み込みする。
		if (effect != nullptr)
		{
			effect->ReloadResources();
		}
	};

	return effect;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMyEffekseer::Uninit()
{
	// 全停止
	m_efkManager->StopAllEffects();

	// 先頭を保存
	CListManager<CEffekseerObj> list = CEffekseerObj::GetListObj();
	std::list<CEffekseerObj*>::iterator itr = list.GetEnd();
	CEffekseerObj* pObj = nullptr;

	std::vector<CEffekseerObj*> deletelist;

	// リストループ
	while (list.ListLoop(itr))
	{
		deletelist.push_back((*itr));
	}

	for (const auto& obj : deletelist)
	{
		obj->Uninit();
	}

	delete m_pMyEffekseer;
	m_pMyEffekseer = nullptr;
}

//==========================================================================
// 全て停止
//==========================================================================
void CMyEffekseer::StopAll()
{
	// 全て停止
	m_efkManager->StopAllEffects();

	
}

//==========================================================================
// 更新処理
//==========================================================================
void CMyEffekseer::Update()
{
	bool bPause = CManager::GetInstance()->GetPause()->IsPause();
	if (!bPause)
	{
		// 全更新
		UpdateAll();

		// レイヤーパラメータの設定
		Effekseer::Manager::LayerParameter layerParameter;
		::Effekseer::Matrix44 invEfkCameraMatrix;
		::Effekseer::Matrix44::Inverse(invEfkCameraMatrix, cameraMatrix);
		layerParameter.ViewerPosition = ::Effekseer::Vector3D(invEfkCameraMatrix.Values[3][0], invEfkCameraMatrix.Values[3][1], invEfkCameraMatrix.Values[3][2]);
		m_efkManager->SetLayerParameter(0, layerParameter);

		// マネージャーの更新
		Effekseer::Manager::UpdateParameter updateParameter;
		m_efkManager->Update(updateParameter);
	}

	// 描画処理
	Draw();

	if (!bPause)
	{
		time++;
	}
}

//==========================================================================
// 全更新
//==========================================================================
void CMyEffekseer::UpdateAll()
{
	// 障害物のリスト取得
	CListManager<CEffekseerObj> list = CEffekseerObj::GetListObj();

	// 先頭を保存
	std::list<CEffekseerObj*>::iterator itr = list.GetEnd();
	CEffekseerObj* pObj = nullptr;

	// リストループ
	while (list.ListLoop(itr))
	{
		(*itr)->Update();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CMyEffekseer::Draw()
{
	// 時間を更新する
	efkRenderer->SetTime(time / 60.0f);

	// 投影行列を設定
	efkRenderer->SetProjectionMatrix(projectionMatrix);

	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	D3DXMATRIX appProjectionMatrix = pCamera->GetMtxProjection();

	// EffekseerのCameraMatrixは、カメラの姿勢行列の逆行列を指す。カメラ行列がカメラの姿勢行列である場合は、逆行列化しておく。
	auto invAppCameraMatrix = pCamera->GetMtxView();

	// 自分の環境とEffekseerの、投影(射影)行列とカメラ行列を同期
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			projectionMatrix.Values[i][j] = appProjectionMatrix.m[i][j];
			cameraMatrix.Values[i][j] = invAppCameraMatrix.m[i][j];
		}
	}

	// カメラ行列を設定
	efkRenderer->SetCameraMatrix(cameraMatrix);

	// エフェクトの描画開始処理を行う。
	efkRenderer->BeginRendering();

	// エフェクトの描画を行う。
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1.0f;
	drawParameter.ViewProjectionMatrix = efkRenderer->GetCameraProjectionMatrix();
	m_efkManager->Draw(drawParameter);

	// エフェクトの描画終了処理
	efkRenderer->EndRendering();
}

//==========================================================================
// モジュールのセットアップ
//==========================================================================
void CMyEffekseer::SetupEffekseerModules(::Effekseer::ManagerRef m_efkManager)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 描画デバイスの作成
	auto graphicsDevice = ::EffekseerRendererDX9::CreateGraphicsDevice(pDevice);

	// エフェクトのレンダラーの作成
	efkRenderer = ::EffekseerRendererDX9::Renderer::Create(graphicsDevice, 8000);

	// 描画モジュールの設定
	m_efkManager->SetSpriteRenderer(efkRenderer->CreateSpriteRenderer());
	m_efkManager->SetRibbonRenderer(efkRenderer->CreateRibbonRenderer());
	m_efkManager->SetRingRenderer(efkRenderer->CreateRingRenderer());
	m_efkManager->SetTrackRenderer(efkRenderer->CreateTrackRenderer());
	m_efkManager->SetModelRenderer(efkRenderer->CreateModelRenderer());

	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	m_efkManager->SetTextureLoader(efkRenderer->CreateTextureLoader());
	m_efkManager->SetModelLoader(efkRenderer->CreateModelLoader());
	m_efkManager->SetMaterialLoader(efkRenderer->CreateMaterialLoader());
	m_efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}
