//=============================================================================
// 
//  アーチ状の魚障害物処理(実体なし) [obstacle_fisharch.cpp]
//  Author : Ibuki Okusada
// 
//=============================================================================
#include "obstacle_fisharch.h"
#include "debugproc.h"
#include "manager.h"
#include "baggage.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int NUM_FISH = (10);				// 魚の総数
	const float FISH_ROT = (1.0f / NUM_FISH);	// 1魚辺りの角度割合
	const int RANGE_MOTION = 2;					// モーションする
}

// デフォルト情報
namespace FISHDEFAULT
{
	const float ROTATE_SPEED = (-0.02f);		// 回転速度
	const float WIDTH = (750.0f);			// 中心からの距離
	const float HEIGHT = (1500.0f);			// 変化する距離
	const float PLUS_HEIGHT = (0.0f);		// 加算高さ
}

//==========================================================================
// コンストラクタ
//==========================================================================
CObstacle_FishArch::CObstacle_FishArch(int nPriority,
	CObject::LAYER layer) : CMap_Obstacle(nPriority, layer)
{
	// 値のクリア
	m_FishList.clear();
	m_fRot = 0.0f;
	m_scale = (1.0f);
}

//==========================================================================
// デストラクタ
//==========================================================================
CObstacle_FishArch::~CObstacle_FishArch()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObstacle_FishArch* CObstacle_FishArch::Create(const CMap_ObstacleManager::SObstacleInfo& info)
{
	// メモリの確保
	CObstacle_FishArch* pObj = DEBUG_NEW CObstacle_FishArch;

	if (pObj != nullptr)
	{
		pObj->SetObstacleInfo(info);
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObstacle_FishArch::Init()
{
	// 自分のブロック計算
	CMap_Obstacle::CalMyBlock();

	CMap_Obstacle::ListRegist(this);
	MyLib::Vector3 rot;

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 魚を生成する
	for (int i = 0; i < NUM_FISH; i++)
	{
		FishInfo info;
		info.nIdx = i;
		info.pFish = CMap_Obstacle::Create(GetObstacleInfo(), GetPosition(), false, false);
		rot.x = (i * FISH_ROT) * (D3DX_PI * 2);
		UtilFunc::Transformation::RotNormalize(rot.x);
		info.pFish->SetRotation(rot);

		// 魚のオフセット設定
		SetFishOffSet(info);
		info.pFish->SetPosition(info.offset);
		//info.pFish->CalMyBlock();
		m_FishList.push_back(info);
	}

	// 初期値設定
	m_Info.fDefHeight = FISHDEFAULT::HEIGHT;
	m_Info.fPlusHeight = FISHDEFAULT::PLUS_HEIGHT;
	m_Info.fRotSpeed = FISHDEFAULT::ROTATE_SPEED;
	m_Info.fNowHeight = FISHDEFAULT::HEIGHT;

	// メイン操作
	MainControll();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObstacle_FishArch::Uninit()
{
	for (auto& it : m_FishList)
	{
		if (it.pFish != nullptr)
		{
			it.pFish->Uninit();
			it.pFish = nullptr;
		}
	}

	// リストクリア
	m_FishList.clear();

	// 終了処理
	CMap_Obstacle::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CObstacle_FishArch::Kill()
{
	for (auto& it : m_FishList)
	{
		if (it.pFish != nullptr)
		{
			it.pFish->Kill();
			it.pFish = nullptr;
		}
	}

	// リストクリア
	m_FishList.clear();

	// 終了処理
	CMap_Obstacle::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObstacle_FishArch::Update()
{

#if _DEBUG
	// マトリックス設定
	CalWorldMtx();
	CMap_Obstacle::Update();
#endif // _DEBUG

	// 荷物取得
	CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);
	if (pBaggage == nullptr) return;

	// 荷物のブロックの範囲外なら抜ける
	int baggageBlock = pBaggage->GetMapBlock();
	if (!(baggageBlock + RANGE_MOTION >= m_nMapBlock &&
		baggageBlock - RANGE_MOTION <= m_nMapBlock))
	{
		return;
	}

	// メイン操作
	MainControll();
}

//==========================================================================
// メイン操作
//==========================================================================
void CObstacle_FishArch::MainControll()
{
	// 回転
	m_fRot += m_Info.fRotSpeed;
	UtilFunc::Transformation::RotNormalize(m_fRot);

	// 高さ設定
	SetNowHeight();

	// マトリックス設定
	CalWorldMtx();

	// 魚の更新
	ControllFish();
}

//==========================================================================
// 描画処理
//==========================================================================
void CObstacle_FishArch::Draw()
{
	
}

//==========================================================================
// 魚の操作設定
//==========================================================================
void CObstacle_FishArch::ControllFish()
{
	for (auto& it : m_FishList)
	{
		if (it.pFish == nullptr) continue;	// 存在していない

		MyLib::Vector3 rot = it.pFish->GetRotation();

		// 位置の設定
		{
			// 過去の位置設定
			it.pFish->SetOldPosition(it.pFish->GetPosition());

			MyLib::Matrix mtxRot, mtxTrans, mtxScale, mtxFish;	// 計算用マトリックス宣言
			MyLib::Matrix mtxParent = m_mtxWorld;	// 親のマトリックス
			MyLib::Vector3 FishPos = it.offset;

			// 座標用マトリックス初期化
			mtxFish.Identity();

			// 位置を反映する
			mtxTrans.Translation(FishPos);
			mtxFish.Multiply(mtxFish, mtxTrans);

			//// 向きを反映する
			//mtxRot.RotationYawPitchRoll(GetRotation().y, GetRotation().x, GetRotation().z);
			//mtxFish.Multiply(mtxFish, mtxRot);

			mtxParent.Multiply(mtxFish, mtxParent);

			// 座標を設定
			FishPos = (mtxFish._41, mtxFish._42, mtxFish._43);

			MyLib::Vector3 setpos = mtxParent.GetWorldPosition();
			if (it.pFish->GetOldPosition().y > 0.0f &&
				setpos.y <= 0.0f)
			{
				// 入水
				CEffekseerObj::Create(
					CMyEffekseer::EFKLABEL::EFKLABEL_WATERJUMP,
					setpos, MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 40.0f, true);

				// サウンド再生
				CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_INTHEWATER2);
			}

			// 位置設定
			it.pFish->SetPosition(setpos);
		}

		// 向きとオフセット設定
		{	
			rot.x = m_fRot + (D3DX_PI * 2) * (FISH_ROT * it.nIdx);
			rot.y = GetRotation().y;
			UtilFunc::Transformation::RotNormalize(rot.x);
			it.pFish->SetRotation(rot);
			SetFishOffSet(it);
			it.pFish->SetScale(GetScale() + 5.0f);
		}
	}
}

//==========================================================================
// 魚のオフセット設定
//==========================================================================
void CObstacle_FishArch::SetFishOffSet(FishInfo& info)
{
	float rot = m_fRot;
	rot += (D3DX_PI * 2) * (FISH_ROT * info.nIdx);
	UtilFunc::Transformation::RotNormalize(rot);

	// 割合を求める
	float rate = rot;

	rate = fabsf(fabsf(rate) / D3DX_PI - 1.0f);

	info.offset.z = sinf(rot) * FISHDEFAULT::WIDTH;
	info.offset.y = cosf(rot) * m_Info.fNowHeight;
}

//==========================================================================
// 可変情報設定
//==========================================================================
void CObstacle_FishArch::SetInfo(const float fDefHeight, const float fPlusHeight, const float fRotSpd)
{
	// 値の設定
	m_Info.fDefHeight = fDefHeight;
	m_Info.fPlusHeight = fPlusHeight;
	m_Info.fRotSpeed = fRotSpd;
}

//==========================================================================
// 現在の距離設定
//==========================================================================
void CObstacle_FishArch::SetNowHeight()
{
	// 本体の向きをから加算距離を設定
	float rate = sinf(m_fRot);
	m_Info.fNowHeight = m_Info.fDefHeight + (m_Info.fPlusHeight * rate);
}

//==========================================================================
// ワールドマトリックス設定
//==========================================================================
void CObstacle_FishArch::CalWorldMtx() 
{// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	MyLib::Matrix mtxRot, mtxTrans, mtxScale;	// 計算用マトリックス宣言

	// ワールドマトリックスの初期化
	m_mtxWorld.Identity();

	// スケールを反映する
	mtxScale.Scaling(GetScale());
	m_mtxWorld.Multiply(m_mtxWorld, mtxScale);

	// 向きを反映する
	mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRot);

	// 位置を反映する
	mtxTrans.Translation(pos);
	m_mtxWorld.Multiply(m_mtxWorld, mtxTrans);
}