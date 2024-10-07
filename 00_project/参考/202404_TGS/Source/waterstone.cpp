//=============================================================================
// 
//  水中石処理 [waterstone.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "waterstone.h"
#include "manager.h"
#include "calculation.h"
#include "3D_effect.h"
#include "player.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	float DEFAULT_SPLASHTIME = 0.14f;	// 通常のしぶき時間
	const float UPDATE_LEN = 8000.0f;	// 更新の距離
}
CListManager<CWaterStone> CWaterStone::m_List = {};				// リスト
const std::vector<std::string> CWaterStone::m_vecModelFile =	// モデルファイルのコンテナ
{
	"data\\MODEL\\map_object\\rock_01_small.x",
	"data\\MODEL\\map_object\\rock_02.x",
	"data\\MODEL\\map_object\\rock_03.x",
	"data\\MODEL\\map_object\\rock_04.x",
	"data\\MODEL\\map_object\\rock_05.x",
};

//==========================================================================
// コンストラクタ
//==========================================================================
CWaterStone::CWaterStone(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_fSplashTimer = 0.0f;		// しぶきタイマー
	m_fIntervalSplash = 0.0f;	// しぶきまでのインターバル
}

//==========================================================================
// デストラクタ
//==========================================================================
CWaterStone::~CWaterStone()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CWaterStone *CWaterStone::Create(const CWaterStone_Manager::SStoneInfo& info)
{
	// メモリの確保
	CWaterStone* pObj = DEBUG_NEW CWaterStone;

	if (pObj != nullptr)
	{
		// 引数情報設定
		pObj->m_StoneInfo = info;

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CWaterStone::Init()
{
	// 位置設定
	SetPosition(m_StoneInfo.pos);

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 初期化処理
	HRESULT hr = CObjectX::Init(m_vecModelFile[m_StoneInfo.type]);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// リスト追加
	m_List.Regist(this);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CWaterStone::Uninit()
{
	// リスト削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CWaterStone::Kill()
{
	// リスト削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CWaterStone::Update()
{
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(0);
	MyLib::Vector3 playerPos = pPlayer->GetPosition();

	if (GetPosition().x > playerPos.x + UPDATE_LEN ||
		GetPosition().x < playerPos.x - UPDATE_LEN)
	{
		return;
	}

	// しぶきタイマー加算
	m_fSplashTimer += CManager::GetInstance()->GetDeltaTime();

#if 0
	static int createIdx = 8;
	static int Life = 50;
	static int Randrange = 28;
	static int posXZrange = 50;
	static int XZrange = 117;
	static int MinYRange = 37;
	static int MaxYRange = 43;
	static int colorRandRange = 22;
	static float defradius = 25.2f;
	static float gravity = 0.16f;

#else

	static int createIdx = 8;
	static int Life = 50;
	static int Randrange = 11;
	static int posXZrange = 67;
	static int XZrange = 23;
	static int MinYRange = 37;
	static int MaxYRange = 43;
	static int ForwardRange_Min = 544;
	static int ForwardRange = 775;
	static int colorRandRange = 22;
	static float defradius = 25.2f;
	static float gravity = 0.16f;
#endif

	// カラーエディット
	static ImVec4 myColor = ImVec4(0.658f, 0.658f, 1.0, 0.87f); // RGBA

#if 0
	if (ImGui::TreeNode("Stone Water"))
	{
		ImGui::DragInt("CreateNum", &createIdx, 1);
		ImGui::DragFloat("Interval", &DEFAULT_SPLASHTIME, 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::DragInt("Interval_Randrange", &Randrange, 1);
		ImGui::DragInt("Life", &Life, 1);
		ImGui::DragInt("pos XZrange", &posXZrange, 1);
		ImGui::DragInt("XZrange", &XZrange, 1, 0);
		ImGui::DragInt("MinYRange", &MinYRange, 1);
		ImGui::DragInt("MaxYRange", &MaxYRange, 1, MinYRange);
		ImGui::DragInt("ForwardRange_Min", &ForwardRange_Min, 1, 0);
		ImGui::DragInt("ForwardRange_Max", &ForwardRange, 1, ForwardRange_Min +1);
		ImGui::DragInt("colorRandRange", &colorRandRange, 1);
		ImGui::DragFloat("defradius", &defradius, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("gravity", &gravity, 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::ColorEdit4("Color", &myColor.x);

		ImGui::TreePop();
	}
#endif

//#ifndef _DEBUG
	// スプラッシュ！
	if (m_fIntervalSplash <= m_fSplashTimer)
	{

		MyLib::Vector3 move;
		MyLib::Vector3 posOrigin = GetPosition();
		MyLib::Vector3 pos;

		for (int i = 0; i < createIdx; i++)
		{
			// 移動量
			float randmoveX = UtilFunc::Transformation::Random(0, XZrange) * 0.1f;
			float randmoveY = UtilFunc::Transformation::Random(MinYRange, MaxYRange) * 0.1f;
			float randRadius = UtilFunc::Transformation::Random(-30, 30) * 0.1f;
			

			float angle = 0.3f;
			angle += UtilFunc::Transformation::Random(-20, 20) * 0.001f;

			if (i >= createIdx / 2)
			{
				angle *= -1;
			}
			move.x = sinf(D3DX_PI * angle + m_StoneInfo.rot.y) * randmoveX;
			move.z = cosf(D3DX_PI * angle + m_StoneInfo.rot.y) * randmoveX;
			move.y = randmoveY;

			// 半径
			float radius = defradius + randRadius;

			// 生成位置のぶれ
			int xz = UtilFunc::Transformation::Random(0, posXZrange);
			if (i >= createIdx / 2)
			{
				xz *= -1;
			}
			
			// 生成位置
			MyLib::Vector3 randpos;
			randpos.x = sinf(D3DX_PI * 0.5f + m_StoneInfo.rot.y) * xz;
			randpos.z = cosf(D3DX_PI * 0.5f + m_StoneInfo.rot.y) * xz;

			float forwardRandPos = UtilFunc::Transformation::Random(ForwardRange_Min, ForwardRange) * 0.1f;
			randpos.x += sinf(D3DX_PI + m_StoneInfo.rot.y) * forwardRandPos;
			randpos.z += cosf(D3DX_PI + m_StoneInfo.rot.y) * forwardRandPos;
			pos = posOrigin + randpos;


			float colorrand = UtilFunc::Transformation::Random(-colorRandRange, colorRandRange) * 0.01f;
			CEffect3D* pEffect = CEffect3D::Create(
				pos,
				move,
				D3DXCOLOR(myColor.x + colorrand, myColor.y + colorrand, myColor.z, myColor.w),
				radius,
				Life,
				CEffect3D::MOVEEFFECT::MOVEEFFECT_ADD,
				CEffect3D::TYPE::TYPE_SMOKE);
			pEffect->SetEnableGravity();
			pEffect->SetGravityValue(gravity);
		}

		// インターバル更新
		m_fIntervalSplash = DEFAULT_SPLASHTIME + UtilFunc::Transformation::Random(-Randrange, Randrange) * 0.01f;

		// しぶきタイマー
		m_fSplashTimer = 0.0f;
	}
//#endif

	m_StoneInfo.pos = GetPosition();
}

//==========================================================================
// 描画処理
//==========================================================================
void CWaterStone::Draw()
{
	// 描画
	CObjectX::Draw();
}

//==========================================================================
// モデルの種類変更
//==========================================================================
void CWaterStone::ChangeModelType(int type)
{
	// Xファイルのデータ取得
	CXLoad* pXLoad = CXLoad::GetInstance();

	// Xファイルのロード
	int modelIdx = pXLoad->XLoad(m_vecModelFile[type]);
	BindXData(modelIdx);
}
