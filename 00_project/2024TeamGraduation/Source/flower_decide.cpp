//=============================================================================
// 
//  完成花処理 [flower_decide.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "flower_decide.h"
#include "manager.h"
#include "sound.h"
#include "particle.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* MODEL[] =	// モデルファイル
	{
		"data\\MODEL\\flower\\blue.x",
		"data\\MODEL\\flower\\pink.x",
		"data\\MODEL\\flower\\ranunculus.x",
		"data\\MODEL\\flower\\red.x",
		"data\\MODEL\\flower\\white.x",
	};
		
	const int MAX_SPAWNNUM = 2048;		// 最大生成数
	const float TIME_CHARGE = 3.0f;		// チャージ時間
	const int TIME_FLOWERING = 60;	// 開花時間
}

//==========================================================================
// コンストラクタ
//==========================================================================
CDecideFlower::CDecideFlower(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CDecideFlower::~CDecideFlower()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CDecideFlower* CDecideFlower::Create(const MyLib::Vector3& pos, const MyLib::Vector3& move)
{
	// メモリの確保
	CDecideFlower* pBullet = DEBUG_NEW CDecideFlower;

	if (pBullet != nullptr)
	{// メモリの確保が出来ていたら

		// 割り当て
		pBullet->SetPosition(pos);
		pBullet->SetOriginPosition(pos);
		pBullet->SetMove(move);

		// 初期化処理
		pBullet->Init();
	}

	return pBullet;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CDecideFlower::Init()
{
	// 種類の設定
	CObject::SetType(TYPE::TYPE_OBJECTX);

	int rand = UtilFunc::Transformation::Random(0, 4);

	// 初期化処理
	HRESULT hr = CObjectX::Init(MODEL[rand]);
	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CDecideFlower::Uninit()
{

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CDecideFlower::Update()
{
	MyLib::Vector3 move = GetMove();
	MyLib::Vector3 pos = GetPosition();
	pos += move;

	move.x += (0.0f - move.x) * 0.1f;
	move.z += (0.0f - move.z) * 0.1f;

	SetPosition(pos);
	SetMove(move);

}
