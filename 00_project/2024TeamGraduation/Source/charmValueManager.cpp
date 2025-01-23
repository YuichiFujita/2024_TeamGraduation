//=============================================================================
// 
// モテ値マネージャ処理 [charmValueManager.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "charmValueManager.h"
#include "manager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float ADDVALUE[CCharmValueManager::ETypeAdd::ADD_MAX] =	// 加算量
	{
		1.0f,	// ヒット
		0.6f,	// キャッチ
		2.0f,	// ジャストキャッチ
		3.0f,	// カバーキャッチ
		1.0f,	// ジャストキャッチ直後の投げ
		1.0f,	// ドロップキャッチ直後の投げ
		4.0f,	// 回避
		10.0f,	// スペシャル
		10.0f,	// スペシャルをキャッチした
		3.0f,	// 蘇生
	};

	const float SUBVALUE[CCharmValueManager::ETypeSub::SUB_MAX] =	// 減算量
	{
		1.0f,	// ライン越えて戻ってる最中にあたる
		2.0f,	// 端に逃げまくる
		3.0f,	// 走っていってライン越え(ボール所持)
		4.0f,	// ずっとボール持って投げない
		10.0f,	// スペシャルをキャッチされた
	};
}

//==========================================================================
// スタティックアサート
//==========================================================================
static_assert(NUM_ARRAY(ADDVALUE) == CCharmValueManager::ETypeAdd::ADD_MAX, "ERROR : Type Count Mismatch");
static_assert(NUM_ARRAY(SUBVALUE) == CCharmValueManager::ETypeSub::SUB_MAX, "ERROR : Type Count Mismatch");

//==========================================================================
// 静的メンバ変数
//==========================================================================
CCharmValueManager* CCharmValueManager::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CCharmValueManager::CCharmValueManager()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CCharmValueManager::~CCharmValueManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCharmValueManager* CCharmValueManager::Create()
{
	if (m_pThisPtr != nullptr)
	{// 既にある場合
		return m_pThisPtr;
	}

	// メモリの確保
	m_pThisPtr = DEBUG_NEW CCharmValueManager;

	if (m_pThisPtr != nullptr)
	{
		// 初期化処理
		m_pThisPtr->Init();
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCharmValueManager::Init()
{
	for (int i = 0; i < CCharmValueManager::ETypeAdd::ADD_MAX; i++)
	{
		m_fAddValue[i] = ADDVALUE[i];
	}
	
	for (int i = 0; i < CCharmValueManager::ETypeSub::SUB_MAX; i++)
	{
		m_fSubValue[i] = SUBVALUE[i];
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCharmValueManager::Uninit()
{
	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// 加算量取得
//==========================================================================
float CCharmValueManager::GetAddValue(ETypeAdd type)
{
	// 範囲外
	if (ETypeAdd::ADD_MAX <= type) return 0.0f;

	return m_fAddValue[type];
}

//==========================================================================
// 加算量取得
//==========================================================================
float CCharmValueManager::GetSubValue(ETypeSub type)
{
	// 範囲外
	if (ETypeSub::SUB_MAX <= type) return 0.0f;

	return m_fSubValue[type];
}

//==========================================================================
// デバッグ
//==========================================================================
void CCharmValueManager::Debug()
{
	if (ImGui::TreeNode("CCharmValueManager"))
	{// モテ値

		if (ImGui::TreeNode("Add"))
		{// 加算

			for (int i = 0; i < ETypeAdd::ADD_MAX; i++)
			{
				ETypeAdd type = static_cast<ETypeAdd>(i);
				std::string label = magic_enum::enum_name(type).data();
				ImGui::DragFloat(label.c_str(), &m_fAddValue[i], 0.01f, 0.0f, 100.0f, "%f");
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Sub"))
		{// 減算

			for (int i = 0; i < ETypeSub::SUB_MAX; i++)
			{
				ETypeSub type = static_cast<ETypeSub>(i);
				std::string label = magic_enum::enum_name(type).data();
				ImGui::DragFloat(label.c_str(), &m_fAddValue[i], 0.01f, 0.0f, 100.0f, "%.2f");
			}

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}
