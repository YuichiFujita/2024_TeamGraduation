//==========================================================================
// 
//  パッドUI処理 [padUI.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
//==========================================================================
// インクルードファイル
//==========================================================================
#include "padUI.h"
#include "manager.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\entry\\playerMarker003.png";	// テクスチャ

	// 矢印情報
	namespace arrow
	{
		const float				SIZE = 25.0f;	// 大きさ
		const MyLib::Vector3	SPACE = MyLib::Vector3(70.0f, 10.0f, 0.0f);	// 空白
	}
}

//==========================================================================
//	コンストラクタ
//==========================================================================
CPadUI::CPadUI(int nPriority) : CObject2D_Anim(nPriority)
{

}

//==========================================================================
//	デストラクタ
//==========================================================================
CPadUI::~CPadUI()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CPadUI* CPadUI::Create(
	const MyLib::Vector3& pos,	// 位置
	const float width,			// 横幅
	const int pattern,			// パターン数
	int nPriority				// 優先順位
)
{
	// メモリの確保
	CPadUI* pObj = DEBUG_NEW CPadUI(nPriority);

	if (pObj != nullptr)
	{
		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合
			SAFE_UNINIT(pObj);
			return nullptr;
		}

		// 引数情報
		pObj->SetPosition(pos);			// 位置
		pObj->SetOriginPosition(pos);	// 原点位置
		pObj->SetPatternAnim(pattern);	// パターン数
		pObj->SetSizeByWidth(width);	// サイズ
	}

	return pObj;
}

//==========================================================================
//	初期化処理
//==========================================================================
HRESULT CPadUI::Init()
{
	// UV分割数設定
	SetDivisionUV(mylib_const::MAX_PLAYER, 2);

	//	初期化処理
	CObject2D_Anim::Init();

	// 種類をオブジェクト2Dにする
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist(TEXTURE);
	BindTexture(nTexID);

	// 自動再生をOFFにする
	SetEnableAutoPlay(false);

	// 頂点情報設定
	SetVtx();

	// 矢印UIの生成
	if (FAILED(CreateArrowUI()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 矢印UIの生成
//==========================================================================
HRESULT CPadUI::CreateArrowUI()
{
	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
	{
		// 矢印の生成
		m_apArrow[i] = CArrowUI::Create
		( // 引数
			(CArrowUI::EDirection)i,	// 方向
			pos,						// 位置
			arrow::SIZE,				// サイズ
			MyLib::color::White(1.0f),	// 色
			GetPriority()				// 優先順位
		);

		if (m_apArrow[i] == nullptr)
		{ // 生成に失敗した場合
			return E_FAIL;
		}

		// オフセット位置設定
		m_apArrow[i]->SetOffset(arrow::SPACE);
		m_apArrow[i]->SetOffsetOrigin(m_apArrow[i]->GetOffset());
	}

	return S_OK;
}

//==========================================================================
//	終了処理
//==========================================================================
void CPadUI::Uninit()
{
	// オブジェクトを破棄
	Release();
}

//==========================================================================
//	削除処理
//==========================================================================
void CPadUI::Kill()
{
	for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
	{
		// 矢印の終了
		SAFE_KILL(m_apArrow[i]);
	}

	// 自身の終了
	CPadUI::Uninit();
}

//==========================================================================
//	更新処理
//==========================================================================
void CPadUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親の更新
	CObject2D_Anim::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 決定
//==========================================================================
void CPadUI::Decide()
{
	// 決定のパターンに変更
	SetPatternAnim(GetPatternAnim() + GetDivisionU());
}

//==========================================================================
// キャンセル
//==========================================================================
void CPadUI::Cancel()
{
	// キャンセルのパターンに変更
	SetPatternAnim(GetPatternAnim() - GetDivisionU());
}

//==========================================================================
// 横幅からサイズ設定
//==========================================================================
void CPadUI::SetSizeByWidth(const float width)
{
	// 画像サイズ取得
	CTexture* pTexture = CTexture::GetInstance();
	MyLib::Vector2 size = pTexture->GetImageSize(GetIdxTexture());

	// 分割数で割る
	size.x *= (1.0f / (float)GetDivisionU());
	size.y *= (1.0f / (float)GetDivisionV());

	// 縦幅を元にサイズ計算
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, width);
	SetSize(size);
}

//==========================================================================
// アクション設定
//==========================================================================
void CPadUI::SetAction(CArrowUI::EDirection dir)
{
	// 選択時移動
	m_apArrow[dir]->SetState(CArrowUI::EState::STATE_SELECTMOVE);
}

//==========================================================================
// 矢印の描画設定
//==========================================================================
void CPadUI::SetEnableDispArrow(bool bDisp)
{
	for (const auto& arrow : m_apArrow)
	{
		if (arrow == nullptr) continue;
		arrow->SetEnableDisp(bDisp);
	}
}

//==========================================================================
// 位置設定
//==========================================================================
void CPadUI::SetPosition(const MyLib::Vector3& pos)
{
	CObject::SetPosition(pos);

	// 矢印の位置も同期
	for (const auto& arrow : m_apArrow)
	{
		arrow->SetPosition(pos);
		arrow->SetOriginPosition(pos);
	}
}

//========================================================================================
// 描画状況設定
//========================================================================================
void CPadUI::SetEnableDisp(bool bDisp)
{
	// 描画状況設定
	CObject::SetEnableDisp(bDisp);

	// 描画状況設定
	for (const auto& arrow : m_apArrow)
	{
		arrow->SetEnableDisp(bDisp);
	}
}
