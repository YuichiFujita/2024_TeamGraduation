//=============================================================================
// 
//  円形で回転する鳥障害物処理(実体なし) [obstacle_birdcircle.cpp]
//  Author : Ibuki Okusada
// 
//=============================================================================
#include "obstacle_birdcircle.h"
#include "debugproc.h"
#include "manager.h"
#include "baggage.h"


//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int	NUM_BIRD = (10);				// 鳥の総数
	const float BIRD_ROT = (1.0f / NUM_BIRD);	// 1鳥辺りの角度割合
	const int RANGE_MOTION = 2;					// モーションする
	const float INTERVAL_WING = 0.8f;
}

// デフォルト情報
namespace BIRDDEFAULT
{
	const float ROTATE_SPEED = (0.02f);	// 回転速度
	const float LENGTH = (750.0f);			// 中心からの距離
	const float PLUSLENGTH = (250.0f);		// 変化する距離
}

//==========================================================================
// コンストラクタ
//==========================================================================
CObstacle_BirdCircle::CObstacle_BirdCircle(int nPriority,
	CObject::LAYER layer) : CMap_Obstacle(nPriority, layer)
{
	// 値のクリア
	m_BirdList.clear();
	m_scale = 1.0f;
	m_fIntervalWing = 0.0f;		// 羽ばたきの間隔
}

//==========================================================================
// デストラクタ
//==========================================================================
CObstacle_BirdCircle::~CObstacle_BirdCircle()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObstacle_BirdCircle* CObstacle_BirdCircle::Create(const CMap_ObstacleManager::SObstacleInfo& info)
{
	// メモリの確保
	CObstacle_BirdCircle* pObj = DEBUG_NEW CObstacle_BirdCircle;

	if (pObj != nullptr)
	{
		pObj->SetObstacleInfo(info);
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObstacle_BirdCircle::Init()
{
	// 自分のブロック計算
	CMap_Obstacle::CalMyBlock();
	CMap_Obstacle::ListRegist(this);

	
	MyLib::Vector3 rot;

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 鳥を生成する
	for (int i = 0; i < NUM_BIRD; i++)
	{
		BirdInfo info;
		info.nIdx = i;
		info.pBird = CMap_Obstacle::Create(GetObstacleInfo(), GetPosition(), false, false);
		rot.y = (i * BIRD_ROT) * (D3DX_PI * 2);
		UtilFunc::Transformation::RotNormalize(rot.y);
		info.pBird->SetRotation(rot);
		SetBirdOffSet(info);
		info.pBird->SetPosition(info.offset);
		m_BirdList.push_back(info);
	}

	// 初期値設定
	m_Info.fDefLength = BIRDDEFAULT::LENGTH;
	m_Info.fPlusLength = BIRDDEFAULT::PLUSLENGTH;
	m_Info.fRotSpeed = BIRDDEFAULT::ROTATE_SPEED;
	m_Info.fNowLength = BIRDDEFAULT::LENGTH;

	// メイン操作
	MainControll();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObstacle_BirdCircle::Uninit()
{
	for (auto& it : m_BirdList)
	{
		if (it.pBird != nullptr)
		{
			it.pBird->Uninit();
			it.pBird = nullptr;
		}
	}

	// リストクリア
	m_BirdList.clear();

	// 終了処理
	CMap_Obstacle::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CObstacle_BirdCircle::Kill()
{
	for (auto& it : m_BirdList)
	{
		if (it.pBird != nullptr)
		{
			it.pBird->Kill();
			it.pBird = nullptr;
		}
	}

	// リストクリア
	m_BirdList.clear();

	// 終了処理
	CMap_Obstacle::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObstacle_BirdCircle::Update()
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


	// サウンド再生
	m_fIntervalWing += CManager::GetInstance()->GetDeltaTime();
	if (m_fIntervalWing >= INTERVAL_WING)
	{
		m_fIntervalWing = 0.0f;
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_HABATAKI);
		
		for (auto& it : m_BirdList)
		{
			if (it.pBird == nullptr) continue;	// 存在していない

			MyLib::Vector3 rot = it.pBird->GetRotation();

			// 位置の設定
			{
				MyLib::Matrix mtxRot, mtxTrans, mtxScale, mtxBird;	// 計算用マトリックス宣言
				MyLib::Matrix mtxParent = m_mtxWorld;	// 親のマトリックス
				MyLib::Vector3 BirdPos = it.offset;

				// 羽ばたきエフェクト生成
				CEffekseerObj::Create(
					CMyEffekseer::EFKLABEL::EFKLABEL_WING,
					it.pBird->GetPosition(), MyLib::Vector3(0.0f, m_rot.y, 0.0f), 0.0f, 10.0f, true);
			}
		}
	}

	// メイン操作
	MainControll();
}

//==========================================================================
// メイン操作
//==========================================================================
void CObstacle_BirdCircle::MainControll()
{
	// 回転(yは向き、xは移動に使用)
	m_rot.y += m_Info.fRotSpeed;
	m_rot.x -= m_Info.fRotSpeed;
	UtilFunc::Transformation::RotNormalize(m_rot.y);
	UtilFunc::Transformation::RotNormalize(m_rot.x);

	// 距離設定
	SetNowLength();

	// マトリックス設定
	CalWorldMtx();

	// 鳥の更新
	ControllBird();
}

//==========================================================================
// 描画処理
//==========================================================================
void CObstacle_BirdCircle::Draw()
{

}

//==========================================================================
// 鳥の操作設定
//==========================================================================
void CObstacle_BirdCircle::ControllBird()
{
	for (auto& it : m_BirdList)
	{
		if (it.pBird == nullptr) continue;	// 存在していない

		MyLib::Vector3 rot = it.pBird->GetRotation();

		// 位置の設定
		{
			MyLib::Matrix mtxRot, mtxTrans, mtxScale, mtxBird;	// 計算用マトリックス宣言
			MyLib::Matrix mtxParent = m_mtxWorld;	// 親のマトリックス
			MyLib::Vector3 BirdPos = it.offset;

			// 座標用マトリックス初期化
			mtxBird.Identity();

			// 位置を反映する
			mtxTrans.Translation(BirdPos);
			mtxBird.Multiply(mtxBird, mtxTrans);

			mtxParent.Multiply(mtxBird, mtxParent);

			// 座標を設定
			BirdPos = (mtxBird._41, mtxBird._42, mtxBird._43);

			it.pBird->SetPosition(mtxParent.GetWorldPosition());
		}

		// 向きとオフセット設定
		{
			rot.y = m_rot.y + (D3DX_PI * 2.0f) * (BIRD_ROT * it.nIdx) + D3DX_PI * 0.5f;
			UtilFunc::Transformation::RotNormalize(rot.y);
			it.pBird->SetRotation(rot);
			SetBirdOffSet(it);
			it.pBird->SetScale(GetScale() + 1.0f);
		}
	}
}

//==========================================================================
// 鳥のオフセット設定
//==========================================================================
void CObstacle_BirdCircle::SetBirdOffSet(BirdInfo& info)
{
	// 自身の存在する向きを取得
	MyLib::Vector3 rot = m_rot;
	rot.x = m_rot.x + (info.nIdx * BIRD_ROT) * (-D3DX_PI * 2);
	UtilFunc::Transformation::RotNormalize(rot.y);

	// 割合を求める
	float rate = rot.x;
	rate = fabsf(fabsf(rate) / D3DX_PI - 1.0f);

	// オフセットを設定
	info.offset.z = sinf(rot.x) * m_Info.fNowLength;
	info.offset.x = cosf(rot.x) * m_Info.fNowLength;
}

//==========================================================================
// 現在の距離設定
//==========================================================================
void CObstacle_BirdCircle::SetNowLength()
{
	// 本体の向きをから加算距離を設定
	MyLib::Vector3 rot = m_rot;
	float rate = sinf(rot.x);
	m_Info.fNowLength = m_Info.fDefLength + (m_Info.fPlusLength * rate);
}

//==========================================================================
// 可変情報設定
//==========================================================================
void CObstacle_BirdCircle::SetInfo(const float fDefLen, const float fPlusLen, const float fRotSpd)
{
	// 値の設定
	m_Info.fDefLength = fDefLen;
	m_Info.fPlusLength = fPlusLen;
	m_Info.fRotSpeed = fRotSpd;
}

//==========================================================================
// ワールドマトリックス設定
//==========================================================================
void CObstacle_BirdCircle::CalWorldMtx()
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