//=============================================================================
// 
//  ゴールフラグ処理 [goalflag.cpp]
//  Author : 日野澤匠泉のプログラム(お借りしてません)
// 
//=============================================================================
#include "checkpoint.h"
#include "manager.h"
#include "calculation.h"
#include "player.h"
#include "game.h"
#include "spline.h"
#include "course.h"
#include "timer.h"
#include "splashwater_manager.h"
#include "gamemanager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\checkpoint\\flag.x";	// モデルパス
	const std::string TEX_EFFECT = "data\\TEXTURE\\effect\\rolling2.png";	// エフェクトパス
	const ImVec4 WATERCOLOR = ImVec4(0.658f, 0.658f, 1.0, 0.87f); // RGBA
}

namespace StateTime
{
	const float SWING = 2.0f;	// ゆらゆら
	const float ROTATE = 0.6f;	// 回転
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CCheckpoint::STATE_FUNC CCheckpoint::m_StateFunc[] =
{
	&CCheckpoint::StateSwing,	// ゆらゆら
	&CCheckpoint::StateRotate,	// 回転
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CCheckpoint> CCheckpoint::m_List = {};	// リスト
int CCheckpoint::m_nAll = 0;
int CCheckpoint::m_nSaveID = 0;

//==========================================================================
// コンストラクタ
//==========================================================================
CCheckpoint::CCheckpoint(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_fLength = 0.0f;
	m_bIsPassed = false;
	m_state = State::SWING;	// 状態
	m_fStateTime = 0.0f;	// 状態カウンター
	m_fPassedTime = 0.0f;
	m_pEffect = nullptr;		// エフェクトのポインタ
	m_pEffekseerObj = nullptr;	// エフェクシアのオブジェクト
	m_MyIndex = 0;
}

//==========================================================================
// デストラクタ
//==========================================================================
CCheckpoint::~CCheckpoint()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCheckpoint* CCheckpoint::Create(const float length)
{
	// メモリの確保
	CCheckpoint* pObj = DEBUG_NEW CCheckpoint;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();

		// 距離設定
		pObj->SetLength(length);

		// 総数加算
		m_nAll++;
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCheckpoint::Init()
{
	// リストに追加
	m_List.Regist(this);

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// ID設定
	m_MyIndex = m_nAll;

	// 初期化処理
	HRESULT hr = CObjectX::Init(MODEL);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// ゆらゆらの向き設定
	m_DestRot.x = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
	m_DestRot.y = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
	m_DestRot.z = UtilFunc::Transformation::Random(40, 80) * 0.001f;

	return S_OK;
}

//==========================================================================
// エフェクト生成
//==========================================================================
void CCheckpoint::CreateEffect()
{
	// エフェクト生成
	if (m_pEffect == nullptr)
	{
		m_pEffect = CObjectBillboard::Create(GetPosition(), 0.0f);
	}
	m_pEffect->SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	if (pTexture == nullptr) return;

	int texIdx = pTexture->Regist(TEX_EFFECT);
	m_pEffect->BindTexture(texIdx);

	// サイズ設定
	D3DXVECTOR2 size = UtilFunc::Transformation::AdjustSizeByWidth(pTexture->GetImageSize(texIdx), 400.0f);
	m_pEffect->SetSize(size);

	m_pEffect->SetAlpha(0.7f);


	// 水エフェクト生成
	CreateWaterEffect(8);
}

//==========================================================================
// 終了処理
//==========================================================================
void CCheckpoint::Uninit()
{
	// 総数減らす
	if (m_nAll > 0)
	{
		m_nAll--;
	}

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CCheckpoint::Kill()
{
	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CCheckpoint::Update()
{
	// 状態タイマー加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 状態別更新
	(this->*(m_StateFunc[m_state]))();


	// 通過済みなら処理しない
	if (m_bIsPassed) return;

	// リストループ
	CListManager<CPlayer> PlayerList = CPlayer::GetListObj();
	CPlayer* pPlayer = PlayerList.GetData(0);
	float playerlen = pPlayer->GetPosition().x;

	if (playerlen >= GetPosition().x)
	{// チェックポイント通過したら

		if (m_nSaveID < m_MyIndex)
		{
			// ID保存
			m_nSaveID = m_MyIndex;

			// 通過した時間を保存
			m_fPassedTime = CTimer::GetInstance()->GetTime();
			m_bIsPassed = true;

			// 通過した時のポイント設定
			{
				CGameManager* pMgr = CGame::GetInstance()->GetGameManager();

				if (pMgr != nullptr)
				{
					pMgr->SaveEvaluationPoint();
				}
			}

			// 状態設定
			m_state = State::ROTATE;
			m_fStateTime = 0.0f;

			// SE再生
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_KARAKURI);

			// エフェクト作成
			CreateEffect();
		}
	}
}

//==========================================================================
// ゆらゆら
//==========================================================================
void CCheckpoint::StateSwing()
{
	MyLib::Vector3 rot = GetRotation();

	// サインカーブ補間
	float ratio = UtilFunc::Correction::EasingEaseInOutSine(0.0f, 1.0f, 0.0f, StateTime::SWING, m_fStateTime);
	rot = m_DestRot_Old + (m_DestRot - m_DestRot_Old) * ratio;

	if (m_bIsPassed)
	{// 到着済みは半回転
		rot.z += D3DX_PI;
	}
	SetRotation(rot);

	if (m_fStateTime >= StateTime::SWING)
	{
		m_fStateTime = 0.0f;

		// 過去の向き保存
		m_DestRot_Old = m_DestRot;

		// 目標の向き設定
		m_DestRot.x = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
		m_DestRot.y = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
		m_DestRot.z *= -1;
	}
}

//==========================================================================
// 回転
//==========================================================================
void CCheckpoint::StateRotate()
{
	if (m_fStateTime <= StateTime::ROTATE * 0.5f)
	{
		// 水エフェクト生成
		CreateWaterEffect(4);
	}
	else if (m_pEffekseerObj == nullptr)
	{
		// エフェクト
		m_pEffekseerObj = CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_WATERJUMP,
			GetPosition() + MyLib::Vector3(300.0f, 0.0f, 0.0f), 0.0f, 0.0f, 60.0f, true);

		// しぶき生成
		CSplashwater_Manager::Create();
	}


	// 回転
	MyLib::Vector3 rot = GetRotation();
	rot.z = UtilFunc::Correction::EasingEaseIn(0.0f, -D3DX_PI, 0.0f, StateTime::ROTATE, m_fStateTime);
	SetRotation(rot);

	// エフェクト回転
	MyLib::Vector3 effectRot = m_pEffect->GetRotation();
	effectRot.z += UtilFunc::Correction::EasingLinear(0.0f, D3DX_PI * 0.5f, 0.0f, StateTime::ROTATE, m_fStateTime);
	m_pEffect->SetRotation(effectRot);

	// エフェクト不透明度
	float alpha = UtilFunc::Correction::EasingEaseOut(0.7f, 0.0f, 0.0f, StateTime::ROTATE, m_fStateTime);
	m_pEffect->SetAlpha(alpha);

	// 時間経過
	if (m_fStateTime >= StateTime::ROTATE)
	{
		m_state = State::SWING;
		m_fStateTime = 0.0f;

		// ゆらゆらの向き設定
		m_DestRot.x = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
		m_DestRot.y = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
		m_DestRot.z = UtilFunc::Transformation::Random(40, 80) * 0.001f;

		if (m_pEffect != nullptr)
		{
			m_pEffect->Uninit();
			m_pEffect = nullptr;
		}
	}
}

//==========================================================================
// 水エフェクト生成
//==========================================================================
void CCheckpoint::CreateWaterEffect(int max)
{
	for (int i = 0; i < max; i++)
	{
		// 移動量
		MyLib::Vector3 setmove;
		float randmove = UtilFunc::Transformation::Random(30, 50) * 0.1f;
		float randmoveY = UtilFunc::Transformation::Random(130, 170) * 0.1f;

		float randAngle = UtilFunc::Transformation::Random(-20, 20) * 0.01f;
		setmove.x = sinf(D3DX_PI * 0.5f + randAngle) * randmove;
		setmove.z = cosf(D3DX_PI * 0.5f + randAngle) * randmove;
		setmove.y = randmoveY;

		// 色
		float colorrand = UtilFunc::Transformation::Random(-22, 22) * 0.01f;

		// 半径
		float randRadius = UtilFunc::Transformation::Random(-100, 100) * 0.1f;
		float radius = 25.0f + randRadius;

		// 生成
		MyLib::Vector3 distance(
			-250.0f + UtilFunc::Transformation::Random(-200, 200) * 0.1f,
			0.0f,
			UtilFunc::Transformation::Random(-100, 100) * 0.1f);

		CEffect3D* pEffect = CEffect3D::Create(
			GetPosition() + distance,
			setmove,
			D3DXCOLOR(WATERCOLOR.x + colorrand, WATERCOLOR.y + colorrand, WATERCOLOR.z, WATERCOLOR.w),
			radius,
			50,
			CEffect3D::MOVEEFFECT::MOVEEFFECT_ADD,
			CEffect3D::TYPE::TYPE_SMOKE);
		pEffect->SetEnableGravity();
		pEffect->SetGravityValue(0.4f);
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CCheckpoint::Draw()
{
	// ボックスコライダーの描画
	if (!CManager::IsDisp_CheckPoint()) return;

	// 描画
	CObjectX::Draw();
}

//==========================================================================
// 距離設定
//==========================================================================
void CCheckpoint::SetLength(const float length)
{
	// 距離を設定
	m_fLength = length;

	// 座標を設定
	MyLib::Vector3 pos = MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), m_fLength);
	SetPosition(pos);
}

//==========================================================================
// チェックポイントファイル読み込み
//==========================================================================
void CCheckpoint::Load(const std::string filename)
{
	// ファイルを開く
	std::ifstream File(filename);
	if (!File.is_open()) {
		return;
	}

	float length = 0.0f;

	// コメント用
	std::string hoge;

	// データ読み込み
	std::string line;
	while (std::getline(File, line))
	{
		// コメントはスキップ
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("SET_LENGTH") != std::string::npos)
		{// TYPEで配置物の種類

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>	// ＝
				length;	// 配置物の種類

			CCheckpoint::Create(length);

			continue;
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// ファイルを閉じる
	File.close();
}

//==========================================================================
// チェックポイント通過情報リセット
//==========================================================================
void CCheckpoint::ResetSaveID()
{
	m_nSaveID = -1;

	// リストループ
	std::list<CCheckpoint*>::iterator itr = m_List.GetEnd();
	CCheckpoint* pObj = nullptr;

	while (m_List.ListLoop(itr))
	{
		pObj = (*itr);
		pObj->m_bIsPassed = false;
		pObj->m_fPassedTime = 0.0f;
		pObj->SetRotation(0.0f);
		// pObjか(*itr)を使って処理
	}
}