//=============================================================================
// 
//  巻き物ヘッダー [scroll.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SCROLL_H_
#define _SCROLL_H_	// 二重インクルード防止

#include "object2d.h"

//==========================================================================
// クラス定義
//==========================================================================
// 巻き物クラス
class CScroll : public CObject2D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum STATE
	{
		STATE_NONE = 0,	// なにもない
		STATE_OPEN,		// オープン
		STATE_WAIT,		// 待機
		STATE_WAITPRESS,		// 押下待機
		STATE_CLOSE,	// クローズ
		STATE_FADEOUT,	// フェードアウト
		STATE_MAX
	};

	CScroll(int nPriority = 7);
	~CScroll();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();						// 削除
	void SetState(const STATE& state);		// 状態設定
	STATE GetState() { return m_state; }	// 状態取得

	//=============================
	// 静的関数
	//=============================
	static CScroll* Create(const MyLib::Vector3& pos, const float toOpenTime, const float height, const float scrollLength, bool bAutoWaitPress = true, bool bFadeOut = false, int nPriority = 7);	// 生成処理

private:
	
	//=============================
	// 関数リスト
	//=============================
	// 状態リスト
	typedef void(CScroll::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void UpdateState();	// 状態更新
	void StateNone();	// なにもなし
	void StateOpen();	// オープン
	void StateWait();	// 待機
	void StateWaitPress();	// 押下待機
	void StateClose();	// クローズ
	void StateFadeout();// フェードアウト

	// その他
	void ResetFlag();	// フラグリセット
	void CreatePaper();	// 紙部分生成
	void CreateEdge();	// 端部分生成
	void AdjustRoll();	// ロール部分の調整
	void AdjustEdge();	// 端部分の調整
	void OpenSkip();	// オープンスキップ

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	STATE m_state;			// 状態
	float m_fStateTimer;	// 状態タイマー
	float m_fToOpenTimer;	// オープンまでの時間
	float m_fScrollLength;	// 巻き物の長さ
	bool m_bFinishOpen;		// オープン終了判定
	bool m_bFadeOut;		// フェードアウト判定
	bool m_bAutoWaitPress;	// 自動押下待機判定

	// その他
	CObject2D* m_pPapaer;	// 紙部分
	CObject2D* m_pEdge;		// 端部分
};


#endif