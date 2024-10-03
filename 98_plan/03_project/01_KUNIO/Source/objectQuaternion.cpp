//=============================================================================
// 
//  クォータニオン処理 [objectQuaternion.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "objectQuaternion.h"
#include "calculation.h"
#include "manager.h"
#include "debugproc.h"
#include "input.h"


//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	float DEFAULT_MOVEFACTOR = 0.1f;	// デフォルトの移動補正係数
}

//==========================================================================
// コンストラクタ
//==========================================================================
CObjectQuaternion::CObjectQuaternion(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	D3DXQuaternionIdentity(&m_quaternion);
	m_vecAxis = 0.0f;			// 回転軸
	m_fValueRot = 0.0f;			// 回転角
	m_fMoveValueRot = 0.0f;		// 回転角の移動量
	m_fMoveFactor = 0.0f;		// 移動量の補正係数
	m_RotationMtx.Identity();	// 計算用マトリックス宣言
}

//==========================================================================
// デストラクタ
//==========================================================================
CObjectQuaternion::~CObjectQuaternion()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObjectQuaternion* CObjectQuaternion::Create()
{
	// メモリの確保
	CObjectQuaternion* pObjChara = DEBUG_NEW CObjectQuaternion;

	if (pObjChara != nullptr)
	{
		// 初期化処理
		HRESULT hr = pObjChara->Init();
		if (FAILED(hr)){
			return nullptr;
		}
	}

	return pObjChara;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectQuaternion::Init()
{
	CObjectX::Init("data\\MODEL\\box.x");
	CObject::SetType(CObject::TYPE::TYPE_OBJECTX);

	m_fMoveFactor = DEFAULT_MOVEFACTOR;		// 移動量の補正係数

	return S_OK;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectQuaternion::Init(const std::string& file)
{
	CObjectX::Init(file);
	CObject::SetType(CObject::TYPE::TYPE_OBJECTX);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObjectQuaternion::Uninit()
{
	// 終了処理
	CObjectX::Uninit();

	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObjectQuaternion::Update()
{
	// オブジェクトXの更新処理
	CObjectX::Update();
#if 0
	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();
	MyLib::Vector3 rot = GetRotation();


	MyLib::Vector3 vec = MyLib::Vector3(sinf(D3DX_PI + rot.y), 0.0f, cosf(D3DX_PI + rot.y));

	m_vecAxis.x = vec.z;
	m_vecAxis.z = -vec.x;



	// 位置更新
	pos += move;

	// 回転角設定
	m_fValueRot = move.x * 0.005f;

	move.x += (0.0f - move.x) * 0.05f;


	// 設定
	SetPosition(pos);
	SetMove(move);
	SetRotation(rot);
#endif

	//// 回転角加算
	//m_fValueRot = m_fMoveValueRot;
	//m_fMoveValueRot += (0.0f - m_fMoveValueRot) * m_fMoveFactor;

}

//==========================================================================
// クォータニオン反映
//==========================================================================
void CObjectQuaternion::BindQuaternion(const MyLib::Vector3& vecAxis, float valueRot)
{
	MyLib::Matrix mtxRot;	// 計算用マトリックス宣言

	D3DXQuaternionIdentity(&m_quaternion);

	// 任意の回転軸における回転角からクォータニオン計算
	D3DXQuaternionRotationAxis(&m_quaternion, &vecAxis, valueRot);

	// 回転マトリックスを作成
	mtxRot.Identity();
	D3DXMATRIX calRotMtx = mtxRot.ConvertD3DXMATRIX();
	D3DXMatrixRotationQuaternion(&calRotMtx, &m_quaternion);
	mtxRot = calRotMtx;

	// クォータニオンを正規化
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	// 向きを反映する
	m_RotationMtx.Multiply(m_RotationMtx, mtxRot);
}

//==========================================================================
// クォータニオン計算
//==========================================================================
void CObjectQuaternion::CalQuaternion()
{
	MyLib::Matrix mtxRot;	// 計算用マトリックス宣言

	D3DXQuaternionIdentity(&m_quaternion);

	// 任意の回転軸における回転角からクォータニオン計算
	D3DXQuaternionRotationAxis(&m_quaternion, &m_vecAxis, m_fValueRot);

	// 回転マトリックスを作成
	mtxRot.Identity();
	D3DXMATRIX calRotMtx = mtxRot.ConvertD3DXMATRIX();
	D3DXMatrixRotationQuaternion(&calRotMtx, &m_quaternion);
	mtxRot = calRotMtx;

	// クォータニオンを正規化
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);


	// 向きを反映する
	m_RotationMtx.Multiply(m_RotationMtx, mtxRot);
}

//==========================================================================
// ワールドマトリックスの計算処理
//==========================================================================
void CObjectQuaternion::CalWorldMtx()
{
#if 1

	// クォータニオン計算
	//CalQuaternion();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	MyLib::Matrix mtxTrans, mtxScale;	// 計算用マトリックス宣言
	MyLib::Matrix mtxWorld = GetWorldMtx();

	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 scale = GetScale();

	// 初期化
	mtxWorld.Identity();

	// スケールを反映する
	mtxScale.Scaling(scale);
	mtxWorld.Multiply(mtxWorld, mtxScale);

	// 向きを反映する
	mtxWorld.Multiply(mtxWorld, m_RotationMtx);

	// 位置を反映する
	mtxTrans.Translation(pos);
	mtxWorld.Multiply(mtxWorld, mtxTrans);

	// ワールドマトリックスの設定
	D3DXMATRIX setMtx = mtxWorld.ConvertD3DXMATRIX();
	pDevice->SetTransform(D3DTS_WORLD, &setMtx);
	SetWorldMtx(mtxWorld);
#else


	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	MyLib::Matrix mtxRot, mtxTrans;	// 計算用マトリックス宣言

	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 rotOld = GetOldRotation();
	MyLib::Matrix mtxWorld = GetWorldMtx();


	// ワールドマトリックスの初期化
	mtxWorld.Identity();
	D3DXQuaternionIdentity(&m_quaternion);

	MyLib::Vector3 vec;

	D3DXQUATERNION quatX;
	vec = { 1.0f, 0.0f, 0.0f };
	D3DXQuaternionRotationAxis(&quatX, &vec, rotOld.x);

	D3DXQUATERNION quatZ;
	vec = { 0.0f, 0.0f, 1.0f };
	D3DXQuaternionRotationAxis(&quatZ, &vec, rotOld.z);

	D3DXQUATERNION quatY;
	vec = { 0.0f, 1.0f, 0.0f };
	D3DXQuaternionRotationAxis(&quatY, &vec, rotOld.y);

	// x軸、y軸、z軸の順で回転を適用
	m_quaternion = quatY * m_quaternion;
	m_quaternion = quatX * m_quaternion;
	m_quaternion = quatZ * m_quaternion;

	// クォータニオンを正規化
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	// クォータニオンを組み合わせて回転マトリックスを作成
	mtxRot.Identity();
	D3DXMATRIX D3DmtxRot = mtxRot.ConvertD3DXMATRIX();
	D3DXMatrixRotationQuaternion(&D3DmtxRot, &m_quaternion);
	mtxRot = D3DmtxRot;

	// 向きを反映する
	mtxWorld.Multiply(mtxWorld, mtxRot);


	// 位置を反映する
	mtxTrans.Translation(pos);
	mtxWorld.Multiply(mtxWorld, mtxTrans);

	// ワールドマトリックスの設定
	D3DXMATRIX mtx = mtxWorld.ConvertD3DXMATRIX();
	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	SetWorldMtx(mtxWorld);
#endif
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectQuaternion::Draw()
{
	// ワールドマトリックスの計算処理
	CalWorldMtx();

	// 描画のみ
	CObjectX::DrawOnly();
}
