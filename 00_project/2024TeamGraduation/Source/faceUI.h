//============================================================
//
//	プレイヤー顔UIヘッダー [faceUI.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FACE_UI_H_
#define _FACE_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "player.h"

//************************************************************
// 前方宣言
//************************************************************
class CObject2D;		// オブジェクト2Dクラス
class CObject2D_Anim;	// オブジェクト2Dアニメクラス

//************************************************************
//	クラス定義
//************************************************************
// プレイヤー顔UIクラス
class CFaceUI : public CObject
{
public:
	// コンストラクタ
	CFaceUI(const CPlayer* pPlayer);

	// デストラクタ
	~CFaceUI() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw() override;		// 描画
	void SetEnableDisp(const bool bDraw) override;			// 描画状況設定
	void SetPosition(const MyLib::Vector3& pos) override;	// 位置設定
	void BindDressUp(int nHair, int nAccessory, int nFace);	// 着せ替え割当
	void SetMyPlayerIdx(int nIdx);							// 自分のインデックス設定

	// 静的メンバ関数
	static CFaceUI* Create(const CPlayer* pPlayer, const MyLib::Vector3& rPos);

private:
	// メンバ関数
	HRESULT CreateUI();				// UI生成
	HRESULT CreatePlayerIdxUI();	// プレイヤーインデックスUI生成
	HRESULT CreatePlayerUI();		// プレイヤーUI生成
	HRESULT CreatePlayer();			// プレイヤー生成
	HRESULT CreateRenderTexture();	// レンダーテクスチャ生成
	void CreateTexture();			// テクスチャ作成
	void SetPositionRelative();		// 相対位置設定

	// メンバ変数
	const CPlayer* m_pPlayerParent;	// 親プレイヤー
	CRenderTexture* m_pRenderScene;	// シーンレンダーテクスチャ
	CObject2D_Anim* m_pPlayerIdxUI;	// プレイヤーインデックスUI情報
	CObject2D* m_pPlayerUI;			// プレイヤーUI情報
	CPlayer* m_pPlayer;				// プレイヤー
};

#endif	// _FACE_UI_H_
