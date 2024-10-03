//=============================================================================
// 
// エフェクシアオブジェクト処理 [EffekseerObj.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "EffekseerObj.h"
#include "manager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	
}
CListManager<CEffekseerObj> CEffekseerObj::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CEffekseerObj::CEffekseerObj()
{
	m_Label = CMyEffekseer::EFKLABEL::EFKLABEL_SAMPLE_LASER;	// エフェクトのラベル
	m_bDeath = false;		// 死亡フラグ
	m_bDeleteLater = false;	// 後で死亡フラグ
	m_fDeleteTimer = 0.0f;	// 死亡タイマー
}

//==========================================================================
// デストラクタ
//==========================================================================
CEffekseerObj::~CEffekseerObj()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CEffekseerObj* CEffekseerObj::Create(const CMyEffekseer::EFKLABEL& label, const MyLib::Vector3& pos, const MyLib::Vector3& rot, const MyLib::Vector3 move, float scale, bool bAutoDeath)
{
	// メモリの確保
	CEffekseerObj* pObj = DEBUG_NEW CEffekseerObj;

	if (pObj != nullptr)
	{// メモリの確保が出来ていたら

		pObj->m_EffekseerInfo = CMyEffekseer::MyEffekseerInfo(pos, rot, move, scale, nullptr, 0, bAutoDeath);	// エフェクト情報
		pObj->m_Label = label;

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEffekseerObj::Init()
{
	// マネージャ取得
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return E_FAIL;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// エフェクト読み込み
	m_EffekseerInfo.efcRef = pMyEffekseer->LoadEffect(m_Label);

	// エフェクトの再生
	m_EffekseerInfo.handle = efkManager->Play(m_EffekseerInfo.efcRef, 0.0f, 0.0f, 0.0f);

	// 情報設定
	efkManager->SetLocation(m_EffekseerInfo.handle, m_EffekseerInfo.pos.x, m_EffekseerInfo.pos.y, m_EffekseerInfo.pos.z);
	efkManager->SetRotation(m_EffekseerInfo.handle, m_EffekseerInfo.rot.x, m_EffekseerInfo.rot.y, m_EffekseerInfo.rot.z);
	efkManager->SetScale(m_EffekseerInfo.handle, m_EffekseerInfo.scale, m_EffekseerInfo.scale, m_EffekseerInfo.scale);

	// リスト追加
	m_List.Regist(this);
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEffekseerObj::Uninit()
{
	// マネージャ取得
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// エフェクトを停止する
	efkManager->StopEffect(m_EffekseerInfo.handle);

	// リスト削除
	m_List.Delete(this);

	delete this;
}

//==========================================================================
// 停止
//==========================================================================
void CEffekseerObj::Stop()
{

}

//==========================================================================
// 更新処理
//==========================================================================
void CEffekseerObj::Update()
{
	// マネージャ取得
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// 死亡
	if (m_bDeath)
	{
		return;
	}

	// 後で消す
	if (m_bDeleteLater)
	{
		m_fDeleteTimer -= CManager::GetInstance()->GetDeltaTime();
		if (m_fDeleteTimer <= 0.0f)
		{// 削除
			m_bDeath = true;
			Uninit();
			return;
		}
	}

	if (!efkManager->Exists(m_EffekseerInfo.handle))
	{// 再生終了

		// エフェクトを停止する
		efkManager->StopEffect(m_EffekseerInfo.handle);

		// 自動削除
		if (m_EffekseerInfo.bAutoDeath)
		{
			// 死亡フラグON
			m_bDeath = true;

			Uninit();
			return;
		}
		else
		{// 自動削除しない(ループ)

			// 新たにエフェクトを再生し直す。
			// 位置や回転、拡大縮小も設定
			m_EffekseerInfo.handle = efkManager->Play(m_EffekseerInfo.efcRef, 0.0f, 0.0f, 0.0f);
			efkManager->SetLocation(m_EffekseerInfo.handle, m_EffekseerInfo.pos.x, m_EffekseerInfo.pos.y, m_EffekseerInfo.pos.z);
			efkManager->SetRotation(m_EffekseerInfo.handle, m_EffekseerInfo.rot.x, m_EffekseerInfo.rot.y, m_EffekseerInfo.rot.z);
			efkManager->SetScale(m_EffekseerInfo.handle, m_EffekseerInfo.scale, m_EffekseerInfo.scale, m_EffekseerInfo.scale);
		}
	}
	else
	{// 再生中

		if (!m_EffekseerInfo.move.IsNearlyZero(0.01f))
		{
			// エフェクトの移動
			efkManager->AddLocation(
				m_EffekseerInfo.handle,
				Effekseer::Vector3D(m_EffekseerInfo.move.x, m_EffekseerInfo.move.y, m_EffekseerInfo.move.z));
		}

		// 拡大率
		Effekseer::Vector3D scale;
		efkManager->GetMatrix(m_EffekseerInfo.handle).GetScale(scale);
		efkManager->SetScale(m_EffekseerInfo.handle, m_EffekseerInfo.scale, m_EffekseerInfo.scale, m_EffekseerInfo.scale);
	}
}

//==========================================================================
// トリガー送信
//==========================================================================
void CEffekseerObj::SetTrigger(int trigger)
{
	// マネージャ取得
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// トリガー送信
	efkManager->SendTrigger(m_EffekseerInfo.handle, trigger);
}

//==========================================================================
// 後で消す
//==========================================================================
void CEffekseerObj::DeleteLater(float time)
{
	m_bDeleteLater = true;	// 後で死亡フラグ
	m_fDeleteTimer = time;	// 死亡タイマー
}

//==========================================================================
// 描画処理
//==========================================================================
void CEffekseerObj::Draw()
{
	
}

//==========================================================================
// 位置設定
//==========================================================================
void CEffekseerObj::SetPosition(const MyLib::Vector3& pos)
{
	// マネージャ取得
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// 位置設定
	m_EffekseerInfo.pos = pos;
	efkManager->SetLocation(m_EffekseerInfo.handle, Effekseer::Vector3D(pos.x, pos.y, pos.z));
}

//==========================================================================
// 向き設定
//==========================================================================
void CEffekseerObj::SetRotation(const MyLib::Vector3& rot)
{
	// マネージャ取得
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// 向き設定
	m_EffekseerInfo.rot = rot;
	efkManager->SetRotation(m_EffekseerInfo.handle, rot.x, rot.y, rot.z);
}

//==========================================================================
// マトリックス設定
//==========================================================================
void CEffekseerObj::SetMatrix(const MyLib::Matrix& mtx)
{
	// マネージャ取得
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// 軌跡のマトリックス取得
	Effekseer::Matrix43 efcmtx = GetMatrix();

	// 4x3行列に向きを設定
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			efcmtx.Value[i][j] = mtx.m[i][j];
		}
	}

	// エフェクトのインスタンスに変換行列を設定
	efkManager->SetMatrix(m_EffekseerInfo.handle, efcmtx);

	// スケール情報設定
	efkManager->SetScale(m_EffekseerInfo.handle, m_EffekseerInfo.scale, m_EffekseerInfo.scale, m_EffekseerInfo.scale);
}

//==========================================================================
// マトリックス取得
//==========================================================================
Effekseer::Matrix43 CEffekseerObj::GetMatrix()
{
	// マネージャ取得
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return Effekseer::Matrix43();
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	return efkManager->GetMatrix(m_EffekseerInfo.handle);
}

//==========================================================================
// スケール設定
//==========================================================================
void CEffekseerObj::SetScale(const float scale)
{
	// マネージャ取得
	CMyEffekseer* pMyEffekseer = CMyEffekseer::GetInstance();
	if (pMyEffekseer == nullptr) return;
	Effekseer::ManagerRef efkManager = pMyEffekseer->GetEfkManager();

	// スケール設定
	m_EffekseerInfo.scale = scale;
	efkManager->SetScale(m_EffekseerInfo.handle, scale, scale, scale);
}