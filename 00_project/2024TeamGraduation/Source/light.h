//============================================================
//
//	ライトヘッダー [light.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _LIGHT_H_
#define _LIGHT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// ライトクラス
class CLight : public CObject
{
public:
	// コンストラクタ
	CLight();

	// デストラクタ
	~CLight() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw() override;		// 描画

	// メンバ関数
	void SetLightType(const D3DLIGHTTYPE type);		// 種類設定
	void SetPosition(const MyLib::Vector3& rPos);	// 位置設定
	void SetDirection(const MyLib::Vector3& rDir);	// 方向設定
	void SetRange(const float fRange);				// 光源範囲設定
	void SetDiffuse(const MyLib::Color& rCol);		// 拡散光設定
	void SetEnableLight(const bool bLight);			// 有効設定
	inline D3DLIGHT9 GetLight() { return m_light; }	// ライト取得

private:
	// 静的メンバ変数
	static int m_nNumLight;	// ライトの総数

	// メンバ変数
	D3DLIGHT9 m_light;		// ライト情報
	const int m_nLightIdx;	// 自身のライトインデックス
};

#endif	// _LIGHT_H_
