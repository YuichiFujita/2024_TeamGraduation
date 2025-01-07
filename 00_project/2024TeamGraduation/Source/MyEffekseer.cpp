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
	"data/Effekseer/Laser01.efkefc",					// サンプルのレーザー
	"data/Effekseer/ball.efkefc",						// ボール
	"data/Effekseer/throwLine_normal.efkefc",			// 投げた時の線(通常)
	"data/Effekseer/throwLine_fast.efkefc",				// 投げた時の線(早い)
	"data/Effekseer/throwLine_pass.efkefc",				// 投げた時の線(パス)
	"data/Effekseer/kamehame_chargeStart.efkefc",		// かめはめ波(開始)
	"data/Effekseer/kamehame_aura.efkefc",				// かめはめ波(チャージ)
	"data/Effekseer/kamehame_chargeWind.efkefc",		// かめはめ波(チャージ)(風)
	"data/Effekseer/kamehame_chargeThunder.efkefc",		// かめはめ波(チャージ)(雷)
	"data/Effekseer/kamehame_atmosphere.efkefc",		// かめはめ波(空間オーラ)
	"data/Effekseer/kamehame_brust.efkefc",				// かめはめ波(発射)
	"data/Effekseer/kamehame_Flash.efkefc",				// かめはめ波(フラッシュ)
	"data/Effekseer/kamehame_ballast.efkefc",			// かめはめ波(空間がれき)
	"data/Effekseer/hit.efkefc",						// ヒット
	"data/Effekseer/bound.efkefc",						// バウンド
	"data/Effekseer/sweat.efkefc",						// 汗
	"data/Effekseer/walk.efkefc",						// 歩き
	"data/Effekseer/run.efkefc",						// 走り
	"data/Effekseer/blink.efkefc",						// ブリンク
	"data/Effekseer/jump.efkefc",						// ジャンプ
	"data/Effekseer/land.efkefc",						// 着地
	"data/Effekseer/cantch_stance.efkefc",				// キャッチ(構え)
	"data/Effekseer/cantch_normal.efkefc",				// キャッチ(通常)
	"data/Effekseer/cantch_just.efkefc",				// キャッチ(ジャスト)
	"data/Effekseer/confetti.efkefc",					// 紙吹雪
	"data/Effekseer/crown.efkefc",						// 王冠
	"data/Effekseer/logo.efkefc",						// ロゴ
	"data/Effekseer/logo_always.efkefc",				// ロゴ(常に出現)
	"data/Effekseer/sandsmoke.efkefc",					// 砂煙
};
CMyEffekseer* CMyEffekseer::m_pMyEffekseer = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CMyEffekseer::CMyEffekseer()
{
	// 変数のクリア
	fTime = 0.0f;
	efkHandle = 0;
	m_vecLoadInfo.clear();	// 読み込み情報
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
	fTime = 0.0f;
	efkHandle = 0;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

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

	// 読み込み情報
	m_vecLoadInfo.clear();

	return S_OK;
}

//==========================================================================
// 読み込み処理
//==========================================================================
Effekseer::EffectRef CMyEffekseer::LoadEffect(std::string efkpath)
{
	// char16_tに変換
	std::u16string string16t = UtilFunc::Transformation::ConvertUtf8ToUtf16(efkpath);

	return LoadProcess(string16t);
}

//==========================================================================
// エフェクト読み込み
//==========================================================================
Effekseer::EffectRef CMyEffekseer::LoadEffect(EEfkLabel label)
{
	// char16_tに変換
	std::u16string string16t = UtilFunc::Transformation::ConvertUtf8ToUtf16(m_EffectName[label]);

	return LoadProcess(string16t);
}

//==========================================================================
// 読み込み処理
//==========================================================================
Effekseer::EffectRef CMyEffekseer::LoadProcess(const std::u16string& efkpath)
{
	// 構造体の中の文字列を探す
	auto itr = std::find_if(m_vecLoadInfo.begin(), m_vecLoadInfo.end(),
		[&efkpath](const CMyEffekseer::SLoadInfo& info) {return info.filename == efkpath; });

	// 見つかった場合
	if (itr != m_vecLoadInfo.end())
	{// 読み込み済み情報を返す
		return (*itr).ref;
	}

	// エフェクトの読込
	Effekseer::EffectRef effect = Effekseer::Effect::Create(m_efkManager, efkpath.c_str());

	onLostDevice = [effect]() -> void
	{
		// 読み込んだエフェクトのリソースは全て破棄する。
		if (effect != nullptr)
		{
			assert(false);
			effect->UnloadResources();
		}
	};

	onResetDevice = [effect]() -> void
	{
		// エフェクトのリソースを再読み込みする。
		if (effect != nullptr)
		{
			assert(false);
			effect->ReloadResources();
		}
	};

	// 読み込み情報追加
	m_vecLoadInfo.emplace_back();
	m_vecLoadInfo.back().filename = efkpath;	// ファイル名
	m_vecLoadInfo.back().ref = effect;			// リファレンス

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

	// 障害物のリスト取得
	CListManager<CEffekseerObj> list = CEffekseerObj::GetListObj();

	// 先頭を保存
	std::list<CEffekseerObj*>::iterator itr = list.GetEnd();
	CEffekseerObj* pObj = nullptr;

	// リストループ
	while (list.ListLoop(itr))
	{
		(*itr)->Uninit();
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CMyEffekseer::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	bool bPause = CManager::GetInstance()->GetPause()->IsPause();
	if (!bPause)
	{
		// 全更新
		UpdateAll(fDeltaTime, fDeltaRate, fSlowRate);

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
		fTime += fDeltaTime * fSlowRate;
	}

#if _DEBUG
	// デバッグ
	Debug();
#endif
}

//==========================================================================
// 全更新
//==========================================================================
void CMyEffekseer::UpdateAll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 障害物のリスト取得
	CListManager<CEffekseerObj> list = CEffekseerObj::GetListObj();

	// 先頭を保存
	std::list<CEffekseerObj*>::iterator itr = list.GetEnd();
	CEffekseerObj* pObj = nullptr;

	// リストループ
	while (list.ListLoop(itr))
	{
		(*itr)->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CMyEffekseer::Draw()
{
	// 時間を更新する
	efkRenderer->SetTime(fTime);

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

//==========================================================================
// デバッグ
//==========================================================================
void CMyEffekseer::Debug()
{

	if (ImGui::TreeNode("Effekseer"))
	{
		//--------------------------
		// ラベル切り替え
		//--------------------------
		int label = m_debugInfo.label;
		ImGui::SliderInt("Label", &label, 0, static_cast<int>(EEfkLabel::EFKLABEL_MAX) - 1, "%d");
		ImGui::Text("%s", magic_enum::enum_name(m_debugInfo.label));
		m_debugInfo.label = static_cast<EEfkLabel>(label);

		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		//--------------------------
		// 生成
		//--------------------------
		if (ImGui::Button("Create"))
		{
			if (m_debugInfo.pEfkObj != nullptr)
			{
				m_debugInfo.pEfkObj->Uninit();
				m_debugInfo.pEfkObj = nullptr;
			}

			m_debugInfo.pEfkObj = CEffekseerObj::Create(m_debugInfo.label,
				m_debugInfo.pos,
				m_debugInfo.rot,
				MyLib::Vector3(),
				m_debugInfo.scale, false);
		}

		//--------------------------
		// トリガー送信
		//--------------------------
		if (ImGui::Button("Trigger00") &&
			m_debugInfo.pEfkObj != nullptr)
		{
			m_debugInfo.pEfkObj->SetTrigger(0);
		}
		ImGui::SameLine();

		if (ImGui::Button("Trigger01") &&
			m_debugInfo.pEfkObj != nullptr)
		{
			m_debugInfo.pEfkObj->SetTrigger(1);
		}
		ImGui::SameLine();

		if (ImGui::Button("Trigger02") &&
			m_debugInfo.pEfkObj != nullptr)
		{
			m_debugInfo.pEfkObj->SetTrigger(2);
		}
		ImGui::SameLine();

		if (ImGui::Button("Trigger03") &&
			m_debugInfo.pEfkObj != nullptr)
		{
			m_debugInfo.pEfkObj->SetTrigger(3);
		}

		//--------------------------
		// 位置
		//--------------------------
		if (ImGui::Button("pos Reset"))
		{// リセット
			m_debugInfo.pos = MyLib::Vector3();
		}
		ImGui::SameLine();

		ImGui::DragFloat3("pos", (float*)&m_debugInfo.pos, 1.0f, 0.0f, 0.0f, "%.2f");

		//--------------------------
		// 向き
		//--------------------------
		if (ImGui::Button("rot Reset"))
		{// リセット
			m_debugInfo.rot = MyLib::Vector3();
		}
		ImGui::SameLine();

		ImGui::DragFloat3("rot", (float*)&m_debugInfo.rot, D3DX_PI * 0.01f, 0.0f, 0.0f, "%.2f");
		UtilFunc::Transformation::RotNormalize(m_debugInfo.rot);


		//--------------------------
		// スケール
		//--------------------------
		if (ImGui::Button("scale Reset"))
		{// リセット
			m_debugInfo.scale = 10.0f;
		}
		ImGui::SameLine();

		ImGui::DragFloat("scale", (float*)&m_debugInfo.scale, 0.1f, 0.0f, 100.0f, "%.2f");


		ImGui::TreePop();
	}


	// 情報反映
	if (m_debugInfo.pEfkObj == nullptr) return;

	m_debugInfo.pEfkObj->SetPosition(m_debugInfo.pos);
	m_debugInfo.pEfkObj->SetRotation(m_debugInfo.rot);
	m_debugInfo.pEfkObj->SetScale(m_debugInfo.scale);

}
