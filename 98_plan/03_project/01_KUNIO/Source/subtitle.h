//=============================================================================
// 
//  字幕ヘッダー [subtitle.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SUBTITLE_H_
#define _SUBTITLE_H_	// 二重インクルード防止

#include "object2d.h"

//==========================================================================
// クラス定義
//==========================================================================
// 字幕クラス
class CSubTitle : public CObject2D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_NONE = 0,		// なにもない
		STATE_FADEIN,		// フェードイン
		STATE_FADEOUT,		// フェードアウト
		STATE_MAX
	};

	CSubTitle(int nPriority = 7);
	~CSubTitle();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void BindSubtitle(const std::string& texfile);	// 字幕割り当て
	void SetSizeByHeight(const float height);		// 高さを基準にサイズ設定
	void SetState(State state);	// 状態設定

	//=============================
	// 静的関数
	//=============================
	static CSubTitle* Create(const MyLib::Vector3& pos, float life = 1.0f);	// 生成処理

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CSubTitle::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void UpdateState();		// 状態更新
	void StateNone();		// なし
	void StateFadeIn();		// フェードイン
	void StateFadeout();	// フェードアウト

	// その他関数

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	float m_fLifeTimer;		// 生存時間
	float m_fStateTime;		// 状態カウンター
	State m_state;			// 状態

};


#endif